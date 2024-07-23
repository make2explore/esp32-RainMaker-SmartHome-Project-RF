// ---------------------------------- make2explore.com -------------------------------------------------------//
// Project           - ESP RainMaker + Alexa + Google Home + Manual Switch + RF Remote control 8 Relays
// Created By        - info@make2explore.com
// Last Modified     - 31/03/2024 17:36:00 @admin
// Software          - C/C++, Arduino IDE, Libraries
// Hardware          - NodeMCU ESP32S, 8 Channel Relay Board, 433 Mhz RF Receiver module, and 8CH RF Remote     
// Sensors Used      - Null
// Source Repo       - github.com/make2explore
// ===========================================================================================================//

// Include necessary libraries
#include "RMaker.h"                     // Include Rainmaker library
#include "WiFi.h"                       // WiFi related libraries
#include "WiFiProv.h"
#include <RCSwitch.h>

#define DECODE_NEC                      // IR Remote Protocol

#define RFrecieverPin 34                // RF Receiver on interrupt 0 => that is pin #34

RCSwitch mySwitch = RCSwitch();

// Provide BLE provisioning service name to ESP32 Device
const char *service_name = "PROV_12345";
const char *pop = "1234567";

// define the Device Names
char deviceName_1[] = "Hall Light";
char deviceName_2[] = "Kitchen Light";
char deviceName_3[] = "Bedroom Light";
char deviceName_4[] = "Lounge Light";
char deviceName_5[] = "Garage Light";
char deviceName_6[] = "Bathroom Light";
char deviceName_7[] = "Basement Light";
char deviceName_8[] = "Garden Light";


// Command Code of RF Remote buttons 
#define RF_Button_1   604728
#define RF_Button_2   604724
#define RF_Button_3   604732
#define RF_Button_4   604722
#define RF_Button_5   604730
#define RF_Button_6   604726
#define RF_Button_7   604734
#define RF_Button_8   604721

// define the GPIO connected with Relays
static uint8_t RelayPin1 = 23;  //D23
static uint8_t RelayPin2 = 22;  //D22
static uint8_t RelayPin3 = 21;  //D21
static uint8_t RelayPin4 = 19;  //D19
static uint8_t RelayPin5 = 18;  //D18
static uint8_t RelayPin6 = 5;   //D5
static uint8_t RelayPin7 = 4;  //D4
static uint8_t RelayPin8 = 15;  //D15

// define the GPIO connected with switches
static uint8_t SwitchPin1 = 13;  //D13
static uint8_t SwitchPin2 = 12;  //D12
static uint8_t SwitchPin3 = 14;  //D14
static uint8_t SwitchPin4 = 27;  //D27
static uint8_t SwitchPin5 = 26;  //D26
static uint8_t SwitchPin6 = 25;  //D25
static uint8_t SwitchPin7 = 33;  //D33
static uint8_t SwitchPin8 = 32;  //32

static uint8_t wifiLed    = 2;   // D2 Blue LED
static uint8_t gpio_reset = 0;

// Variables for reading pin status
// Relay State
// Define integer to remember the toggle state for relays
bool toggleState_1 = LOW; 
bool toggleState_2 = LOW; 
bool toggleState_3 = LOW; 
bool toggleState_4 = LOW; 
bool toggleState_5 = LOW; 
bool toggleState_6 = LOW; 
bool toggleState_7 = LOW; 
bool toggleState_8 = LOW; 

// Switch State
// Define integer to remember the toggle state for Switches
bool SwitchState_1 = LOW;
bool SwitchState_2 = LOW;
bool SwitchState_3 = LOW;
bool SwitchState_4 = LOW;
bool SwitchState_5 = LOW;
bool SwitchState_6 = LOW;
bool SwitchState_7 = LOW;
bool SwitchState_8 = LOW;

