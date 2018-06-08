/*
 * Flip Dot Display
 * Description: Smart Flip Dot Clock
 * Author: Mariano Elia
 * Date: July 2018
 */

#include <vector>
#include <string>
#include "Adafruit_Si7021.h"
Adafruit_Si7021 Si7021 = Adafruit_Si7021();
double humidityDouble, tempDouble;
int humidityInt, tempInt;

int scrollDelay = 80;

const uint8_t startTransmission[3] = {0x80, 0x83, 0b00000001};
const uint8_t endTransmission[1] = {0x8F};


//ASCII integer from 65 to 90
uint8_t alphabetUpperCase[][5]{ //Most are 4 long, but some are 3 and 5 long
  {0b1111110, 0b0001001, 0b0001001, 0b1111110, 0b0000000}, //A
  {0b1111111, 0b1001001, 0b1001001, 0b0110110, 0b0000000}, //B
  {0b0111110, 0b1000001, 0b1000001, 0b0100010, 0b0000000}, //C
  {0b1111111, 0b1000001, 0b1000001, 0b0111110, 0b0000000}, //D
  {0b1111111, 0b1001001, 0b1001001, 0b1000001, 0b0000000}, //E
  {0b1111111, 0b0001001, 0b0001001, 0b0000001, 0b0000000}, //F
  {0b0111110, 0b1000001, 0b1001001, 0b0111010, 0b0000000}, //G
  {0b1111111, 0b0001000, 0b0001000, 0b1111111, 0b0000000}, //H
  {0b1000001, 0b1111111, 0b1000001, 0b0000000, 0b0000000}, //I (3 long)
  {0b0110000, 0b1000001, 0b1000001, 0b0111111, 0b0000000}, //J
  {0b1111111, 0b0001100, 0b0010010, 0b1100001, 0b0000000}, //K
  {0b1111111, 0b1000000, 0b1000000, 0b1000000, 0b0000000}, //L
  {0b1111111, 0b0000010, 0b0000100, 0b0000010, 0b1111111}, //M (5 long)
  {0b1111111, 0b0000100, 0b0010000, 0b1111111, 0b0000000}, //N
  {0b0111110, 0b1000001, 0b1000001, 0b0111110, 0b0000000}, //O
  {0b1111111, 0b0001001, 0b0001001, 0b0000110, 0b0000000}, //P
  {0b0111110, 0b1010001, 0b0100001, 0b1011110, 0b0000000}, //Q
  {0b1111111, 0b0011001, 0b0101001, 0b1000110, 0b0000000}, //R
  {0b0100110, 0b1001001, 0b1010001, 0b0100010, 0b0000000}, //S
  {0b0000001, 0b0000001, 0b1111111, 0b0000001, 0b0000001}, //T (5 long)
  {0b0111111, 0b1000000, 0b1000000, 0b0111111, 0b0000000}, //U
  {0b0001111, 0b0110000, 0b1000000, 0b0001111, 0b0000000}, //V
  {0b1111111, 0b0100000, 0b0010000, 0b0100000, 0b1111111}, //W (5 long)
  {0b1100011, 0b0011100, 0b0011100, 0b1100011, 0b0000000}, //X
  {0b0000111, 0b0001000, 0b1110000, 0b0001000, 0b0000111}, //Y (5 long)
  {0b1100001, 0b1010001, 0b1001001, 0b1000101, 0b1000011}  //Z (5 long)
};

