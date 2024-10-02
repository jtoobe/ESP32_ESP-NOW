#include <WiFi.h>
#include <esp_now.h>

// Dirección MAC del ESP8266 (reemplazar con la real)
uint8_t mac_esp8266[] = {0x84,0xF3,0xEB,0x0C,0xD6,0xA6};

// Estructura para enviar y recibir datos
typedef struct struct_message {
  char palabra[6];
  int numero;
} struct_message;

struct_message myData;

// Función que se ejecuta cuando se reciben datos
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Recibido desde ESP8266 -> Palabra: ");
  Serial.println(myData.palabra);
  Serial.print("Número: ");
  Serial.println(myData.numero);
}

// Función que se ejecuta cuando se envían datos
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Datos enviados al ESP8266 -> Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Enviado correctamente" : "Falló el envío");
}

void setup() {
  Serial.begin(115200);

  // Configura el ESP32 en modo Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Inicializa ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al inicializar ESP-NOW");
    return;
  }

  // Registra la función de envío y recepción de datos
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);

  // Añade el peer ESP8266
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, mac_esp8266, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Error al añadir el peer ESP8266");
    return;
  }
}

void loop() {
  // Envía datos cada 5 segundos
  strcpy(myData.palabra, "Mundo");
  myData.numero = random(100, 200);
  
  esp_now_send(mac_esp8266, (uint8_t *) &myData, sizeof(myData));
  delay(5000);
}