// The Rainmaker framework provides some standard device types like switch, lightbulb, fan, temperature sensor. Etc.
static Switch my_hall_Light(deviceName_1, &RelayPin1);
static Switch my_Kitchen_Light(deviceName_2, &RelayPin2);
static Switch my_bedroom_Light(deviceName_3, &RelayPin3);
static Switch my_lounge_Light(deviceName_4, &RelayPin4);
static Switch my_Garage_Light(deviceName_5, &RelayPin5);
static Switch my_Bathroom_Light(deviceName_6, &RelayPin6);
static Switch my_Basement_Light(deviceName_7, &RelayPin7);
static Switch my_Garden_Light(deviceName_8, &RelayPin8);

// BLE Provisioning Service function
void sysProvEvent(arduino_event_t *sys_event)
{
    switch (sys_event->event_id) {      
        case ARDUINO_EVENT_PROV_START:
#if CONFIG_IDF_TARGET_ESP32
        Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on BLE\n", service_name, pop);
        printQR(service_name, pop, "ble");
#else
        Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on SoftAP\n", service_name, pop);
        printQR(service_name, pop, "softap");
#endif        
        break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        Serial.printf("\nConnected to Wi-Fi!\n");
        digitalWrite(wifiLed, true);
        break;
    }
}

// Callback functions for devices state
void write_callback(Device *device, Param *param, const param_val_t val, void *priv_data, write_ctx_t *ctx)
{
    const char *device_name = device->getDeviceName();
    const char *param_name = param->getParamName();

    if(strcmp(device_name, deviceName_1) == 0) {
      
      Serial.printf("Lightbulb = %s\n", val.val.b? "true" : "false");
      
      if(strcmp(param_name, "Power") == 0) {
          Serial.printf("Received value = %s for %s - %s\n", val.val.b? "true" : "false", device_name, param_name);
        toggleState_1 = val.val.b;
        (toggleState_1 == false) ? digitalWrite(RelayPin1, HIGH) : digitalWrite(RelayPin1, LOW);
        param->updateAndReport(val);
      }
      
    } else if(strcmp(device_name, deviceName_2) == 0) {
      
      Serial.printf("Switch value = %s\n", val.val.b? "true" : "false");

      if(strcmp(param_name, "Power") == 0) {
        Serial.printf("Received value = %s for %s - %s\n", val.val.b? "true" : "false", device_name, param_name);
        toggleState_2 = val.val.b;
        (toggleState_2 == false) ? digitalWrite(RelayPin2, HIGH) : digitalWrite(RelayPin2, LOW);
        param->updateAndReport(val);
      }
  
    } else if(strcmp(device_name, deviceName_3) == 0) {
      
      Serial.printf("Switch value = %s\n", val.val.b? "true" : "false");

      if(strcmp(param_name, "Power") == 0) {
        Serial.printf("Received value = %s for %s - %s\n", val.val.b? "true" : "false", device_name, param_name);
        toggleState_3 = val.val.b;
        (toggleState_3 == false) ? digitalWrite(RelayPin3, HIGH) : digitalWrite(RelayPin3, LOW);
        param->updateAndReport(val);
      }
  
    } else if(strcmp(device_name, deviceName_4) == 0) {
      
      Serial.printf("Switch value = %s\n", val.val.b? "true" : "false");

      if(strcmp(param_name, "Power") == 0) {
        Serial.printf("Received value = %s for %s - %s\n", val.val.b? "true" : "false", device_name, param_name);
        toggleState_4 = val.val.b;
        (toggleState_4 == false) ? digitalWrite(RelayPin4, HIGH) : digitalWrite(RelayPin4, LOW);
        param->updateAndReport(val);
      } 
       
    } else if(strcmp(device_name, deviceName_5) == 0) {
      
      Serial.printf("Lightbulb = %s\n", val.val.b? "true" : "false");
      
      if(strcmp(param_name, "Power") == 0) {
          Serial.printf("Received value = %s for %s - %s\n", val.val.b? "true" : "false", device_name, param_name);
        toggleState_5 = val.val.b;
        (toggleState_5 == false) ? digitalWrite(RelayPin5, HIGH) : digitalWrite(RelayPin5, LOW);
        param->updateAndReport(val);
      }
      
    } else if(strcmp(device_name, deviceName_6) == 0) {
      
      Serial.printf("Switch value = %s\n", val.val.b? "true" : "false");

      if(strcmp(param_name, "Power") == 0) {
        Serial.printf("Received value = %s for %s - %s\n", val.val.b? "true" : "false", device_name, param_name);
        toggleState_6 = val.val.b;
        (toggleState_6 == false) ? digitalWrite(RelayPin6, HIGH) : digitalWrite(RelayPin6, LOW);
        param->updateAndReport(val);
      }
  
    } else if(strcmp(device_name, deviceName_7) == 0) {
      
      Serial.printf("Switch value = %s\n", val.val.b? "true" : "false");

      if(strcmp(param_name, "Power") == 0) {
        Serial.printf("Received value = %s for %s - %s\n", val.val.b? "true" : "false", device_name, param_name);
        toggleState_7 = val.val.b;
        (toggleState_7 == false) ? digitalWrite(RelayPin7, HIGH) : digitalWrite(RelayPin7, LOW);
        param->updateAndReport(val);
      }
  
    } else if(strcmp(device_name, deviceName_8) == 0) {
      
      Serial.printf("Switch value = %s\n", val.val.b? "true" : "false");

      if(strcmp(param_name, "Power") == 0) {
        Serial.printf("Received value = %s for %s - %s\n", val.val.b? "true" : "false", device_name, param_name);
        toggleState_8 = val.val.b;
        (toggleState_8 == false) ? digitalWrite(RelayPin8, HIGH) : digitalWrite(RelayPin8, LOW);
        param->updateAndReport(val);
      }  
    } 
}

