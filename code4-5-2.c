/*
 * SensorController.c
 *
 *  Created on: Oct 24, 2022
 *      Author: kadh1
 * 		modified by: Henry Zou
 */


#include <stdio.h>

#include "main.h"
#include "User/L2/Comm_Datalink.h"
#include "User/L3/AcousticSensor.h"
#include "User/L3/DepthSensor.h"
#include "User/L4/SensorPlatform.h"
#include "User/L4/SensorController.h"
#include "User/util.h"

//Required FreeRTOS header files
#include "FreeRTOS.h"
#include "Timers.h"
#include "semphr.h"

QueueHandle_t Queue_Sensor_Data;
QueueHandle_t Queue_HostPC_Data;
TimerHandle_t xTimers[2];

//debug
char main_string[50];
uint32_t main_counter = 0;

static void ResetMessageStruct(struct CommMessage* currentRxMessage){

	static const struct CommMessage EmptyMessage = {0};
	*currentRxMessage = EmptyMessage;
}

/******************************************************************************
This task is created from the main.
******************************************************************************/
void SensorControllerTask(void *params) {
    char Msg[50];
    enum HostPCCommands HostPCCommand = PC_Command_NONE;
    struct CommMessage currentRxMessage = {0};
    struct SensorStates sensorState = {0};
    bool IsRemotePlatformEnabled = false; //sensor disabled



    do {
        //Check if there is any received Sensor Data
//        if (xQueueReceive(Queue_Sensor_Data, &currentRxMessage, 0) == pdPASS) {
//            switch (currentRxMessage.SensorID) {
//                case Controller:
//                    switch (currentRxMessage.messageId) {
//                        case 0: // Do Nothing
//                            break;
//                        case 1:
//                            // Received Acknowledgment
//                            sensorState.IsAcousticAck = false;
//                            sensorState.IsDepthAck = false;
//                            break;
//                        case 3:
//                        	// Do Nothing
//                            break;
//                    }
//                    break;
//                case Acoustic:
//                    switch (currentRxMessage.messageId) {
//                        case 0: // Do Nothing
//                            break;
//                        case 1: // Received Acknowledgment
//                            sensorState.IsAcousticAck = true;
//                            break;
//                        case 3: // Received Acoustic Data
//                            sensorState.AcousticData = currentRxMessage.params;
//                            break;
//                    }
//                    break;
//
//                    case Depth:
//
//						switch (currentRxMessage.messageId) {
//
//						 case 0: // Do Nothing
//
//							 break;
//
//						 case 1: // Received Acknowledgment
//
//							 sensorState.IsDepthAck = true;
//
//							 break;
//
//						 case 3:
//
//							 // Received Depth Data
//
//							 sensorState.DepthData = currentRxMessage.params;
//
//							 break;
//
//						}
//                default: // Should not get here
//                    ResetMessageStruct(&currentRxMessage);
//            }
//        }

    	//debug
		//print_str("Controller executing\r\n");
		//sprintf(main_string,"Task iteration: 0x%08lx\r\n", main_counter++);
		//print_str(main_string);

        if (IsRemotePlatformEnabled == true) {
            if (sensorState.IsAcousticAck == true) {
                sprintf(Msg, "Acoustic Data: %d mV | ", sensorState.AcousticData);
                print_str(Msg);
            } else {
                sprintf(Msg, "Acoustic Data: NACK mV | ");
                print_str(Msg);
            }
        }

		// Check if there is any received Host PC Command Data
		if (xQueueReceive(Queue_HostPC_Data, &HostPCCommand, 0) == pdPASS) {//if data received
			switch (HostPCCommand) {
				case PC_Command_START:
					print_str("Sending Start Command!\r\n");
					send_sensorEnable_message(Acoustic, 5000);
					send_sensorEnable_message(Depth, 5000);
					IsRemotePlatformEnabled = true;
					break;
				case PC_Command_RESET:
					print_str("Sending Reset Command\r\n");
					send_sensorReset_message();
					IsRemotePlatformEnabled = false;
					break;
				case 0:
					break;
				default:
					break;
			}
		}

		vTaskDelay(1000 / portTICK_RATE_MS);
	} while (1);
}




