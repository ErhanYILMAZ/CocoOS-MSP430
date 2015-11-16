/************************************************************
 * Designer: Erhan YILMAZ									*
 * Application:  CocoOS Test								*
 * Date: 20-01-2015											*
 * Description:	Simple test application for CocoOS which	*
 * have 3 tasks and 1 event. 								*
 * **********************************************************/

#include <msp430.h>
#include "cocoOS_3.1.0/Source/cocoos.h"
#include "cocoOS_3.1.0/Source/clock.h"

Evt_t ButtonEvent;		// Buton olayý için deðiþken tanýmla

// ButtonTask: Creates event(ButtonEvent) signal when button pressed and released
// ButtonTask: Butona basýp býrakýnca buton olayý oluþturur.
void ButtonTask(void) {
    task_open();				// Görevi aç
    for (;;) {
    if(!(P1IN & BIT3)){			// Butona(P1.3) baasýldý mý?
    task_wait(20);				// Buton arký için 20 tick(20ms) bekle
    while(!(P1IN & BIT3));		// Butonun býrakýlmasýný bekle
    event_signal(ButtonEvent);	// Buton olayý oluþtur
    }
    task_wait(100);				// 100ms'de bir görevi çalýþtýr.
    }
    task_close();				// Görevi kapat
}

// LEDToggleTask: Wait for the event(ButtonEvent) and toggle LED(P1.0) when event occurred
// LEDToggleTask: buton olayý oluþmasýný bekler. Olay oluþunca LED'in(P1.0) durumunu deðiþtirir.
void LEDToggleTask(void) {
    task_open();				// Görevi aç
    for (;;) {
    event_wait(ButtonEvent);	// Buton olayý için bekle
    P1OUT ^= BIT0;				// Kýrmýzý LED'i(P1.0) tersle
    }
    task_close();				// Görevi kapat
}

// LEDBlinkTask: Toggle LED(P1.6) state in every 500ms
// LEDBlinkTask: Her 500ms'de bir LED'in(P1.6) durumunu deðiþtirir
void LEDBlinkTask(void) {
    task_open();			// Görevi aç
    for (;;) {
    P1OUT ^= BIT6;			// Yeþil LED'i(P1.6) tersle
    task_wait(500);			// 500 tick(500ms) bekle
    }
    task_close();			// Görevi kapat
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Watchdog timeri durdur
    BCSCTL1 = CALBC1_16MHZ;		// Dahili osilatörü 16 MHz olarak ayarla
    DCOCTL = CALDCO_16MHZ;		// Dahili osilatörü 16 MHz olarak ayarla
    P1DIR &= ~BIT3;				// P1.3'ü(buton) giriþ olarak ayarla
    P1DIR |= BIT0+BIT6;			// P1.0(LED) ve P1.6'yý(LED) çýkýþ olarak ayarla
    P1REN |= BIT3;				// P1.3 Pull-Up/Down özelliðini aktif et
    P1OUT |= BIT3;				// P1.3 Pul-Up direncini aktif et
    P1OUT &= ~(BIT0+BIT6);		// LEDleri baþlangýçta söndür.

    ButtonEvent = event_create();		// Buton olayýný oluþtur
// Görevler tanýmlanýr. OS round robin çalýþacaðý için tüm görevler sýralý çalýþtýrýlýr.
// Öncelik tanýmlamalarýnýn öenmi yoktur.
    task_create(ButtonTask,0,0,0,0);		// Button durmunu kontrol eden görev(task)
    task_create(LEDToggleTask,1,0,0,0);		// LED durumunu deðiþtiren görev(task)
    task_create(LEDBlinkTask,2,0,0,0);		// LED blink görevi(task)

    os_init();			// OS ayarlarýný yap
    clock_init(1);		// Clock ayarlarý os_tick=1ms
    os_start();			// OS'yi baþlat
	return 1;
}
