/*
 * Para que el ESP32 emita un mensaje random 
 * 
 * 
 * 
 */



#include <esp_now.h>
#include <WiFi.h>

// REEMPLAZAR CON LA DIRECCIÓN MAC DEL RECEPTOR
uint8_t broadcastAddress[] = {0x84,0xF3,0xEB,0x0C,0xD6,0xA6};

// Ejemplo de estructura para enviar datos
// Debe coincidir con la estructura del receptor IMPORTANTE !!!!!
typedef struct Estruct {
    int id; // debe ser único para cada placa emisora
    int x;
    int y;
} Estruct;

// Crear una Estruct llamada Data
Estruct Data;

// Crear información de par entre pares
esp_now_peer_info_t peerInfo;

// Callback cuando se envían datos
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nÚltimo Estado de Envío del Paquete:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Entrega Exitosa" : "Fallo en la Entrega");
}
 
void setup() {
  // Inicializar Monitor Serie
  Serial.begin(115200);

  // Configurar el dispositivo como una Estación Wi-Fi
  WiFi.mode(WIFI_STA);

  // Inicializar ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }

  // Una vez que ESPNow está inicializado correctamente, nos registraremos para el CB de Envío para
  // obtener el estado del paquete transmitido
  esp_now_register_send_cb(OnDataSent);
  
  // Registrar par
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  // ojo con el canal al transmitir a un ESP8266
  peerInfo.encrypt = false;
  
  // Agregar par        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Error al agregar par");
    return;
  }
}
 
void loop() {
  // Establecer valores para enviar
  Data.id = 1;
  Data.x = random(0,50);
  Data.y = random(0,50);

  // Enviar mensaje a través de ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &Data, sizeof(Data));
   
  if (result == ESP_OK) {
    Serial.println("Enviado con éxito");
  }
  else {
    Serial.println("Error al enviar los datos");
  }
  delay(2000);
}
