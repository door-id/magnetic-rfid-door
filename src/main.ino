/**
 * Porta magnetica com RFID
 * Engenharia Eletrica: UDESC
 * Turma IEE 24/1
 * 
 * Equipe responsavel:
 * - Arthur Bernardo Olsen
 * - Augusto Ogawa
 * - Daniel Felipe Rosenscheg
 * - Guilherme Guanho Sovat Cancio
 * - Gustavo Gomes Richter
 * - Lucas Hitoshi Ogawa
 * - Matheus Henrique Finkler
 * 
 * Todos os direitos reservados (c)
 */

#include <SPI.h>
#include <MFRC522.h>

#define PIN_RST 9
#define PIN_SS 10

#define PIN_BUTTON 2
#define PIN_LOCKER 3

#define PIN_BUZZER 7

#define PIN_RED 6
#define PIN_BLUE 4
#define PIN_GREEN 5

#define PIN_RESTART 8

#define MAX_UP_TIME 12500

#define CORRECT_CARD_UUID "A3 80 EC A6"

MFRC522 rfid(PIN_SS, PIN_RST);

struct theCardInfo {
  String content = "";
  int csize;
} theCard;

int restartTime;

void setup()
{
  // Esse pino tem prioridade no setup
  // Caso ele nao seja setado no comeco, a placa desliga
  digitalWrite(PIN_RESTART, HIGH);
  
  setupSerial();
  
  setupRfid();
  
  setupPins();
  Serial.println("Pinos configurados com sucesso.\n");

  restartTime = millis() + MAX_UP_TIME;

  Serial.println("Setup feito em '" + String(millis()) + "' segundos.\n");
  Serial.println("Aguardando um cartao...\n");
}

void loop()
{
  if(restartTime <= millis())
    resetArduino();

  if(digitalRead(PIN_BUTTON) == LOW){
    openLock();
    return;
  }
    
  if (!verifyCardExistence()) return;
  
  registerCard();
  Serial.println("Novo cartao registrado: [" + theCard.content + " ]\n");
  Serial.println("Tamanho do conteudo do novo cartao: " + String(theCard.csize) + "");

  if (theCard.content.substring(1) == CORRECT_CARD_UUID)
    openLock(); 
  else
    cardDeclined();
  
  // Reseta a placa quando um cartao for lido
  resetArduino();
}

void openLock()
{
  // Buzzer apita correto e acende led verde
  Serial.println("DEU BOA");
  tone(PIN_BUZZER, 950);
 // digitalWrite(PIN_BUZZER, HIGH);
  digitalWrite(PIN_GREEN, HIGH);
  delay(1000);

  //digitalWrite(PIN_BUZZER, LOW);
  digitalWrite(PIN_GREEN, LOW);
  noTone(PIN_BUZZER);

  digitalWrite(PIN_LOCKER, HIGH);
  delay(3000);
  digitalWrite(PIN_LOCKER, LOW);
}

void cardDeclined()
{
  // Buzzer apita incorreto e acende led vermelho
  Serial.println("DEU RUIM");
  tone(PIN_BUZZER,100);
  //digitalWrite(PIN_BUZZER, HIGH);
  digitalWrite(PIN_RED, HIGH);

  delay(1000);
  //digitalWrite(PIN_BUZZER, LOW);
  digitalWrite(PIN_RED, LOW);
  noTone(PIN_BUZZER);
}

bool verifyCardExistence() {
  return rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial();
}

void registerCard()
{
  String newCardContent = "";
  byte letter;
  for (byte i = 0; i < rfid.uid.size; i++) 
  {
     newCardContent.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
     newCardContent.concat(String(rfid.uid.uidByte[i], HEX));
  }

  newCardContent.toUpperCase();

  theCard.content = newCardContent;
  theCard.csize = rfid.uid.size;
}

void resetArduino() {
  digitalWrite(PIN_RESTART, LOW);
}

void setupSerial() {
  Serial.begin(9600);
  
  while (!Serial);
}

void setupRfid() {
  SPI.begin();
  
  rfid.PCD_Init();
  
  delay(4);
}

void setupPins()
{
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_LOCKER, OUTPUT);
  
  pinMode(PIN_BUZZER, OUTPUT);

  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);

  pinMode(PIN_RESTART, OUTPUT);
}
