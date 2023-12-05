/*

 *                                     _oo0oo_
 *                                    o8888888o
 *                                    88" . "88
 *                                    (| -_- |)
 *                                    0\  =  /0
 *                                  ___/`---'\___
 *                                .' \\|     |// '.
 *                               / \\|||  :  |||// \
 *                              / _||||| -:- |||||- \
 *                             |   | \\\  - /// |   |
 *                             | \_|  ''\---/''  |_/ |
 *                             \  .-\__  '-'  ___/-. /
 *                           ___'. .'  /--.--\  `. .'___
 *                        ."" '<  `.___\_<|>_/___.' >' "".
 *                       | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *                       \  \ `_.   \_ __\ /__ _/   .-` /  /
 *                   =====`-.____`.___ \_____/___.-`___.-'=====
 *                                     `=---='
 *
 *                             佛祖保佑，万事顺遂，不出BUG
 *                   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  说明 开始   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*--------------------------------  说明 结束   -------------------------------*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  #INCLUDE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include <stdio.h>
#include "gd32f30x.h"
#include "SDIO.h"
#include "SD_Card.h"
#include "string.h"
#include "sys.h"
#include "LCD.h"
/*-----------------------------  #INCLUDE   END   ----------------------------*/

/* card status of R1 definitions */
#define SD_R1_OUT_OF_RANGE BIT(31)			  /* command's argument was out of the allowed range */
#define SD_R1_ADDRESS_ERROR BIT(30)			  /* misaligned address which did not match the block length */
#define SD_R1_BLOCK_LEN_ERROR BIT(29)		  /* transferred block length is not allowed */
#define SD_R1_ERASE_SEQ_ERROR BIT(28)		  /* an error in the sequence of erase commands occurred */
#define SD_R1_ERASE_PARAM BIT(27)			  /* an invalid selection of write-blocks for erase occurred */
#define SD_R1_WP_VIOLATION BIT(26)			  /* the host attempts to write to a protected block or to the temporary or permanent write protected card */
#define SD_R1_CARD_IS_LOCKED BIT(25)		  /* the card is locked by the host */
#define SD_R1_LOCK_UNLOCK_FAILED BIT(24)	  /* a sequence or password error has been detected in lock/unlock card command */
#define SD_R1_COM_CRC_ERROR BIT(23)			  /* CRC check of the previous command failed */
#define SD_R1_ILLEGAL_COMMAND BIT(22)		  /* command not legal for the card state */
#define SD_R1_CARD_ECC_FAILED BIT(21)		  /* card internal ECC was applied but failed to correct the data */
#define SD_R1_CC_ERROR BIT(20)				  /* internal card controller error */
#define SD_R1_GENERAL_UNKNOWN_ERROR BIT(19)	  /* a general or an unknown error occurred during the operation */
#define SD_R1_CSD_OVERWRITE BIT(16)			  /* read only section of the CSD does not match or attempt to reverse the copy or permanent WP bits */
#define SD_R1_WP_ERASE_SKIP BIT(15)			  /* partial address space was erased */
#define SD_R1_CARD_ECC_DISABLED BIT(14)		  /* command has been executed without using the internal ECC */
#define SD_R1_ERASE_RESET BIT(13)			  /* an erase sequence was cleared before executing */
#define SD_R1_READY_FOR_DATA BIT(8)			  /* correspond to buffer empty signaling on the bus */
#define SD_R1_APP_CMD BIT(5)				  /* card will expect ACMD */
#define SD_R1_AKE_SEQ_ERROR BIT(3)			  /* error in the sequence of the authentication process */
#define SD_R1_ERROR_BITS (uint32_t)0xFDF9E008 /* all the R1 error bits */

/* card status of R6 definitions */
#define SD_R6_COM_CRC_ERROR BIT(15)			/* CRC check of the previous command failed */
#define SD_R6_ILLEGAL_COMMAND BIT(14)		/* command not legal for the card state */
#define SD_R6_GENERAL_UNKNOWN_ERROR BIT(13) /* a general or an unknown error occurred during the operation */

/* card state */
#define SD_CARDSTATE_IDLE ((uint8_t)0x00)		   /* card is in idle state */
#define SD_CARDSTATE_READY ((uint8_t)0x01)		   /* card is in ready state */
#define SD_CARDSTATE_IDENTIFICAT ((uint8_t)0x02)   /* card is in identificat state */
#define SD_CARDSTATE_STANDBY ((uint8_t)0x03)	   /* card is in standby state */
#define SD_CARDSTATE_TRANSFER ((uint8_t)0x04)	   /* card is in transfer state */
#define SD_CARDSTATE_DATA ((uint8_t)0x05)		   /* card is in data sending state */
#define SD_CARDSTATE_RECEIVING ((uint8_t)0x06)	   /* card is in receiving state */
#define SD_CARDSTATE_PROGRAMMING ((uint8_t)0x07)   /* card is in programming state */
#define SD_CARDSTATE_DISCONNECT ((uint8_t)0x08)	   /* card is in disconnect state */
#define SD_CARDSTATE_LOCKED ((uint32_t)0x02000000) /* card is in locked state */

#define SD_CHECK_PATTERN ((uint32_t)0x000001AA)	 /* check pattern for CMD8 */
#define SD_VOLTAGE_WINDOW ((uint32_t)0x80100000) /* host 3.3V request in ACMD41 */

/* parameters for ACMD41(voltage validation) */
#define SD_HIGH_CAPACITY ((uint32_t)0x40000000) /* high capacity SD memory card */
#define SD_STD_CAPACITY ((uint32_t)0x00000000)	/* standard capacity SD memory card */

/* SD bus width, check SCR register */
#define SD_BUS_WIDTH_4BIT ((uint32_t)0x00040000) /* 4-bit width bus mode */
#define SD_BUS_WIDTH_1BIT ((uint32_t)0x00010000) /* 1-bit width bus mode */

/* masks for SCR register */
#define SD_MASK_0_7BITS ((uint32_t)0x000000FF)	 /* mask [7:0] bits */
#define SD_MASK_8_15BITS ((uint32_t)0x0000FF00)	 /* mask [15:8] bits */
#define SD_MASK_16_23BITS ((uint32_t)0x00FF0000) /* mask [23:16] bits */
#define SD_MASK_24_31BITS ((uint32_t)0xFF000000) /* mask [31:24] bits */

#define SDIO_FIFO_ADDR ((uint32_t)SDIO + 0x80U)	 /* address of SDIO_FIFO */
#define SD_FIFOHALF_WORDS ((uint32_t)0x00000008) /* words of FIFO half full/empty */
#define SD_FIFOHALF_BYTES ((uint32_t)0x00000020) /* bytes of FIFO half full/empty */

#define SD_DATATIMEOUT ((uint32_t)0xFFFFFFFF)		  /* DSM data timeout */
#define SD_MAX_VOLT_VALIDATION ((uint32_t)0x0000FFFF) /* the maximum times of voltage validation */
#define SD_MAX_DATA_LENGTH ((uint32_t)0x01FFFFFF)	  /* the maximum length of data */
#define SD_ALLZERO ((uint32_t)0x00000000)			  /* all zero */
#define SD_RCA_SHIFT ((uint8_t)0x10)				  /* RCA shift bits */
#define SD_CLK_DIV_INIT ((uint16_t)0x012A)			  /* SD clock division in initilization phase */
#define SD_CLK_DIV_TRANS ((uint16_t)0x0008)			  /* SD clock division in transmission phase */

#define SDIO_MASK_INTC_FLAGS ((uint32_t)0x00C007FF) /* mask flags of SDIO_INTC */

/*~~~~~~~~~~~~~~~~~~~~~~~~~  DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/
static sdio_card_type_enum cardtype = SDIO_STD_CAPACITY_SD_CARD_V1_1; /* SD card type */
uint32_t sd_scr[2] = {0, 0};										  /* content of SCR register */
static uint32_t sd_csd[4] = {0,0,0,0};                                /* content of CSD register */
static uint32_t sd_cid[4] = {0,0,0,0};                                /* content of CID register */
static uint16_t sd_rca = 0;     
sd_card_info_struct sd_cardinfo;                            /* information of SD card */
static uint32_t transmode = SD_POLLING_MODE;
/*-------------------------  DECLARE  VARIABLE  END   ------------------------*/

/*~~~~~~~~~~~~~~~~~~~~~  REPEAT DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~*/

/*---------------------  REPEAT DECLARE  VARIABLE  END   ---------------------*/

/*********************************************************************
 * @fn       : </函数名>
 * @brief    : <检查CMD0的执行状态>
 * @note     : <复制来的>
 * @param    : <param>
 * @return   : <sd卡错误码>
 * @author   : <editor>
 * @date     :
 ********************************************************************/
sd_error_enum CMD_Error_Check(void)
{
	/*1-这里相当于初始化返回变量*/
	sd_error_enum errorstatus = SD_OK;
	u32 timeout = SDIO_CMD0TIMEOUT; // 10000

	/*2-等待回复?,反正就是等待*/
	while (timeout--)
	{
		if (sdio_flag_get(SDIO_FLAG_CMDSEND) != RESET)
			break; // 命令已发送(无需响应)
	}

	/*3-判断退出的原因*/
	if (timeout == 0)
	{
		return SD_CMD_RESP_TIMEOUT;
	}
	sdio_flag_clear(SDIO_MASK_INTC_FLAGS); // 清除所有标记
	return errorstatus;
}

/*********************************************************************
 * @fn       : <r1_error_check/函数名>
 * @brief    : <check if error occurs for R1 response>
 * @note     : <>
 * @param    : <param>
 * @return   : <sd_error_enum>
 * @author   : <editor>
 * @date     :
 ********************************************************************/
