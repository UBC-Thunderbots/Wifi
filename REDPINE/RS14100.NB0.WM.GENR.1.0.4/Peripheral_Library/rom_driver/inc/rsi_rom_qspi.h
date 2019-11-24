/**
 * @file       rsi_rom_qspi.h
 * @version    1.0
 * @date       1 AUG 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains functions prototypes related to QSPI.
 *
 * @section Description
 * This file contains the list of function prototypes for the QSPI and low level function definitions
 * Following are list of API's which need to be defined in this file.
 *
 */


/**
 * Includes
 */

#ifndef  __RSI_ROM_QSPI_H__
#define  __RSI_ROM_QSPI_H__

/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup RSI_QSPI_DRIVERS RSI:RS1xxxx QSPI 
 *  @{
 *
 */
#include "rsi_qspi.h"
#include "rsi_qspi_proto.h"
#include "rsi_packing.h"
#include "rsi_rom_table_RS1xxxx.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @fn      STATIC INLINE void  RSI_QSPI_WriteToFlash(qspi_reg_t *qspi_reg,uint32_t len_in_bits,
		                        uint32_t cmd_addr_data,uint32_t cs_no)
 *  @brief   This function writes to flash (8-bit) cmd or address or data
 *  @param   qspi_reg      :  pointer to qspi_reg_t structure contains all qspi regsisters, \ref qspi_reg_t
 *  @param   len_in_bits   :  no. of valid bits
 *  @param   cmd_addr_data :  cmd/addr/data
 *  @param   cs_no         :  chip select no.
 *  @return  none
 */
STATIC INLINE void  RSI_QSPI_WriteToFlash(qspi_reg_t *qspi_reg, uint32_t len_in_bits,	uint32_t cmd_addr_data,uint32_t cs_no)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_write_to_flash(qspi_reg, len_in_bits, cmd_addr_data,cs_no );
#else
	qspi_write_to_flash(qspi_reg, len_in_bits, cmd_addr_data,cs_no );
#endif

}

/**
 *  @fn      STATIC INLINE void  RSI_QSPI_SwitchQspi2(qspi_reg_t *qspi_reg,uint32_t mode,uint32_t cs_no)
 *  @brief   This function changes the mode of qspi to SPI/DUAL/QUAD
 * 					 for the required chip select
 *  @param   qspi_reg   :  pointer to qspi_reg_t structure contains all qspi regsisters, \ref qspi_reg_t
 *  @param   mode      	:	 the target bus mode for QSPI i.e. SPI/DUAL/QUAD
 *  @param   cs_no     	:	 chip select no.
 *  @return  none
 */

STATIC INLINE void  RSI_QSPI_SwitchQspi2(qspi_reg_t *qspi_reg,uint32_t mode,uint32_t cs_no)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_switch_qspi2(qspi_reg,mode,cs_no);
#else
	qspi_switch_qspi2(qspi_reg,mode,cs_no);
#endif
}

/**
 *  @fn      STATIC INLINE uint32_t  RSI_QSPI_WaitFlashStatusIdle(qspi_reg_t *qspi_reg,
                                                   spi_config_t *spi_config,	uint32_t wr_reg_delay_ms)
 *  @brief   This function waits for flash status to go idle
 *  @param   qspi_reg       :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   spi_config     :  pointer to spi_config_t structure,spi configuration \ref spi_config_t
 *  @param   wr_reg_delay_ms :  delay in ms provided after a register,write operation is performed on flash.
 *  @return  return flash status value
 */
STATIC INLINE uint32_t  RSI_QSPI_WaitFlashStatusIdle(qspi_reg_t *qspi_reg,	spi_config_t *spi_config,	uint32_t wr_reg_delay_ms)
{
#if defined(ROMDRIVER_PRESENT )
	return ROMAPI_QSPI_API->qspi_wait_flash_status_Idle(qspi_reg,  spi_config, wr_reg_delay_ms);
#else
	return qspi_wait_flash_status_Idle(qspi_reg,  spi_config, wr_reg_delay_ms);
#endif
}

