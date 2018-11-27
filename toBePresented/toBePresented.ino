//PIN CONNECTIONS
int sensor = A4;
int forwardMotor=6;
int backwardMotor=5;

int lockState=0, limSensorReading=-1024, timeLeft=90, timeExtended=100, firstRise=0, finalLock=0;
unsigned long tlStart=0, tlCurrent=0, teStart=0, teCurrent=0; 


//MENU 0 VARIABLES
int printState=0, inputState=0, printRepeat=0, inputRepeat=0;
char input[20], temp, id[20], password[20], user_id[20]="user@advec", user_password[20]="user@123", ss_id[20]="ss@advec", ss_password[20]="ss@123", pcb_id[20]="pcb@advec", pcb_password[20]="pcb@123";
int loginType=0, i=0, temp2=0, check=0, tookPass=0, menuStatus=0;

//MENU 1 VARIABLES
int userPrintRepeat=0, userPrintState=0, userInputState=0, userChoice=0, userSensorReading, userTimeExtendPerm=0;
char veh_reg[15]="PB11 A 1111";

//MENU 2 VARIABLES
int ssPrintRepeat=0, ssPrintState=0, ssInputState=0, ssChoice=0, ssDue=0;
char ss_reg[15]="PNB0025875", ss_veh_reg[15];

//MENU 3 VARIABLES
int ssPrintRepeat=0, pcbPrintState=0, pcbInputState=0, pcbChoice=0;

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
    ssPrintStuff();
    ssInputStuff();
  }
  else if(menuStatus==3)
  {
    pcbPrintStuff();
    pcbInputStuff();
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
          else if(loginType!=NULL)
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
    if(strcmp(id,ss_id)==0 && strcmp(password,ss_password)==0)
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
    if(strcmp(id,pcb_id)==0 && strcmp(password,pcb_password)==0)
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
          Serial.print("The current emission level of your vehicle is higher than the limiting emission level which stands at ");
          Serial.println(limSensorReading);
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

}


//MENU 2 STARTS

void ssPrintStuff()
{
  if(ssPrintRepeat==0)
  {
    if(ssPrintState==0)
    {
      Serial.println("\n\n\n\n\n\n\n\n\nWELCOME TO SERVICE CENTRE PORTAL");
      Serial.print("The registration number of the Service Centre is ");
      Serial.println(ss_reg);
      Serial.println("\nChoose an action to perform\n");
      Serial.println("\n1.Add a new entry");
      Serial.println("\n2.Logout");
      ssPrintState++;
    }
    else if(ssPrintState==1)
    {
      if(ssChoice==1)
      {
        Serial.println("\n\nEnter the registration number of the vehicle being repaired");
        ssPrintState++;
      }
      else
      {
        Serial.println("\nLogged out Successfully");
        Serial.println("\nRedirecting to Home Page");
        printState=inputState=printRepeat=inputRepeat=loginType=0;
        ssPrintState=ssInputState=ssPrintRepeat=ssChoice=menuStatus=0;
      }
    }
    else if(ssPrintState==2)
    {
       Serial.print("\n\n\n\nThe registration number of the vehicle is ");
       Serial.println(ss_veh_reg);
       if(finalLock==0)
        {
          Serial.println("\n\nVehicle repaired successfully");
        }
        else
        {
          Serial.println("\n\nVehicle repaired too late. Requires PCB examination before running");
          ssDue++;
        }
        Serial.println("\nRedirecting to Home Page in 5 seconds");
        delay(5000);
        ssPrintState=ssInputState=ssChoice=0;
        ssPrintRepeat=-1;

    }

    ssPrintRepeat++;
  }
}


void ssInputStuff()
{
  if(ssInputState==0)
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
      ssInputState++;
      ssPrintRepeat--;
      ssChoice = atoi(input);
      temp2=0;
    }
  }

  if(ssInputState==1)
  {
     while(Serial.available()>0)
    {
      temp=Serial.read();
      if(temp!='\r' && i<19)
      {
        ss_veh_reg[i++]=temp;
      }
      temp2=1;
      delay(1);
    }
    if(temp2==1)
    {
      ss_veh_reg[i]='\0';
      i=0;
      ssInputState++;
      ssPrintRepeat--;
      temp2=0;
      if(strcmp(veh_reg,ss_veh_reg)!=0)
      {
        Serial.print("\nNo records found for the vehicle ");
        Serial.println(ss_veh_reg);
        Serial.println("\nKindly recheck the entered field");
        Serial.println("\nRedirecting to Home Page in 5 seconds");
        delay(5000);
        ssPrintState=ssInputState=ssChoice=0;
        ssPrintRepeat=0;
      }
    }
  }
}
