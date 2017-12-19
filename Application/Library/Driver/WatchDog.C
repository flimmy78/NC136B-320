//#include  "config.h"
#include <includes.h>

//#define		EX_DOG		sp706
#define		WATCHDOG	28		//P028

/****************************************************************************
* 名称：void  InitWatchDog(void)
* 功能：初始化看门狗
* 入口参数：无
* 出口参数：无
****************************************************************************/
void  InitWatchDog(void)
{
////	WDT_Init(WDT_CLKSRC_IRC,WDT_MODE_RESET);
////	WDT_Start(2000000);
////	FeedDog();
	GPIO_PinselConfig(WATCHDOG,0);	
//
//	FeedDog();

}

/****************************************************************************
* 名称：void  FeedDog(void)
* 功能：看门狗喂狗操作。
* 入口参数：无
* 出口参数：无
****************************************************************************/
void  FeedDog(void)
{  
	/////WDT_Feed();
//   if(GPIO_ReadIoVal(WATCHDOG))
//   {
//   		GPIO_SetOrClearValue(WATCHDOG,0);
//   }
//   else
//   {
//   		GPIO_SetOrClearValue(WATCHDOG,1);	
//   }
}



//__asm void JMP_Boot( uint32_t address ){
//   LDR SP, [R0]		;Load new stack pointer address
//   LDR PC, [R0, #4]	;Load new program counter address
//}

#define	Boot_APP_START_ADDR 0
/****************************************************************************
* 名称：void  FeedDog(void)
* 功能：看门狗喂狗操作。
* 入口参数：无
* 出口参数：无
****************************************************************************/
void  ResetSys(void)
{  
//	while(1);	
	//启动app
//void Boot( void )
//{
//	 void (*userProgram)();   					           /*函数指针*/

	SCB->VTOR = Boot_APP_START_ADDR & 0x1FFFFF80;	//偏移中断向量

////	JMP_Boot(Boot_APP_START_ADDR);
//	userProgram = (void (*)()) (USER_APP_START_ADDR+1);
//	(*userProgram)();													/*启动						*/	 
	
}


