/**
 * @file node_identity.cpp
 * @brief Implementación de identidad de nodo con persistencia EEPROM
 * @date 22:00 17/6/2025
 */

#include "node_identity.h"

// Definición de la lista de blacklist por defecto.
const uint8_t NodeIdentity::defaultBlacklist[2] = {0x00, 0xFF};

NodeIdentity::NodeIdentity()
{
    Serial.print("Inicializando ");
    //begin();
}

uint8_t NodeIdentity::getNodeID(size_t blacklist_len, const uint8_t *blacklist)
{
    Serial.println("--- NodeIdentity::getNodeID() INICIO ---");
    Serial.print("Parámetros recibidos: blacklist_len = ");
    Serial.println(String(blacklist_len));
    // No imprimimos el 'blacklist' completo aquí, ya que puede ser largo.
    // Si necesitas depurarlo, puedes iterar sobre él y Serial.println() cada byte.

    uint8_t storedHash = HASH_NOT_SET;
    Serial.print("Intentando cargar NODE_ID desde el archivo '");
    Serial.print(NODE_ID_FILE);
    Serial.print("'. Valor inicial: 0x");
    Serial.println(String(storedHash, HEX));

    // Llama a la función para cargar el byte desde el archivo
    loadByteFromFile(NODE_ID_FILE, storedHash);

    Serial.print("Valor cargado desde archivo: 0x");
    Serial.println(String(storedHash, HEX));
    Serial.print("HASH_NOT_SET es: 0x");
    Serial.println(String(HASH_NOT_SET, HEX));

    if (storedHash != HASH_NOT_SET && storedHash != 0)
    {
        Serial.println("HASH encontrado en el archivo (no es HASH_NOT_SET).");
        Serial.print("Devolviendo ID almacenado: 0x");
        Serial.println(String(storedHash, HEX));
        Serial.println("--- NodeIdentity::getNodeID() FIN (ID existente) ---");
        return storedHash; // Si hay un hash guardado, lo devuelve
    }

    Serial.println("No hay HASH almacenado o es HASH_NOT_SET. Generando nuevo ID...");

    String mac = getDeviceMAC();
    Serial.print("MAC del dispositivo obtenida: ");
    Serial.println(mac);

    uint8_t macBytes[6];
    int values[6];

    // Intenta parsear la MAC del string a valores hexadecimales
    int sscanfResult = sscanf(mac.c_str(), "%x:%x:%x:%x:%x:%x",
                              &values[0], &values[1], &values[2],
                              &values[3], &values[4], &values[5]);

    Serial.print("sscanf(): Numero de items convertidos: ");
    Serial.println(String(sscanfResult));

    // Verifica si sscanf fue exitoso (debe convertir 6 valores)
    if (sscanfResult != 6)
    {
        Serial.println("ERROR: Fallo al parsear la MAC. Se generara un hash con MAC invalida.");
        // Considera aquí un manejo de error más robusto, como retornar un ID por defecto o reiniciar.
    }
    else
    {
        Serial.print("Bytes MAC parseados (HEX): ");
        for (int i = 0; i < 6; ++i)
        {
            macBytes[i] = (uint8_t)values[i];
            Serial.print(String(macBytes[i], HEX));
            Serial.print(" ");
        }
        Serial.println(""); // Nueva línea después de imprimir los bytes
    }

    uint8_t hash = generateSafeHash(macBytes, 6, blacklist, blacklist_len);

    Serial.print("Hash generado: 0x");
    Serial.println(String(hash, HEX));

    // Guarda el nuevo hash generado en el archivo
    saveByteToFile(NODE_ID_FILE, hash);
    Serial.print("Nuevo ID (0x");
    Serial.print(String(hash, HEX));
    Serial.println(") guardado en el archivo.");

    Serial.println("--- NodeIdentity::getNodeID() FIN (Nuevo ID generado) ---");
    return hash; // Devuelve el hash recién generado
}

