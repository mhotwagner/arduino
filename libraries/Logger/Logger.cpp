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

	logSerial("[ERROR] Failed to log to api: " + status);
	logSerial(message);
	return false;
}

void Logger::log(int message) {
	log(String(message));
}

void Logger::log(String message) {
	if (_usingApi) {
		if (!logApi(message)) logSerial(message);
	} else {
		logSerial(message);
	}
}
