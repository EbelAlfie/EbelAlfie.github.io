#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <RTClib.h>

//Pin declaration
const int ledPin = 33 ;
const int remoteIR = 12 ;
const int buzzerPin = 18 ;
const int triggerPin = 32 ;
const int echoPin = 35 ;
//Devices Object
LiquidCrystal_I2C lcd(0x27, 16, 2);
IRrecv irRecv(remoteIR) ;
RTC_DS1307 rtcKeep;

//Variables
char* SSID = "Wokwi-GUEST" ;
char* pass = "" ;
String url = "https://api.thingspeak.com/update?api_key=O32V7OXLQQ7O7F56" ;

decode_results result;
int initTime = 0 ;
int laterTime ; 
int reminderTime[2] ; 
int intensity = 255 ;
DateTime startStudy, stopStudy ;
//Boolean states
bool alarmState = false ;
bool ledOn = false ;

void setup() {
  Serial.begin(115200);
  //set Pins and start devices
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.init() ;
  irRecv.enableIRIn() ;

  while(!rtcKeep.begin()) {
    Serial.println("Starting RTC") ;
  }
  Serial.println("RTC module initialized") ;
  //Set RTC date time
  if(!rtcKeep.isrunning()) {
    rtcKeep.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  //Start wifi
  WiFi.begin(SSID, pass) ;
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print('.'); 
  }
  Serial.println() ;
  Serial.println("Connected!") ;
  Serial.print("IP : ");
  Serial.println(WiFi.localIP()) ;
  setAlarm() ; //set alarm for sleep
}

void printToLcd(int x, int y, String input) {
  lcd.clear() ;
  lcd.setCursor(x, y) ;
  lcd.print(input) ;
}

void setAlarm() {
  reminderTime[0] = 0 ;
  reminderTime[1] = 0 ;
  String timeDigit = "" ;
  int i = 0 ;
  
  while(i < 2) {//Hour and minute
    bool exit = 0 ;
    bool resetTime ;
    printToLcd(0,0, "Set alarm tidur") ;
    lcd.setCursor(0,1) ;

    if (i == 0) {
      lcd.print("Hour: ") ;
    }
    if (i == 1) {
      lcd.print("Minute: ") ;
    }
    lcd.setCursor(7,1) ;

    while(!exit) {
      resetTime = false ;
      if(irRecv.decode(&result)) {
        irRecv.resume() ;
        switch(result.value) {
          case 0x6B7832FF: //1
            timeDigit += '1' ;
            lcd.print('1') ;
            break ;
          case 0x3D9AE3F7: //2 
            timeDigit += '2' ;
            lcd.print('2') ;
            break ;
          case 0x31C5DD7B: //3
            timeDigit += '3' ;
            lcd.print('3') ;
            break ;
          case 0x45473C1B: //4
            timeDigit += '4' ;
            lcd.print('4') ;
            break ;
          case 0x63CBDADB: //5
            timeDigit += '5' ;
            lcd.print('5') ;
            break ;
          case 0x449E79F: //6
            timeDigit += '6' ;
            lcd.print('6') ;
            break ;
          case 0x32C6FDF7: //7
            timeDigit += '7' ;
            lcd.print('7') ;
            break ;
          case 0x3EC3FC1B: //8
            timeDigit += '8' ;
            lcd.print('8') ;
            break ;
          case 0x1BC0157B: // 9
            timeDigit += '9' ;
            lcd.print('9') ;
            break ;
          case 0xE39FAC1B: // 0
            timeDigit += '0' ;
            lcd.print('0') ;
            break ;
          case 0x4A2DB008: // c
            resetTime = true ;
            break;
          case 0x29494F40: //return
            return ;
            break;
          case 0xA16E9120: //enter
            exit = 1 ;
            break;
          default :
            break ;
        }
        if (resetTime == true) break ;
      }
    }
    if(!resetTime) {
      reminderTime[i] = timeDigit.toInt() ;
      //check Hour and minute validity  
      reminderTime[i] = reminderTime[i] == 24 && i == 0? 0 : reminderTime[i] ;
      if (i == 0 && reminderTime[i] > 23 || i == 1 && reminderTime[i] > 59) {
        printToLcd(0,0, "Invalid time!") ;
        delay(1000) ;
      }else {
        i++ ;
      }
    }else {
      i = 0 ;
      reminderTime[0] = 0 ;
      reminderTime[1] = 0 ;
    }
    timeDigit = "" ;
  }
  //mungkin simpan reminder time ke EEPROM RTC convert ke date
  printToLcd(0,0, "Time set!") ;
  lcd.setCursor(0,1) ;
  lcd.print(reminderTime[0]) ;
  lcd.print(":") ;
  lcd.print(reminderTime[1]) ;
  delay(2000) ;
}

