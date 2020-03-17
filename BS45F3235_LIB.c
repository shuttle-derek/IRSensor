#include <BS45F3235.H>
#include "BS45F3235.h"
#include "include.h"

bit_type Flag1;

//unsigned int	adc_16bit,Status[10],Status_Max,Status_Min;
//unsigned char	Uart_L,Uart_H,M_1sec,i;
unsigned long int adc_32bit;
unsigned char UART_31, UART_23, UART_15, UART_7, under_threshold_count, i;
unsigned int adc_16bit, previous_adc, adc_count, val_count, adc_16bit_max, max_idx;
unsigned int myCounter;
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
	unsigned long adc32;

	adc32 	= adc_32bit;
    UART_31	=(adc32>>24);
	UART_23	=(adc32>>16);
	UART_15	=(adc32>>8);
	UART_7	=adc32;
	
	_utxen=1;
	
	_utxr_rxr=UART_31;
	while(!_utidle)
	GCC_CLRWDT();
	
	_utxr_rxr=UART_23;
	while(!_utidle)
	GCC_CLRWDT();
	
	_utxr_rxr=UART_15;
	while(!_utidle)
	GCC_CLRWDT();
	
	_utxr_rxr=UART_7;
	while(!_utidle)
	GCC_CLRWDT();
	
	_utxr_rxr=adc_16bit_max>>8;
	while(!_utidle)
	GCC_CLRWDT();
	
	_utxr_rxr=adc_16bit_max&0xff;
	while(!_utidle)
	GCC_CLRWDT();
	
	_utxr_rxr=max_idx&0xff;
	while(!_utidle)
	GCC_CLRWDT();
	
	_utxr_rxr=val_count&0xff;//adc_count>>8;
	while(!_utidle)
	GCC_CLRWDT();

	_utxr_rxr=adc_count&0xff;
	while(!_utidle)
	GCC_CLRWDT();
}
void main()
{
	if(_to==0)	
					//watch dog timeout flag
	{
		while(_hircf==0)			//hirc stable flag
		GCC_DELAY(100);
		GCC_DELAY(20000);			//wait lirc stable(cause no flag)
		
		clear_ram();
		initial();
		_idata=2;
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
		
		if(myCounter>=125)
		{
			
		rx_control=1;
		previous_adc=0; // pre adc value
		adc_32bit=0;
		val_count=0;
		adc_16bit_max=0;
		max_idx=0;
		_isgen=1;					//isink output
/*		rx_control=0;*/
		// Status[0] : count increse curve times , 
		while(adc_count < adc_duration)
		{
			opa_adc();
//			uart_sent();
			if (adc_16bit > start_threshold) {
				under_threshold_count=0;
				if (adc_count==0) {
					previous_adc=adc_16bit;
					adc_32bit=adc_16bit;
//					continue;
				}
				else {
					adc_32bit +=adc_16bit;
					previous_adc = adc_16bit;
				}
				if (adc_16bit_max < adc_16bit)
				{
					adc_16bit_max = adc_16bit;
					max_idx = adc_count;
				}
				val_count++;
			}
			else// if ( adc_16bit < end_threshold )
			{
				if (adc_16bit < previous_adc) {
					if (++under_threshold_count > 5) // continue lower then the adc threshold
						break;
				}			
			}
			adc_count++;
		}
//		adc_32bit >>=4;
		_isgen=0;
		uart_sent();
		adc_count=0;
		val_count=0;
		
			
			myCounter = 0;
		}
		else
		{
			myCounter++;
		}


		if(halt_sleep)				//halt function(sleep)
		{
			halt_set();
			GCC_HALT();
		}
		else
			GCC_DELAY(2000);
	}
}