static sd_error_enum r1_error_check(uint8_t cmdindex)
{
	sd_error_enum errorstatus = SD_OK;
	u32 reg_status = 0, resp_r1 = 0;
	__IO u32 timeout = SDIO_CMD0TIMEOUT;

	/*1-store the content of SDIO_STAT */
	reg_status = SDIO_STAT;
	while (!(reg_status & (SDIO_FLAG_CCRCERR | SDIO_FLAG_CMDTMOUT | SDIO_FLAG_CMDRECV))) // 这里可以看成是等待收到回复
	{
		reg_status = SDIO_STAT; // 不断的更新reg_status 内的值，取自于 SDIO_STAT
	}

	/*2-check whether an error or timeout occurs or command response received 查询回复的具体值或者是否有回复*/
	if (reg_status & SDIO_FLAG_CCRCERR) // 收到数据,但是CRC校验错误
	{
		errorstatus = SD_CMD_CRC_ERROR;
		sdio_flag_clear(SDIO_FLAG_CCRCERR);
		return errorstatus;
	}
	else if (reg_status & SDIO_FLAG_CMDTMOUT) // 超时标志
	{
		errorstatus = SD_CMD_RESP_TIMEOUT;
		sdio_flag_clear(SDIO_FLAG_CMDTMOUT);
		return errorstatus;
	}

	/*3-check whether the last response command index is the desired one 检查最后的一个回复 是不是想要的*/
	if (sdio_command_index_get() != cmdindex)
	{
		errorstatus = SD_ILLEGAL_COMMAND;
		return errorstatus;
	}

	/*4-clear all the SDIO_INTC flags 上面都完成了，到现在还没跳出，那么这里就要全部进行一下清理了*/
	sdio_flag_clear(SDIO_MASK_INTC_FLAGS);

	/*5-get the SDIO response register 0 for checking 获取接收（反馈）寄存器0内的值进行检查*/
	resp_r1 = sdio_response_get(SDIO_RESPONSE0); // 不明所以
	if (SD_ALLZERO == (resp_r1 & SD_R1_ERROR_BITS))
	{
		/* no error occurs, return SD_OK */
		errorstatus = SD_OK;
		return errorstatus;
	}

	/*6-if some error occurs, return the error type */
	errorstatus = r1_error_type_check(resp_r1);
	return errorstatus;
}

/*********************************************************************
 * @fn       : <r1_error_check/函数名>
 * @brief    : <check if error occurs for R1 response>
 * @note     : <>
 * @param    : <param>
 * @return   : <sd_error_enum>
 * @author   : <editor>
 * @date     :
 ********************************************************************/
sd_error_enum r1_error_type_check(uint32_t resp)
{
	sd_error_enum status = SD_ERROR;
	/* check which error occurs */
	if (resp & SD_R1_OUT_OF_RANGE)
	{
		status = SD_OUT_OF_RANGE;
	}
	else if (resp & SD_R1_ADDRESS_ERROR)
	{
		status = SD_ADDRESS_ERROR;
	}
	else if (resp & SD_R1_BLOCK_LEN_ERROR)
	{
		status = SD_BLOCK_LEN_ERROR;
	}
	else if (resp & SD_R1_ERASE_SEQ_ERROR)
	{
		status = SD_ERASE_SEQ_ERROR;
	}
	else if (resp & SD_R1_ERASE_PARAM)
	{
		status = SD_ERASE_PARAM;
	}
	else if (resp & SD_R1_WP_VIOLATION)
	{
		status = SD_WP_VIOLATION;
	}
	else if (resp & SD_R1_LOCK_UNLOCK_FAILED)
	{
		status = SD_LOCK_UNLOCK_FAILED;
	}
	else if (resp & SD_R1_COM_CRC_ERROR)
	{
		status = SD_COM_CRC_ERROR;
	}
	else if (resp & SD_R1_ILLEGAL_COMMAND)
	{
		status = SD_ILLEGAL_COMMAND;
	}
	else if (resp & SD_R1_CARD_ECC_FAILED)
	{
		status = SD_CARD_ECC_FAILED;
	}
	else if (resp & SD_R1_CC_ERROR)
	{
		status = SD_CC_ERROR;
	}
	else if (resp & SD_R1_GENERAL_UNKNOWN_ERROR)
	{
		status = SD_GENERAL_UNKNOWN_ERROR;
	}
	else if (resp & SD_R1_CSD_OVERWRITE)
	{
		status = SD_CSD_OVERWRITE;
	}
	else if (resp & SD_R1_WP_ERASE_SKIP)
	{
		status = SD_WP_ERASE_SKIP;
	}
	else if (resp & SD_R1_CARD_ECC_DISABLED)
	{
		status = SD_CARD_ECC_DISABLED;
	}
	else if (resp & SD_R1_ERASE_RESET)
	{
		status = SD_ERASE_RESET;
	}
	else if (resp & SD_R1_AKE_SEQ_ERROR)
	{
		status = SD_AKE_SEQ_ERROR;
	}
	return status;
}


/*********************************************************************
 * @fn       : <r2_error_check/函数名>
 * @brief    : <check if error occurs for R2 response>
 * @note     : <感觉对于所有的CMD指令，都配备了相应的回复指令判断功能。>
 * @param    : <param>
 * @return   : <sd_error_enum>
 * @author   : <editor>
 * @date     :
 ********************************************************************/
static sd_error_enum r2_error_check(void)
{
	sd_error_enum error_status=SD_OK;
	u32 reg_status =0;			//应该是用来存寄存器的值得

	/*1-获取sdio装态寄存器的返回值，当超时、接收到或…… 都行*/
	reg_status=SDIO_STAT;
	while(!(reg_status&(SDIO_FLAG_CCRCERR | SDIO_FLAG_CMDTMOUT | SDIO_FLAG_CMDRECV)))
	{
		reg_status=SDIO_STAT;			//这里就是类似于等待回复，一直读寄存器内的回复值
	}

	/*2-根据收回到的3个指令判断究竟是哪个指令*/
	if(reg_status&SDIO_FLAG_CCRCERR)
	{
		error_status=SD_CMD_CRC_ERROR;
		sdio_flag_clear(SD_CMD_CRC_ERROR);
		return error_status;
	}

	/*3-如果是超时了*/
	else if(reg_status&SDIO_FLAG_CMDTMOUT)
	{
		error_status= SD_CMD_RESP_TIMEOUT;
		sdio_flag_clear(SDIO_FLAG_CMDTMOUT);
		return error_status;
	}

	/*4-前面两个错都没有，那么就是第3个了，所以要清楚所有的标志，然后返回呗*/
	sdio_flag_clear(SDIO_MASK_INTC_FLAGS);			//所有的标志全清
	return error_status;

}


/*********************************************************************
 * @fn       : <r3_error_check/函数名>
 * @brief    : <check if error occurs for R3 response>
 * @note     : <>
 * @param    : <param>
 * @return   : <sd_error_enum>
 * @author   : <editor>
 * @date     :
 ********************************************************************/
static sd_error_enum r3_error_check(void)
{
	sd_error_enum error_status = SD_OK;
	u32 reg_status = 0;

	/*1- 获取SDIO_STAT 寄存器内的值*/
	reg_status = SDIO_STAT;
	while (!(reg_status & (SDIO_FLAG_CCRCERR | SDIO_FLAG_CMDTMOUT | SDIO_FLAG_CMDRECV)))
	{
		reg_status = SDIO_STAT;
	}

	if (reg_status & SDIO_FLAG_CMDTMOUT)
	{
		error_status = SD_CMD_RESP_TIMEOUT;
		sdio_flag_clear(SDIO_FLAG_CMDTMOUT);
		return error_status;
	}

	/* clear all the SDIO_INTC flags */
	sdio_flag_clear(SDIO_MASK_INTC_FLAGS);
	return error_status;
}


/*********************************************************************
 * @fn       : <function_name/函数名>
 * @brief    : <brief>
 * @note     : <note>
 * @param    : <cmdindex: the index of command
 * 				prca: a pointer that store the RCA of card>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
static sd_error_enum r6_error_check(uint8_t cmdindex, uint16_t *prca)
{
	sd_error_enum errorstatus;
	u32 reg_status=0,response=0;
	/*sdio the content of sdio_stat*/
	reg_status=SDIO_STAT;
	while(!(reg_status&(SDIO_FLAG_CCRCERR|SDIO_FLAG_CMDTMOUT|SDIO_FLAG_CMDRECV)))
	{
		reg_status=SDIO_STAT;
	}

	/* check whether an error or timeout occurs or command response received */
	if(reg_status&SDIO_FLAG_CCRCERR)
	{
		errorstatus=SD_CMD_CRC_ERROR;
		sdio_flag_clear(SDIO_FLAG_CCRCERR);
		return errorstatus;
	}
	else if(reg_status&SDIO_FLAG_CMDTMOUT)
	{
		errorstatus=SD_CMD_RESP_TIMEOUT;
		sdio_flag_clear(SDIO_FLAG_CMDTMOUT);
		return errorstatus;
	}

	/* check whether the last response command index is the desired one */
	if(sdio_command_index_get()!=cmdindex)
	{
		errorstatus=SD_ILLEGAL_COMMAND;
		return errorstatus;
	}

	/* clear all the SDIO_INTC flags */
    sdio_flag_clear(SDIO_MASK_INTC_FLAGS);

	/* get the SDIO response register 0 for checking */
    response = sdio_response_get(SDIO_RESPONSE0);

	if(SD_ALLZERO==(response&(SD_R6_COM_CRC_ERROR|SD_R6_ILLEGAL_COMMAND|SD_R6_GENERAL_UNKNOWN_ERROR)))
	{
		*prca=(u16)(response>>16);
		return errorstatus;
	}

	/*if some error occurs ,return the error type*/
	if(errorstatus&SD_R6_COM_CRC_ERROR)
	{
		errorstatus=SD_COM_CRC_ERROR;
	}
	else if (errorstatus & SD_R6_ILLEGAL_COMMAND)
	{
		errorstatus=SD_ILLEGAL_COMMAND;
	}
	else if(errorstatus & SD_R6_GENERAL_UNKNOWN_ERROR)
	{
		errorstatus=SD_GENERAL_UNKNOWN_ERROR;
	}
	return errorstatus;
}