//Lower case alphabet, ASCII integer from 97 to 122
uint8_t alphabetLowerCase[][5]{ //Most are 4 long, but some are 3/5 long
  {0b1110100, 0b1010010, 0b1010010, 0b1111100}, //A
  {0b1111110, 0b1010000, 0b1010000, 0b1110000}, //B
  {0b0111000, 0b1000100, 0b1000100, 0b0101000}, //C
  {0b1110000, 0b1010000, 0b1010000, 0b1111110}, //D
  {0b0111000, 0b1010100, 0b1010100, 0b0011000}, //E
  {0b0001000, 0b1111100, 0b0001010, 0b0000000}, //F
  {0b1011100, 0b1010100, 0b1010100, 0b0111100}, //G
  {0b1111110, 0b0001000, 0b0001000, 0b1110000}, //H
  {0b0000000, 0b1111010, 0b0000000}, //I (3 long)
  {0b0100000, 0b1000000, 0b1000000, 0b0111010}, //J
  {0b1111110, 0b0011000, 0b0100100, 0b1000010}, //K
  {0b0000000, 0b1000010, 0b1111110, 0b1000000}, //L
  {0b1111100, 0b0000100, 0b1111000, 0b0000100, 0b1111100}, //M (5 long)
  {0b1111100, 0b0000100, 0b0000100, 0b1111000}, //N
  {0b0111000, 0b1000100, 0b1000100, 0b0111000}, //O
  {0b1111110, 0b0001010, 0b0001010, 0b0001100}, //P
  {0b0001110, 0b0010010, 0b0010010, 0b1111110}, //Q
  {0b1111000, 0b0000100, 0b0000100, 0b0001000}, //R
  {0b1001000, 0b1010100, 0b1010100, 0b0100100}, //S
  {0b0001000, 0b0111100, 0b1001000, 0b0000000}, //T (3 long)
  {0b0111100, 0b1000000, 0b1000000, 0b1111100}, //U
  {0b0011100, 0b0100000, 0b1000000, 0b0111100}, //V
  {0b1111100, 0b0100000, 0b0010000, 0b0100000, 0b1111100}, //W (5 long)
  {0b1000100, 0b0101000, 0b0010000, 0b0101000, 0b1000100}, //X (5 long)
  {0b1001100, 0b1010000, 0b1010000, 0b0111100}, //Y
  {0b1100100, 0b1010100, 0b1001100}  //Z (3 long)
 };

//ASCII integer from 48 to 57
 uint8_t numbers[][4]{
   {0b0111110, 0b1000001, 0b1000001, 0b0111110}, // 0
   {0b0000100, 0b0000010, 0b1111111, 0b0000000}, // 1 (3 long)
   {0b1100010, 0b1010001, 0b1001001, 0b1000110}, // 2
   {0b1000001, 0b1001001, 0b1001001, 0b0110110}, // 3
   {0b0011000, 0b0010100, 0b0010010, 0b1111111}, // 4
   {0b1001111, 0b1001001, 0b1001001, 0b0110001}, // 5
   {0b0111110, 0b1001001, 0b1001001, 0b0110010}, // 6
   {0b0000001, 0b1100001, 0b0011001, 0b0000111}, // 7
   {0b0110110, 0b1001001, 0b1001001, 0b0110110}, // 8
   {0b0100110, 0b1001001, 0b1001001, 0b0111110}  // 9
 };

//ASCII integer 32 to 47, 58 to 64, 91 to 96, and 123 to 126
uint8_t specialCharacters[][5]{
//  {0b0000000, 0b0000000, 0b0000000},    // space (32)
  {0b0000000, 0b1011111, 0b0000000},    // ! (33)
  {0b0000011, 0b0000000, 0b0000011},    // " (34)
  {0b0010100, 0b1111111, 0b0010100, 0b1111111, 0b0010100},   // # (35)
  {0b0101110, 0b0101010, 0b1101011, 0b0101010, 0b0111010},   //$ (36)
  {0b0100011, 0b0010011, 0b0001000, 0b1100100, 0b1100010},   //% (37)
  {0b0110110, 0b1001001, 0b1010110, 0b0100000, 0b1010000},   //& (38)
  {0b0000000, 0b0000001, 0b0000000},   // ' (39)
  {0b0011100, 0b0100010, 0b1000001},   // ( (40)
  {0b1000001, 0b0100010, 0b0011100},   // ) (41)
  {0b0010100, 0b0001100, 0b0000111, 0b0001100, 0b0010100},   // * (42)
  {0b0001000, 0b0011100, 0b0001000},   // + (43)
  {0b1000000, 0b1100000, 0b0000000},   // , (44)
  {0b0001000, 0b0001000, 0b0001000},   // - (45)
  {0b0000000, 0b1000000, 0b0000000},   // . (46)
  {0b1100000, 0b0011000, 0b0001100, 0b0000011},   // / (47)

  {0b0100100},   // : (58)
  {0b1000000, 0b0101000, 0b0000000},   // ; (59)
  {0b0001000, 0b0010100, 0b0100010},   // < (60)
  {0b0101000, 0b0101000, 0b0101000},   // = (61)
  {0b0100010, 0b0010100, 0b0001000},   // > (62)
  {0b0000010, 0b1011001, 0b0001001, 0b0000110},   // ? (63)
  {0b0011100, 0b0100010, 0b1011101, 0b1010101, 0b0011110},   // @ (64)

  {0b1111111, 0b1000001, 0b1000001},   // [ (91)
  {0b0000011, 0b0001100, 0b0011000, 0b1100000},   // \ (92)
  {0b1000001, 0b1000001, 0b1111111},   // ] (93)
  {0b0000010, 0b0000001, 0b0000010},   // ^ (94)
  {0b1000000, 0b1000000, 0b1000000},   // _ (95)
  {0b0000000, 0b0000001, 0b0000000},   // ' (96)

  {0b0001000, 0b0110110, 0b1000001},   // { (123)
  {0b0000000, 0b1111111, 0b0000000},   // | (124)
  {0b1000001, 0b0110110, 0b0001000},   // } (125)
  {0b0010000, 0b0001000, 0b0010000, 0b0000100},   // ~ (126)
};



