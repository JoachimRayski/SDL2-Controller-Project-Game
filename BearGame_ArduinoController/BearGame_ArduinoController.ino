double frame_delay = 1000/60;
int timer = 3000/frame_delay;
int timer_start_value = timer;

void setup() {
  // put your setup code here, to run once:
  pinMode(9, OUTPUT);
  pinMode(4, INPUT);
  pinMode(2, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0)
  {
    char readData = Serial.read();

    if(readData == 'M')
    {
      timer = timer_start_value;
      analogWrite(9, 125);
    }
    if(readData == 'W')
    {
      analogWrite(9, 0);
      timer = timer_start_value;
    }
  }

  if(digitalRead(4) == 0)
  {
    timer--;
    if(timer <= 0)
    {
      analogWrite(9, 0);
      timer = timer_start_value;
    }
  }
  else
    timer = timer_start_value;
  
  bool buttonValue = digitalRead(2);
  int potentiometerValue = analogRead(A1);
  Serial.print("b");
  Serial.print(buttonValue);
  Serial.print("p");
  Serial.print(potentiometerValue);

  delay(frame_delay);
}
