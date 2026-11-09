#include <WiFi.h>
#include <esp_wifi.h>
#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define CE_PIN   4
#define CSN_PIN  5
RF24 radio(CE_PIN, CSN_PIN);
const uint64_t radioPipe = 0xE8E8F0F0E1LL;

int threatCount = 0;
String sysStatus = "System Armed";
unsigned long timer = 0;

void wifiCallback(void* buf, wifi_promiscuous_pkt_type_t type) {
    wifi_promiscuous_pkt_t *packet = (wifi_promiscuous_pkt_t*)buf;
    if (packet->payload[0] == 0xC0 || packet->payload[0] == 0xA0) {
        threatCount++;
        sysStatus = "Deauth Alert!";
    }
}

void setup() {
    Serial.begin(115200);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        while(1);
    }
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Mama Papa Secure Init");
    display.display();
    delay(800);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(&wifiCallback);

    radio.begin();
    radio.openReadingPipe(1, radioPipe);
    radio.setChannel(76);
    radio.setDataRate(RF24_2MBPS);
    radio.startListening();
}

void loop() {
    if (radio.available()) {
        char buffer[32] = {0};
        radio.read(&buffer, sizeof(buffer));
        threatCount++;
        sysStatus = "RF Packet Sensed";
    }

    if (millis() - timer > 1000) {
        timer = millis();
        
        display.clearDisplay();
        
        display.setCursor(0, 0);
        display.print("=== MAMA & PAPA ===");
        
        display.setCursor(0, 16);
        display.print("Status: ");
        display.print(sysStatus);
        
        display.setCursor(0, 32);
        display.print("Threats: ");
        display.print(threatCount);
        
        display.setCursor(0, 48);
        display.print("2.4G RF: ACTIVE");
        
        for (int x = 0; x < 128; x += 4) {
            int h = random(2, 10);
            display.drawFastVLine(x, 63 - h, h, SSD1306_WHITE);
        }
        
        display.display();
        
        if (sysStatus != "System Armed") {
            sysStatus = "System Armed";
        }
    }
}