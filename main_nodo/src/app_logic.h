// 03:00 16/6/2025
//  AppLogic.h
#ifndef APP_LOGIC_H
#define APP_LOGIC_H
 
#include "node_identity.h"  // Para NodeIdentity (dirección MAC, clave)
#include "radio_manager.h"  // Para RadioManager (gestión de radio LoRa)
#include "protocol.h"       // Para Protocol (serialización/deserialización de mensajes)
#include "sensor_manager.h" // Para GetData (obtención de datos de sensores)
#include "config.h"

/**
 * @class AppLogic
 * @brief Orquesta la lógica principal de un nodo sensor en la red mesh agrícola.
 *
 * Esta clase integra la identidad del nodo, la gestión de sensores y la comunicación LoRa mesh.
 * Se encarga de la lógica de ciclo de vida del nodo: inicialización, recepción y envío de mensajes, manejo de solicitudes y eventos críticos.
 *
 * Es el punto de unión entre la adquisición de datos, la comunicación mesh y la interacción con el gateway central.
 */
class AppLogic
{
private:
    SensorManager& getData;      ///< Referencia al gestor de sensores
    NodeIdentity& nodeIdentity;  ///< Referencia a la identidad del nodo
    RadioManager& radio;         ///< Referencia al gestor de radio LoRa mesh
    uint8_t gatewayAddress;      ///< Dirección del gateway asociado
    uint8_t nodeID;              ///< ID lógico del nodo
    bool gatwayRegistred = false;///< Flag de registro de gateway
    unsigned long temBuf = 0;    ///< Buffer de tiempo para control de envíos
    char MacNodeID[MAC_STR_LEN_WITH_NULL]; ///< MAC del nodo en formato char[]

    /**
     * @brief Maneja la recepción de mensajes ANNOUNCE del gateway.
     */
    void handleAnnounce(uint8_t *buf, uint8_t len, uint8_t from);

    /**
     * @brief Envía un mensaje HELLO al gateway para anunciar el nodo.
     */
    void sendHello();

    /**
     * @brief Envía los datos atmosféricos actuales al gateway.
     */
    void sendAtmosphericData();

    /**
     * @brief Envía los datos de suelo y GPS actuales al gateway.
     */
    void sendGroungGpsData();

    /**
     * @brief Cambia el ID del nodo en caso de conflicto de dirección.
     */
    void changeID(uint8_t *buf, uint8_t len);

public:
    /**
     * @brief Constructor de AppLogic.
     * @param data Referencia al gestor de sensores.
     * @param identity Referencia a la identidad del nodo.
     * @param radioMgr Referencia al gestor de radio LoRa mesh.
     */
    AppLogic(SensorManager& data, NodeIdentity& identity, RadioManager& radioMgr);

    /**
     * @brief Inicializa la lógica del nodo (debe llamarse en setup()).
     */
    void begin();

    /**
     * @brief Actualiza la lógica del nodo (debe llamarse en loop()).
     * Gestiona la recepción y envío de mensajes, y el ciclo de vida del nodo.
     */
    void update();
};

#endif // APP_LOGIC_H
