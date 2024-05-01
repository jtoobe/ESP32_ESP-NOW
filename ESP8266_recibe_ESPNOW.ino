#include <ESP8266WiFi.h>
#include <espnow.h>

// Estructura para los datos recibidos
typedef struct {
  char message[50];
  int value;
} Data;

Data receivedData;

// Función de callback que se ejecuta cuando se reciben datos
void onDataReceived(uint8_t *mac_addr, uint8_t *data, uint8_t len) {
  memcpy(&receivedData, data, sizeof(receivedData));
  Serial.println("Datos recibidos:");
  Serial.print("Mensaje: ");
  Serial.println(receivedData.message);
  Serial.print("Valor: ");
  Serial.println(receivedData.value);
}

void setup() {
  Serial.begin(115200);

  // Configurar el canal de transmisión (ejemplo: canal 1)
  wifi_set_channel(1);

  // Configurar el dispositivo como estación Wi-Fi
  WiFi.mode(WIFI_STA);

  // Inicializar ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error al inicializar ESP-NOW");
    return;
  }

  // Registrar la función de callback para la recepción de datos
  esp_now_register_recv_cb(onDataReceived);
}

void loop() {
  // No se realiza ninguna tarea adicional en el bucle principal
}
