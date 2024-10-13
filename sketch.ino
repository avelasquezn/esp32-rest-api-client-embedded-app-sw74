#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

// WiFi Credentials
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

// Endpoint URL
#define ENDPOINT_URL "https://iot-edge-sample-sw74.free.beeceptor.com/api/v1/data-records"

// HTTP Client
HTTPClient httpClient;

// HTTP Header Parameter and Value
#define CONTENT_TYPE_HEADER "Content-Type"
#define APPLICATION_JSON "application/json"

// Device Identification
#define DEVICE_ID  "HC001"


void setup() {
  // Initialize Serial Output
  Serial.begin(115200);
  
  // WiFi setup
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  


  // Post Request Sample

  // Start HTTP Request
  httpClient.begin(ENDPOINT_URL);
  
  // Data Record Build
  JsonDocument dataRecord;
  dataRecord["deviceId"] = DEVICE_ID;
  dataRecord["distance"] = 250;

  // Data Record Serialization
  String dataRecordResource;
  serializeJson(dataRecord, dataRecordResource);

  // Data Record Posting
  httpClient.addHeader(CONTENT_TYPE_HEADER, APPLICATION_JSON);
  httpClient.POST(dataRecordResource);

  // Check Response
  JsonDocument response;
  String responseResource;
  responseResource = httpClient.getString();
  deserializeJson(response, responseResource);
  serializeJsonPretty(response, Serial);
  
  // Manipulate Response Data
  Serial.println();
  Serial.print("Distance: ");
  Serial.println(response["distance"].as<long>());

  // End HTTP Request
  httpClient.end();

  // Get All Request Sample

  // Start HTTP Request
  httpClient.begin(ENDPOINT_URL);

  // Get Data Records
  int httpResponseCode = httpClient.GET();
  Serial.print("Response code: ");
  Serial.println(httpResponseCode);

  responseResource = httpClient.getString();
  deserializeJson(response, responseResource);
  serializeJsonPretty(response, Serial);

  // Manipulate Data Records
  Serial.println("About to show ids");
  JsonArray dataRecords = response.as<JsonArray>();
  if(dataRecords != NULL) {
    for(JsonVariant dataRecord : dataRecords) {
      Serial.print("id: ");
      Serial.println(dataRecord["id"].as<String>());
    }
  }

  // End HTTP Request
  httpClient.end();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10); // this speeds up the simulation
}