/**
 *  @fn      STATIC INLINE void  RSI_QSPI_EnableStatusRegWrite(qspi_reg_t *qspi_reg,uint32_t flash_type,
                                                spi_config_t *spi_config,	uint32_t cs_no)
 *  @brief   This function enables status register write
 *  @param   qspi_reg       :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   flash_type     :  type of the flash
 *  @param   spi_config     :  pointer to spi_config_t structure,spi configuration \ref spi_config_t
 *  @param   cs_no     	    :  chip select no.
 *  @return  none
 */
STATIC INLINE void  RSI_QSPI_EnableStatusRegWrite(qspi_reg_t *qspi_reg,uint32_t flash_type, spi_config_t *spi_config,	uint32_t cs_no)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_enable_status_reg_write(qspi_reg,  flash_type,spi_config,cs_no);
#else
	qspi_enable_status_reg_write(qspi_reg,  flash_type,spi_config, cs_no);
#endif
}

/**
 *  @fn      STATIC INLINE void  RSI_QSPI_StatusRegWrite(qspi_reg_t *qspi_reg,	uint16_t write_value,
                                          spi_config_t *spi_config,uint32_t wr_reg_delay_ms)
 *  @brief   This function writes to status register ,the protect word and waits till the write is in progress
 *  @param   qspi_reg        :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   write_value     :  Value to write into register
 *  @param   spi_config      :  pointer to spi_config_t structure,spi configuration \ref spi_config_t
 *  @param   wr_reg_delay_ms :  delay in ms provided after a register ,write operation is performed on flash
 *  @return  none  
 */
STATIC INLINE void  RSI_QSPI_StatusRegWrite(qspi_reg_t *qspi_reg,	uint16_t write_value, spi_config_t *spi_config,uint32_t wr_reg_delay_ms)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_status_reg_write(qspi_reg,  write_value, spi_config,wr_reg_delay_ms);
#else
	qspi_status_reg_write(qspi_reg,  write_value, spi_config,wr_reg_delay_ms);
#endif
}

/**
 *  @fn      STATIC INLINE uint32_t  RSI_QSPI_FlashRegRead(qspi_reg_t *qspi_reg, uint8_t reg_read_cmd,
                                             uint32_t cs_no,  spi_config_t *spi_config)
 *  @brief   This function reads a register from the flash
 *  @param   qspi_reg       :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   reg_read_cmd   : register write command to be used 
 *  @param   cs_no  	    :  chip select no.
 *  @param   spi_config     :  pointer to spi_config_t structure,spi configuration \ref spi_config_t
 *  @return  value read from config register
 */
STATIC INLINE uint32_t  RSI_QSPI_FlashRegRead(qspi_reg_t *qspi_reg, uint8_t reg_read_cmd, uint32_t cs_no,  spi_config_t *spi_config)
{
#if defined(ROMDRIVER_PRESENT )
	return ROMAPI_QSPI_API->qspi_flash_reg_read(qspi_reg, reg_read_cmd, cs_no, spi_config);
#else
	return qspi_flash_reg_read(qspi_reg, reg_read_cmd, cs_no,spi_config);
#endif
}

/**
 *  @fn     STATIC INLINE void  RSI_QSPI_FlashRegWrite(qspi_reg_t *qspi_reg,uint32_t reg_write_cmd,
 *                                       uint32_t reg_write_value,	uint32_t cs_no,uint32_t wr_reg_delay_ms)
 *  @brief   This function writes a register in the flash
 *  @param   qspi_reg        :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   reg_write_cmd   :  register write command to be used
 *  @param   reg_write_value :  value to be written to the register
 *  @param   cs_no           :  chip select no.
 *  @param   wr_reg_delay_ms :  delay in ms provided after a register ,write operation is performed on flash
 *  @return  none
 */
STATIC INLINE void  RSI_QSPI_FlashRegWrite(qspi_reg_t *qspi_reg,uint32_t reg_write_cmd,	uint32_t reg_write_value,	uint32_t cs_no,uint32_t wr_reg_delay_ms)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_flash_reg_write(qspi_reg,  reg_write_cmd, reg_write_value, cs_no,wr_reg_delay_ms);
#else
	qspi_flash_reg_write(qspi_reg,  reg_write_cmd, reg_write_value, cs_no,wr_reg_delay_ms);
#endif
}