// Manual Switch control function
void manual_control()
{
  if (digitalRead(SwitchPin1) == LOW && SwitchState_1 == LOW) {
    digitalWrite(RelayPin1, LOW);
    toggleState_1 = 1;
    SwitchState_1 = HIGH;
    my_hall_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_1);
    Serial.println("Switch-1 on");
  }
  if (digitalRead(SwitchPin1) == HIGH && SwitchState_1 == HIGH) {
    digitalWrite(RelayPin1, HIGH);
    toggleState_1 = 0;
    SwitchState_1 = LOW;
    my_hall_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_1);
    Serial.println("Switch-1 off");
  }
  if (digitalRead(SwitchPin2) == LOW && SwitchState_2 == LOW) {
    digitalWrite(RelayPin2, LOW);
    toggleState_2 = 1;
    SwitchState_2 = HIGH;
    my_Kitchen_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_2);
    Serial.println("Switch-2 on");
  }
  if (digitalRead(SwitchPin2) == HIGH && SwitchState_2 == HIGH) {
    digitalWrite(RelayPin2, HIGH);
    toggleState_2 = 0;
    SwitchState_2 = LOW;
    my_Kitchen_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_2);
    Serial.println("Switch-2 off");
  }
  if (digitalRead(SwitchPin3) == LOW && SwitchState_3 == LOW) {
    digitalWrite(RelayPin3, LOW);
    toggleState_3 = 1;
    SwitchState_3 = HIGH;
    my_bedroom_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_3);
    Serial.println("Switch-3 on");
  }
  if (digitalRead(SwitchPin3) == HIGH && SwitchState_3 == HIGH) {
    digitalWrite(RelayPin3, HIGH);
    toggleState_3 = 0;
    SwitchState_3 = LOW;
    my_bedroom_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_3);
    Serial.println("Switch-3 off");
  }
  if (digitalRead(SwitchPin4) == LOW && SwitchState_4 == LOW) {
    digitalWrite(RelayPin4, LOW);
    toggleState_4 = 1;
    SwitchState_4 = HIGH;
    my_lounge_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_4);
    Serial.println("Switch-4 on");
  }
  if (digitalRead(SwitchPin4) == HIGH && SwitchState_4 == HIGH) {
    digitalWrite(RelayPin4, HIGH);
    toggleState_4 = 0;
    SwitchState_4 = LOW;
    my_lounge_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_4);
    Serial.println("Switch-4 off");
  }
  if (digitalRead(SwitchPin5) == LOW && SwitchState_5 == LOW) {
    digitalWrite(RelayPin5, LOW);
    toggleState_5 = 1;
    SwitchState_5 = HIGH;
    my_Garage_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_5);
    Serial.println("Switch-5 on");
  }
  if (digitalRead(SwitchPin5) == HIGH && SwitchState_5 == HIGH) {
    digitalWrite(RelayPin5, HIGH);
    toggleState_5 = 0;
    SwitchState_5 = LOW;
    my_Garage_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_5);
    Serial.println("Switch-5 off");
  }
  if (digitalRead(SwitchPin6) == LOW && SwitchState_6 == LOW) {
    digitalWrite(RelayPin6, LOW);
    toggleState_6 = 1;
    SwitchState_6 = HIGH;
    my_Bathroom_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_6);
    Serial.println("Switch-6 on");
  }
  if (digitalRead(SwitchPin6) == HIGH && SwitchState_6 == HIGH) {
    digitalWrite(RelayPin6, HIGH);
    toggleState_6 = 0;
    SwitchState_6 = LOW;
    my_Bathroom_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_6);
    Serial.println("Switch-6 off");
  }
  if (digitalRead(SwitchPin7) == LOW && SwitchState_7 == LOW) {
    digitalWrite(RelayPin7, LOW);
    toggleState_7 = 1;
    SwitchState_7 = HIGH;
    my_Basement_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_7);
    Serial.println("Switch-7 on");
  }
  if (digitalRead(SwitchPin7) == HIGH && SwitchState_7 == HIGH) {
    digitalWrite(RelayPin7, HIGH);
    toggleState_7 = 0;
    SwitchState_7 = LOW;
    my_Basement_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_7);
    Serial.println("Switch-7 off");
  }
  if (digitalRead(SwitchPin8) == LOW && SwitchState_8 == LOW) {
    digitalWrite(RelayPin8, LOW);
    toggleState_8 = 1;
    SwitchState_8 = HIGH;
    my_Garden_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_8);
    Serial.println("Switch-8 on");
  }
  if (digitalRead(SwitchPin8) == HIGH && SwitchState_8 == HIGH) {
    digitalWrite(RelayPin8, HIGH);
    toggleState_8 = 0;
    SwitchState_8 = LOW;
    my_Garden_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_8);
    Serial.println("Switch-8 off");
  }
}  


