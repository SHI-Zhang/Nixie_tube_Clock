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
#include "gd32f30x.h"
#include "SDIO.h"
#include "SD_Card.h"
#include "string.h"
#include "sys.h"
/*-----------------------------  #INCLUDE   END   ----------------------------*/

/* card status of R1 definitions */
#define SD_R1_OUT_OF_RANGE                  BIT(31)                   /* command's argument was out of the allowed range */
#define SD_R1_ADDRESS_ERROR                 BIT(30)                   /* misaligned address which did not match the block length */
#define SD_R1_BLOCK_LEN_ERROR               BIT(29)                   /* transferred block length is not allowed */
#define SD_R1_ERASE_SEQ_ERROR               BIT(28)                   /* an error in the sequence of erase commands occurred */
#define SD_R1_ERASE_PARAM                   BIT(27)                   /* an invalid selection of write-blocks for erase occurred */
#define SD_R1_WP_VIOLATION                  BIT(26)                   /* the host attempts to write to a protected block or to the temporary or permanent write protected card */
#define SD_R1_CARD_IS_LOCKED                BIT(25)                   /* the card is locked by the host */
#define SD_R1_LOCK_UNLOCK_FAILED            BIT(24)                   /* a sequence or password error has been detected in lock/unlock card command */
#define SD_R1_COM_CRC_ERROR                 BIT(23)                   /* CRC check of the previous command failed */
#define SD_R1_ILLEGAL_COMMAND               BIT(22)                   /* command not legal for the card state */
#define SD_R1_CARD_ECC_FAILED               BIT(21)                   /* card internal ECC was applied but failed to correct the data */
#define SD_R1_CC_ERROR                      BIT(20)                   /* internal card controller error */
#define SD_R1_GENERAL_UNKNOWN_ERROR         BIT(19)                   /* a general or an unknown error occurred during the operation */
#define SD_R1_CSD_OVERWRITE                 BIT(16)                   /* read only section of the CSD does not match or attempt to reverse the copy or permanent WP bits */
#define SD_R1_WP_ERASE_SKIP                 BIT(15)                   /* partial address space was erased */
#define SD_R1_CARD_ECC_DISABLED             BIT(14)                   /* command has been executed without using the internal ECC */
#define SD_R1_ERASE_RESET                   BIT(13)                   /* an erase sequence was cleared before executing */
#define SD_R1_READY_FOR_DATA                BIT(8)                    /* correspond to buffer empty signaling on the bus */
#define SD_R1_APP_CMD                       BIT(5)                    /* card will expect ACMD */
#define SD_R1_AKE_SEQ_ERROR                 BIT(3)                    /* error in the sequence of the authentication process */
#define SD_R1_ERROR_BITS                    (uint32_t)0xFDF9E008      /* all the R1 error bits */

/* card status of R6 definitions */
#define SD_R6_COM_CRC_ERROR                 BIT(15)                   /* CRC check of the previous command failed */
#define SD_R6_ILLEGAL_COMMAND               BIT(14)                   /* command not legal for the card state */
#define SD_R6_GENERAL_UNKNOWN_ERROR         BIT(13)                   /* a general or an unknown error occurred during the operation */

/* card state */
#define SD_CARDSTATE_IDLE                   ((uint8_t)0x00)           /* card is in idle state */
#define SD_CARDSTATE_READY                  ((uint8_t)0x01)           /* card is in ready state */
#define SD_CARDSTATE_IDENTIFICAT            ((uint8_t)0x02)           /* card is in identificat state */
#define SD_CARDSTATE_STANDBY                ((uint8_t)0x03)           /* card is in standby state */
#define SD_CARDSTATE_TRANSFER               ((uint8_t)0x04)           /* card is in transfer state */
#define SD_CARDSTATE_DATA                   ((uint8_t)0x05)           /* card is in data sending state */
#define SD_CARDSTATE_RECEIVING              ((uint8_t)0x06)           /* card is in receiving state */
#define SD_CARDSTATE_PROGRAMMING            ((uint8_t)0x07)           /* card is in programming state */
#define SD_CARDSTATE_DISCONNECT             ((uint8_t)0x08)           /* card is in disconnect state */
#define SD_CARDSTATE_LOCKED                 ((uint32_t)0x02000000)    /* card is in locked state */

