#include "mbed.h"
#include "Dht11.h"
#include "arm_book_lib.h"
#include <cstdint>



int main()
{
    DigitalIn B1(BUTTON1);
    Dht11 sensor(D1);
    UnbufferedSerial pc(USBTX, USBRX);
    DigitalOut Teyu(LED1);
    char buffer[9];

    // Set desired properties (9600-8-N-1).
    pc.baud(9600);
    pc.format(8, SerialBase::None, 1);
    uint8_t temperatura;

    while (true)
    {
        Teyu = !B1;
        temperatura=sensor.getCelsius();
        sprintf(buffer, "T: %.2d", temperatura);
        pc.write(buffer,9);
        delay(1000);
    }
}
