

/**
 * Includes
 */

#ifndef __RSI_POWER_SAVE_H__
#define __RSI_POWER_SAVE_H__



#include "rsi_error.h"
#include "base_types.h"
#include "RS1xxxx.h"
#include "rsi_reg_spi.h"
#include "rsi_ipmu.h"

#ifdef __cplusplus
extern "C" {
#endif

/*M4SS peripheral power gates */
#define M4SS_PWRGATE_ULP_EXT_ROM            BIT(22)
#define M4SS_PWRGATE_ULP_M4_CORE            BIT(18)

#define M4SS_PWRGATE_ULP_M4_DEBUG           BIT(17)
#define M4SS_PWRGATE_ULP_M4_FPU             BIT(16)
#define M4SS_PWRGATE_ULP_ICACHE             BIT(15)
#define M4SS_PWRGATE_ULP_QSPI               BIT(13)
#define M4SS_PWRGATE_ULP_ETHERNET           BIT(12)
#define M4SS_PWRGATE_ULP_SDIO_SPI           BIT(11)
#define M4SS_PWRGATE_ULP_USB                BIT(10)
#define M4SS_PWRGATE_ULP_RPDMA              BIT(9)
#define M4SS_PWRGATE_ULP_PERI1              BIT(8)
#define M4SS_PWRGATE_ULP_PERI2              BIT(7)
#define M4SS_PWRGATE_ULP_PERI3              BIT(6)
#define M4SS_PWRGATE_ULP_CCI                BIT(5)
#define M4SS_PWRGATE_ULP_EFUSE              BIT(4)
#define M4SS_PWRGATE_ULP_SD_MEM             BIT(2)

/*ULPSS Peripheral power gate */
#define ULPSS_PWRGATE_ULP_MISC 	            BIT(18)
#define ULPSS_PWRGATE_ULP_CAP  	            BIT(19)
#define ULPSS_PWRGATE_ULP_VAD  	            BIT(20)
#define ULPSS_PWRGATE_ULP_UART 	            BIT(21)
#define ULPSS_PWRGATE_ULP_SSI  	            BIT(22)
#define ULPSS_PWRGATE_ULP_I2S  	            BIT(23)
#define ULPSS_PWRGATE_ULP_I2C  	            BIT(24)
#define ULPSS_PWRGATE_ULP_AUX  	            BIT(25)
#define ULPSS_PWRGATE_ULP_IR 	              BIT(26)
#define ULPSS_PWRGATE_ULP_UDMA 	            BIT(27)
#define ULPSS_PWRGATE_ULP_FIM 	            BIT(28)

/*NPSS Peripheral power gates */
#define SLPSS_PWRGATE_ULP_NWPAPB_MCU_CTRL   BIT(16)
#define SLPSS_PWRGATE_ULP_TIMEPERIOD        BIT(10)
#define SLPSS_PWRGATE_ULP_MCUSTORE3         BIT(9)
#define SLPSS_PWRGATE_ULP_MCUSTORE2         BIT(8)
#define SLPSS_PWRGATE_ULP_MCUSTORE1         BIT(7)
#define SLPSS_PWRGATE_ULP_MCUTS             BIT(6)
#define SLPSS_PWRGATE_ULP_MCUPS             BIT(5)
#define SLPSS_PWRGATE_ULP_MCUWDT            BIT(4)
#define SLPSS_PWRGATE_ULP_MCURTC            BIT(3)
#define SLPSS_PWRGATE_ULP_MCUFSM            BIT(2)
#define SLPSS_PWRGATE_ULP_MCUBFFS           BIT(1)

/*Power supply controls*/
#define ENABLE_WDT_IN_SLEEP                 BIT(0)
#define ENABLE_WURX_DETECTION               BIT(1)
#define RESET_MCU_BBF_DM_EN                 BIT(2)
#define DISABLE_TURNOFF_SRAM_PERI           BIT(3)
#define ENABLE_SRAM_DS_CTRL                 BIT(4)
#define POWER_ENABLE_FSM_PERI               BIT(16)
#define POWER_ENABLE_TIMESTAPING            BIT(17)
#define POWER_ENABLE_DEEPSLEEP_TIMER        BIT(18)
#define POWER_ENABLE_RETENTION_DM           BIT(19)

/*Wake up sources configuration*/
#define WDT_INTR_BASED_WAKEUP               BIT(29)
#define MSEC_BASED_WAKEUP                   BIT(28)
#define SEC_BASED_WAKEUP                    BIT(27)
#define	ALARM_BASED_WAKEUP                  BIT(26)
#define SDCSS_BASED_WAKEUP                  BIT(25)
#define	ULPSS_BASED_WAKEUP                  BIT(24)
#define WAKEIF_BASED_WAKEUP                 BIT(22)
#define COMPR_BASED_WAKEUP                  BIT(21)
#define GPIO_BASED_WAKEUP                   BIT(20)
#define M4_PROCS_BASED_WAKEUP               BIT(19)
#define WIRELESS_BASED_WAKEUP               BIT(18)
#define HOST_BASED_WAKEUP                   BIT(17)
#define DST_BASED_WAKEUP                    BIT(16)
#define WIC_BASED_WAKEUP                    BIT(23)

/*RAM retention controls */
#define M4ULP_RAM16K_RETENTION_MODE_EN      BIT(7)
#define ULPSS_RAM_RETENTION_MODE_EN         BIT(6)
#define TA_RAM_RETENTION_MODE_EN            BIT(5)
#define M4ULP_RAM_RETENTION_MODE_EN         BIT(4)
#define M4SS_RAM_RETENTION_MODE_EN          BIT(3)

/*RAM banks power gate controls*/
/*These banks typically used in M4SS high power mode */
#define RAM_BANK_0                          BIT(0) /*!< RAM BANK 0 SIZE : 16K*/
#define RAM_BANK_1                          BIT(1) /*!< RAM BANK 1 SIZE : 32K*/
#define RAM_BANK_2                          BIT(2) /*!< RAM BANK 2 SIZE : 16K*/
/*Typically used in M4 ULP mode*/
/*m4ulp_ram16k_retention_mode_en*/
#define RAM_BANK_3                          BIT(3) /*!< RAM BANK 3 SIZE : 4K*/
#define RAM_BANK_4                          BIT(4) /*!< RAM BANK 4 SIZE : 4K*/
#define RAM_BANK_5                          BIT(5) /*!< RAM BANK 4 SIZE : 4K*/
#define RAM_BANK_6                          BIT(6) /*!< RAM BANK 4 SIZE : 4K*/
#define RAM_BANK_7                          BIT(7) /*!< RAM BANK 4 SIZE : 32K*/
#define RAM_BANK_8                          BIT(8) /*!< RAM BANK 4 SIZE : 64K*/
#define RAM_BANK_9                          BIT(9) /*!< RAM BANK 4 SIZE : 16K*/
/*typically these banks are used in TASS */
#define RAM_BANK_10                         BIT(16) /*!< RAM BANK 4 SIZE : 16K*/
#define RAM_BANK_11                         BIT(17) /*!< RAM BANK 4 SIZE : 32K*/
#define RAM_BANK_12                         BIT(18) /*!< RAM BANK 4 SIZE : 80K*/
#define RAM_BANK_13                         BIT(19) /*!< RAM BANK 4 SIZE : 64K*/

/*ULPSS RAM banks power gates */
#define ULPSS_2K_BANK_0                     BIT(0)
#define ULPSS_2K_BANK_1                     BIT(1)
#define ULPSS_2K_BANK_2                     BIT(2)
#define ULPSS_2K_BANK_3                     BIT(3)
#define ULPSS_2K_BANK_4                     BIT(4)
#define ULPSS_2K_BANK_5                     BIT(5)
#define ULPSS_2K_BANK_6                     BIT(6)
#define ULPSS_2K_BANK_7                     BIT(7)

/*Controls the PMU off delay time*/
#define PMU_LDO_OFF_DELAY                   31U

/*LP control reg */
#define M4LP_CTRL_REG                       *((volatile uint32_t *)(0x24041424))
#define ULP_M4_CORE_CLK_ENABLE               BIT(2)

/*NPSS INTERRUPT */
#define NPSS_INTR_BASE                      0x12080000
#define NPSS_INTR_MASK_SET_REG              (*(volatile uint32_t *)( NPSS_INTR_BASE + 0x00))
#define NPSS_INTR_MASK_CLR_REG              (*(volatile uint32_t *)( NPSS_INTR_BASE + 0x04))
#define NPSS_INTR_CLEAR_REG                 (*(volatile uint32_t *)( NPSS_INTR_BASE + 0x08))
#define NPSS_INTR_STATUS_REG                (*(volatile uint32_t *)( NPSS_INTR_BASE + 0x0C))
#define M4_ULP_SLP_STATUS_REG               (*(volatile uint32_t *)( NPSS_INTR_BASE + 0x1C))
#define NWPAON_POR_CTRL_BITS                *(volatile uint32_t *)(0x41300000 + 0x3C)
#define POC_CNTRL_REG_0                     BIT(0)

/*NPSS interrupt numbers*/
#define NPSS_TO_MCU_WDT_INTR                BIT(0)
#define NPSS_TO_MCU_GPIO_INTR_0             BIT(1)
#define NPSS_TO_MCU_GPIO_INTR_1             BIT(2)
#define NPSS_TO_MCU_GPIO_INTR_2             BIT(3)
#define NPSS_TO_MCU_GPIO_INTR_3             BIT(4)
#define NPSS_TO_MCU_GPIO_INTR_4             BIT(5)

#define NPSS_TO_MCU_CMP_INTR_1              BIT(6)
#define NPSS_TO_MCU_CMP_INTR_2              BIT(7)
#define NPSS_TO_MCU_CMP_INTR_3              BIT(8)
#define NPSS_TO_MCU_CMP_INTR_4              BIT(9)

#define NPSS_TO_MCU_RFWAKEUP_INTR           BIT(10)

#define NPSS_TO_MCU_BOD_INTR                BIT(11)
#define NPSS_TO_MCU_BUTTON_INTR             BIT(12)

#define NPSS_TO_MCU_SDC_INTR                BIT(13)
#define NPSS_TO_MCU_WIRELESS_INTR           BIT(14)
#define NPSS_TO_MCU_WAKEUP_INTR             BIT(15)
#define NPSS_TO_MCU_ALARM_INTR              BIT(16)
#define NPSS_TO_MCU_SEC_INTR                BIT(17)
#define NPSS_TO_MCU_MSEC_INTR               BIT(18)
#define NPSS_TO_MCU_PROCESSOR_INTR          BIT(19)
#define NPSS_TO_MCU_HOST_INTR               BIT(20)
#define NPSS_TO_MCU_DST_INTR                BIT(21)

#define ENABLE_NEGEDGE_ULP                  BIT(1)
#define ULP_MODE_SWITCHED_NPSS              BIT(3)
#define ULP_MODE_AFTR_CLK_SW                BIT(2)

/*wake up status register */
#define CDBG_POWER_UP_REQUEST_WAKEUP        BIT(6)
#define WATCH_DOG_WINDOW_RESET_INTERRUPT    BIT(5)
#define WWD_RESET_LOOKUP_WAKEUP             BIT(4)
#define WATCHDOG_RESET_INTERRUPT            BIT(3)
#define HOST_BASED_WAKEUP_S                 BIT(2)
#define TIMEOUT_WAKEUP                      BIT(1)
#define WAKEUP_INDICATION                   BIT(0)

/*0x3C Wake up status clear */
#define MCU_WAKEUP_INTERRUPT                BIT(11)
#define RF_WAKEUP_CLEAR                     BIT(10)
#define COMP6_BASED_WAKEUP_STATUS_CLEAR     BIT(9)
#define COMP5_BASED_WAKEUP_STATUS_CLEAR     BIT(8)
#define COMP4_BASED_WAKEUP_STATUS_CLEAR     BIT(7)
#define COMP3_BASED_WAKEUP_STATUS_CLEAR     BIT(6)
#define COMP2_BASED_WAKEUP_STATUS_CLEAR     BIT(5)
#define COMP1_BASED_WAKEUP_STATUS_CLEAR     BIT(4)
#define RTC_ALARM_BASED_WAKEUP_STATUS_CLEAR BIT(3)
#define RTC_SEC_BASED_STATUS_CLEAR          BIT(2)
#define MILLI_SEC_BASED_STATUS_CLEAR        BIT(1)
#define WWD_INTERRUPT_STATUS_CLEAR          BIT(0)

/*PMU status configuration bits */
#define PMU_STS_DCDC_ON                     BIT(10)
#define PMU_STS_FLASH_LDO_ON                BIT(9)
#define PMU_STS_SOC_LDO_ON                  BIT(8)

/*PMU  */
//#define STANDBY_DC1P3_R  BIT(19)
#define STANDBY_LDOSOC_R BIT(18)
#define STANDBY_LDORF_R  BIT(17)
#define BGPMU_SLEEP_EN_R BIT(16)

/*Retention sleep configurations*/
#define RSI_WAKEUP_FROM_FLASH_MODE             1
#define RSI_WAKEUP_WITH_OUT_RETENTION          2
#define RSI_WAKEUP_WITH_RETENTION              3
#define RSI_WAKEUP_WITH_RETENTION_WO_ULPSS_RAM 4
#define RSI_WAKEUP_WO_RETENTION_WO_ULPSS_RAM   5

typedef enum FSM_CLK
{
	FSM_NO_CLOCK   = 0,  
	FSM_20MHZ_RO   = 1,
	FSM_32MHZ_RC   = 2,
	FSM_40MHZ_XTAL = 4
}FSM_CLK_T;

typedef enum AON_CLK
{
	KHZ_RO_CLK_SEL = 1 ,
	KHZ_RC_CLK_SEL = 2 ,
	KHZ_XTAL_CLK_SEL =4
}AON_CLK_T;

/*Ship modes*/
typedef enum SHUT_DOWN_WKP_MODE
{
	NPSS_GPIO_2_BASED = 0,
	NPSS_GPIO_1_BASED = 1,
	NPSS_GPIO_2_AND_3_BASED = 2,
	NPSS_GPIO_2_OR_3_BASED = 3,
}SHUT_DOWN_WKP_MODE_T;

/*m4ss context switch top ULP mode selection */
typedef enum ULP_MODE
{
	ULP_MCU_MODE  = 1 ,
	UULP_MCU_MODE = 3
}ULP_MODE_T;

/*NPSS Comparator selection*/
typedef enum NPSS_COMPARATOR{
	NPSS_CMP_1,
	NPSS_CMP_2,
	NPSS_CMP_3,
	NPSS_CMP_4,
	NPSS_CMP_5,
}NPSS_COMPARATOR_T;

#define _SOC_LDO                            3
#define MCU_SOC_LDO_LVL                     BIT(17)
#define MCU_DCDC_LVL                        BIT(18)

/*Select value for ULPSS RAM Power MUX*/
#define PWR_MUX_SEL_ULPSSRAM_SOC_LDO        3
#define PWR_MUX_SEL_ULPSSRAM_SCDC_0_9       1
#define PWR_MUX_SEL_ULPSSRAM_SCDC_0_6       0

/*Select value for M4 ULP RAM Power MUX*/
#define PWR_MUX_SEL_M4_ULP_RAM_SOC_LDO      3
#define PWR_MUX_SEL_M4_ULP_RAM_SCDC_0_9     1
#define PWR_MUX_SEL_M4_ULP_RAM_SCDC_0_6     0

/*Select value for M4 ULP RAM 16K Power MUX*/
#define PWR_MUX_SEL_M4_ULP_RAM16K_SOC_LDO   3
#define PWR_MUX_SEL_M4_ULP_RAM16K_SCDC_0_9  1
#define PWR_MUX_SEL_M4_ULP_RAM16K_SCDC_0_6  0

/*Select value for M4 ULP (Cortex Core)Power MUX*/
#define PWR_MUX_SEL_M4ULP_SOC_LDO           3
#define PWR_MUX_SEL_M4ULP_SCDC_0_9          1
#define PWR_MUX_SEL_M4ULP_SCDC_0_6          0

/*Select value for ULPSS (Peripherals)Power MUX*/
#define PWR_MUX_SEL_ULPSS_SOC_LDO           1
#define PWR_MUX_SEL_ULPSS_SCDC_0_9          0

/*BG SAMPLE ENABLE AND DISABLE /ENABLE MACROS*/
#define ENABLE_BG_SAMPLE_ENABLE             1
#define DISABLE_BG_SAMPLE_ENABLE            0

/*DC DC ENABLE AND DISABLE /ENABLE MACROS*/
#define ENABLE_DC_DC_ENABLE                 1
#define DISABLE_DC_DC_ENABLE                0

/*SOC LDO ENABLE AND DISABLE /ENABLE MACROS*/
#define ENABLE_SOCLDO_ENABLE                1
#define DISABLE_SOCLDO_ENABLE               0

/*STAND BY ENABLE AND DISABLE /ENABLE MACROS*/
#define ENABLE_STANDBYDC                    1
#define DISABLE_STANDBYDC                   0

/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup RSI_POWER_SAVE RSI:RS1xxxx POWER SAVE	
 *  @{
 *
 */
/**
 * @fn            STATIC INLINE void RSI_PS_PowerStateChangePs4toPs3(void)
 * @brief         This API is used to Change the power state from PS4 to PS3
 *                \n
 *                \ref MCU_PMU_LDO_CTRL_CLEAR
 */
STATIC INLINE void RSI_PS_PowerStateChangePs4toPs3(void)
{
	//FIXME: programming SOC LDO voltage high/low values who needs to program?
	/*Clear the MCU_SOC_LDO_LVL */
	BATT_FF->MCU_PMU_LDO_CTRL_CLEAR = MCU_SOC_LDO_LVL ;
}

/**
 * @fn            STATIC INLINE void RSI_PS_SetDcDcToHigerVoltage(void)
 * @brief         This API is used configure DCDC to give higher output voltage.
 *                \n
 *                \ref MCU_PMU_LDO_CTRL_SET
 */
STATIC INLINE void RSI_PS_SetDcDcToHigerVoltage(void)
{
	BATT_FF->MCU_PMU_LDO_CTRL_SET = MCU_DCDC_LVL ;
}

/**
 * @fn            STATIC INLINE void RSI_PS_SetDcDcToLowerVoltage(void)
 * @brief         This API is used configure DCDC to give lower output voltage.
 *                \n
 *                \ref MCU_PMU_LDO_CTRL_CLEAR
 */
STATIC INLINE void RSI_PS_SetDcDcToLowerVoltage(void)
{
	BATT_FF->MCU_PMU_LDO_CTRL_CLEAR = MCU_DCDC_LVL ;
}

/**
 * @fn            STATIC INLINE void RSI_PS_PowerStateChangePs3toPs4(void)
 * @brief         This API is used to Change the power state from PS3 to PS4
 *                \n
 *                \ref MCU_PMU_LDO_CTRL_SET
 */
STATIC INLINE void RSI_PS_PowerStateChangePs3toPs4(void)
{
	BATT_FF->MCU_PMU_LDO_CTRL_SET = MCU_SOC_LDO_LVL;
	return ;
}

/**
 * @fn            STATIC INLINE void RSI_PS_M4ssPeriPowerDown(uint32_t mask)
 * @brief	  This API is used to power gate the M4SS peripherals
 * @param[in]     mask  OR'ed value of the power gates
 *                \n
 *                \ref M4SS_PWRCTRL_CLEAR_REG
 * @return 	  none
 */
STATIC INLINE void RSI_PS_M4ssPeriPowerDown(uint32_t mask)
{
	BATT_FF->M4SS_PWRCTRL_CLEAR_REG = mask;
}

/**
 * @fn            STATIC INLINE void RSI_PS_M4ssPeriPowerUp(uint32_t mask)
 * @brief	  This API is used to un power gate the M4SS peripherals
 * @param[in]     mask  OR'ed value of the power gates
 *                \n
 *                \ref M4SS_PWRCTRL_SET_REG
 * @return 	  none
 */
STATIC INLINE void RSI_PS_M4ssPeriPowerUp(uint32_t mask)
{
	BATT_FF->M4SS_PWRCTRL_SET_REG = mask;
}

/**
 * @fn            STATIC INLINE void RSI_PS_M4ss_Tass_Ctrl_Clear(uint32_t mask)
 * @brief	  This API is used to clear the M4SS TASS controls
 * @param[in]     mask  OR'ed value of the power gates
 *                \n
 *                \ref M4SS_TASS_CTRL_CLEAR_REG
 * @return 	  none
 */
STATIC INLINE void RSI_PS_M4ss_Tass_Ctrl_Clear(uint32_t mask)
{
	BATT_FF->M4SS_TASS_CTRL_CLEAR_REG = mask;
}

/**
 * @fn            STATIC INLINE void RSI_PS_UlpssPeriPowerDown(uint32_t mask)
 * @brief	  This API is used to  power gate the ULPSS peripherals
 * @param[in]     mask  OR'ed value of the power gates
 *                \n
 *                \ref  ULPSS_PWRCTRL_CLEAR_REG
 * @return 	  none
 */
STATIC INLINE void RSI_PS_UlpssPeriPowerDown(uint32_t mask)
{
	BATT_FF->ULPSS_PWRCTRL_CLEAR_REG= mask;
}

/**
 * @fn            STATIC INLINE void RSI_PS_UlpssPeriPowerUp(uint32_t mask)
 * @brief	  This API is used to un power gate the ULPSS peripherals
 * @param[in]     mask  OR'ed value of the power domains
 *                \n
 *                \ref ULPSS_PWRCTRL_SET_REG
 * @return 	  none
 */
STATIC INLINE void RSI_PS_UlpssPeriPowerUp(uint32_t mask)
{
	BATT_FF->ULPSS_PWRCTRL_SET_REG = mask;
}

/**
 * @fn            STATIC INLINE void RSI_PS_NpssPeriPowerUp(uint32_t mask) 
 * @brief	  This API is used to un power gate the NPSS peripherals
 * @param[in]     mask  OR'ed value of the power domains
 *                \n
 *                \ref MCUAON_NPSS_PWRCTRL_SET_REG
 * @return 	  none
 */
STATIC INLINE void RSI_PS_NpssPeriPowerUp(uint32_t mask)
{
	MCU_AON->MCUAON_NPSS_PWRCTRL_SET_REG = mask;
}

/**
 * @fn            STATIC INLINE void RSI_PS_NpssPeriPowerDown(uint32_t mask) 
 * @brief	  This API is used to power gate the NPSS peripherals
 * @param[in]     mask  OR'ed value of the power domains
 *                \n
 *                \ref MCUAON_NPSS_PWRCTRL_CLEAR_REG
 * @return 	  none
 */
STATIC INLINE void RSI_PS_NpssPeriPowerDown(uint32_t mask)
{
	MCU_AON->MCUAON_NPSS_PWRCTRL_CLEAR_REG = mask;
}

/**
 * @fn            STATIC INLINE void RSI_PS_M4ssRamBanksPowerDown(uint32_t mask)
 * @brief	  This API is used to power gate the M4SS RAM Banks
 * @param[in]     mask  OR'ed value of the RAM power gates
 *                \n
 *                \ref M4_SRAM_PWRCTRL_CLEAR_REG1 
 * @return 	  none
 */
STATIC INLINE void RSI_PS_M4ssRamBanksPowerDown(uint32_t mask)
{
	BATT_FF->M4_SRAM_PWRCTRL_CLEAR_REG1 = mask;
}

/**
 * @fn            STATIC INLINE void RSI_PS_M4ssRamBanksPowerUp(uint32_t mask)
 * @brief	  This API is used to un power gate the M4SS RAM Banks
 * @param[in]     mask  OR'ed value of the RAM power gates
 *                \n
 *                \ref M4_SRAM_PWRCTRL_SET_REG1
 * @return        none
 */
STATIC INLINE void RSI_PS_M4ssRamBanksPowerUp(uint32_t mask)
{
	BATT_FF->M4_SRAM_PWRCTRL_SET_REG1 = mask;
}

/**
 * @fn            STATIC INLINE uint32_t RSI_PS_M4ssRamBanksGetPowerSts(void)
 * @brief	  This API is used to get the power gate status of M4SS RAM Banks
 *                \ref M4_SRAM_PWRCTRL_SET_REG1
 * @return        Ored bits of M4_SRAM_PWRCTRL_SET_REG1 reg
 */
STATIC INLINE uint32_t RSI_PS_M4ssRamBanksGetPowerSts(void)
{
	return BATT_FF->M4_SRAM_PWRCTRL_SET_REG1;
}

/**
 * @fn            STATIC INLINE void RSI_PS_M4ssRamBanksPeriPowerDown(uint32_t mask)
 * @brief	  This API is used to power gate the M4SS RAM Banks periphery domain
 * @param[in]     mask  OR'ed value of the RAM power gates
 *                \n
 *                \ref M4_SRAM_PWRCTRL_CLEAR_REG1
 * @return 	  none
 */
STATIC INLINE void RSI_PS_M4ssRamBanksPeriPowerDown(uint32_t mask)
{
	BATT_FF->M4_SRAM_PWRCTRL_CLEAR_REG2 = mask;
}

/**
 * @fn           STATIC INLINE void RSI_PS_M4ssRamBanksPeriPowerUp(uint32_t mask)
 * @brief	  This API is used to un-power gate the M4SS RAM Banks periphery domain
 * @param[in]     mask  OR'ed value of the RAM power gates
 *                \n
 *                \ref M4_SRAM_PWRCTRL_SET_REG1
 * @return        none
 */
STATIC INLINE void RSI_PS_M4ssRamBanksPeriPowerUp(uint32_t mask)
{
	BATT_FF->M4_SRAM_PWRCTRL_SET_REG2 = mask;
}

/**
 * @fn           STATIC INLINE uint32_t RSI_PS_M4ssRamBanksGetPeriPowerSts(void)
 * @brief	  This API is used to get the power gate status of M4SS RAM Banks periphery domain
 *                \ref M4_SRAM_PWRCTRL_SET_REG1
 * @return        Ored bits of M4_SRAM_PWRCTRL_SET_REG2 register.
 */
STATIC INLINE uint32_t RSI_PS_M4ssRamBanksGetPeriPowerSts(void)
{
	return BATT_FF->M4_SRAM_PWRCTRL_SET_REG2;
}
/**
 * @fn            STATIC INLINE void RSI_PS_UlpssRamBanksPeriPowerDown(uint32_t mask)
 * @brief	  This API is used to power gate the ULPSS RAM Banks periphery domain
 * @param[in]     mask  OR'ed value of the RAM power gates
 *                \n
 *                \ref M4_SRAM_PWRCTRL_CLEAR_REG1
 * @return 	  none
 */
STATIC INLINE void RSI_PS_UlpssRamBanksPeriPowerDown(uint32_t mask)
{
	BATT_FF->ULPSS_RAM_PWRCTRL_CLEAR_REG3 = mask;
}

/**
 * @fn           STATIC INLINE void RSI_PS_UlpssRamBanksPeriPowerUp(uint32_t mask)
 * @brief	  This API is used to un-power gate the ULPSS RAM Banks periphery domain
 * @param[in]     mask  OR'ed value of the RAM power gates
 *                \n
 *                \ref M4_SRAM_PWRCTRL_SET_REG1
 * @return        none
 */
STATIC INLINE void RSI_PS_UlpssRamBanksPeriPowerUp(uint32_t mask)
{
	BATT_FF->ULPSS_RAM_PWRCTRL_SET_REG3 = mask;
}

/**
 * @fn            STATIC INLINE void RSI_PS_SetRamRetention(uint32_t ramRetention)
 * @brief	  This API is used to set the RAM retention enable for the RAM during sleep
 * @param[in]     ramRetention OR'ed value of the RAM retention bits
 *                \n
 *                \ref MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE
 * @return 	  none
 */
STATIC INLINE void RSI_PS_SetRamRetention(uint32_t ramRetention)
{
	MCU_FSM->MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE |= ramRetention;
}

/**
 * @fn            STATIC INLINE void RSI_PS_ClrRamRetention(uint32_t ramRetention)
 * @brief	  This API is used to clear the RAM retention enable for the RAM during sleep
 * @param[in]     ramRetention  OR'ed value of the RAM retention bits
 *                \n
 *                \ref MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE
 * @return 	  none
 */
STATIC INLINE void RSI_PS_ClrRamRetention(uint32_t ramRetention)
{
	MCU_FSM->MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE &= ~ramRetention;
}

/**
 * @fn            STATIC INLINE void RSI_PS_UlpssRamBanksPowerDown(uint32_t mask)
 * @brief	  This API is used to  power gate the ULPSS RAM Banks
 * @param[in]     mask  OR'ed value of the RAM power gates
 *                \n
 *                \ref ULPSS_RAM_PWRCTRL_CLEAR_REG1
 * @return 	  none
 */
STATIC INLINE void RSI_PS_UlpssRamBanksPowerDown(uint32_t mask)
{
	BATT_FF->ULPSS_RAM_PWRCTRL_CLEAR_REG1 = mask;
}

/**
 * @fn            STATIC INLINE void RSI_PS_UlpssRamBanksPowerUp(uint32_t mask)
 * @brief	  This API is used to  un power gate the ULPSS RAM Banks
 * @param[in]     mask  OR'ed value of the RAM power gates
 *                \n
 *                \ref ULPSS_RAM_PWRCTRL_SET_REG1
 * @return 	  none
 */
STATIC INLINE void RSI_PS_UlpssRamBanksPowerUp(uint32_t mask)
{
	BATT_FF->ULPSS_RAM_PWRCTRL_SET_REG1 = mask;
}

/**
 * @fn            STATIC INLINE void RSI_PS_SetWkpSources(uint32_t wakeUpsrcMask)
 * @brief	  This API is used to set the wake up source to wake up from deep sleep
 * @param[in]     wakeUpsrcMask OR'ed value of the wake up sources
 *                \n
 *                \ref MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE
 * @return 	  none
 */
STATIC INLINE void RSI_PS_SetWkpSources(uint32_t wakeUpsrcMask)
{
	MCU_FSM->MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE |= wakeUpsrcMask;
}

/**
 * @fn            STATIC INLINE void RSI_PS_ClrWkpSources(uint32_t wakeUpsrcMask)
 * @brief	  This API is used to clear the wake up source
 * @param[in]     wakeUpsrcMask OR'ed value of the wake up sources
 *                \ref MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE
 * @return 	  none
 */
STATIC INLINE void RSI_PS_ClrWkpSources(uint32_t wakeUpsrcMask)
{
	MCU_FSM->MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE &= ~wakeUpsrcMask;
}

/**
 * @fn            STATIC INLINE uint32_t RSI_PS_GetWkpSources(void)
 * @brief	  This API is used to get the wake up source
 * @return 	  register bits of wake up sources
 */
STATIC INLINE uint32_t RSI_PS_GetWkpSources(void)
{
	return MCU_FSM->MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE;
}

/**
 * @fn            STATIC INLINE void RSI_PS_EnableFirstBootUp(boolean_t enable)
 * @brief	  This API is used to SET and CLEAR the First boot up bit
 * @param[in]     enable : 
 *                \ref MCU_FSM_CLK_ENS_AND_FIRST_BOOTUP_b  0: disable the first boot , 
 *                \ref MCU_FSM_CLK_ENS_AND_FIRST_BOOTUP_b  1: enable the first boot up
 * @return 	  none
 */
STATIC INLINE void RSI_PS_EnableFirstBootUp(boolean_t enable)
{
	MCU_FSM->MCU_FSM_CLK_ENS_AND_FIRST_BOOTUP_b.FIRST_BOOTUP_MCU_N_b = enable;
}

/**
 * @fn            STATIC INLINE void RSI_PS_PowerSupplyEnable(uint32_t mask)
 * @brief	  This API is used to enable the supply to some NPSS peripherals
 * @param[in]     mask 0: disable the first boot ,  1: enable the first boot up
 * @return 	  none
 */
STATIC INLINE void RSI_PS_PowerSupplyEnable(uint32_t mask)
{
	MCU_FSM->MCU_FSM_CRTL_PDM_AND_ENABLES |= mask;
}

/**
 * @fn            STATIC INLINE void RSI_PS_PowerSupplyDisable(uint32_t mask)
 * @brief	  This API is used to disable the supply to some NPSS peripherals
 * @param[in]     mask  0: disable the first boot ,  1: enable the first boot up
 *                \ref MCU_FSM_CRTL_PDM_AND_ENABLES
 * @return 	  none
 */
STATIC INLINE void RSI_PS_PowerSupplyDisable(uint32_t mask)
{
	MCU_FSM->MCU_FSM_CRTL_PDM_AND_ENABLES &= ~mask;
}

/**
 * @fn            STATIC INLINE void RSI_PS_FsmHfClkSel(FSM_CLK_T fsmHfClk)
 * @brief	  This API is used to  configure the FSM high frequency clock
 * @param[in]     fsmHfClk : enum value of the high frequency clock sources
 *                \ref MCU_FSM_CLKS_REG_b
 * @return 	  none
 */
STATIC INLINE void RSI_PS_FsmHfClkSel(FSM_CLK_T fsmHfClk)
{
	MCU_FSM->MCU_FSM_CLKS_REG_b.HF_FSM_CLK_SELECT = fsmHfClk;
	//FIXME:wait logic cross check
	while(MCU_FSM->MCU_FSM_CLKS_REG_b.HF_FSM_CLK_SWITCHED_SYNC != 1);
}

/**
 * @fn            STATIC INLINE void RSI_PS_FsmHfFreqConfig(uint32_t freq)
 * @brief	  This API is used to  configure the FSM high frequency clock range
 * @param[in]     freq : frequency in MHz pass 2 if it is 2MHz
 * @return 	  none
 */
STATIC INLINE void RSI_PS_FsmHfFreqConfig(uint32_t freq)
{
	MCU_FSM->MCU_FSM_CLKS_REG_b.HF_FSM_CLK_FREQ = freq;
}

/**
 * @fn            STATIC INLINE void RSI_PS_FsmLfClkSel(AON_CLK_T fsmLfClk)
 * @brief	  This API is used to  configure the FSM low frequency clock
 * @param[in]     fsmLfClk enum value of the low frequency clock sources
 *                \ref MCUAON_KHZ_CLK_SEL_POR_RESET_STATUS_b
 * @return 	  none
 */
STATIC INLINE void RSI_PS_FsmLfClkSel(AON_CLK_T fsmLfClk)
{
	MCU_AON->MCUAON_KHZ_CLK_SEL_POR_RESET_STATUS_b.AON_KHZ_CLK_SEL = fsmLfClk;
	//FIXME:wait logic check if it is working RTC blocked mode
	while(MCU_AON->MCUAON_KHZ_CLK_SEL_POR_RESET_STATUS_b.AON_KHZ_CLK_SEL_CLOCK_SWITCHED != 1);
}

/**
 * @fn            STATIC INLINE void RSI_PS_PmuGoodTimeDurationConfig(uint8_t pmuDuration) 
 * @brief	  This API is used to  configure the PMU good time.
 * @param[in]     pmuDuration  (0 to 31) are possible value is applied in power of 2.
 * @return 	  none
 */
STATIC INLINE void RSI_PS_PmuGoodTimeDurationConfig(uint8_t pmuDuration)
{
	MCU_FSM->MCU_FSM_XTAL_AND_PMU_GOOD_COUNT_REG_b.MCUFSM_PMU_POWERGOOD_DURATION_COUNT = pmuDuration;
}

/**
 * @fn            STATIC INLINE void  RSI_PS_XtalGoodTimeDurationConfig(uint8_t xtalDuration)
 * @brief	  This API is used to  configure the XTAL good time.
 * @param[in]     xtalDuration  (0 to 31) are possible value is applied in power of 2.
 *                \ref MCU_FSM_XTAL_AND_PMU_GOOD_COUNT_REG_b
 * @return 	  none
 */
STATIC INLINE void  RSI_PS_XtalGoodTimeDurationConfig(uint8_t xtalDuration)
{
	MCU_FSM->MCU_FSM_XTAL_AND_PMU_GOOD_COUNT_REG_b.MCUFSM_XTAL_GOODTIME_DURATION_COUNT = xtalDuration;
}

/**
 * @fn            STATIC INLINE void RSI_PS_Ps2PmuLdoOffDelayConfig(uint8_t ldoOffDelay)
 * @brief	  This API is used to  configure LDO off delay
 * @param[in]     ldoOffDelay  (0 to 31) are possible value is applied in power of 2.
 * @return 	  none
 */
STATIC INLINE void RSI_PS_Ps2PmuLdoOffDelayConfig(uint8_t ldoOffDelay)
{
	MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.PS2_PMU_LDO_OFF_DELAY = ldoOffDelay;
}

/**
 * @fn            STATIC INLINE void RSI_PS_Ps4PmuLdoOnDelayConfig(uint8_t ldoOnDelay)
 * @brief	  This API is used to  configure LDO on delay
 * @param[in]     ldoOnDelay  (0 to 31) are possible value is applied in power of 2.
 *                \ref MCU_FSM_POWER_CTRL_AND_DELAY_b
 * @return 	  none
 */
STATIC INLINE void RSI_PS_Ps4PmuLdoOnDelayConfig(uint8_t ldoOnDelay)
{
	MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.PS4_SOCLDO_ON_DELAY = ldoOnDelay;
}

/**
 * @fn            STATIC INLINE void RSI_PS_Ps4PmuBuckOnDelayConfig(uint8_t pmuBuckOnDelay)
 * @brief	  This API is used to  configure buck on delay
 * @param[in]     pmuBuckOnDelay  (0 to 31) are possible value is applied in power of 2.
 *                \ref MCU_FSM_POWER_CTRL_AND_DELAY_b
 * @return 	  none
 */
STATIC INLINE void RSI_PS_Ps4PmuBuckOnDelayConfig(uint8_t pmuBuckOnDelay)
{
	MCU_FSM->MCU_FSM_POWER_CTRL_AND_DELAY_b.PG4_BUCK_ON_DELAY = pmuBuckOnDelay;
}

/**
 * @fn            STATIC INLINE uint32_t RSI_PS_GetWkpUpStatus(void)
 * @brief	  This API is used to  get the wake up/ NPSS interrupt status
 *                \ref NPSS_INTR_STATUS_REG
 * @return        register bits of NPSS interrupt status register
 * @return 	  none
 */
STATIC INLINE uint32_t RSI_PS_GetWkpUpStatus(void)
{
	return NPSS_INTR_STATUS_REG;
}

/**
 * @fn            STATIC INLINE uint32_t RSI_PS_GetComnIntrSts(void)
 * @brief	  This API is used to  get the wake up/ NPSS common interrupt status
 * @return 	  register bits of NPSS interrupt status register
 *                \ref MCU_FSM_WAKEUP_STATUS_REG
 * @return 	  none
 */
STATIC INLINE uint32_t RSI_PS_GetComnIntrSts(void)
{
	return MCU_FSM->MCU_FSM_WAKEUP_STATUS_REG;
}

/**
 * @fn            STATIC INLINE void RSI_PS_NpssIntrUnMask(uint32_t mask)
 * @brief	  This API is used to  un mask the NPSS interrupts
 * @param[in]     mask  is OR'ed value of the NPSS interrupt bits
 *                \ref NPSS_INTR_MASK_CLR_REG
 * @return 	  none 
 */
STATIC INLINE void RSI_PS_NpssIntrUnMask(uint32_t mask)
{
	NPSS_INTR_MASK_CLR_REG = mask;
}

/**
 * @fn            STATIC INLINE void RSI_PS_NpssIntrMask(uint32_t mask)
 * @brief	  This API is used to  mask the NPSS interrupts
 * @param[in]     mask  is OR'ed value of the NPSS interrupt bits
 *                \ref NPSS_INTR_MASK_SET_REG
 * @return 	  none 
 */
STATIC INLINE void RSI_PS_NpssIntrMask(uint32_t mask)
{
	NPSS_INTR_MASK_SET_REG = mask;
}

/**
 * @fn            STATIC INLINE void RSI_PS_EnableLpSleep(boolean_t lpSleep)
 * @brief	  This API is used to  enable/disable the lp sleep mode
 * @param[in]     lpSleep  1:enable lp sleep , 0 : disable lp sleep
 *                \ref MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE_b
 * @return 	  none 
 */
STATIC INLINE void RSI_PS_EnableLpSleep(boolean_t lpSleep)
{
	MCU_FSM->MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE_b.LP_SLEEP_MODE_b = lpSleep;
}

/**
 *@fn              STATIC INLINE void RSI_PS_SkipXtalWaitTime(boolean_t val)
 *@brief           This API is used to  skip the XTAL wait time
 *@param[in]       val 1: skip XTAL wait time
 *                 0 Do not skip XTAL wait time
 *                 \ref MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE_b
 * @return 	  none
 */
STATIC INLINE void RSI_PS_SkipXtalWaitTime(boolean_t val)
{
	MCU_FSM->MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE_b.SKIP_XTAL_WAIT_TIME = val;
}

/**
 *@fn              STATIC INLINE void RSI_PS_UlpToDcDcMode(void)
 *@brief           This API is configures SC-DCDC from LDO to DCDC Mode
 * @return 	  none 
 */
STATIC INLINE void RSI_PS_UlpToDcDcMode(void)
{
	ULP_SPI_MEM_MAP(0x126) = 0x3E002F;
	ULP_SPI_MEM_MAP(0x128) = 0x200020;
}

/**
 *@fn              STATIC INLINE void RSI_PS_LatchCntrlSet(uint32_t val)
 *@brief           This API is used to set the latch configurations
 * @return 	  none 
 */
STATIC INLINE void RSI_PS_LatchCntrlSet(uint32_t val)
{
	ULP_SPI_MEM_MAP(SELECT_BG_CLK) |= val;
}

/**
 *@fn              STATIC INLINE void RSI_PS_LatchCntrlClr(uint32_t val)
 *@brief           This API is used to clear the latch configurations
 * @return 	  none 
 */
STATIC INLINE void RSI_PS_LatchCntrlClr(uint32_t val)
{
	ULP_SPI_MEM_MAP(SELECT_BG_CLK) &= ~val;
}

/**
 *@fn              STATIC INLINE void RSI_PS_BodPwrGateButtonCalibEnable(void)
 *@brief           This API is used to enable the power-gate enable signal for button calib and vbatt status checking block
 * @return 	  none 
 */
STATIC INLINE void RSI_PS_BodPwrGateButtonCalibEnable(void)
{
	ULP_SPI_MEM_MAP(0x1E3) |= (BIT(15));
}

/**
 *@fn              STATIC INLINE void RSI_PS_BodPwrGateButtonCalibDisable(void)
 *@brief           This API is used to disable the power-gate enable signal for button calib and vbatt status checking block
 * @return 	  none 
 */
STATIC INLINE void RSI_PS_BodPwrGateButtonCalibDisable(void)
{
	ULP_SPI_MEM_MAP(0x1E3) &= ~(BIT(15));
}

/**
 *@fn              STATIC INLINE void RSI_PS_XtalEnable(void)
 *@brief           This API is used to enable the XTAL
 *@return 	  none
 */
STATIC INLINE void RSI_PS_XtalEnable(void)
{
	*(volatile uint32_t *)0x41300480 |= (BIT(10));
}

/**
 *@fn              STATIC INLINE void RSI_PS_XtalDisable(void)
 *@brief           This API is used to disable the XTAL
 *@return 	  none 
 */
STATIC INLINE void RSI_PS_XtalDisable(void)
{
	*(volatile uint32_t *)0x41300480 &= ~(BIT(10));
}

/**
 *@fn              STATIC INLINE void RSI_PS_QspiDllDomainEnable(void)
 *@brief           This API is used to enable the power to the QSPI-DLL module
 *@return 	  none 
 */
STATIC INLINE void RSI_PS_QspiDllDomainEnable(void)
{
	*(volatile uint32_t *)0x24048484 |= (BIT(2) | BIT(6));
}

/**
 *@fn              STATIC INLINE void RSI_PS_QspiDllDomainDisable(void)
 *@brief           This API is used to disable the power to the QSPI-DLL module
 *@return 	  none
 */
STATIC INLINE void RSI_PS_QspiDllDomainDisable(void)
{
	*(volatile uint32_t *)0x24048484 &= ~(BIT(2) | BIT(6));
}

/**
 *@fn              STATIC INLINE void RSI_PS_LdoSocDefaultModeEnable(void)
 *@brief           This API is used to enable the SOC LDO default mode (Set high for 1.1 V (default mode))
 *@return 	  none 
 */
STATIC INLINE void RSI_PS_LdoSocDefaultModeEnable(void)
{
	PMU_SPI_MEM_MAP(0x1D6) |= (BIT(5));
}

/**
 *@fn              STATIC INLINE void RSI_PS_LdoSocDefaultModeDisable(void)
 *@brief           This API is used to disable the SOC LDO default mode
 *@return 	  none 
 */
STATIC INLINE void RSI_PS_LdoSocDefaultModeDisable(void)
{
	PMU_SPI_MEM_MAP(0x1D6) &= ~(BIT(5));
}

/**
 *@fn              void RSI_PS_BypassLdoRfEnable(void)
 *@brief           This API is used to enable bypass of LDO-RF enable and control bits to control from outside
 *@return 	  none
 */
STATIC INLINE void RSI_PS_BypassLdoRfEnable(void)
{
	PMU_SPI_MEM_MAP(0x1D8) |= BIT(2);  
}

/**
 *@fn              STATIC INLINE void RSI_PS_FlashLdoEnable(void)
 *@brief           This API is used to flash LDO enable.
 *@return 	  none 
 */
STATIC INLINE void RSI_PS_FlashLdoEnable(void)
{
	BATT_FF->MCU_PMU_LDO_CTRL_SET = BIT(0);
}

/**
 *@fn              STATIC INLINE void RSI_PS_FlashLdoDisable(void)
 *@brief           This API is used to disable flash LDO.
 *@return 	  none
 */
STATIC INLINE void RSI_PS_FlashLdoDisable(void)
{
  BATT_FF->MCU_PMU_LDO_CTRL_CLEAR = BIT(0);  
}

/**
 *@fn              void RSI_PS_BypassLdoRfDisable(void)
 *@brief           This API is used to disable bypass of LDO-RF enable and control bits to control from outside(i.e internal logic)
 *@return 	  none 
 */
STATIC INLINE void RSI_PS_BypassLdoRfDisable(void)
{
	PMU_SPI_MEM_MAP(0x1D8) &= ~BIT(2);
}

/**
 *@fn              STATIC INLINE void RSI_PS_SocPllSpiDisable(void)
 *@brief           This API is used to disable the Soc-PLL SPI PG
 *@return 	  none
 */
STATIC INLINE void RSI_PS_SocPllSpiDisable(void)
{
	BATT_FF->PLLCCI_PWRCTRL_REG_b.SOCPLL_SPI_PG_EN =0U;
}

/**
 *@fn              STATIC INLINE void RSI_PS_SocPllVddIsoEnable(void)
 *@brief           This API is used to enable the Soc-PLL ISO VDD
 *@return 	  none 
 */
STATIC INLINE void RSI_PS_SocPllVddIsoEnable(void)
{
	BATT_FF->PLLCCI_PWRCTRL_REG_b.SOCPLL_ISO_ENABLE =1U;
}

/**
 *@fn              STATIC INLINE void RSI_PS_SocPllVddIsoDiable(void)
 *@brief           This API is used to disable the Soc-PLL ISO VDD
 *@return 	  none 
 */
STATIC INLINE void RSI_PS_SocPllVddIsoDiable(void)
{
	BATT_FF->PLLCCI_PWRCTRL_REG_b.SOCPLL_ISO_ENABLE =0U;
}

/**
 *@fn              STATIC INLINE void RSI_PS_SocPllSpiEnable(void)
 *@brief           This API is used to enable the Soc-PLL SPI PG
 *@return 	  none
 */
STATIC INLINE void RSI_PS_SocPllSpiEnable(void)
{
	BATT_FF->PLLCCI_PWRCTRL_REG_b.SOCPLL_SPI_PG_EN =1U;
}

/**
 *@fn              STATIC INLINE void RSI_ConfigBuckBoost(uint8_t cntrl , uint8_t enable)
 *@brief           This API is used to control the buck boost
 *@param[in]       cntrl 0: Software controlled 1: Hardware controlled.
 *@param[in]       enable 0: Disabled if controlled by software(cntrl = 0) 1: Enabled if controlled by software(cntrl = 1)
 *@return 	  none 
 */
STATIC INLINE void RSI_ConfigBuckBoost(uint8_t cntrl , uint8_t enable)
{
	BATT_FF->MCU_FSM_CTRL_BYPASS_b.MCU_BUCK_BOOST_ENABLE_BYPASS = (enable & 0x1);
	BATT_FF->MCU_FSM_CTRL_BYPASS_b.MCU_BUCK_BOOST_ENABLE_BYPASS_CTRL = (cntrl & 0x1);
}

/**
 *@fn              STATIC INLINE void RSI_ConfigPmuShutDown(uint8_t cntrl , uint8_t enable)
 *@brief           This API is used to control the pmu shut down mode
 *@param[in]       cntrl 0: Software controlled 1: Hardware controlled.
 *@param[in]       enable 0: Disabled if controlled by software(cntrl = 0) 1: Enabled if controlled by software(cntrl = 1)
 *@return 	  none
 */
STATIC INLINE void RSI_ConfigPmuShutDown(uint8_t cntrl , uint8_t enable)
{
	BATT_FF->MCU_FSM_CTRL_BYPASS_b.MCU_PMU_SHUT_DOWN_BYPASS = (enable & 0x1);
	BATT_FF->MCU_FSM_CTRL_BYPASS_b.MCU_PMU_SHUT_DOWN_BYPASS_CTRL = (cntrl & 0x1);
}

/**
 *@fn              STATIC INLINE void RSI_ChangeTassRefClock(void)
 *@brief           This API is used to change the TASS reference clock to 32MHz RC , This API is used only in MCU mode , should not be used in WiSeMCU mode.
 *@return 	  none
 */
STATIC INLINE void RSI_ChangeTassRefClock(void)
{
  *(volatile uint32_t *)0x41300110 = (1<<23)|(1<<16)|(1<<4)|1;
}

/**
 *@fn              STATIC INLINE void RSI_SetRegSpiDivision(uint8_t div)
 *@brief           This API is used to change the reg access SPI clock division factor.
 *@return 	  none
 */
STATIC INLINE void RSI_SetRegSpiDivision(uint8_t div)
{
  *(volatile uint32_t *)(REG_SPI_BASE_ADDR_ULP + 0x00) &= ~(0xF);
  *(volatile uint32_t *)(REG_SPI_BASE_ADDR_ULP + 0x00) |= div;
}

/**
 *@fn              STATIC INLINE void RSI_ConfigXtal(uint8_t cntrl , uint8_t enable)
 *@brief           This API is used to control the Xtal
 *@param[in]       cntrl 0: Software controlled 1: Hardware controlled.
 *@param[in]       enable 0: Disabled if controlled by software(cntrl = 0) 1: Enabled if controlled by software(cntrl = 1)
 *@return 	  none
 */
STATIC INLINE void RSI_ConfigXtal(uint8_t cntrl , uint8_t enable)
{
	BATT_FF->MCU_FSM_CTRL_BYPASS_b.MCU_XTAL_EN_40MHZ_BYPASS = (enable & 0x1);
	BATT_FF->MCU_FSM_CTRL_BYPASS_b.MCU_XTAL_EN_40MHZ_BYPASS_CTRL = (cntrl & 0x1);
}

/**
 *@fn              STATIC INLINE void RSI_PS_PmuUltraSleepConfig(boolean_t en)
 *@brief           This API is used to enable the 'NPSS_PMU_STANDBY' 
 *@param[in]       en 1: enable the 'NPSS_PMU_STANDBY'
 *                 en 0: disable the 'NPSS_PMU_STANDBY'
 *@return 	  none
 */
STATIC INLINE void RSI_PS_PmuUltraSleepConfig(boolean_t en)
{
	MCU_FSM->MCU_FSM_PMU_STATUS_REG_b.STANDBY_DC1P3_R = en;
}

/**
 *@fn              STATIC INLINE void RSI_PS_PmuSetConfig(uint32_t  mask)
 *@brief           This API is used to enable/set the PMU status
 *@param[in]       mask : Ored values of PMU status bits 
 *                 \n Following are the possible parameters for this parameter    
 *                 \n PMU_STS_DCDC_ON
 *                 \n PMU_STS_FLASH_LDO_ON
 *                 \n PMU_STS_SOC_LDO_ON
 *@return 	  none
 */
STATIC INLINE void RSI_PS_PmuSetConfig(uint32_t  mask)
{
	MCU_FSM->MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE |= mask;
}

/**
 *@fn              STATIC INLINE void RSI_PS_PmuClrConfig(uint32_t  mask)
 *@brief           This API is used to disable/clear the PMU status 
 *@param[in]       mask : Ored values of PMU status bits 
 *                 \n Following are the possible parameters for this parameter    
 *                 \n PMU_STS_DCDC_ON
 *                 \n PMU_STS_FLASH_LDO_ON
 *                 \n PMU_STS_SOC_LDO_ON
 *@return 	  none
 */
STATIC INLINE void RSI_PS_PmuClrConfig(uint32_t  mask)
{
	MCU_FSM->MCU_FSM_SLEEP_CTRLS_AND_WAKEUP_MODE &= ~mask;
}

/* @} end of RSI_POWER_SAVE */

void ps_clr_wkp_up_status(uint32_t wakeUpIntrClear);

void ps_bg_ldo_config(uint8_t ldo_0p6_ctrl, uint8_t ldo_0p6_lp_mode);

void ps_configure_trim_values(uint16_t lf_ro_trim ,uint16_t lf_rc_trim , uint16_t hf_ro_trim ,uint16_t hf_rc_trim ,uint16_t bg_ptat_trim , uint16_t bg_trim);

void ps_wireless_shutdown(void);

error_t RSI_PS_AnalogPeriPtatEnable(void);
error_t RSI_PS_AnalogPeriPtatDisable(void);
error_t RSI_PS_BodClksPtatEnable(void);
error_t RSI_PS_BodClksPtatDisable(void);

/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup RSI_POWER_SAVE RSI:RS1xxxx POWER SAVE	
 *  @{
 *
 */
/**
 * @fn            void RSI_PS_EnterShipMode(uint8_t wakeUpDelay ,  boolean_t gpioBased , SHUT_DOWN_WKP_MODE_T gpioSel)
 * @brief	  This API is used to keep MCU in shelf mode
 * @param[in]	  wakeUpDelay  wake up delay (Possible values : 0, 1, 2, ... 7 ,Delay will be 2^8 = 256 clock with 32Khz RC clock.)
 * @param[in]	  gpioBased  GPIO based Shelf mode Entering. If set '1' by processor, On Falling edge of NPSS GPIO
 * @param[in]	  gpioSel  Post Division factor2. See user manual for more info.
 * @return        returns none
 */
void RSI_PS_EnterShipMode(uint8_t wakeUpDelay ,  boolean_t gpioBased , SHUT_DOWN_WKP_MODE_T gpioSel);

/* @} end of RSI_POWER_SAVE */
error_t ps_power_state_change_ps4tops2
(
		ULP_MODE_T enCtxSel          ,
		uint8_t PwrMuxSelUlpssRam    ,
		uint8_t pwrMuxSelM4UlpRam    ,
		uint8_t pwrMuxSelM4UlpRam16K ,
		uint8_t pwrMuxSelM4Ulp       ,
		uint8_t pwrMuxSelUlpss       ,
		uint8_t bgSampleEnable       ,
		uint8_t dcDcEnable           ,
		uint8_t socLdoEnable         ,
		uint8_t standByDc
);

error_t ps_power_state_change_ps2_to_Ps4(uint32_t PmuBuckTurnOnWaitTime , uint32_t SocLdoTurnOnWaitTime);

/*End of file not truncated*/
#ifdef __cplusplus
}
#endif

#endif /*__RSI_POWER_SAVE_H__*/
