/*******************************************************************************
 *   Filename:       bsp_wdt.c
 *   Revised:        All copyrights reserved to Roger.
 *   Date:           2015-08-11
 *   Revision:       v1.0
 *   Writer:	     wumingshen.
 *
 *   Description:    看门狗驱动模块
 *
 *
 *   Notes:
 *   独立看门狗工作原理：在键值寄存器（IWDG_KR）中写入0XCCCC，开始启用独立看门狗，
 *   此时计数器开始从其复位值OXFFF递减计数，当计数器计数到末尾0X000的时候，会产生
 *   一个复位信号（IWDG_RESET），无论何时，只要寄存器IWDG_KR中被写入0XAAAA，IWDG_RLR
 *   中的值就会被重新加载到计数器中从而避免产生看门狗复位。
 *
 *   All copyrights reserved to wumingshen
 *******************************************************************************/

/*******************************************************************************
* INCLUDES
*/
#include <includes.h>
#include <bsp_wdt.h>

#define BSP_WDT_MODULE_EN 1
#if BSP_WDT_MODULE_EN > 0

// 0:禁止；1：外部看门狗；2：内部看门狗；3：同时使用内部和外部看门狗;
INT08U BSP_WdtMode  = 0;
/*******************************************************************************
 * 描述： 该模块是否搭载uCOS系统使用
 */
#define  UCOS_EN        DEF_ENABLED

/*******************************************************************************
* 名    称： BSP_WdtRst
* 功    能： 喂狗
* 入口参数： 无
* 出口参数： 无
* 作    者： 无名沈
* 创建日期： 2014-08-18
* 修    改：
* 修改日期：
* 备    注：
*******************************************************************************/
void  BSP_WDT_Rst(void)
{
    if ( BSP_WdtMode == BSP_WDT_MODE_NONE )
        return;
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0u;
#endif
    /***********************************************
    * 描述：STM内部看门狗喂狗 Reload IWDG counter
    */
    if ( ( BSP_WdtMode == BSP_WDT_MODE_INT ) || ( BSP_WdtMode == BSP_WDT_MODE_ALL ) ) {
        //IWDG_ReloadCounter();
        WDT_Feed(); 
    }
    /***********************************************
    * 描述：外部看门狗喂狗
    */
    if ( ( BSP_WdtMode == BSP_WDT_MODE_EXT ) || ( BSP_WdtMode == BSP_WDT_MODE_ALL ) ) {
        //CPU_SR_ALLOC();
        //OS_CRITICAL_ENTER();
        //GPIO_SetBits(WDI_GPIO_PORT, WDI_GPIO_PIN); 
        GPIO_SetOrClearValue(WDI_GPIO_PIN,0);
        Delay_Nus(5);
        //GPIO_ResetBits(WDI_GPIO_PORT, WDI_GPIO_PIN);
        GPIO_SetOrClearValue(WDI_GPIO_PIN,1);
        //OS_CRITICAL_EXIT();
    }
}

/*******************************************************************************
* 名    称： BSP_WDT_GetMode
* 功    能： 获取用了什么看门狗
* 入口参数： 无
* 出口参数： 无
* 作    者： 无名沈
* 创建日期： 2014-08-18
* 修    改：
* 修改日期：
* 备    注： （内部的？外部的？还是两个都用？）
*******************************************************************************/
uint8_t  BSP_WDT_GetMode(void)
{
    return BSP_WdtMode;
}

/*******************************************************************************
* 名    称： BSP_WDT_Init
* 功    能： 独立看门狗初始化
* 入口参数： 0:禁止；1：外部看门狗；2：内部看门狗；3：同时使用内部和外部看门狗;
* 出口参数： 0：初始化成功 1：初始化失败
* 作    者： 无名沈
* 创建日期： 2014-08-18
* 修    改：
* 修改日期：
* 备    注：
*******************************************************************************/
uint8_t BSP_WDT_Init(uint8_t mode)
{
    BSP_WdtMode = mode;

    if ( mode == BSP_WDT_MODE_NONE )  //禁止狗
        return 0;

    if ( ( mode == BSP_WDT_MODE_INT ) || ( mode == BSP_WDT_MODE_ALL ) ) { //使用内部狗或者内部外部狗一起用
        WDT_Init(WDT_CLKSRC_IRC,WDT_MODE_RESET);
        WDT_Start(2000000);
    }
    if ( ( mode == BSP_WDT_MODE_EXT ) || ( mode == BSP_WDT_MODE_ALL ) ) { //使用外狗或者内狗外狗一起用
        GPIO_PinselConfig(WDI_GPIO_PIN,0);	
    }

    return 1;
}

#define	Boot_APP_START_ADDR 0
/****************************************************************************
* 名称：void  FeedDog(void)
* 功能：看门狗喂狗操作。
* 入口参数：无
* 出口参数：无
****************************************************************************/
void  ResetSys(void)
{  
	SCB->VTOR = Boot_APP_START_ADDR & 0x1FFFFF80;	//偏移中断向量
}

/****************************************************************************
* 名称：void  FeedDog(void)
* 功能：看门狗喂狗操作。
* 入口参数：无
* 出口参数：无
****************************************************************************/
void  FeedDog(void)
{  
	WDT_Feed();
}

/*******************************************************************************
* 名    称： BSP_SystemSoftReset
* 功    能： 系统软件重启
* 入口参数： 无
* 出口参数： 无
* 作    者： 无名沈
* 创建日期： 2014-08-18
* 修    改：
* 修改日期：
* 备    注： 在Cortex-M3权威指南中指出：从SYSRESETREQ 被置为有效，到复位发生器执行复位命令，
*       往往会有一个延时。在此延时期间，处理器仍然可以响应中断请求。但我们的本意往往是要
*       让此次执行到此为止，不要再做任何其它事情了。所以，最好在发出复位请求前，先把
*       FAULTMASK 置位 。
*******************************************************************************/
void  BSP_RST_SystemRst(void)
{
#if (UCOS_EN == ENABLE)
    #if OS_CRITICAL_METHOD == 3u                    /* Allocate storage for CPU status register */
        OS_CPU_SR  cpu_sr = 0u;
        cpu_sr  = cpu_sr;
        cpu_sr  = 0u;
    #endif
    CPU_CRITICAL_ENTER();                           //关闭所有中断(cpu.h文件提供该函数)
#else
    __set_FAULTMASK(1);                             //关闭所有中断(core_cm3.h文件提供该函数)
#endif

    NVIC_SystemReset();                             // 重启（core_cm3.h文件提供该函数）
    
	SCB->VTOR = Boot_APP_START_ADDR & 0x1FFFFF80;	//偏移中断向量
    /***************************************************
    * 描述： 防止系统软件重启出错，打开看门狗，死等待系统复位
    */
    BSP_WDT_Init(BSP_WDT_MODE_ALL);
    while(1);
}

/*******************************************************************************
*              end of file                                                    *
*******************************************************************************/
#endif