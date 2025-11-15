#include "serial.h"

SerialDevice::SerialDevice(uint32_t port, int32_t baudrate):
    index(port - 1),
    baudrate(baudrate)
{
    vexGenericSerialEnable(index, 0);
    vexGenericSerialBaudrate(index, baudrate);
}

void SerialDevice::flush()
{
    vexGenericSerialFlush(index);
}

int32_t SerialDevice::available()
{
    return vexGenericSerialReceiveAvail(index);
}

int32_t SerialDevice::peek()
{
    return vexGenericSerialPeekChar(index);
}

int32_t SerialDevice::read()
{
    return vexGenericSerialReadChar(index);
}

int32_t SerialDevice::read(uint8_t *buffer, int32_t length)
{
    return vexGenericSerialReceive(index, buffer, length);
}

int32_t SerialDevice::write(uint8_t character)
{
    return vexGenericSerialWriteChar(index, character);
}

int32_t SerialDevice::write(uint8_t *buffer, int32_t length)
{
    return vexGenericSerialTransmit(index, buffer, length);
}

int32_t SerialDevice::writefree()
{
    return vexGenericSerialWriteFree(index);
}
