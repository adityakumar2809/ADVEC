int printState=0, inputState=0, printRepeat=0, inputRepeat=0;
char input=NULL;
void setup() 
{
 Serial.begin(9600);
}

void loop() 
{
  printStuff();
  inputStuff();
}

void inputStuff()
{
  if(inputState==0)
  {
    if(Serial.available()>0)
    {
      input=Serial.read();
      inputState=1;
      printRepeat=0;
    }
  }
}



void printStuff()
{
  if(printRepeat==0)
  {
      if(printState==0)
      { 
        Serial.println("How do you want to login?\n\n");
        Serial.println("1.User\n");
        Serial.println("2.Service Center In-Charge\n");
        Serial.println("3.Pollution Control Board Representative\n\n");
        Serial.println("Please enter your choice");  
        printState=1;
      }
      else if(printState==1)
      {
          if(input=='1')
          {
            Serial.println("You want to login as a user");
          }
          else if(input=='2')
          {
            Serial.println("You want to login as a Service Center In-Charge");
          }
          else if(input=='3')
          {
            Serial.println("You want to login as a Pollution Control Board Representative");
          }
          else if(input!=NULL)
          {
            Serial.println("Wrong Input\nRedirecting to Home Page");
          }
          printState=2;
      }
      printRepeat=1;
  }
}
