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

#ifndef __SDIO_H
#define __SDIO_H
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  #INCLUDE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "Configur.h"
/*-----------------------------  #INCLUDE   END   ----------------------------*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  #DEFINE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// SDIO时钟计算公式:SDIO_CK时钟=SDIOCLK/[clkdiv+2];其中,SDIOCLK一般为72Mhz
// 使用DMA模式的时候,传输速率可以到24Mhz,不过如果你的卡不是高速卡,可能也会出错
// 出错就请降低时钟,使用查询模式的话,推荐SDIO_TRANSFER_CLK_DIV设置为3或者更大
#define SDIO_INIT_CLK_DIV 0x012A   // SDIO初始化频率，最大400Kh  298+2=300
#define SDIO_TRANSFER_CLK_DIV 0x04 // SDIO传输频率,该值太小可能会导致读写文件出错

// SDIO 指令集
// #define SD_CMD_GO_IDLE_STATE ((u8)0)
// #define SD_CMD_SEND_OP_COND ((u8)1)
// #define SD_CMD_ALL_SEND_CID ((u8)2)
// #define SD_CMD_SET_REL_ADDR ((u8)3) /*!< SDIO_SEND_REL_ADDR for SD Card */
// #define SD_CMD_SET_DSR ((u8)4)
// #define SD_CMD_SDIO_SEN_OP_COND ((u8)5)
// #define SD_CMD_HS_SWITCH ((u8)6)
// #define SD_CMD_SEL_DESEL_CARD ((u8)7)
// #define SD_CMD_HS_SEND_EXT_CSD ((u8)8)
// #define SD_CMD_SEND_CSD ((u8)9)
// #define SD_CMD_SEND_CID ((u8)10)
// #define SD_CMD_READ_DAT_UNTIL_STOP ((u8)11) /*!< SD Card doesn't support it */
// #define SD_CMD_STOP_TRANSMISSION ((u8)12)
// #define SD_CMD_SEND_STATUS ((u8)13)
// #define SD_CMD_HS_BUSTEST_READ ((u8)14)
// #define SD_CMD_GO_INACTIVE_STATE ((u8)15)
// #define SD_CMD_SET_BLOCKLEN ((u8)16)
// #define SD_CMD_READ_SINGLE_BLOCK ((u8)17)
// #define SD_CMD_READ_MULT_BLOCK ((u8)18)
// #define SD_CMD_HS_BUSTEST_WRITE ((u8)19)
// #define SD_CMD_WRITE_DAT_UNTIL_STOP ((u8)20)
// #define SD_CMD_SET_BLOCK_COUNT ((u8)23)
// #define SD_CMD_WRITE_SINGLE_BLOCK ((u8)24)
// #define SD_CMD_WRITE_MULT_BLOCK ((u8)25)
// #define SD_CMD_PROG_CID ((u8)26)
// #define SD_CMD_PROG_CSD ((u8)27)
// #define SD_CMD_SET_WRITE_PROT ((u8)28)
// #define SD_CMD_CLR_WRITE_PROT ((u8)29)
// #define SD_CMD_SEND_WRITE_PROT ((u8)30)
// #define SD_CMD_SD_ERASE_GRP_START ((u8)32) /*!< To set the address of the first write \
// 																											   block to be erased. (For SD card only) */
// #define SD_CMD_SD_ERASE_GRP_END ((u8)33)   /*!< To set the address of the last write block of the \
// 																											   continuous range to be erased. (For SD card only) */
// #define SD_CMD_ERASE_GRP_START ((u8)35)	   /*!< To set the address of the first write block to be erased. \
// 																											   (For MMC card only spec 3.31) */

// #define SD_CMD_ERASE_GRP_END ((u8)36) /*!< To set the address of the last write block of the \
// 																										  continuous range to be erased. (For MMC card only spec 3.31) */

// #define SD_CMD_ERASE ((u8)38)
// #define SD_CMD_FAST_IO ((u8)39)		 /*!< SD Card doesn't support it */
// #define SD_CMD_GO_IRQ_STATE ((u8)40) /*!< SD Card doesn't support it */
// #define SD_CMD_LOCK_UNLOCK ((u8)42)
// #define SD_CMD_APP_CMD ((u8)55)
// #define SD_CMD_GEN_CMD ((u8)56)
// #define SD_CMD_NO_CMD ((u8)64)

/*-----------------------------  #DEFINE   END   -----------------------------*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~   DECLARE VARIABLE BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~*/
typedef struct 
{
	uint32_t index;
	uint32_t argument;
	uint32_t response_type;
	uint32_t wait_type;

}SDIO_SMD_Send_struct;

/*--------------------------- DECLARE  VARIABLE  END   -----------------------*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~  DECLARE FUNCTION BEGIN   ~~~~~~~~~~~~~~~~~~~~~~~~*/
void MY_SDIO_Init(void);
void SDIO_SendCommand(SDIO_SMD_Send_struct* data);
/*-------------------------  DECLARE FUNCTION  END   -------------------------*/

#endif /*__SDIO_H*/