uint8_t immediateBuffer[28] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t futureBuffer[28] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
std::vector <uint8_t> fullBuffer;
retained std::vector <char> modesArray;

// Cloud functions
int handleModes(String command);
int showStaticTxtTxt(String message);
int scroll(String message);
int wiggleText(String message);


//Software Timers
Timer tempTimer(2000, getTemperature);

void setup() {
  Serial1.begin(57600);
  Serial.begin(9600);
  tempTimer.start();
  updateDisplay(); //clear the display
  initialTestSequence();
  Serial.print("hello world!");

  Si7021.begin();

 // Cloud functions must return int and take one String
//Particle.function("Modes", handleModes);
Particle.function("Scrolling", scroll);
Particle.function("Static", showStaticTxt);
Particle.function("Wiggle", wiggleText);
Particle.variable("Temp", tempDouble);
Particle.variable("Humidity", humidityDouble);



}

int i = 0;
void loop() {
 displayTime(12, -4);
 getTemperature();
 getHumidity();
 delay(10000);
 displayTemp("quick");
 delay(1000);

 /*
  delay(1000);
t = getTemperature();
displayTemp("static");

//delay(1000);
showStaticTxt("ghil");
delay(1000);
showStaticTxt("asdfasldf");
//delay(3000);
//scroll("ABCDEFGHJKIL");
//delay(1000);
*/
/*
  if (i>=150){
    h = getHumidity();
    Serial.println(h);
    t = getTemperature();
    int temp = (int)t;
    Serial.println("displayScrollingTemp");
//  int na = scroll()
  displayTemp("static");
   i=0;
  }
*/
//  i++;
 //scroll("abcd");
//  scroll("efgh");
//convertStringToByteArray("Hello");
/*
String time = "The current time is ";
time.concat(1054);
scroll(time);
Serial.println("Finished");
*/


} //End Loop


void displayTemp(String displayType){
  getTemperature(); //put
  String tempString = String(tempInt);

  if(displayType=="scrolling"){
  String tempString = "The temp is ";

  tempString.concat(tempString);
  Serial.print("The string to be printed is: ");
  Serial.println(tempString);
  int na = scroll(tempString);
}
else if(displayType=="quick"){
  String tempString = "T:";
  tempString.concat(tempInt);
  showStaticTxt(tempString);
  delay(2000);
}
else {
  showStaticTxt("The");
  delay(600);
  showStaticTxt("Temp");
  delay(600);
  showStaticTxt("IS");
  delay(600);
  showStaticTxt(tempString);
  delay(1200);
}
}

double getTemperature(){
  tempDouble = Si7021.readTemperature();
  tempInt = int (tempDouble);
  return tempDouble;
}

double getHumidity(){
  humidityDouble = Si7021.readHumidity();
  humidityInt = int (humidityDouble);
  return humidityDouble;
}


int handleModes(String command){
//  String commandCopy = command;
int i = command.length();
Serial.println(command);
int hello = scroll((String)command);
//Serial.print(i);
  while(i>=0){
  //  Serial.print("i: ");
  //  Serial.println(i);
    char currentChar = command.charAt(i);
    if(currentChar == ','){

    }
    else{
      modesArray.push_back(currentChar);
        Serial.println(currentChar);
    }
  //  Serial.println(currentChar);
  --i;
} //end while loop


//  Serial.println(modesArray)

  return 5;

}



/*
 * Display the time with a colon in the center
 * hoursType: 24 or 12
 * timeZone: offset from
 */
