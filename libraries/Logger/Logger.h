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
    Logger(String id, HTTPClient* apiClient, HardwareSerial* serialClient, int logLevel);
    Logger(String id, HTTPClient* apiClient, HardwareSerial* serialClient, int apiLogLevel, int serialLogLevel);
    Logger(String id, HTTPClient* apiClient, HardwareSerial* serialClient, int apiLogLevel, int serialLogLevel, int defaultLogLevel);

    void setApi(String apiHost);
    
    bool useApi();
    void useSerial();

    void log(String message);
    void log(int message);
    void log(long message);
    void log(double message);
    void log(IPAddress message);
    void log(size_t message);
    void log(long unsigned int message);

    void log(String message, int logLevel);
    void log(int message, int logLevel);
    void log(long message, int logLevel);
    void log(double message, int logLevel);
    void log(IPAddress message, int logLevel);
    void log(size_t message, int logLevel);
    void log(long unsigned int message, int logLevel);

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
    HTTPClient* _apiClient;
    HardwareSerial* _serialClient;

    int _apiLogLevel;
    int _serialLogLevel;
    int _defaultLogLevel;
};
#endif
