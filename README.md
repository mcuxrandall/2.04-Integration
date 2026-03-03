# 2.04-Integration
Early code for Senior Design 2.04 NXP Intgegration

## IMU
### Running IMU "whoami" code
Import the IMU/IMU_current/New_SDK_Project_LSM6DSO into MCUXpresso and run on the MCXN947 and debug if possible.

## Temperature Sensors
### Internal Temperaure Sensor
To read the internal I3C P3T1755 temperature sensor import temp_sensors/internal_i3c_temp/internal_temp_i3c_master_read_sensor_p3t1755_cm33_core0 and run on the MCXN947.

### External Temperature Sensor
To read the external I3C P3T1755 temperature sensor import temp_sensors/external_i3c_temp/temp_sensor_testing_i3c_master_read_sensor_p3t1755_cm33_core0 and run on the MCXN947. It is assumed that the external temperature sensor is hard wired to the 0x4F address in order to initiate I3C communication. Address can be changed in app.h if needed (under "#define SENSOR_SLAVE_ADDR").
