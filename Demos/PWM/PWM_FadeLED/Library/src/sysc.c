#include "air105.h"
#include "sysc.h"

///-------------------------------------------------------------------------------------------------
/// <summary>    Config system clock with HSI(Internal clock). </summary>
///
/// <remarks>    Tony Wang, 17:2 17/3/21. </remarks>
///-------------------------------------------------------------------------------------------------

void SystemClock_Config_HSI(void)
{
    SYSCTRL_PLLConfig(SYSCTRL_PLL_108MHz);
    SYSCTRL_PLLDivConfig(SYSCTRL_PLL_Div_None);
    SYSCTRL_HCLKConfig(SYSCTRL_HCLK_Div2);
    SYSCTRL_PCLKConfig(SYSCTRL_PCLK_Div2);
}

///-------------------------------------------------------------------------------------------------
/// <summary>    Config system clock with HSE(External clock). </summary>
///
/// <remarks>    Tony Wang, 16:57 17/3/21. </remarks>
///-------------------------------------------------------------------------------------------------

void SystemClock_Config_HSE(void)
{
    SYSCTRL_SYSCLKSourceSelect(SELECT_EXT12M);

    SystemClock_Config_HSI();
}
