/*
 * global.h
 *
 *  Created on: 2018Äê4ÔÂ1ÈÕ
 *      Author: weihaochen
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_
#include <msp430.h>
#include "ds18b20.h"
#include "oled.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


#define                 CPU_F                               ((double)8000000)
#define                 DELAY_US(x)                         __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define                 DELAY_MS(x)                         __delay_cycles((long)(CPU_F*(double)x/1000.0))

#endif /* GLOBAL_H_ */
