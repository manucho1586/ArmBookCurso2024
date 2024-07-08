#include "mbed.h"
#include "dht11.h"
#include "arm_book_lib.h"
#include <cstdint>


int main()
{
//***********Inicialización de comunicación serie***************************//

    UnbufferedSerial pc(USBTX, USBRX);      //Configuración de comunicación serie a través de la UART.
    // Set desired properties (9600-8-N-1).
    pc.baud(9600);
    pc.format(8, SerialBase::None, 1);
//**************************************************************************//

//**********Declaracion de entradas y salidas*******************************//
    DHT11 d(D2);                            //Instancia de una variable de tipo DHT11.
    DigitalIn boton_buzzer(BUTTON1);        //Boton para activar o desactivar el buzzer.
    DigitalIn boton_ventilador(D6);         //Boton para activar o desactivar el ventilador.
    DigitalOut LED_rojo(LED1);              //LED indicador de sobretemperatura.
    DigitalOut LED_verde(D3);               //LED indicador de condiciones de temperatura y humedad normales.
    DigitalOut ventilador(D4);              //Salida para activar el ventilador.
    DigitalOut buzzer(D5);                  //Salida para activar el buzzer.
//**************************************************************************//

//*********Declaración de variables e inicialización de entradas-salidas****//
    char buffer[16];                        //Buffer donde se almacenará los bytes de datos de temperatura y humedad.
    int s;                                  //Variable donde se guardan los datos traidos por la función "readData()".     
    int umbral=20;                          //Valor umbral de temperatura.
    int tiempo_acumulado=0;             
    int tiempo_incremento=10;
    int cont=0;                             //Contador para función antirrebote al presionar botones.
    LED_rojo=OFF;                           //LED indicador de alarma.
    LED_verde=ON;                           //LED indicador de funcionamiento en condiciones normales.
    ventilador=LOW;         
    buzzer=LOW;                             //Apagado
//***************************************************************************//

//************************LOOP PRINCIPAL*************************************//
    while (true)
    {
        if(d.readTemperature()>umbral)      //Condición de sobretemperatura.
        {
            if(LED_rojo==OFF)               //Se encienden alarmas (LED + buzzer) y el ventilador.
            {
                LED_rojo=ON;
                LED_verde=OFF;
                buzzer=HIGH;
                ventilador=HIGH;
            }
        }
        else                                //Caso contrario se apagan las alarmas y se enciende LED verde.
        {
            if(LED_verde==OFF)
            {
                LED_verde=ON;
                LED_rojo=OFF;
                ventilador=LOW;
                buzzer=LOW;
            }
        }

///**********Presionado de botón**********************************///
///**********Este algoritmo de efecto antirrebote*****************///

        while(boton_buzzer==LOW)
        {
            delay(1);
            if(boton_buzzer==LOW && cont<=150)      
            {
                cont++;
                if(cont==150)
                {
                    buzzer=!buzzer;
                    cont=0;
                }
            }
        }
        while(boton_ventilador==LOW)
        {
            delay(1);
            if(boton_ventilador==LOW && cont<=150)
            {
                cont++;
                if(cont==150)
                {
                    ventilador=!ventilador;
                    cont=0;
                }
            }
        }

///***************************************************************///    
        
        if(tiempo_acumulado==2000)            //Condición de tiempo acumulador cumplido.
        {
        s=d.readData();                       //Lectura de datos del DHT.
        //printf("T:%d, H:%d\r\n", d.readTemperature(), d.readHumidity()); // esta instrucción ya imprime en pantala del monitor serie.
        sprintf(buffer, "T:%d, H:%d\r\n", d.readTemperature(), d.readHumidity());
        pc.write(buffer,16);                  //Transmisión de los datos ya convertidos a caracteres para presentar en pantalla.
        tiempo_acumulado=0;                   //Reinicio del contador de tiempo acumulador para presentar en pantalla cada 2 segundos los datos leídos del sensor.
        }
        delay(10); //delay de 10 ms
        tiempo_acumulado=tiempo_acumulado + tiempo_incremento;
    }
}