void displayTime(int hoursType, int timeZone){

  int currentTime = getTime(hoursType,timeZone);
//  clearImmediateBuffer();
  String stringCurrentTime = (String)currentTime;
  int stringLength = stringCurrentTime.length();


//if time is 4 digits such as 1245, add colon to make 12:45
if (stringLength == 4){
    char firstChar = stringCurrentTime.charAt(0);
    char secondChar = stringCurrentTime.charAt(1);
    char thirdChar = stringCurrentTime.charAt(2);
    char fourthChar = stringCurrentTime.charAt(3);
  stringCurrentTime = (String)firstChar + (String)secondChar + ":" + (String)thirdChar + (String)fourthChar;
}

//if time is 3 digits such as 145, add colon to make 1:45
else if (stringLength == 3){
  char firstChar = stringCurrentTime.charAt(0);
  char secondChar = stringCurrentTime.charAt(1);
  char thirdChar = stringCurrentTime.charAt(2);
    stringCurrentTime = (String)firstChar + ":" + (String)secondChar + (String)thirdChar;
  }
int notused = showStaticTxt(stringCurrentTime);
} //end displayTime()

void drawPixel(int row, int col){
  if(row < 0 || row > 6 || col < 0 || col > 27)
  return;
  immediateBuffer[col] |= 1 << (row);
} //end drawPixel

void clearPixel(int row, int col){
  if(row < 0 || row > 6 || col < 0 || col > 27)
  return;
  immediateBuffer[col] &= ~(1 << (row));
} //end clearPixel

/*
 * Draw all pixels, and then erase all.
 * Used to confirm that the photon updated
 */
void initialTestSequence(){
  for(int i = 0; i<28; i++){
    for(int j = 0; j < 7; j++){
      drawPixel(j, i);
      Serial.print("Row: ");
      Serial.print(j);
      Serial.print(" Column: ");
      Serial.print(i);
      Serial.println();
    }
    updateDisplay();
    delay(20);
  }
  for(int i = 0; i < 28; i++){
    for(int j = 0; j < 7; j++){
      clearPixel(j,i);
    }
    updateDisplay();
    delay(20);
  }
}

/*
 * Display a static message
 */
int showStaticTxt(String message){

  fullBuffer.clear();
  clearImmediateBuffer();
  convertStringToByteArray(message,0,0); //put message in fullBuffer vector
  int messageLength = fullBuffer.size();
  Serial.print("Message Length: ");
  Serial.println(messageLength);

  if (messageLength<=0)
  return -1;
  else if (messageLength>=28 && messageLength<=45){
    wiggleText(message);
    return 0;
  }
  else if (messageLength>=46) {
  scroll(message);
  return 0;
}
for(int arrIndex = 0; arrIndex < messageLength; arrIndex++){
    immediateBuffer[arrIndex] = fullBuffer[arrIndex];
}
int middle = (28 - messageLength) / 2;
Serial.print("Middle: ");
Serial.println(middle);
shiftBuffer(true, middle);
updateDisplay();
return 1;
}


/*
 * Shift the immediateBuffer left or right.
 * Left: 0, Right: 1
 *
 */
void shiftBuffer(bool direction, int increments){
if (direction == true){ //right shift
  for(int j = 0; j<increments; j++){
  int x = immediateBuffer[28-1];
     for (int i = 28-1; i > 0; i--)
        immediateBuffer[i] = immediateBuffer[i-1];
     immediateBuffer[0] = x;
}
}
else{ //left shift

}

}

int wiggleText(String message){
  fullBuffer.clear();
  clearImmediateBuffer();
  convertStringToByteArray(message,0,0); //put message in fullBuffer vector
//If the message isn't 27 cols
if (fullBuffer.size() < 27){
  int difference = 28-fullBuffer.size();
  fullBuffer.clear();
  clearImmediateBuffer();
  convertStringToByteArray(message, difference, difference);
//Scroll right
int arrIndex;
  for(arrIndex = difference; arrIndex >=0 ; arrIndex--){
    for (int j = 0; j < 28; j++){
      immediateBuffer[j] = fullBuffer[j+arrIndex];
    }
    updateDisplay();
    if(arrIndex == difference){
      delay(500);
    }
    delay(scrollDelay);
  }
  delay(600);
//Scroll left
  for(arrIndex = 0; arrIndex <= difference; arrIndex++){
    for (int j = 0; j < 28; j++){
      immediateBuffer[j] = fullBuffer[j+arrIndex];
    }
    updateDisplay();

    delay(scrollDelay);
  }
  delay(600);
  return 1;
}
else{
int arrIndex;
  for(arrIndex = 0; arrIndex < fullBuffer.size()-27; arrIndex++){
    for (int j = 0; j < 28; j++){
      immediateBuffer[j] = fullBuffer[j+arrIndex];
    }
    Serial.println();
    Serial.print("Index: ");
    Serial.print(arrIndex);
    updateDisplay();
    if(arrIndex == 0){
      delay(800);
    }
    delay(scrollDelay);
  }
  delay(600);
//Scrolled to the left, now scroll to the right to 'wiggle'
arrIndex--;
for(arrIndex; arrIndex >= 0; arrIndex--){
  for (int j = 0; j < 28; j++){
    immediateBuffer[j] = fullBuffer[j+arrIndex];
  }
  updateDisplay();
  delay(scrollDelay);
}
delay(400);
  return 1;
}
return 0;
}

