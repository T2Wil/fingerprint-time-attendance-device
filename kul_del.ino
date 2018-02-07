#include <Keypad.h>
#include <Adafruit_Fingerprint.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd(8,9,10,11,12,13);//rs,enable,D4,D5,D6,D7

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};
byte rowPins[ROWS] = {4,5,6,7}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {A3,A2,A1}; //connect to the column pinouts of the kpd
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );



//fingerprint to serial communication
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);



Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);


uint8_t lectArray[30];//let the max number of users be 30
static uint8_t lectCounter=0;//has not to exceed 29 (0-30)

uint8_t studArray[30];//let the max number of users be 30
static uint8_t studCounter=1;//has not to exceed 29 (0-30)

uint8_t retakerArray[30];//let the max number of users be 30
static uint8_t retakerCounter=1;//has not to exceed 29 (0-30)

//global variables
uint8_t arrCode[10];//array to keep every input data in its array
char key;


//for fingerprint to eeprom
static uint8_t id=1;//takes all the users'fingerprint
  int i;//for iteration
int passID;//for attendance to check the ID inside the records

//student's records

struct students{
        
        //5 is the max number of students
        //static int studSize=5;
        //static int studCounter=1;
        int studID[30];//take the number of students registered consecutively
        int regNo[30];
        int pswd[30];//will take the value of the fingerprint scanned
        }stud;

//lecturer's records
struct lecturer{
        // is the max number of students
       
        int lessonID[10];
        int lectCode[10];//consecutive numbers of the registered lectures
        int pswd[10];//will take the value of the fingerprint scanned
        }lect;

//retaker's records

void setup() {
  lcd.begin(20,4);
  //lcd.clear();
  Serial.begin(9600);
  delay(100);
  Serial.println("\n\nGiants Fingerprint sensor ");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  //-----------------------FINGERPRINT SCANNER PRESENCE TEST------------------------
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
 //---------------------------------------------------------------------------------
 
 
homeScreen();
}

void loop() {

 //homeScreen();
 key = keypad.getKey();

 switch(key)
  {
    case '1'://for registration

          regOptions();//REGISTERING OPTIONS
          
         
            break;
      case '2'://for attendance
            lcd.clear();
            lcd.setCursor(2,0);
            lcd.print("no attend");
            delay(1000);
            lcd.clear();
            lcd.setCursor(0,0);
            break;
    default: 
          break;  
  } 
}


