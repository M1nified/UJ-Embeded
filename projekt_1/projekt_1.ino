int timer1_counter;
int hh = 0;
int mm = 0;
int ss = 0;
const int xx = 40;
//int change = 0;
int time_change_mode = 0;
int inactivity_time = 0;


#include <LiquidCrystal.h>

const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int rot_a = 13;
const int rot_b = 12;
const int rot_sw = 11;
int rot_state = 1;

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(5000); 
  pinMode(rot_a, INPUT);
  pinMode(rot_b, INPUT);
  pinMode(rot_sw, INPUT);
  
  rot_state = digitalRead(rot_a);
  
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
  if ( time_change_mode == 0 ){
    increment_time();
  }
  else{
    inactivity_time ++;
    if ( inactivity_time == 5){
      time_change_mode ++;
      time_change_mode %=  3;
      inactivity_time = 0;
    }
  }
  
  lcd.clear();
  lcd.print(hh);
  lcd.print(":");
  lcd.print(mm);
  lcd.print(":");
  lcd.print(ss);
  lcd.print("   ");
  lcd.print(time_change_mode);
  lcd.print("   ");
  lcd.print(rot_state);
}

void increment_time(){
  ss++;
  if ( ss == 60){
    ss = 0;
   change_mm(1);
  }
}

void change_mm(int dirrection){
  if ( dirrection > 0){
    mm ++;
  }
  else{
    mm --;
  }
   if ( mm == 60){
      mm = 0;
      change_hh(1);
    }
   if( mm == -1 ){
    mm = 59;
    change_hh(-1);
   }
}

void change_hh(int dirrection){
   if ( dirrection > 0){
    hh ++;
  }
  else{
    hh --;
  }
  if ( hh == 24){
    hh = 00;
  }
  if ( hh == -1){
    hh = 23;
  }
}

void change_time_by_mode(int dirrection){
    if ( time_change_mode == 1 ){
      change_hh(dirrection);
    }
    else if ( time_change_mode == 2 ){
      change_mm(dirrection);
    }
}
void check_rot(){
   int current_rot_state =  digitalRead(rot_a);
   String str = String(current_rot_state);
   char c[5];
   str.toCharArray(c,5);

    if ( current_rot_state != rot_state){
      Serial.write(c);
      Serial.write("\n"); 
      inactivity_time = 0;
      ss = 0;
      if (time_change_mode == 0){
        time_change_mode = 1;
      }
      if ( digitalRead(rot_b) != current_rot_state )
        change_time_by_mode(-1);
        
      else {
        change_time_by_mode(1);
      }
      //rot_state = current_rot_state;
      Serial.write("In check rot \n"); 
      delay(100);
  }
  //rot_state = current_rot_state;
}

void loop() {
  check_rot();
}
