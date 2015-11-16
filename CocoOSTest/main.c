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

Evt_t ButtonEvent;		// Buton olay� i�in de�i�ken tan�mla

// ButtonTask: Creates event(ButtonEvent) signal when button pressed and released
// ButtonTask: Butona bas�p b�rak�nca buton olay� olu�turur.
void ButtonTask(void) {
    task_open();				// G�revi a�
    for (;;) {
    if(!(P1IN & BIT3)){			// Butona(P1.3) baas�ld� m�?
    task_wait(20);				// Buton ark� i�in 20 tick(20ms) bekle
    while(!(P1IN & BIT3));		// Butonun b�rak�lmas�n� bekle
    event_signal(ButtonEvent);	// Buton olay� olu�tur
    }
    task_wait(100);				// 100ms'de bir g�revi �al��t�r.
    }
    task_close();				// G�revi kapat
}

// LEDToggleTask: Wait for the event(ButtonEvent) and toggle LED(P1.0) when event occurred
// LEDToggleTask: buton olay� olu�mas�n� bekler. Olay olu�unca LED'in(P1.0) durumunu de�i�tirir.
void LEDToggleTask(void) {
    task_open();				// G�revi a�
    for (;;) {
    event_wait(ButtonEvent);	// Buton olay� i�in bekle
    P1OUT ^= BIT0;				// K�rm�z� LED'i(P1.0) tersle
    }
    task_close();				// G�revi kapat
}

// LEDBlinkTask: Toggle LED(P1.6) state in every 500ms
// LEDBlinkTask: Her 500ms'de bir LED'in(P1.6) durumunu de�i�tirir
void LEDBlinkTask(void) {
    task_open();			// G�revi a�
    for (;;) {
    P1OUT ^= BIT6;			// Ye�il LED'i(P1.6) tersle
    task_wait(500);			// 500 tick(500ms) bekle
    }
    task_close();			// G�revi kapat
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Watchdog timeri durdur
    BCSCTL1 = CALBC1_16MHZ;		// Dahili osilat�r� 16 MHz olarak ayarla
    DCOCTL = CALDCO_16MHZ;		// Dahili osilat�r� 16 MHz olarak ayarla
    P1DIR &= ~BIT3;				// P1.3'�(buton) giri� olarak ayarla
    P1DIR |= BIT0+BIT6;			// P1.0(LED) ve P1.6'y�(LED) ��k�� olarak ayarla
    P1REN |= BIT3;				// P1.3 Pull-Up/Down �zelli�ini aktif et
    P1OUT |= BIT3;				// P1.3 Pul-Up direncini aktif et
    P1OUT &= ~(BIT0+BIT6);		// LEDleri ba�lang��ta s�nd�r.

    ButtonEvent = event_create();		// Buton olay�n� olu�tur
// G�revler tan�mlan�r. OS round robin �al��aca�� i�in t�m g�revler s�ral� �al��t�r�l�r.
// �ncelik tan�mlamalar�n�n �enmi yoktur.
    task_create(ButtonTask,0,0,0,0);		// Button durmunu kontrol eden g�rev(task)
    task_create(LEDToggleTask,1,0,0,0);		// LED durumunu de�i�tiren g�rev(task)
    task_create(LEDBlinkTask,2,0,0,0);		// LED blink g�revi(task)

    os_init();			// OS ayarlar�n� yap
    clock_init(1);		// Clock ayarlar� os_tick=1ms
    os_start();			// OS'yi ba�lat
	return 1;
}
