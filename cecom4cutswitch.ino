/** CECOM4CUT

#define SS_PIN    21
#define RST_PIN   22

PIN MAP
RPI 23 =  회색 = ESP 32 (SHOOT)
RPI 24 = 보라색 = ESP 25 (REPRINT)
나머지 =  전원선 = 건들지마셈
-> 뽑았다가 까먹었다면 유0민씨에게 사진을 요구할 것 **/

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN    21
#define RST_PIN   22

#define CARD_UID_SIZE 7
#define CARD_SHOOT 1
#define CARD_REPRINT 2

#define GPIO_SHOOT 32
#define GPIO_REPRINT 25 

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

byte nuidPICC[7];
const uint16_t shoot_card_uid_1[7]={0x04, 0x29, 0xF8, 0x32, 0x64, 0x52, 0x81};
const uint16_t shoot_card_uid_2[7]={ 0x04, 0xCE, 0xF9, 0x32, 0x64, 0x52, 0x80};
const uint16_t reprint_card_uid[7]={ 0x04, 0xBE, 0xF9, 0x32, 0x64, 0x52, 0x80};
void trigger_rpi (int card_type);
int recognize_card(byte *buffer, byte bufferSize);


void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 
  pinMode(GPIO_SHOOT, OUTPUT);
  pinMode(GPIO_REPRINT, OUTPUT);  
  digitalWrite(GPIO_SHOOT, LOW);
  digitalWrite(GPIO_REPRINT, LOW);


}
 
void loop() 
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  trigger_rpi ( recognize_card(mfrc522.uid.uidByte, CARD_UID_SIZE));
  delay(1000);
}



int recognize_card(byte *buffer, byte bufferSize){
  if (buffer[0] == shoot_card_uid_1[0] &&
      buffer[1] == shoot_card_uid_1[1] && 
      buffer[2] == shoot_card_uid_1[2] && 
      buffer[3] == shoot_card_uid_1[3] &&
      buffer[4] == shoot_card_uid_1[4] && 
      buffer[5] == shoot_card_uid_1[5] && 
      buffer[6] == shoot_card_uid_1[6] ){
    Serial.print("Shoot photo!");
    return CARD_SHOOT;
  }else if (buffer[0] == shoot_card_uid_2[0] &&
      buffer[1] == shoot_card_uid_2[1] && 
      buffer[2] == shoot_card_uid_2[2] && 
      buffer[3] == shoot_card_uid_2[3] &&
      buffer[4] == shoot_card_uid_2[4] && 
      buffer[5] == shoot_card_uid_2[5] && 
      buffer[6] == shoot_card_uid_2[6] ){
    Serial.print("Shoot photo!");
    return CARD_SHOOT;
  }else if (buffer[0] == reprint_card_uid[0] &&
      buffer[1] == reprint_card_uid[1] && 
      buffer[2] == reprint_card_uid[2] && 
      buffer[3] == reprint_card_uid[3] &&
      buffer[4] == reprint_card_uid[4] && 
      buffer[5] == reprint_card_uid[5] && 
      buffer[6] == reprint_card_uid[6] ){
    Serial.print("Reprint photo!");
    return CARD_REPRINT;
  }
  else {
    Serial.print("먼가 잘못됨 CALL 유빈");
  }
    return 0;
}

void trigger_rpi (int card_type){
  if(card_type == CARD_SHOOT){
    digitalWrite(GPIO_SHOOT, HIGH);
    delay (500);
    digitalWrite(GPIO_SHOOT, LOW);
  }else if(card_type == CARD_REPRINT){
    digitalWrite(GPIO_REPRINT, HIGH);
    delay (500);
    digitalWrite(GPIO_REPRINT, LOW);
  }

}
