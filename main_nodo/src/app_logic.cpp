// 03:00 16/6/2025
//  AppLogic.cpp (Lógica para un nodo sensor)
#include "app_logic.h" // Incluye la definición de la clase AppLogic.

// TODO:queda implementar logica  errores y posible reinicio si se acomulan
/**
 * @file app_logic.cpp
 * @brief Implementación de la clase AppLogic para un nodo sensor.
 */

/**
 * @brief Constructor de la clase AppLogic para un nodo sensor.
 *
 * Inicializa las referencias a los objetos `NodeIdentity`, `SensorManager` (formalmente GetData) y `RadioManager`
 * que se utilizarán para la gestión de la identidad, la adquisición de datos
 * y la comunicación por radio. También inicializa la dirección del Gateway.
 *
 * @param identity Referencia al objeto NodeIdentity.
 * @param radioMgr Referencia al objeto RadioManager.
 * @param data Referencia al objeto SensorManager (formalmente GetData).
 * @note La declaración en el .h es `AppLogic(NodeIdentity &identity, RadioManager &radioMgr, SensorManager &data, uint8_t gwAddress);`
 * La implementación aquí es `AppLogic(NodeIdentity &identity, RadioManager &radioMgr, SensorManager &data)`.
 * Para que coincida, esta implementación debería aceptar y usar `gwAddress`.
 * Voy a asumir que el constructor del .h es el correcto y que esta implementación
 * debería ser actualizada para tomar `gwAddress`. Por ahora, documento la versión actual del .cpp
 * e inicializo `gatewayAddress` a un valor por defecto o inválido si no se proporciona.
 */
AppLogic::AppLogic(SensorManager &data, NodeIdentity &identity, RadioManager &radioMgr)
    : getData(data),
      nodeIdentity(identity),
      radio(radioMgr)
{
    begin();
}

/**
 * @brief Inicia la lógica de la aplicación del nodo sensor.
 *
 * Este método se llama una vez al inicio del programa (normalmente en `setup()`).
 * Su propósito es realizar configuraciones iniciales. Actualmente, envía un mensaje HELLO
 * al `gatewayAddress` para anunciar la presencia de este nodo sensor en la red.
 */
void AppLogic::begin()
{
    Serial.println("[DEBUG] AppLogic::begin INICIO");
    Serial.print("inicio logic-----------------");
    nodeID = nodeIdentity.getNodeID();
    Serial.println("nodeID:");
    Serial.print(String(nodeID));
    gatwayRegistred = nodeIdentity.getGetway(gatewayAddress);
    Serial.println("gatewayAddress:");
    Serial.print(String(gatewayAddress));
    // Array donde se almacenará la MAC de forma global o en el ámbito necesario

    // Paso 1: Obtener la String de la MAC y guardarla
    String macStringTemporal = nodeIdentity.getDeviceMAC();

    // Paso 2: Usar toCharArray() sobre esa String para copiarla al array de char
    // toCharArray() automáticamente añade el '\0' si hay suficiente espacio.
    macStringTemporal.toCharArray(MacNodeID, MAC_STR_LEN_WITH_NULL);
    Serial.println("MacNodeID:");
    Serial.print(MacNodeID);
    Serial.println("MacNodeID size:");
    Serial.println(sizeof(MacNodeID));
    Serial.println("[DEBUG] AppLogic::begin FIN");
}

/**
 * @brief Actualiza la lógica de la aplicación en cada ciclo del bucle principal.
 *
 * Este método debe ser llamado repetidamente (en `loop()`).
 * Comprueba si hay mensajes pendientes en la radio utilizando `radio.recvMessage()`.
 * Si se recibe un mensaje, intenta parsearlo con `Protocol::parse()`.
 * Los mensajes parseados correctamente se procesan según su `msg.type`:
 * - `Protocol::ANNOUNCE`: Llama a `handleAnnounce()`.
 * - `Protocol::REQUEST_DATA`: Llama a `handleRequestData()`.
 * - Otros tipos: Se ignora y se imprime un mensaje si no son esperados por un sensor.
 */
