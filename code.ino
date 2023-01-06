#include <avr/sleep.h>
#include <avr/power.h>

#define button 4
#define showtime 8000
#define showtime_fast_div 80
#define showtime_on 1500

byte shots;

ISR (PCINT0_vect) {}

void setup ()
{
  ADCSRA = 0; // turn off ADC
  DDRB = B00001111; //P0-3 Ausgang, rest eingang
  // pin change interrupt
  PCMSK |= bit (PCINT4); //Button on 4
  GIFR  |= bit (PCIF); // clear any outstanding interrupts
  GIMSK |= bit (PCIE); // enable pin change interrupts
  PORTB = B00111111; //Leds an
  delay(showtime_on);
  PORTB = B00110000; //Leds aus

  shots = B00010000; //
}

void loop()
{
  shots++;
  if(shots > B00001111) shots = 0;
  if(shots == 0) {
    for(byte i = 0; i<B00001111; i++) {
      shots++;
      displayshots_fast();
    }
    for(byte i = B00001111; i>B00000000; i--) {
      shots--;
      displayshots_fast();
    }
    goto sleep;
  }
  displayshots();
  sleep:
  goToSleep();
}

void displayshots() {
  PORTB = B00110000 | shots;  //shots anzeigen
  delay(showtime); //warten
  PORTB = B00110000; //Leds aus
}

void displayshots_fast() {
  PORTB = B00110000 | shots;  //shots anzeigen
  delay(showtime/showtime_fast_div); //warten
  PORTB = B00110000; //Leds aus
}

//I cant get no sleep.
void goToSleep()
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    power_all_disable();  // power off ADC, Timer 0 and 1, serial interface
    sleep_enable();
    sleep_cpu();
    sleep_mode(); 
    sleep_disable();
    //nach dem weck-interrupt gehts hier wieder weiter ;)
    power_all_enable();    // power everything back on
}
