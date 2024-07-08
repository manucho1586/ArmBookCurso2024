#include "mbed.h"
#include "dht11.h"
#include "arm_book_lib.h"
#include <cstdint>



int main()
{
    DigitalIn boton_buzzer(BUTTON1);
    DigitalIn boton_ventilador(D6);
    DHT11 d(D2);
    UnbufferedSerial pc(USBTX, USBRX);
    DigitalOut LED_rojo(LED1);
    DigitalOut LED_verde(D3);
    DigitalOut ventilador(D4);
    DigitalOut buzzer(D5);
    char buffer[16];
    int s;
    int umbral=20;
    LED_rojo=OFF;
    LED_verde=ON;
    ventilador=OFF;
    buzzer=OFF;

    // Set desired properties (9600-8-N-1).
    pc.baud(9600);
    pc.format(8, SerialBase::None, 1);

    while (true)
    {
        if(d.readTemperature()>umbral)
        {
            if(LED_rojo==OFF)
            {
                LED_rojo=ON;
                LED_verde=OFF;
                buzzer=ON;
            }
            if(ventilador==OFF)
            {
                ventilador=ON;
            }
        }
        else
        {
            if(LED_verde==OFF)
            {
                LED_verde=ON;
                LED_rojo=OFF;
                ventilador=OFF;
                buzzer=OFF;
            }
        }
        if(boton_buzzer==OFF)
        {
            buzzer=!buzzer;
        }
        if(boton_ventilador)
        {
            ventilador=ventilador;
        }
        s=d.readData();
        //printf("T:%d, H:%d\r\n", d.readTemperature(), d.readHumidity()); // esta instrucción ya imprime en pantala del monitor serie.
        sprintf(buffer, "T:%d, H:%d\r\n", d.readTemperature(), d.readHumidity());
        pc.write(buffer,16);
        delay(2000);
    }
}
