/**
 * @file       rsi_dac_example.c
 * @version    1.0
 * @date       12 Augest 2018
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This file contains example describing usage of DAC peripheral testing.
 *
 * @section Description
 * Demonstrate the DAC functionality. 
 */
 
/* Includes ------------------------------------------------------------------*/
#include "rsi_chip.h"
#include "UDMA.h"

/* Private typedef -----------------------------------------------------------*/
extern RSI_DRIVER_UDMA Driver_UDMA1;
static RSI_DRIVER_UDMA *UDMAdrv1 = &Driver_UDMA1;
extern RSI_UDMA_HANDLE_T udmaHandle1;
RSI_UDMA_CHA_CONFIG_DATA_T control;
RSI_UDMA_CHA_CFG_T config;

/* Private macro -------------------------------------------------------------*/
#define  UDMA1_IRQHandler  IRQ010_Handler
#define  ADC_IRQ_Handler   IRQ011_Handler
#define  OUTPUT_PIN_NUMBER  4
#define  OUTPUT_MUX_ENABLE  1
#define  OUTPUT_MUX_SEL     0   /* Select DAC output on ULP_GPIO4 */
#define  DAC_FIFO_THR       4   /* Set FIFO threshold as 4 */
#define  DAC_FIFO_MODE      0
#define  BUFFER_SIZE        1024
/* Private function prototype ---------------------------------------------------------*/  
volatile uint32_t done,ret;
uint16_t out_data_ping[100];
uint16_t out_data_pong[100];
uint16_t tx_buffer[BUFFER_SIZE] __attribute__((at(0x24061000)));   
uint32_t i,j;