void all_SwitchOff(){
  toggleState_1 = 0; digitalWrite(RelayPin1, HIGH); my_hall_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_1); delay(100);
  toggleState_2 = 0; digitalWrite(RelayPin2, HIGH); my_Kitchen_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_2); delay(100);
  toggleState_3 = 0; digitalWrite(RelayPin3, HIGH); my_bedroom_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_3); delay(100);
  toggleState_4 = 0; digitalWrite(RelayPin4, HIGH); my_lounge_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_4); delay(100);
  toggleState_5 = 0; digitalWrite(RelayPin5, HIGH); my_Garage_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_5); delay(100);
  toggleState_6 = 0; digitalWrite(RelayPin6, HIGH); my_Bathroom_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_6); delay(100);
  toggleState_7 = 0; digitalWrite(RelayPin7, HIGH); my_Basement_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_7); delay(100);
  toggleState_8 = 0; digitalWrite(RelayPin8, HIGH); my_Garden_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_8); delay(100);
}

void all_SwitchOn(){
  toggleState_1 = 1; digitalWrite(RelayPin1, LOW); my_hall_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_1); delay(100);
  toggleState_2 = 1; digitalWrite(RelayPin2, LOW); my_Kitchen_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_2); delay(100);
  toggleState_3 = 1; digitalWrite(RelayPin3, LOW); my_bedroom_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_3); delay(100);
  toggleState_4 = 1; digitalWrite(RelayPin4, LOW); my_lounge_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_4); delay(100);
  toggleState_5 = 1; digitalWrite(RelayPin5, LOW); my_Garage_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_5); delay(100);
  toggleState_6 = 1; digitalWrite(RelayPin6, LOW); my_Bathroom_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_6); delay(100);
  toggleState_7 = 1; digitalWrite(RelayPin7, LOW); my_Basement_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_7); delay(100);
  toggleState_8 = 1; digitalWrite(RelayPin8, LOW); my_Garden_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_8); delay(100);
}


