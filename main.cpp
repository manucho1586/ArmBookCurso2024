#include "mbed.h"
#include "Dht11.h"
#include "arm_book_lib.h"

static UnbufferedSerial pc(USBTX, USBRX);

int main()
{
    DigitalIn B1(BUTTON1);
    Dht11 sensor(D1);
    DigitalOut Teyu(LED1);
    char buffer[9];

    // Set desired properties (9600-8-N-1).
    pc.baud(9600);
    pc.format(8, SerialBase::None, 1);

    while (true)
    {
        Teyu = !B1;
        sprintf(buffer, "T: %.2d", sensor.getCelsius());
        pc.write(buffer,9);
        delay(2000);
    }
}
