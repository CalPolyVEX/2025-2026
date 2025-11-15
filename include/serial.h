#include "v5_apiuser.h"

class SerialDevice
{
private:
    uint32_t index;
    int32_t baudrate;

public:
    /** 
     * Initialize communication with an RS485 device connected to a Smart Port
     * @param port The physical port number that the serial device is connected to [1-21]
     * @param baudrate The expected data rate for all transceiving operations (default = 115200)
    */
    SerialDevice(uint32_t port, int32_t baudrate = 115200);
    
    /**
     * Flush the serial transmitting buffer
     */
    void flush();

    /**
     * Gets the availability of bytes in the receiving buffer
     */
    int32_t available();

    /**
     * Reads the next byte from the receiving buffer without removing it from the queue
     * @return A single character
     */
    int32_t peek();

    /**
     * Reads and removes the next byte from the receiving buffer
     * @return A single character
     */
    int32_t read();

    /**
     * Reads and removes the specified number of bytes from the receiving buffer
     * @param buffer The buffer to read to
     * @param length The number of bytes to read
     */
    int32_t read(uint8_t *buffer, int32_t length);

    /**
     * Writes a single character to transmitting buffer
     * @param character The character to write
     */
    int32_t write(uint8_t character);

    /**
     * Writes the specified number of bytes to the transmitting buffer
     * @param buffer The characters to write
     * @param length The number of bytes
     */
    int32_t write(uint8_t *buffer, int32_t length);

    /**
     * Gets the available size of the transmitting buffer? REQUIRES TESTING
     */
    int32_t writefree();
};
