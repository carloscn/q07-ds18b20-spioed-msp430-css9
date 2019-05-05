//////////////////////////////////////////////////////////////////////////////////	 
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   P1.1 SCL
//              D1   P1.2 SDA
//              RES  P1.3
//              DC   P1.4
//              CS   P1.5
//              ----------------------------------------------------------------
//******************************************************************************/

#ifndef __OLED_H
#define __OLED_H			  	 
//#include "sys.h"
//#include "stdlib.h"	  
#define  u8 unsigned char 
#define  u32 unsigned int 
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define OLED_MODE 0

#define             OLED_CS_LOW()                 (P1OUT &= ~BIT5)
#define             OLED_CS_HIGH()                (P1OUT |= BIT5 )

#define             OLED_RST_LOW()                 (P1OUT &= ~BIT3)
#define             OLED_RST_HIGH()                (P1OUT |= BIT3 )

#define             OLED_DC_LOW()                 (P1OUT &= ~BIT4)
#define             OLED_DC_HIGH()                (P1OUT |= BIT4 )

#define             OLED_SCLK_LOW()                 (P1OUT &= ~BIT1)
#define             OLED_SCLK_HIGH()                (P1OUT |= BIT1 )

#define             OLED_SDI_LOW()                 (P1OUT &= ~BIT2)
#define             OLED_SDI_HIGH()                (P1OUT |= BIT2 )


//OLED模式设置
//0:4线串行模式
//1:并行8080模式

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED端口定义----------------  					   

void delay_ms(unsigned int ms);


 		     

//OLED控制用函数
extern void OLED_WR_Byte(u8 dat,u8 cmd);
extern void OLED_Display_On(void);
extern void OLED_Display_Off(void);
extern void OLED_Init(void);
extern void OLED_Clear(void);
extern void OLED_DrawPoint(u8 x,u8 y,u8 t);
extern void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
extern void OLED_ShowChar(u8 x,u8 y,u8 chr);
extern void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
extern void OLED_ShowString(u8 x,u8 y, u8 *p);
extern void OLED_Set_Pos(unsigned char x, unsigned char y);
extern void OLED_ShowCHinese(u8 x,u8 y,u8 no);
extern void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  
	 



