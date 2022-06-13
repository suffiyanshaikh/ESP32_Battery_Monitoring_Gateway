# ESP32 Battery Monitoring Gateway


### Overview 
An application is made on ESP32 to monitor the battery's voltage over the Internet. Enabling IoT in Battery monitoring system.
Reading battery voltage on ESP32 ADC and publishing the data in JSON format over the cloud using MQTT protocol and Wifi 
Connection.

Gateway Features
* WiFi
* MQTT
* ADC

### NOTES
* The following application can be run on **ESP32** and **ESP32S2** Target.
* The batteries up to 3.3V ratings can be directly connected to ESP32 ADC pins, the voltage divider is mandatory if batteries
rating are above 3.3V


## How to use the example

 **Hardware Connections**

 For ESP32 Target
* Connect Battery Positive terminal to GPIO_36(ADC1_CH0).
* Make the GND common of Battery and ESP

 For ESP32S2 Target
* Connect Battery Positive terminal to GPIO_01(ADC1_CH0).
* Make the GND common of Battery and ESP

**Configure the Project**

Open the project configuration menu ( idf.py menuconfig).

In the Device Configuration Menu:
* Provide the Wi-Fi SSID and password of the AP you wish to connect to
* Provide the MQTT Credentials you wish to connect with,ex:mqtt://test.mosquitto.org


![Esp32_Battery_Monitoring](https://user-images.githubusercontent.com/42150715/173247536-c4a6a4b3-ff5c-4541-945a-b4b66561cbac.png)

**Build and Flash**
* Run idf.py -p PORT flash monitor to build and flash the project.



## Example Output

![image](https://user-images.githubusercontent.com/42150715/173248049-5dd8ac40-8a2e-403f-a9d6-b507e3fbbb9b.png)














