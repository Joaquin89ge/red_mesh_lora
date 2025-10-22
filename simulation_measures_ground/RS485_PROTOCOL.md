## RS485 Binary Protocol – Ground Sensor (ESP32 ↔ ESP32)

This document defines the binary protocol used over UART via RS485 (MAX485) to request and transmit ground sensor data between two ESP32 devices (master and slave). It specifies message types, data layout, byte ordering, timings, and example code for a receiver implementation.

### Electrical/Hardware
- Physical layer: RS485 half‑duplex using MAX485 (or compatible).
- Default UART: ESP32 `Serial2` at 9600 baud, 8 data bits, no parity, 1 stop (9600 8N1).
- Pins (slave, configurable in `src/config.h`):
  - RS485_RX_PIN = 17 (Serial2 RX)
  - RS485_TX_PIN = 16 (Serial2 TX)
  - RS485_DE_PIN = 2  (DE/RE tied together, output HIGH = transmit, LOW = receive)
- Bus idle: DE/RE LOW (receive). Raise HIGH only while sending, then lower to LOW immediately after flush + small delay.
- It is assumed both ESP32s share common ground and RS485 A/B are correctly wired with end termination (120Ω) and biasing when needed.

### Transport
- No framing or checksum (no HEADER/LEN/CRC/FOOTER). Messages are minimalist and self‑describing by first byte.
- Byte ordering: little‑endian for multibyte fields (ESP32 default).

### Message Types (1 byte)
Defined in `src/protocol.h`:

```c
enum MessageType : uint8_t {
  // Control
  PING            = 0x10, // Ping to check presence
  PONG            = 0x11, // Reply to PING

  // Requests
  REQ_GROUND_DATA = 0x20, // Request ground sensor data (no payload)

  // Data
  DATA_GROUND     = 0x30, // GroundSensorBinary payload (13 bytes)

  // Maintenance (reserved)
  NODE_ANNOUNCE   = 0x40,

  // Errors
  ERROR_GENERIC   = 0xE0,
};
```

### Payload: GroundSensorBinary (13 bytes)
Defined packed in `src/protocol.h`:

```c
struct __attribute__((packed)) GroundSensorBinary {
  int16_t  temp;      // tenths of °C [-400..800] represents -40.0..80.0 °C
  uint16_t moisture;  // tenths of %  [0..1000] represents 0.0..100.0 %
  uint16_t n;         // mg/kg [0..1999]
  uint16_t p;         // mg/kg [0..1999]
  uint16_t k;         // mg/kg [0..1999]
  uint16_t EC;        // µS/cm [0..20000]
  uint8_t  PH;        // tenths [30..90] represents 3.0..9.0 pH
};
// sizeof(GroundSensorBinary) == 13
```

### Transaction Flow
1) Master sends 1 byte `REQ_GROUND_DATA (0x20)` while holding bus in transmit (its DE/RE HIGH). After sending, master returns DE/RE to LOW (receive).
2) Slave reads the command byte. If it is `REQ_GROUND_DATA`:
   - Samples local sensors (or maps potentiometers), builds a `GroundSensorBinary` payload.
   - Raises DE/RE HIGH, sends `DATA_GROUND (0x30)` followed by 13 payload bytes.
   - Flushes UART, waits a small delay (see Timing), then returns DE/RE to LOW.

No newline or extra bytes are appended. Total response length is exactly 14 bytes.

### Timing Recommendations
- UART: 9600 8N1
- On slave before transmit: small settle delay after raising DE (config: `RS485_TX_DELAY` ≈ 2–5 ms).
- After transmit: `Serial.flush()` then `RS485_RESPONSE_DELAY` ≈ 2–5 ms before lowering DE.
- Master should also add a small delay after returning to receive mode before starting to read.

### Receiver (Master) Implementation Notes
- Always read the first byte to determine the message type.
- For `DATA_GROUND (0x30)`, read exactly 13 subsequent bytes into a `GroundSensorBinary` struct.
- Validate the total received length; since there’s no CRC, use timeouts and length checks.

#### Example (ESP32 Master, Arduino)
```c
#include <Arduino.h>
#include "protocol.h"

static const int RX2 = 17; // match your board wiring
static const int TX2 = 16;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);
}

bool requestGroundData(GroundSensorBinary &out) {
  // Send request byte
  uint8_t req = (uint8_t)REQ_GROUND_DATA;
  Serial2.write(req);
  Serial2.flush();

  // Wait for response type
  unsigned long t0 = millis();
  while (Serial2.available() < 1) {
    if (millis() - t0 > 200) return false; // timeout
  }

  uint8_t type = (uint8_t)Serial2.read();
  if (type != (uint8_t)DATA_GROUND) return false;

  // Read payload (13 bytes)
  t0 = millis();
  while (Serial2.available() < (int)sizeof(GroundSensorBinary)) {
    if (millis() - t0 > 200) return false; // timeout
  }

  size_t got = Serial2.readBytes((char*)&out, sizeof(GroundSensorBinary));
  return got == sizeof(GroundSensorBinary);
}

void loop() {
  GroundSensorBinary gs;
  if (requestGroundData(gs)) {
    Serial.print("temp(0.1C)="); Serial.print(gs.temp);
    Serial.print(" moisture(0.1%)="); Serial.print(gs.moisture);
    Serial.print(" pH(0.1)="); Serial.print(gs.PH);
    Serial.print(" EC(uS/cm)="); Serial.print(gs.EC);
    Serial.print(" N="); Serial.print(gs.n);
    Serial.print(" P="); Serial.print(gs.p);
    Serial.print(" K="); Serial.println(gs.k);
  } else {
    Serial.println("request failed");
  }
  delay(500);
}
```

### Slave Behavior Summary (already implemented in this repo)
- Reads the first incoming byte as `MessageType`.
- On `REQ_GROUND_DATA (0x20)`: samples, converts to `GroundSensorBinary`, transmits:
  - 1 byte: `DATA_GROUND (0x30)`
  - 13 bytes: `GroundSensorBinary`
- Controls DE/RE: LOW idle, HIGH only while writing, then `flush()` + delay and back to LOW.

### Error Handling
- No checksum used. If needed in future, extend with a framed variant or append CRC16.
- If an unknown `MessageType` is received, slave may ignore or return `ERROR_GENERIC (0xE0)` (no payload specified).

### Compatibility and Byte Order
- Both sides must compile the same `protocol.h` so that `sizeof(GroundSensorBinary) == 13` and field order matches.
- All multi‑byte fields are little‑endian; ESP32↔ESP32 is naturally compatible.

### Test Checklist
- Check `sizeof(GroundSensorBinary)` prints 13 on the slave at startup.
- Master sends 0x20 and receives 14 bytes total (0x30 + 13 bytes payload).
- Verify DE/RE timing and that both devices share ground; add termination and bias as needed.


