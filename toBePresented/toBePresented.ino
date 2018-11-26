int printState=0, inputState=0, printRepeat=0, inputRepeat=0;
//char input=NULL;
char input[20], temp, id[20], password[20];
int loginType=0, i=0, temp2=0;
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
    while(Serial.available()>0)
    {
      temp=Serial.read();
      if(temp!='\r' && i<19)
      {
        input[i++]=temp;
      }
      temp2=1;
    }
    if(temp2==1)
    {
      input[i]='\0';
      i=0;
      inputState++;
      printRepeat--;
      loginType = atoi(input);
      temp2=0;
    }
  }
  else if(inputState==1)
  {
    while(Serial.available()>0)
    {
      
      temp=Serial.read();
      if(temp!='\r' && i<19)
      {
        id[i++]=temp;
      }
      temp2=1;
      delay(1);
    }
    if(temp2==1)
    {
      id[i]='\0';
      i=0;
      inputState++;
      printRepeat--;
      temp2=0;
      Serial.println(id);
    }
  }
  else if(inputState==2)
  {
    while(Serial.available()>0)
    {
      
      temp=Serial.read();
      if(temp!='\r' && i<19)
      {
        password[i++]=temp;
      }
      temp2=1;
      delay(1);
    }
    if(temp2==1)
    {
      password[i]='\0';
      i=0;
      inputState++;
      printRepeat--;
      temp2=0;
      Serial.println(password);
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
          if(loginType==1)
          {
            Serial.println("You want to login as a user");
          }
          else if(loginType==2)
          {
            Serial.println("You want to login as a Service Center In-Charge");
          }
          else if(loginType==3)
          {
            Serial.println("You want to login as a Pollution Control Board Representative");
          }
          else if(loginType!=NULL)
          {
            Serial.println("Wrong Input\nRedirecting to Home Page");
            printState=inputState=printRepeat=inputRepeat=0;
            return;
          }
          printState=2;
          printRepeat--;
      }
     else if(printState==2)
     {
       Serial.println("Please enter your login id\n");
       printState=3;
     }
    else if(printState==3)
    {
      Serial.println("Please enter your password\n");
        printState=4; 
    }
      printRepeat++;
  }
}
