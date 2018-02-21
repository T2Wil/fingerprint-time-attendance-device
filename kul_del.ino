#include <Keypad.h>
//FOR LECTURER AND STUDENT
#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal_PCF8574.h>
LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};
byte rowPins[ROWS] = {10,9,8,7}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {6,5,A0}; //connect to the column pinouts of the kpd
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );





//fingerprint to serial communication
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);//



Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void retakerReg();
uint8_t getFingerprintEnroll(uint8_t ,uint8_t ,uint8_t );

static uint8_t lectCounter=0;//has not to exceed 29 (0-30)
static uint8_t lessonCounter=0;
static uint8_t retakerLessons=0;

static uint8_t studCounter=0;//has not to exceed 29 (0-30)

static uint8_t retakerCounter=0;

uint8_t lectFinger;

uint8_t arrCode[10];//array to keep every input data in its array
char key;

//for fingerprint to eeprom
static uint8_t id=0;//takes all the users'fingerprint
  int i;//for iteration
    

//student's records

struct students{
        
     
        int studID[30];//take the number of students registered consecutively
        double regNo[30];
        int prints[30];//will take the value of the fingerprint scanned
        }stud;

//lecturer's records
struct lecturer{
        // is the max number of students
       
        double lessonID[10];
        double lectCode[10];//consecutive numbers of the registered lectures
        int prints[10];//will take the value of the fingerprint scanned
        uint8_t lectArray[30];//let the max number of users be 30
        }lect;

//retaker's records

struct retaker{
        
        double lessonID[10];
        int studID[30];//take the number of students registered consecutively
        double regNo[30];
        int prints[30];//will take the value of the fingerprint scanned
        }ret;


void setup() {
  lcd.setBacklight(50);//255
  lcd.begin(20,4);
  //lcd.clear();
  Serial.begin(9600);
  delay(100);
  ////Serial.println(F("\n\nGiants Fingerprint sensor "));//F() to avoid the string copying to the SRAM

  // set the data rate for the sensor serial port
  finger.begin(57600);
  //-----------------------FINGERPRINT SCANNER PRESENCE TEST------------------------
  if (finger.verifyPassword()) {
    ////Serial.println("Found fingerprint sensor!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(F("FINGERPRINT "));
    lcd.setCursor(0,1);
    lcd.print(F(" WORKING"));

    //if working then empty the fingerprints to restart over
   //finger.emptyDatabase();
    delay(1000);
  } else {
    ////Serial.println("Did not find fingerprint sensor :(");
    lcd.print(F("FINGERPRINT"));
    lcd.print(F(" NOT FOUND!"));
    delay(2000);
    while (1) { delay(1); }
  }
 //---------------------------------------------------------------------------------
 
homeScreen();
}

void loop()
{
  //REGISTER OR ATTEND
   key = keypad.getKey();
   
    switch(key)
  {
    case '1'://for registration

          regOptions();//REGISTERING OPTIONS
          
         
            break;
      case '2'://for attendance
            lcd.clear();
            lcd.setCursor(2,0);
            //lcd.print(F("no attend"));
            attendance();
            break;
    default: 
          break;  
  } 
}

void homeScreen()
    {
       lcd.clear();
       lcd.setCursor(2,0);  
       lcd.print(F("1.REGISTER"));
       lcd.setCursor(2,1);
       lcd.print(F("2.ATTEND"));
    }

void regOptions(){
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("REGISTER AS:"));
  
  lcd.setCursor(0,1);
  lcd.print(F("1.LECTURER"));
  
  lcd.setCursor(0,2);
  lcd.print(F("2.STUDENT"));

  lcd.setCursor(0,3);
  lcd.print(F("3.RETAKER"));

  key ='x';//trick the keypad... to let key be x
          
          while(key=='x'){
            
            key=keypad.getKey();//let it be null
            
            while(key==NULL)//while key will be still NULL
            {
              key=keypad.getKey();//try to find a typed non NULL value
            }
           
   switch(key){
       case '1'://lecturer's registration
        lecturerReg();
    
    
    break;
    case '2'://student registration
        studentReg();
      break;
    case '3'://retaker's registration
        retakerReg();
      break;
    default:
       break;
    
  }
          }
}


