#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>

#include "config.h"

WiFiMulti wiFiMulti;

void startWiFi() {
  WiFi.mode(WIFI_STA);
  wiFiMulti.addAP(ssid, wifipw);

  log_i("Waiting for WiFi to connect...");
  while ((wiFiMulti.run() != WL_CONNECTED)) {
    log_i(".");
  }
  log_i(" connected");
}

void setup() {
  log_v("Hello world: %d", millis());
  log_d("Free PSRAM: %d", ESP.getFreePsram());
  log_i("Free heap: %d", ESP.getFreeHeap());
  log_w("Total PSRAM: %d", ESP.getPsramSize());
  log_e("Total heap: %d", ESP.getHeapSize());

  startWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
}
