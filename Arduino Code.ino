#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <SoftwareSerial.h>

#define BT_RX 10  // HC-05 RX 
#define BT_TX 11  // HC-05 TX 
#define BUTTON 4  // Push button
#define RED_LED 7 // Red light
#define WHITE_LED 8 // White light
#define BUZZER_POS 5  // Positive terminal of the passive buzzer
#define BUZZER_NEG 6  // Negative terminal (simulated ground)

SoftwareSerial BT(BT_RX, BT_TX); 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

bool deviceOn = true;  // Device state
unsigned long offStartTime = 0; // Timestamp when device was turned off
const unsigned long offDuration = 3600000; // 1 hour in milliseconds

void setup() {
    pinMode(BUTTON, INPUT_PULLUP); // Enable internal resistor
    pinMode(RED_LED, OUTPUT);
    pinMode(WHITE_LED, OUTPUT);
    pinMode(BUZZER_POS, OUTPUT);
    pinMode(BUZZER_NEG, OUTPUT);
    
    BT.begin(9600);   // Start Bluetooth 
    Serial.begin(9600); // Start Monitor
    mlx.begin();       // Initialize sensor
    
    digitalWrite(WHITE_LED, HIGH); 
    digitalWrite(RED_LED, LOW);
    
    // Set buzzer to idle (no buzzing)
    digitalWrite(BUZZER_POS, LOW);
    digitalWrite(BUZZER_NEG, HIGH);  
}

void loop() {
    static bool lastButtonState = HIGH;
    bool buttonState = digitalRead(BUTTON);
    
    // Handle button press
    if (buttonState == LOW && lastButtonState == HIGH) {
        toggleDeviceState();
        delay(300); // Small debounce delay
    }
    lastButtonState = buttonState;
    
    // Restore device after 1 hour
    if (!deviceOn && millis() - offStartTime >= offDuration) {
        deviceOn = true;
        digitalWrite(WHITE_LED, HIGH);
        Serial.println("Device back ON");
    }
    
    // Check Bluetooth for "cook" command
    if (BT.available()) {
        String command = BT.readString();
        command.trim();
        if (command == "cook") {        // COOK BUTTON ON APP RECEIVER
            toggleDeviceState();
        }
    }
    
    // Process temperature readings if device is ON
    if (deviceOn) {
        float objectTemp = mlx.readObjectTempC();
        float ambientTemp = mlx.readAmbientTempC();

        // Send temperature over Bluetooth
        BT.println(objectTemp);
        Serial.print("Sent temperature: ");      // TEMPERATURE TO APP SENDER
        Serial.println(objectTemp);
        Serial.println(ambientTemp);
        
        // Red LED alert for high object temperature
        if (objectTemp > 50) {
            digitalWrite(RED_LED, HIGH);
            digitalWrite(WHITE_LED, LOW);
        } else {
            digitalWrite(RED_LED, LOW);
            digitalWrite(WHITE_LED, HIGH);

        }
        
        // Buzzer and fire alert for high ambient temperature
        if (ambientTemp > 50) {
            BT.println("fire");      // FIRE NOTIFIER ON APP SENDER
            beepBuzzer();
        } else {
            digitalWrite(BUZZER_POS, LOW);
            digitalWrite(BUZZER_NEG, HIGH);
        }
    }
    delay(5000); // Send data every 5 seconds
}

void toggleDeviceState() {
    deviceOn = !deviceOn;
    if (!deviceOn) {
        digitalWrite(WHITE_LED, LOW);
        digitalWrite(RED_LED, LOW);
        Serial.println("Device OFF for 1 hour");
        offStartTime = millis();
    } else {
        digitalWrite(WHITE_LED, HIGH);
        Serial.println("Device ON");
    }
}

void beepBuzzer() {
    for (int i = 0; i < 5; i++) {
        digitalWrite(BUZZER_POS, HIGH);
        digitalWrite(BUZZER_NEG, LOW);
        delay(200);
        digitalWrite(BUZZER_POS, LOW);
        digitalWrite(BUZZER_NEG, HIGH);
        delay(200);
    }
}
