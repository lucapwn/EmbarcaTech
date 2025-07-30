# Aplicação Multitarefa

## Unidade 3 / Capítulo 1

### Objetivos

- Projetar e integrar sensores e atuadores específicos em redes IoT.
- Realizar atividades práticas com sensores e atuadores.

### Enunciado

**Leitura de Luminosidade com o Sensor BH1750:**

- Configurar o sensor BH1750 para medir a intensidade de luz, utilizando o RP2040.
- A programação do microcontrolador será feita para coletar os dados de luminosidade e exibi-los diretamente em um monitor serial, permitindo a observação em tempo real das variações na intensidade da luz.
- Além disso, como um desafio extra, programe o RP2040 para ajustar automaticamente a posição de um Servo Motor 9G SG90 em resposta ao nível de luz captado pelo sensor, de forma que o servo reaja dinamicamente conforme a iluminação detectada.

### Conectando os Componentes

| Pico W | BH1750 | Servo |
|--------|--------|-------|
| GP0    | SDA    |       |
| GP1    | SCL    |       |
| GP2    |        | IN    |
| 3V3    | VCC    |       |
| 5V     |        | VCC   |
| GND    | GND    | GND   |

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
