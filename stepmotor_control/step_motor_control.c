#include "stc12c5a60s2.h"
#include "intrins.h"
#define uchar unsigned char
#define uint unsigned int
//void delay1ms(void);
//void delay_nms(unsigned short int t);
void init(void);
uchar code turn[8] = {0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09}; //45 degree per time


uchar i = 0;
bit turn_direction_flag = 1;   // 0 - backward ; 1 - forward;
//bit start_stop_flag = 0; // 0 - stop;1 - start;
int length = 0;
int main()
{
	init();
	while(1){
		if(length > 8000){
			TR0 = ~TR0;
			TR1 = ~TR1;
			
			turn_direction_flag = 0;
			length = 8000;
			
		}
		
		if(length < 0){
			TR0 = ~TR0;
			TR1 = ~TR1;
			
			turn_direction_flag = 1;
			length = 0;
			
		}
	}
}

void init(){

	TMOD = 0x11;
	TH0 = 0xfc;  //1ms
	TL0 = 0x18;
	
	EA = 1;
	ET0 = 1;
	ET1 = 1;
	EX0 = 1;
	EX1 = 1;
	
	IT0 = 1;
	IT1 = 1;
	
	TR0 = 0;
	TR1 = 0;
}


// forward ,backward
void int0() interrupt 0
{
	turn_direction_flag = ~turn_direction_flag;
}

//start , stop
void int1() interrupt 2
{

	TR0 = ~TR0;
	TR1 = ~TR1;
}


//moto turn func
void timer0() interrupt 1
{
	
	TH0 = 0xfc;  //1ms
	TL0 = 0x18;

	
	
	P2 = turn[i];
	
	if(turn_direction_flag == 1){
		i--;  			//forward
		if(i == -1){
			i = 7;
		}
	}
	
	else{
		i++;				// backward
		if(i == 8){
			i = 0;
		}
	}
	
	

	

	
}
void timer1() interrupt 3
{
	TH1 = 0xfc;
	TL1 = 0x18;
	
	if(turn_direction_flag == 1){
		length++;
	}
	else{
		length--;
	}
}



/*
void delay1ms(void)   //?? -0.018084490741us
{
    unsigned char a,b;
    for(b=21;b>0;b--)
        for(a=130;a>0;a--);
    _nop_();  //if Keil,require use intrins.h
}

void delay_nms(unsigned short int t)
{
	unsigned short int i;
	for(i=0;i<t;i++){
		delay1ms();
	}
}

*/