#define SD_CHECK_PATTERN                    ((uint32_t)0x000001AA)    /* check pattern for CMD8 */
#define SD_VOLTAGE_WINDOW                   ((uint32_t)0x80100000)    /* host 3.3V request in ACMD41 */

/* parameters for ACMD41(voltage validation) */
#define SD_HIGH_CAPACITY                    ((uint32_t)0x40000000)    /* high capacity SD memory card */
#define SD_STD_CAPACITY                     ((uint32_t)0x00000000)    /* standard capacity SD memory card */

/* SD bus width, check SCR register */
#define SD_BUS_WIDTH_4BIT                   ((uint32_t)0x00040000)    /* 4-bit width bus mode */
#define SD_BUS_WIDTH_1BIT                   ((uint32_t)0x00010000)    /* 1-bit width bus mode */

/* masks for SCR register */
#define SD_MASK_0_7BITS                     ((uint32_t)0x000000FF)    /* mask [7:0] bits */
#define SD_MASK_8_15BITS                    ((uint32_t)0x0000FF00)    /* mask [15:8] bits */
#define SD_MASK_16_23BITS                   ((uint32_t)0x00FF0000)    /* mask [23:16] bits */
#define SD_MASK_24_31BITS                   ((uint32_t)0xFF000000)    /* mask [31:24] bits */

#define SDIO_FIFO_ADDR                      ((uint32_t)SDIO + 0x80U)  /* address of SDIO_FIFO */
#define SD_FIFOHALF_WORDS                   ((uint32_t)0x00000008)    /* words of FIFO half full/empty */
#define SD_FIFOHALF_BYTES                   ((uint32_t)0x00000020)    /* bytes of FIFO half full/empty */

#define SD_DATATIMEOUT                      ((uint32_t)0xFFFFFFFF)    /* DSM data timeout */
#define SD_MAX_VOLT_VALIDATION              ((uint32_t)0x0000FFFF)    /* the maximum times of voltage validation */
#define SD_MAX_DATA_LENGTH                  ((uint32_t)0x01FFFFFF)    /* the maximum length of data */
#define SD_ALLZERO                          ((uint32_t)0x00000000)    /* all zero */
#define SD_RCA_SHIFT                        ((uint8_t)0x10)           /* RCA shift bits */
#define SD_CLK_DIV_INIT                     ((uint16_t)0x012A)        /* SD clock division in initilization phase */
#define SD_CLK_DIV_TRANS                    ((uint16_t)0x0008)        /* SD clock division in transmission phase */

#define SDIO_MASK_INTC_FLAGS                ((uint32_t)0x00C007FF)    /* mask flags of SDIO_INTC */


