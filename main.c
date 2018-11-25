/*
===============================================================================
 Name        : TPO_QUIZA_FINAL.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "Aplicacion.h"
#include "main.h"

char PuntosALCD[10];
char PuntosBLCD[10];
char TiempoPartidoLCD[10];
volatile uint8_t PuntosA;
volatile uint8_t PuntosB;

//volatile uint8_t BufferEntradas;		//Buffer de Entradas Digitales

int A=000;

int main(void)
{
	uint8_t i, j;
	uint8_t n[4][8];
	uint8_t str[5]="hola";

	Inicializar();

	InitMatrix();

	for(i=0;i<8;i++)
	{
		for(j=0;j<4;j++)
		{
			stringConverter(j, str[j], n);
			sendCmd( (DIGIT+i) , n[j][i] );
		}
	}

    while(1)
    {
    	//Aplicacion();
    	//TimerEvent();
    }
}

void Delay(uint8_t time)
{
	for(uint8_t j = 0; j < time; j++);
}

void InitMatrix(void)
{
	uint32_t j;

	SetPINSEL(PDIN,0x00);
	SetPINSEL(PCLK,0x00);
	SetPINSEL(PCS,0x00);

	SetDIR(PDIN,SALIDA);
	SetDIR(PCLK,SALIDA);
	SetDIR(PCS,SALIDA);

	for(j=0;j<4;j++) { sendCmd(SHDOWN,SHUTDO); }
	for(j=0;j<4;j++) { sendCmd(SHDOWN,NOSHUT); }
	for(j=0;j<4;j++) { sendCmd(SLIMIT,MAXSCAN); }
	for(j=0;j<4;j++) { sendCmd(INTEN,MINTEN); }
	for(j=0;j<4;j++) { sendCmd(DEMODE,NODECO); }
	for(j=0;j<4;j++) { sendCmd(DSPTEST,NOROP); }
}

void stringConverter(uint8_t j, uint8_t l, uint8_t msg[][8])
{
	uint8_t i;
	int aux=l;
	aux=(aux-32)+(7*(aux-32));
	for(i=0;i<8;i++)
	{
		msg[j][i]=ucFont[aux+i];
	}
}

void sendCmd(uint8_t address, uint8_t data)	// Envía una trama
{
	uint8_t i;
	uint16_t frame= ( (address<<(0x08)) | (data) );

	SetPIN(PCLK,AP);
	SetPIN(PDIN,AP);
	SetPIN(PCS,AP);

	for(i=0;i<16;i++)
	{
		SetPIN( PDIN, ( ~(frame>>(15-i)) & (0x01)) );
		Delay(10);
		SetPIN(PCLK,EN);
		Delay(10);
		SetPIN(PCLK,AP);
		Delay(10);
	}
	SetPIN(PCS,EN);
}