/*
 * This task reads the queue of characters from the Sensor Platform when available
 * It then sends the processed data to the Sensor Controller Task
 */
void SensorPlatform_RX_Task(){
	struct CommMessage currentRxMessage = {0};
	Queue_Sensor_Data = xQueueCreate(80, sizeof(struct CommMessage));

	request_sensor_read();  // requests a usart read (through the callback)

	while(1){
		parse_sensor_message(&currentRxMessage);

		if(currentRxMessage.IsMessageReady == true && currentRxMessage.IsCheckSumValid == true){

			xQueueSendToBack(Queue_Sensor_Data, &currentRxMessage, 0);
			ResetMessageStruct(&currentRxMessage);
		}
	}
}



/*
 * This task reads the queue of characters from the Host PC when available
 * It then sends the processed data to the Sensor Controller Task
 */
void HostPC_RX_Task(){

	enum HostPCCommands HostPCCommand = PC_Command_NONE;

    //debug
    print_str("PC receiving\r\n");
    sprintf(main_string,"Task iteration: 0x%08lx\r\n",main_counter++);
    print_str(main_string);

	Queue_HostPC_Data = xQueueCreate(80, sizeof(enum HostPCCommands));

	request_hostPC_read();

	while(1){
		HostPCCommand = parse_hostPC_message();

		if (HostPCCommand != PC_Command_NONE){
			xQueueSendToBack(Queue_HostPC_Data, &HostPCCommand, 0);
		}

	}
}


/*
 * main_user.c
 *
 *  Created on: Aug 8, 2022
 *      Author: Andre Hendricks
 */

#include <stdio.h>

//STM32 generated header files
#include "main.h"

//User generated header files
#include "User/main_user.h"
#include "User/util.h"
#include "User/L1/USART_Driver.h"
#include "User/L2/Comm_Datalink.h"
#include "User/L4/SensorPlatform.h"
#include "User/L4/SensorController.h"

//Required FreeRTOS header files
#include "FreeRTOS.h"
#include "task.h"

#define SENSORPLATFORM_MODE 0
#define SENSORCONTROLLER_MODE 1

#define CODE_MODE SENSORCONTROLLER_MODE//SENSORPLATFORM_MODE

//debug
extern UART_HandleTypeDef huart2;


void main_user(){
	uint8_t tx_buffer[27] = "welcome!\n\r";

	util_init();

	initialize_sensor_datalink();

#if CODE_MODE == SENSORCONTROLLER_MODE
	initialize_hostPC_datalink();
#endif

#if CODE_MODE == SENSORCONTROLLER_MODE
	//HAL_UART_Transmit(&huart2, tx_buffer, 27, 10); //debug
	//HAL_Delay(1000); //debug

	xTaskCreate(SensorControllerTask,"Sensor_Controller_Task", configMINIMAL_STACK_SIZE + 100, NULL, tskIDLE_PRIORITY + 2, NULL);

	xTaskCreate(HostPC_RX_Task,"HostPC_RX_Task", configMINIMAL_STACK_SIZE + 100, NULL, tskIDLE_PRIORITY + 3, NULL);

	//xTaskCreate(SensorPlatform_RX_Task,"SensorPlatform_RX_Task", configMINIMAL_STACK_SIZE + 100, NULL, tskIDLE_PRIORITY + 2, NULL);


#elif CODE_MODE == SENSORPLATFORM_MODE
	xTaskCreate(SensorPlatformTask,"Sensor_Platform_Task", configMINIMAL_STACK_SIZE + 100, NULL, tskIDLE_PRIORITY + 2, NULL);
#endif
	vTaskStartScheduler();

	while(1);

}