/*********************************************************************
 * @fn       : <r7_error_check/函数名>
 * @brief    : <check if error occurs for R7 response>
 * @note     : <>
 * @param    : <param>
 * @return   : <sd_error_enum>
 * @author   : <editor>
 * @date     :
 ********************************************************************/
static sd_error_enum r7_error_check(void)
{
	sd_error_enum errorstatus = SD_ERROR;
	u32 reg_status = 0;
	__IO u32 timeout = SDIO_CMD0TIMEOUT;

	/*1- 等待回复store the content of SDIO_STAT */
	reg_status = SDIO_STAT; /*!< SDIO status register */
							/*没有超时，且没有以下3个回复，才会一直运行下去*/
	while (!(reg_status & (SDIO_FLAG_CCRCERR | SDIO_FLAG_CMDTMOUT | SDIO_FLAG_CMDRECV)) && (timeout > 0))
	{
		reg_status = SDIO_STAT;
		--timeout;
	}

	/*2-查询回复的具体值或者是否有回复*/
	if ((reg_status & SDIO_FLAG_CMDTMOUT) || timeout == 0) // 超时了
	{
		errorstatus = SD_CMD_RESP_TIMEOUT;
		return errorstatus;
	}

	if (reg_status & SDIO_FLAG_CMDRECV) // 成功
	{
		errorstatus = SD_OK;
		sdio_flag_clear(SDIO_FLAG_CMDRECV);
		return errorstatus;
	}
	return errorstatus;
}

/*********************************************************************
 * @fn       : </函数名>
 * @brief    : <SD卡上电函数>
 * @note     : <note>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     :
 ********************************************************************/
sd_error_enum SD_Power_on(void)
{
	u8 i = 0;
	sd_error_enum errorstatus = SD_OK;
	u32 response = 0;
	u32 count = 0, validvoltage = 0;
	u32 SDType = SD_STD_CAPACITY; // is also called sdcardtype
	SDIO_SMD_Send_struct SDIO_Send_Data;
	u8 busyflag = 0;

	/*1-时钟配置、数据线宽度和硬件流(在stm32的库函数中，这是一个函数就设置了)*/
	/*HCLK = 120MHz, SDIOCLK = 120MHz, SDIO_CK = HCLK/(178 + 2) = 400 KHz*/
	sdio_clock_config(SDIO_SDIOCLKEDGE_RISING, SDIO_CLOCKBYPASS_DISABLE, SDIO_CLOCKPWRSAVE_DISABLE, SDIO_INIT_CLK_DIV);
	sdio_bus_mode_set(SDIO_BUSMODE_1BIT);
	sdio_hardware_clock_disable();

	/*2-配置上电状态*/
	sdio_power_state_set(SDIO_POWER_ON);

	/*3-开启SDIO-CK时钟，时钟使能*/
	sdio_clock_enable();

	/*4-上电发送等待74个低速时钟，使其进入IDLE STAGE模式*/
	for (i = 0; i < 74; i++)
	{
		SDIO_Send_Data.index = SD_CMD_GO_IDLE_STATE;
		SDIO_Send_Data.argument = 0x00;
		SDIO_Send_Data.response_type = SDIO_RESPONSETYPE_NO;
		SDIO_Send_Data.wait_type = SDIO_WAITTYPE_NO;
		SDIO_SendCommand(&SDIO_Send_Data);

		errorstatus = CMD_Error_Check();
		if (errorstatus == SD_OK)
		{
			break;
		}
	}
	if (errorstatus != SD_OK)
	{
		return errorstatus;
	}

	/*5-发送CMD8，短响应，检查SD卡接口的特性*/
	SDIO_Send_Data.index = SD_CMD_SEND_IF_COND;
	SDIO_Send_Data.argument = SD_CHECK_PATTERN;
	SDIO_Send_Data.response_type = SDIO_RESPONSETYPE_SHORT;
	SDIO_Send_Data.wait_type = SDIO_WAITTYPE_NO;
	SDIO_SendCommand(&SDIO_Send_Data); // 内含   sdio_csm_enable();

	if (SD_OK == r7_error_check()) /* SD Card 2.0 */
	{
		cardtype = SDIO_STD_CAPACITY_SD_CARD_V2_0; // SD 2.0卡
		SDType = SD_HIGH_CAPACITY;				   // 高容量卡
	}

	/*6-send CMD55(APP_CMD) to indicate next command is application specific command ,
	为下一步发送CMD41做准备，先发CMD55*/
	SDIO_Send_Data.index = SD_CMD_APP_CMD;
	SDIO_Send_Data.argument = (uint32_t)0x0; // SD_STD_CAPACITY;
	SDIO_Send_Data.response_type = SDIO_RESPONSETYPE_SHORT;
	SDIO_Send_Data.wait_type = SDIO_WAITTYPE_NO;
	SDIO_SendCommand(&SDIO_Send_Data); // 内含   sdio_csm_enable();

	if (SD_OK == r1_error_check(SD_CMD_APP_CMD)) /* SD memory card */
	{
		// while((!busyflag) && (count < SD_MAX_VOLT_VALIDATION))
		while ((!busyflag) && (count < 0xffff))
		{
			/* 6-1 send CMD55(APP_CMD) to indicate next command is application specific command */
			SDIO_Send_Data.index = SD_CMD_APP_CMD;
			SDIO_Send_Data.argument = 0x00; // SD_STD_CAPACITY;
			SDIO_Send_Data.response_type = SDIO_RESPONSETYPE_SHORT;
			SDIO_Send_Data.wait_type = SDIO_WAITTYPE_NO;
			SDIO_SendCommand(&SDIO_Send_Data); // 内含   sdio_csm_enable();
			/*6-2 check if some error occurs */
			errorstatus = r1_error_check(SD_CMD_APP_CMD);
			if (SD_OK != errorstatus)
			{
				return errorstatus;
			}

			/* 6-3 send ACMD41(SD_SEND_OP_COND) to get host capacity support information (HCS) and OCR content */
			SDIO_Send_Data.index = SD_APPCMD_SD_SEND_OP_COND;
			SDIO_Send_Data.argument = (SD_VOLTAGE_WINDOW | SDType); // SD_STD_CAPACITY;
			SDIO_Send_Data.response_type = SDIO_RESPONSETYPE_SHORT;
			SDIO_Send_Data.wait_type = SDIO_WAITTYPE_NO;
			SDIO_SendCommand(&SDIO_Send_Data); // 内含   sdio_csm_enable();
			/*6-4 check if some error occurs */
			errorstatus = r3_error_check();
			if (SD_OK != errorstatus)
			{
				return errorstatus;
			}

			/*6-5 get the response and check card power up status bit(busy)*/
			response = sdio_response_get(SDIO_RESPONSE0); // SDIO_RESPONSE0  这是一个寄存器，可能是来存着回复回来的数
			busyflag = (u8)((response >> 31) & (u32)0x01);
			++count;
		}

		if (count >= SD_MAX_VOLT_VALIDATION)
		{
			errorstatus = SD_VOLTRANGE_INVALID;
			return errorstatus;
		}

		if (response &= SD_HIGH_CAPACITY)
		{
			cardtype = SDIO_HIGH_CAPACITY_SD_CARD;
		}
	}
	return errorstatus;
}

/*********************************************************************
 * @fn       : <SD_Card_init/函数名>
 * @brief    : <给sd卡进行初始化，>
 * @note     : <上面的函数是sd卡上电检测，这是初始化的代码>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
sd_error_enum SD_Card_init()
{
	sd_error_enum errorstatus = SD_OK;
	u16 temp_rca = 0x01;
	SDIO_SMD_Send_struct SDIO_Send_Data;
	if (SDIO_POWER_OFF == sdio_power_state_get()) //"sdio_power_state_get"是库中的标准函数，获取sdio的电源装态？？
	{
		errorstatus = SD_OPERATION_IMPROPER;
		return errorstatus;
	}

	/* the card is not I/O only card */
	if (SDIO_SECURE_DIGITAL_IO_COMBO_CARD != cardtype)
	{
		/* send CMD2(SD_CMD_ALL_SEND_CID) to get the CID numbers */
		SDIO_Send_Data.index = SD_CMD_ALL_SEND_CID;
		SDIO_Send_Data.argument = (uint32_t)0x0;
		SDIO_Send_Data.response_type = SDIO_RESPONSETYPE_LONG;
		SDIO_Send_Data.wait_type = SDIO_WAITTYPE_NO;
		SDIO_SendCommand(&SDIO_Send_Data); // 内含   sdio_csm_enable();
		/* check if some error occurs */
		errorstatus = r2_error_check(); // 接收cmd2的返回值
		// if (errorstatus != SD_OK)
		// {
		// 	return errorstatus;
		// }

		/* 存储 CID 号码 store the CID numbers */
		sd_cid[0] = sdio_response_get(SDIO_RESPONSE0);
		sd_cid[1] = sdio_response_get(SDIO_RESPONSE1);
		sd_cid[2] = sdio_response_get(SDIO_RESPONSE2);
		sd_cid[3] = sdio_response_get(SDIO_RESPONSE3);
	}
	/* the card is SD memory card or the I/O card has the memory portion */
	if ((cardtype == SDIO_STD_CAPACITY_SD_CARD_V1_1) || (cardtype == SDIO_STD_CAPACITY_SD_CARD_V2_0) ||
		(cardtype == SDIO_HIGH_CAPACITY_SD_CARD) || (cardtype == SDIO_SECURE_DIGITAL_IO_CARD))
	{
		/* send CMD3(SEND_RELATIVE_ADDR) to ask the card to publish a new relative address (RCA) */
		SDIO_Send_Data.index = SD_CMD_SEND_RELATIVE_ADDR;
		SDIO_Send_Data.argument = (uint32_t)0x0;
		SDIO_Send_Data.response_type = SDIO_RESPONSETYPE_SHORT;
		SDIO_Send_Data.wait_type = SDIO_WAITTYPE_NO;
		SDIO_SendCommand(&SDIO_Send_Data); // 内含   sdio_csm_enable();
		/* check if some error occurs */
		errorstatus = r6_error_check(SD_CMD_SEND_RELATIVE_ADDR, &temp_rca);
		if (errorstatus != SD_OK)
		{
			return errorstatus;
		}
	}

	if(SDIO_SECURE_DIGITAL_IO_CARD!=cardtype)
	{
		/* the card is not I/O only card */
		sd_rca=temp_rca;

		/* send CMD9(SEND_CSD) to get the addressed card's card-specific data (CSD) */
		SDIO_Send_Data.index = SD_CMD_SEND_CSD;
		SDIO_Send_Data.argument = (uint32_t)(temp_rca << SD_RCA_SHIFT);
		SDIO_Send_Data.response_type = SDIO_RESPONSETYPE_LONG;
		SDIO_Send_Data.wait_type = SDIO_WAITTYPE_NO;
		SDIO_SendCommand(&SDIO_Send_Data); // 内含   sdio_csm_enable();
		/* check if some error occurs */
		errorstatus=r2_error_check();
		if(errorstatus!=SD_OK)
		{
			return errorstatus;
		}

		/* store the card-specific data (CSD) */
		sd_csd[0]=sdio_response_get(SDIO_RESPONSE0);
		sd_csd[1]=sdio_response_get(SDIO_RESPONSE1);
		sd_csd[2]=sdio_response_get(SDIO_RESPONSE2);
		sd_csd[3]=sdio_response_get(SDIO_RESPONSE3);
	}
	return errorstatus;
}