/**
 *  @fn      STATIC INLINE void  RSI_QSPI_SetFlashMode(qspi_reg_t *qspi_reg,	uint32_t data_mode,
 *                                      uint32_t cs_no,	uint32_t ddr_en,	uint32_t flash_type)
 *  @brief   This function sets the mode for SST_QUAD_FLASH & QSPI.
 *  @param   qspi_reg        :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   data_mode       :  the operational mode for QSPI and flash
 *  @param   cs_no           :  chip select no.
 *  @param   ddr_en          :  To enable dual data rate
 *  @param   flash_type      :  type of the flash variant
 *  @return  none
 */
STATIC INLINE void  RSI_QSPI_SetFlashMode(qspi_reg_t *qspi_reg,	uint32_t data_mode,	uint32_t cs_no,	uint32_t ddr_en,	uint32_t flash_type)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_set_flash_mode(qspi_reg,  data_mode, cs_no, ddr_en, flash_type);
#else
	qspi_set_flash_mode(qspi_reg,  data_mode, cs_no, ddr_en, flash_type);
#endif
}

/**
 * @fn       STATIC INLINE void  RSI_QSPI_ConfigQflash4Read(qspi_reg_t *qspi_reg,spi_config_t *spi_config,	uint32_t addr)
 * @brief    This function configures the flash to the desired ,mode specified by spi_config i.e. the instruction,
 * 					 address and any other stages before data phase are executed by this function.
 *  @param   qspi_reg       :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   spi_config     :  pointer to spi_config_t structure,spi configuration \ref spi_config_t
 *  @param   addr           :  Address in flash memory
 *  @return  none
 */
STATIC INLINE void  RSI_QSPI_ConfigQflash4Read(qspi_reg_t *qspi_reg,spi_config_t *spi_config,	uint32_t addr)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_config_qflash4_read(qspi_reg,  spi_config, addr);
#else
	qspi_config_qflash4_read(qspi_reg,  spi_config, addr);
#endif
}

/**
 * @fn       void RSI_QSPI_ManualRead(qspi_reg_t *qspi_reg,spi_config_t *spi_config,uint32_t addr,uint8_t *data,
 *                                  uint32_t hsize,uint32_t len_in_bytes, uint32_t manual_udma_read,
 *                                   void *udmaHandle, void *rpdmaHandle)
 * @brief    This function reads from the flash in manual mode,for that it configures the qspi and flash into the
 * 					 required read mode. Data from the qspi fifo can be read using dma mode or using the i/o reads.
 *  @param   qspi_reg          :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   spi_config        :  pointer to spi_config_t structure,spi configuration \ref spi_config_t
 *  @param   addr              :  Address in flash memory
 *  @param   data              :  Pointer to read data buffer
 *  @param   hsize             :  8 bits, 16 bits or 32 bits xfer on AHB
 *  @param   len_in_bytes      :  no. of bytes to be read
 *  @param   manual_udma_read  :  1 : uses UDMA, 0 - uses RPDMA
 *  @param   udmaHandle        :  udma context handler
 *  @param   rpdmaHandle       :  rpdma context handler
 *  @return  none
 */
STATIC INLINE void  RSI_QSPI_ManualRead(qspi_reg_t *qspi_reg,spi_config_t *spi_config,uint32_t addr,uint8_t *data,uint32_t hsize,	uint32_t len_in_bytes, uint32_t manual_udma_read, void *udmaHandle, void *rpdmaHandle)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_manual_read(qspi_reg,  spi_config, addr, data, hsize, len_in_bytes, manual_udma_read,udmaHandle,rpdmaHandle);
#else
	qspi_manual_read(qspi_reg,  spi_config, addr, data, hsize, len_in_bytes, manual_udma_read,udmaHandle, rpdmaHandle);
#endif
}

/**
 *  @fn      void RSI_QSPI_AutoInit(qspi_reg_t *qspi_reg,spi_config_t *spi_config)
 *  @brief   This function initializes auto mode
 *  @param   qspi_reg         :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   spi_config       :  pointer to spi_config_t structure,spi configuration \ref spi_config_t
 *  @return  none
 */