/*
 * UART_Driver.c
 *
 *  Created on: Oct. 21, 2022
 *      Author: Andre Hendricks / Dr. JF Bousquet
 */


#include <string.h>

#include "main.h"
#include "User/L1/USART_Driver.h"


//Required FreeRTOS header files
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#define MAX_RX_BUFFER_LENGTH   40

uint8_t rx_buffer_extern[MAX_RX_BUFFER_LENGTH];
uint8_t rx_buffer_hostPC[MAX_RX_BUFFER_LENGTH];

QueueHandle_t Queue_extern_UART;
QueueHandle_t Queue_hostPC_UART;

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart6;
static SemaphoreHandle_t mutexHandle_printStr_extern;
static void printStr_local_extern(char * str);

/******************************************************************************
This triggers a read and the buffer will be filled asynchronously.
******************************************************************************/
void request_sensor_read(void)
{
	HAL_UART_Receive_IT (&huart6, rx_buffer_extern, 1);
}

/******************************************************************************
This triggers a read and the buffer will be filled asynchronously.
******************************************************************************/
void request_hostPC_read(void)
{
	HAL_UART_Receive_IT (&huart2, rx_buffer_hostPC, 1);
}



/******************************************************************************
Configures the external USART.
******************************************************************************/
void configure_usart_extern(void)
{
	//Start interrupt for extern UART
	request_sensor_read();

	// a queue will be filled by the external UART
	Queue_extern_UART = xQueueCreate(80, sizeof(uint8_t));

	mutexHandle_printStr_extern = xSemaphoreCreateMutex();
}

/******************************************************************************
Configures the external USART.
******************************************************************************/
void configure_usart_hostPC(void)
{
	//Start interrupt for Host PC UART, also read char from putty
	request_hostPC_read();

	// a queue will be filled by the Host PC UART
	Queue_hostPC_UART = xQueueCreate(80, sizeof(uint8_t));
}


/******************************************************************************
******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	BaseType_t xStatus;

	//Toggle onboard LED to indicate interrupt
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

	if(huart == &huart2){//Handle Host PC RX UART


		//Request UART Interrupt Rx
		request_hostPC_read();

		// fill the queue one character at a time
		xStatus = xQueueSendToBackFromISR(Queue_hostPC_UART, rx_buffer_hostPC, 0);

		//debug
		HAL_UART_Transmit(&huart2, rx_buffer_hostPC, 6, 10);
		//printStr_extern(rx_buffer_hostPC);
		//end of debug
	}
	else if(huart == &huart6){//Handle extern RX UART
		// fill the queue one character at a time***
		//xStatus = xQueueSendToBackFromISR(Queue_extern_UART, rx_buffer_extern, 0);

		//Request UART Interrupt Rx
		//request_sensor_read();
	}

	if(xStatus == pdPASS){
		//Toggle onboard LED
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	}



}


void printStr_extern(char * str){
	xSemaphoreTake(mutexHandle_printStr_extern, portMAX_DELAY);
	printStr_local_extern(str);
	xSemaphoreGive(mutexHandle_printStr_extern);
}

static void printStr_local_extern(char * str){
	HAL_UART_Transmit(&huart6,(uint8_t*) str, strlen(str), HAL_MAX_DELAY);//
}


/*
 * Comm_Datalink.c
 *
 *  Created on: Oct. 21, 2022
 *      Author: Andre Hendricks / Dr. JF Bousquet
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "User/L1/USART_Driver.h"
#include "User/L2/Comm_Datalink.h"
#include "User/util.h"

enum ParseMessageState_t {Waiting_S, SensorID_S, MessageID_S, ParamsID_S, Star_S, CS_S};

static void sendStringSensor(char* tx_string);

/******************************************************************************
******************************************************************************/
void initialize_sensor_datalink(void)
{
	configure_usart_extern();
}

void initialize_hostPC_datalink(void){
	configure_usart_hostPC();
}