/*********************************************************************
 * @fn       : </函数名>
 * @brief    : <SD卡初始化函数>
 * @note     : <对标标准参考库中的函数“sd_init”>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     :
 ********************************************************************/
sd_error_enum SD_Init(void)    
{
	sd_error_enum errorstatus = SD_OK;

	/*1-GPIO init 已在别处完成*/

	/*2-SDIO初始化*/
	MY_SDIO_Init();

	/*3-SD卡识别*/   /* configure the clock and work voltage */
	errorstatus = SD_Power_on();
	if(errorstatus!=SD_OK)
	{
		return errorstatus;
	}

	/*4- initialize the card and get CID and CSD of the card */
	errorstatus=SD_Card_init();
	if(SD_OK!=errorstatus)
	{
		return errorstatus;
	}

	/* 配置SDIO外设 configure the SDIO peripheral */    //这里具体是什么意思啊
	sdio_clock_config(SDIO_SDIOCLKEDGE_RISING,SDIO_CLOCKBYPASS_DISABLE,SDIO_CLOCKPWRSAVE_DISABLE,SD_CLK_DIV_TRANS);
	sdio_bus_mode_set(SDIO_BUSMODE_1BIT);
	sdio_hardware_clock_disable();

	return errorstatus;
}


/*********************************************************************
 * @fn       : <function_name/函数名>
 * @brief    : <brief>
 * @note     : <get the detailed information of the SD card based on received CID and CSD>
 * @param    : <param>
 * @return   : <a pointer that store the detailed card information>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
sd_error_enum sd_card_information_get(sd_card_info_struct *pcardinfo)
{
	sd_error_enum status = SD_OK;
    uint8_t tempbyte = 0;
    
    if(NULL == pcardinfo){
        status = SD_PARAMETER_INVALID;
        return status;
    }
    
    /* store the card type and RCA */
    pcardinfo->card_type = cardtype;
    pcardinfo->card_rca = sd_rca;
    
    /* CID byte 0 */
    tempbyte = (uint8_t)((sd_cid[0] & SD_MASK_24_31BITS) >> 24);
    pcardinfo->card_cid.mid = tempbyte;
    
    /* CID byte 1 */
    tempbyte = (uint8_t)((sd_cid[0] & SD_MASK_16_23BITS) >> 16);
    pcardinfo->card_cid.oid = (uint16_t)((uint16_t)tempbyte << 8);
    
    /* CID byte 2 */
    tempbyte = (uint8_t)((sd_cid[0] & SD_MASK_8_15BITS) >> 8);
    pcardinfo->card_cid.oid |= (uint16_t)tempbyte;
    
    /* CID byte 3 */
    tempbyte = (uint8_t)(sd_cid[0] & SD_MASK_0_7BITS);
    pcardinfo->card_cid.pnm0 = (uint32_t)((uint32_t)tempbyte << 24);
    
    /* CID byte 4 */
    tempbyte = (uint8_t)((sd_cid[1] & SD_MASK_24_31BITS) >> 24);
    pcardinfo->card_cid.pnm0 |= (uint32_t)((uint32_t)tempbyte << 16);
    
    /* CID byte 5 */
    tempbyte = (uint8_t)((sd_cid[1] & SD_MASK_16_23BITS) >> 16);
    pcardinfo->card_cid.pnm0 |= (uint32_t)((uint32_t)tempbyte << 8);
    
    /* CID byte 6 */
    tempbyte = (uint8_t)((sd_cid[1] & SD_MASK_8_15BITS) >> 8);
    pcardinfo->card_cid.pnm0 |= (uint32_t)(tempbyte);
    
    /* CID byte 7 */
    tempbyte = (uint8_t)(sd_cid[1] & SD_MASK_0_7BITS);
    pcardinfo->card_cid.pnm1 = tempbyte;
    
    /* CID byte 8 */
    tempbyte = (uint8_t)((sd_cid[2] & SD_MASK_24_31BITS) >> 24);
    pcardinfo->card_cid.prv = tempbyte;
    
    /* CID byte 9 */
    tempbyte = (uint8_t)((sd_cid[2] & SD_MASK_16_23BITS) >> 16);
    pcardinfo->card_cid.psn = (uint32_t)((uint32_t)tempbyte << 24);
    
    /* CID byte 10 */
    tempbyte = (uint8_t)((sd_cid[2] & SD_MASK_8_15BITS) >> 8);
    pcardinfo->card_cid.psn |= (uint32_t)((uint32_t)tempbyte << 16);
    
    /* CID byte 11 */
    tempbyte = (uint8_t)(sd_cid[2] & SD_MASK_0_7BITS);
    pcardinfo->card_cid.psn |= (uint32_t)tempbyte;
    
    /* CID byte 12 */
    tempbyte = (uint8_t)((sd_cid[3] & SD_MASK_24_31BITS) >> 24);
    pcardinfo->card_cid.psn |= (uint32_t)tempbyte;
    
    /* CID byte 13 */
    tempbyte = (uint8_t)((sd_cid[3] & SD_MASK_16_23BITS) >> 16);
    pcardinfo->card_cid.mdt = (uint16_t)((uint16_t)(tempbyte & 0x0F) << 8);
    
    /* CID byte 14 */
    tempbyte = (uint8_t)((sd_cid[3] & SD_MASK_8_15BITS) >> 8);
    pcardinfo->card_cid.mdt |= (uint16_t)tempbyte;
    
    /* CID byte 15 */
    tempbyte = (uint8_t)(sd_cid[3] & SD_MASK_0_7BITS);
    pcardinfo->card_cid.cid_crc = (tempbyte & 0xFE) >> 1;
    
    /* CSD byte 0 */
    tempbyte = (uint8_t)((sd_csd[0] & SD_MASK_24_31BITS) >> 24);
    pcardinfo->card_csd.csd_struct = (tempbyte & 0xC0) >> 6;
    
    /* CSD byte 1 */
    tempbyte = (uint8_t)((sd_csd[0] & SD_MASK_16_23BITS) >> 16);
    pcardinfo->card_csd.taac = tempbyte;
    
    /* CSD byte 2 */
    tempbyte = (uint8_t)((sd_csd[0] & SD_MASK_8_15BITS) >> 8);
    pcardinfo->card_csd.nsac = tempbyte;
    
    /* CSD byte 3 */
    tempbyte = (uint8_t)(sd_csd[0] & SD_MASK_0_7BITS);
    pcardinfo->card_csd.tran_speed = tempbyte;
    
    /* CSD byte 4 */
    tempbyte = (uint8_t)((sd_csd[1] & SD_MASK_24_31BITS) >> 24);
    pcardinfo->card_csd.ccc = (uint16_t)((uint16_t)tempbyte << 4);
    
    /* CSD byte 5 */
    tempbyte = (uint8_t)((sd_csd[1] & SD_MASK_16_23BITS) >> 16);
    pcardinfo->card_csd.ccc |= (uint16_t)((uint16_t)(tempbyte & 0xF0) >> 4);
    pcardinfo->card_csd.read_bl_len = tempbyte & 0x0F;
    
    /* CSD byte 6 */
    tempbyte = (uint8_t)((sd_csd[1] & SD_MASK_8_15BITS) >> 8);
    pcardinfo->card_csd.read_bl_partial = (tempbyte & 0x80) >> 7;
    pcardinfo->card_csd.write_blk_misalign = (tempbyte & 0x40) >> 6;
    pcardinfo->card_csd.read_blk_misalign = (tempbyte & 0x20) >> 5;
    pcardinfo->card_csd.dsp_imp = (tempbyte & 0x10) >> 4;
    
    if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == cardtype) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == cardtype)){
        /* card is SDSC card, CSD version 1.0 */
        pcardinfo->card_csd.c_size = (uint32_t)((uint32_t)(tempbyte & 0x03) << 10);
        
        /* CSD byte 7 */
        tempbyte = (uint8_t)(sd_csd[1] & SD_MASK_0_7BITS);
        pcardinfo->card_csd.c_size |= (uint32_t)((uint32_t)tempbyte << 2);
        
        /* CSD byte 8 */
        tempbyte = (uint8_t)((sd_csd[2] & SD_MASK_24_31BITS) >> 24);
        pcardinfo->card_csd.c_size |= (uint32_t)((uint32_t)(tempbyte & 0xC0) >> 6);
        pcardinfo->card_csd.vdd_r_curr_min = (tempbyte & 0x38) >> 3;
        pcardinfo->card_csd.vdd_r_curr_max = tempbyte & 0x07;
        
        /* CSD byte 9 */
        tempbyte = (uint8_t)((sd_csd[2] & SD_MASK_16_23BITS) >> 16);
        pcardinfo->card_csd.vdd_w_curr_min = (tempbyte & 0xE0) >> 5;
        pcardinfo->card_csd.vdd_w_curr_max = (tempbyte & 0x1C) >> 2;
        pcardinfo->card_csd.c_size_mult = (tempbyte & 0x03) << 1;
        
        /* CSD byte 10 */
        tempbyte = (uint8_t)((sd_csd[2] & SD_MASK_8_15BITS) >> 8);
        pcardinfo->card_csd.c_size_mult |= (tempbyte & 0x80) >> 7;
        
        /* calculate the card block size and capacity */
        pcardinfo->card_blocksize = 1 << (pcardinfo->card_csd.read_bl_len);
        pcardinfo->card_capacity = pcardinfo->card_csd.c_size + 1;
        pcardinfo->card_capacity *= (1 << (pcardinfo->card_csd.c_size_mult + 2));
        pcardinfo->card_capacity *= pcardinfo->card_blocksize;
    }else if(SDIO_HIGH_CAPACITY_SD_CARD == cardtype){
        /* card is SDHC card, CSD version 2.0 */
        /* CSD byte 7 */
        tempbyte = (uint8_t)(sd_csd[1] & SD_MASK_0_7BITS);
        pcardinfo->card_csd.c_size = (uint32_t)((uint32_t)(tempbyte & 0x3F) << 16);
        
        /* CSD byte 8 */
        tempbyte = (uint8_t)((sd_csd[2] & SD_MASK_24_31BITS) >> 24);
        pcardinfo->card_csd.c_size |= (uint32_t)((uint32_t)tempbyte << 8);
        
        /* CSD byte 9 */
        tempbyte = (uint8_t)((sd_csd[2] & SD_MASK_16_23BITS) >> 16);
        pcardinfo->card_csd.c_size |= (uint32_t)tempbyte;
        
        /* calculate the card block size and capacity */
        pcardinfo->card_blocksize = 512;
        pcardinfo->card_capacity = (pcardinfo->card_csd.c_size + 1) * 512 *1024;
    }
    
    pcardinfo->card_csd.erase_blk_en = (tempbyte & 0x40) >> 6;
    pcardinfo->card_csd.sector_size = (tempbyte & 0x3F) << 1;
    
    /* CSD byte 11 */
    tempbyte = (uint8_t)(sd_csd[2] & SD_MASK_0_7BITS);
    pcardinfo->card_csd.sector_size |= (tempbyte & 0x80) >> 7;
    pcardinfo->card_csd.wp_grp_size = (tempbyte & 0x7F);
    
    /* CSD byte 12 */
    tempbyte = (uint8_t)((sd_csd[3] & SD_MASK_24_31BITS) >> 24);
    pcardinfo->card_csd.wp_grp_enable = (tempbyte & 0x80) >> 7;
    pcardinfo->card_csd.r2w_factor = (tempbyte & 0x1C) >> 2;
    pcardinfo->card_csd.write_bl_len = (tempbyte & 0x03) << 2;
    
    /* CSD byte 13 */
    tempbyte = (uint8_t)((sd_csd[3] & SD_MASK_16_23BITS) >> 16);
    pcardinfo->card_csd.write_bl_len |= (tempbyte & 0xC0) >> 6;
    pcardinfo->card_csd.write_bl_partial = (tempbyte & 0x20) >> 5;
    
    /* CSD byte 14 */
    tempbyte = (uint8_t)((sd_csd[3] & SD_MASK_8_15BITS) >> 8);
    pcardinfo->card_csd.file_format_grp = (tempbyte & 0x80) >> 7;
    pcardinfo->card_csd.copy_flag = (tempbyte & 0x40) >> 6;
    pcardinfo->card_csd.perm_write_protect = (tempbyte & 0x20) >> 5;
    pcardinfo->card_csd.tmp_write_protect = (tempbyte & 0x10) >> 4;
    pcardinfo->card_csd.file_format = (tempbyte & 0x0C) >> 2;
    
    /* CSD byte 15 */
    tempbyte = (uint8_t)(sd_csd[3] & SD_MASK_0_7BITS);
    pcardinfo->card_csd.csd_crc = (tempbyte & 0xFE) >> 1;
    
    return status;
}


