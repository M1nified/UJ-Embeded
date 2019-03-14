int timer1_counter;
int counter = 0;

#include <LiquidCrystal.h>

const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(5000); 

  
  lcd.begin(16, 2);
  // Print a message to the LCD.

  // initialize timer1          
  cli(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  //timer1_counter = 64911;   // preload timer 65536-16MHz/256/100Hz
  //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
  timer1_counter = 62500;

  OCR0A = timer1_counter;

  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
          
  sei();  // enable all interrupts
}

ISR(TIMER1_OVF_vect)        // interrupt service routine 
{

  TCNT1 = 0;
  counter++;

  lcd.clear();
  lcd.print(counter);
}

void loop() {

}
