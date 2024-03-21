#include <CAN.h>
#include <Arduino.h>

volatile float mux;
volatile float BP1_P, BP2_P, BP3_P, BP4_P, BP5_P, BP6_P;
volatile int16_t BP1_T, BP2_T, BP3_T, BP4_T, BP5_T, BP6_T;
volatile float BP_P_Supply;
volatile int16_t BP_Flow;
volatile float Exch_P, Suct_P, Disch_P;
volatile int16_t Exch_T, Master_T;
volatile float IN1_P, IN2_P, IN_P_Supply;
volatile int16_t IN_Temp;
volatile int16_t IN_Flow;
volatile float M_P, M_P_Supply;
volatile int16_t M_T, M_Flow;


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
  static int i;
  
  digitalWrite(A11, i & 1);
  digitalWrite(A12, i & (1<<1));
  digitalWrite(A13, i & (1<<2));
  digitalWrite(A14, i & (1<<3));
  digitalWrite(A15, i & (1<<4));

  mux=analogRead(A0);
  Serial.println(mux);
  
  i++;

  if(i > 31){
    i = 0;
  }
  CAN.beginExtendedPacket(400000000);
  CAN.write(mux);
  CAN.endPacket();
  
  delay(100);
}
