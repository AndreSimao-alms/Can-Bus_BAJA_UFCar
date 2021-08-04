/******************************************************************************************************************************************************************************
  BAJA UFSCar - 2021
  Eletrônica - Renan - André - Jack - Paloma

  Protocolo CAN BUS - Transmissor.

  Descrição: transmitir dados para placa escravo (slave).

  Em curso: conversão de leitura de dados hexadecimais para uma variável tipo float

  Próxima etapa: Implementação de valores analógicos e dispositivos referentes ao veículo

  Local de implementação: Programação indicada, por hora, na placa de datalog, onde estão presentes os sensores de temperatura do CVT,
  pressao dos freios (dia e tra), velocímetro e cartão SD
 ******************************************************************************************************************************************************************************
*/
#include <stdio.h>
#include <stdint.h>
//Para usar a bibliotecade comunicação SPI
#include <SPI.h>
//Para usar a biblioteca de comunicação CAN
#include <mcp2515.h>
//Declara uma estrutura de dados com base numa estrutura predefinida na biblioteca do Controlador CAN
struct can_frame canMsg1;
struct can_frame canMsg2;
//Pino de entrada que será o CS (Chip Select) da comunicação SPI entre Arduino e o Controlador CAN
MCP2515 mcp2515(10);

//VARIÁVEIS GLOBAIS

//Velocimetro JACK
#define pi 3.14
byte vel = 0;
int velPin = A2;
long rpm = 0;

// TESTE
long test;
long test1;

void setup() {
  while (!Serial);
  Serial.begin(115200);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();

  Serial.println("Transmissor CAN BUS");
}
void loop() {
  canmsg1();
  delay(100);
  canmsg2();
  delay(100);
}
//Programa responsável por armazenar os valores recebidos do ATMEGA escravo relacionados à velocidade do veículo
double rpm_function() {
  //Variaveis entro da função
  int state = 0, cont = 0, t;
  float d = 0.5334, r = 0;
  long lastmillis = 0;
  //Programação do sensor velocimetro JACK
  state = digitalRead(velPin);
  if (state == 1) {
    cont++;
  }
  t = cont;
  if (millis() - lastmillis >= 1000) {
    r = d / 2;
    rpm = t * 60 / 1000;
    cont = 0;
    lastmillis = millis();
  }
  return rpm;
}
//Programa responsável por armazenar os valores recebidos do ATMEGA escravo relacionados à velocidade do veículo
double velocimetro() {
  //Variaveis entro da função
  int state = 0, cont = 0, t;
  float d = 0.5334, r = 0;
  long rpm = 0, lastmillis = 0;
  //Programação do sensor velocimetro JACK
  state = digitalRead(velPin);
  if (state == 1) {
    cont++;
  }
  t = cont;
  if (millis() - lastmillis >= 1000) {
    r = d / 2;
    rpm = t * 60;
    vel = rpm * pi * 2 * r * 3.6 / 60;
    cont = 0;
    lastmillis = millis();
  }
  return vel;//duvidas se utiliza este comando juntamente com a troca da função de float para double
}
void canmsg1() {
  rpm = rpm_function();
  //test1 = 154; //rpm
  canMsg1.can_id  = 0x036;          //CAN id como 0x036
  canMsg1.can_dlc = 2;              //CAN comprimento do dados como 8 byte
  canMsg1.data[0] = 0x036;
  canMsg1.data[1] = rpm;
  /*canMsg1.data[2] = 0x00;
    canMsg1.data[3] = 0x00;
    canMsg1.data[4] = 0;
    canMsg1.data[5] = 6;
    canMsg1.data[6] = 7;
    canMsg1.data[7] = 8;*/
  mcp2515.sendMessage(MCP2515::TXB0, &canMsg1);
  delay(100);
}
void canmsg2() {
  //test=100;
  vel = velocimetro();
  canMsg2.can_id  = 0x034;          //CAN id como 0x036
  canMsg2.can_dlc = 2;              //CAN comprimento do dados como 8 byte
  canMsg2.data[0] = 0x034;
  canMsg2.data[1] = vel;
  /*canMsg2.data[2] = 0x00;
    canMsg2.data[3] = 0x00;
    canMsg2.data[4] = 0;
    canMsg2.data[5] = 6;
    canMsg2.data[6] = 7;
    canMsg2.data[7] = 8;*/
  mcp2515.sendMessage(MCP2515::TXB0, &canMsg2);
  delay(100);
}
