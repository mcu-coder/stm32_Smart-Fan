#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//注释部分为另一种矩阵接法

unsigned char Key_KeyNumber;
void MatrixKey_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14|GPIO_Pin_15;// | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
}
/**
  * @brief  获取当前按键的状态，无消抖及松手检测
  * @param  无
  * @retval 按下按键的键码，范围：0,1~4,0表示无按键按下
  */
unsigned char Key_GetState()
{
	unsigned char Key_Y=0;
	
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==0){Key_Y=1;}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)==0){Key_Y=2;}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==0){Key_Y=3;}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0){Key_Y=4;}
	
	return Key_Y;
}

/**
  * @brief  按键驱动函数，在中断中调用
  * @param  无
  * @retval 无
  */
uint8_t Key_LoopY(void)		//第key_x行
{
	static uint8_t NowState,LastState;
	Key_KeyNumber = 0;
	LastState=NowState;				//按键第key_x行状态更新
	NowState=Key_GetState();		//获取当前第key_x行按键状态
	//如果上个时间点按键按下，这个时间点未按下，则是松手瞬间，以此避免消抖和松手检测
	if(LastState==1 && NowState==0)
	{
		Key_KeyNumber=1;
	}
	if(LastState==2 && NowState==0)
	{
		Key_KeyNumber=2;
	}
	if(LastState==3 && NowState==0)
	{
		Key_KeyNumber=3;
	}
	if(LastState==4 && NowState==0)
	{
		Key_KeyNumber=4;
	}
	
	return Key_KeyNumber;
}

