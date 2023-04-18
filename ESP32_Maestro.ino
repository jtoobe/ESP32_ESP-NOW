// MAESTRO (transmite datos al esclavo)

#include <esp_now.h>
#include <WiFi.h>

// MAC Address de la placa Esclava
uint8_t slaveAddress[] = {0x4C, 0x75, 0x25, 0x0C, 0x73, 0x55};
// Nota: el tipo de variable "uint8_t" solo admite valores de 0 a 255. En la instruccion se declara un array cuyos valores seran del tipo "uint8_t"

// Estructura de datos a transmitir, NO usar el tipo “String”
struct aEnviar {
  char palabra[6];
  int numero;
};
// Se crea una estructura llamada  “Datos”
aEnviar Datos;
String cadena;

// Devolución de llamada para tener un seguimiento de los mensajes enviados
void OnSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nStatus envio:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Enviado correctamente" : "Envio fallido");
}

void setup() {
  // Inicializo Serial Monitor
  Serial.begin(115200);

  // Seteo placa como estacion Wi-Fi
  WiFi.mode(WIFI_STA);
  // Inicializo ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }

  // Registro de la funcion de devolucion de llamada para responder al evento.
  esp_now_register_send_cb(OnSent);

  // Registro de la placa esclava
  esp_now_peer_info_t slaveInfo;
  memcpy(slaveInfo.peer_addr, slaveAddress, 6);
  slaveInfo.channel = 0;
  slaveInfo.encrypt = false;

  // Agrego placa esclava
  if (esp_now_add_peer(&slaveInfo) != ESP_OK) {
    Serial.println("Error registrando placa esclava");
    return;
  }
}

void loop() {
  // Cargo valores a enviar

  cadena = "39652";
  cadena.toCharArray(Datos.palabra, 6);
  Datos.numero = 5;

  //Serial.println(Datos.palabra);

  // Envio del mensaje via ESP-NOW
  esp_err_t result = esp_now_send(slaveAddress, (uint8_t *) &Datos, sizeof(Datos));

  if (result == ESP_OK) {
    Serial.println("Mensaje enviado correctamente.");
  }
  else {
    Serial.println("Error al enviar el mensaje.");
  }
  delay(2000);
}
