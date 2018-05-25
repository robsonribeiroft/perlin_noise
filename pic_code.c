/*
 * File:   newPerlin.c
 * Author: Robson Ribeiro
 *
 * Created on 25 de Maio de 2018, 14:02
 */


#include <stdio.h>
#include <stdlib.h>
#include<p18f4550.h>
#include<usart.h>
#include<delays.h>
/*
 * 
 */

#pragma config FOSC     = HS    /// EXTERN CLOCK 8MHZ
#pragma config IESO     = OFF   /// INTERNAL/EXTERNAL OSCILATOR DISABLE
#pragma config PWRT     = OFF   /// DISABLE POWER-UP TIMER
#pragma config BORV     = 3     /// BROWN-OUT RESET MINIMUM
#pragma config WDT      = OFF   /// DISABLE WATCHDOG TIMER
#pragma config WDTPS    = 32768 /// WATCHDOG TIMER 32768s
#pragma config MCLRE    = OFF   /// MASTER CLEAR PIN (RE3) DISBALE
#pragma config LPT1OSC  = OFF   /// TIMER1 LOW POWER OPERATION
#pragma config PBADEN   = OFF   /// PORTB.RB0,1,2,3,4 AS I/O DIGITAL
#pragma config STVREN   = ON    /// STACK FULL/UNDERFLOW CAUSE RESET
#pragma config LVP      = OFF   /// DISABLE LOW VOLTAGE PROGRAM (ICSP DISABLE)

rom int permutation[256] = { 151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
   };

double fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double lerp(double t, double a, double b) {
    return ((1 - t) * a + t * b);
}

double grad(int hash, double x, double y) {
    switch(hash & 3){
        case 0: return x + y;
        case 1: return -x + y;
        case 2: return x - y;
        case 3: return -x - y;
        default: return 0;
    }
}

double noise(double x, double y) {
    int xi, yi, g1, g2, g3, g4;
    double xf, yf, d1, d2, d3, d4, u, v, x1Inter, x2Inter, yInter;
    
    xi = (int) x & 255;
    yi = (int) y & 255;
    
    g1 = permutation[permutation[xi] + yi];
    g2 = permutation[permutation[xi + 1] + yi];
    g3 = permutation[permutation[xi] + yi + 1];
    g4 = permutation[permutation[xi + 1] + yi + 1];

    xf = x - (int) x;
    yf = y - (int) y;

    d1 = grad(g1, xf, yf);
    d2 = grad(g2, xf - 1, yf);
    d3 = grad(g3, xf, yf - 1);
    d4 = grad(g4, xf - 1, yf - 1);

    u = fade(xf);
    v = fade(yf);

    x1Inter = lerp(u, d1, d2);
    x2Inter = lerp(u, d3, d4);
    yInter = lerp(v, x1Inter, x2Inter);
    
    return yInter;
}

void send_Serial(int soma){
    int centena,dezena,unidade,n,aux;
    n = soma;
    
    if(soma >= 100){
        
        centena=n/100;
        centena = centena + 48;
        aux = n%100;
        dezena = aux/10;
        dezena = dezena + 48;
        aux = n%100;
        unidade = aux%10;
        unidade = unidade + 48;
        putcUSART(centena);
        Delay100TCYx(100);
        putcUSART(dezena);
        Delay100TCYx(100);
        putcUSART(unidade);
        putcUSART(32);
        
    }
    else{
        
        dezena = n/10;
        dezena = dezena + 48;
        aux = n%10;
        unidade = aux + 48;
        putcUSART(dezena);
        Delay100TCYx(100);
        putcUSART(unidade);
        putcUSART(32);
    }
    
    
}

void sendSerial(int value){
    int unit, dec, cent, aux;
    
    if (value>=100){
        cent = value/100;
        aux = value%100;
        dec = aux/10;
        aux = value%100;
        unit = aux%10;
        cent = cent + 48;
        dec = dec + 48;
        unit = unit + 48;
        
        putcUSART(cent);
        Delay100TCYx(100);
        putcUSART(dec);
        Delay100TCYx(100);
        putcUSART(unit);
        putcUSART(32);
        
    } else {
        dec = aux/10;
        aux = value%100;
        unit = aux%10;
        dec = dec + 48;
        unit = unit + 48;
        
        putcUSART(dec);
        Delay100TCYx(100);
        putcUSART(unit);
        putcUSART(32);
    }
}

void configurarUSART(){
	OpenUSART(USART_TX_INT_OFF &  // Transmit Interrupt OFF OBS: Tem no datasheet -> mudei para ON
            USART_RX_INT_ON & // Receive Interrupt ON OBS: Tem no datasheet -> mudei para ON
            USART_ASYNCH_MODE & // Asynchronous Mode OBS: Tem no datasheet
            USART_EIGHT_BIT & // 8-bit Transmit/Receive OBS: Tem no datasheet
            USART_CONT_RX & // Continuous Reception OBS: Pesquisar para qu? serve. Deve ter no datasheet
			  		USART_SYNC_SLAVE &
            USART_BRGH_HIGH, // High Baud Rate
    		  	51); // Baud Rate 9600 OBS: Formula no livro
	baudUSART(BAUD_8_BIT_RATE & BAUD_AUTO_OFF & BAUD_WAKEUP_OFF);
	
	BAUDCONbits.RXDTP = 0; //Inversao
	BAUDCONbits.TXCKP = 0; //Inversao
}

void main(void) {
    int i, j, holder;
    TRISC=0b10000000;
    configurarUSART();
    for (i = 50; i < 100; ++i) {
        for (j = 0; j < 50; ++j) {
           holder = (int) (noise((double)i/255, (double)j/255) * 255);
           send_Serial(holder);
        }
    }
    return;
}