void regOptions(){
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("REGISTER AS:");
  lcd.setCursor(0,1);
  lcd.print("1.LECTURER");
  lcd.setCursor(0,2);
  lcd.print("2.STUDENT");
  lcd.setCursor(0,3);
  lcd.print("3.RETAKER");
  lcd.setCursor(0,4);

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

int regKey(char key2){

 lcd.clear();
 lcd.setCursor(0,0);
  switch(key2){
    case '1'://lecturer's registration
    lecturerReg();
    
    
    break;
    case '2'://student registration
      break;
    case '3'://retaker's registration
      break;
    default:
       break;
    
  }
}

void codeInput(int row)
{
  
  int i;//for  for loop iteration
  char key;//get input character from the keypad
  
   for(i=0;i<10;i++)
  {
    lcd.setCursor(i,row);//i for the cols and j for the rows

    char   key ='x';//trick the keypad... to let key be x
   
          while(key=='x')
            
            key=keypad.getKey();//let it be null
            
            while(key==NULL)//while key will be still NULL
            {
              key=keypad.getKey();//try to find a typed non NULL value
            }
            //check if is not ENTER
            //enterPressed(key,i);
            //pass the input character to check if it is '*' ENTER
            //and the index of the last to know where to stop while reading from the arrayCode

            
            arrCode[i]=key;//keep every input data in its array
            if(key=='*')
            {
              
            break;
            }
           lcd.print(key);
            
         
       
  } 
 //while ((i==10)&&(key!='*'))
 //key=keypad.getKey();
}



//pass it an input character from the keypad and
//the last character's index position to know where to stop reading from the array
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

void lecturerReg()
{
    lcd.clear();
    lcd.setCursor(0,0);
    
    lcd.print("lesson code:");
    lcd.setCursor(0,1);
    codeInput(1);//input codes not more than 10 digits on the second row

    //if enter is pressed
    lcd.setCursor(0,2);
    lcd.print("lecturer's code:");
    lcd.setCursor(0,3);
    codeInput(3);//input lecturer's code on the 4th line
   
    //if enter is pressed again
    
    //FINGERPRINT ENROLL
    getFingerprintEnroll(1,0,0);//(uint8_t lectbit,uint8_t studbit,uint8_t retakerbit)

    
    
    //if enter is pressed again go to home screen
      enterToHomeScreen();
    
    }

    void homeScreen()
    {
      lcd.clear();
       lcd.setCursor(2,0);  
       lcd.print("1.REGISTER");
       lcd.setCursor(2,1);
       lcd.print("2.ATTEND");
    }
  void studentReg()
  {
    lcd.clear();
    lcd.setCursor(0,0);
    
    lcd.print("Reg. No:");
    lcd.setCursor(0,1);
    codeInput(1);//input codes not more than 10 digits on the second row

    //if enter is pressed
    lcd.setCursor(0,2);
    lcd.print("FingerPrint:");
    lcd.setCursor(0,3);
    //fingerprint read
    //FINGERPRINT ENROLL
    getFingerprintEnroll(0,1,0);//(uint8_t lectbit,uint8_t studbit,uint8_t retakerbit)
    

    
    
    //if enter is pressed again go to home screen
      enterToHomeScreen();
  }

  void retakerReg()
  {
    lcd.clear();
    lcd.setCursor(0,0);
    
    lcd.print("lesson code:");
    lcd.setCursor(0,1);
    codeInput(1);//input codes not more than 10 digits on the second row

    //if enter is pressed
    lcd.setCursor(0,2);
    lcd.print("Reg. No:");
    lcd.setCursor(0,3);
    codeInput(3);//input lecturer's code on the 4th line
   
    //if enter is pressed again
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Fingerprint:");

    lcd.setCursor(0,1);
    
    
    //FINGERPRINT ENROLL
    getFingerprintEnroll(0,0,1);//(uint8_t lectbit,uint8_t studbit,uint8_t retakerbit)
    
    //if enter is pressed again go to home screen
      enterToHomeScreen();
  }

//---------------------------FINGER ENROL-------------------------------------------------------
uint8_t getFingerprintEnroll(uint8_t lectbit,uint8_t studbit,uint8_t retakerbit)
{
  uint8_t userCount;
  //if enrolling a lecturer........... increment to the next No
  if(lectbit==1 && studbit==0 && retakerbit==0)
  {
    lectArray[lectCounter]=lectCounter;
    userCount=lectCounter;
    lectCounter++;
  }
//if enrolling a student...increment to the next No
  if(lectbit==0 && studbit==1 && retakerbit==0)
  {
    studArray[studCounter]=studCounter;
    userCount=studCounter;
    studCounter++;
  }
//if enrolling a retaker..........increment to the next No
  if(lectbit==0 && studbit==0 && retakerbit==1)
  {
    retakerArray[retakerCounter]=retakerCounter;
    userCount=retakerCounter;
    retakerCounter++;
  }
  
  int p = -1;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("No:");

  if(userCount == 29)//if reached the max users
  {
    lcd.clear();
    lcd.print("users full");

    //return an error
    return -1;
  }
  id=userCount;//each id corresponds to the user

  userCount++;//increment for the next to register have next ID
  
  lcd.print(id);//print the user's ID
  lcd.setCursor(0,1);
  lcd.print("Place Finger");
  delay(2000);

  

  //the loop will stop iteration if FINGERPRINT_OK
  //remember p we assigned -1....so the iteration will stop if the p==FINGERPRINT_OK
  while (p != FINGERPRINT_OK)
  {
    //SCANNING THE FINGER
    p = finger.getImage();
    switch (p)
    {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      //lcd.clear();
      printId(id);
      lcd.setCursor(0,1);
      lcd.print("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No Finger");
      //lcd.clear();
      printId(id);
       lcd.setCursor(0,1);
      lcd.print("No Finger");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      //lcd.clear();
      printId(id);
       lcd.setCursor(0,1);
      lcd.print("Comm Error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      //lcd.clear();
      printId(id);
       lcd.setCursor(0,1);
      lcd.print("Imaging Error");
      break;
    default:
      Serial.println("Unknown error");
      // lcd.clear();
      printId(id);
       lcd.setCursor(0,1);
      lcd.print("Unknown Error");
      break;
    }
  }
  //IF FINGER SCANNING IS SUCCESSFULLY DONE THEN
  
  //CONVERTING THE FINGERPRINT IMAGE
  
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      //lcd.clear();
      printId(id);
      
       lcd.setCursor(0,1);
      lcd.print("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
       //lcd.clear();
       printId(id);
        lcd.setCursor(0,1);
       lcd.print("Image too messy");
      return p;//p is -1
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
            //lcd.clear();
            printId(id);
             lcd.setCursor(0,1);
      lcd.print("Comm Error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
           // lcd.clear();
           printId(id);
             lcd.setCursor(0,1);
      lcd.print("Feature Not Found");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
                 // lcd.clear();
                 printId(id);
                  lcd.setCursor(0,1);
      lcd.print("Feature Not Found");
      return p;
    default:
      Serial.println("Unknown error");
                  //lcd.clear();
                  printId(id);
                  lcd.setCursor(0,1);
      lcd.print("Unknown Error");
      return p;
  }
  //DO A DOUBLE CHECK
  
  Serial.println("Remove finger");
  //lcd.clear();
  printId(id);
   lcd.setCursor(0,1);
  lcd.print("Remove Finger");
  delay(500);
  p = 0;
  
  //if no finger detected then it will wait till the finger is pressed again
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); 
  //Serial.println(id);
  p = -1;
  //place the same finger
  Serial.println("Place same finger again");
   //lcd.clear();
   printId(id);
    lcd.setCursor(0,1);
      lcd.print("Place again");
      //lcd.setCursor(0,1);
      //lcd.print("   Again");
   delay(1000);//enough delay is required for better readings
   //wait for getting finger to scan
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      return;
    }
  }
  // OK success!
  //convert the scanned finger-> (2)
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  // OK converted!
  
  //creating a model
  Serial.print("Creating model for #");  
  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
  Serial.print("ID "); Serial.println(id);

  //store the model of the id
  p = finger.storeModel(id);
  
  if (p == FINGERPRINT_OK) {
    //-------------------------FINGER IS STORED-----------------------------------------
    Serial.println("REGISTERED");
   // lcd.clear();
   printId(id);
    lcd.setCursor(0,1);
    lcd.print("REGISTERED!");
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  }
  else {
    Serial.println("Unknown error");
    return p;
  }
}
//------------------------------END OF FINGERPRINT ENRLOMENT---------------------------------

void printId(uint8_t id)//output the user's number
{
        lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("No:");
  lcd.print(id);
}

void attend()
{
  //get the fingerprint ID
  int returnedFingerID=getFingerprintID();

  

  



  
  
}

//-------------------------------------READING THE FINGER TO MATCH---------------------------------
//---IF MATCHES IT RETURNS THE FINGER'S ID >=1 ---------
int getFingerprintID()
{
  //scan the finger
  uint8_t p = finger.getImage();
 
  //if image not well taken return Error
  if (p != FINGERPRINT_OK)
  return -1;
  
  //if the image is well taken then convert it
  p = finger.image2Tz();

  //if the image not well converted then return error
  if (p != FINGERPRINT_OK)
  return -1;

  //being well converted find if there can be a match
  p = finger.fingerFastSearch();

  //if no match...then the finger is not found and needs to try later and return an error
  if (p != FINGERPRINT_OK)
  {
   lcd.clear();
   lcd.print("Finger Not Found");
   lcd.setCursor(0,1);
   lcd.print("Try Later");
   delay(2000);
  return -1;
  }
  // found a match!..will only reach here if no error
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  return finger.fingerID;
}
 //--------------------------------------------------------------------------------------------

