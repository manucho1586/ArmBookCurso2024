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
    int tiempo_acumulado=0;
    int tiempo_incremento=10;
    int cont=0;
    LED_rojo=OFF;
    LED_verde=ON;
    ventilador=OFF;
    buzzer=LOW;    //Apagado

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
                buzzer=HIGH;
                ventilador=HIGH;
            }
        }
        else
        {
            if(LED_verde==OFF)
            {
                LED_verde=ON;
                LED_rojo=OFF;
                ventilador=LOW;
                buzzer=LOW;
            }
        }
        while(boton_buzzer==LOW)
        {
            delay(1);
            if(boton_buzzer==LOW && cont<=120)
            {
                cont++;
                if(cont==120)
                {
                    buzzer=!buzzer;
                    cont=0;
                }
            }
        }
        while(boton_ventilador==LOW)
        {
            delay(1);
            if(boton_ventilador==LOW && cont<=120)
            {
                cont++;
                if(cont==120)
                {
                    ventilador=!ventilador;
                    cont=0;
                }
            }
        }
        if(tiempo_acumulado==2000)
        {
        s=d.readData();
        //printf("T:%d, H:%d\r\n", d.readTemperature(), d.readHumidity()); // esta instrucción ya imprime en pantala del monitor serie.
        sprintf(buffer, "T:%d, H:%d\r\n", d.readTemperature(), d.readHumidity());
        pc.write(buffer,16);
        tiempo_acumulado=0;
        }
        delay(10); //delay de 200 ms
        tiempo_acumulado=tiempo_acumulado + tiempo_incremento;
    }
}
