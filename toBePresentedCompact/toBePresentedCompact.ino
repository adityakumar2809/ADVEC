//PIN CONNECTIONS
int sensor = A4;
int forwardMotor=6;
int backwardMotor=5;

int lockState=0, limSensorReading=-5, timeLeft=5, timeExtended=3, firstRise=0, finalLock=0;
unsigned long tlStart=0, tlCurrent=0, teStart=0, teCurrent=0; 


//MENU 0 VARIABLES
int printState=0, inputState=0, printRepeat=0, inputRepeat=0;
char input[20], temp, id[20], password[20], user_id[20]="user@advec", user_password[20]="user@123", ss_id[20]="ss@advec", ss_password[20]="ss@123", pcb_id[20]="pcb@advec", pcb_password[20]="pcb@123";
int loginType=0, i=0, temp2=0, check=0, tookPass=0, menuStatus=0;

//MENU 1 VARIABLES
int userPrintRepeat=0, userPrintState=0, userInputState=0, userChoice=0, userSensorReading, userTimeExtendPerm=0;
char veh_reg[15]="PB11 A 1111";


//MENU 2 VARIABLES
int pcbPrintRepeat=0, pcbPrintState=0, pcbInputState=0, pcbChoice=0, pcbPerm=0, pcbChangeChoice=0;
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

void loop()
{
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
  else if(menuStatus==1)
  {
    userPrintStuff();
    userInputStuff();
  }
  else if(menuStatus==2)
  {
    pcbPrintStuff();
    pcbInputStuff();
  } 
  delay(500);
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
  if(pcbPerm==1 && userSensorReading>limSensorReading)
  {
    pcbPerm=0;
    finalLock=1;
    Serial.println("\nEmissions still above limits");
  }
  if(pcbPerm==1 && userSensorReading<limSensorReading)
  {
    pcbPerm=lockState=userTimeExtendPerm=firstRise=0;
    analogWrite(backwardMotor,255);
    delay(3000);
    analogWrite(backwardMotor,0);
    
  }
}


//MENU 0 STARTS
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
        Serial.println("2.Pollution Control Board Representative\n\n");
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

void checkCredentials()
{
  if(loginType==1)
  {
    if(strcmp(id,user_id)==0 && strcmp(password,user_password)==0)
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
    if(strcmp(id,pcb_id)==0 && strcmp(password,pcb_password)==0)
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
}


//MENU 1 STARTS
void userPrintStuff()
{
  if(userPrintRepeat==0)
  {
    if(userPrintState==0)
    {
      Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nWELCOME TO USER PORTAL");
      Serial.print("\nThe registration number of your vehicle is ");
      Serial.println(veh_reg);
      if(lockState==1)
      {
        Serial.println("Your vehicle is forced to come to a halt\n");
      }
      Serial.println("\nChoose an action to perform\n");
      Serial.println("1.Display current emission levels\n");
      Serial.println("2.Logout\n");
      if(lockState==1 && userTimeExtendPerm==0)
      {
        Serial.println("3.Request time\n");
      }
      
      userPrintState++;
    }
    else if(userPrintState==1)
    {
      if(userChoice==1)
      {
        Serial.print("The current emission level of your vehicle is ");
        userSensorReading=analogRead(sensor);
        Serial.println(userSensorReading);
        if(userSensorReading>limSensorReading)
        {
          Serial.print("It is higher than the limiting emission level which stands at ");
          Serial.println(limSensorReading);
          delay(200);
          if(lockState==1)
          {
            Serial.println("\nYour vehicle is currently forced to come to halt");
          }
          else
          {
            Serial.print("\nYour vehicle will be forced to come to halt in ");
            if(userTimeExtendPerm==0)
            {
              Serial.print(timeLeft-((millis()-tlStart)/1000.0));
            }
            else
            {
              Serial.print(timeExtended-((millis()-teStart)/1000.0));
            }
            Serial.println(" seconds\n");
          }
          Serial.println("Kindly move your vehicle to the nearest Service Center as soon as possible");
        }
        Serial.println("\nRedirecting to Home Page in 5 seconds");
        delay(5000);
        userPrintState=userInputState=userChoice=0;
        userPrintRepeat=-1;
      }
      else if(userChoice==3 && userTimeExtendPerm==0 && lockState==1)
      {
        Serial.print("\nYou have been granted an extended period of ");
        Serial.print(timeExtended);
        Serial.println(" seconds\n");
        //FUNCTIONALITY
        teStart=millis();
        analogWrite(backwardMotor,255);
        delay(3000);
        analogWrite(backwardMotor,0);
        lockState=0;
        //FUNCTIONALITY
        Serial.println("\nRedirecting to Home Page in 5 seconds");
        delay(2000);
        userPrintState=userInputState=userChoice=0;
        userPrintRepeat=-1;
        userTimeExtendPerm=1;
      }
      else
      {
        Serial.println("\nLogged out Successfully");
        Serial.println("\nRedirecting to Home Page");
        printState=inputState=printRepeat=inputRepeat=loginType=0;
        userPrintState=userInputState=userPrintRepeat=userChoice=menuStatus=0;
      }
      
    }
    userPrintRepeat++;
  }
}

void userInputStuff()
{
  if(userInputState==0)
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
      userInputState++;
      userPrintRepeat--;
      userChoice = atoi(input);
      temp2=0;
    }
  }
}


//MENU 2 STARTS
void pcbPrintStuff()
{
  if(pcbPrintRepeat==0)
  {
    if(pcbPrintState==0)
    {
      Serial.println("\n\n\n\n\nWELCOME TO POLLUTION CONTROL BOARD PORTAL");
      Serial.print("You have ");
      Serial.print(finalLock);
      Serial.println(" request pending");
      Serial.println("\nChoose an action to perform\n");
      Serial.println("\n1.Grant Permissions");
      Serial.println("2.Logout");
      pcbPrintState++;
    }
    else if(pcbPrintState==1)
    {
      if(pcbChoice==1)
      {
        if(finalLock==0)
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
          delay(100);
          Serial.println(" awaits for your permission");
          Serial.print("Its current emission levels are ");
          delay(100);
          Serial.println(userSensorReading);
          Serial.println("\nDo you wish to grant permission?");
          Serial.println("\n1.YES\n2.NO");
          pcbPrintState++;
        }
      }
      else if(pcbChoice==2)
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
          finalLock--;
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
  }
}
