/******************************************************************************************************************************************************************************
  BAJA UFSCar - 2021
  Eletrônica - Renan - André - Jack - Paloma

  Protocolo CAN BUS - Receptor.
  Dispositivo conectados na placa:
  - Sensor de Temperatura
  - Sensor de pressão de via de freio (dia e tra)
  - Velocímetro
  - Módulo CAN BUS
  - Cartão SD (!dúvidas!)

  Descrição: receber dados transmitidos de uma placa mestre.

  Em curso: construção de leitura de dados hexadecimais para uma variável tipo float

  Próxima etapa: Implementação de valores analógicos e dispositivos referentes ao veículo

  Local de implementação: Programação indicada, por hora, na placa de datalog, onde estão presentes os sensores de temperatura do CVT,
  pressao dos freios (dia e tra), velocímetro e cartão SD
 ******************************************************************************************************************************************************************************
*/
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

int pinoCS = 10, tempPin = A2, velPin = 2, fdiaPin = A0, ftraPin = A1;

//Sensor de Temperatura
int leitura;
float voltagem;
float temperatura;

//Variaveis dentro da função freio_tra
int val1 = 0;
float pressao1 = 0;
float volt1 = 0;

//Variaveis dentro da função freio_dia
int val2 = 0;
float pressao2 = 0;
float volt2 = 0;

//Variável booleana volátil (varia seu valor de forma intermitente de false para true e vice-versa
volatile bool interrupt = false;

void setup() {
  //Inicia a comunicação Serial
  Serial.begin(115200);
  //Reset do Controlador CAN atraves da SPI do Arduino
  mcp2515.reset();
  //Configura a velocidade de comunicação CAN para 500KBPS com um Clock de 8MHz. Clock do cristal do Controlador MCP2515
  mcp2515.setBitrate(CAN_125KBPS);
  //Modos de Operação do Controlador CAN: 1. Configuration mode 2. Normal mode 3. Sleep mode 4. Listen-Only mode 5. Loopback mode
  //Configura o modo normal
  mcp2515.setNormalMode();
  Serial.println("------- Leitor CAN BUS ----------");
  Serial.println("ID  DLC   DATA");
  attachInterrupt(0, irqHandler, FALLING);
}
void loop() {
  // Se houver dado disponivel na Serial para ser lido
  // while (Serial.available() > 0)
  // {
  if (interrupt) {
    interrupt = false;
    uint8_t irq = mcp2515.getInterrupts();
    if (irq & MCP2515::CANINTF_RX0IF) {
      if ((mcp2515.readMessage(MCP2515::RXB0, &canMsg1) == MCP2515::ERROR_OK)) {
        if (canMsg1.data[0] == 0x036) {
          Serial.print("RPM (rot/min)x1000: " );
          Serial.println(canMsg1.data[1], DEC);
          Serial.print("\n");
        }
      }
    }
    if (irq & MCP2515::CANINTF_RX0IF) {
      if ((mcp2515.readMessage(MCP2515::RXB0, &canMsg2) == MCP2515::ERROR_OK)) {
        if (canMsg2.data[0] == 0x034) {
          Serial.print("Velocidade (km/h): ");
          Serial.println(canMsg2.data[1], DEC);
          Serial.print("\n");
        }
      }
    }
  }
}
//Sensor de temperatura ANDRE
void temp() {
  leitura = analogRead(tempPin);
  voltagem = leitura * (5.0 / 1023);
  temperatura = (voltagem * 100) - 50;
  Serial.print("temp: ");
  Serial.print(temperatura, 0);
  Serial.println("C");
  delay(1000);
}
//Programa responsável por armazenar os valores do sensores de pressão nas vias do freio dianteiro
void freio_dia() {
  //Programação do sensor do freio dianteiro DANTE
  val2 = analogRead(fdiaPin);
  volt2 = 0.004887 * val2;
  pressao2 = (2 * volt2 - 0.9) - 0.07375;
  //Filtro de ruídos
  if (pressao2 <= 0.1) {
    pressao2 = 0;
  }
  Serial.print("Freio_dia_press: ");
  Serial.print(pressao1, 0);
  Serial.println("Pa.");
  delay(1000);
}
//Função que torna a volátil bool
void irqHandler() {
  interrupt = true;
}
//Programa responsável por armazenar os valores do sensores de pressão nas vias do freio traseiro
void freio_tra() {
  //Programação do sensor do freio traseiro DANTE
  val1 = analogRead(ftraPin);
  volt1 = 0.004887 * val1;
  pressao1 = (2 * volt1 - 0.9) - 0.03;
  //Filtro de ruídos
  if (pressao1 <= 0.1) {
    pressao1 = 0;
  }
  Serial.print("Freio_tra_press: ");
  Serial.print(pressao1, 0);
  Serial.println("Pa.");
  delay(1000);
}
