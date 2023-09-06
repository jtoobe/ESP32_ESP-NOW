/*
* Para LOLIN D1 mini clone, FUNCIONA TAMBIEN CON esp01
* Envia datos cuando se conecta a la fuente
* Como esta lo hace cada 10 segundos, 
* lo tendria que hacer una vez sola  
*/
 
#include <ESP8266WiFi.h>
#include <espnow.h>
 
// REPLACE WITH RECEIVER MAC Address
// macaddress ESP01 pcb azul (original)
uint8_t broadcastAddress[] = {0x18, 0xFE, 0x34, 0xF3, 0x44, 0xE3};
// macaddress ESP8266 1 marca
//uint8_t broadcastAddress[] = {0x8C, 0xCE, 0x4E, 0xCE, 0x92, 0x23};
int cuenta=0;
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
char palabra[6];
  int numero;
} struct_message;
 
// Create a struct_message called myData
struct_message myData;
 
// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Mensaje enviado: ");
  if (sendStatus == 0){
    Serial.println("Ok ");
  }
  else{
    Serial.println("Fallo ");
  }
  cuenta++;
  Serial.print(cuenta);
}
  
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
 
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
 
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
   
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
  
void loop() {
  // Set values to send
  strcpy(myData.palabra, "39552");
  myData.numero =5;
   
  // Send message via ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
 
  delay(20000);
}
