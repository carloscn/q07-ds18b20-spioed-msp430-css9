/*
 * ds18b20.c
 *
 *
 * you should setup this function in main.
struct  ds18b20_t  dev, *p_dev;
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
}

 */

#include "global.h"

const unsigned char  crc_table[256]={

        0,  94, 188,  226,  97,  63,  221,  131,  194,  156,  126,  32,  163,  253,  31,  65,
        157,  195,  33,  127,  252,  162,  64,  30,  95,  1,  227,  189,  62,  96,  130,  220,
        35,  125,  159,  193,  66,  28,  254,  160,  225,  191,  93,  3,  128,  222,  60,  98,
        190,  224,  2,  92,  223,  129,  99,  61,  124,  34,  192,  158,  29,  67,  161,  255,
        70,  24,  250,  164,  39,  121,  155,  197,  132,  218,  56,  102,  229,  187,  89,  7,
        219,  133, 103,  57,  186,  228,  6,  88,  25,  71,  165,  251,  120,  38,  196,  154,
        101,  59, 217,  135,  4,  90,  184,  230,  167,  249,  27,  69,  198,  152,  122,  36,
        248,  166, 68,  26,  153,  199,  37,  123,  58,  100,  134,  216,  91,  5,  231,  185,
        140,  210, 48,  110,  237,  179,  81,  15,  78,  16,  242,  172,  47,  113,  147,  205,
        17,  79,  173,  243,  112,  46,  204,  146,  211,  141,  111,  49,  178,  236,  14,  80,
        175,  241, 19,  77,  206,  144,  114,  44,  109,  51,  209,  143,  12,  82,  176,  238,
        50,  108,  142,  208,  83,  13,  239,  177,  240,  174,  76,  18,  145,  207,  45,  115,
        202,  148, 118,  40,  171,  245,  23,  73,  8,  86,  180,  234,  105,  55,  213, 139,
        87,  9,  235,  181,  54,  104,  138,  212,  149,  203,  41,  119,  244,  170,  72,  22,
        233,  183,  85,  11,  136,  214,  52,  106,  43,  117,  151,  201,  74,  20,  246,  168,
        116,  42,  200,  150,  21,  75,  169,  247,  182,  232,  10,  84,  215,  137,  107,  53
};
// You should setup a function.

void    ds18b20_hw_set_high( struct ds18b20_hw_t *self )
{
    SET_LINE_OUT();
    LINE_HIGH();
}

void    ds18b20_hw_set_low( struct ds18b20_hw_t *self )
{
    SET_LINE_OUT();
    LINE_LOW();
}

uint8   ds18b20_hw_read_line( struct ds18b20_hw_t *self )
{

    SET_LINE_IN();
    if( LINE_DATA() == 1 ) {
        return 1;
    }else{
        return 0;
    }
}

void    ds18b20_init( struct ds18b20_t *self )
{
    DELAY_US(2);

    self->hw.set_low( &self->hw );
    DELAY_US(490);
    self->hw.set_high( &self->hw );
    DELAY_US(100);
    DELAY_US(480);
    self->hw.set_high( &self->hw );

}

void    ds18b20_reset( struct  ds18b20_t   *self )
{

}

void    ds18b20_write_byte( struct ds18b20_t *self, uint8 val )
{
    uint8   i;
    for( i = 0; i < 8; i++ ) {
        self->hw.set_low( &self->hw );
        DELAY_US(2);
        if( val & 0x01 ) {
            self->hw.set_high( &self->hw );
        }else{
            self->hw.set_low( &self->hw );
        }
        DELAY_US(45);
        self->hw.set_high( &self->hw );
        val >>= 1;
    }
}


uint8   ds18b20_read_byte( struct ds18b20_t *self )
{
    uint8  i,u = 0;

    for( i = 0 ; i < 8 ; i ++) {

        self->hw.set_low( &self->hw );
        DELAY_US (2);
        u >>= 1;

        self->hw.set_high( &self->hw );
        DELAY_US (4);

        if( self->hw.read_line( &self->hw )  == 1)
            u |= 0x80;
        DELAY_US (65);

    }
    return(u);
}

