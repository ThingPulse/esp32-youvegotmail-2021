#include <Arduino.h>

#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiMulti.h>

#include "config.h"

// declare external variable for the linker
extern const uint8_t rootCACertificate[] asm("_binary_src_rootCA_pem_start");

WiFiMulti wiFiMulti;

// ********* function declarations **************
void printPushoverLicenseInformation();
void setClock();
void startWiFi();
// ********* END function declarations **********


void setup() {
  log_v("Hello world: %d", millis());
  log_d("Free PSRAM: %d", ESP.getFreePsram());
  log_i("Free heap: %d", ESP.getFreeHeap());
  log_w("Total PSRAM: %d", ESP.getPsramSize());
  log_e("Total heap: %d", ESP.getHeapSize());

  startWiFi();
  setClock();
  printPushoverLicenseInformation();
}

void loop() {
  // put your main code here, to run repeatedly:
}


// ********* functions **************************
void printPushoverLicenseInformation() {
  WiFiClientSecure *client = new WiFiClientSecure;
  if(client) {
    client -> setCACert((const char *)rootCACertificate);

    {
      // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is 
      HTTPClient https;
  
      log_i("[HTTPS] begin...");
      if (https.begin(*client, "https://api.pushover.net/1/licenses.json?token=" + String(apiToken))) {  // HTTPS
        log_i("[HTTPS] GET...");
        // start connection and send HTTP header
        int httpCode = https.GET();
  
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been sent and server response header has been handled
          log_i("[HTTPS] GET... code: %d", httpCode);
  
          // resource found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            log_i("%s", https.getString().c_str());
          }
        } else {
          log_i("[HTTPS] GET... failed, error: %s", https.errorToString(httpCode).c_str());
        }
  
        https.end();
      } else {
        log_i("[HTTPS] Unable to connect");
      }

      // End extra scoping block
    }
  
    delete client;
  } else {
    log_e("Unable to create client");
  }
}

void setClock() {
  configTime(0, 0, "pool.ntp.org");

  log_i("Waiting for NTP time sync: ");
  time_t nowSecs = time(nullptr);
  while (nowSecs < 8 * 3600 * 2) {
    delay(500);
    log_i(".");
    yield();
    nowSecs = time(nullptr);
  }

  Serial.println();
  struct tm timeinfo;
  gmtime_r(&nowSecs, &timeinfo);
  log_i("Current time UTC: %s", asctime(&timeinfo));
}

void startWiFi() {
  WiFi.mode(WIFI_STA);
  wiFiMulti.addAP(ssid, wifipw);

  log_i("Waiting for WiFi to connect...");
  while ((wiFiMulti.run() != WL_CONNECTED)) {
    log_i(".");
  }
  log_i(" connected");
}
