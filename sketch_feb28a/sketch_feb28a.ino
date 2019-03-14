int RXLED = 13; 
int i = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RXLED, OUTPUT); // Set RX LED as an output

  
  Serial.begin(9600);
  Serial.setTimeout(5000); 
}

void loop() {
  // put your main code here, to run repeatedly:
//  digitalWrite(RXLED, HIGH);
//  delay(1000);
//  digitalWrite(RXLED, LOW);
//  delay(1000);

//  Serial.write("hello\n\r");
  String cmd = Serial.readStringUntil('\n');
  char cmd_c[10];
  cmd.toCharArray(cmd_c, 10);

  if(cmd == "on"){
    digitalWrite(RXLED, HIGH);
    
  }
  if(cmd == "off"){
    digitalWrite(RXLED, LOW);
  }
  
  String str = String(i++);
  char c[5];
  str.toCharArray(c,5);
  Serial.write(c);
  Serial.write(cmd_c);
  Serial.write("\n\r");

}
