int led = 13; 
char str[4];
bool flag = false; 

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  Serial.begin(9600);
  for (int i = 0; i<4; i++)
  {
     str[i] = 0;
  }
}

void loop() {
  int i=0;
  if (Serial.available()) 
  {
    delay(100);
    while(Serial.available() && i<4) 
    {
      str[i] = Serial.read();
      i++;
    }
  }
  if (str[0] == '1')
  {
    flag = !flag;
    digitalWrite(led,flag);
  }

}