void AppLogic::update()
{
    uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    uint8_t from = 0;
    uint8_t flag = 0;

    // Intenta recibir un mensaje.
    if (radio.recvMessage(buf, &len, &from, &flag))
    {
        Serial.print(F("---------------[AppLogic] Mensaje recibido de 0x"));
        Serial.print(String(from));
        Serial.print(F(" con longitud "));
        Serial.println(len);
        Serial.print("Tipo de mensaje: 0x");
        Serial.println(String(flag));
        if (static_cast<Protocol::MessageType>(flag) == Protocol::MessageType::ANNOUNCE)
        {
            handleAnnounce(buf, len, from);
        }

        else if (gatwayRegistred == true && from == gatewayAddress)
        {
            Serial.print("----------------Tipo de mensaje: 0x");
            Serial.println(String(flag));
            Serial.print("----------------no es announcher: 0x");
            switch (static_cast<Protocol::MessageType>(flag))
            {
            case Protocol::MessageType::REQUEST_DATA_ATMOSPHERIC:
                Serial.print("----------------Tipo de mensaje-------------------/n: 0x");
                Serial.println(String(flag));
                Serial.print("REQUEST_DATA_ATMOSPHERIC: 0x");
                sendAtmosphericData();
                break;
            case Protocol::MessageType::REQUEST_DATA_GPC_GROUND:
                sendGroungGpsData();
                break;
            case Protocol::MessageType::ERROR_DIRECCION:
                changeID(buf, len);
                break;
            default:
                Serial.print("Tipo de mensaje desconocido o no relevante para este nodo: 0x");
                Serial.println(String(flag));
                break;
            }
        }
    }

    getData.update();

    unsigned long tiempoActual = millis();
    if (tiempoActual - temBuf >= INTERVALOHELLO && gatwayRegistred == true)
    {
        temBuf = tiempoActual;
        sendHello();
    }
}
/**
 * @brief Envía un mensaje HELLO al Gateway.
 *
 * Crea un mensaje de tipo `Protocol::HELLO`.
 * Establece el `nodeID` del mensaje con la identidad de este nodo.
 * El `jsonPayload` para un mensaje HELLO es actualmente una cadena JSON vacía "{}".
 * Serializa el mensaje y lo envía a través de `radio.sendMessage()` al `gatewayAddress`.
 * Registra el envío en la consola serial.
 */
void AppLogic::sendHello()
{
    unsigned long startTime = millis();
    const unsigned long TIMEOUT_MS = 5000; // 5 segundos

    Serial.println(F("[AppLogic] Intentando enviar mensaje HELLO."));


    // 1. Debug del buffer y su tamaño
    Serial.print(F("[AppLogic] Tamano del buffer (sizeof): "));
    Serial.println(MAC_STR_LEN_WITH_NULL);

    // 2. Debug de la dirección del gateway
    Serial.print(F("[AppLogic] Direccion del Gateway (gatewayAddress): "));
    Serial.println(gatewayAddress);

    // 3. Debug del tipo de mensaje (MessageType) que se pasa
    Serial.print(F("[AppLogic] Tipo de mensaje a enviar (MessageType): "));
    Serial.println(static_cast<uint8_t>(Protocol::MessageType::HELLO));


    // Enviar con timeout - si se cuelga por más de 5 segundos, salir
    bool sendResult = false;
    unsigned long sendStartTime = millis();

    sendResult = radio.sendMessage(gatewayAddress, reinterpret_cast<uint8_t*>(MacNodeID), MAC_STR_LEN_WITH_NULL, static_cast<uint8_t>(Protocol::MessageType::HELLO));

    unsigned long totalTime = millis() - startTime;
    Serial.print(F("[AppLogic] Tiempo total: "));
    Serial.print(totalTime);
    Serial.println(F(" ms"));

    if (totalTime >= TIMEOUT_MS)
    {
        Serial.println(F("[AppLogic] TIMEOUT: sendHello se colgó por más de 5 segundos"));
        return;
    }



    if (sendResult)
    {
        Serial.println(F("[AppLogic] HELLO enviado exitosamente."));
    }
    else
    {
        Serial.println(F("[AppLogic] Fallo al enviar HELLO."));
    }

}

/**
 * @brief Maneja un mensaje ANNOUNCE.
 *
 */

void AppLogic::handleAnnounce(uint8_t *buf, uint8_t len, uint8_t from)
{

    Serial.println("--- handleAnnounce INICIO ---");
    Serial.print("Recibido ANNOUNCE de: ");
    Serial.println(String(from)); // Convierte uint8_t a String

    Serial.print("Longitud del buffer (len): ");
    Serial.println(String(len)); // Convierte uint8_t a String

    if (gatwayRegistred == true && gatewayAddress == from)
    {
        Serial.println("Gateway ya registrado y es el mismo remitente. Ignorando ANNOUNCE.");
        Serial.println("--- handleAnnounce FIN ---");
        return; // Salir de la función si el gateway ya está registrado y es el mismo
    }

    Serial.print("Byte 0 del buffer (buf[0]): 0x");
    Serial.println(String(buf[0], HEX)); // ¡Aquí está el cambio clave!
                                         // String(valor, BASE) lo convierte a String en el formato deseado.
    Serial.print("KEY: 0x");
    Serial.println(String(Protocol::KEY, HEX)); // Lo mismo para la clave del protocolo

    if (buf[0] == Protocol::KEY)
    {
        Serial.println("Clave de protocolo (Protocol::KEY) coincide.");
        Serial.print("Antigua gatewayAddress: ");
        Serial.println(String(gatewayAddress)); // Convierte uint8_t a String

        gatewayAddress = from;                   // Asigna la dirección del remitente como la del gateway
        nodeIdentity.saveGetway(gatewayAddress); // Guarda la dirección del gateway
        gatwayRegistred = true;                  // Marca que el gateway ha sido registrado

        Serial.print("Nueva gatewayAddress establecida: ");
        Serial.println(String(gatewayAddress)); // Convierte uint8_t a String
        Serial.println("Gateway registrado: TRUE");
    }
    else
    {
        Serial.println("Clave de protocolo (Protocol::KEY) NO coincide. ANNOUNCE inválido.");
    }

    Serial.println("--- handleAnnounce FIN ---");
    return; // Retorno final de la función
}

