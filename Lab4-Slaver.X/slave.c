////******************************************************************************
//   UNIVERSIDAD DEL VALLE DE GUATEMALA
//   IE3054 ELECTRÓNICA DIGITAL 2 
//   AUTOR: MICHELLE SERRANO
//   COMPILADOR: XC8 (v1.41), MPLAB X IDE (v6.00)
//   PROYECTO: LABORATORIO 3
//   HARDWARE: PIC16F887
//   CREADO: 02/02/2023

#pragma config FOSC = INTRC_NOCLKOUT  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF             // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF            // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF            // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF               // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF            // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF             // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF            // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF              // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V         // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF              // Flash Program Memory Self Write Enable bits (Write protection off)

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include "ADC.h"
#include <xc.h>

#define _XTAL_FREQ 8000000
//*****************************************************************************
// Definición de variables
//*****************************************************************************
unsigned char dato_adc;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
  if (PIR1bits.SSPIF == 1) //si se enciende la bandera
    {
        if(I2C_Error_Read() != 0) //rutina para verificar errores
        {
            I2C_Error_Data();
        }
        
        if(I2C_Read_Mode() == 1) //mandar datos
        {
            I2C_Write_Slave(dato_adc); //enviamos el ADC
        }
        PIR1bits.SSPIF = 0; //apagamos la bandera
    }
}
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    ADC_Init(AN0);
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        dato_adc = ADC_Read(0);
       __delay_ms(500);
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSELbits.ANS0 = 1;
    ANSELH = 0;
    
    TRISB = 0;
    TRISD = 0;
    
    PORTB = 0;
    PORTD = 0;
    I2C_Init_Slave(0x50);   
}
