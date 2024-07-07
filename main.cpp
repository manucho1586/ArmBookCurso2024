#include "mbed.h"
#include "dht11.h"
#include "arm_book_lib.h"
#include <cstdint>



int main()
{
    DigitalIn B1(BUTTON1);
    DHT11 d(D2);
    UnbufferedSerial pc(USBTX, USBRX);
    DigitalOut Teyu(LED1);
    char buffer[9];

    // Set desired properties (9600-8-N-1).
    pc.baud(9600);
    pc.format(8, SerialBase::None, 1);

    while (true)
    {
        Teyu = !B1;
        pc.write(buffer,9);

        int s;
        s=d.readData();
        if (s!=DHT11::OK) 
        {
        printf("Error!\r\n");
        delay(2000);
        sprintf(buffer, "%d", s);
        }
        else 
        {
        printf("T:%d, H:%d\r\n", d.readTemperature(), d.readHumidity());
        }
    }
    delay(1000);
}
