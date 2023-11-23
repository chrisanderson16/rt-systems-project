INC=-I.

sensor_make: main.c src/pressure_sensor/pressure_sensor.c src/inertia_measurement_unit_sensor/imu.c src/camera/camera.c src/gps/gps.c
	gcc -o out main.c src/pressure_sensor/pressure_sensor.c src/inertia_measurement_unit_sensor/imu.c src/camera/camera.c src/gps/gps.c -I. -lm
