/*
 * ds18b20.h
 *
 *  Created on: 2018Äê4ÔÂ1ÈÕ
 *      Author: weihaochen
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include <msp430.h>
#include "type.h"


#define             SET_LINE_IN()                 (P1DIR &= ~BIT0 )
#define             SET_LINE_OUT()                (P1DIR |= BIT0 )
#define             LINE_HIGH()                   (P1OUT |= BIT0 )
#define             LINE_LOW()                    (P1OUT &= ~BIT0 )
#define             LINE_DATA()                   ( P1IN&BIT0 )



struct  ds18b20_hw_t{

    uint16    line_pin;
    struct  ds18b20_hw_t *self;

    void      (*set_high)( struct ds18b20_hw_t *self );
    void      (*set_low)( struct ds18b20_hw_t *self );
    uint8     (*read_line)( struct ds18b20_hw_t *self );
};

struct  ds18b20_t{

    struct ds18b20_hw_t hw;
    struct ds18b20_t    *self;
    float   temp_value;
    uint8   temp_str[8];
    uint8   read_buffer[16];
    uint8   *op;
    uint8   id_buffer[8];

    void    (*init)( struct  ds18b20_t   *self );
    void    (*reset)( struct  ds18b20_t   *self );
    void    (*config)( struct  ds18b20_t   *self );
    void    (*write_byte)(struct ds18b20_t *self, uint8 val );
    uint8   (*read_byte)( struct ds18b20_t *self );
    void    (*read_bytes)( struct ds18b20_t *self, uint8 length );
    uint8   (*check_crc)( struct ds18b20_t *self, uint8 checksum );
    float   (*get_temp_float)( struct ds18b20_t *self );
    void    (*save_temp_string)( struct ds18b20_t *self, uint8 *dis_str );
    void    (*read_id)( struct ds18b20_t *self, uint8 *id_str );
    void    (*float_2_ascii)( struct ds18b20_t *self, float val, uint8 *ascii_str );

};
extern void    ds18b20_float_2_ascii( struct ds18b20_t *self, float f_data, uint8 *fltBuffer);
extern void    ds18b20_save_temp_string( struct ds18b20_t *self , uint8 *dis_str);
extern float   ds18b20_get_temp( struct ds18b20_t *self );
extern void    ds18b20_read_id( struct ds18b20_t *self, uint8 *id_val );
extern void    ds18b20_config( struct ds18b20_t *self );
extern uint8    ds18b20_check_crc( struct ds18b20_t *self, uint8 checksum );
extern void    ds18b20_read_bytes( struct ds18b20_t *self, uint8 length );
extern uint8   ds18b20_read_byte( struct ds18b20_t *self );
extern void    ds18b20_write_byte( struct ds18b20_t *self, uint8 val );
extern void    ds18b20_reset( struct  ds18b20_t   *self );
extern void    ds18b20_init( struct ds18b20_t *self );
extern uint8   ds18b20_hw_read_line( struct ds18b20_hw_t *self );
extern void    ds18b20_hw_set_low( struct ds18b20_hw_t *self );
extern void    ds18b20_hw_set_high( struct ds18b20_hw_t *self );

#endif /* DS18B20_H_ */