//Detect person within 30 cm
bool isPersonPresent() {
  long duration ;
  float distance ;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(3);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (float)(duration/2) * 0.0343 ;
  return distance < 30? true : false ;// false artinya ga ada orang
}

bool manageLed() { //Kalau ada orang di meja belajar, pir deteksi orangnya dan  nyalain led
  bool status = isPersonPresent() ;
  if(status == true && ledOn == false) {
    analogWrite(ledPin, 255) ;
    ledOn = true ;
    startStudy = rtcKeep.now() ;
    return true ; //intensitas cahaya bisa diubah
  }
  if(status == false && ledOn == true){    
    analogWrite(ledPin, 0) ;
    ledOn = false ;
    stopStudy = rtcKeep.now() ;
    if (WiFi.status() == WL_CONNECTED) {
      httpSend(timeInterval(startStudy.minute(), stopStudy), 0) ;
    }
    return false ;
  }
  return false ;
}

void ledIntensity() {//Tekan plus untuk menambah intensitas cahaya
  if(irRecv.decode(&result)) {
    irRecv.resume() ;
    switch(result.value) {
      case 0x12209C7B:
        if (intensity + 17 <= 255) {
          printToLcd(0,0, "Light intensity: ") ;
          lcd.setCursor(0, 1) ;
          intensity += 17 ;
          lcd.print(intensity) ;
          analogWrite(ledPin, intensity) ;
        }else{
          lcd.clear() ;
          lcd.print("Reached maximum brightness") ;
        }
        break ;
      case 0x1C506100:
        if(intensity - 17 >= 0){
          printToLcd(0,0, "Light intensity: ") ;
          lcd.setCursor(0, 1) ;
          intensity -= 17 ;
          lcd.print(intensity) ;
          analogWrite(ledPin, intensity) ;
        }else{
          lcd.clear() ;
          lcd.print("Reached minimum brightness") ;
        }
        break ;
      default: break;
    }
  }
}

//Hitung interval waktu antara mulai/ alarm sampai stop
int timeInterval(int startMinute, DateTime currentTime) {
  int initHour = currentTime.hour();
  int initMinute = currentTime.minute() ;
  while (startMinute > initMinute) {
      --initHour;
      initMinute += 60;
   }
   return initMinute - startMinute ;
}

void loop() {
  DateTime currentTime = rtcKeep.now() ;
  laterTime = millis() ;
  if (laterTime - initTime > 1000) 
  {
    if (currentTime.hour() == reminderTime[0] && currentTime.minute() == reminderTime[1] && !alarmState) {
      printToLcd(0,0, "Waktunya tidur!") ;
      digitalWrite(buzzerPin, HIGH);
      alarmState = true ;
    }

    if (alarmState && irRecv.decode(&result) && result.value == 0xF0DB1AE0) { 
      //ketik tombol power off untuk mematikan alarm ketika menyala
      printToLcd(0,0, "Alarm mati") ;
      digitalWrite(buzzerPin, LOW) ;
      alarmState = false ;
      if (WiFi.status() == WL_CONNECTED) {
        httpSend(timeInterval(reminderTime[1], currentTime), 1) ;
      }
    }

    if (manageLed()) {
      ledIntensity() ;
    }

    initTime = millis() ;
  }
}

void httpSend(int variable, bool code) {//1 == send time 0 == send study frequency
  HTTPClient httpClient ;
  String completeWrite ;
  if (code) {
    completeWrite = url + "&field1=" + variable ;
  }else{
    completeWrite = url + "&field2=" + variable ;
  }
  Serial.println(completeWrite) ;
  httpClient.begin(completeWrite.c_str()) ;
  
  int requestCode = httpClient.GET() ;
  if (requestCode > 0) {
    Serial.println("Success to writing data!") ;
  }else {
    Serial.println("Failed to write data!") ;
  }
  Serial.println(requestCode) ;
  httpClient.end() ;
}
