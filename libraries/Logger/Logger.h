/*
  Logger.cpp - Ornament Logger
  Michael Hotwagner
  mhotwagner@gmail.com
  Released into the public domain.
*/

#ifndef Logger_h
#define Logger_h

#include <ArduinoJson.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

class Logger {
  public:
  	Logger(String id, HTTPClient* apiClient, HardwareSerial* serialClient);

    void setApi(String apiHost);
    
    bool useApi();
    void useSerial();

    void log(String message);
    void log(int message);

  private:
    bool testApi();

    void logSerial(String message);
    bool logApi(String message);

    bool _apiConfigured = false;
    bool _apiConnected = false;
    bool _usingApi = false;

    String _id;
    String _apiHost;
    HTTPClient* _apiClient;
    HardwareSerial* _serialClient;

};
#endif
