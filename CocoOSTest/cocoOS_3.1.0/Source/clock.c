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

#include <msp430.h>
#include "cocoos.h"
#include "clock.h"
uint16_t wTimerValue;				// Calculated timer value register

void clock_init(uint16_t tick_ms) {
  uint32_t lPulses;
  uint8_t bPrescaler=0;
  TACTL=0;
  // check prescaler 1,2,4,8 values
  while(bPrescaler<4){
  lPulses = ((CPU_CLOCK * tick_ms)/1000)/(1<<bPrescaler);		// Calculate pulses count
  if(lPulses <= 0x10000)										// if reasonable
  break;														// Finish calculating
  bPrescaler++;													// else increase prescaler value
  }

  // Set presclaer value if pulses count reasonable and prescaler value
  // less than 4 and tick value not equal zero
  if(lPulses <= 0x10000 && lPulses > 0 && bPrescaler<4 && tick_ms != 0)
  {
  TACTL = TASSEL_2 + MC_2 + (bPrescaler<<6); // SMCLK, contmode
  }
  else
  {
  // if time interval not possible or tick value equal zero then set tick value 1ms as default
  lPulses = CPU_CLOCK/1000;
  TACTL = TASSEL_2 + MC_2;                  // SMCLK, contmode
  }

  // Configure timerA0(CCRO) depending on the calculated values
  wTimerValue = (uint16_t)(lPulses-1);
  CCR0 = wTimerValue;
  CCTL0 = CCIE;                             // CCR0 interrupt enabled
}


// TimerA0 interrupt vector.
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
  CCR0 += wTimerValue;					// Add Offset to CCR0
  os_tick();							// Run CocoOS os_tick function
}
