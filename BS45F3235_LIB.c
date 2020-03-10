#include <BS45F3235.H>
#include "BS45F3235.h"
#include "include.h"

bit_type Flag1;

unsigned int	adc_16bit,Status[10],Status_Max,Status_Min;
unsigned char	Uart_L,Uart_H,M_1sec,i;
///////////////////////////////////////////////////////////interrupt
//void __attribute((interrupt(0x10))) stm_p (void)
//{
//	_stmpf=0;							//512us
//}
/////////////////////////////////////////////////////////////
//void __attribute((interrupt(0x28))) uart (void)
//{
//	_usimf=0;
//}
///////////////////////////////////////////////////////////
void uart_sent(void)
{
	adc_16bit=(adc_16bit>>2);
	Uart_H=(adc_16bit>>8);
	Uart_L=(adc_16bit&0xff);
	_utxen=1;
	
	_utxr_rxr=Uart_H;
	while(!_utidle)
	GCC_CLRWDT();
	
	_utxr_rxr=Uart_L;
	while(!_utidle)
	GCC_CLRWDT();
	
	_utxr_rxr=0x0d;
	while(!_utidle)
	GCC_CLRWDT();
	
	_utxr_rxr=0x0a;
	while(!_utidle)
	GCC_CLRWDT();
}
void main()
{
	if(_to==0)						//watch dog timeout flag
	{
		while(_hircf==0)			//hirc stable flag
		GCC_DELAY(100);
		GCC_DELAY(20000);			//wait lirc stable(cause no flag)
		
		clear_ram();
		initial();
		_idata=10;
		_opda0en=1;					//enable opa
	}
	else
	{
		_to=0;
		wake_set();
	}
	while(1)
	{	
		GCC_CLRWDT();
		
		rx_control=1;
		_isgen=1;					//isink output
		GCC_DELAY(isink_delay);
		opa_adc();
		_isgen=0;
/*		rx_control=0;*/
		Status[i]=adc_16bit;
		if(i<9)
		{
			i++;
		}
		else
		{
			i=0;
		}
//		uart_sent();
		if(M_1sec>=62)
		{
			M_1sec=0;
			Status_Max=0x00;
			Status_Min=0xffff;
			for(i=0;i<10;i++)
			{
				if(Status[i]>Status_Max)
					Status_Max=Status[i];
				if(Status[i]<Status_Min)
					Status_Min=Status[i];
				adc_16bit+=Status[i];
			}
			adc_16bit-=(Status_Max+Status_Min);
			adc_16bit>>=3;
			uart_sent();
		}
		else
			M_1sec++;
			
		if(halt_sleep)				//halt function(sleep)
		{
			halt_set();
			GCC_HALT();
		}
		else
			GCC_DELAY(2000);
	}
}