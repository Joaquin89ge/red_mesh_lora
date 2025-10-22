/**
 * @file node_identity.h
 * @brief Cabecera para la gestión de identidad de nodos con clave compartida
 * @date 22:00 17/6/2025
 */

#ifndef NODE_IDENTITY_H
#define NODE_IDENTITY_H

#include <Arduino.h>

#include <WiFi.h>
#include <LittleFS.h> // ¡Cambiamos de EEPROM.h a LittleFS.h!
#include "config.h"

// Nombres de archivos para LittleFS (más descriptivo que direcciones de EEPROM)
#define NODE_ID_FILE "/node_id.json"      ///< Nombre del archivo para el ID del nodo
#define GATEWAY_ADDR_FILE "/gateway.json" ///< Nombre del archivo para la dirección del gateway

#define HASH_NOT_SET 255   ///< Valor no inicializado (seguimos usándolo para la lógica interna)
#define GETWAY_NOT_SET 255 ///< Valor no inicializado (seguimos usándolo para la lógica interna)

extern const uint8_t defaultBlacklist[2]; ///< lista por defecto de direcciones prohibidas 0x00 y 0xFF

/**
 * @class NodeIdentity
 * @brief Gestiona la identidad única y persistente de un nodo en la red mesh agrícola.
 *
 * Esta clase se encarga de generar, almacenar y recuperar el identificador único del nodo (basado en la MAC y un hash), así como la dirección del gateway asociado.
 * Utiliza almacenamiento persistente (LittleFS) para mantener la identidad entre reinicios y evitar colisiones en la red.
 *
 * Es fundamental para la auto-configuración y robustez de la red mesh, permitiendo que cada nodo tenga una dirección única y pueda reencontrar su gateway tras reinicios o cambios de topología.
 */
class NodeIdentity
{
public:
    /**
     * @brief Constructor de NodeIdentity.
     * Inicializa la clase y prepara la clave de autenticación.
     */
    NodeIdentity();

    /**
     * @brief Obtiene el identificador lógico único del nodo.
     * Genera o recupera el ID desde almacenamiento persistente, evitando colisiones.
     * @param blacklist Lista de IDs prohibidos.
     * @param blacklist_len Tamaño de la lista negra.
     * @return ID único del nodo (1-254).
     */
    uint8_t getNodeID(size_t blacklist_len = 2, const uint8_t *blacklist = defaultBlacklist);

    /**
     * @brief Recupera la dirección MAC del hardware.
     * @return String con la MAC en formato XX:XX:XX:XX:XX:XX.
     */
    String getDeviceMAC();

    /**
     * @brief Guarda la dirección del gateway asociada a este nodo.
     * @param getwayAdress Dirección a guardar.
     */
    void saveGetway(const uint8_t getwayAdress);

    /**
     * @brief Verifica si hay una dirección de gateway guardada y la recupera.
     * @param stored_getway Variable de salida para la dirección recuperada.
     * @return true si existe una dirección guardada, false en caso contrario.
     */
    bool getGetway(uint8_t &stored_getway);

    /**
     * @brief Cambia el ID del nodo en caso de conflicto o error de dirección.
     * @param blacklist Lista de IDs prohibidos.
     * @param blacklist_len Tamaño de la lista negra.
     * @return Nuevo ID generado y almacenado.
     */
    uint8_t changeNodeID(const size_t blacklist_len, uint8_t *blacklist);

    /**
     * @brief Inicializa el sistema de archivos y prepara la identidad del nodo.
     * Debe llamarse en setup() antes de usar el resto de métodos.
     */
    void begin();

private:
    uint8_t key[4]; ///< Clave compartida interna (para autenticación, si se usa)
    static const uint8_t defaultBlacklist[2]; ///< IDs reservados (0x00 y 0xFF)

    /**
     * @brief Genera un hash seguro evitando colisiones con la blacklist.
     */
    uint8_t generateSafeHash(const uint8_t *data, size_t len, const uint8_t *blacklist, size_t blacklist_len);

    /**
     * @brief Implementación de CRC-8 para generación de hash.
     */
    uint8_t crc8(const uint8_t *data, size_t len);

    bool loadByteFromFile(const char *filename, uint8_t &value);
    void saveByteToFile(const char *filename, uint8_t value);
};
#endif
