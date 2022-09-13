/* Board: Arduino Uno
 * Purpose: Faking WGCTF22 Badge Cross Infection IR signals
 * IR LED: GND --- LED --- 100 ohm resistor --- D3
*/
#include <Arduino.h>
#include <IRremote.h>

#define DEBUG_ENABLED

#ifdef DEBUG_ENABLED
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
  #define DEBUG_PRINTLN_HEX(x) Serial.println(x, HEX)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTLN_HEX(x)
#endif

union split32 {
    uint32_t fullHex;
    uint16_t splitHex[2];
};

uint32_t badgeID;
uint16_t badgeIDtoAck;
unsigned int badgeType;
unsigned long mutation = 0xFFFFFFFF;
unsigned long variant  = 0xFFFFFFFF;

String ack_str = "";
int wait = 10;
bool wait_for_input = false;

void sendCrossInfectMessage(unsigned int badgeID, unsigned long mutation, unsigned long variant, unsigned int badgeType)
{
    DEBUG_PRINTLN("Sending cross infect greet");
    
    split32 badgeIdSplit;
    badgeIdSplit.fullHex = badgeID;
    split32 mutationSplit;
    mutationSplit.fullHex = mutation;
    split32 variantSplit;
    variantSplit.fullHex = variant;
    
    IrSender.sendNEC(badgeIdSplit.splitHex[0], 0x01, 2);
    delay(50);
    IrSender.sendNEC(badgeIdSplit.splitHex[1], 0x02, 2);
    delay(50);
    IrSender.sendNEC(mutationSplit.splitHex[0], 0x03, 2);
    delay(50);
    IrSender.sendNEC(mutationSplit.splitHex[1], 0x04, 2);
    delay(50);
    IrSender.sendNEC(variantSplit.splitHex[0], 0x05, 2);
    delay(50);
    IrSender.sendNEC(variantSplit.splitHex[1], 0x06, 2);
    delay(50);
    IrSender.sendNEC(badgeType, 0x07, 1);
    delay(50);
}

void sendCrossInfectAck(uint16_t msgBadgeId)
{
    DEBUG_PRINTLN("Sending cross infect ACK");
    IrSender.sendNEC(msgBadgeId, 0x08, 2);
    delay(50);
}

// ----------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10000);
  DEBUG_PRINTLN("-------------------------------------------------------");
  DEBUG_PRINTLN(F("START " __FILE__ " from " __DATE__));

  IrSender.begin(3);

  randomSeed(analogRead(0));

  DEBUG_PRINTLN();
  DEBUG_PRINTLN("Input the first 4 hex characters, ex 0xBEEF, of the Badge ID to ACK, followed by Enter");
}

void loop() {

  if(Serial.available() > 0 && ack_str == "") {
      ack_str = Serial.readStringUntil('\n');
      badgeIDtoAck = strtol(ack_str.c_str(), NULL, 16);
      DEBUG_PRINT("Badge ID to Ack: ");
      DEBUG_PRINTLN_HEX(badgeIDtoAck);
      DEBUG_PRINTLN("On your badge, enter the BOTNET menu on screen, then Cross Infect, then OK");
      DEBUG_PRINTLN("Press the enter key here...");
      DEBUG_PRINTLN();
      while(Serial.available() == 0) {}
  }

  if( ack_str != "" && wait_for_input == false ) {
    badgeType = random(0, 3);
    badgeID   = random(1, 0xFFFF);
    DEBUG_PRINT("Emulating Badge type: ");
    DEBUG_PRINT(badgeType);
    DEBUG_PRINT("  ID: ");
    DEBUG_PRINTLN_HEX(badgeID);

    for(int i = 0; i<3; i++){
      sendCrossInfectMessage(badgeID, mutation, variant, badgeType);
      sendCrossInfectAck(badgeIDtoAck);
    }

    DEBUG_PRINTLN();
    DEBUG_PRINTLN("Press the space bar ...");
    wait_for_input = true;
  }

  if( ack_str != "" && wait_for_input == true ) {
    char input = Serial.read();
    if( input == ' ' ) {
      wait_for_input = false;
      DEBUG_PRINTLN();
    }
  }

}