STATIC INLINE void  RSI_QSPI_AutoInit(qspi_reg_t *qspi_reg,spi_config_t *spi_config)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_auto_init(qspi_reg,  spi_config);
#else
	qspi_auto_init(qspi_reg,  spi_config);
#endif
}

/**
 *  @fn      STATIC INLINE void  RSI_QSPI_AutoRead(uint32_t cs_no,uint32_t addr,uint8_t *data,uint32_t hsize,
                                    uint32_t len_in_bytes,spi_config_t *spi_config, uint32_t dma_flags)
 *  @brief   This function reads from the flash in auto mode
 *  @param   cs_no        :   chip select no.
 *  @param   addr         :   Address in flash memory
 *  @param   data         :   Pointer to read data buffer
 *  @param   hsize        :   8 bits, 16 bits or 32 bits xfer on AHB
 *  @param   len_in_bytes :   no. of bytes
 *  @param   spi_config   :   pointer to spi_config_t structure,spi configuration \ref spi_config_t
 *  @param   dma_flags    :   no. of bytes
 *  @return  none
 */
STATIC INLINE void  RSI_QSPI_AutoRead(uint32_t cs_no,uint32_t addr,uint8_t *data,uint32_t hsize,uint32_t len_in_bytes,spi_config_t *spi_config, uint32_t dma_flags)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_auto_read(cs_no, addr, data, hsize, len_in_bytes, spi_config, dma_flags);
#else
	qspi_auto_read(cs_no, addr, data, hsize, len_in_bytes, spi_config, dma_flags);
#endif
}

/**
 *  @fn      STATIC INLINE void  RSI_QSPI_FlashInit(qspi_reg_t *qspi_reg,spi_config_t *spi_config,uint32_t wr_reg_delay_ms)
 *  @brief   This function initializes  QSPI_flash
 *  @param   qspi_reg         :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   spi_config       :  pointer to spi_config_t structure,spi configuration \ref spi_config_t
 *  @param   wr_reg_delay_ms  :  delay in ms provided after a register ,write operation is performed on flash
 *  @return  none
 */
STATIC INLINE void  RSI_QSPI_FlashInit(qspi_reg_t *qspi_reg,spi_config_t *spi_config,uint32_t wr_reg_delay_ms)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_flash_init(qspi_reg,  spi_config, wr_reg_delay_ms);
#else
	qspi_flash_init(qspi_reg,  spi_config, wr_reg_delay_ms);
#endif
}

/**
 *  @fn      STATIC INLINE void  RSI_QSPI_SpiInit(qspi_reg_t *qspi_reg,spi_config_t *spi_config,uint32_t flash_init_req,
                                   uint32_t wr_reg_delay_ms, uint8_t fifo_thrsld)
 *  @brief   This function initializes GPIO, QSPI and flash
 *  @param   qspi_reg          :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   spi_config        :  pointer to spi_config_t structure,spi configuration \ref spi_config_t
 *  @param   flash_init_req    :  if set only then the qspi_flash_init is called,
 *                                allows to skip qspi_flash_init in case of ulp_wakeup
 *  @param   wr_reg_delay_ms   :  delay in ms provided after a register ,write operation is performed on flash
 *  @param   fifo_thrsld       : FIFO threshold value 
 *  @return  none
 *  @note    It is expected that gpio init for qspi is already done by the caller
 */
STATIC INLINE void  RSI_QSPI_SpiInit(qspi_reg_t *qspi_reg,spi_config_t *spi_config,uint32_t flash_init_req,uint32_t wr_reg_delay_ms, uint8_t fifo_thrsld)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_spi_init(qspi_reg,  spi_config, flash_init_req, wr_reg_delay_ms, fifo_thrsld);
#else
	qspi_spi_init(qspi_reg,  spi_config, flash_init_req, wr_reg_delay_ms, fifo_thrsld);
#endif
}

/**
 *  @fn      STATIC INLINE void  RSI_QSPI_SpiErase(qspi_reg_t *qspi_reg, spi_config_t *spi_config, uint32_t erase_cmd,
                                    uint32_t blk_sec_addr, uint32_t dis_hw_ctrl, uint32_t wr_reg_delay_ms)
 *  @brief   This function erases the flash
 *  @param   qspi_reg          :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   spi_config        :  pointer to spi_config_t structure,spi configuration \ref spi_config_t
 *  @param   erase_cmd         :  Erase command to be used
 *  @param   blk_sec_addr      :  Block or Sector Address, in case of block/sector erase
 *  @param   dis_hw_ctrl       :  Disable hw ctrl, while waiting for flash to go idle
 *  @param   wr_reg_delay_ms   :  delay in ms provided after a register ,write operation is performed on flash
 *  @return  none
 */