/*Standard sin wave samples */
 uint16_t dac_100k_n0dbfs[1024] = {
0x200,
0x222,
0x245,
0x267,
0x288,
0x2A9,
0x2C9,
0x2E9,
0x307,
0x324,
0x33F,
0x359,
0x372,
0x389,
0x39E,
0x3B1,
0x3C3,
0x3D2,
0x3DF,
0x3EA,
0x3F3,
0x3F9,
0x3FD,
0x3FF,
0x3FE,
0x3FC,
0x3F6,
0x3EF,
0x3E5,
0x3D9,
0x3CB,
0x3BB,
0x3A9,
0x395,
0x37F,
0x367,
0x34E,
0x333,
0x317,
0x2F9,
0x2DA,
0x2BB,
0x29A,
0x279,
0x257,
0x235,
0x213,
0x1F0,
0x1CE,
0x1AC,
0x18A,
0x169,
0x148,
0x128,
0x10A,
0x0EC,
0x0D0,
0x0B5,
0x09B,
0x083,
0x06D,
0x059,
0x047,
0x036,
0x028,
0x01C,
0x012,
0x00A,
0x005,
0x002,
0x001,
0x003,
0x007,
0x00D,
0x015,
0x020,
0x02D,
0x03C,
0x04D,
0x060,
0x075,
0x08C,
0x0A4,
0x0BE,
0x0DA,
0x0F7,
0x115,
0x134,
0x154,
0x175,
0x196,
0x1B8,
0x1DA,
0x1FD,
0x21F,
0x242,
0x264,
0x285,
0x2A6,
0x2C6,
0x2E6,
0x304,
0x321,
0x33D,
0x357,
0x370,
0x387,
0x39C,
0x3B0,
0x3C1,
0x3D1,
0x3DE,
0x3E9,
0x3F2,
0x3F9,
0x3FD,
0x3FF,
0x3FF,
0x3FC,
0x3F7,
0x3F0,
0x3E6,
0x3DA,
0x3CD,
0x3BD,
0x3AB,
0x397,
0x381,
0x369,
0x350,
0x335,
0x319,
0x2FC,
0x2DD,
0x2BE,
0x29D,
0x27C,
0x25A,
0x238,
0x216,
0x1F3,
0x1D1,
0x1AF,
0x18D,
0x16C,
0x14B,
0x12B,
0x10C,
0x0EF,
0x0D2,
0x0B7,
0x09D,
0x085,
0x06F,
0x05B,
0x048,
0x038,
0x029,
0x01D,
0x013,
0x00B,
0x005,
0x002,
0x001,
0x002,
0x006,
0x00C,
0x014,
0x01F,
0x02C,
0x03A,
0x04B,
0x05E,
0x073,
0x08A,
0x0A2,
0x0BC,
0x0D7,
0x0F4,
0x112,
0x131,
0x151,
0x172,
0x193,
0x1B5,
0x1D7,
0x1FA,
0x21C,
0x23F,
0x261,
0x282,
0x2A3,
0x2C4,
0x2E3,
0x301,
0x31E,
0x33A,
0x355,
0x36E,
0x385,
0x39A,
0x3AE,
0x3C0,
0x3CF,
0x3DD,
0x3E8,
0x3F1,
0x3F8,
0x3FD,
0x3FF,
0x3FF,
0x3FC,
0x3F7,
0x3F0,
0x3E7,
0x3DC,
0x3CE,
0x3BE,
0x3AC,
0x399,
0x383,
0x36C,
0x352,
0x338,
0x31C,
0x2FF,
0x2E0,
0x2C1,
0x2A0,
0x27F,
0x25E,
0x23B,
0x219,
0x1F7,
0x1D4,
0x1B2,
0x190,
0x16F,
0x14E,
0x12E,
0x10F,
0x0F1,
0x0D5,
0x0B9,
0x0A0,
0x087,
0x071,
0x05C,
0x04A,
0x039,
0x02A,
0x01E,
0x014,
0x00B,
0x006,
0x002,
0x001,
0x002,
0x006,
0x00B,
0x014,
0x01E,
0x02A,
0x039,
0x04A,
0x05C,
0x071,
0x087,
0x0A0,
0x0B9,
0x0D5,
0x0F1,
0x10F,
0x12E,
0x14E,
0x16F,
0x190,
0x1B2,
0x1D4,
0x1F7,
0x219,
0x23B,
0x25E,
0x27F,
0x2A0,
0x2C1,
0x2E0,
0x2FF,
0x31C,
0x338,
0x352,
0x36C,
0x383,
0x399,
0x3AC,
0x3BE,
0x3CE,
0x3DC,
0x3E7,
0x3F0,
0x3F7,
0x3FC,
0x3FF,
0x3FF,
0x3FD,
0x3F8,
0x3F1,
0x3E8,
0x3DD,
0x3CF,
0x3C0,
0x3AE,
0x39A,
0x385,
0x36E,
0x355,
0x33A,
0x31E,
0x301,
0x2E3,
0x2C4,
0x2A3,
0x282,
0x261,
0x23F,
0x21C,
0x1FA,
0x1D7,
0x1B5,
0x193,
0x172,
0x151,
0x131,
0x112,
0x0F4,
0x0D7,
0x0BC,
0x0A2,
0x08A,
0x073,
0x05E,
0x04B,
0x03A,
0x02C,
0x01F,
0x014,
0x00C,
0x006,
0x002,
0x001,
0x002,
0x005,
0x00B,
0x013,
0x01D,
0x029,
0x038,
0x048,
0x05B,
0x06F,
0x085,
0x09D,
0x0B7,
0x0D2,
0x0EF,
0x10C,
0x12B,
0x14B,
0x16C,
0x18D,
0x1AF,
0x1D1,
0x1F3,
0x216,
0x238,
0x25A,
0x27C,
0x29D,
0x2BE,
0x2DD,
0x2FC,
0x319,
0x335,
0x350,
0x369,
0x381,
0x397,
0x3AB,
0x3BD,
0x3CD,
0x3DA,
0x3E6,
0x3F0,
0x3F7,
0x3FC,
0x3FF,
0x3FF,
0x3FD,
0x3F9,
0x3F2,
0x3E9,
0x3DE,
0x3D1,
0x3C1,
0x3B0,
0x39C,
0x387,
0x370,
0x357,
0x33D,
0x321,
0x304,
0x2E6,
0x2C6,
0x2A6,
0x285,
0x264,
0x242,
0x21F,
0x1FD,
0x1DA,
0x1B8,
0x196,
0x175,
0x154,
0x134,
0x115,
0x0F7,
0x0DA,
0x0BE,
0x0A4,
0x08C,
0x075,
0x060,
0x04D,
0x03C,
0x02D,
0x020,
0x015,
0x00D,
0x007,
0x003,
0x001,
0x002,
0x005,
0x00A,
0x012,
0x01C,
0x028,
0x036,
0x047,
0x059,
0x06D,
0x083,
0x09B,
0x0B5,
0x0D0,
0x0EC,
0x10A,
0x128,
0x148,
0x169,
0x18A,
0x1AC,
0x1CE,
0x1F0,
0x213,
0x235,
0x257,
0x279,
0x29A,
0x2BB,
0x2DA,
0x2F9,
0x317,
0x333,
0x34E,
0x367,
0x37F,
0x395,
0x3A9,
0x3BB,
0x3CB,
0x3D9,
0x3E5,
0x3EF,
0x3F6,
0x3FC,
0x3FE,
0x3FF,
0x3FD,
0x3F9,
0x3F3,
0x3EA,
0x3DF,
0x3D2,
0x3C3,
0x3B1,
0x39E,
0x389,
0x372,
0x359,
0x33F,
0x324,
0x307,
0x2E9,
0x2C9,
0x2A9,
0x288,
0x267,
0x245,
0x222,
0x200,
0x1DE,
0x1BB,
0x199,
0x178,
0x157,
0x137,
0x117,
0x0F9,
0x0DC,
0x0C1,
0x0A7,
0x08E,
0x077,
0x062,
0x04F,
0x03D,
0x02E,
0x021,
0x016,
0x00D,
0x007,
0x003,
0x001,
0x002,
0x004,
0x00A,
0x011,
0x01B,
0x027,
0x035,
0x045,
0x057,
0x06B,
0x081,
0x099,
0x0B2,
0x0CD,
0x0E9,
0x107,
0x126,
0x145,
0x166,
0x187,
0x1A9,
0x1CB,
0x1ED,
0x210,
0x232,
0x254,
0x276,
0x297,
0x2B8,
0x2D8,
0x2F6,
0x314,
0x330,
0x34B,
0x365,
0x37D,
0x393,
0x3A7,
0x3B9,
0x3CA,
0x3D8,
0x3E4,
0x3EE,
0x3F6,
0x3FB,
0x3FE,
0x3FF,
0x3FD,
0x3F9,
0x3F3,
0x3EB,
0x3E0,
0x3D3,
0x3C4,
0x3B3,
0x3A0,
0x38B,
0x374,
0x35C,
0x342,
0x326,
0x309,
0x2EB,
0x2CC,
0x2AC,
0x28B,
0x26A,
0x248,
0x226,
0x203,
0x1E1,
0x1BE,
0x19C,
0x17B,
0x15A,
0x13A,
0x11A,
0x0FC,
0x0DF,
0x0C3,
0x0A9,
0x090,
0x079,
0x064,
0x050,
0x03F,
0x02F,
0x022,
0x017,
0x00E,
0x007,
0x003,
0x001,
0x001,
0x004,
0x009,
0x010,
0x01A,
0x026,
0x033,
0x043,
0x055,
0x069,
0x07F,
0x097,
0x0B0,
0x0CB,
0x0E7,
0x104,
0x123,
0x142,
0x163,
0x184,
0x1A6,
0x1C8,
0x1EA,
0x20D,
0x22F,
0x251,
0x273,
0x294,
0x2B5,
0x2D5,
0x2F4,
0x311,
0x32E,
0x349,
0x363,
0x37B,
0x391,
0x3A5,
0x3B8,
0x3C8,
0x3D7,
0x3E3,
0x3ED,
0x3F5,
0x3FB,
0x3FE,
0x3FF,
0x3FE,
0x3FA,
0x3F4,
0x3EC,
0x3E1,
0x3D4,
0x3C6,
0x3B5,
0x3A2,
0x38D,
0x376,
0x35E,
0x344,
0x329,
0x30C,
0x2EE,
0x2CF,
0x2AF,
0x28E,
0x26D,
0x24B,
0x229,
0x206,
0x1E4,
0x1C1,
0x19F,
0x17E,
0x15D,
0x13C,
0x11D,
0x0FF,
0x0E2,
0x0C6,
0x0AB,
0x092,
0x07B,
0x066,
0x052,
0x040,
0x031,
0x023,
0x018,
0x00F,
0x008,
0x003,
0x001,
0x001,
0x004,
0x009,
0x010,
0x019,
0x024,
0x032,
0x042,
0x054,
0x067,
0x07D,
0x094,
0x0AE,
0x0C8,
0x0E4,
0x101,
0x120,
0x13F,
0x160,
0x181,
0x1A2,
0x1C5,
0x1E7,
0x209,
0x22C,
0x24E,
0x270,
0x291,
0x2B2,
0x2D2,
0x2F1,
0x30F,
0x32B,
0x347,
0x360,
0x379,
0x38F,
0x3A4,
0x3B6,
0x3C7,
0x3D6,
0x3E2,
0x3EC,
0x3F5,
0x3FA,
0x3FE,
0x3FF,
0x3FE,
0x3FA,
0x3F5,
0x3EC,
0x3E2,
0x3D6,
0x3C7,
0x3B6,
0x3A4,
0x38F,
0x379,
0x360,
0x347,
0x32B,
0x30F,
0x2F1,
0x2D2,
0x2B2,
0x291,
0x270,
0x24E,
0x22C,
0x209,
0x1E7,
0x1C5,
0x1A2,
0x181,
0x160,
0x13F,
0x120,
0x101,
0x0E4,
0x0C8,
0x0AE,
0x094,
0x07D,
0x067,
0x054,
0x042,
0x032,
0x024,
0x019,
0x010,
0x009,
0x004,
0x001,
0x001,
0x003,
0x008,
0x00F,
0x018,
0x023,
0x031,
0x040,
0x052,
0x066,
0x07B,
0x092,
0x0AB,
0x0C6,
0x0E2,
0x0FF,
0x11D,
0x13C,
0x15D,
0x17E,
0x19F,
0x1C1,
0x1E4,
0x206,
0x229,
0x24B,
0x26D,
0x28E,
0x2AF,
0x2CF,
0x2EE,
0x30C,
0x329,
0x344,
0x35E,
0x376,
0x38D,
0x3A2,
0x3B5,
0x3C6,
0x3D4,
0x3E1,
0x3EC,
0x3F4,
0x3FA,
0x3FE,
0x3FF,
0x3FE,
0x3FB,
0x3F5,
0x3ED,
0x3E3,
0x3D7,
0x3C8,
0x3B8,
0x3A5,
0x391,
0x37B,
0x363,
0x349,
0x32E,
0x311,
0x2F4,
0x2D5,
0x2B5,
0x294,
0x273,
0x251,
0x22F,
0x20D,
0x1EA,
0x1C8,
0x1A6,
0x184,
0x163,
0x142,
0x123,
0x104,
0x0E7,
0x0CB,
0x0B0,
0x097,
0x07F,
0x069,
0x055,
0x043,
0x033,
0x026,
0x01A,
0x010,
0x009,
0x004,
0x001,
0x001,
0x003,
0x007,
0x00E,
0x017,
0x022,
0x02F,
0x03F,
0x050,
0x064,
0x079,
0x090,
0x0A9,
0x0C3,
0x0DF,
0x0FC,
0x11A,
0x13A,
0x15A,
0x17B,
0x19C,
0x1BE,
0x1E1,
0x203,
0x226,
0x248,
0x26A,
0x28B,
0x2AC,
0x2CC,
0x2EB,
0x309,
0x326,
0x342,
0x35C,
0x374,
0x38B,
0x3A0,
0x3B3,
0x3C4,
0x3D3,
0x3E0,
0x3EB,
0x3F3,
0x3F9,
0x3FD,
0x3FF,
0x3FE,
0x3FB,
0x3F6,
0x3EE,
0x3E4,
0x3D8,
0x3CA,
0x3B9,
0x3A7,
0x393,
0x37D,
0x365,
0x34B,
0x330,
0x314,
0x2F6,
0x2D8,
0x2B8,
0x297,
0x276,
0x254,
0x232,
0x210,
0x1ED,
0x1CB,
0x1A9,
0x187,
0x166,
0x145,
0x126,
0x107,
0x0E9,
0x0CD,
0x0B2,
0x099,
0x081,
0x06B,
0x057,
0x045,
0x035,
0x027,
0x01B,
0x011,
0x00A,
0x004,
0x002,
0x001,
0x003,
0x007,
0x00D,
0x016,
0x021,
0x02E,
0x03D,
0x04F,
0x062,
0x077,
0x08E,
0x0A7,
0x0C1,
0x0DC,
0x0F9,
0x117,
0x137,
0x157,
0x178,
0x199,
0x1BB,
0x1DE,
};



