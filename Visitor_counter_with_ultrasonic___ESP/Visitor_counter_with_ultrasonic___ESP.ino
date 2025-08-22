#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <U8g2lib.h>
#include <NewPing.h>

#define BLYNK_AUTH "YourAuthToken" // Enter your Blynk authentication token
#define TRIGGER_PIN_1 2 // Pin connected to the trigger pin of HC-SR04 sensor 1
#define ECHO_PIN_1 4 // Pin connected to the echo pin of HC-SR04 sensor 1
#define TRIGGER_PIN_2 16 // Pin connected to the trigger pin of HC-SR04 sensor 2
#define ECHO_PIN_2 17 // Pin connected to the echo pin of HC-SR04 sensor 2
#define RELAY_PIN 13 // Pin connected to the relay

char auth[] = BLYNK_AUTH;
char ssid[] = "nameofwife"; // Enter your WiFi SSID
char pass[] = "123456789"; // Enter your WiFi password

#define OLED_SDA 21 // Pin connected to the SDA pin of OLED display
#define OLED_SCL 22 // Pin connected to the SCL pin of OLED display
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, OLED_SCL, OLED_SDA, U8X8_PIN_NONE);

NewPing sonar1(TRIGGER_PIN_1, ECHO_PIN_1, 500); // Max distance 500cm for sensor 1
NewPing sonar2(TRIGGER_PIN_2, ECHO_PIN_2, 500); // Max distance 500cm for sensor 2

int In = 0;
int Out = 0;
int total = Out - In; 
void updateCounter() {
  int distance1 = sonar1.ping_cm();
  int distance2 = sonar2.ping_cm();
  
  if (distance1 > 0 && distance1 < 50) { // Adjust this distance based on your setup
    countIn++;
    Blynk.virtualWrite(V1, countIn); // Update countIn on Blynk app
    digitalWrite(RELAY_PIN, HIGH); // Activate relay for entry
    delay(500); // Adjust the delay as needed
    digitalWrite(RELAY_PIN, LOW); // Deactivate relay
  }
  
  if (distance2 > 0 && distance2 < 50) { // Adjust this distance based on your setup
    countOut++;
    Blynk.virtualWrite(V2, countOut); // Update countOut on Blynk app
    digitalWrite(RELAY_PIN, HIGH); // Activate relay for exit
    delay(500); // Adjust the delay as needed
    digitalWrite(RELAY_PIN, LOW); // Deactivate relay
  }
}

void setup() {
  Serial.begin(9600);
  u8g2.begin();
  Blynk.begin(auth, ssid, pass);
  pinMode(RELAY_PIN, OUTPUT);
}

void loop() {
  Blynk.run();
  updateCounter();
  drawDisplay();
  delay(100);
}

void drawDisplay() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(0, 20);
    u8g2.print("In: ");
    u8g2.print(countIn);
    u8g2.setCursor(0, 40);
    u8g2.print("Out: ");
    u8g2.print(countOut);
   u8g2.setCursor(80,80 );
    u8g2.print("total: ");
    u8g2.print(total);
  } while (u8g2.nextPage());
}
