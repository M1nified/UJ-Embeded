int timer1_counter;
int hh = 0;
int mm = 0;
int ss = 0;
const int xx = 3;
int xx_timer = 0;
const int measurement_send_hh = 0;
const int measurement_send_mm = 0;
int time_change_mode = 0;
int inactivity_time = 0;
const int gas_din=2;
const int gas_ain=A0;
int next_index = 0;
typedef struct{
  int hh;
  int mm;
  int ss;
  int value;
} measurement;

measurement measurements[100];

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
  pinMode(gas_din,INPUT);
  pinMode(gas_ain,INPUT);

  for ( int i =0 ; i < 100 ; i ++) {
    measurements[i].value = -1;
  }
  
  rot_state = digitalRead(rot_a);
  
  lcd.begin(16, 2);
  // Print a message to the LCD.

  // initialize timer1          
  cli(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval

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
    measurement_check();
    send_check();
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
  lcd.print(get_timestamp(hh, mm, ss));
//  lcd.print(hh);
//  lcd.print(":");
//  lcd.print(mm);
//  lcd.print(":");
//  lcd.print(ss);
//  lcd.print("   ");
//  lcd.print(time_change_mode);
//  lcd.print("   ");
//  lcd.print(rot_state);
}

void measurement_check(){
  xx_timer ++;
  if (xx_timer == xx){
    xx_timer = 0;
    measurements[next_index].value =  analogRead(gas_ain);
    measurements[next_index].hh = hh;
    measurements[next_index].mm = mm;
    measurements[next_index].ss = ss;
    next_index++;
    next_index %= 100;
  }
}

void print_measurement(int i){
//    Serial.print(measurements[i].hh);
//    Serial.print(":");
//    Serial.print(measurements[i].mm);
//    Serial.print(":");
//    Serial.print(measurements[i].ss);
    //Serial.print(" tu");
    String timestamp = get_timestamp(measurements[i].hh, measurements[i].mm, measurements[i].ss);
    Serial.print(timestamp);
    Serial.print("  ");
    Serial.println( measurements[i].value);
}

void send_check(){
  if ( measurement_send_hh == hh && measurement_send_mm == mm && ss == 4){
    for ( int i = next_index; i < 100; i++){
      if (measurements[i].value != -1){
        print_measurement(i);
      }
      else{
        break;
      }
    }
    for ( int i=0; i < next_index ; i ++){
       print_measurement(i);
    }
  }
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
      delay(100);
  }

}
String get_timestamp(int hh, int mm, int ss){
  String timestamp = two_digit_format(hh) + ":" +  two_digit_format(mm) + ":" + two_digit_format(ss);
  return timestamp;
}
String two_digit_format(int i){
  if ( i < 10){
    return String(0) + String(i);
  }
  else{
    return String(i);
  }
    
}
void loop() {
  check_rot();
}
