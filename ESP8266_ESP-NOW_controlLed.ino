/*
El 8266 recibe un mensaje (cualquiera) de un ESP32 y enciende un led por X segundos
incluye un control del tiempo que esta encendido el led
para evitar los "Soft WDT reset"

*/



#include <ESP8266WiFi.h>
#include <espnow.h>

int LED_PIN = 5;

struct aRecibir {
  char palabra[6];
  int numero;
};
// crea una estructura para recibir los datos
aRecibir Datos;

// Variable para almacenar el estado del LED
bool ledEncendido = false;

// Variables para el control de tiempo
unsigned long tiempoInicio = 0;
const unsigned long DURACION_ENCENDIDO = 5000; // Duración de encendido en milisegundos (5 segundos)

// Función para encender o apagar el LED
void controlarLED(bool encender) {
  digitalWrite(LED_PIN, encender ? HIGH : LOW);
  ledEncendido = encender;
}

// Callback ejecutado cuando se recibe un mensaje
void OnRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  // Copiar los datos recibidos a la estructura
  memcpy(&Datos, incomingData, sizeof(Datos));

  // Mostrar información recibida en el monitor serie
  Serial.print("Bytes recibidos: ");
  Serial.println(len);
  Serial.print("Palabra recibida: ");
  Serial.println(Datos.palabra);
  Serial.print("Número recibido: ");
  Serial.println(Datos.numero);

  // Encender el LED cuando se recibe un mensaje
  controlarLED(true);
  tiempoInicio = millis(); // Registrar el tiempo de inicio
}

void setup() {
  // Inicializar el monitor serie
  Serial.begin(115200);

  // Configurar el modo y el pin del LED
  pinMode(LED_PIN, OUTPUT);
  controlarLED(false);

  // Establecer el dispositivo como una estación Wi-Fi
  WiFi.mode(WIFI_STA);

  // Inicializar ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error al inicializar ESP-NOW");
    return;
  }

  // Registrar la función de callback para recibir datos
  esp_now_register_recv_cb(OnRecv);
}

void loop() {
  // Verificar si se excedió la duración de encendido del LED
  if (ledEncendido && (millis() - tiempoInicio >= DURACION_ENCENDIDO)) {
    controlarLED(false); // Apagar el LED
  }

  // No se realiza ninguna tarea adicional en el bucle principal
}