void    ds18b20_read_bytes( struct ds18b20_t *self, uint8 length )
{
    uint8  i;
    for( i = 0 ; i < length ; i ++)
    {
        *(self->op) = self->read_byte( self );
        self->op ++;
    }
}

uint8    ds18b20_check_crc( struct ds18b20_t *self, uint8 checksum )
{
    uint8 i,crc_data = 0;
    for( i = 0; i < checksum; i++ )
        crc_data    =   crc_table[ crc_data^( *(self->read_buffer + i) ) ];
    return crc_data;
}

void    ds18b20_config( struct ds18b20_t *self )
{
    self->init( self );
    self->write_byte( self, 0xcc);  //skip rom
    self->write_byte( self, 0x4e);  //write scratchpad
    self->write_byte( self, 0x19);  //上限
    self->write_byte( self, 0x1a);  //下限
    self->write_byte( self, 0x7f);     //set 12 bit (0.125)
    self->init( self );
    self->write_byte( self, 0xcc);  //skip rom
    self->write_byte( self, 0x48);  //保存设定值
    self->init( self );
    self->write_byte( self, 0xcc);  //skip rom
    self->write_byte( self, 0xb8);  //回调设定值
}

void    ds18b20_read_id( struct ds18b20_t *self, uint8 *id_val )
{
    self->init( self );
    self->write_byte( self, 0x33);  //skip rom
    self->read_bytes( self, 8 );
}



float   ds18b20_get_temp( struct ds18b20_t *self )
{
    float temps;

    self->op = self->id_buffer;
    self->read_id( self ,self->id_buffer );
    self->config( self );
    self->init( self );
    self->write_byte( self, 0xcc );
    self->write_byte( self, 0x44 );
    self->init( self );
    self->write_byte( self, 0xcc );
    self->write_byte( self, 0xbe );

    self->op = self->read_buffer;
    self->read_bytes(  self , 9 ) ;
    if ( self->check_crc( self, 9) == 0 ) {
        temps = self->read_buffer[1] * 0x100 + self->read_buffer[0];
        //      Temperature *= 0.625;
        temps /= 16;
        DELAY_US(10);
    }

    return temps;
}

void    ds18b20_save_temp_string( struct ds18b20_t *self, uint8 *dis_str )
{

    float   temp;
    temp = self->get_temp_float( self );
    self->float_2_ascii( self, temp, self->temp_str );
    memcpy( dis_str,self->temp_str ,8);

}

void    ds18b20_float_2_ascii( struct ds18b20_t *self, float f_data, uint8 *fltBuffer)
{

    if (f_data < 0) {
        fltBuffer[0] = (long)'-';
        f_data = -f_data;
    } else {
        fltBuffer[0] = (long)' ';
    }
    if (f_data < 999 && f_data > 99) {
        fltBuffer[1] = (long)((long)(f_data) / 100) % 1000 + 0x30;
        fltBuffer[2] = (long)((long)(f_data) / 10) % 10 + 0x30;
        fltBuffer[3] = (long)((long)(f_data) / 1) % 10 + 0x30;
        fltBuffer[4] = (long)('.');
        fltBuffer[5] = (long)(f_data * 10) % 10 + 0x30;
        fltBuffer[6] = (long)(f_data * 100) % 10 + 0x30;
        fltBuffer[7] = (long)'\0';
    } else if (f_data < 99 && f_data > 9) {
        fltBuffer[1] = (long)' ';
        fltBuffer[2] = (long)((long)(f_data) / 10) % 10 + 0x30;
        fltBuffer[3] = (long)((long)(f_data) / 1) % 10 + 0x30;
        fltBuffer[4] = (long)('.');
        fltBuffer[5] = (long)(f_data * 10) % 10 + 0x30;
        fltBuffer[6] = (long)(f_data * 100) % 10 + 0x30;
        fltBuffer[7] = (long)'\0';
    } else if (f_data < 9) {
        fltBuffer[1] = (long)' ';
        fltBuffer[2] = (long)' ';
        fltBuffer[3] = (long)((long)(f_data) / 1) % 10 + 0x30;
        fltBuffer[4] = (long)('.');
        fltBuffer[5] = (long)(f_data * 10) % 10 + 0x30;
        fltBuffer[6] = (long)(f_data * 100) % 10 + 0x30;
        fltBuffer[7] = (long)'\0';
    }
}

