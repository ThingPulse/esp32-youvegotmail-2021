#include <Arduino.h>

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiMulti.h>

#include "config.h"

// declare external variable for the linker
extern const uint8_t rootCACertificate[] asm("_binary_src_rootCA_pem_start");
extern const uint8_t cat_start[] asm("_binary_src_cat_jpg_start");
extern const uint8_t cat_end[] asm("_binary_src_cat_jpg_end");

WiFiMulti wiFiMulti;

// ********* function declarations **************
void printPushoverLicenseInformation();
void sendPushoverMessage(String title, String message);
void sendPushoverMessageWithCat(String title, String message);
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
  // printPushoverLicenseInformation();
  // sendPushoverMessage("Workshoptage 2021", "POSTing from ESP32 😀");
  sendPushoverMessageWithCat("Workshoptage 2021", "POSTing a cat from ESP32 😀");
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

void sendPushoverMessage(String title, String message) {
  WiFiClientSecure *client = new WiFiClientSecure;
  if(client) {
    client -> setCACert((const char *)rootCACertificate);

    {
      HTTPClient https;
  
      log_i("[HTTPS] begin...");
      if (https.begin(*client, "https://api.pushover.net/1/messages.json")) {
        log_i("[HTTPS] POST...");
        https.addHeader("Content-Type", "application/json");
        
        StaticJsonDocument<250> doc;
        doc["token"] = apiToken;
        doc["user"] = userKey;
        doc["title"] = title;
        doc["message"] = message;

        String httpRequestData;
        serializeJson(doc, httpRequestData);

        int httpCode = https.POST(httpRequestData);
  
        if (httpCode > 0) {
          log_i("[HTTPS] POST... code: %d", httpCode);
  
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            log_i("%s", https.getString().c_str());
          }
        } else {
          log_i("[HTTPS] POST... failed, error: %s", https.errorToString(httpCode).c_str());
        }
  
        https.end();
      } else {
        log_i("[HTTPS] Unable to connect");
      }

    }
  
    delete client;
  } else {
    log_e("Unable to create client");
  }
}

void sendPushoverMessageWithCat(String title, String message) {
  WiFiClientSecure *client = new WiFiClientSecure;
  if(client) {
    client -> setCACert((const char *)rootCACertificate);

    {
      HTTPClient https;
  
      log_i("[HTTPS] begin...");
      if (https.begin(*client, "https://api.pushover.net/1/messages.json")) {
        log_i("[HTTPS] POST...");
        String boundary = "--47110815";
        String tokenPart = "--" + boundary  + "\r\nContent-Disposition: form-data; name=\"token\"\r\n\r\n" + apiToken  + "\r\n";
        String userPart =  "--" + boundary  + "\r\nContent-Disposition: form-data; name=\"user\"\r\n\r\n" + userKey  + "\r\n";
        String titlePart =  "--" + boundary  + "\r\nContent-Disposition: form-data; name=\"title\"\r\n\r\n" + title  + "\r\n";
        String messagePart = "--" + boundary  + "\r\nContent-Disposition: form-data; name=\"message\"\r\n\r\n" + message  + "\r\n";
        String attachmentPartHead = "--" + boundary + "\r\nContent-Disposition: form-data; name=\"attachment\"; filename=\"cat.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
        String attachmentPartTail = "\r\n--" + boundary + "--\r\n";

        int attachmentSize = cat_end - cat_start;
        log_i("attachment size is %d", attachmentSize);
        int attachmentPartLength = attachmentPartHead.length() +
                                   attachmentSize +
                                   attachmentPartTail.length();

        int contentLength = tokenPart.length() +
                            userPart.length() +
                            titlePart.length() +
                            messagePart.length() +
                            attachmentPartLength;

        https.addHeader("Content-Type", "multipart/form-data; boundary=" + boundary);
        https.addHeader("Content-Length", String(contentLength));

        uint8_t *payload = (uint8_t*) malloc(contentLength);
        memcpy(payload, tokenPart.c_str(), tokenPart.length());                                         int currentPayloadSize = tokenPart.length();
        memcpy(payload + currentPayloadSize, userPart.c_str(), userPart.length());                      currentPayloadSize += userPart.length();
        memcpy(payload + currentPayloadSize, titlePart.c_str(), titlePart.length());                    currentPayloadSize += titlePart.length();
        memcpy(payload + currentPayloadSize, messagePart.c_str(), messagePart.length());                currentPayloadSize += messagePart.length();
        memcpy(payload + currentPayloadSize, attachmentPartHead.c_str(), attachmentPartHead.length());  currentPayloadSize += attachmentPartHead.length();
        memcpy(payload + currentPayloadSize, cat_start, attachmentSize);                                currentPayloadSize += attachmentSize;
        memcpy(payload + currentPayloadSize, attachmentPartTail.c_str(), attachmentPartTail.length());  currentPayloadSize += attachmentPartTail.length();

        int httpCode = https.POST(payload, contentLength);
        free(payload);
  
        if (httpCode > 0) {
          log_i("[HTTPS] POST... code: %d", httpCode);
  
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            log_i("%s", https.getString().c_str());
          }
        } else {
          log_i("[HTTPS] POST... failed, error: %s", https.errorToString(httpCode).c_str());
        }
  
        https.end();
      } else {
        log_i("[HTTPS] Unable to connect");
      }

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
