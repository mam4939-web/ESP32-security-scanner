# ESP32-security-scanner

ESP32 and nRF24L01 based 2.4GHz RF security scanner and packet monitor with SSD1306 OLED display.

## Hardware Components
* ESP32 DevKit V1
* nRF24L01+PA+LNA
* 0.96" I2C OLED Display (SSD1306)

## Pinout
### OLED Display
* GND -> GND
* VCC -> 3V3
* SCL -> GPIO 22
* SDA -> GPIO 21

### nRF24L01
* CE -> GPIO 4
* CSN -> GPIO 5
* SCK -> GPIO 18
* MOSI -> GPIO 23
* MISO -> GPIO 19
