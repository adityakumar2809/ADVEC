int printState=0, inputState=0;
char input=NULL;
void setup() 
{
 Serial.begin(9600);
}

void loop() 
{
  if(printState==0)
  {
    loginMenu();
  }
  takeInput();
  if(printState==1 && inputState==1)
  {
    printStuff();
  }
}


void loginMenu()
{
  Serial.println("How do you want to login?\n\n");
  Serial.println("1.User\n");
  Serial.println("2.Service Center In-Charge\n");
  Serial.println("3.Pollution Control Board Representative\n\n");
  Serial.println("Please enter your choice");  
  printState=1;
}

void takeInput()
{
  if(Serial.available()>0)
  {
    input=Serial.read();
    inputState=1;
  }
}

void printStuff()
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
