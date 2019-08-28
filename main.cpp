/*
 * 7-31-2019 
 * 
 *    this works with simple_wifi android app, as does sketch simple_wifi. the Android device wifi has to be connected to the esp, and mine 
 *    tends to go back to the router when I upload this sketch. If I disabled the wifi router in the Android
 *    settings that might stop happening.
 *  
 * 
 * 8-1-2019
 * 
 *    one point that confused me until I looked at https://github.com/espressif/arduino-esp32/issues/1393  
 *    
 *    the default behavior is for the esp to save the last ssid/password in non-volatile storage, across power cycles, resets, new sketches, 
 *    whatever. so, when I use simple_wifi app and scan the range that my router assigned to the esp, it will find that IP even if this sketch 
 *    does not include that ssid/password or IP. There's another concern which might be overblown where some people say it was wearing out the 
 *    flash quickly by writing the ssid/password on every boot, so whether or not that's a valid concern, I have included WiFi.persistent(false)
 * 
 */

#include "server_loop.h"
#include "parse.h"

uint32_t rgbcycles;

// SSID and password for esp AP. make up whatever you like.
const char * ssidAP = "rgb_wifi_88";
const char * passwordAP = "7123954ABC@#";


#define one_sec_led LED_BUILTIN  //D4    // builtin led on nodemcu

int wifistatus;

#define OUT_BUFFER_SIZE 6u

char Out_Buffer[OUT_BUFFER_SIZE] = { 0 };

char In_Buffer;

int ledcnt = 0;


//----------------------------------------------------------------------------------------------------------------------


void setup() {
  
  pinMode(one_sec_led, OUTPUT);

  #ifdef serial1
  Serial.begin(115200);
  Serial.println();
  #endif

  WiFi.persistent(false); // according to readthedocs, the flash can wear out if I call WiFi.begin(ssid,pwd) too many times. setting persistent = false is supposed to make it use whatever is already there and not re-write it. so, I think
                          // that I can leave this alone in every sketch as long as I'm using the same router ssid and pwd. I think it also writes to flash for WiFi.softAP() too.

  
  WiFi.softAP("hello");

//  WiFi.softAP(); // this function is not defined, so I have to send the ssid/pwd every time. hopefully the persistent(false) works

  WiFi.softAP(ssidAP,passwordAP);  

  #ifdef serial1  
  
  IPAddress myIP = WiFi.softAPIP();
  
  Serial.print(" ap ip ");
  Serial.print(myIP);
  #endif
  
  server.begin(); 
}





//-----------------------------------------------------------------------------


uint32_t ccnt;


void loop() {

  ccnt = ESP.getCycleCount();   // 2^32 / 80e6 = 53.7 seconds to rollover, esp clock is 80 mhz, so there are 80e6 cycles per second, and the 
                                //count is stored in 32 bits. 

  uptime_minutes = millis() / 60000;  // this tells the app how long the esp has been running without a reset or power cycle.
                                      // millis() rolls over every 49.7 days i think: 2^32 / 1000 / 60 / 60 / 24 = 49.7 days to rollover

  server_loop();    // check for clients connecting

  if ((ccnt - rgbcycles) > 250000){  // this flashes the built-in led about once per second. one half second is about 0.5 / 160 / 80e6 = 250000

    // when getCycleCount rolls over, ccnt - rgbcycles will be negative, but I think the unsigned math results in a large unsigned value which 
    // enables this loop to handle the rollover with an error less than 3.2 msec
    
      ledcnt++;
  
      if (ledcnt == 160){
  
        digitalWrite(one_sec_led,LOW);
        
      } else if (ledcnt == 320){
  
        ledcnt = 0;
  
        digitalWrite(one_sec_led,HIGH);
      }
  
    rgbcycles = ccnt;
  }
}
