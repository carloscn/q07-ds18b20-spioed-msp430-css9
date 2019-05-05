#include <msp430.h> 
#include "ds18b20.h"
#include "global.h"
#include "main.h"


struct  ds18b20_t  dev,*p_dev;

unsigned char nihao[] = "nihao,hello msp430!\n";
const unsigned char temp_str[] = "temp : ";
const unsigned char r_n_str[] = "\r\n";
const unsigned char oled_welcome_str[] = "     ";
unsigned char temp[11];


int main(void)
{
    SYSTEM_INIT();
    GPIO_INIT();
    UART_INIT();
    OLED_Init();
    OLED_Clear();
    DS18B20_INIT();
    OLED_ShowString(0,0,oled_welcome_str);
    do_tast();

    return 0;
}

void    do_tast( void )
{
    float   current_temp;
    while( 1 ) {

        current_temp =  p_dev->get_temp_float(p_dev);
        if( current_temp > 40 | current_temp < -10 ) {
            alarm();
        }else {
            no_alarm();
        }

        uart_0_send_temp();

        DELAY_MS(1000);
    }
}

void    uart_0_send_temp( void )
{

    p_dev->save_temp_string( &dev, temp  );
    OLED_ShowString(0,2,"Temperature: ");
    OLED_ShowString(8,4,temp);

    uart_0_send_str( (unsigned char *)temp_str );
    uart_0_send_str( temp );
    uart_0_put_char(' ');
    uart_0_put_char('.');
    uart_0_put_char('C');
    uart_0_put_char('\n');
    uart_0_put_char('\r');


}

void    SYSTEM_INIT( void )
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    BCSCTL1 =   RSEL2+RSEL1+RSEL0;      // Set clock DCO max
    DCOCTL  =   DCO2 + DCO1 + DCO0;   // Set DCO max
}

void    UART_INIT( void )
{
    // UART0
    ME1     |=  UTXE0 + URXE0 ;                         // 使能USART0+1收发
    UCTL0   |=  CHAR;                                   // 8-bit character
    UTCTL0 |=   SSEL0;                                  // UCLK = ACLK
    UBR00   =   0x03;                                   // 9600波特率,最高波特率
    UBR10   =   0x00;                                   //
    UMCTL0 =    0x4A;                                   // Modulation
    UCTL0   &=  ~SWRST;                                 // 初始化UART0状态机
    IE1     |=  URXIE0;                                 // 使能接收中断

    // UART1
    ME2     |=  UTXE1 + URXE1;                      // 使能USART1的TXD和RXD
    UCTL1   |=  CHAR;                                   // 选择8-bit字符
    UTCTL1 |=   SSEL0;                                  // 驱动时钟选择ACLK
    UBR01   =   0x03;                                   // 波特率2400
    UBR11   =   0x00;
    UMCTL1 =    0x4A;                                   // 调整
    UCTL1   &= ~SWRST;                                  // 初始化UART状态机
    _EINT();
}

void uart_0_send_str( unsigned char *ptr )
{

    while( (*ptr != '\0') ) {

        while ( !( IFG1 & UTXIFG0 ) );              // TX缓存空闲？
        TXBUF0 = *ptr++;                                // 发送数据
    }

}

void    uart_0_put_char( unsigned char ch )
{
    while ( !( IFG1 & UTXIFG0 ) );              // TX缓存空闲？
    TXBUF0 = ch;                                // 发送数据
}

void    OLED_INIT( void )
{

}


void    GPIO_INIT( void )
{
    // 关闭按所有的IO口
    P1DIR = 0xFF;P1OUT = 0xFF;
    P2DIR = 0xFF;P2OUT = 0xFF;
    P3DIR = 0xFF;P3OUT = 0xFF;
    P4DIR = 0xFF;P4OUT = 0xFF;
    P5DIR = 0xFF;P5OUT = 0xFF;
    P6DIR = 0xFF;P6OUT = 0xFF;
    // LED
    P2DIR = 0x0f;                             // P2端口设置为输出
    P2OUT=0x0e;//给初始化值
    // 启动IIC占用的引脚
    // P12   <------> SCL       P13 <-----> SDA
    P1DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4;                           // set SCL interface and set SDA interface.
    P1OUT |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4;                           // set SCL and SDA is out.
    // 设定P21 22 23 24 为4个EEPROM的片选信号
    //P2DIR |= BIT1 + BIT2 + BIT3 + BIT4;
    //P2OUT &= ~BIT1 + ~BIT2 + ~BIT3 + ~BIT4;         // 输出全都为高电平
    // 启用SCI引脚
    P3DIR |= BIT4 + BIT5 + BIT6 + BIT7;
    P3OUT |= BIT4 + BIT5 + BIT6 + BIT7;
    P3SEL |= 0xF0;
}

void    DS18B20_INIT( void )
{
    p_dev = &dev;

    p_dev->init =   &ds18b20_init;
    p_dev->check_crc    =   &ds18b20_check_crc;
    p_dev->config       =   &ds18b20_config;
    p_dev->float_2_ascii    =   &ds18b20_float_2_ascii;
    p_dev->get_temp_float   =   &ds18b20_get_temp;
    p_dev->hw.set_high      =   &ds18b20_hw_set_high;
    p_dev->hw.set_low       =   &ds18b20_hw_set_low;
    p_dev->hw.read_line     =   &ds18b20_hw_read_line;
    p_dev->read_byte        =   &ds18b20_read_byte;
    p_dev->read_bytes       =   &ds18b20_read_bytes;
    p_dev->read_id          =   &ds18b20_read_id;
    p_dev->reset            =   &ds18b20_reset;
    p_dev->save_temp_string =   &ds18b20_save_temp_string;
    p_dev->write_byte       =   &ds18b20_write_byte;

    p_dev->reset( p_dev );
}

void  alarm( void )
{

    P2OUT = 0x00;
}
void  no_alarm( void )
{

    P2OUT = 0xff;

}
