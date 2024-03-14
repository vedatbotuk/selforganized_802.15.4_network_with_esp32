# selforganized_802.15.4_network_with_esp32
Selforganized network library for esp32-C6 and esp32-H2

## sensor_devices
equipped with:
- DHT22 Sensor
- Battery level Sensor

## Folder Structure
- COORDINATOR
  - can only create network. Temperature, Humidity and Power Cluster are not included.
- FFD
  - router_only
  - router_sensor_device
- RFD
  - sensor_device_light_sleep
  - sensor_devoce_deepsleep

# Install following libs in /components

- esp-zboss-lib
- esp-zigbee-lib

# Signal handlar maping
Sensor or feature | Binary value
------------------|-------------
SENSOR_WATERLEAK  | 0 or 1
DEEP_SLEEP        | 0 or 1
LIGHT_SLEEP       | 0 or 1
BATTERY           | 0 or 1
Temperature       | 0 or 1
Humidity          | 0 or 1

For example use a RFD, which suppert temperature, humidity and light sleep.
```001011```