int scroll(String message){
fullBuffer.clear();
clearImmediateBuffer();
convertStringToByteArray(message,28,28); //Put the message into a converted vector fullBuffer

for(int arrIndex = 0; arrIndex < fullBuffer.size()-27; arrIndex++){
  for (int j = 0; j < 28; j++){
    immediateBuffer[j] = fullBuffer[j+arrIndex];
  }
  Serial.println();
  Serial.print("Index: ");
  Serial.print(arrIndex);
  updateDisplay();
  delay(scrollDelay);
}
 return 5;
}


/*
 * A string is converted into the byte vector fullBuffer
 */

 void convertStringToByteArray(String sentence, int frontPadding, int backPadding){
 //sentence = sentence.toUpperCase(); //convert the
 fullBuffer.clear();
 Serial.println();
 Serial.print("UpperCaseSentence: ");
 Serial.println(sentence);
 sentence.trim(); //remove leading and trailing whitespace
 int strLength = sentence.length();
 //int arrayLength = strLength * 5 + strLength + 28*2; //5 columns per character, plus spaces


 //int stringIndex = 0;
 //The display is
 //ASCII A is 66, Z is at 90. 0 starts at 48, to 9 at 57. a is 97, z is 122.

 for (int i = 0; i < frontPadding; i++){
   fullBuffer.push_back(0b0000000); //28 spaces at end
 }

 //while loop iterates through the string
 int i = 0;
 while(i<=strLength-1){
int numCharacterCols = 5; //character is 2,3,4, or 5 columns long, default 5
 Serial.println();
 int j = 0;
 char currentChar = sentence.charAt(i);
 int intChar = (int)currentChar;
Serial.print("The intChar is: ");
Serial.println(intChar);

 //A through Z (uppercase)
 if((intChar<=90 && intChar>=65)){
   int row = intChar - 65; // Offset the array index to start at 0
   Serial.print("current alphabet at char ");
   Serial.print(i);
   Serial.print(" is: ");
   Serial.print(row);
   if (row == 12 || row == 19 || row == 21 || row == 22 || row == 24 || row == 25)
     numCharacterCols = 5;
   else
    numCharacterCols = 4;

 while(j<numCharacterCols){
 fullBuffer.push_back(alphabetUpperCase[row][j]);
 j++;
 }
 }


//a to z (lowercase)
 else if ((intChar<=122 && intChar>=97)){
int row = intChar - 97; // Offset the array index to start at 0
numCharacterCols = 4;

if (row == 12 || row == 22 || row == 23)
numCharacterCols = 5;
else if (row == 25 || row == 8)
numCharacterCols = 3;

while(j<numCharacterCols){
  fullBuffer.push_back(alphabetLowerCase[row][j]);
  j++;
}
}

 //0 to 9
 else if (intChar<=57 && intChar>=48){
 int row = intChar - 48; // Offset the array index to start at 0
 numCharacterCols = 4;
 Serial.print("current number: ");
 Serial.print(row);
 while (j<numCharacterCols){
 fullBuffer.push_back(numbers[row][j]);
 j++;
 }
 }
 //Space
 else if (intChar == 32){
   Serial.print("Space");
   numCharacterCols = 4;
   while(j<numCharacterCols){
 fullBuffer.push_back(0b0000000);
 j++;
 }
 }

 //Special Characters 1 ASCII 32 to 47
 else if (intChar <= 47 && intChar >=33){
 int row = intChar - 33; // Offset the array index to start at 0
 if (row == 2 || row == 3|| row == 4 || row == 5 || row == 9)
  numCharacterCols = 5;
  else if (row == 4 || row == 14)
  numCharacterCols = 4;
 else
  numCharacterCols = 3;

   while(j<numCharacterCols){
     fullBuffer.push_back(specialCharacters[row][j]);
     j++;
   }
 }
 //Special Characters 2 ASCII 58 to 64
 else if (intChar >=58 && intChar <= 64){
   int row = intChar - 58 + 15; // Offset the array index to start at 0
   Serial.print("Row number: ");
   Serial.println(row);
   if (row == 15)
   numCharacterCols = 1;
   else if (row == 20)
   numCharacterCols = 4;
   else if (row == 21)
   numCharacterCols = 5;
   else
   numCharacterCols = 3;
while(j<numCharacterCols){
  fullBuffer.push_back(specialCharacters[row][j]);
  j++;
}

}

//Special Characters 3 ASCII 91 to 96
else if (intChar >=91 && intChar <= 96){
int row = intChar - 91 + 22; // Offset the array index to start at 0
if (row == 23) //1
numCharacterCols = 4;
else
numCharacterCols = 3;
while(j<numCharacterCols){
  fullBuffer.push_back(specialCharacters[row][j]);
  j++;
}

}

//Special Characters 4 ASCII 123 to 126
else if (intChar >=123 && intChar <= 126){
int row = intChar - 123 + 28; // Offset the array index to start at 0
if (row == 31)
numCharacterCols = 4;
else
numCharacterCols = 3;
while(j<numCharacterCols){
  fullBuffer.push_back(specialCharacters[row][j]);
  j++;
}

}


Serial.println();
Serial.print("Current i: ");
Serial.println(i);
Serial.print("strLength is: ");
Serial.println(strLength);
 if(i!=(strLength-1)){ // || i != strLength){
 fullBuffer.push_back(0b0000000);
 Serial.println("adding space");
}
 i++;
 }

 for (int i = 0; i < frontPadding; i++){
   fullBuffer.push_back(0b0000000); //28 spaces at end
 }
 } //end convertStringToByteArray


