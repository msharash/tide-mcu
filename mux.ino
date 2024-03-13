#include <CAN.h>

volatile int mux;
volatile int i = 0;

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
  digitalWrite(A11, i & 1);
  digitalWrite(A12, i & (1<<1));
  digitalWrite(A13, i & (1<<2));
  digitalwrite(A14, i & (1<<3));
  digitalWrite(A15, i & (1<<4));

  mux=analogRead(A0);
  serial.Println(mux);
  
  i++;

  if(i > 31){
    i = 0;
  }
  CAN.beginExtendedPacket(400000000);
  CAN.write(mux);
  CAN.endPacket();
}
