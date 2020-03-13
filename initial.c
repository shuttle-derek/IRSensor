#include "BS45F3235.h"
#include "include.h"

void initial(void)
{
	_cks2=0;
	_cks1=1;
	_cks0=0;
	/////io initial/////
	_pa=0b00100000;
	_pac=0b10011010;
	_papu=0b00000000;
	_pawu=0b00000000;
	
	_pb=0b00000010;
	_pbc=0b00000100;
	_pbpu=0b00000100;
	
	_pas0=0b00000100;
	_pas1=0b10000001;
	_pbs0=0b00000010;
	
	_ifs=0x0c;
	_simc2=0;
	_simc0=0b00010000;		//uart pa7 pb0
	_uucr1=0b10000000;		//n81
	_uucr2=0b10000001;
	_ubrg=12;
	/////lirc/////
	switch(wdt_timeout)
	{
		case 0:
		_ws2=0;
		_ws1=0;
		_ws0=0;
		break;
		
		case 1:
		_ws2=0;
		_ws1=0;
		_ws0=1;
		break;
		
		case 2:
		_ws2=0;
		_ws1=1;
		_ws0=0;
		break;
		
		case 3:
		_ws2=0;
		_ws1=1;
		_ws0=1;
		break;
		
		case 4:
		_ws2=1;
		_ws1=0;
		_ws0=0;
		break;
		
		case 5:
		_ws2=1;
		_ws1=0;
		_ws0=1;
		break;
		
		case 6:
		_ws2=1;
		_ws1=1;
		_ws0=0;
		break;
		
		case 7:
		_ws2=1;
		_ws1=1;
		_ws0=1;
		break;
		
	}
	/////opa/////
	_opdc0=0b11100000;
	_opdc1=0b00000011;
	_opdccal=0x00;
	opa_offset();
	initial_opa();
	_opdda=0;
	_opdccal=0;
	/////adc/////
	initial_adc();
}
void initial_opa(void)
{
	/////opa/////
	_opdswa=0;
	_opdswb=0;
	_opdswc=0;
	_opdswd=0;
	_opdsw26=1;
	_opdsw3=1;
	_opdsw4=1;
	_opdsw27=1;
	/////opa0/////
	_opdsw2=1;
	_opdsw5=1;
	_opdsw6=1;
	_opda0ofm=0;
	_opdsw20=0;
	_opdsw21=0;
	_opdsw22=0;
	/////opa1/////
	_opdsw15=1;
	_opdsw16=0;
	_opdsw25=1;
	_opda1ofm=0;
		
	_opdc0=0b11100000;
	_opdc1=0b00000011;
	_opdccal=0x00;
}
void initial_adc(void)
{
	_sadc0=0b00001000;
	_sadc1=0b00000000;
	_sadc2=0b00000000;
}
void halt_set(void)
{
	_scc=0;
	_sadc0=0;
	_sadc1=0;
	_sadc2=0;
	_emi=0;	
}
void wake_set(void)
{
	_scc=0;						//wdt timeout
	initial_adc();
}
void clear_ram(void)
{
	_bp=0;
	for(_mp0=0xff;_mp0>0xbf;_mp0--)
	{
		GCC_CLRWDT();
		_iar0=0;
	}
}
void opa_offset(void)
{
	unsigned char	kopa_stat0=0,kopa_stat1=0,kopa1;

	_opdswa=0b11000000;		//OPDSW7~OPDSW0
	_opdswb=0b00000011;		//OPDSW15~OPDSW8
	_opdswc=0b00000000;		//OPDSW23~OPDSW16
	_opdswd=0b00000000;		//OPDSW28~OPDSW24

	_opda0cal=0xc0;			//Set OPDAnOF[5:0]=000000
	kopa1=_opd0o;
	for(_opda0cal=0b11000000;_opda0cal<0xff;_opda0cal++)
	{
		GCC_CLRWDT();
		if(kopa1!=_opd0o)
		{
			kopa_stat0=_opda0cal;
			break;
		}
		if(_opda0cal==0xfe)
		_opda0cal=0b11000000;
	}

	_opda0cal=0xff;
	kopa1=_opd0o;
	for(_opda0cal=0xff;_opda0cal>0xc0;_opda0cal--)
	{
		GCC_CLRWDT();
		if(kopa1!=_opd0o)
		{
			kopa_stat1=_opda0cal;
			break;
		}
		if(_opda0cal==0xc1)
		_opda0cal=0xff;
	}
	_opda0cal=((((kopa_stat1&0x3f)+(kopa_stat0&0x3f))>>1)+1);

///////////////////////////////////////////////////////////
	kopa_stat0=0;
	kopa_stat1=0;
	kopa1=0;

	_opdswa=0b00000000;		//OPDSW7~OPDSW0
	_opdswb=0b00000011;		//OPDSW15~OPDSW8
	_opdswc=0b00000010;		//OPDSW23~OPDSW16
	_opdswd=0b00000000;		//OPDSW28~OPDSW24

	_opda1cal=0xc0;			//Set OPDAnOF[5:0]=000000
	kopa1=_opd1o;
	for(_opda1cal=0b11000000;_opda1cal<0xff;_opda1cal++)
	{
		GCC_CLRWDT();
		if(kopa1!=_opd1o)
		{
			kopa_stat0=_opda1cal;
			break;
		}
		if(_opda1cal==0xfe)
		_opda1cal=0b11000000;
	}
	
	_opda1cal=0xff;
	kopa1=_opd1o;
	for(_opda1cal=0xff;_opda1cal>0xc0;_opda1cal--)
	{
		GCC_CLRWDT();
		if(kopa1!=_opd1o)
		{
			kopa_stat1=_opda1cal;
			break;
		}
		if(_opda1cal==0xc1)
		_opda1cal=0xff;
	}
	_opda1cal=((((kopa_stat1&0x3f)+(kopa_stat0&0x3f))>>1)+1);
}