uint8_t NodeIdentity::changeNodeID(const size_t blacklist_len, uint8_t *blacklist)
{
    String mac = getDeviceMAC();
    uint8_t mac_bytes[6];
    int values[6];
    sscanf(mac.c_str(), "%x:%x:%x:%x:%x:%x",
           &values[0], &values[1], &values[2],
           &values[3], &values[4], &values[5]);

    for (int i = 0; i < 6; ++i)
    {
        mac_bytes[i] = (uint8_t)values[i];
    }

    uint8_t hash = generateSafeHash(mac_bytes, 6, blacklist, blacklist_len);

    saveByteToFile(NODE_ID_FILE, hash);
    return hash;
}

// Carga un solo byte desde un archivo de LittleFS
bool NodeIdentity::loadByteFromFile(const char *filename, uint8_t &value)
{
    return true;/*File file = LittleFS.open(filename, "r");
    if (!file)
    {
        // Serial.printf("NodeIdentity: Archivo '%s' no encontrado.\n", filename);
        return false;
    }

    if (file.available())
    {
        value = file.read(); // Lee el primer byte
        file.close();
        // Serial.printf("NodeIdentity: Valor %d cargado desde '%s'.\n", value, filename);
        return true;
    }
    // Serial.printf("NodeIdentity: Archivo '%s' vacío.\n", filename);
    file.close();
    return false;*/
}

// Guarda un solo byte en un archivo de LittleFS
void NodeIdentity::saveByteToFile(const char *filename, uint8_t value)
{/*
    File file = LittleFS.open(filename, "w"); // "w" sobrescribe el archivo existente
    if (!file)
    {
        // Serial.printf("NodeIdentity: Fallo al abrir archivo '%s' para escritura.\n", filename);
        return;
    }

    file.write(value); // Escribe el byte
    file.close();      // Cierra el archivo para asegurar que se guarde
*/
    // Serial.printf("NodeIdentity: Valor %d guardado en '%s'.\n", value, filename);
}

uint8_t NodeIdentity::generateSafeHash(
    const uint8_t *data,
    size_t len,
    const uint8_t *blacklist,
    size_t blacklist_len)
{
    uint8_t hash = crc8(data, len);
    bool conflict = true;

    while (conflict)
    {
        conflict = false;
        for (size_t i = 0; i < blacklist_len; ++i)
        {
            if (hash == blacklist[i] || hash == 255) // valor prohibido prevee errores de faltar pasarlo en blacklist
            {
                hash = (hash + 1) % 256;
                conflict = true;
                break;
            }
        }
    }
    return hash;
}

uint8_t NodeIdentity::crc8(const uint8_t *data, size_t len)
{
    uint8_t crc = 0x00;
    for (size_t i = 0; i < len; ++i)
    {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; ++j)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x07;
            else
                crc <<= 1;
        }
    }
    return crc;
}

String NodeIdentity::getDeviceMAC()
{
  byte mac[6];
  WiFi.macAddress(mac); // Obtiene la dirección MAC en un array de bytes

  String macAddressString = "";
  for (int i = 0; i < 6; ++i) {
    if (mac[i] < 0x10) { // Añade un '0' al inicio si el byte es menor que 16 (0x10)
      macAddressString += "0";
    }
    macAddressString += String(mac[i], HEX); // Convierte el byte a hexadecimal
    if (i < 5) {
      macAddressString += ":"; // Añade dos puntos entre los bytes
    }
  }
  return macAddressString;
}

bool NodeIdentity::getGetway(uint8_t &getwayAdress)
{
    uint8_t value = GETWAY_NOT_SET;
    if (loadByteFromFile(GATEWAY_ADDR_FILE, value) == false)
    {
        Serial.println("error en load memori");
        return false;
    }
    if (value != GETWAY_NOT_SET)
    {
        getwayAdress = value;
        return true;
    }
    return false;
}