void lecturerReg()
{
    lcd.clear();
    lcd.setCursor(0,0);
    
    lcd.print(F("lesson code:"));
    lcd.setCursor(0,1);
    lect.lessonID[lessonCounter]=codeInput(1);//input codes not more than 10 digits on the second row
    
    //if enter is pressed
    lcd.setCursor(0,2);
    lcd.print(F("lecturer's code:"));
    lcd.setCursor(0,3);
    lect.lectCode[lectCounter]=codeInput(3);//input lecturer's code on the fourth line
   
    //if enter is pressed again
    
    //FINGERPRINT ENROLL
    getFingerprintEnroll(1,0,0);//(uint8_t lectbit,uint8_t studbit,uint8_t retakerbit)

    
    
    
    }

   void studentReg()
  {
    lcd.clear();
    lcd.setCursor(0,0);
    
    lcd.print(F("Reg. No:"));
    lcd.setCursor(0,1);
    stud.regNo[studCounter]=codeInput(1);//input codes not more than 10 digits on the second row

   
    //if enter is pressed
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(F("FingerPrint:"));
    //FINGERPRINT ENROLL
    getFingerprintEnroll(0,1,0);//(uint8_t lectbit,uint8_t studbit,uint8_t retakerbit)
  }

  void enterToHomeScreen()//check if ENTER and output the input characters 
{

              char   key ='x';//trick the keypad... to let key be x
   
          while(key=='x')
            
            key=keypad.getKey();//let it be null
            
            while(key==NULL)//while key will be still NULL
            {
              key=keypad.getKey();//try to find a typed non NULL value
            }
            if(key=='*')//if ENTER pressed
            {
              homeScreen();
            } 
}



double codeInput(int row)//will return the double codeInput
{
  
  int i;//for  for loop iteration
  char key;//get input character from the keypad
  double temp=0;
  char lastKey;

  i=0;
   while(i<10)//for(i=0;i<10;i++)
  {
    
    lcd.setCursor(i,row);//i for the cols and j for the rows

    char   key ='x';//trick the keypad... to let key be x
          while(key=='x')
            
            key=keypad.getKey();//let it be null
            
            while(key==NULL)//while key will be still NULL
            {
              key=keypad.getKey();//try to find a typed non NULL value
            }

            if(key=='*')//if ENTER IS PRESSED
            {
              
            break;
            }
            //delete key
            if(key=='#')//if DELETE key IS PRESSED
            {
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print(F("lesson code:"));
              lcd.setCursor(0,1);
              temp=temp/10;//return  back one column
              ////Serial.println("new temp value:");
              ////Serial.println(temp);
              if(lastKey=='0' || lastKey=='1' || lastKey=='2'|| lastKey=='3'|| lastKey=='4')
              {
                lcd.print((double)temp,0);
              }

              if(lastKey=='5' || lastKey=='6' || lastKey=='7'|| lastKey=='8'|| lastKey=='9')
              {
                temp=temp-1;//125=12 not 13; 456=45 not 46 AFTER DELETE
                lcd.print((double)temp,0);
              }
              i--;
             continue;
            }
             lastKey=key;//to prevent 1234=123,126=13 while deleting
           lcd.print(key);
            temp=temp*10 +(key-'0');
            
          i++;
            
         
       
  } 
    ////Serial.println("the value in temp:");
           ////Serial.println(temp);
           return temp;
}


