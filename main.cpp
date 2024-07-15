#include "mbed.h"
#include "dht11.h"
#include "arm_book_lib.h"
#include <cstdint>

//**********Declaracion de botones buzzer y ventilador**********************//
 DigitalOut buzzer(D5);
 DigitalOut ventilador(D4);
 DigitalIn boton_buzzer(BUTTON1); 
 DigitalIn boton_ventilador(D6);
 //*************************************************************************//

// *********Variables de debounce*******************************************//
Ticker ticker1;                     //Instanciación de variable tipo Ticker para manejar la función de presioanr el botón del ventilador.
Ticker ticker2;                     //Instanciación de variable tipo Ticker para manejar la función de presioanr el botón del buzzer.
volatile int cont1 = 0;             //Contador de retardo cuando se presiona el botón ventilador.
volatile bool checking1 = false;    //Variable que chequea el estado del botón que se presiona.
volatile int cont2 = 0;             //Contador de retardo cuando se presiona el botón buzzer.
volatile bool checking2 = false;    //Variable que chequea el estado del botoón que se presiona.
 //*************************************************************************//

// *********Deteccion de estado y activacion/desactivacion de buzzer********//
void check_boton_buzzer()
 {
    if (boton_buzzer == 0) {                                // Botón presionado
        if (checking1) {
            cont1++;
            if (cont1 >= 150) {
                buzzer = !buzzer;                           // Alternar el estado del buzzer
                cont1 = 0;                                  // Reiniciar el contador
                checking1 = false;                          // Dejar de chequear
            }
        } else {
            checking1 = true;                               // Comenzar a chequear
            cont1 = 0;                                      // Reiniciar el contador
        }
    } else {
        checking1 = false;                                  // Botón no presionado
        cont1 = 0;                                          // Reiniciar el contador
    }
}
//*************************************************************************//

// ******Deteccion de estado y activacion/desactivacion de ventilador******//
void check_boton_ventilador() 
{
    if (boton_ventilador == 0) {                            // Botón presionado
        if (checking2) {
            cont2++;
            if (cont2 >= 150) {
                ventilador = !ventilador;                   // Alternar el estado del buzzer
                cont2 = 0;                                  // Reiniciar el contador
                checking2 = false;                          // Dejar de chequear
            }
        } else {
            checking2 = true;                               // Comenzar a chequear
            cont2 = 0;                                      // Reiniciar el contador
        }
    } else {
        checking2 = false;                                  // Botón no presionado
        cont2 = 0;                                          // Reiniciar el contador
    }
}
//*************************************************************************//


int main()
{
//***********Inicialización de comunicación serie***************************//
    UnbufferedSerial pc(USBTX, USBRX);              //Configuración de comunicación serie a través de la UART.
                                                    // Establecimiento de configuraciones (9600-8-N-1).
    pc.baud(9600);
    pc.format(8, SerialBase::None, 1);
//**************************************************************************//

//***********LLamado a las funciones cada 1 ms******************************//
ticker1.attach(&check_boton_ventilador, 0.001);   //Se configura ticker1 para llamar a la funcion cada 1 ms.
ticker2.attach(&check_boton_buzzer, 0.001);       //Se configura ticker2 para llamar a la funcion cada 1 ms.
//*************************************************************************//

//**********Declaracion de entradas y salidas*******************************//
    DHT11 sensordht(D2);                           //Instancia de una variable de tipo DHT11.
    //DigitalIn boton_buzzer(BUTTON1);             //Boton para activar o desactivar el buzzer.
    //DigitalIn boton_ventilador(D6);              //Boton para activar o desactivar el ventilador.
    DigitalOut LED_rojo(LED1);                     //LED indicador de sobretemperatura.
    DigitalOut LED_verde(D3);                      //LED indicador de condiciones de temperatura y humedad normales.
    //DigitalOut ventilador(D4);                   //Salida para activar el ventilador.
    //DigitalOut buzzer(D5);                       //Salida para activar el buzzer.
//**************************************************************************//

//*********Declaración de variables e inicialización de entradas-salidas****//
    char buffer[16];                               //Buffer donde se almacenará los bytes de datos de temperatura y humedad.
    int Lectura;                                   //Variable donde se guardan los datos traidos por la función "readData()".     
    int umbral=20;                                 //Valor umbral de temperatura.
    int tiempo_acumulado=0;             
    int tiempo_incremento=1;
    //int cont=0;                                  //Contador para función antirrebote al presionar botones.
    LED_rojo=OFF;                                  //LED indicador de alarma.
    LED_verde=ON;                                  //LED indicador de funcionamiento en condiciones normales.
    ventilador=LOW;         
    buzzer=LOW;                                    //Apagado
//***************************************************************************//

//************************LOOP PRINCIPAL*************************************//
    while (true)
    {
        if(sensordht.readTemperature()>umbral)     //Condición de sobretemperatura.
        {
            if(LED_rojo==OFF)                      //Se encienden alarmas (LED + buzzer) y el ventilador.
            {
                LED_rojo=ON;
                LED_verde=OFF;
                buzzer=HIGH;
                ventilador=HIGH;
            }
        }
        else                                      //Caso contrario se apagan las alarmas y se enciende LED verde.
        {
            if(LED_verde==OFF)
            {
                LED_verde=ON;
                LED_rojo=OFF;
                ventilador=LOW;
                buzzer=LOW;
            }
        }

///***************************************************************///       
        if(tiempo_acumulado==2000)            //Condición de tiempo acumulador cumplido.
        {
        Lectura=sensordht.readData();         //Lectura de datos del DHT.
        //printf("T:%d, H:%d\r\n", d.readTemperature(), d.readHumidity()); // esta instrucción ya imprime en pantala del monitor serie.
        sprintf(buffer, "T:%d, H:%d\r\n", sensordht.readTemperature(), sensordht.readHumidity());
        pc.write(buffer,16);                  //Transmisión de los datos ya convertidos a caracteres para presentar en pantalla.
        tiempo_acumulado=0;                   //Reinicio del contador de tiempo acumulador para presentar en pantalla cada 2 segundos los datos leídos del sensor.
        }
        delay(1); //delay de 1 ms
        tiempo_acumulado=tiempo_acumulado + tiempo_incremento;
    }
}