/*~~~~~~~~~~~~~~~~~~~~~~~~~  DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/
static sdio_card_type_enum cardtype = SDIO_STD_CAPACITY_SD_CARD_V1_1; /* SD card type */
uint32_t sd_scr[2] = {0,0};                                           /* content of SCR register */
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
	u32 timeout=SDIO_CMD0TIMEOUT;			//10000  

	/*2-等待回复?,反正就是等待*/ 
	while(timeout--)
	{
	  if(sdio_flag_get(SDIO_FLAG_CMDSEND) != RESET)break;	//命令已发送(无需响应)	 
	}	 
	
	/*3-判断退出的原因*/ 
	if(timeout==0)
	{
		return SD_CMD_RESP_TIMEOUT;
	}  
	sdio_flag_clear(SDIO_MASK_INTC_FLAGS);//清除所有标记
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
	u32 reg_status=0,resp_r1=0;
	__IO u32 timeout = SDIO_CMD0TIMEOUT;

	/*1-store the content of SDIO_STAT */
	reg_status=SDIO_STAT;
	while(!(reg_status&(SDIO_FLAG_CCRCERR|SDIO_FLAG_CMDTMOUT|SDIO_FLAG_CMDRECV)))		//这里可以看成是等待收到回复
	{
		reg_status=SDIO_STAT;			//不断的更新reg_status 内的值，取自于 SDIO_STAT
	}

	/*2-check whether an error or timeout occurs or command response received 查询回复的具体值或者是否有回复*/
	if(reg_status&SDIO_FLAG_CCRCERR)			//收到数据,但是CRC校验错误
	{
		errorstatus=SD_CMD_CRC_ERROR;
		sdio_flag_clear(SDIO_FLAG_CCRCERR);
		return errorstatus;
	}
	else if(reg_status&SDIO_FLAG_CMDTMOUT)		//超时标志
	{
		errorstatus=SD_CMD_RESP_TIMEOUT;
		sdio_flag_clear(SDIO_FLAG_CMDTMOUT);
		return errorstatus;
	}

	/*3-check whether the last response command index is the desired one 检查最后的一个回复 是不是想要的*/
	if(sdio_command_index_get()!=cmdindex)
	{
		errorstatus=SD_ILLEGAL_COMMAND;
		return errorstatus;
	}

	/*4-clear all the SDIO_INTC flags 上面都完成了，到现在还没跳出，那么这里就要全部进行一下清理了*/
	sdio_flag_clear(SDIO_MASK_INTC_FLAGS);

	/*5-get the SDIO response register 0 for checking 获取接收（反馈）寄存器0内的值进行检查*/
	resp_r1=sdio_response_get(SDIO_RESPONSE0);			//不明所以
	if(SD_ALLZERO==resp_r1 & SD_R1_ERROR_BITS)
	{
		/* no error occurs, return SD_OK */
		errorstatus=SD_OK;
		return errorstatus;
	}

	/*6-if some error occurs, return the error type */
	errorstatus=r1_error_type_check(resp_r1);
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
    if(resp & SD_R1_OUT_OF_RANGE){
        status = SD_OUT_OF_RANGE;
    }else if(resp & SD_R1_ADDRESS_ERROR){
        status = SD_ADDRESS_ERROR;
    }else if(resp & SD_R1_BLOCK_LEN_ERROR){
        status = SD_BLOCK_LEN_ERROR;
    }else if(resp & SD_R1_ERASE_SEQ_ERROR){
        status = SD_ERASE_SEQ_ERROR;
    }else if(resp & SD_R1_ERASE_PARAM){
        status = SD_ERASE_PARAM;
    }else if(resp & SD_R1_WP_VIOLATION){
        status = SD_WP_VIOLATION;
    }else if(resp & SD_R1_LOCK_UNLOCK_FAILED){
        status = SD_LOCK_UNLOCK_FAILED;
    }else if(resp & SD_R1_COM_CRC_ERROR){
        status = SD_COM_CRC_ERROR;
    }else if(resp & SD_R1_ILLEGAL_COMMAND){
        status = SD_ILLEGAL_COMMAND;
    }else if(resp & SD_R1_CARD_ECC_FAILED){
        status = SD_CARD_ECC_FAILED;
    }else if(resp & SD_R1_CC_ERROR){
        status = SD_CC_ERROR;
    }else if(resp & SD_R1_GENERAL_UNKNOWN_ERROR){
        status = SD_GENERAL_UNKNOWN_ERROR;
    }else if(resp & SD_R1_CSD_OVERWRITE){
        status = SD_CSD_OVERWRITE;
    }else if(resp & SD_R1_WP_ERASE_SKIP){
        status = SD_WP_ERASE_SKIP;
    }else if(resp & SD_R1_CARD_ECC_DISABLED){
        status = SD_CARD_ECC_DISABLED;
    }else if(resp & SD_R1_ERASE_RESET){
        status = SD_ERASE_RESET;
    }else if(resp & SD_R1_AKE_SEQ_ERROR){
        status = SD_AKE_SEQ_ERROR;
    }
    return status;
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
	sd_error_enum error_status=SD_OK;
	u32 reg_status=0;

	/*1- 获取SDIO_STAT 寄存器内的值*/
	reg_status=SDIO_STAT;
	while(!(reg_status & (SDIO_FLAG_CCRCERR | SDIO_FLAG_CMDTMOUT | SDIO_FLAG_CMDRECV)))
	{
		reg_status=SDIO_STAT;
	}

	if(reg_status & SDIO_FLAG_CMDTMOUT)
	{
		error_status=SDIO_FLAG_CMDTMOUT;
		sdio_flag_clear(SDIO_FLAG_CMDTMOUT);
		return error_status;
	}

	/* clear all the SDIO_INTC flags */
    sdio_flag_clear(SDIO_MASK_INTC_FLAGS);
    return error_status;
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
	u32 reg_status=0;
	__IO u32 timeout = SDIO_CMD0TIMEOUT;

	/*1- 等待回复store the content of SDIO_STAT */
	reg_status=SDIO_STAT;		/*!< SDIO status register */
		/*没有超时，且没有以下3个回复，才会一直运行下去*/
	while(!(reg_status&(SDIO_FLAG_CCRCERR|SDIO_FLAG_CMDTMOUT|SDIO_FLAG_CMDRECV))&&(timeout>0))
	{
		reg_status=SDIO_STAT;
		--timeout;
	}

	/*2-查询回复的具体值或者是否有回复*/
	if((reg_status&SDIO_FLAG_CMDTMOUT)||timeout==0)			//超时了
	{
		errorstatus = SD_CMD_RESP_TIMEOUT;
		return errorstatus;
	}

	if(reg_status&SDIO_FLAG_CMDRECV)					//成功
	{
		errorstatus= SD_OK;
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
	u32 SDType = SD_STD_CAPACITY;			//is also called sdcardtype
	SDIO_SMD_Send_struct SDIO_Send_Data;
	u8 busyflag=0;

	 /*1-时钟配置、数据线宽度和硬件流(在stm32的库函数中，这是一个函数就设置了)*/
	 /*HCLK = 120MHz, SDIOCLK = 120MHz, SDIO_CK = HCLK/(178 + 2) = 400 KHz*/
	 sdio_clock_config(SDIO_SDIOCLKEDGE_RISING,SDIO_CLOCKBYPASS_DISABLE,SDIO_CLOCKPWRSAVE_DISABLE,SDIO_INIT_CLK_DIV);
	 sdio_bus_mode_set(SDIO_BUSMODE_1BIT);
	 sdio_hardware_clock_disable();
	 
	 /*2-配置上电状态*/
	 sdio_power_state_set(SDIO_POWER_ON);
	 
	 /*3-开启SDIO-CK时钟，时钟使能*/
	 sdio_clock_enable();

	/*4-上电发送等待74个低速时钟，使其进入IDLE STAGE模式*/
	for(i=0;i<74;i++)
	{
		SDIO_Send_Data.index=SD_CMD_GO_IDLE_STATE;
		SDIO_Send_Data.argument=0x00;
		SDIO_Send_Data.response_type=SDIO_RESPONSETYPE_NO;
		SDIO_Send_Data.wait_type=SDIO_WAITTYPE_NO;
		SDIO_SendCommand(&SDIO_Send_Data);

		errorstatus=CMD_Error_Check();
		if(errorstatus==SD_OK)
		{
			break;
		}
	}
	if(errorstatus!=SD_OK)
	{
		return errorstatus;
	}

	/*5-发送CMD8，短响应，检查SD卡接口的特性*/
	SDIO_Send_Data.index=SD_CMD_SEND_IF_COND;
	SDIO_Send_Data.argument=SD_CHECK_PATTERN;
	SDIO_Send_Data.response_type=SDIO_RESPONSETYPE_SHORT;
	SDIO_Send_Data.wait_type=SDIO_WAITTYPE_NO;
	SDIO_SendCommand(&SDIO_Send_Data);			//内含   sdio_csm_enable();

	if(SD_OK==r7_error_check())		 /* SD Card 2.0 */
	{
		cardtype=SDIO_STD_CAPACITY_SD_CARD_V2_0;  //SD 2.0卡
		SDType=SD_HIGH_CAPACITY;		//高容量卡
	}

	/*6-send CMD55(APP_CMD) to indicate next command is application specific command ,
	为下一步发送CMD41做准备，先发CMD55*/
	SDIO_Send_Data.index=SD_CMD_APP_CMD;
	SDIO_Send_Data.argument=0x00;   //SD_STD_CAPACITY;
	SDIO_Send_Data.response_type=SDIO_RESPONSETYPE_SHORT;
	SDIO_Send_Data.wait_type=SDIO_WAITTYPE_NO;
	SDIO_SendCommand(&SDIO_Send_Data);			//内含   sdio_csm_enable();

	if(SD_OK==r1_error_check(SD_CMD_APP_CMD))   /* SD memory card */
	{
		// while((!busyflag) && (count < SD_MAX_VOLT_VALIDATION))
		while((!busyflag) && (count < 0xffff))
		{
			/* 6-1 send CMD55(APP_CMD) to indicate next command is application specific command */
			SDIO_Send_Data.index=SD_CMD_APP_CMD;
			SDIO_Send_Data.argument=0x00;   //SD_STD_CAPACITY;
			SDIO_Send_Data.response_type=SDIO_RESPONSETYPE_SHORT;
			SDIO_Send_Data.wait_type=SDIO_WAITTYPE_NO;
			SDIO_SendCommand(&SDIO_Send_Data);			//内含   sdio_csm_enable();
			/*6-2 check if some error occurs */
			errorstatus=r1_error_check(SD_CMD_APP_CMD);
			if(SD_OK!=errorstatus)
			{
				return errorstatus;
			}

			/* 6-3 send ACMD41(SD_SEND_OP_COND) to get host capacity support information (HCS) and OCR content */
			SDIO_Send_Data.index=SD_APPCMD_SD_SEND_OP_COND;
			SDIO_Send_Data.argument=(SD_VOLTAGE_WINDOW | SDType);   //SD_STD_CAPACITY;
			SDIO_Send_Data.response_type=SDIO_RESPONSETYPE_SHORT;
			SDIO_Send_Data.wait_type=SDIO_WAITTYPE_NO;
			SDIO_SendCommand(&SDIO_Send_Data);			//内含   sdio_csm_enable();
			/*6-4 check if some error occurs */
			errorstatus=r3_error_check();
			if(SD_OK!=errorstatus)
			{
				return errorstatus;
			}

			/*6-5 get the response and check card power up status bit(busy)*/
			response = sdio_response_get(SDIO_RESPONSE0);     //SDIO_RESPONSE0  这是一个寄存器，可能是来存着回复回来的数
			busyflag=(u8) ((response>>31)&(u32)0x01);
			++count;
		}

		if(count>=SD_MAX_VOLT_VALIDATION)
		{
			errorstatus=SD_VOLTRANGE_INVALID;
			return errorstatus;
		}

		if(response &= SD_HIGH_CAPACITY)
		{
			cardtype=SDIO_HIGH_CAPACITY_SD_CARD;
		}
	}
	return errorstatus;
}

/*********************************************************************
 * @fn       : </函数名>
 * @brief    : <SD卡初始化函数>
 * @note     : <note>
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

	/*3-SD卡识别*/
	errorstatus = SD_Power_on();
	return errorstatus;
}
