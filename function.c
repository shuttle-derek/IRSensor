#include <BS45F3235.H>
#include "BS45F3235.h"
#include "include.h"

void opa_adc(void)					//read opda0o
{
	unsigned int	buf;
	initial_adc();
#ifdef OPAMP2
	_sadc1=0b10010000;
#else	
	_sadc1=0b10000000;
#endif	
	_adcen=1;
	adc_16bit=0;
// A/D converter start
	_start =0;
	_start =1;
	_start =0;

	while(_adbz==1) // Wait A/D converter ready
		GCC_CLRWDT();
	buf=_sadoh;
	adc_16bit = (buf<<4) + (_sadol>>4) ;


	_sadc0=0;		//Non-existed channel and disable adc function
	_sadc1=0;
	_sadc2=0;
}