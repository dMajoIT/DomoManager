
#pragma once

// ============================================================================
// DOMOMANAGER - MODBUS PLATFORM BACKEND
// ============================================================================
// OPTA / Arduino Mbed : ArduinoModbus
// ESP32-S3             : ArduinoModbus
//
// Le API client e server utilizzate dall'applicazione sono centralizzate qui.
//
// IMPORTANTE:
// - Nessuna classe adattatrice eModbus.
// - Nessuna modifica al trasporto TCP.
// - Nessuna modifica alla logica applicativa.
// - Le due piattaforme utilizzano la stessa libreria.
// ============================================================================

#include <Arduino.h>
#include <stdint.h>

#if defined(ARDUINO_ARCH_MBED) || defined(ARDUINO_ARCH_ESP32)

    #include <ArduinoModbus.h>

#else

    #error "Unsupported DomoManager platform for Modbus"

#endif


namespace DMPlatform
{
namespace Modbus
{

// ============================================================================
// TYPES
// ============================================================================

using TCPClient = ::ModbusTCPClient;
using TCPServer = ::ModbusTCPServer;


// ============================================================================
// READ FUNCTION
// ============================================================================
// I nomi evitano collisioni con le macro definite da ArduinoModbus:
// HOLDING_REGISTERS, INPUT_REGISTERS, DISCRETE_INPUTS.
// ============================================================================

enum class ReadFunction : uint8_t
{
    HoldingRegisters,
    InputRegisters,
    DiscreteInputs
};


// ============================================================================
// CLIENT - CONNECTION
// ============================================================================

inline bool begin(
    TCPClient& client,
    const IPAddress& ip,
    int port)
{
    return client.begin(ip, port);
}


inline bool connected(
    TCPClient& client)
{
    return client.connected();
}


inline void stop(
    TCPClient& client)
{
    client.stop();
}


// ============================================================================
// CLIENT - READ REQUEST
// ============================================================================

inline int requestFrom(
    TCPClient& client,
    int deviceAddress,
    ReadFunction function,
    int startingAddress,
    int quantity)
{
    int nativeFunction = 0;

    switch (function)
    {
        case ReadFunction::HoldingRegisters:
            nativeFunction = HOLDING_REGISTERS;
            break;

        case ReadFunction::InputRegisters:
            nativeFunction = INPUT_REGISTERS;
            break;

        case ReadFunction::DiscreteInputs:
            nativeFunction = DISCRETE_INPUTS;
            break;

        default:
            return 0;
    }

    return client.requestFrom(
        deviceAddress,
        nativeFunction,
        startingAddress,
        quantity
    );
}


// ============================================================================
// CLIENT - READ RESULT
// ============================================================================

inline int read(
    TCPClient& client)
{
    return client.read();
}


inline const char* lastError(
    TCPClient& client)
{
    return client.lastError();
}


// ============================================================================
// CLIENT - WRITE HOLDING REGISTER
// ============================================================================

inline int writeHoldingRegister(
    TCPClient& client,
    int deviceAddress,
    int address,
    int value)
{
    return client.holdingRegisterWrite(
        deviceAddress,
        address,
        value
    );
}


// ============================================================================
// CLIENT - WRITE COIL
// ============================================================================

inline int writeCoil(
    TCPClient& client,
    int deviceAddress,
    int address,
    int value)
{
    return client.coilWrite(
        deviceAddress,
        address,
        value
    );
}


// ============================================================================
// SERVER - LIFECYCLE
// ============================================================================

inline int serverBegin(
    TCPServer& server)
{
    return server.begin();
}


inline void serverEnd(
    TCPServer& server)
{
    server.end();
}


// ============================================================================
// SERVER - ACCEPT CLIENT
// ============================================================================

template <typename TCPClientType>
inline void serverAccept(
    TCPServer& server,
    TCPClientType& client)
{
    server.accept(client);
}


// ============================================================================
// SERVER - POLL
// ============================================================================

inline int serverPoll(
    TCPServer& server)
{
    return server.poll();
}


// ============================================================================
// SERVER - CONFIGURE HOLDING REGISTERS
// ============================================================================

inline int serverConfigureHoldingRegisters(
    TCPServer& server,
    int startAddress,
    int count)
{
    return server.configureHoldingRegisters(
        startAddress,
        count
    );
}


// ============================================================================
// SERVER - WRITE HOLDING REGISTER
// ============================================================================

inline int serverHoldingRegisterWrite(
    TCPServer& server,
    int address,
    uint16_t value)
{
    return server.holdingRegisterWrite(
        address,
        value
    );
}


// ============================================================================
// SERVER - READ HOLDING REGISTER
// ============================================================================

inline long serverHoldingRegisterRead(
    TCPServer& server,
    int address)
{
    return server.holdingRegisterRead(
        address
    );
}


} // namespace Modbus
} // namespace DMPlatform