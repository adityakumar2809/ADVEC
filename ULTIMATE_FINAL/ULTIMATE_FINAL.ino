int forward_motor=6;
int backward_motor=5;

int sensor=A4;


int lock;
char data = '0'; 
char data_temp = '0';

void setup() {
  // put your setup code here, to run once:
  pinMode(forward_motor,OUTPUT);
  pinMode(backward_motor,OUTPUT);
  
  pinMode(sensor,INPUT);
  analogWrite(forward_motor, 0);
  analogWrite(backward_motor, 0);


  lock=0;
  Serial.begin(38400);
  
}

void loop() {
    // put your main code here, to run repeatedly:

  Serial.print("Sensor Reading  :");
  Serial.println(analogRead(sensor));
  Serial.print("State of lock  :");
  Serial.println(lock);
  if(Serial.available() > 0)
  {
    data_temp = Serial.read();
    if(data_temp=='1')
    {
      data='1';
    }
  }

  Serial.print("Permission :");
  Serial.println(data);
  if(lock==0)
  {
    if(analogRead(sensor)>=420)
    {
     
      analogWrite(backward_motor,0);
      analogWrite(forward_motor,255);
      Serial.println("Running forward motor"); 
      delay(3000);
      analogWrite(forward_motor,0);
      lock=1;
    }
  }
  else if(analogRead(sensor)<=400)
  {
    
    if(data == '1')
    {
      analogWrite(forward_motor,0);
      analogWrite(backward_motor,175);
      Serial.println("Running backward motor");
      delay(3000);
      analogWrite(backward_motor,0);
     
      lock=0;
      data='0';
    }
  
      
 }
 delay(1000);
}
 