//---------------------------FINGER CHECK -------------------------------------------------------
uint8_t attendance()
{
   //------------------------------------SCANNING THE FINGER----------------------------
 uint8_t p=1;
 uint8_t startAttend;
  
  lcd.print(F("press finger:"));
  lcd.setCursor(0,1);
 
  while(p!=FINGERPRINT_OK)
  {
    p = finger.getImage();
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
  return -1;
  //-------------------------------SEARCHING THE CORRESPONDING FINGER TEMPLATE-----------

  p = finger.fingerFastSearch();
  
  if (p != FINGERPRINT_OK)// if finger not found
  {
   //lcd.clear();
   lcd.print(F("Finger Not Found"));
   lcd.setCursor(0,2);
   lcd.print(F("TRY AGAIN"));
   delay(1000);
   homeScreen();
  return -1;
  }
 
  for(int i=0;i<=id;i++)//check from all users fingerprint IDs
  {
    //--------------------check if print belongs to the lecturer-------
    //----------------------if not NO CLASS---------------------------
    
    if((lect.prints[i]==finger.fingerID) && (stud.prints[i]!=finger.fingerID) && (ret.prints[i]!=finger.fingerID))
    {
      //START ATTENDANCE
      lectFinger=finger.fingerID;//variable to hold the ID of who started attendance
     startAttend=1;//lectFinger scans
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(F("ATTENDANCE STARTED"));
      Serial.println(F("ATTENDANCE STARTED"));
      lcd.setCursor(0,1);

      //WAITING FOR THE STUDENTS AND RETAKERS TO ATTEND OR THE SAME LECTURER TO CLOSE
      
      //infinite loop to allow students/retakers attendance to stop only if the same
      //lecturer pressed the fingerprint again
      
      while(startAttend==1)//if lecturer's finger was once scanned
      {
        //-----------------------------------PRESS THE FINGER----------
        delay(1000);//enough delay to prevents finger multiscanning
        p=1;
 
  BLOCK:
  lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(F("ATTENDANCE STARTED"));
      lcd.setCursor(0,1);
  lcd.print(F("press finger:"));
  Serial.println(F("press finger:"));
  //lcd.setCursor(0,1);
  
 
  while(p!=FINGERPRINT_OK)
  {
    p = finger.getImage();
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
  return -1;
   //-----------------------CHECK THE MATCHING FINGER TEMPLATE IN THE DATABASE-----
        
  p = finger.fingerFastSearch();
  
  if (p != FINGERPRINT_OK)// if finger not found
  {
   lcd.clear();
   lcd.print(F("Finger Not Found"));
   Serial.println(F("Finger Not Found"));
   
   lcd.setCursor(0,1);
   lcd.print(F("TRY AGAIN"));
   Serial.println(F("TRY AGAIN"));
   delay(1000);
  
 goto BLOCK;//if finger is not known then return to the BLOCK to read the next finger
  }
  //----------------------------------------------------------------------------------
        //---------------IF IS OF STUDENT OR A RETAKER THEN HAS ATTENDED SUCCESSFULLY----
  
  for(int j=0;j<=id;j++)//check from all users fingerprint IDs
  {
    //if the fingerID found belongs to that of student
    if(stud.prints[j]==finger.fingerID)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(F("ATTENDANCE STARTED"));
      lcd.setCursor(0,1);
      lcd.print(stud.regNo[j]);
      lcd.setCursor(0,2);
      lcd.print(F("ATTENDED"));
      delay(1000);
      break;//if student's fingerprint found then break
    }
    //if the fingerID found belongs to that of retaker
    else if(ret.prints[j]==finger.fingerID)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(F("ATTENDANCE STARTED"));
      lcd.setCursor(0,1);
      lcd.print(ret.regNo[j]);
      lcd.setCursor(0,2);
      lcd.print(F("ATTENDED"));
      delay(1000);
      break;//if retaker's fingerprint found then break

      
    }

      //IF IS OF THE SAME LECTURER ATTENDANCE CLOSED....break;
      
      if(lectFinger==finger.fingerID)
      {
      lcd.clear();  
      lcd.setCursor(0,1);
      lcd.print(F("ATTENDANCE CLOSED"));
       //Serial.println(F("ATTENDANCE CLOSED"));
       startAttend=0;
       delay(1000);
       break;
      }
      
       
          
        
        
  }  
  }

  //IF FINGER NOT BELONG TO A LECTURER
  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(F("NO CLASS"));
   
    
    delay(1000);
    homeScreen();
 
}





//---------------------------FINGER ENROL-------------------------------------------------------
uint8_t getFingerprintEnroll(uint8_t lectbit,uint8_t studbit,uint8_t retakerbit)
{
  lcd.clear();
  lcd.setCursor(4,0);
  uint8_t userCount;


  //------------------ANALYSE THE FINGERPRINT FIRST-----------------------------------
  int p = -1;
  lcd.print(F("PLACE FINGER:"));
  delay(1000);

  

  //the loop will stop iteration if FINGERPRINT_OK
  while (p != FINGERPRINT_OK)
  {
    //SCANNING THE FINGER
    p = finger.getImage();
    //delay(5000);
    switch (p)
    {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      //lcd.clear();
      printId(userCount);
      lcd.setCursor(0,1);
      lcd.print(F("Image taken"));
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println("No Finger");
      //lcd.clear();
      printId(userCount);
       lcd.setCursor(0,1);
      lcd.print(F("No Finger"));
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      //lcd.clear();
      printId(userCount);
       lcd.setCursor(0,1);
      lcd.print(F("Comm Error"));
      break;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      //lcd.clear();
      printId(userCount);
       lcd.setCursor(0,1);
      lcd.print(F("Imaging Error"));
      break;
    default:
      //Serial.println("Unknown error");
      // lcd.clear();
      printId(userCount);
       lcd.setCursor(0,1);
      lcd.print(F("Unknown Error"));
      //break;
    }
  }
  //IF FINGER SCANNING IS SUCCESSFULLY DONE THEN
  
  //CONVERTING THE FINGERPRINT IMAGE
  
  p = finger.image2Tz(1);//put 1st template in slot location 1 for verification
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      //lcd.clear();
      printId(userCount);
      
       lcd.setCursor(0,1);
      lcd.print(F("Image converted"));
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
       //lcd.clear();
       printId(userCount);
        
      return p;//p is -1
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
            //lcd.clear();
            printId(userCount);
             lcd.setCursor(0,1);
      lcd.print(F("Comm Error"));
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
           // lcd.clear();
           printId(userCount);
             lcd.setCursor(0,1);
      lcd.print(F("Feature Not Found"));
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
                 // lcd.clear();
                 printId(userCount);
                  lcd.setCursor(0,1);
      lcd.print(F("Feature Not Found"));
      return p;
    default:
      //Serial.println("Unknown error");
                  //lcd.clear();
                  printId(userCount);
                  lcd.setCursor(0,1);
      lcd.print(F("Unknown Error"));
      return p;
  }
  //DO A DOUBLE CHECK
  
  //Serial.println("Remove finger");
  //lcd.clear();
  printId(userCount);
   lcd.setCursor(0,1);
  lcd.print(F("Remove Finger"));
  delay(500);
  p = 0;
  
  //if no finger detected then it will wait till the finger is pressed again
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  //---------------------PLACE THE SAME FINGER--------------------------------------
  //Serial.println("Place same finger again");
   //lcd.clear();
   printId(userCount);
    lcd.setCursor(0,1);
      lcd.print(F("Place again"));
      //lcd.setCursor(0,1);
      //lcd.print(F("   Again"));
   
   //wait for getting finger to scan
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      ////Serial.println("Communication error");
      
    
      break;
    case FINGERPRINT_IMAGEFAIL:
      ////Serial.println("Imaging error");
      
   
      break;
    default:
      ////Serial.println("Unknown error");
      
   
      return;
    }
  }
  // OK success!
  //convert the scanned finger-> (2)
  p = finger.image2Tz(2);//place template in the location 2 for verification
  switch (p) {
    case FINGERPRINT_OK:
      ////Serial.println("Image converted");
     
      break;
    case FINGERPRINT_IMAGEMESS:
      ////Serial.println("Image too messy");
      lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    delay(500);
    homeScreen();
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      ////Serial.println("Communication error");
      lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    delay(500);
    homeScreen();
      return p;
    case FINGERPRINT_FEATUREFAIL:
      ////Serial.println("Could not find fingerprint features");
      lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    enterToHomeScreen();
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      ////Serial.println("Could not find fingerprint features");
      lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    delay(500);
    homeScreen();
      return p;
    default: 
    lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    delay(500);
    homeScreen();
      ////Serial.println("Unknown error");
      return p;
  }
  // OK converted!
  
  //take a two print feature template and create a model
 
  p = finger.createModel();//model created
  if (p == FINGERPRINT_OK) { 
    //////Serial.println("Prints matched!");
    lcd.print(F("matched"));
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    ////Serial.println("Communication error");
    lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    delay(500);
    homeScreen();
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    ////Serial.println("mismatch");
    lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    delay(500);
    homeScreen();
    return p;
  } else {
       lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    delay(500);
    homeScreen();
    return p;
  }
  
  //store the model of the id
  p = finger.storeModel(id);//store calculated model for later matching
  if (p == FINGERPRINT_OK) {
    //-------------------------FINGER IS STORED-----------------------------------------
    ////Serial.println("REGISTERED");
   // lcd.clear();
   printId(userCount);
    lcd.setCursor(0,1);
    lcd.print(F("REGISTERED!"));
    

    
    
    
  } 
  else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    ////Serial.println("Communication error");
    lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    
    return p;
  }
  else if (p == FINGERPRINT_BADLOCATION) {
    ////Serial.println("Could not store in that location");
    lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    
    return p;
  } 
  else if (p == FINGERPRINT_FLASHERR) {
    ////Serial.println("Error writing to flash");
    
    return p;
  }
  else {
    ////Serial.println("Unknown error");
    lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    
    return p;
  }
 //--------------------WHOSE FINGERPRINT IS THIS? LECTURER OR STUDENT---------------------
  //if enrolling a lecturer...
  if(lectbit==1 && studbit==0 && retakerbit==0)
  {
    
    lect.lectArray[lectCounter]=lectCounter;
    
    
    lect.prints[lectCounter]=id;//save the fingerprintID of the lecturer
   
    userCount=lectCounter;
    lectCounter++;
    
    id++;//increase the number of overall registered users(lecturer,student,retaker)
   
  }