/*********************************************************************
 * @fn       : <function_name/函数名>
 * @brief    : <select or deselect a card>
 * @note     : <note>
 * @param    : <cardrca: the RCA of a card>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
sd_error_enum sd_card_select_deselect(uint16_t cardrca)
{
    sd_error_enum errorstatus = SD_OK;
    /* send CMD7(SELECT/DESELECT_CARD) to select or deselect the card */
    sdio_command_response_config(SD_CMD_SELECT_DESELECT_CARD, (uint32_t)(cardrca << SD_RCA_SHIFT), SDIO_RESPONSETYPE_SHORT);
    sdio_wait_type_set(SDIO_WAITTYPE_NO);
    sdio_csm_enable();
    
    errorstatus = r1_error_check(SD_CMD_SELECT_DESELECT_CARD);
    return errorstatus;
}


/*********************************************************************
 * @fn       : <function_name/函数名>
 * @brief    : <brief>
 * @note     : <get the card status whose response format R1 contains a 32-bit field>
 * @param    : <param>
 * @return   : <a pointer that store card status>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
sd_error_enum sd_cardstatus_get(uint32_t *pcardstatus)
{
    sd_error_enum errorstatus = SD_OK;
    if(NULL == pcardstatus){
        errorstatus = SD_PARAMETER_INVALID;
        return errorstatus;
    }
    
    /* send CMD13(SEND_STATUS), addressed card sends its status register */
    sdio_command_response_config(SD_CMD_SEND_STATUS, (uint32_t)sd_rca << SD_RCA_SHIFT, SDIO_RESPONSETYPE_SHORT);
    sdio_wait_type_set(SDIO_WAITTYPE_NO);
    sdio_csm_enable();
    /* check if some error occurs */
    errorstatus = r1_error_check(SD_CMD_SEND_STATUS);
    if(SD_OK != errorstatus){
        return errorstatus;
    }
    
    *pcardstatus = sdio_response_get(SDIO_RESPONSE0);
    return errorstatus;
}


/*********************************************************************
 * @fn       : <function_name/函数名>
 * @brief    : <configure the bus mode>
 * @note     : <note>
 * @param    : <the bus mode
 * 				SDIO_BUSMODE_1BIT: 1-bit SDIO card bus mode
 * 				SDIO_BUSMODE_4BIT: 4-bit SDIO card bus mode
 * 				SDIO_BUSMODE_8BIT: 8-bit SDIO card bus mode (MMC only)>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
sd_error_enum sd_bus_mode_config(uint32_t busmode)
{
	sd_error_enum status = SD_OK;
	if (SDIO_MULTIMEDIA_CARD == cardtype)
	{
		/* MMC card doesn't support this function */
		status = SD_FUNCTION_UNSUPPORTED;
		return status;
	}
	else if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == cardtype) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == cardtype) ||
			 (SDIO_HIGH_CAPACITY_SD_CARD == cardtype))
	{
		if (SDIO_BUSMODE_8BIT == busmode)
		{
			/* 8 bit bus mode doesn't support */
			status = SD_FUNCTION_UNSUPPORTED;
			return status;
		}
		else if (SDIO_BUSMODE_4BIT == busmode)
		{
			/* configure SD bus width and the SDIO */
			status = sd_bus_width_config(SD_BUS_WIDTH_4BIT);
			if (SD_OK == status)
			{
				sdio_clock_config(SDIO_SDIOCLKEDGE_RISING, SDIO_CLOCKBYPASS_DISABLE,
								  SDIO_CLOCKPWRSAVE_DISABLE, SD_CLK_DIV_TRANS);
				sdio_bus_mode_set(busmode);
				sdio_hardware_clock_disable();
			}
		}
		else if (SDIO_BUSMODE_1BIT == busmode)
		{
			/* configure SD bus width and the SDIO */
			status = sd_bus_width_config(SD_BUS_WIDTH_1BIT);
			if (SD_OK == status)
			{
				sdio_clock_config(SDIO_SDIOCLKEDGE_RISING, SDIO_CLOCKBYPASS_DISABLE,
								  SDIO_CLOCKPWRSAVE_DISABLE, SD_CLK_DIV_TRANS);
				sdio_bus_mode_set(busmode);
				sdio_hardware_clock_disable();
			}
		}
		else
		{
			status = SD_PARAMETER_INVALID;
		}
	}
	return status;
}

/*********************************************************************
 * @fn       : <function_name/函数名>
 * @brief    : <brief>
 * @note     : <initialize the card, get the card information, set the bus mode and transfer mode
 * 				初始化卡，获取卡的信息，设置总线模式和传输模式>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
sd_error_enum sd_io_init(void)
{
	sd_error_enum errorstatus =SD_OK;
	u32 cardstate=0;
	errorstatus=SD_Init();
	if(SD_OK == errorstatus)
	{
		errorstatus = sd_card_information_get(&sd_cardinfo);
	}
	if(SD_OK==errorstatus)
	{
		errorstatus=sd_card_select_deselect(sd_cardinfo.card_rca);
	}
	errorstatus = sd_cardstatus_get(&cardstate);

	if(cardstate & 0x02000000)
	{
		//the card is locked !
	}

	if((SD_OK==errorstatus)&&(!(cardstate&0x02000000)))
	{
		/*set bus mode */
		errorstatus = sd_bus_mode_config(SDIO_BUSMODE_4BIT);
	}

	if(SD_OK==errorstatus)
	{
		/* set data transfer mode */
		errorstatus =sd_transfer_mode_config(SD_POLLING_MODE);
	}
	return errorstatus;
}




