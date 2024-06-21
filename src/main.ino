#include <SPI.h>
#include <MFRCB22.h>
#include <bool.h>

#define PIN_RST 6
#define PIN_SS 2
#define PIN_TX 5
#define PIN_MOSI 4
#define PIN_SCK 3

#define PIN_BUTTON 7

#define PIN_BUZZER 8

#define PIN_RED 9
#define PIN_BLUE 11
#define PIN_GREEN 10

#define PIN_LOCKER 12

void cardAccepted();
void cardDeclined();

void registerCard(char* cardByte, int cardSize);
bool isItTheCard(char* cardByte, int cardSize);

void setupRfid();
void setupPins();

const MFRCB22 rfid(PIN_RST, PIN_SS);
char* theCard = NULL;
int theCardSize = -1;

void setup() {
  Serial.begin(9600);
  
  setupRfid();

  setupPins();
}

void loop() {
  if(rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    const char* cardByte = rfid.uid.uidByte;
    const int cardSize = rfid.uid.size;

    if(theCardSize < 1)
      registerCard(cardByte, cardSize);

    if(isItTheCard(cardByte, cardSize))
      cardAccepted();
    else
      cardDeclined();
  }
}

void cardAccepted() {
// buzzer apita correto
//  tone(BUZZ,500);
//  digitalWrite(BUZZ, HIGH);
//  delay(1000);
//  digitalWrite(BUZZ, LOW);

}

void cardDeclined() {
// buzzer apita incorreto
//  tone(BUZZ,100);
//  digitalWrite(BUZZ, HIGH);
//  delay(1000);
//  digitalWrite(BUZZ, LOW);
  
}

bool isItTheCard(char* cardByte, int cardSize) {
  if(cardSize != theCardSize)
    return false;

  for(int i = 0; i < theCardSize; i++)
    if(theCard[i] != cardByte[i])
      return false;
    
  return true;
}

void registerCard(char* cardByte, int cardSize) {
  if(theCard != NULL)
    throw "The card is already set!";

  theCard = cardByte;
  theCardSize = cardSize;
}

void setupRfid() {
  SPI.begin();
  rfid.PCD_Init();
}

void setupPins() {
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_LOCKER, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
}