void updateDisplay(){
Serial1.write(startTransmission, 3);
Serial1.write(immediateBuffer, 28);
Serial1.write(endTransmission, 1);
}

void clearImmediateBuffer(){
  for(int i = 0; i<28; i++){
    immediateBuffer[i] = 0b0000000;
  }
}
//returned in format "hourminute"
int getTime(int hoursType, int timeZone){ //default parameters
    Time.zone(timeZone);
      if (hoursType == 24)
    return (Time.hour())*100+Time.minute(); //24 hour
    else
    return (Time.hourFormat12())*100+Time.minute(); //12 hour
}

/*
 * return format "weekday,month,year"
 * EX. 01,12,2017
 */

String getDate(){
String concatDate;
concatDate.concat(String(Time.weekday()));
concatDate.concat(",");
concatDate.concat(String(Time.month()));
concatDate.concat(",");
concatDate.concat(String(Time.year()));
Serial.print("Returned date: ");
Serial.println(concatDate);
  return concatDate;
}



/****************************************
 * Transitions
 ****************************************/


void checkerBoardTransisiton(){
//Draw pixels
clearImmediateBuffer();
  for(int i = 0; i < 28; i++){
for (int j = 0; j < 7; j+=2){
  if (j % 2 == 0 && i % 2 ==0)
  drawPixel(j,i);
  else
  drawPixel(j+1,i);
}
}
updateDisplay();
delay(500);
for(int i = 0; i < 28; i++){
for (int j = 0; j < 7; j++){
clearPixel(j,i);
}
}

for(int i = 0; i < 28; i++){
  for(int j = 0; j < 7; j+=2){
    if (j % 2 == 0 && i % 2 == 0)
    drawPixel(j+1,i);
    else
    drawPixel(j,i);
  }
}
updateDisplay();
delay(500);

}


void swipeRightToLeftTransition(){
for(int i = 0; i<28; i++){
  for(int j = 0; j<7; j++){
    if(i != 0)
    clearPixel(j,i-1);
    drawPixel(j,i);
  }
  updateDisplay();
  delay(40);
}
}

void swipeTopToBottomTransition(){

  for(int j = 0; j<7; j++){
    for(int i = 0; i<28; i++){
      if(j != 0)
      clearPixel(j-1,i);

      drawPixel(j,i);
    }
    updateDisplay();
    delay(100);
  }
}