/*********************************************************************
 * @fn       : <card_info_get/函数名>
 * @brief    : <brief>
 * @note     : <get the card information and print it out by USRAT>
 * @param    : <param>
 * @return   : <return>
 * @author   : <editor>
 * @date     : 
 ********************************************************************/
void card_info_get(void)
{
	u8 sd_spec, sd_spec3, sd_spec4, sd_security;
	u32 block_count, block_size;
	u16 temp_ccc;
	// printf("\r\n Card information");
	Print_my("\r\n Card information",2);

	sd_spec = (sd_scr[1] & 0x0F000000) >> 24;
	sd_spec3 = (sd_scr[1] & 0x00008000) >> 15;
	sd_spec4 = (sd_scr[1] & 0x00000400) >> 10;
	if (2 == sd_spec)
	{
		if (1 == sd_spec3)
		{
			if (1 == sd_spec4)
			{
				Print_my("\r\n## Card version 4.xx ##",3);
			}
			else
			{
				Print_my("\r\n## Card version 3.0x ##",3);
			}
		}
		else
		{
			Print_my("\r\n## Card version 2.00 ##",3);
		}
	}
	else if (1 == sd_spec)
	{
		Print_my("\r\n## Card version 1.10 ##",3);
	}
	else if (0 == sd_spec)
	{
		Print_my("\r\n## Card version 1.0x ##",3);
	}

	sd_security = (sd_scr[1] & 0x00700000) >> 20;
	if (2 == sd_security)
	{
		Print_my("\r\n## SDSC card ##",4);
	}
	else if (3 == sd_security)
	{
		Print_my("\r\n## SDHC card ##",4);
	}
	else if (4 == sd_security)
	{
		Print_my("\r\n## SDXC card ##",4);
	}

	block_count = (sd_cardinfo.card_csd.c_size + 1) * 1024;
	block_size = 512;

	printf("\r\n## Device size is %dKB ##", sd_card_capacity_get());
	printf("\r\n## Block size is %dB ##", block_size);
	printf("\r\n## Block count is %d ##", block_count);

	if (sd_cardinfo.card_csd.read_bl_partial)
	{
		printf("\r\n## Partial blocks for read allowed ##");
	}
	if (sd_cardinfo.card_csd.write_bl_partial)
	{
		printf("\r\n## Partial blocks for write allowed ##");
	}
	temp_ccc = sd_cardinfo.card_csd.ccc;
	printf("\r\n## CardCommandClasses is: %x ##", temp_ccc);
	if ((SD_CCC_BLOCK_READ & temp_ccc) && (SD_CCC_BLOCK_WRITE & temp_ccc))
	{
		printf("\r\n## Block operation supported ##");
	}
	if (SD_CCC_ERASE & temp_ccc)
	{
		printf("\r\n## Erase supported ##");
	}
	if (SD_CCC_WRITE_PROTECTION & temp_ccc)
	{
		printf("\r\n## Write protection supported ##");
	}
	if (SD_CCC_LOCK_CARD & temp_ccc)
	{
		printf("\r\n## Lock unlock supported ##");
	}
	if (SD_CCC_APPLICATION_SPECIFIC & temp_ccc)
	{
		printf("\r\n## Application specific supported ##");
	}
	if (SD_CCC_IO_MODE & temp_ccc)
	{
		printf("\r\n## I/O mode supported ##");
	}
	if (SD_CCC_SWITCH & temp_ccc)
	{
		printf("\r\n## Switch function supported ##");
	}
}



/*!
    \brief      configure the bus width mode
    \param[in]  buswidth: the bus width
      \arg        SD_BUS_WIDTH_1BIT: 1-bit bus width
      \arg        SD_BUS_WIDTH_4BIT: 4-bit bus width
    \param[out] none
    \retval     sd_error_enum
*/
static sd_error_enum sd_bus_width_config(uint32_t buswidth)
{
    sd_error_enum status = SD_OK;
    /* check whether the card is locked */
    if(sdio_response_get(SDIO_RESPONSE0) & SD_CARDSTATE_LOCKED){
        status = SD_LOCK_UNLOCK_FAILED;
        return status;
    }
    /* get the SCR register */
    status = sd_scr_get(sd_rca, sd_scr);
    if(SD_OK != status){
        return status;
    }
    
    if(SD_BUS_WIDTH_1BIT == buswidth){
        if(SD_ALLZERO != (sd_scr[1] & buswidth)){
            /* send CMD55(APP_CMD) to indicate next command is application specific command */
            sdio_command_response_config(SD_CMD_APP_CMD, (uint32_t)sd_rca << SD_RCA_SHIFT, SDIO_RESPONSETYPE_SHORT);
            sdio_wait_type_set(SDIO_WAITTYPE_NO);
            sdio_csm_enable();
            /* check if some error occurs */
            status = r1_error_check(SD_CMD_APP_CMD);
            if(SD_OK != status){
                return status;
            }
            
            /* send ACMD6(SET_BUS_WIDTH) to define the data bus width */
            sdio_command_response_config(SD_APPCMD_SET_BUS_WIDTH, (uint32_t)0x0, SDIO_RESPONSETYPE_SHORT);
            sdio_wait_type_set(SDIO_WAITTYPE_NO);
            sdio_csm_enable();
            /* check if some error occurs */
            status = r1_error_check(SD_APPCMD_SET_BUS_WIDTH);
            if(SD_OK != status){
                return status;
            }
        }else{
            status = SD_OPERATION_IMPROPER;
        }
        return status;
    }else if(SD_BUS_WIDTH_4BIT == buswidth){
        if(SD_ALLZERO != (sd_scr[1] & buswidth)){
            /* send CMD55(APP_CMD) to indicate next command is application specific command */
            sdio_command_response_config(SD_CMD_APP_CMD, (uint32_t)sd_rca << SD_RCA_SHIFT, SDIO_RESPONSETYPE_SHORT);
            sdio_wait_type_set(SDIO_WAITTYPE_NO);
            sdio_csm_enable();
            /* check if some error occurs */
            status = r1_error_check(SD_CMD_APP_CMD);
            if(SD_OK != status){
                return status;
            }
            
            /* send ACMD6(SET_BUS_WIDTH) to define the data bus width */
            sdio_command_response_config(SD_APPCMD_SET_BUS_WIDTH, (uint32_t)0x2, SDIO_RESPONSETYPE_SHORT);
            sdio_wait_type_set(SDIO_WAITTYPE_NO);
            sdio_csm_enable();
            /* check if some error occurs */
            status = r1_error_check(SD_APPCMD_SET_BUS_WIDTH);
            if(SD_OK != status){
                return status;
            }
        }else{
            status = SD_OPERATION_IMPROPER;
        }
        return status;
    }else{
        status = SD_PARAMETER_INVALID;
        return status;
    }
}


/*!
    \brief      get the SCR of corresponding card
    \param[in]  rca: RCA of a card
    \param[out] pscr: a pointer that store the SCR content
    \retval     sd_error_enum
*/
static sd_error_enum sd_scr_get(uint16_t rca, uint32_t *pscr)
{
    sd_error_enum status = SD_OK;
    uint32_t temp_scr[2] = {0, 0}, idx_scr = 0;
    /* send CMD16(SET_BLOCKLEN) to set block length */
    sdio_command_response_config(SD_CMD_SET_BLOCKLEN, (uint32_t)8, SDIO_RESPONSETYPE_SHORT);
    sdio_wait_type_set(SDIO_WAITTYPE_NO);
    /* check if some error occurs */
    status = r1_error_check(SD_CMD_SET_BLOCKLEN);
    if(SD_OK != status){
        return status;
    }
    
    /* send CMD55(APP_CMD) to indicate next command is application specific command */
    sdio_command_response_config(SD_CMD_APP_CMD, (uint32_t)rca << SD_RCA_SHIFT, SDIO_RESPONSETYPE_SHORT);
    sdio_wait_type_set(SDIO_WAITTYPE_NO);
    sdio_csm_enable();
    /* check if some error occurs */
    status = r1_error_check(SD_CMD_APP_CMD);
    if(SD_OK != status){
        return status;
    }
    
    /* configure SDIO data */
    sdio_data_config(SD_DATATIMEOUT, (uint32_t)8, SDIO_DATABLOCKSIZE_8BYTES);
    sdio_data_transfer_config(SDIO_TRANSMODE_BLOCK, SDIO_TRANSDIRECTION_TOSDIO);
    sdio_dsm_enable();
    
    /* send ACMD51(SEND_SCR) to read the SD configuration register */
    sdio_command_response_config(SD_APPCMD_SEND_SCR, (uint32_t)0x0, SDIO_RESPONSETYPE_SHORT);
    sdio_wait_type_set(SDIO_WAITTYPE_NO);
    sdio_csm_enable();
    /* check if some error occurs */
    status = r1_error_check(SD_APPCMD_SEND_SCR);
    if(SD_OK != status){
        return status;
    }
    
    /* store the received SCR */
    while(!sdio_flag_get(SDIO_FLAG_DTCRCERR | SDIO_FLAG_DTTMOUT | SDIO_FLAG_RXORE | SDIO_FLAG_DTBLKEND | SDIO_FLAG_STBITE)){
        if(RESET != sdio_flag_get(SDIO_FLAG_RXDTVAL)){
            *(temp_scr + idx_scr) = sdio_data_read();
            ++idx_scr;
        }
    }
    
    /* check whether some error occurs */
    if(RESET != sdio_flag_get(SDIO_FLAG_DTCRCERR)){
        status = SD_DATA_CRC_ERROR;
        sdio_flag_clear(SDIO_FLAG_DTCRCERR);
        return status;
    }else if(RESET != sdio_flag_get(SDIO_FLAG_DTTMOUT)){
        status = SD_DATA_TIMEOUT;
        sdio_flag_clear(SDIO_FLAG_DTTMOUT);
        return status;
    }else if(RESET != sdio_flag_get(SDIO_FLAG_RXORE)){
        status = SD_RX_OVERRUN_ERROR;
        sdio_flag_clear(SDIO_FLAG_RXORE);
        return status;
    }else if(RESET != sdio_flag_get(SDIO_FLAG_STBITE)){
        status = SD_START_BIT_ERROR;
        sdio_flag_clear(SDIO_FLAG_STBITE);
        return status;
    }
    
    /* clear all the SDIO_INTC flags */
    sdio_flag_clear(SDIO_MASK_INTC_FLAGS);
    /* readjust the temp SCR value */
    *(pscr) = ((temp_scr[1] & SD_MASK_0_7BITS) << 24) | ((temp_scr[1] & SD_MASK_8_15BITS) << 8) | 
                ((temp_scr[1] & SD_MASK_16_23BITS) >> 8) | ((temp_scr[1] & SD_MASK_24_31BITS) >> 24);
    *(pscr + 1) = ((temp_scr[0] & SD_MASK_0_7BITS) << 24) | ((temp_scr[0] & SD_MASK_8_15BITS) << 8) | 
                ((temp_scr[0] & SD_MASK_16_23BITS) >> 8) | ((temp_scr[0] & SD_MASK_24_31BITS) >> 24);
    return status;
}