//if enrolling a student...
  if(lectbit==0 && studbit==1 && retakerbit==0)
  {
    stud.studID[studCounter]=studCounter;
    stud.prints[studCounter]=id;//save the fingerprintID of a student
    
    userCount=studCounter;
    studCounter++;
    id++;// increase the number of overall registered users(lecturer,student,retaker)
     //return to the homeScreen
    
  }

  //if enrolling a retaker...
  if(lectbit==0 && studbit==0 && retakerbit==1)
  {
    ret.studID[retakerCounter]=retakerCounter;
    ret.prints[retakerCounter]=id;//save the fingerprintID of a student
    
    userCount=retakerCounter;
    retakerCounter++;
    id++;// increase the number of overall registered users(lecturer,student,retaker)
     //return to the homeScreen
    
  }

  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("No:"));

  if(userCount == 29)//if reached the max users
  {
    lcd.clear();
    lcd.print(F("users full"));

    //return an error
    return -1;
  }
  //id=userCount;//each id corresponds to the user

  //userCount++;//increment for the next to register have next ID
  
  lcd.print(userCount);//print the user's ID
  lcd.setCursor(0,1);

  delay(2000);
    homeScreen();
  
}
//---------------------------------------------------------------------------------------

//---------------------NUMBER OF ALL USERS---UNUSED-----------------------------------
void printId(uint8_t id)//output the user's number
{
        lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("No:"));
  lcd.print(id);
}

void retakerReg()
{
  lcd.clear();
    lcd.setCursor(0,0);
    
    lcd.print(F("lesson code:"));
    //lcd.setCursor(0,1);
    ret.lessonID[retakerLessons]=codeInput(1);//input codes not more than 10 digits on the second row
    
   
    //if enter is pressed
     lcd.setCursor(0,2);
    lcd.print(F("Reg. No:"));
   // lcd.setCursor(0,3);
    ret.regNo[retakerCounter]=codeInput(3);//input codes not more than 10 digits on the second row

    //FINGERPRINT ENROLL
    getFingerprintEnroll(0,0,1);//(uint8_t lectbit,uint8_t studbit,uint8_t retakerbit)
    
}

void deleteAllPrints()
{
  
}
