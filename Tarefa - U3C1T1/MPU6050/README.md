# Aplicação Multitarefa

## Unidade 3 / Capítulo 1

### Objetivos

- Projetar e integrar sensores e atuadores específicos em redes IoT.
- Realizar atividades práticas com sensores e atuadores.

### Enunciado

**Controle e Monitoramento de Movimento com MPU6050:**

- Configurar o sensor MPU6050 para captar dados de movimento, como aceleração e rotação, e exibir as leituras de inclinação no monitor serial.
- Além disso, programe o RP2040 para ajustar a posição de um Servo Motor 9G SG90 em função do ângulo de inclinação detectado, promovendo um controle dinâmico de movimento em resposta à orientação do sensor.
- No desafio extra, você deverá adicionar um alerta visual na tela LCD 320x240 que indique quando o sistema ultrapassa um determinado ângulo de inclinação, criando um sistema de monitoramento visual das mudanças de posição.

### Conectando os Componentes

| Pico W | Display | Servo | MPU6050 |
|--------|---------|-------|---------|
| GP0    | SDA     |       |         |
| GP1    | SCL     |       |         |
| GP2    |         | IN    |         |
| GP4    |         |       | SDA     |
| GP5    |         |       | SCL     |
| 3V3    | VCC     |       | VCC     |
| 5V     |         | VCC   |         |
| GND    | GND     | GND   | GND     |

### Executando a Aplicação

Para configurar, compilar e executar a aplicação, utilize a extensão [Raspberry Pi Pico](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico) no [VSCode](https://code.visualstudio.com/) e realize os seguintes passos:

- Importe o projeto em ***Import Project***.
- Limpe os arquivos de configuração executando o ***Clean CMake***.
- Configure a aplicação executando o ***Configure CMake***.
- Compile a aplicação executando o ***Compile Project***.
- Coloque a placa Raspberry Pi Pico W em modo de gravação segurando o botão `BOOTSEL` antes de conectá-la ao cabo USB.
- Faça o processo de flashing do firmware executando o ***Run Project (USB)*** ou copie o arquivo `build/main.uf2` para a unidade de disco `RPI-RP2`.

### Autor

Desenvolvido por [Lucas Araújo](https://github.com/lucapwn).
