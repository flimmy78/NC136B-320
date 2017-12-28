/*******************************************************************************
 *   Filename:       bsp_wdt.c
 *   Revised:        All copyrights reserved to Roger.
 *   Date:           2015-08-11
 *   Revision:       v1.0
 *   Writer:	     wumingshen.
 *
 *   Description:    ���Ź�����ģ��
 *
 *
 *   Notes:
 *   �������Ź�����ԭ���ڼ�ֵ�Ĵ�����IWDG_KR����д��0XCCCC����ʼ���ö������Ź���
 *   ��ʱ��������ʼ���临λֵOXFFF�ݼ���������������������ĩβ0X000��ʱ�򣬻����
 *   һ����λ�źţ�IWDG_RESET�������ۺ�ʱ��ֻҪ�Ĵ���IWDG_KR�б�д��0XAAAA��IWDG_RLR
 *   �е�ֵ�ͻᱻ���¼��ص��������дӶ�����������Ź���λ��
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

// 0:��ֹ��1���ⲿ���Ź���2���ڲ����Ź���3��ͬʱʹ���ڲ����ⲿ���Ź�;
INT08U BSP_WdtMode  = 0;
/*******************************************************************************
 * ������ ��ģ���Ƿ����uCOSϵͳʹ��
 */
#define  UCOS_EN        DEF_ENABLED

/*******************************************************************************
* ��    �ƣ� BSP_WdtRst
* ��    �ܣ� ι��
* ��ڲ����� ��
* ���ڲ����� ��
* ��    �ߣ� ������
* �������ڣ� 2014-08-18
* ��    �ģ�
* �޸����ڣ�
* ��    ע��
*******************************************************************************/
void  BSP_WDT_Rst(void)
{
    if ( BSP_WdtMode == BSP_WDT_MODE_NONE )
        return;
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0u;
#endif
    /***********************************************
    * ������STM�ڲ����Ź�ι�� Reload IWDG counter
    */
    if ( ( BSP_WdtMode == BSP_WDT_MODE_INT ) || ( BSP_WdtMode == BSP_WDT_MODE_ALL ) ) {
        //IWDG_ReloadCounter();
        WDT_Feed(); 
    }
    /***********************************************
    * �������ⲿ���Ź�ι��
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
* ��    �ƣ� BSP_WDT_GetMode
* ��    �ܣ� ��ȡ����ʲô���Ź�
* ��ڲ����� ��
* ���ڲ����� ��
* ��    �ߣ� ������
* �������ڣ� 2014-08-18
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� ���ڲ��ģ��ⲿ�ģ������������ã���
*******************************************************************************/
uint8_t  BSP_WDT_GetMode(void)
{
    return BSP_WdtMode;
}

/*******************************************************************************
* ��    �ƣ� BSP_WDT_Init
* ��    �ܣ� �������Ź���ʼ��
* ��ڲ����� 0:��ֹ��1���ⲿ���Ź���2���ڲ����Ź���3��ͬʱʹ���ڲ����ⲿ���Ź�;
* ���ڲ����� 0����ʼ���ɹ� 1����ʼ��ʧ��
* ��    �ߣ� ������
* �������ڣ� 2014-08-18
* ��    �ģ�
* �޸����ڣ�
* ��    ע��
*******************************************************************************/
uint8_t BSP_WDT_Init(uint8_t mode)
{
    BSP_WdtMode = mode;

    if ( mode == BSP_WDT_MODE_NONE )  //��ֹ��
        return 0;

    if ( ( mode == BSP_WDT_MODE_INT ) || ( mode == BSP_WDT_MODE_ALL ) ) { //ʹ���ڲ��������ڲ��ⲿ��һ����
        WDT_Init(WDT_CLKSRC_IRC,WDT_MODE_RESET);
        WDT_Start(2000000);
    }
    if ( ( mode == BSP_WDT_MODE_EXT ) || ( mode == BSP_WDT_MODE_ALL ) ) { //ʹ���⹷�����ڹ��⹷һ����
        GPIO_PinselConfig(WDI_GPIO_PIN,0);	
    }

    return 1;
}

#define	Boot_APP_START_ADDR 0
/****************************************************************************
* ���ƣ�void  FeedDog(void)
* ���ܣ����Ź�ι��������
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void  ResetSys(void)
{  
	SCB->VTOR = Boot_APP_START_ADDR & 0x1FFFFF80;	//ƫ���ж�����
}

/****************************************************************************
* ���ƣ�void  FeedDog(void)
* ���ܣ����Ź�ι��������
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void  FeedDog(void)
{  
	WDT_Feed();
}

/*******************************************************************************
* ��    �ƣ� BSP_SystemSoftReset
* ��    �ܣ� ϵͳ�������
* ��ڲ����� ��
* ���ڲ����� ��
* ��    �ߣ� ������
* �������ڣ� 2014-08-18
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� ��Cortex-M3Ȩ��ָ����ָ������SYSRESETREQ ����Ϊ��Ч������λ������ִ�и�λ���
*       ��������һ����ʱ���ڴ���ʱ�ڼ䣬��������Ȼ������Ӧ�ж����󡣵����ǵı���������Ҫ
*       �ô˴�ִ�е���Ϊֹ����Ҫ�����κ����������ˡ����ԣ�����ڷ�����λ����ǰ���Ȱ�
*       FAULTMASK ��λ ��
*******************************************************************************/
void  BSP_RST_SystemRst(void)
{
#if (UCOS_EN == ENABLE)
    #if OS_CRITICAL_METHOD == 3u                    /* Allocate storage for CPU status register */
        OS_CPU_SR  cpu_sr = 0u;
        cpu_sr  = cpu_sr;
        cpu_sr  = 0u;
    #endif
    CPU_CRITICAL_ENTER();                           //�ر������ж�(cpu.h�ļ��ṩ�ú���)
#else
    __set_FAULTMASK(1);                             //�ر������ж�(core_cm3.h�ļ��ṩ�ú���)
#endif

    NVIC_SystemReset();                             // ������core_cm3.h�ļ��ṩ�ú�����
    
	SCB->VTOR = Boot_APP_START_ADDR & 0x1FFFFF80;	//ƫ���ж�����
    /***************************************************
    * ������ ��ֹϵͳ������������򿪿��Ź������ȴ�ϵͳ��λ
    */
    BSP_WDT_Init(BSP_WDT_MODE_ALL);
    while(1);
}

/*******************************************************************************
*              end of file                                                    *
*******************************************************************************/
#endif