/*!
    \brief      configure the mode of transmission
    \param[in]  txmode: transfer mode
      \arg        SD_DMA_MODE: DMA mode
      \arg        SD_POLLING_MODE: polling mode
    \param[out] none
    \retval     sd_error_enum
*/
sd_error_enum sd_transfer_mode_config(uint32_t txmode)
{
    sd_error_enum status = SD_OK;
    /* set the transfer mode */
    if((SD_DMA_MODE == txmode) || (SD_POLLING_MODE == txmode)){
        transmode = txmode;
    }else{
        status = SD_PARAMETER_INVALID;
    }
    return status;
}





/*!
    \brief      get SD card capacity
    \param[in]  none
    \param[out] none
    \retval     capacity of the card(KB)
*/
uint32_t sd_card_capacity_get(void)
{
    uint8_t tempbyte = 0, devicesize_mult = 0, readblklen = 0;
    uint32_t capacity = 0, devicesize = 0;
    if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == cardtype) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == cardtype)){
        /* calculate the c_size(device size) */
        tempbyte = (uint8_t)((sd_csd[1] & SD_MASK_8_15BITS) >> 8);
        devicesize = (uint32_t)((uint32_t)(tempbyte & 0x03) << 10);
        tempbyte = (uint8_t)(sd_csd[1] & SD_MASK_0_7BITS);
        devicesize |= (uint32_t)((uint32_t)tempbyte << 2);
        tempbyte = (uint8_t)((sd_csd[2] & SD_MASK_24_31BITS) >> 24);
        devicesize |= (uint32_t)((uint32_t)(tempbyte & 0xC0) >> 6);
        
        /* calculate the c_size_mult(device size multiplier) */
        tempbyte = (uint8_t)((sd_csd[2] & SD_MASK_16_23BITS) >> 16);
        devicesize_mult = (tempbyte & 0x03) << 1;
        tempbyte = (uint8_t)((sd_csd[2] & SD_MASK_8_15BITS) >> 8);
        devicesize_mult |= (tempbyte & 0x80) >> 7;

        /* calculate the read_bl_len */
        tempbyte = (uint8_t)((sd_csd[1] & SD_MASK_16_23BITS) >> 16);
        readblklen = tempbyte & 0x0F;
        
        /* capacity = BLOCKNR*BLOCK_LEN, BLOCKNR = (C_SIZE+1)*MULT, MULT = 2^(C_SIZE_MULT+2), BLOCK_LEN = 2^READ_BL_LEN */
        capacity = (devicesize + 1)*(1 << (devicesize_mult + 2));
        capacity *= (1 << readblklen);
        
        /* change the unit of capacity to KByte */
        capacity /= 1024;
    }else if(SDIO_HIGH_CAPACITY_SD_CARD == cardtype){
        /* calculate the c_size */
        tempbyte = (uint8_t)(sd_csd[1] & SD_MASK_0_7BITS);
        devicesize = (uint32_t)((uint32_t)(tempbyte & 0x3F) << 16);
        tempbyte = (uint8_t)((sd_csd[2] & SD_MASK_24_31BITS) >> 24);
        devicesize |= (uint32_t)((uint32_t)tempbyte << 8);
        tempbyte = (uint8_t)((sd_csd[2] & SD_MASK_16_23BITS) >> 16);
        devicesize |= (uint32_t)tempbyte;
        
        /* capacity = (c_size+1)*512KByte */
        capacity = (devicesize + 1)*512;
    }
    return capacity;
}







// /*!
//     \brief      write a block data to the specified address of a card
//     \param[in]  pwritebuffer: a pointer that store a block data to be transferred
//     \param[in]  writeaddr: the read data address
//     \param[in]  blocksize: the data block size
//     \param[out] none
//     \retval     sd_error_enum
// */
// sd_error_enum sd_block_write(uint32_t *pwritebuffer, uint32_t writeaddr, uint16_t blocksize)
// {
//     /* initialize the variables */
//     sd_error_enum status = SD_OK;
//     uint8_t cardstate = 0;
//     uint32_t count = 0, align = 0, datablksize = SDIO_DATABLOCKSIZE_1BYTE, *ptempbuff = pwritebuffer;
//     uint32_t transbytes = 0, restwords = 0, response = 0;
//     __IO uint32_t timeout = 0;

//     if(NULL == pwritebuffer){
//         status = SD_PARAMETER_INVALID;
//         return status;
//     }
    
//     transerror = SD_OK;
//     transend = 0;
//     totalnumber_bytes = 0;
//     /* clear all DSM configuration */
//     sdio_data_config(0, 0, SDIO_DATABLOCKSIZE_1BYTE);
//     sdio_data_transfer_config(SDIO_TRANSMODE_BLOCK, SDIO_TRANSDIRECTION_TOCARD);
//     sdio_dsm_disable();
//     sdio_dma_disable();
    
//     /* check whether the card is locked */
//     if(sdio_response_get(SDIO_RESPONSE0) & SD_CARDSTATE_LOCKED){
//         status = SD_LOCK_UNLOCK_FAILED;
//         return status;
//     }
    
//     /* blocksize is fixed in 512B for SDHC card */
//     if(SDIO_HIGH_CAPACITY_SD_CARD == cardtype){
//         blocksize = 512;
//         writeaddr /= 512;
//     }
    
//     align = blocksize & (blocksize - 1);
//     if((blocksize > 0) && (blocksize <= 2048) && (0 == align)){
//         datablksize = sd_datablocksize_get(blocksize);
//         /* send CMD16(SET_BLOCKLEN) to set the block length */
//         sdio_command_response_config(SD_CMD_SET_BLOCKLEN, (uint32_t)blocksize, SDIO_RESPONSETYPE_SHORT);
//         sdio_wait_type_set(SDIO_WAITTYPE_NO);
//         sdio_csm_enable();
        
//         /* check if some error occurs */
//         status = r1_error_check(SD_CMD_SET_BLOCKLEN);
//         if(SD_OK != status){
//             return status;
//         }
//     }else{
//         status = SD_PARAMETER_INVALID;
//         return status;
//     }
    
//     /* send CMD13(SEND_STATUS), addressed card sends its status registers */
//     sdio_command_response_config(SD_CMD_SEND_STATUS, (uint32_t)sd_rca << SD_RCA_SHIFT, SDIO_RESPONSETYPE_SHORT);
//     sdio_wait_type_set(SDIO_WAITTYPE_NO);
//     sdio_csm_enable();
//     /* check if some error occurs */
//     status = r1_error_check(SD_CMD_SEND_STATUS);
//     if(SD_OK != status){
//         return status;
//     }
    
//     response = sdio_response_get(SDIO_RESPONSE0);
//     timeout = 100000;

//     while((0 == (response & SD_R1_READY_FOR_DATA)) && (timeout > 0)){
//         /* continue to send CMD13 to polling the state of card until buffer empty or timeout */
//         --timeout;
//         /* send CMD13(SEND_STATUS), addressed card sends its status registers */
//         sdio_command_response_config(SD_CMD_SEND_STATUS, (uint32_t)sd_rca << SD_RCA_SHIFT, SDIO_RESPONSETYPE_SHORT);
//         sdio_wait_type_set(SDIO_WAITTYPE_NO);
//         sdio_csm_enable();
//         /* check if some error occurs */
//         status = r1_error_check(SD_CMD_SEND_STATUS);
//         if(SD_OK != status){
//             return status;
//         }
//         response = sdio_response_get(SDIO_RESPONSE0);
//     }
//     if(0 == timeout){
//         return SD_ERROR;
//     }
    
//     /* send CMD24(WRITE_BLOCK) to write a block */
//     sdio_command_response_config(SD_CMD_WRITE_BLOCK, writeaddr, SDIO_RESPONSETYPE_SHORT);
//     sdio_wait_type_set(SDIO_WAITTYPE_NO);
//     sdio_csm_enable();
//     /* check if some error occurs */
//     status = r1_error_check(SD_CMD_WRITE_BLOCK);
//     if(SD_OK != status){
//         return status;
//     }
    
//     stopcondition = 0;
//     totalnumber_bytes = blocksize;
    
//     /* configure the SDIO data transmission */
//     sdio_data_config(SD_DATATIMEOUT, totalnumber_bytes, datablksize);
//     sdio_data_transfer_config(SDIO_TRANSMODE_BLOCK, SDIO_TRANSDIRECTION_TOCARD);
//     sdio_dsm_enable();
    
