#include "stm32f10x.h"                  // Device header

void Red_Init(void)		
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
}

uint8_t Red_Rdata(void)//获取有人无人的数据
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1);
}

