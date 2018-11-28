//PIN CONNECTIONS
int sensor = A4;
int forwardMotor=6;
int backwardMotor=5;

int  limSensorReading=-1024, timeLeft=5, timeExtended=2;
byte lockState=0, firstRise=0, finalLock=0;
unsigned int tlStart=0, tlCurrent=0, teStart=0, teCurrent=0; 


//MENU 0 VARIABLES
byte printState=0, inputState=0, printRepeat=0, inputRepeat=0;
char input[10], temp, id[10], password[10], std_id[]="pcb@advec", std_password[]="pcb@123";
byte loginType=0, i=0, temp2=0, check=0, tookPass=0, menuStatus=0;

//MENU 1 VARIABLES
byte userPrintRepeat=0, userPrintState=0, userInputState=0, userChoice=0, userSensorReading, userTimeExtendPerm=0;
char veh_reg[15]="PB11 A 1111";

//MENU 2 VARIABLES
byte ssPrintRepeat=0, ssPrintState=0, ssInputState=0, ssChoice=0, ssDue=0;
char ss_reg[10]="PNB025875", ss_veh_reg[15];

//MENU 3 VARIABLES
byte pcbPrintRepeat=0, pcbPrintState=0, pcbInputState=0, pcbChoice=0, pcbPerm=0, pcbChangeChoice=0;
char pcb_veh_reg[15];
void setup() 
{
  pinMode(forwardMotor,OUTPUT);
  pinMode(backwardMotor,OUTPUT);
  pinMode(sensor,INPUT);
  analogWrite(forwardMotor, 0);
  analogWrite(backwardMotor, 0);
  Serial.begin(9600);
}
void loop() {
  // put your main code here, to run repeatedly:

 checkEmissions();
  if(menuStatus==0)
  {
    printStuff();
    inputStuff();
    if(check==1 && tookPass==1)
    {
      checkCredentials();
    }
  }
    else if(menuStatus==3)
  {
    pcbPrintStuff();
    pcbInputStuff();
  } 
  
  
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
      tookPass=1;
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
            check=1;
          }
          else if(loginType==2)
          {
            Serial.println("You want to login as a Service Center In-Charge");
            check=1;
          }
          else if(loginType==3)
          {
            Serial.println("You want to login as a Pollution Control Board Representative");
            check=1;
          }
          else if(loginType!=0)
          {
            Serial.println("Wrong Input\nRedirecting to Home Page");
            printState=inputState=printRepeat=inputRepeat=loginType=0;
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


//EMMISIONS CHECK

void checkEmissions()
{
  userSensorReading=analogRead(sensor);
  if(userSensorReading>limSensorReading && lockState==0 && firstRise==0)
  {
    firstRise=1;
    tlStart=millis();
  }
  if(userSensorReading<limSensorReading && lockState==0 && firstRise==1 && userTimeExtendPerm==0 )
  {
    firstRise=0;
  }
  if(userSensorReading>limSensorReading && lockState==0 && firstRise==1 && userTimeExtendPerm==0 )
  {
    tlCurrent=millis();
    if((tlCurrent-tlStart)>(timeLeft*1000))
    {
      analogWrite(forwardMotor,255);
      delay(3000);
      analogWrite(forwardMotor,0);
      lockState=1;
    }
  }
  if(userTimeExtendPerm==1 && lockState==0 && userSensorReading<limSensorReading)
  {
    userTimeExtendPerm=0;
    firstRise=0;
  }
  if(userTimeExtendPerm==1 && lockState==0 && userSensorReading>limSensorReading)
  {
    teCurrent = millis();
    if((teCurrent-teStart)>(timeExtended*1000))
    {
      analogWrite(forwardMotor,255);
      delay(3000);
      analogWrite(forwardMotor,0);
      lockState=1;
      finalLock=1;
    }

  }
  if(finalLock==1 && pcbPerm==1)
  {
    analogWrite(backwardMotor,255);
    delay(300);
    analogWrite(backwardMotor,0);
    lockState=finalLock=pcbPerm=userTimeExtendPerm=firstRise=0;
  }
}

void checkCredentials()
{
  if(loginType==1)
  {
    if(strcmp(id,std_id)==0 && strcmp(password,std_password)==0)
    {
      Serial.println("\nLogin successfull\n");
      check=0;
      tookPass=0;
      menuStatus=1;
    }
    else
    {
      Serial.println("\nLogin Unsuccessfull!!!\nWrong ID or PASSWORD\nRedirecting to Home Page");
      printState=inputState=printRepeat=inputRepeat=loginType=check=tookPass=0;
      return;
    }
      
  }
  if(loginType==2)
  {
    if(strcmp(id,std_id)==0 && strcmp(password,std_password)==0)
    {
      Serial.println("\nLogin successfull\n");
      check=tookPass=0;
      menuStatus=2;
    }
    else
    {
      Serial.println("\nLogin Unsuccessfull!!!\nWrong ID or PASSWORD\nRedirecting to Home Page");
      printState=inputState=printRepeat=inputRepeat=loginType=tookPass=check=0;
      return;
    }
      
  }
  if(loginType==3)
  {
    if(strcmp(id,std_id)==0 && strcmp(password,std_password)==0)
    {
      Serial.println("\nLogin successfull\n");
      check=tookPass=0;
      menuStatus=3;
    }
    else
    {
      Serial.println("\nLogin Unsuccessfull!!!\nWrong ID or PASSWORD\nRedirecting to Home Page");
      printState=inputState=printRepeat=inputRepeat=loginType=tookPass=check=0;
      return;
    }
      
  }
}
void pcbPrintStuff()
{
  if(pcbPrintRepeat==0)
  {
    if(pcbPrintState==0)
    {
      Serial.println("\n\n\n\n\nWELCOME TO POLLUTION CONTROL BOARD PORTAL");
      Serial.print("You have ");
      Serial.print(ssDue);
      Serial.println(" request pending");
      Serial.println("\nChoose an action to perform\n");
      Serial.println("\n1.Grant Permissions");
      Serial.println("2.Check emissions of a vehicle");
      Serial.println("3.Change standard limits");
      Serial.println("4.Logout");
      pcbPrintState++;
    }
    else if(pcbPrintState==1)
    {
      if(pcbChoice==1)
      {
        if(ssDue==0)
        {
          Serial.println("\nNone of the registered vehicles await for permission");
          Serial.println("\nRedirecting to Home Page in 5 seconds");
          delay(5000);
          pcbPrintState=pcbInputState=pcbChoice=0;
          pcbPrintRepeat=-1;
        }
        else
        {
          Serial.print("The vehicle with registration number ");
          Serial.print(veh_reg);
          Serial.println(" awaits for your permission");
          Serial.print("It was serviced in the service centre with registration number ");
          Serial.println(ss_reg);
          Serial.print("Its current emission levels are ");
          Serial.println(userSensorReading);
          Serial.println("\nDo you wish to grant permission?");
          Serial.println("\n1.YES\n2.NO");
          pcbPrintState++;
        }
      }
      else if(pcbChoice==2)
      {
        Serial.println("\n\nEnter the registration number of the vehicle you wish to examine");
        pcbPrintState++;
      }
      else if(pcbChoice==3)
      {
        Serial.println("\n\nWhich values you wish to change?\n\n1.Limiting Emission Values\n2.Minimum time limit\n3.Extended time limit");
        pcbPrintState++;
      }
      else if(pcbChoice==4)
      {
        Serial.println("Logged out successfully");
        pcbPrintState=pcbInputState=pcbChoice=pcbChangeChoice=0;
        pcbPrintRepeat=-1;
        printState=inputState=printRepeat=inputRepeat=loginType=0;
        menuStatus=0;
      }
    }
    else if(pcbPrintState==2)
    {
      if(pcbChoice==1)
      {
        if(pcbPerm==1)
        {
          Serial.println("\nPermission Granted");
          Serial.println("\nRedirecting to Home Page in 5 seconds");
          delay(5000);
          pcbPrintState=pcbInputState=pcbChoice=pcbChangeChoice=0;
          pcbPrintRepeat=-1;
          ssDue--;
        }
        else if(pcbPerm==0)
        {
          Serial.println("\nPermission Denied");
          Serial.println("\nRedirecting to Home Page in 5 seconds");
          delay(5000);
          pcbPrintState=pcbInputState=pcbChoice=pcbChangeChoice=0;
          pcbPrintRepeat=-1;
        }
      }
      else if(pcbChoice==2)
      {
        if(strcmp(pcb_veh_reg,veh_reg)==0)
        {
          Serial.print("Record of ");
          Serial.print(veh_reg);
          Serial.println(" found");
          Serial.print("Its current emission levels are ");
          Serial.println(userSensorReading);
        }
        else
        {
          Serial.print("\nNo records found for the vehicle ");
          Serial.println(pcb_veh_reg);
          Serial.println("\nKindly recheck the entered field");
        }
        Serial.println("\nRedirecting to Home Page in 5 seconds");
        delay(5000);
        pcbPrintState=pcbInputState=pcbChoice=pcbChangeChoice=0;
        pcbPrintRepeat=-1;
      }
      else if(pcbChoice==3)
      {
        Serial.println("\nEnter new value for the chosen parameter");
        pcbPrintState++;
      }
    }
    pcbPrintRepeat++;
  }
}


void pcbInputStuff()
{
  if(pcbInputState==0)
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
      pcbInputState++;
      pcbPrintRepeat--;
      pcbChoice = atoi(input);
      temp2=0;
    }
  }
  else if(pcbInputState==1)
  {
    if(pcbChoice==1)
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
        pcbInputState++;
        pcbPrintRepeat--;
        pcbPerm = atoi(input);
        if(pcbPerm==2)
        {
          pcbPerm=0;
        }
        temp2=0;
      }
    }
    else if(pcbChoice==2)
    {
      while(Serial.available()>0)
      {
        temp=Serial.read();
        if(temp!='\r' && i<19)
        {
          pcb_veh_reg[i++]=temp;
        }
        temp2=1;
        delay(1);
      }
      if(temp2==1)
      {
        pcb_veh_reg[i]='\0';
        i=0;
        pcbInputState++;
        pcbPrintRepeat--;
        temp2=0;
      }
    }
    else if(pcbChoice==3)
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
        pcbInputState++;
        pcbPrintRepeat--;
        pcbChangeChoice = atoi(input);
        temp2=0;
      }
    }
  }
  else if(pcbInputState==2)
  {
    if(pcbChoice==3)
    {
      while(Serial.available()>0)
      {
        temp=Serial.read();
        if(temp!='\r' && i<19)
        {
          input[i++]=temp;
        }
        delay(1);
        temp2=1;
      }
      if(temp2==1)
      {
        input[i]='\0';
        i=0;
        pcbInputState++;
        pcbPrintRepeat--;
        if(pcbChangeChoice==1)
        {
          limSensorReading = atoi(input);
        }
        else if(pcbChangeChoice==2)
        {
          timeLeft = atoi(input);
        }
        else if(pcbChangeChoice==3)
        {
          timeExtended = atoi(input);
        }
        temp2=0;
        Serial.println("The updated values are ");
        Serial.print("Limiting Emission Value = ");
        Serial.println(limSensorReading);
        Serial.print("Minimum time limit = ");
        Serial.println(timeLeft);
        Serial.print("Extended time limit = ");
        Serial.println(timeExtended);
        Serial.println("\nRedirecting to Home Page in 5 seconds");
        delay(5000);
        pcbPrintState=pcbInputState=pcbChoice=0;
        pcbPrintRepeat=0;
      }
    }
  }
}
