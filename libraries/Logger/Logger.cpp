/*
  Logger.cpp - Ornament Logger
  Michael Hotwagner
  mhotwagner@gmail.com
  Released into the public domain.
*/

#include <Arduino.h>
#include "Logger.h"

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

Logger::Logger(String id, HTTPClient* apiClient, HardwareSerial* serialClient) {
	_id = id;
	_apiClient = apiClient;
	_serialClient = serialClient;
	_serialLogLevel = DEBUG;
	_apiLogLevel = ERROR;
	_defaultLogLevel = INFO;
}

Logger::Logger(String id, HTTPClient* apiClient, HardwareSerial* serialClient, int apiLogLevel) {
	_id = id;
	_apiClient = apiClient;
	_serialClient = serialClient;
	_apiLogLevel = apiLogLevel;
	_serialLogLevel = DEBUG;
	_defaultLogLevel = INFO;
}

Logger::Logger(String id, HTTPClient* apiClient, HardwareSerial* serialClient, int apiLogLevel, int serialLogLevel) {
	_id = id;
	_apiClient = apiClient;
	_serialClient = serialClient;
	_apiLogLevel = apiLogLevel;
	_serialLogLevel = serialLogLevel;
	_defaultLogLevel = INFO;
}

Logger::Logger(String id, HTTPClient* apiClient, HardwareSerial* serialClient, int apiLogLevel, int serialLogLevel, int defaultLogLevel) {
	_id = id;
	_apiClient = apiClient;
	_serialClient = serialClient;
	_apiLogLevel = apiLogLevel;
	_serialLogLevel = serialLogLevel;
	_defaultLogLevel = defaultLogLevel;
}

bool Logger::testApi() {
	logSerial("[Logger] Testing api");
	_apiClient->begin("http://" + _apiHost + "/api/status/");
	int status = _apiClient->GET();
	_apiClient->end();
	logSerial(String(status));
	logSerial("[Logger] Done testing api");
	if (status == 200) return true;
	return false;
}

void Logger::setApi(String apiHost) {
	_apiHost = apiHost;
	_apiConfigured = true;
	_apiConnected = testApi();
	if (_apiConnected) {
		logSerial("[Logger] Connected");
		logApi("[Logger] Connected");
		_usingApi = true;
	} else {
		logSerial("[Logger] Failed to connect");
	}
}

bool Logger::useApi() {
	if (_apiConfigured && _apiConnected) {
		_usingApi = true;
	} else if (_apiConfigured) {
		_apiConnected = _usingApi = testApi();
	}
}
void Logger::useSerial() {
	_usingApi = false;
}

void Logger::logSerial(String message) {
	_serialClient->println("\n" + message);
}
bool Logger::logApi(String message) {
	DynamicJsonDocument doc(256);
	doc["message"] = message;
	doc["ornament_mac_address"] = WiFi.macAddress();
	_apiClient->begin("http://" + _apiHost + "/api/logs/");
	_apiClient->addHeader("Content-Type", "application/json");
	
	String content;
	serializeJson(doc, content);

	int status = _apiClient->POST(content);
	_apiClient->end();
	if (status == 201) return true;

	logSerial("[LOGGER] Error, failed to log to api: " + status);
	return false;
}

void Logger::log(String message, int logLevel) {
	if (_usingApi && logLevel >= _apiLogLevel) logApi(message);
	if (logLevel >= _serialLogLevel) logSerial(message);
}

void Logger::log(String message) {
	log(message, _defaultLogLevel);
}

void Logger::log(int message, int logLevel) {
	log(String(message), logLevel);
}

void Logger::log(int message) {
	log(String(message), _defaultLogLevel);
}

void Logger::log(long message, int logLevel) {
	log(String(message), logLevel);
}

void Logger::log(long message) {
	log(String(message), _defaultLogLevel);
}

void Logger::log(double message, int logLevel) {
	log(String(message), logLevel);
}

void Logger::log(double message) {
	log(String(message), _defaultLogLevel);
}

void Logger::log(IPAddress message, int logLevel) {
  String stringified = String(message[0]) + String(".") +\
  String(message[1]) + String(".") +\
  String(message[2]) + String(".") +\
  String(message[3]);
  log(stringified, logLevel);
}

void Logger::log(IPAddress message) {
  String stringified = String(message[0]) + String(".") +\
  String(message[1]) + String(".") +\
  String(message[2]) + String(".") +\
  String(message[3]);
  log(stringified, _defaultLogLevel);
}

void Logger::log(size_t message, int logLevel) {
	log(String(message), logLevel);
}

void Logger::log(size_t message) {
	log(String(message), _defaultLogLevel) ;
}

void Logger::log(long unsigned int message, int logLevel) {
	log(String(message), logLevel);
}

void Logger::log(long unsigned int message) {
	log(String(message), _defaultLogLevel);
}