//     if(SD_POLLING_MODE == transmode){
//         /* polling mode */
//         while(!sdio_flag_get(SDIO_FLAG_DTCRCERR | SDIO_FLAG_DTTMOUT | SDIO_FLAG_TXURE | SDIO_FLAG_DTBLKEND | SDIO_FLAG_STBITE)){
//             if(RESET != sdio_flag_get(SDIO_FLAG_TFH)){
//                 /* at least 8 words can be written into the FIFO */
//                 if((totalnumber_bytes - transbytes) < SD_FIFOHALF_BYTES){
//                     restwords = (totalnumber_bytes - transbytes)/4 + (((totalnumber_bytes - transbytes)%4 == 0) ? 0 : 1);
//                     for(count = 0; count < restwords; count++){
//                         sdio_data_write(*ptempbuff);
//                         ++ptempbuff;
//                         transbytes += 4;
//                     }
//                 }else{
//                     for(count = 0; count < SD_FIFOHALF_WORDS; count++){
//                         sdio_data_write(*(ptempbuff + count));
//                     }
//                     /* 8 words(32 bytes) has been transferred */
//                     ptempbuff += SD_FIFOHALF_WORDS;
//                     transbytes += SD_FIFOHALF_BYTES;
//                 }
//             }
//         }
        
//         /* whether some error occurs and return it */
//         if(RESET != sdio_flag_get(SDIO_FLAG_DTCRCERR)){
//             status = SD_DATA_CRC_ERROR;
//             sdio_flag_clear(SDIO_FLAG_DTCRCERR);
//             return status;
//         }else if(RESET != sdio_flag_get(SDIO_FLAG_DTTMOUT)){
//             status = SD_DATA_TIMEOUT;
//             sdio_flag_clear(SDIO_FLAG_DTTMOUT);
//             return status;
//         }else if(RESET != sdio_flag_get(SDIO_FLAG_TXURE)){
//             status = SD_TX_UNDERRUN_ERROR;
//             sdio_flag_clear(SDIO_FLAG_TXURE);
//             return status;
//         }else if(RESET != sdio_flag_get(SDIO_FLAG_STBITE)){
//             status = SD_START_BIT_ERROR;
//             sdio_flag_clear(SDIO_FLAG_STBITE);
//             return status;
//         }
//     }else if(SD_DMA_MODE == transmode){
//         /* DMA mode */
//         /* enable the SDIO corresponding interrupts and DMA */
//         sdio_interrupt_enable(SDIO_INT_DTCRCERR | SDIO_INT_DTTMOUT | SDIO_INT_TXURE | SDIO_INT_DTEND | SDIO_INT_STBITE);
//         dma_transfer_config(pwritebuffer, blocksize);
//         sdio_dma_enable();
        
//         timeout = 100000;
//         while((RESET == dma_flag_get(DMA1, DMA_CH3, DMA_FLAG_FTF)) && (timeout > 0)){
//             timeout--;
//             if(0 == timeout){
//                 return SD_ERROR;
//             }
//         }
//         while ((0 == transend) && (SD_OK == transerror)){
//         }

//         if(SD_OK != transerror){
//             return transerror;
//         }
//     }else{
//         status = SD_PARAMETER_INVALID;
//         return status;
//     }
    
//     /* clear the SDIO_INTC flags */
//     sdio_flag_clear(SDIO_MASK_INTC_FLAGS);
//     /* get the card state and wait the card is out of programming and receiving state */
//     status = sd_card_state_get(&cardstate);
//     while((SD_OK == status) && ((SD_CARDSTATE_PROGRAMMING == cardstate) || (SD_CARDSTATE_RECEIVING == cardstate))){
//         status = sd_card_state_get(&cardstate);
//     }
//     return status;
// }


// /*!
//     \brief      read a block data into a buffer from the specified address of a card
//     \param[out] preadbuffer: a pointer that store a block read data
//     \param[in]  readaddr: the read data address
//     \param[in]  blocksize: the data block size
//     \retval     sd_error_enum
// */
// sd_error_enum sd_block_read(uint32_t *preadbuffer, uint32_t readaddr, uint16_t blocksize)
// {
//     /* initialize the variables */
//     sd_error_enum status = SD_OK;
//     uint32_t count = 0, align = 0, datablksize = SDIO_DATABLOCKSIZE_1BYTE, *ptempbuff = preadbuffer;
//     __IO uint32_t timeout = 0;

//     if(NULL == preadbuffer){
//         status = SD_PARAMETER_INVALID;
//         return status;
//     }
    
//     transerror = SD_OK;
//     transend = 0;
//     totalnumber_bytes = 0;
//     /* clear all DSM configuration */
//     sdio_data_config(0, 0, SDIO_DATABLOCKSIZE_1BYTE);
//     sdio_data_transfer_config(SDIO_TRANSMODE_BLOCK, SDIO_TRANSDIRECTION_TOCARD);
//     sdio_dsm_disable();
//     sdio_dma_disable();
    
//     /* check whether the card is locked */
//     if(sdio_response_get(SDIO_RESPONSE0) & SD_CARDSTATE_LOCKED){
//         status = SD_LOCK_UNLOCK_FAILED;
//         return status;
//     }
    
//     /* blocksize is fixed in 512B for SDHC card */
//     if(SDIO_HIGH_CAPACITY_SD_CARD == cardtype){
//         blocksize = 512;
//         readaddr /= 512;
//     }
    
//     align = blocksize & (blocksize - 1);
//     if((blocksize > 0) && (blocksize <= 2048) && (0 == align)){
//         datablksize = sd_datablocksize_get(blocksize);
//         /* send CMD16(SET_BLOCKLEN) to set the block length */
//         sdio_command_response_config(SD_CMD_SET_BLOCKLEN, (uint32_t)blocksize, SDIO_RESPONSETYPE_SHORT);
//         sdio_wait_type_set(SDIO_WAITTYPE_NO);
//         sdio_csm_enable();
        
//         /* check if some error occurs */
//         status = r1_error_check(SD_CMD_SET_BLOCKLEN);
//         if(SD_OK != status){
//             return status;
//         }
//     }else{
//         status = SD_PARAMETER_INVALID;
//         return status;
//     }
    
//     stopcondition = 0;
//     totalnumber_bytes = blocksize;
    
//     /* configure SDIO data transmission */
//     sdio_data_config(SD_DATATIMEOUT, totalnumber_bytes, datablksize);
//     sdio_data_transfer_config(SDIO_TRANSMODE_BLOCK, SDIO_TRANSDIRECTION_TOSDIO);
//     sdio_dsm_enable();
    
//     /* send CMD17(READ_SINGLE_BLOCK) to read a block */
//     sdio_command_response_config(SD_CMD_READ_SINGLE_BLOCK, (uint32_t)readaddr, SDIO_RESPONSETYPE_SHORT);
//     sdio_wait_type_set(SDIO_WAITTYPE_NO);
//     sdio_csm_enable();
//     /* check if some error occurs */
//     status = r1_error_check(SD_CMD_READ_SINGLE_BLOCK);
//     if(SD_OK != status){
//         return status;
//     }
    
//     if(SD_POLLING_MODE == transmode){
//         /* polling mode */
//         while(!sdio_flag_get(SDIO_FLAG_DTCRCERR | SDIO_FLAG_DTTMOUT | SDIO_FLAG_RXORE | SDIO_FLAG_DTBLKEND | SDIO_FLAG_STBITE)){
//             if(RESET != sdio_flag_get(SDIO_FLAG_RFH)){
//                 /* at least 8 words can be read in the FIFO */
//                 for(count = 0; count < SD_FIFOHALF_WORDS; count++){
//                     *(ptempbuff + count) = sdio_data_read();
//                 }
//                 ptempbuff += SD_FIFOHALF_WORDS;
//             }
//         }
        
//         /* whether some error occurs and return it */
//         if(RESET != sdio_flag_get(SDIO_FLAG_DTCRCERR)){
//             status = SD_DATA_CRC_ERROR;
//             sdio_flag_clear(SDIO_FLAG_DTCRCERR);
//             return status;
//         }else if(RESET != sdio_flag_get(SDIO_FLAG_DTTMOUT)){
//             status = SD_DATA_TIMEOUT;
//             sdio_flag_clear(SDIO_FLAG_DTTMOUT);
//             return status;
//         }else if(RESET != sdio_flag_get(SDIO_FLAG_RXORE)){
//             status = SD_RX_OVERRUN_ERROR;
//             sdio_flag_clear(SDIO_FLAG_RXORE);
//             return status;
//         }else if(RESET != sdio_flag_get(SDIO_FLAG_STBITE)){
//             status = SD_START_BIT_ERROR;
//             sdio_flag_clear(SDIO_FLAG_STBITE);
//             return status;
//         }
//         while(RESET != sdio_flag_get(SDIO_FLAG_RXDTVAL)){
//             *ptempbuff = sdio_data_read();
//             ++ptempbuff;
//         }
//         /* clear the SDIO_INTC flags */
//         sdio_flag_clear(SDIO_MASK_INTC_FLAGS);
//     }else if(SD_DMA_MODE == transmode){
//         /* DMA mode */
//         /* enable the SDIO corresponding interrupts and DMA function */
//         sdio_interrupt_enable(SDIO_INT_CCRCERR | SDIO_INT_DTTMOUT | SDIO_INT_RXORE | SDIO_INT_DTEND | SDIO_INT_STBITE);
//         sdio_dma_enable();
//         dma_receive_config(preadbuffer, blocksize);
//         timeout = 100000;
//         while((RESET == dma_flag_get(DMA1, DMA_CH3, DMA_FLAG_FTF)) && (timeout > 0)){
//             timeout--;
//             if(0 == timeout){
//                 return SD_ERROR;
//             }
//         }
//     }else{
//         status = SD_PARAMETER_INVALID;
//     }
//     return status;
// }


