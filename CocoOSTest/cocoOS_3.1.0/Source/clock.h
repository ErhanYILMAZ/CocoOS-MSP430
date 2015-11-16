/************************************************************************
 * Designer: Erhan YILMAZ												*
 * Application:  CocoOS clock setup codes for MSP430					*
 * Date: 20-01-2015														*
 * Description:	Configures os_tick for selected value in ms				*
 * CPUC_CLOCK value must be defined to calculate timer count value		*
 * os_tick value depending on the CPU_CLOCK. Min os_tick time is 1ms	*
 * max os_tick value is 512ms when CPUCLOK=1MHz							*
 * max os_tick value is 32ms when CPUCLOK=16MHz							*
 * os_tick usually chosen as 1 or 10 ms. Tested on the MSP430G2553		*
 * with CCS 5.5.0 and TI's compiler. Used Timer0_A3 for timing. If		*
 * desired can be use other timers. Maybe some changes, it is not 		*
 * guaranteed that works with all msp430 devices						*
 * **********************************************************************/

#ifndef CLOCK_H
#define CLOCK_H

#define CPU_CLOCK 16000000UL		//CPU Clock frequency define

void clock_init(uint16_t tick_ms);

#endif