/**
 * @brief Envía los datos actuales del sensor al Gateway.

 */
void AppLogic::sendAtmosphericData()
{
    // Imprimir información de depuración antes de enviar
    Serial.println("[DEBUG] ---- DEPURACIÓN DE ENVÍO DE DATA ATMOSFÉRICA ----");

    Serial.print("Tamaño de DATA atmosferica a enviar: ");
    Serial.println(sizeof(getData.atmosSamples));
    Serial.print("RH_MESH_MAX_MESSAGE_LEN: ");
    Serial.println(RH_MESH_MAX_MESSAGE_LEN);
    Serial.print("Número de muestras atmosféricas: ");
    Serial.println(getData.atmosSampleCount);
    // Imprimir las primeras muestras para ver su contenido

    for (int i = 0; i <= getData.atmosSampleCount; ++i)
    {
        Serial.print("Muestra ");
        Serial.print(i);
        Serial.print(": temp=");
        Serial.print(getData.atmosSamples[i].temp);
        Serial.print(", humedad=");
        Serial.print(getData.atmosSamples[i].moisture);
        Serial.print(", hora=");
        Serial.print(getData.atmosSamples[i].hour);
        Serial.print(":");
        Serial.println(getData.atmosSamples[i].minute);
    }
    // Enviar el mensaje con la estructura de prueba
    Serial.println("[DEBUG] 7. Antes de radio.sendMessage (DATA_ATMOSPHERIC)");
    bool ok = radio.sendMessage(gatewayAddress, reinterpret_cast<uint8_t *>(&getData.atmosSamples), sizeof(getData.atmosSamples), Protocol::MessageType::DATA_ATMOSPHERIC);
    Serial.println("[DEBUG] 8. Después de radio.sendMessage (DATA_ATMOSPHERIC)");

    if (ok)
    {
        Serial.println(F("[AppLogic] DATA atmosferica enviado exitosamente."));
    }
    else
    {
        Serial.println(F("[AppLogic] Falló el envío de DATA atmosferica al Gateway."));
    }
    Serial.println("[DEBUG] 9. ---- FIN DEPURACIÓN DATA ATMOSFÉRICA ----");
}

/**
 * @brief Envía los datos actuales del sensor al Gateway.

 */
void AppLogic::sendGroungGpsData()
{
    getData.readGroundGpsSensors();
    Protocol::GroundGpsPacket packet;
    packet.ground = getData.groundData;
    // Usar la última coordenada guardada
    auto lastCoord = getData.getLastGpsCoordinate();
    packet.gps.latitude = lastCoord.latitude;
    packet.gps.longitude = lastCoord.longitude;
    packet.gps.hour = lastCoord.hour;
    packet.gps.minute = lastCoord.minute;
    // Mantener altitud y flags como estaban
    packet.gps.altitude = getData.gpsData.altitude;
    packet.gps.flags = getData.gpsData.flags;
    // Agregar datos de energía
    packet.energy = getData.energyData;
    Serial.println("[DEBUG] Antes de radio.sendMessage (DATA_GPS_CROUND)");
    bool ok = radio.sendMessage(gatewayAddress, reinterpret_cast<uint8_t *>(&packet), sizeof(packet), Protocol::MessageType::DATA_GPS_CROUND);
    Serial.println("[DEBUG] Después de radio.sendMessage (DATA_GPS_CROUND)");
    if (ok)
    {
        Serial.println(F("[AppLogic] DATA gps, ground y energía enviado exitosamente."));
    }
    else
    {
        Serial.println(F("[AppLogic] Falló el envío de DATA gps, ground y energía al Gateway."));
    }
}

void AppLogic::changeID(uint8_t *buf, uint8_t len)
{
    Serial.println("Error de dirección detectado.");
    if (len != 0)
    {
        nodeID = nodeIdentity.changeNodeID(len, buf);
    }
    else
    {
        Serial.print("no llego data de valores usados asumo sera mi direccion la repetida");
        uint8_t blacklist[3] = {nodeID, 0, 255};
        nodeID = nodeIdentity.changeNodeID(1, blacklist);
    }
    sendHello(); // envio de Hello nuevamente
}