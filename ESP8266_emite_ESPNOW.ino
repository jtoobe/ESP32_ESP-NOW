/*
* Para LOLIN D1 mini clone, FUNCIONA TAMBIEN CON esp01
* Envia un dato cada 2 segundos
*/
 
#include <ESP8266WiFi.h>
#include <espnow.h>
 
// Reemplazar con la MAC Address del ESP que recibe
uint8_t broadcastAddress[] = {0xD4,0x8A,0xFC,0xD0,0x8C,0xF8};

int cuenta=0;
// Creo una estructura para enviar los datos
// esta debe estar replicada en el receptor
typedef struct struct_message {
char palabra[6];
  int numero;
} struct_message;
 
// Creo la estructura para enviar los datos
struct_message myData;
 
// Funcion que se ejecuta para enviar el dato
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
  
  Serial.begin(115200);
  
  // Defino el funcionamiento del ESP01 como Wi-Fi Station
  WiFi.mode(WIFI_STA);
 
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }
 
  
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
   
  // Registro el receptor
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
  
void loop() {
  // Valores a enviar
  strcpy(myData.palabra, "39552");
  myData.numero =5;
   
  // envio el mensaje via ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
 
  delay(2000);
}
