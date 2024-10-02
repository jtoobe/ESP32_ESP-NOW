#include <ESP8266WiFi.h>
#include <espnow.h>

// Dirección MAC del ESP32 (reemplazar con la real)
uint8_t mac_esp32[] = {0xD4,0x8A,0xFC,0xD0,0x8C,0xF8};

// Estructura para enviar y recibir datos
typedef struct struct_message {
  char palabra[6];
  int numero;
} struct_message;

struct_message myData;

// Función que se ejecuta cuando se reciben datos
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Recibido desde ESP32 -> Palabra: ");
  Serial.println(myData.palabra);
  Serial.print("Número: ");
  Serial.println(myData.numero);
}

// Función que se ejecuta cuando se envían datos
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Datos enviados al ESP32 -> Status: ");
  Serial.println(sendStatus == 0 ? "Enviado correctamente" : "Falló el envío");
}

void setup() {
  Serial.begin(115200);

  // Configura el ESP8266 como estación Wi-Fi
  WiFi.mode(WIFI_STA);

  // Inicializa ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }

  // Registra la función de envío y recepción de datos
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);

  // Añade el peer ESP32
  esp_now_add_peer(mac_esp32, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
}

void loop() {
  // Envía datos cada 5 segundos
  strcpy(myData.palabra, "Hola");
  myData.numero = random(0, 100);
  
  esp_now_send(mac_esp32, (uint8_t *) &myData, sizeof(myData));
  delay(5000);
}
