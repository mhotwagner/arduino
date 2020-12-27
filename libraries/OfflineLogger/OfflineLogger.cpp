/*
  Logger.cpp - Ornament Logger
  Michael Hotwagner
  mhotwagner@gmail.com
  Released into the public domain.
*/

#include <Arduino.h>
#include "OfflineLogger.h"

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

OfflineLogger::OfflineLogger(String id, HardwareSerial* serialClient) {
	_id = id;
	_serialClient = serialClient;
	_logLevel = DEBUG;
}

OfflineLogger::OfflineLogger(String id, HardwareSerial* serialClient, int logLevel) {
	_id = id;
	_serialClient = serialClient;
	_logLevel = logLevel;
}

bool OfflineLogger::testApi() {
	return false;
}

void OfflineLogger::setApi(String apiHost) {
	_apiHost = apiHost;
	logSerial("[Logger] Failed to connect");
}

bool OfflineLogger::useApi() {
	logSerial("[Logger] Failed to connect");
}
void OfflineLogger::useSerial() {
	_usingApi = false;
}

void OfflineLogger::logSerial(String message) {
	_serialClient->println("\n" + message);
}

void OfflineLogger::log(String message) {
	logSerial(message);
}

void OfflineLogger::log(String message, int logLevel) {
	if (logLevel > _logLevel) log(message);
}

void OfflineLogger::log(int message) {
	log(String(message));
}

void OfflineLogger::log(int message, int logLevel) {
	if(logLevel > _logLevel) log(String(message));
}

