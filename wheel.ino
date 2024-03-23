#include <CAN.h>

#define TrimUpPin 1
#define TrimDownPin 2
#define GainUpPin 3
#define GainDownPin 4
#define EStopPin 5
#define EncoderPin 6
#define ThrottlePin 7
#define LED1 10

float encoderVal = 0;
float throttleVal = 0;
uint16_t i = 0;

void setup() {
  pinMode(TrimUpPin, INPUT);
  pinMode(TrimDownPin, INPUT);
  pinMode(GainUpPin, INPUT);
  pinMode(GainDownPin, INPUT);
  pinMode(EStopPin, INPUT);
  pinMode(EncoderPin, INPUT);
  pinMode(ThrottlePin, INPUT);
  pinMode(LED1, OUTPUT);
  
  Serial.begin(115200);

  CAN.begin(500E3);

  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}

void loop() {
  encoderVal = readEncoder();
  encodertoCAN(encoderVal);

  throttleVal = readThrottle();
  throttletoCAN(throttleVal);

  buttonstoCAN();
}

float readEncoder(){
  return (float)analogRead(EncoderPin)*360./4095.; //Scale => [0,360]
}

float readThrottle(){
  return (float)analogRead(ThrottlePin)*360./4095.; //Scale => [0,360]
}

void encodertoCAN(float val){
  CAN.beginPacket(1999);
  char data[sizeof(val)];                //Create char array
  memcpy(data, &val, sizeof(val));       //Store bytes of val to array
  for(int j = 0; j < sizeof(val); j++){  //Write bytes one by one to CAN
    CAN.write(data[j]);
  }
  CAN.endPacket();
}

void throttletoCAN(float val){
  CAN.beginPacket(1998);
  char data[sizeof(val)];                //Create char array
  memcpy(data, &val, sizeof(val));       //Store bytes of val to array
  for(int j = 0; j < sizeof(val); j++){  //Write bytes one by one to CAN
    CAN.write(data[j]);
  }
  CAN.endPacket();
}

/*----------------------------------
IDs
2000
-----------------------------------*/
void buttonstoCAN(){
  CAN.beginPacket(2000);
  CAN.write(analogRead(TrimUpPin));
  CAN.write(analogRead(TrimDownPin));
  CAN.write(analogRead(GainUpPin));
  CAN.write(analogRead(GainDownPin));
  CAN.write(analogRead(EStopPin));
  CAN.endPacket();
}