/******************************************************************************
this function calculates the checksum and sends a given input string to the uart.
******************************************************************************/
static void sendStringSensor(char* tx_string)
{
	uint8_t checksum;
	uint16_t str_length;

	// Check the length of the command
	str_length = strlen((char *)tx_string)-1;

	// Compute the checksum
	checksum = tx_string[0];
	for (int idx = 1; idx < str_length-2; idx++) {
		checksum ^= tx_string[idx];
	}

	sprintf(&tx_string[str_length-2], "%02x\n", checksum);
	printStr_extern(tx_string);
}


/******************************************************************************
This function parses the queue to identify received messages.
******************************************************************************/
void parse_sensor_message(struct CommMessage* currentRxMessage)
{
	static enum ParseMessageState_t currentState = Waiting_S;
	uint8_t CurrentChar;
	static uint16_t sensorIdIdx = 0, MessageIdIdx = 0, ParamIdx = 0, checksumIdx = 0;
	static char sensorId[6],CSStr[3];
	static uint8_t checksum_val;
	static const struct CommMessage EmptyMessage = {0};

	while(xQueueReceive(Queue_extern_UART, &CurrentChar, portMAX_DELAY) == pdPASS && currentRxMessage->IsMessageReady == false)  // as long as there are characters in the queue.
	{
		if (CurrentChar == '$'){ //Reset State Machine
			checksum_val = CurrentChar;
			sensorIdIdx = 0;
			MessageIdIdx = 0;
			ParamIdx = 0;
			checksumIdx = 0;
			currentState = SensorID_S;
			*currentRxMessage = EmptyMessage;
			continue;
		}

		// TO DO: we must calculate the received checksum!
		switch (currentState)
		{
			case Waiting_S: // Do nothing
				break;
			case SensorID_S: //Get Sensor ID Code
				checksum_val ^= CurrentChar;
				if(CurrentChar == ','){
					currentState = MessageID_S;
					break;
				}
				else if (sensorIdIdx < 5){
					sensorId[sensorIdIdx++] = CurrentChar;
				}
				if(sensorIdIdx == 5){
					//Add NULL Terminator
					sensorId[sensorIdIdx] = '\0';
					if(strcmp(sensorId, "CNTRL") == 0)//Sensor ID: Controller
						currentRxMessage->SensorID = Controller;
					else if(strcmp(sensorId, "ACSTC") == 0)//Sensor ID: Acoustic
						currentRxMessage->SensorID = Acoustic;
					else if(strcmp(sensorId, "DEPTH") == 0)//Sensor ID: Depth
						currentRxMessage->SensorID = Depth;
					else{//Sensor ID: None
						currentRxMessage->SensorID = None;
						currentState = Waiting_S;
					}
				}
				break;

			case MessageID_S: //Get Message Type
				checksum_val ^= CurrentChar;
				if(CurrentChar == ','){
					currentState = ParamsID_S;
				}
				else{
					if(MessageIdIdx < 2){
						currentRxMessage->messageId = currentRxMessage->messageId * 10;
						currentRxMessage->messageId += CurrentChar -  '0';
					}
					MessageIdIdx++;
				}
				break;

			case ParamsID_S: //Get Message Parameter (Period/Data)
				checksum_val ^= CurrentChar;

				if(CurrentChar == ','){
					currentState = Star_S;
				}
				else if(ParamIdx < 8){
					currentRxMessage->params = currentRxMessage->params * 10;
					currentRxMessage->params += CurrentChar -  '0';
				}
				break;

			case Star_S:
				checksum_val ^= CurrentChar;
				if(CurrentChar == ','){
					currentState = CS_S;
				}
				break;

			case CS_S:
				if(checksumIdx < 2){
					CSStr[checksumIdx++] = CurrentChar;
				}
				if(checksumIdx == 2){
					currentState = Waiting_S;
					CSStr[checksumIdx] = '\0';
					currentRxMessage->checksum = strtol(CSStr, NULL, 16);
					if(currentRxMessage->checksum == checksum_val){
						currentRxMessage->IsMessageReady = true;
						currentRxMessage->IsCheckSumValid = true;
					}else{
						currentRxMessage->IsCheckSumValid = false;
					}
				}
					break;
			}
		}
}



