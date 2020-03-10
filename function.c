#include <BS45F3235.H>
#include "BS45F3235.h"
#include "include.h"

void opa_adc(void)					//read opda0o
{
	unsigned int	adc_buf;
	unsigned char	a,buf0;
	initial_adc();
	_sadc1=0b10000000;
	_adcen=1;
	adc_16bit=0;
	
	for(a=0;a<=3;a++)
	{
		_start =0;
		_start =1;
		_start =0;
		while(_adbz==1)
		GCC_CLRWDT();
		
		adc_buf=_sadoh;
		buf0=_sadol;
		adc_buf=((adc_buf<<4)+(buf0>>4));
		if((a==1)||(a==2)||(a==3))
		{	
			adc_16bit+=adc_buf;
		}
	}
	_sadc0=0;		//Non-existed channel and disable adc function
	_sadc1=0;
	_sadc2=0;
}