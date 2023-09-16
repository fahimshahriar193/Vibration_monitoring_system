#include <esp_now.h>
#include <WiFi.h>
uint8_t maca[] = {0xC4, 0x5B, 0xBE, 0x30, 0x0B, 0x0C}; 
uint32_t time_s;
uint32_t time_r;
int32_t  diff1;


// Callback function when data is received
void onDataReceived(const uint8_t *mac, const uint8_t *data, int len) {
  //Serial.println("Data received from Master: " + String((char *)data));
  time_r = *(uint32_t *)data;  // received data to a different variable
  time_s = esp_timer_get_time();
  diff1 = time_r - time_s; // difference between master and slave is measured that can be used to correct the slave timestamp
  //Serial.println(diff1);
  Serial.print(time_r);
  Serial.print(" ");
  Serial.print(time_s);
  Serial.print(" ");
  Serial.println(diff1);
  
  // Respond to the Master
  String response = "Hello from Slave ESP32!";
  esp_err_t result = esp_now_send(maca, (uint8_t *)response.c_str(), response.length());

  if (result == ESP_OK) {
    //Serial.println("Response sent to Master");
  } else {
    Serial.println("Error sending response");
  }
}

void setup() {
  Serial.begin(9600);

  // Set your ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer (Slave) using its MAC Address
  esp_now_peer_info_t peerInfo = {};
  //esp_now_peer_info_t peerInfo;
  peerInfo.ifidx = WIFI_IF_STA; // set other fields of peerInfo

  memcpy(peerInfo.peer_addr, maca, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  else
  Serial.println("Peer added");



  // Register callback function for data reception
  esp_now_register_recv_cb(onDataReceived);
}

void loop() {
  // Additional code or tasks can be placed here
}
