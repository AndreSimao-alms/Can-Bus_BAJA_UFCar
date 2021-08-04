# can_bus_bajaufscar_2021
## Objetivo geral 
Transmissão e recebimento de dados entre placas Arduinos através do protocolo CAN BUS. O circuito será implementado ao carro BAJA UFSCar, este é incluso diversos sensores, como por exemplo, sensor de temperatura, pressão de vias de freio, rpm, velocidade, entre outros. Dessa maneira, para ter o sucesso com a manipulação dos dados gerados por estes dispositivos, é implementado ao projeto a comunicação por via LORAWAN e CAN BUS. 
### Arquivos presentes no repositório
#### Arquivo read.ino:
Leitura de dados fornecidos de uma placa master, neste caso, a leitura é referente aos sensores de RPM e velocidade na placa de datalog do carro. 
#### Arquivo write.ino: 
Transmissão de dados dos sensores de RPM e velocidade. Vale salientar que esta placa aloca somente este sensores devidos as interrupções necessárias para realizar a coleta de dadso, mais conhecido como attachInterrupt().
