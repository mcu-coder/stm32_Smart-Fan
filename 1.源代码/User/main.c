#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "MatrixKey.h"
#include "RED.h"
#include "DHT11.h"
#include <string.h>
#include "Timer.h"
#include "beep.h"
#include "Serial.h"

/*
按键	
OLED	SCL:PB8 SDA:PB9
TB6612	PWMA:PA2 AIN1:PA4 AIN2:PA5 AO1、AO2:接电机
温湿度	PA8
红外	PA1
蓝牙	RX:PA9 TX:PA10
*/



char RxData;					//接收蓝牙数据

uint8_t KeyNum,RED_Data,RH,TH;		//分别用于获取按键，红外，温度，湿度
int8_t Speed;		//风扇速度
uint8_t Key_M = 1,Time_demo;		//用于切换模式
uint8_t Time_1,Time_10,Time;		//个位和十位的时间,总时间
uint8_t temperature = 30;	//阈值设置
int8_t dang = 0;			//档数
uint8_t Num = 1,Timedemo;	//标志位


int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	Motor_Init();
	DHT11_Init(); 

	BEEP_GPIO_Config();
	
	OLED_ShowChinese(0,0,"温度");
	OLED_ShowChinese(16*4,0,"湿度");
	
	OLED_ShowChinese(0,16,"模式");
	OLED_ShowChinese(16*4,16,"档数");
	OLED_ShowNum(16*2,16,Key_M,1,OLED_8X16);
	OLED_ShowNum(16*6,16,dang,1,OLED_8X16);
	
	OLED_ShowChinese(0,16*2,"速度：");
	OLED_ShowSignedNum(16*3,16*2,Speed,3,OLED_8X16);
	
	OLED_ShowChinese(0,16*3,"倒计时：");
	OLED_ShowNum(16*4,16*3,Time,2,OLED_8X16);
	OLED_Update();
	
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
	Delay_ms(100);
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);

	while (1)
	{
		DHT11_Read_Data(&TH,&RH);//获取当前温湿度
		OLED_ShowNum(16*2,0,TH,2,OLED_8X16);
		OLED_ShowNum(16*6,0,RH,2,OLED_8X16);
		OLED_Update();
		 
		Delay_ms(500);

	}
}


uint16_t count;
uint8_t yy;

void TIM1_UP_IRQHandler(void)		//每1毫秒进一次中断
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		count++;
		if(count == 1000)
		{
			count = 0;
			if(Time_demo == 0 && Time > 0)	//进入倒计时模式
			{
				Time--;
				if(Time == 0)		//倒计时结束
				{
					Timedemo = 0;	//关闭自动挡
					Speed = 0;		//速度置0
					dang = 0;		//去除记录
					Time_1 = 0;
					Time_10 = 0;
					Motor_SetSpeed(0);
					OLED_ShowSignedNum(16*3,16*2,Speed,3,OLED_8X16);			
					OLED_ShowNum(16*6,16*1,dang,1,OLED_8X16);
				}
				OLED_ShowNum(16*4,16*3,Time,2,OLED_8X16);
				OLED_Update();
				
			}
		}
		if(count%20 == 0)	//获取按键值
		{
			RED_Data = Red_Rdata();		//检测是否有人
			 
			 
		}
		
		
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}

uint8_t RxData_demo,RxData_de,RxState;

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)		//判断是否是USART1的接收事件触发的中断
	{
		RxData = USART_ReceiveData(USART1);
//		RxData_demo = USART_ReceiveData(USART1);				//读取数据寄存器，存放在接收的数据变量
//		if(RxState == 0)
//		{
//			if(RxData_demo == 0xA5 && RxData == 0)		//包头
//			{
//				RxState = 1;
//			}
//		}
//		else if(RxState == 1)
//		{
//			RxData_de = RxData_demo;
//			RxState = 2;
//		}
//		else if(RxState == 2)
//		{
//			if(RxData_demo == 0x5A)		//包尾
//			{
//				RxState = 3;
//			}
//		}
//		if(RxState == 3)
//		{
//			RxState = 0;
//			RxData = RxData_de;
//		}

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);			//清除USART1的RXNE标志位
																//读取数据寄存器会自动清除此标志位
																//如果已经读取了数据寄存器，也可以不执行此代码
	}
}



