int button = 16;
char str[25];
int i = 0;



void setup() 
{
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(500);
   for (int i = 0; i < 4; i++)
  {
    str[i] = 0;
  }
}

void loop() 
{
  i=0;
  if (Serial.available()) 
  {
    delay(100);
    while(Serial.available() && i<4) 
    {
      str[i] = Serial.read();
      i++;
    }
   Serial1.write( str, i);
    i=0;
  }
  
}