enum HostPCCommands parse_hostPC_message(){

	uint8_t CurrentChar;
	static char HostPCMessage[10];
	static uint16_t HostPCMessage_IDX = 0;


	while (xQueueReceive(Queue_hostPC_UART, &CurrentChar, portMAX_DELAY) == pdPASS){
		if(CurrentChar == '\n' || CurrentChar == '\r'|| HostPCMessage_IDX >=6){
			HostPCMessage[HostPCMessage_IDX++] = '\0';
			HostPCMessage_IDX = 0;
			if(strcmp(HostPCMessage, "START") == 0)
				return PC_Command_START;
			else if(strcmp(HostPCMessage, "RESET") == 0)
				return PC_Command_RESET;
		}else{
			HostPCMessage[HostPCMessage_IDX++] = CurrentChar;
		}

	}
	return PC_Command_NONE;
}


void send_sensorData_message(enum SensorId_t sensorType, uint16_t data){
	char tx_sensor_buffer[50];

	switch(sensorType){
	case Acoustic:
		sprintf(tx_sensor_buffer, "$ACSTC,03,%08u,*,00\n", data);
		break;
	case Depth:
		sprintf(tx_sensor_buffer, "$DEPTH,03,%08u,*,00\n", data);
		break;
	default:
		break;
	}
	sendStringSensor(tx_sensor_buffer);
}

void send_sensorEnable_message(enum SensorId_t sensorType, uint16_t TimePeriod_ms){
	char tx_sensor_buffer[50];

	switch(sensorType){
	case Acoustic:
		sprintf(tx_sensor_buffer, "$ACSTC,00,%08u,*,00\n", TimePeriod_ms);
		break;
	case Depth:
		sprintf(tx_sensor_buffer, "$DEPTH,00,%08u,*,00\n", TimePeriod_ms);
		break;
	default:
		break;
	}
	sendStringSensor(tx_sensor_buffer);
}

void send_sensorReset_message(void){
	char tx_sensor_buffer[50];

	sprintf(tx_sensor_buffer, "$CNTRL,00,,*,00\n");

	sendStringSensor(tx_sensor_buffer);
}

void send_ack_message(enum AckTypes AckType){
	char tx_sensor_buffer[50];

	switch(AckType){
	case RemoteSensingPlatformReset:
		sprintf(tx_sensor_buffer, "$CNTRL,01,,*,00\n");
		break;
	case AcousticSensorEnable:
		sprintf(tx_sensor_buffer, "$ACSTC,01,,*,00\n");
		break;
	case DepthSensorEnable:
		sprintf(tx_sensor_buffer, "$DEPTH,01,,*,00\n");
		break;
	}

	sendStringSensor(tx_sensor_buffer);
}


/*
 * AcousticSensor.c
 *
 *  Created on: Oct. 21, 2022
 *      Author: Andre Hendricks / Dr. JF Bousquet
 */
#include <stdlib.h>

#include "User/L2/Comm_Datalink.h"
#include "User/L3/AcousticSensor.h"

//Required FreeRTOS header files
#include "FreeRTOS.h"
#include "Timers.h"


/******************************************************************************
This is a software callback function.
******************************************************************************/
void RunAcousticSensor(TimerHandle_t xTimer) //Default 1000 ms
{
	const uint16_t variance = 50;
	const uint16_t mean = 100;

	send_sensorData_message(Acoustic, (rand() % variance) + mean);
}


/*
 * DepthSensorController.c
 *
 *  Created on: Oct. 21, 2022
 *      Author: Andre Hendricks / Dr. JF Bousquet
 */

#include <stdbool.h>

#include "User/L2/Comm_Datalink.h"
#include "User/L3/DepthSensor.h"