STATIC INLINE void  RSI_QSPI_SpiErase(qspi_reg_t *qspi_reg, spi_config_t *spi_config, uint32_t erase_cmd, uint32_t blk_sec_addr, uint32_t dis_hw_ctrl, uint32_t wr_reg_delay_ms)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_spi_erase(qspi_reg,  spi_config, erase_cmd, blk_sec_addr, dis_hw_ctrl, wr_reg_delay_ms);
#else
	qspi_spi_erase(qspi_reg,  spi_config, erase_cmd, blk_sec_addr, dis_hw_ctrl, wr_reg_delay_ms);
#endif
}

/**
 *  @fn      STATIC INLINE uint32_t  RSI_QSPI_SpiWrite(qspi_reg_t *qspi_reg, spi_config_t *spi_config, uint32_t write_cmd,
 *                                       uint32_t addr, uint8_t  *data,uint32_t len_in_bytes, uint16_t page_size,
 *                                       uint32_t hsize, uint32_t dis_hw_ctrl, uint32_t wr_reg_delay_ms,
 *                                       uint32_t check_en,uint32_t udma_enable,void *udmaHandle,void *rpdmaHandle)
 *  @brief   This function writes to flash using qspi
 *  @param   qspi_reg          :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   spi_config        :  pointer to spi_config_t structure,spi configuration \ref spi_config_t
 *  @param   write_cmd         :  write command to be used
 *  @param   addr              :  Address in flash memory
 *  @param   data              : Pointer to  data buffer 
 *  @param   len_in_bytes      :  no. of bytes
 *  @param   page_size         :  Max.burst size allowed by flash.
 *  @param   hsize             :  8 bits, 16 bits or 32 bits xfer on AHB
 *  @param   dis_hw_ctrl       :  Disable hw ctrl, while waiting for flash to go idle
 *  @param   wr_reg_delay_ms   :  delay in ms provided after a register ,write operation is performed on flash
 *  @param   check_en          :  check enable is 1 then data read again and compare with transmit data       :
 *  @param   udma_enable       :  1 : uses UDMA, 0 - uses RPDMA
 *  @param   udmaHandle        :  udma context handler
 *  @param   rpdmaHandle       :  rpdma context handler
 *  @return  return the status is return is zero then successfully data write if non-zero then fail.
 */
STATIC INLINE uint32_t  RSI_QSPI_SpiWrite(qspi_reg_t *qspi_reg, spi_config_t *spi_config, uint32_t write_cmd, uint32_t addr, uint8_t  *data,	uint32_t len_in_bytes, uint16_t page_size, uint32_t hsize, uint32_t dis_hw_ctrl, uint32_t wr_reg_delay_ms,uint32_t check_en,uint32_t udma_enable,void *udmaHandle,void *rpdmaHandle)
{
#if defined(ROMDRIVER_PRESENT )
	return ROMAPI_QSPI_API->qspi_spi_write(qspi_reg,  spi_config, write_cmd, addr, data, len_in_bytes, page_size, hsize, dis_hw_ctrl, wr_reg_delay_ms,check_en,udma_enable,udmaHandle,rpdmaHandle);
#else
	return qspi_spi_write(qspi_reg,  spi_config, write_cmd, addr, data, len_in_bytes, page_size, hsize, dis_hw_ctrl, wr_reg_delay_ms,check_en,udma_enable,udmaHandle,rpdmaHandle);
#endif
}

