#include <CAN.h>
#include <Arduino.h>

/*
volatile float BP1_P, BP2_P, BP3_P, BP4_P, BP5_P, BP6_P;
volatile int16_t BP1_T, BP2_T, BP3_T, BP4_T, BP5_T, BP6_T;
volatile float BP_P_Supply;
volatile int16_t BP_Flow;
volatile float Exch_P, Suct_P, Disch_P;
volatile int16_t Exch_T, Master_T;
volatile float IN1_P, IN2_P, IN_P_Supply;
volatile int16_t IN_T;
volatile int16_t IN_Flow;
volatile float M_P, M_P_Supply;
volatile int16_t M_T, M_Flow;
int i = 0;
*/
// floats: 15
//ints: 10
// undef: 3 

void setup() {
  // put your setup code here, to run once:
  if (!CAN.begin(250E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  Serial.begin(115200);
  pinMode(A15,OUTPUT);
  pinMode(A14,OUTPUT);
  pinMode(A13,OUTPUT);
  pinMode(A12,OUTPUT);
  pinMode(A11,OUTPUT);
  pinMode(A0, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  setMUXToReadSensor(i);
  
  sendCANMessage(i, *readSensor(i));
  
  
  i++;
  if(i > 27){
    i = 0;
  }
  
  delay(100);
}

void setMUXToReadSensor(int i) {
  digitalWrite(A11, i & 1);
  digitalWrite(A12, i & (1<<1));
  digitalWrite(A13, i & (1<<2));
  digitalWrite(A14, i & (1<<3));
  digitalWrite(A15, i & (1<<4));
}

void *readSensor(int i) {
  int result = analogRead(A0); //0-1023
  float fresult = 0;
  if (i < 15) { 
    fresult = ((float)result/1023*5*2002.9)-14.5; //-14.5 - 10000
    return *(float *)fresult;
  }else if(i < 25){ // isInt
    result = (result/1023 * 5 * 60) - 50; //-50 - 200
    return *(int *)result;
  }else{
    //scale for flow here
    return *(int *) result;
  }
}

/*-----------------------------------------------------
400000000 - 400000014 is Pressure Sensors
400000015 - 400000024 is Temp Sensor
400000025 - 400000027 is Flow
-------------------------------------------------------*/
void sendCANMessage(int i, void *muxOut) {
  CAN.beginExtendedPacket(400000000+i);
  CAN.write(mux);
  CAN.endPacket();
}
