#include <RHMesh.h>
#include <RH_RF95.h>
#include <SPI.h>

// Pines para ESP32 con RFM95
#define RFM95_CS 5          // Pin Chip Select para LoRa
#define RFM95_RST 14        // Pin Reset para LoRa  
#define RFM95_INT 2         // Pin de interrupción para LoRa
#define VSPI_SCK 18         // Pin SPI SCK
#define VSPI_MISO 19        // Pin SPI MISO
#define VSPI_MOSI 23        // Pin SPI MOSI
#define VSPI_SS 5           // Pin SPI SS

// Dirección del nodo cliente
#define CLIENT_ADDRESS 2

// Singleton instance of the radio driver
RH_RF95 driver(RFM95_CS, RFM95_INT);

// Class to manage message delivery and receipt, using the driver declared above
RHMesh manager(driver, CLIENT_ADDRESS);

void setup() 
{
  Serial.begin(115200);
  while (!Serial) ; // Wait for serial port to be available
  
  Serial.println("RHMesh Client Example");
  
  // Configurar el pin de RESET como OUTPUT
  pinMode(RFM95_RST, OUTPUT);
  
  // Inicializar SPI
  SPI.begin(VSPI_SCK, VSPI_MISO, VSPI_MOSI, VSPI_SS);
  
  // Inicializar el módulo RFM95
  while (!manager.init()) {
    Serial.println("RF95 MESH init failed");
    // Baja el pin RST para poner el módulo en reset
    digitalWrite(RFM95_RST, LOW);
    delay(10); // Mantenerlo bajo por un corto tiempo
    // Sube el pin RST para liberar el módulo del reset
    digitalWrite(RFM95_RST, HIGH);
    delay(10); // Darle tiempo al módulo para arrancar
    delay(100);
  }
  
  Serial.println("RF95 MESH init OK");
  
  
  Serial.println("Cliente mesh listo");
}

void loop() 
{
  // Dirección del servidor (gateway)
  uint8_t serverAddress = 1;
  
  // Mensaje de prueba
  char data[] = "Hola desde cliente mesh!";
  
  Serial.print("Enviando mensaje a servidor: ");
  Serial.println(data);
  
  // Enviar mensaje al servidor
  if (manager.sendtoWait((uint8_t*)data, sizeof(data), serverAddress) == RH_ROUTER_ERROR_NONE) {
    Serial.println("Mensaje enviado exitosamente");
  } else {
    Serial.println("Error al enviar mensaje");
  }
  
  // Esperar respuesta del servidor
  uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  uint8_t from;
  
  Serial.println("Esperando respuesta...");
  
  if (manager.recvfromAckTimeout(buf, &len, 3000, &from)) {
    Serial.print("Respuesta recibida desde nodo: ");
    Serial.println(from);
    Serial.print("Datos: ");
    Serial.println((char*)buf);
  } else {
    Serial.println("No se recibió respuesta");
  }
  
  delay(5000); // Esperar 5 segundos antes del siguiente envío
} 