void NodeIdentity::saveGetway(uint8_t getwayAdress)
{
    saveByteToFile(GATEWAY_ADDR_FILE, getwayAdress);
}

/**
 * @brief Inicializa LittleFS y verifica/crea los archivos de ID y Gateway.
 * Debe ser llamado desde setup() después de Serial.begin().
 */
void NodeIdentity::begin()
{
    Serial.println("NodeIdentity: Iniciando metodo begin().");
/*
    // Intenta montar el sistema de archivos LittleFS.
    if (!LittleFS.begin())
    {
        Serial.println("¡ERROR! LittleFS no pudo ser montado. Intentando formatear...");
        if (LittleFS.format())
        {
            Serial.println("LittleFS formateado con éxito.");
            // Intenta montar de nuevo después de formatear
            if (!LittleFS.begin())
            {
                Serial.println("¡ERROR! LittleFS falló al montar incluso después de formatear. Abortando.");
                // Si no se puede montar después de formatear, es un fallo grave.
                while (true)
                    ; // Bloquear la ejecución
            }
            else
            {
                Serial.println("LittleFS montado con éxito después de formatear.");
            }
        }
        else
        {
            Serial.println("¡ERROR! Falló el formateo de LittleFS. Abortando.");
            // Si no se puede formatear, es un fallo grave.
            while (true)
                ; // Bloquear la ejecución
        }
    }
    else
    {
        Serial.println("LittleFS montado con éxito.");
    }

    // --- LÓGICA PARA ASEGURAR LA EXISTENCIA E INICIALIZACIÓN DE ARCHIVOS ---

    // 1. Asegurar que NODE_ID_FILE exista y tenga un valor inicial
    uint8_t tempNodeId;
    if (!loadByteFromFile(NODE_ID_FILE, tempNodeId) || tempNodeId == HASH_NOT_SET || tempNodeId == 0)
    {
        // El archivo no existe, está vacío o contiene un valor no válido (HASH_NOT_SET o 0)
        DEBUG_PRINTF("Archivo '%s' no encontrado o con valor invalido. Generando y guardando un nuevo NodeID.\n", NODE_ID_FILE);
        // Llama a getNodeID(). Este método se encarga de generar uno nuevo y guardarlo.
        // No pasamos blacklist aquí porque el begin() es solo para la inicialización base.
        // La blacklist es para cuando la aplicación ya está corriendo y necesita un ID sin colisiones.
        getNodeID(); // Esto asegura que el archivo se cree y se guarde.
    }
    else
    {
        DEBUG_PRINTF("Archivo '%s' encontrado. NodeID actual: 0x%X\n", NODE_ID_FILE, tempNodeId);
    }

    // 2. Asegurar que GATEWAY_ADDR_FILE exista y tenga un valor inicial
    uint8_t tempGatewayAddr;
    if (!loadByteFromFile(GATEWAY_ADDR_FILE, tempGatewayAddr) || tempGatewayAddr == GETWAY_NOT_SET)
    {
        // El archivo no existe, está vacío o contiene el valor GETWAY_NOT_SET
        DEBUG_PRINTF("Archivo '%s' no encontrado o con valor invalido. Inicializando Gateway Address con valor por defecto (%d).\n", GATEWAY_ADDR_FILE, GETWAY_NOT_SET);
        // Guarda el valor por defecto para crearlo o sobrescribirlo
        saveByteToFile(GATEWAY_ADDR_FILE, GETWAY_NOT_SET);
    }
    else
    {
        DEBUG_PRINTF("Archivo '%s' encontrado. Gateway Address actual: 0x%X\n", GATEWAY_ADDR_FILE, tempGatewayAddr);
    }
*/
    Serial.println("NodeIdentity: Verificacion/Inicializacion de archivos completada.");
    Serial.println("NodeIdentity: Metodo begin() finalizado.");
}