/*
  Logger.cpp - Ornament Logger
  Michael Hotwagner
  mhotwagner@gmail.com
  Released into the public domain.
*/

#ifndef OfflineLogger_h
#define OfflineLogger_h

#include <ArduinoJson.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

class OfflineLogger {
  public:
  	OfflineLogger(String id, HardwareSerial* serialClient);
    OfflineLogger(String id, HardwareSerial* serialClient, int logLevel);

    void setApi(String apiHost);
    
    bool useApi();
    void useSerial();

    void log(String message);
    void log(int message);

    void log(String message, int logLevel);
    void log(int message, int logLevel);

    int ALL = 0;
    int DEBUG = 1;
    int INFO = 2;
    int WARN = 3;
    int ERROR = 4;

  private:
    bool testApi();

    void logSerial(String message);
    bool logApi(String message);

    bool _apiConfigured = false;
    bool _apiConnected = false;
    bool _usingApi = false;

    String _id;
    String _apiHost;
    HardwareSerial* _serialClient;

    int _logLevel;
};
#endif