// RF Remote control code
void RF_remote(){
  if (mySwitch.available()) {
      uint32_t RFCode = mySwitch.getReceivedValue();
      switch(RFCode){
          case RF_Button_1:  
            digitalWrite(RelayPin1, toggleState_1);
            toggleState_1 = !toggleState_1;
            my_hall_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_1);
            delay(100);            
            break;
          case RF_Button_2:  
            digitalWrite(RelayPin2, toggleState_2);
            toggleState_2 = !toggleState_2;
            my_Kitchen_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_2);
            delay(100);            
            break;
          case RF_Button_3:  
            digitalWrite(RelayPin3, toggleState_3);
            toggleState_3 = !toggleState_3;
            my_bedroom_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_3);
            delay(100);            
            break;
          case RF_Button_4:  
            digitalWrite(RelayPin4, toggleState_4);
            toggleState_4 = !toggleState_4;
            my_lounge_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_4);
            delay(100);            
            break;
          case RF_Button_5:  
            digitalWrite(RelayPin5, toggleState_5);
            toggleState_5 = !toggleState_5;
            my_Garage_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_5);
            delay(100);            
            break;
          case RF_Button_6:  
            digitalWrite(RelayPin6, toggleState_6);
            toggleState_6 = !toggleState_6;
            my_Bathroom_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_6);
            delay(100);            
            break;
          case RF_Button_7:  
            digitalWrite(RelayPin7, toggleState_7);
            toggleState_7 = !toggleState_7;
            my_Basement_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_7);
            delay(100);            
            break;
          case RF_Button_8:  
            digitalWrite(RelayPin8, toggleState_8);
            toggleState_8 = !toggleState_8;
            my_Garden_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, toggleState_8);
            delay(100);            
            break;
          default : break;         
        }      
        mySwitch.resetAvailable();   
  } 
}

void init_GPIO(){
    // Set the Relays GPIOs as output mode
    pinMode(RelayPin1, OUTPUT);
    pinMode(RelayPin2, OUTPUT);
    pinMode(RelayPin3, OUTPUT);
    pinMode(RelayPin4, OUTPUT);
    pinMode(RelayPin5, OUTPUT);
    pinMode(RelayPin6, OUTPUT);
    pinMode(RelayPin7, OUTPUT);
    pinMode(RelayPin8, OUTPUT);  
    pinMode(wifiLed, OUTPUT);
    
    // Configure the input GPIOs
    pinMode(SwitchPin1, INPUT_PULLUP);
    pinMode(SwitchPin2, INPUT_PULLUP);
    pinMode(SwitchPin3, INPUT_PULLUP);
    pinMode(SwitchPin4, INPUT_PULLUP);
    pinMode(SwitchPin5, INPUT_PULLUP);
    pinMode(SwitchPin6, INPUT_PULLUP);
    pinMode(SwitchPin7, INPUT_PULLUP);
    pinMode(SwitchPin8, INPUT_PULLUP);
    pinMode(gpio_reset, INPUT);
    
    // Write to the GPIOs the default state on booting
    digitalWrite(RelayPin1, !toggleState_1);
    digitalWrite(RelayPin2, !toggleState_2);
    digitalWrite(RelayPin3, !toggleState_3);
    digitalWrite(RelayPin4, !toggleState_4);
    digitalWrite(RelayPin5, !toggleState_5);
    digitalWrite(RelayPin6, !toggleState_6);
    digitalWrite(RelayPin7, !toggleState_7);
    digitalWrite(RelayPin8, !toggleState_8);
    digitalWrite(wifiLed, LOW);
}

