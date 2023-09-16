#include <esp_now.h>
#include <WiFi.h>
#include <esp_timer.h>

uint32_t time_m;


// Replace with the MAC address of the Slave ESP32
//34:94:54:D7:00:94
uint8_t slaveMacAddress[] = {0x34, 0x94, 0x54, 0xD7, 0x00, 0x94};

  // Callback function when data is received
void onDataReceived(const uint8_t *mac, const uint8_t *data, int len) {
  Serial.println("Data received from Slave: " + String((char *)data));
  
  // Respond to the Master
  //String response = "Hello from Slave ESP32!";
  //esp_err_t result = esp_now_send(maca, (uint8_t *)response.c_str(), response.length());

  //if (result == ESP_OK) {
    //Serial.println("Response sent to Master");
  //} else {
  //  Serial.println("Error sending response");
  //}
}




void setup() {
  Serial.begin(115200);

  // Set your ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else
  Serial.println("Initialized");



  // Register peer (Slave) using its MAC Address
  esp_now_peer_info_t peerInfo = {};
  //esp_now_peer_info_t peerInfo;
  peerInfo.ifidx = WIFI_IF_STA; // set other fields of peerInfo

  memcpy(peerInfo.peer_addr, slaveMacAddress, 6);
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
  // Data to be sent
  //String dataToSend = "Hello from Master ESP32!";

  time_m = esp_timer_get_time();
  
  // Send data to the Slave ESP32
  //esp_err_t result = esp_now_send(slaveMacAddress, (uint8_t *)dataToSend.c_str(), dataToSend.length());
  esp_err_t result = esp_now_send(slaveMacAddress, (uint8_t *)&time_m, sizeof(time_m));

  if (result == ESP_OK) {
    Serial.println("Data sent to Slave");
  } else {
    Serial.print("Error sending data: ");
    Serial.println(esp_err_to_name(result));
  }

  delayMicroseconds(100000); // Delay between sending data
}
