/*
 * Author: bradosev
 * Project: Passcode Generator
 * Date: Feb 17, 2018
 */
#include <SPI.h>
#include <LiquidCrystal.h>

// Definitions
int lcd_key = 0;
int adc_key_in = 0;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 

#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5 
#define indexMAX 3
#define indexMIN 0 
#define MENU 0
#define SELECT 1

String menuOptions[] = {"1.) Numeric", "2.) Alphabet", "3.) AlphaNumeric"};
int currentIndex=0;
long randNumber;
String numPassword;
int currentState;

char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
char alphabetNum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789$%#@!~*";

// found @ https://www.dfrobot.com/wiki/index.php/LCD_KeyPad_Shield_For_Arduino_SKU:_DFR0009
int read_LCD_buttons(){
  adc_key_in = analogRead(0);
  if (adc_key_in > 1000) return btnNONE;
  if (adc_key_in < 50) return btnRIGHT;
  if (adc_key_in < 250) return btnUP;
  if (adc_key_in < 450) return btnDOWN;
  if (adc_key_in < 650) return btnLEFT;
  if (adc_key_in < 850) return btnSELECT;
  return btnNONE;
}//end 

void setup() {
   // put your setup code here, to run once:
   lcd.begin(16, 2);               // start the library
   lcd.setCursor(0,0);             // set the LCD cursor   position 
   lcd.print("Password Menu");
   Serial.begin(9600);
   currentState = MENU;
   randomSeed(analogRead(0));
}//end setup
void numericPassword(int size){
  String test = "";
  for(int i = 0; i < size; i++){
    test.concat(random(10));
  }
  numPassword = test;
}//end 

void alphaPassword(int size){
  String test = "";
  for(int i = 0; i < size; i++){
    test.concat(alphabet[random(52)]);
  }
  numPassword = test;
}

void alphaNumericPassword(int size){
  String test = "";
   for(int i = 0; i < size; i++){
      test.concat(alphabetNum[random(68)]);
    }
  numPassword = test;
}
void printPassword(String password){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("New Password:");
  lcd.setCursor(0,1);
  lcd.print(password);
}

void printMenu(int index){  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Password Menu");
  lcd.setCursor(0,1);
  lcd.print(menuOptions[index]);
}//end


void loop() {
  lcd.setCursor(0,1);             // move to the begining of the second line
  lcd_key = read_LCD_buttons();   // read the buttons
  Serial.println(lcd_key);
  Serial.print("Current index: ");
  Serial.println(currentIndex);
  if (currentState == MENU){
  
    switch (lcd_key){
      case btnUP:
        if ((currentIndex - 1) >= indexMIN) currentIndex = currentIndex-1;
        printMenu(currentIndex);
        break;
      case btnDOWN:
        if ((currentIndex + 1) <= indexMAX-1) currentIndex++;
        printMenu(currentIndex);
        break;
      case btnSELECT:
        currentState = SELECT;
        lcd.clear();
        lcd.setCursor(0,0);
        switch(currentIndex){
          case 0:   
            //lcd.print("Numeric!");
            numericPassword(8);
            break;
          case 1:
            //lcd.print("Alpha!");
            alphaPassword(12);
            break;
          case 2:
            alphaNumericPassword(12);
            //lcd.print("AlphaNumeric!");
            break;
        }
        break;
      case btnNONE:
        printMenu(currentIndex);
        break;
      default:
        currentIndex = indexMIN;
        printMenu(currentIndex);
        break;
    }//end switch
    }//end 
    else if (currentState == SELECT){
      switch (lcd_key){
        case btnLEFT:
          currentState = MENU;
          break;
        default:
          switch(currentIndex){
            case 0:
              printPassword(numPassword);
              break;
            case 1:
              printPassword(numPassword);
              break;
             case 2:
              printPassword(numPassword);
              break;
          }//end case
      }//end
    }//end if
  delay(50);
}//end 