void init_RainMaker(){
    uint32_t chipId = 0;
    Node my_node;    
    my_node = RMaker.initNode("Rainmaker_Home_Lights");

    //Standard switch device
    my_hall_Light.addCb(write_callback);
    my_Kitchen_Light.addCb(write_callback);
    my_bedroom_Light.addCb(write_callback);
    my_lounge_Light.addCb(write_callback);
    my_Garage_Light.addCb(write_callback);
    my_Bathroom_Light.addCb(write_callback);
    my_Basement_Light.addCb(write_callback);
    my_Garden_Light.addCb(write_callback);

    //Add switch device to the node   
    my_node.addDevice(my_hall_Light);
    my_node.addDevice(my_Kitchen_Light);
    my_node.addDevice(my_bedroom_Light);
    my_node.addDevice(my_lounge_Light);
    my_node.addDevice(my_Garage_Light);
    my_node.addDevice(my_Bathroom_Light);
    my_node.addDevice(my_Basement_Light);
    my_node.addDevice(my_Garden_Light);

    //This is optional 
    RMaker.enableOTA(OTA_USING_PARAMS);
    //If you want to enable scheduling, set time zone for your region using setTimeZone(). 
    //The list of available values are provided here https://rainmaker.espressif.com/docs/time-service.html
    // RMaker.setTimeZone("Asia/Shanghai");
    // Alternatively, enable the Timezone service and let the phone apps set the appropriate timezone
    RMaker.enableTZService();
    RMaker.enableSchedule();

    //Service Name
    for(int i=0; i<17; i=i+8) {
      chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }

    Serial.printf("\nChip ID:  %d Service Name: %s\n", chipId, service_name);

    Serial.printf("\nStarting ESP-RainMaker\n");
    RMaker.start();

    WiFi.onEvent(sysProvEvent);
#if CONFIG_IDF_TARGET_ESP32
    WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, pop, service_name);
#else
    WiFiProv.beginProvision(WIFI_PROV_SCHEME_SOFTAP, WIFI_PROV_SCHEME_HANDLER_NONE, WIFI_PROV_SECURITY_1, pop, service_name);
#endif

    my_hall_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
    my_Kitchen_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
    my_bedroom_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
    my_lounge_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
    my_Garage_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
    my_Bathroom_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
    my_Basement_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
    my_Garden_Light.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);

}


void setup()
{
    Serial.begin(115200);

    Serial.println("Starting...");
    Serial.println("For factory Reset - Press RESET button for 10+ Sec");
    Serial.println("For WiFi Reset - Press RESET button for 3+ Sec");
    
    init_GPIO();

    init_RainMaker();

    mySwitch.enableReceive(34);  // RF Receiver on interrupt 0 => that is pin #34

}

void loop()
{
    // Read GPIO0 (external button to reset device
    if(digitalRead(gpio_reset) == LOW) { //Push button pressed
        Serial.printf("Reset Button Pressed!\n");
        // Handling function for Key debounce and Reset
        delay(100);
        int startTime = millis();
        while(digitalRead(gpio_reset) == LOW) delay(50);
        int endTime = millis();

        if ((endTime - startTime) > 10000) {
          // If key pressed for more than 10secs, reset all
          Serial.printf("Reset to factory.\n");
          RMakerFactoryReset(2);
        } else if ((endTime - startTime) > 3000) {
          Serial.printf("Reset Wi-Fi.\n");
          // If key pressed for more than 3secs, but less than 10, reset Wi-Fi
          RMakerWiFiReset(2);
        }
    }
    delay(100);

    if (WiFi.status() != WL_CONNECTED)
    {
      //Serial.println("WiFi Not Connected");
      digitalWrite(wifiLed, false);
    }
    else
    {
      //Serial.println("WiFi Connected");
      digitalWrite(wifiLed, true);
    }
    RF_remote();
    manual_control();
}

// ---------------------------------- make2explore.com----------------------------------------------------//