//Required FreeRTOS header files
#include "FreeRTOS.h"
#include "Timers.h"


/******************************************************************************
This is a software callback function.
******************************************************************************/
void RunDepthSensor(TimerHandle_t xTimer)
{
	static uint16_t depth = 10;
	static uint8_t down = true;
	if(down)
		depth += 10;
	else
		depth -=10;

	if(depth == 300)
		down = false;
	if(depth== 0)
		down = true;

	send_sensorData_message(Depth, depth);
}


/*
 * remoteSensingPlatform.c
 *
 *  Created on: Oct. 21, 2022
 *      Author: Andre Hendricks / Dr. JF Bousquet
 */
#include <stdio.h>

#include "User/L2/Comm_Datalink.h"
#include "User/L3/AcousticSensor.h"
#include "User/L3/DepthSensor.h"
#include "User/L4/SensorPlatform.h"
#include "User/util.h"

//Required FreeRTOS header files
#include "FreeRTOS.h"
#include "Timers.h"
#include "semphr.h"

static void ResetMessageStruct(struct CommMessage* currentRxMessage){

	static const struct CommMessage EmptyMessage = {0};
	*currentRxMessage = EmptyMessage;
}

/******************************************************************************
This task is created from the main.
It is responsible for managing the messages from the datalink.
It is also responsible for starting the timers for each sensor
******************************************************************************/
//task 1
void SensorPlatformTask(void *params)
{
	const TickType_t TimerDefaultPeriod = 1000;
	TimerHandle_t TimerID_AcousticSensor,TimerID_DepthSensor;

	TimerID_DepthSensor = xTimerCreate(
		"Depth Sensor Task",
		TimerDefaultPeriod,		// Period: Needed to be changed based on parameter
		pdTRUE,		// Autoreload: Continue running till deleted or stopped
		(void*)0,
		RunDepthSensor
		);

	TimerID_AcousticSensor = xTimerCreate(
		"Acoustic Sensor Task",
		TimerDefaultPeriod,		// Period: Needed to be changed based on parameter
		pdTRUE,		// Autoreload: Continue running till deleted or stopped
		(void*)1,
		RunAcousticSensor
		);

	request_sensor_read();  // requests a usart read (through the callback)

	struct CommMessage currentRxMessage = {0};

	do {

		parse_sensor_message(&currentRxMessage);

		if(currentRxMessage.IsMessageReady == true && currentRxMessage.IsCheckSumValid == true){

			switch(currentRxMessage.SensorID){
				case Controller:
					switch(currentRxMessage.messageId){
						case 0:
							xTimerStop(TimerID_DepthSensor, portMAX_DELAY);
							xTimerStop(TimerID_AcousticSensor, portMAX_DELAY);
							send_ack_message(RemoteSensingPlatformReset);
							break;
						case 1: //Do Nothing
							break;
						case 3: //Do Nothing
							break;
						}
					break;
				case Acoustic:
					switch(currentRxMessage.messageId){
						case 0:
							xTimerChangePeriod(TimerID_AcousticSensor, currentRxMessage.params, portMAX_DELAY);
							xTimerStart(TimerID_AcousticSensor, portMAX_DELAY);
							send_ack_message(AcousticSensorEnable);
							break;
						case 1: //Do Nothing
							break;
						case 3: //Do Nothing
							break;
					}
					break;
				case Depth:
					switch(currentRxMessage.messageId){
						case 0:
							xTimerChangePeriod(TimerID_DepthSensor, currentRxMessage.params, portMAX_DELAY);
							xTimerStart(TimerID_DepthSensor, portMAX_DELAY);
							send_ack_message(DepthSensorEnable);
							break;
						case 1: //Do Nothing
							break;
						case 3: //Do Nothing
							break;
					}
					break;
					default://Should not get here
						break;
			}
			ResetMessageStruct(&currentRxMessage);
		}
	} while(1);
}