/**
 *  @fn      STATIC INLINE void  RSI_QSPI_SpiRead(qspi_reg_t *qspi_reg,	spi_config_t *spi_config,	uint32_t addr,uint8_t  *data,
 *                                   uint32_t  hsize,	uint32_t len_in_bytes, uint32_t manual_udma_read,
 *                                  void *udmaHandle,  void *rpdmaHandle)
 *  @brief   This function is a mother function to RSI_QSPI_AutoRead & RSI_QSPI_ManualRead
 *  @param   qspi_reg         :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   spi_config       :  pointer to spi_config_t structure,spi configuration \ref spi_config_t
 *  @param   addr             :  Address in flash memory
 *  @param   data             :  Pointer to read data buffer
 *  @param   hsize            : 8 bits, 16 bits or 32 bits xfer on AHB
 *  @param   len_in_bytes     : read count in bytes
 *  @param   manual_udma_read :  1 : uses UDMA, 0 - uses RPDMA
 *  @param   udmaHandle       :  udma context handler
 *  @param   rpdmaHandle      :  rpdma context handler
 *  @return  none
 */
STATIC INLINE void  RSI_QSPI_SpiRead(qspi_reg_t *qspi_reg,	spi_config_t *spi_config,	uint32_t addr,uint8_t  *data,	uint32_t  hsize,	uint32_t len_in_bytes, uint32_t manual_udma_read, void *udmaHandle,  void *rpdmaHandle)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_spi_read(qspi_reg,  spi_config, addr, data, hsize, len_in_bytes, manual_udma_read,udmaHandle,rpdmaHandle);
#else
	qspi_spi_read(qspi_reg,  spi_config, addr, data, hsize, len_in_bytes, manual_udma_read,udmaHandle, rpdmaHandle);
#endif
}

/**
 *  @fn      STATIC INLINE void  RSI_QSPI_Usleep(uint32_t delay)
 *  @brief   This function is used gives delay.
 *  @param   delay  : uint32_t number of delay which we want to gives
 *  @return  none
 */
STATIC INLINE void  RSI_QSPI_Usleep(uint32_t delay_us)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_usleep(delay_us);
#else
	qspi_usleep(delay_us);
#endif
}

/**
 *  @fn      STATIC INLINE void  RSI_QSPI_WriteBlockProtect(qspi_reg_t *qspi_reg,	uint32_t protect,
                                            uint32_t cs_no,	uint32_t num_prot_bytes,	uint32_t wr_reg_delay_ms)
 *  @brief   This function writes to block protection reg of SST_QUAD_FLASH
 *  @param   qspi_reg        :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   protect         :  Block protection reg value
 *  @param   cs_no           :  chip select no.
 *  @param   num_prot_bytes  :  protection register length either 6 (SST26VF016)
 *  												    or 10 bytes (SST26VF032)
 *  @param   wr_reg_delay_ms :  delay in ms provided after a register write
 *  												    operation is performed on flash
 *  @return  none
 */
STATIC INLINE void  RSI_QSPI_WriteBlockProtect(qspi_reg_t *qspi_reg,	uint32_t protect,	uint32_t cs_no,	uint32_t num_prot_bytes,	uint32_t wr_reg_delay_ms)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_write_block_protect(qspi_reg,  protect, cs_no, num_prot_bytes, wr_reg_delay_ms);
#else
	qspi_write_block_protect(qspi_reg,  protect, cs_no, num_prot_bytes, wr_reg_delay_ms);
#endif
}

/**
 *  @fn      STATIC INLINE void  RSI_QSPI_QspiLoadKey(qspi_reg_t *qspi_reg, uint32_t *key, uint32_t kh_enable)
 *  @brief   This function is used to load the AES key to QSPI Controller.
 *  @param   qspi_reg        :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   key  : pointer to the key
 *  @param   kh_enable : Enables or disables
 *            \n 0 : Disables
 *            \n 1 : Enables 
 *  @return  none
 */
STATIC INLINE void  RSI_QSPI_QspiLoadKey(qspi_reg_t *qspi_reg,	uint32_t *key, uint32_t kh_enable)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_qspiload_key(qspi_reg, key, kh_enable);
#else
	qspi_qspiload_key(qspi_reg, key, kh_enable);
#endif
}

/**
 *  @fn      STATIC INLINE void  RSI_QSPI_QspiLoadNonce(qspi_reg_t *qspi_reg, uint32_t *nonce)
 *  @brief   This function is used to load the Nonce to QSPI Controller.
 *           M4 QSPI controller is a secure controller, it can decrypt the data inline while fetching from flash
 *           Nonce is a 12 byte random data that is appended with flash offset address in generating cipher
 *  @param   qspi_reg        :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   nonce  : pointer to the nonce
 *  @return  none
 */
