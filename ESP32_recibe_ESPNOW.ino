// ESP32 recibe datos vis ESP-NOW

#include <WiFi.h>
#include <esp_now.h>

// Estructura para recibir datos
typedef struct struct_message {
  char palabra[6];
  int numero;
} struct_message;

// Variable para almacenar los datos recibidos
struct_message myData;

// Función que se ejecuta cuando se reciben los datos
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  // Copia los datos recibidos en la estructura myData
  memcpy(&myData, incomingData, sizeof(myData));
  
  // Mostrar la dirección MAC del emisor (si es necesario)
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X", 
           info->src_addr[0], info->src_addr[1], info->src_addr[2], 
           info->src_addr[3], info->src_addr[4], info->src_addr[5]);
  Serial.print("Datos recibidos de: ");
  Serial.println(macStr);

  // Mostrar los datos recibidos
  Serial.print("Palabra: ");
  Serial.println(myData.palabra);
  Serial.print("Número: ");
  Serial.println(myData.numero);
}

void setup() {
  // Inicializa la comunicación serial
  Serial.begin(115200);

  // Configura el ESP32 en modo Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Inicializa ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al inicializar ESP-NOW");
    return;
  }

  // Registra la función de recepción de datos
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // No se requiere código en el loop
}

