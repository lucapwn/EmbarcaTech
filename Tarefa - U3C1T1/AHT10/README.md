# Aplicação Multitarefa

## Unidade 3 / Capítulo 1

### Objetivos

- Projetar e integrar sensores e atuadores específicos em redes IoT.
- Realizar atividades práticas com sensores e atuadores.

### Enunciado

**Monitoramento de Temperatura e Umidade com o Sensor AHT10:**

- Configurar o sensor AHT10 para monitorar tanto a temperatura quanto a umidade do ambiente, utilizando o RP2040.
- A programação do microcontrolador deverá coletar e exibir esses dados em uma tela LCD 320x240, possibilitando uma visualização em tempo real.
- Como um desafio adicional, ajuste o código para que o LCD exiba um aviso caso a umidade ultrapasse 70% ou a temperatura fique abaixo de 20 °C, criando uma interface que alerta o usuário sobre condições específicas do ambiente.

### Conectando os Componentes

| Pico W | AHT10 | Display |
|--------|-------|---------|
| GP0    | SDA   |         |
| GP1    | SCL   |         |
| GP4    |       | SDA     |
| GP5    |       | SCL     |
| 3V3    | VCC   | VCC     |
| GND    | GND   | GND     |

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