STATIC INLINE void  RSI_QSPI_QspiLoadNonce(qspi_reg_t *qspi_reg, uint32_t *nonce)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_qspiload_nonce(qspi_reg, nonce);
#else
	qspi_qspiload_nonce(qspi_reg, nonce);
#endif
}

/**
 *  @fn      STATIC INLINE void  RSI_QSPI_SegSecEn(qspi_reg_t *qspi_reg, uint32_t seg_no, uint32_t start_addr, uint32_t end_addr)
 *  @brief   This function is used to program secure segments of flash to QSPI controller.
 *  @param   qspi_reg        :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   seg_no      : segment number
 *  @param   start_addr  : start address of segment in flash
 *  @param   end_addr    : end address of segment in flash
 *  @return  none
 */
STATIC INLINE void  RSI_QSPI_SegSecEn(qspi_reg_t *qspi_reg,	uint32_t seg_no,	uint32_t start_addr,	uint32_t end_addr)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_seg_sec_en(qspi_reg, seg_no, start_addr, end_addr);
#else
	qspi_seg_sec_en(qspi_reg, seg_no, start_addr, end_addr);
#endif
}

/**
 *  @fn      STATIC INLINE void  RSI_QSPI_StatusControlRegWrite(spi_config_t * spi_config,	qspi_reg_t *qspi_reg,	uint16_t write_command,	uint32_t addr,
 *                                  	uint16_t write_value,uint32_t cs_no,uint32_t wr_reg_delay_ms)
 *  @brief   This function is used to program secure segments of flash to QSPI controller.
 *  @param   spi_config       :  pointer to spi_config_t structure,spi configuration \ref spi_config_t
 *  @param   qspi_reg         :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   write_command     :  write command to be used
 *  @param   addr             :  Address in flash memory
 *  @param   write_value      :  Value to write into register
 *  @param   cs_no            :  chip select no.
 *  @param   wr_reg_delay_ms  :  delay in ms provided after a register write operation is performed on flash
 *  @return  none
 */
STATIC INLINE void  RSI_QSPI_StatusControlRegWrite(spi_config_t * spi_config,	qspi_reg_t *qspi_reg,	uint16_t write_command,	uint32_t addr,	uint16_t write_value,	uint32_t cs_no,uint32_t wr_reg_delay_ms)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_status_control_reg_write(spi_config, qspi_reg,  write_command, addr, write_value, cs_no, wr_reg_delay_ms);
#else
	qspi_status_control_reg_write(spi_config, qspi_reg,  write_command, addr, write_value, cs_no, wr_reg_delay_ms);
#endif

}

/**
 *  @fn      STATIC INLINE void  RSI_QSPI_FlashProtection(spi_config_t *spi_config,qspi_reg_t *qspi_reg,	uint32_t protection, uint32_t wr_reg_delay_ms)
 *  @brief   This function is used to program secure segments of flash to QSPI controller.
 *  @param   spi_config       :  pointer to spi_config_t structure,spi configuration \ref spi_config_t
 *  @param   qspi_reg         :  pointer to qspi_reg_t structure,contains all qspi regsisters \ref qspi_reg_t
 *  @param   protection       :  number of value which is we want protect.
 *  @param   wr_reg_delay_ms  :  delay in ms provided after a register write operation is performed on flash
 *  @return  none
 */
STATIC INLINE void  RSI_QSPI_FlashProtection(spi_config_t *spi_config,qspi_reg_t *qspi_reg,	uint32_t protection, uint32_t wr_reg_delay_ms)
{
#if defined(ROMDRIVER_PRESENT )
	ROMAPI_QSPI_API->qspi_flash_protection(spi_config, qspi_reg,  protection, wr_reg_delay_ms);
#else
	qspi_flash_protection(spi_config, qspi_reg,  protection, wr_reg_delay_ms);
#endif
}

#ifdef __cplusplus
}

#endif

#endif /*__RSI_ROM_QSPI_H__*/

/* @} end of RSI_QSPI_DRIVERS */
