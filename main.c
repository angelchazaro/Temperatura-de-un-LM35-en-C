/*
 * Sensor LM35 en C.c
 *
 * Created: 27/01/2019 07:17:34 p. m.
 * Author : dx_ch
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned int ValorADC,Temperatura;
unsigned char Dec,Uni;

int main(void)
{
	DDRB=0xff;
	PORTB=0xff;
	
	DDRC=0xff;
	PORTC=0xff;
	
	UCSRB=0x98;	//se habilita la rx y tx y la interrupcion de datos completa.
	UCSRC=0x86;	//modo asincrono, 8 bits de datos, sin paridad.
	UBRRL=51;	//9600 baudios a 8 Mhz
	
	TCNT1=0xc180;
	TCCR1B=0x01;
	TIMSK=0x01;
	
	ADMUX=0;	//Valor inicial del ADMUX
	ADCSRA=0x8D;
	ADCSRA|=1<<ADSC;	
	
	sei();
	
    /* Replace with your application code */
    while (1) 
    {
			Dec= Temperatura/10;
			Uni= Temperatura%10;
			
			UDR='T';					_delay_ms(10);
			UDR='e';					_delay_ms(10);
			UDR='m';					_delay_ms(10);
			UDR='p';					_delay_ms(10);
			UDR='e';					_delay_ms(10);
			UDR='r';					_delay_ms(10);
			UDR='a';					_delay_ms(10);
			UDR='t';					_delay_ms(10);
			UDR='u';					_delay_ms(10);
			UDR='r';					_delay_ms(10);
			UDR='a';					_delay_ms(10);
			UDR=':';					_delay_ms(10);
			UDR=' ';					_delay_ms(10);
			
		
			UDR= Dec+0x30;					_delay_ms(10);
			UDR= Uni+0x30;					_delay_ms(10);
			
			UDR='°';					_delay_ms(10);
			UDR='C';					_delay_ms(10);
		
			UDR= '\n';					_delay_ms(10);	//Nueva linea
			UDR= '\r';					_delay_ms(10);	//Retorno de carro (carriage return)
    }
}
ISR(TIMER1_OVF_vect)
{
	TCNT1=0xc180;		//Carga tcnt1 para que entre cada 1mSeg
	ADCSRA|=1<<ADSC;
}
ISR(ADC_vect)
{
	unsigned int ValorADC2;
	ValorADC=ADC;	//Variable asignada al ADC
	switch(ADMUX)	//Para utilizar varios canales del ADC, se usa un switch para leer un canal a la vez
	{
		case 0:		//Canal 0
		ADMUX=1;	//Switch al canal 1
		Temperatura=(5.0*ValorADC*100)/1023;
		PORTB=~Temperatura;
		break;
		
		case 1:		//Canal 1
		ADMUX=2;	//Switch al canal 2
		ValorADC2=(5.0*ValorADC*100)/1023;
		PORTC=~ValorADC2;
		break;
		
		case 2:		//Canal 2
		ADMUX=0;	//Switch al canal 0	(importante hacer un switch al canal 0 si se usa 1 canal o varios)
		break;
	
		default:
		break;
	}
	ADCSRA|=(1<<ADSC);
}

