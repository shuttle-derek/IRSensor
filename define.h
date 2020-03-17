/////////////////////////////
/////////////////////////////
/////////////////////////////	i/o sellection
#define	rx_control		_pb1
#define	sw_power		_pa1

/////////////////////////////
/////////////////////////////
/////////////////////////////	
#define	isink_delay		260			//tx power on delay
									//(this num with ir scan range is positive ratio)
									//(same as power consumption)			
#define	halt_sleep		1			//0:off		1:on

									//Vout=(1+R2/R1)*Vin
#define opa_R1			1			//0:4K		1:8K
#define opa_R2			0			//0:1120k	1:960k	2:800k
									//3:640k	4:480k	5:160k
#define wdt_timeout		0			//0:8ms			1:16ms		2:32ms
									//3:64ms		4:128ms		5:256ms
									//6:512ms		7:1024ms
#define start_threshold	0
//#define start_threshold	1000		// about 780 mV (0.78mV/unit)
//#define end_threshold	300			// about 234 mV (0.78mV/unit)
#define adc_duration	200			// about 5 ms (one operation needs 50 us)
///////////////////////////////////////		FLAG
typedef struct 
{
volatile unsigned  bit0 : 1;
volatile unsigned  bit1 : 1;
volatile unsigned  bit2 : 1;
volatile unsigned  bit3 : 1;
volatile unsigned  bit4 : 1;
volatile unsigned  bit5 : 1;
volatile unsigned  bit6 : 1;
volatile unsigned  bit7 : 1;
}bit_type;

//--- Flag1 define
extern bit_type Flag1;
#define F_0	  		 	Flag1.bit0
#define F_1    		  	Flag1.bit1
#define F_2			 	Flag1.bit2
#define F_3		    	Flag1.bit3
#define F_4				Flag1.bit4
#define F_5				Flag1.bit5
#define F_6				Flag1.bit6
#define F_7				Flag1.bit7

///////////////////////////////////////
extern volatile unsigned char	m_timer[3];
extern unsigned int		adc_16bit,ir_bgd,ir_ref,ir_range,Status[10];
extern unsigned char	adc_mode,led_mode,m_ir_time,m_ir_err,isink;
