# Sistema de Monitoramento de Energia Solar com MQTT e ESP32

Este projeto utiliza um dispositivo ESP32 para monitorar dados de luminosidade de um sensor conectado ao pino 34 do ESP32 e publicar essas informações via MQTT. O sistema é configurado para se conectar automaticamente à rede Wi-Fi e ao broker MQTT. É ideal para aplicações que exigem monitoramento de energia solar e outros dados ambientais.

## Índice :page_with_curl:

* [Descrição do Projeto](#descrição-do-projeto)
* [Requisitos](#requisitos)
* [Dependências](#dependências)
* [Instruções de Uso](#instruções-de-uso)
* [Arquitetura do Projeto](#arquitetura-do-projeto)

## Descrição do Projeto

<p>
  Este projeto é um sistema de monitoramento de energia solar que coleta dados de luminosidade de um sensor (LDR) e publica os resultados em dois tópicos MQTT:
</p>

- ``1.`` Tópico para dados de energia: ``/energia/geracao/valor``— Aqui são enviados os dados relativos à energia solar.
- ``2.`` Tópico para dados de luminosidade: ``/energia/geracao/luminosidade`` — Aqui são enviados os valores de luminosidade do sensor LDR.

<p>
  O ESP32 conecta-se automaticamente à rede Wi-Fi e ao broker MQTT, e o sistema irá continuar publicando dados até que seja desligado ou ocorra algum erro de conexão, que será tratado pelo código.
</p>

## Requisitos

- Hardware:
  - ESP32
  - Sensor de luminosidade (LDR) conectado ao pino 34
  - Acesso a uma rede WiFi
  - Broker MQTT (test.mosquitto.org ou qualquer outro broker compativel)
    
- Software:
  - Arduino IDE ou PlatformIO para programação do ESP32
  - Biblioteca WiFi.h para conectar o ESP32 à rede WiFi
  - Biblioteca PubSubClient.h para comunicação MQTT
 
## Dependências

Este projeto depende das seguintes bibliotecas:
- WiFi.h
- PubSubClient.h
Essas bibliotecas podem ser instaladas diretamente no Arduino IDE através do Gerenciador de Bibliotecas

## Instruções de Uso

- ``1.`` Instalar o Arduino IDE:
    - Baixe e instale o Arduino IDE.
    - No Arduino IDE vá até ``Ferramentas > Placa`` e selecione a placa ESP32
- ``2.`` Instalar as Bibliotecas:
    - Vá para ``Sketch > Incluir Biblioteca > Gerenciar Bibliotecas``
    - Pesquise e instale as bibliotecas ``WiFi`` e ``PubSubClient``
- ``3.`` Configuração de Rede:
    - Substitua as váriaveis ``default_SSID`` e ``default_PASSWORD`` pelas credenciais da sua rede WiFi
    - Substitua a váriavel ``default_BROKER_MQTT`` pelo endereço IP ou URL do seu broker MQTT
- ``4.`` Carregar o código:
    - Carregue o código no seu ESP32 usando o Arduino IDE
    - O ESP32 tentará se conectar à rede WiFi e ao broker MQTT, publicando os dados do sensor de luminosidade
- ``5.`` Monitoramento:
    - Abra o monitor serial no Arduino IDE para acompanhar o estado da conexão e os dados sendo publicados
 
## Arquitetura do Projeto

O projeto segue a arquitetura de cliente MQTT com ESP32 e Wi-Fi:

- ``1.`` Conexão Wi-Fi:
  - O ESP32 tenta conectar-se à rede Wi-Fi usando as credenciais fornecidas. Se a conexão falhar, ele tenta reconectar.
- ``2.`` Conexão MQTT:
    - O ESP32 conecta-se a um broker MQTT (definido pelo endereço ``default_BROKER_MQTT``). Após a conexão, o ESP32 se inscreve no tópico de comando (``default_TOPICO_SUBSCRIBE``), mas a principal função do código é a publicação dos dados de luminosidade.
- ``3.`` Leitura do Sensor e Publicação dos Dados:
    - O sensor de luminosidade (LDR) é lido através de um pino analógico (pino 34). O valor da luminosidade é mapeado para uma escala de 0 a 100 e enviado para o broker MQTT nos tópicos definidos.
- ``4.`` Verificação de Conexões:
    - O código verifica periodicamente a conexão Wi-Fi e MQTT, reconectando automaticamente quando necessário.
 
