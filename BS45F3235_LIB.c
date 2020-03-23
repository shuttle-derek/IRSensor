#include <BS45F3235.H>
#include "BS45F3235.h"
#include "include.h"

bit_type Flag1;

#define DEBUG // Print raw data or distance only

unsigned int adc_16bit, max_adc;
unsigned int ktbl[11]={3680,1,104,83,39,31,21,15,9,9,19};
unsigned char distance,adc_count,i;
unsigned char myCounter;

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
	_utxen=1;
	
#ifdef DEBUG

	_utxr_rxr=(max_adc&0xff00)>>8;
	while(!_utidle)
	GCC_CLRWDT();
	
	_utxr_rxr=max_adc&0xff;
	while(!_utidle)
	GCC_CLRWDT();
	
	_utxr_rxr=distance;
	while(!_utidle)
	GCC_CLRWDT();
	
	_utxr_rxr=adc_count;
	while(!_utidle)
	GCC_CLRWDT();
	
#else
	
	_utxr_rxr=distance;
	while(!_utidle)
	GCC_CLRWDT();

#endif
}
void ktbl_init()
{
	for (i=1;i<11;i++)
	{
		ktbl[i]=ktbl[i-1]-(10*ktbl[i]);	
	}	
}

void transDist()
{
	if (max_adc > ktbl[0])
		distance=0x10; // bottom out	
	else if (max_adc>=ktbl[10]) {
		for (i=1;i<11;i++) {
			if (max_adc > ktbl[i]) {
				distance = (ktbl[i-1]-max_adc)/((ktbl[i-1]-ktbl[i])/10) + (i+2)*10;
				break;
			}
		}
	}
	else // no valid adc value, report error.
		distance=0xf0; // distance too far 
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
		_idata=3;
		_opda0en=1;					//enable opa
#ifdef OPAMP2		
		_opda1en=1;					//enable opa
#endif		
	}
	else
	{
		_to=0;
		wake_set();
	}
	ktbl_init();
	while(1)
	{	
		GCC_CLRWDT();
		
		if(myCounter>=15)
		{
			myCounter=0;
			rx_control=1;
			_isgen=1;					//isink output
			max_adc=0; // pre adc value
			adc_count=0;
			i=0;

			while(i < adc_duration)
			{
				opa_adc();
				if (adc_16bit > max_adc) {
					max_adc=adc_16bit;
					adc_count=i;
				}
				i++;
			}
			transDist();
			uart_sent();
			_isgen=0;
		}
		else
			myCounter++;

		if(halt_sleep)				//halt function(sleep)
		{
			halt_set();
			GCC_HALT();
		}
		else
			GCC_DELAY(2000);
	}
}