/**
 * @brief      This API configure the pin for DAC output.
 * @param[in]  Pin number to use play DAC output on GPIO , (ULP_GPIO4 or ULP_GPIO15)
 * @return     None
 */ 
void dac_output_pin_mux(uint16_t pin_number)
{
	RSI_EGPIO_SetPinMux(EGPIO1,0,pin_number,EGPIO_PIN_MUX_MODE7);
}

/**
 * @brief      UDMA  controller transfer descriptor chain complete callback 
 * @param[in]  event dma transfer events
 * @param[in]  ch    dma channel number
 * @return     None
 */ 
void udmaTransferComplete(uint32_t event, uint8_t ch)
{
  if(event == UDMA_EVENT_XFER_DONE)
  {
    if(ch == 10)
    {
      done = 1;  
    }
  }
} 

/**
 * @brief      UDMA  configuration and enable function. 
 * @param[in]  event : dma transfer events
 * @param[in]  ch    : dma channel number
 * @return     None
 */ 
void UDMA_Write(void)
{
	memset(&control, 0, sizeof(RSI_UDMA_CHA_CONFIG_DATA_T));
	memset(&config, 0, sizeof(RSI_UDMA_CHA_CFG_T));
	
	config.altStruct = 0;
  config.burstReq  = 1;
  config.channelPrioHigh = 0;
  config.periAck = 0;
  config.periphReq = 0;
  config.reqMask = 0;
  config.dmaCh     = 10;
	
	/* Setup source to desination copy for trigger for memory */
	/* Descriptor configurations */	 
	control.transferType       = UDMA_MODE_BASIC;
	control.nextBurst					 = 0;
	control.totalNumOfDMATrans = BUFFER_SIZE -1;
	control.rPower             = ARBSIZE_4;
	control.srcProtCtrl 			 = 0x000;
	control.dstProtCtrl 			 = 0x000;
	control.srcSize            = SRC_SIZE_16;
	control.srcInc             = SRC_INC_16;
	control.dstSize            = DST_SIZE_16;
	control.dstInc             = DST_INC_NONE;
		
   /* Initialise dma */ 
   UDMAdrv1->Initialize();
	
	 /* Configure dma channel */ 
   UDMAdrv1->ChannelConfigure( 10,(uint32_t )tx_buffer, (uint32_t )0x24043810, BUFFER_SIZE, 
                               control, &config, udmaTransferComplete );
   /* Enable dma channel */
   UDMAdrv1->ChannelEnable(10); 	
}

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */ 
int main(void)
{
	/* At this stage the MICROCONTROLLER clock setting is already configured,
	 * this is done through SystemInit() function which is called from startup
	 * file (startup_rs1xxxx.s) before to branch to application main.
	 * To reconfigure the default setting of SystemInit() function, refer to
	 * system_rs1xxxx.c file
	 */	
	
	SystemCoreClockUpdate();
	
	/* Power gate enable for DAC */
	RSI_DAC_PowerControl(DAC_POWER_ON);
	
	/* Enble the 32mrc clock to DAC */
	RSI_ULPSS_AuxClkConfig(ULPCLK, ENABLE_STATIC_CLK ,ULP_AUX_32MHZ_RC_CLK);
	
	/* Configure the refernce LDO voltage as 2.4v */
  RSI_ADC_AUXLdoConfig(AUX_ADC_DAC_COMP,0,0xb);
	
	/* Write the 1024 digital sample to ULPSS memory */
	for(i = 0; i < 1024; i=i+2)
  {
     *(volatile uint32_t *)(0x24061000 + (j * 4)) = (dac_100k_n0dbfs[i+1] << 16) | (dac_100k_n0dbfs[i]);
     j = j + 1;
  }
   
  /* Configure the ULP_GPIO4 as analog mode for DAC output */
  dac_output_pin_mux(OUTPUT_PIN_NUMBER);	
  
	/* Divide DAC clock */ 
	RSI_DAC_ClkDivFactor(AUX_ADC_DAC_COMP, 16);
	
	/* Configure the DAC in FIFO mode */
	RSI_DAC_Config(AUX_ADC_DAC_COMP,DAC_FIFO_MODE,OUTPUT_MUX_ENABLE,OUTPUT_MUX_SEL);
	
	/* Set the FIFO threshold value */
	RSI_DAC_SetFifoThreshold(AUX_ADC_DAC_COMP,DAC_FIFO_THR);
  
	/* Start the DAC */
	RSI_DAC_Start(AUX_ADC_DAC_COMP,1);
	
	/* Enable the UDMA interrupt */
	NVIC_EnableIRQ(UDMA1_IRQn);
  
	while(1)
	{	
		/* write data to dac data reg through udma*/ 
    UDMA_Write();
 	  /* Enable dma controller */  
    UDMAdrv1->DMAEnable();
	  /* Wait till dma done here external dma fill the DAC input buffer*/
    while(!done);
		
	  done =0;	
   }
}

