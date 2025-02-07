# Comunicação Serial com Raspberry Pico W 2040, UART, SPI e I2C

**Autor: Luis Felipe Pereira de Carvalho**

**Técnico em Análises e Desenvolvimento de Sistemas - SENAI**

**Graduando em Bacharelado de Engenharia de Computação - UEFS**

Projeto realizado para a Residência EmbarcaTech 2024/2025

## Índice

* [Objetivos](#objetivos)
* [Descrição](#descri%C3%A7%C3%A3o)
* [Funcionalidades](#funcionalidades)
* [Requisitos do Projeto](#requisitos-do-projeto)
* [Como Executar o Programa](#como-executar-o-programa)
* [Estrutura do Código](#estrutura-do-c%C3%B3digo)
* [Acesso Via Porta Serial](#Acesso-via-Porta-Serial)
* [Referências](#refer%C3%AAncias)
* [Demonstrativo em Vídeo](#demonstrativo-em-v%C3%ADdeo)

## 1. Objetivos

Este projeto visa desenvolver um sistema de controle de LEDs e exibição de caracteres utilizando um display OLED, botões físicos e comunicação UART no Raspberry Pi Pico W. Os principais objetivos incluem:

* **Controle de LEDs via Botões:**
  * O botão A alterna o estado do LED Verde.
  * O botão B alterna o estado do LED Azul.
* **Exibição de Status no Display OLED:**
  * O display mostra o estado atual dos LEDs (Ligado/Desligado).
  * Caracteres recebidos via UART são exibidos no display.
* **Uso de Temporização e Interrupções:**
  * Implementação de debounce via software para evitar acionamentos indesejados dos botões.
  * Atualização periódica do display para refletir as mudanças de estado.
* **Integração com a Matriz de LEDs WS2812:**
  * Exibição de números na matriz de LEDs conforme entrada UART.
  * Limpeza da matriz caso um caractere inválido seja recebido.

## 2. Descrição

O código utiliza a placa Raspberry Pi Pico W para gerenciar a entrada de botões físicos e a comunicação via UART. Os LEDs RGB são controlados por botões físicos e seus estados são exibidos no display OLED. A matriz de LEDs exibe números enviados pela UART.

**Funcionamento do Sistema:**

1. O usuário pressiona o botão A e B para alternar o estado dos LEDs.
2. O estado dos LEDs (ligado/desligado) é atualizado no display OLED enviado via portal serial.
3. Caracteres recebidos via UART são exibidos no display e interpretados pela matriz de LEDs.
4. O sistema utiliza debounce para evitar acionamentos repetidos acidentais.
5. O display OLED é atualizado periodicamente com as informações mais recentes.

## 3. Funcionalidades

* **Interrupções para detecção dos botões.**
* **Implementação de debounce via software.**
* **Controle de LEDs com transição de estado.**
* **Uso de timers para atualização da interface OLED.**
* **Recebimento de dados via UART e exibição no display OLED.**
* **Controle da matriz de LEDs WS2812 baseado em entradas UART.**

## 4. Requisitos do Projeto

* **Placa Raspberry Pi Pico W (RP2040).**
* **Ambiente de desenvolvimento configurado (SDK do RP2040, compilador C/C++).**
* **LEDs RGB conectados aos pinos 11, 12 e 13.**
* **Botões conectados aos pinos 5 e 6.**
* **Display OLED SSD1306 conectado via I2C (pinos 14 e 15).**
* **Matriz de LEDs WS2812 conectada ao pino 7.**
* **Interface UART configurada nos pinos 0 e 1.**

## 5. Como Executar o Programa

### 1. Configuração do Ambiente

* Instale o SDK do Raspberry Pi Pico seguindo a documentação oficial.
* Configure a ferramenta de build CMake e o compilador ARM.

### 2. Compilação do Código

Clone o repositório ou copie o código para um diretório local.

No terminal, navegue até o diretório do projeto e execute:

```sh
mkdir build && cd build
cmake ..
make
```

### 3. Upload para a Raspberry Pi Pico W

* Conecte a Raspberry Pi Pico ao computador enquanto segura o botão BOOTSEL.
* Copie o arquivo `.uf2` gerado para a unidade que aparecerá no sistema.
* A Pico será reiniciada automaticamente e executará o código.

## 6. Estrutura do Código

* **Definição de Pinos:** Configuração dos GPIOs para LEDs, botões e display OLED.
* **Interrupção dos Botões:** Implementação de debounce e acionamento dos LEDs.
* **Atualização do Display:** Exibição do estado dos LEDs e caracteres recebidos via UART.
* **Controle da Matriz de LEDs:** Exibição de números com base nos dados recebidos.
* **Loop Principal:** Leitura de entradas, atualização de display e controle de LEDs.

## 7. Acesso via Porta Serial

A comunicação serial é realizada pela UART0 a uma taxa de 9600 bps. O sistema pode receber comandos tanto via USB (CDC) quanto pela UART física.

### **1. Conectar via USB**

* No Linux/Mac, utilize:
  ```
  screen /dev/ttyACM0 9600
  ```
* No Windows, use um software como PuTTY ou Termial do VS Code e configure a porta COM correspondente.

### **2. Comandos Disponíveis**

* **Digitar um número (0-9)**: Atualiza a matriz de LEDs para exibir o número correspondente exibe no display.
* **Enviar uma letra**: Exibe no display OLED o caractere recebido.
* **Botões A/B**: Alternam os LEDs e atualizam o status do led azul/verde no display OLED.

## 8. Referências

* [Documentação do Raspberry Pi Pico](https://www.raspberrypi.com/documentation/microcontrollers/raspberry-pi-pico.html)
* [SDK do RP2040](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
* [PIO (Programmable Input/Output)](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)

## 9. Demonstrativo em Vídeo

Assista ao funcionamento do projeto no seguinte link:

[Link para o vídeo demonstrativo](https://youtu.be/PLVVGG2ULew)
