#include <WiFi.h>
#include <PubSubClient.h>

// Configurações - variáveis editáveis
const char* default_SSID = "Wokwi-GUEST"; // Nome da rede Wi-Fi
const char* default_PASSWORD = ""; // Senha da rede Wi-Fi
const char* default_BROKER_MQTT = "test.mosquitto.org"; // IP do Broker MQTT
const int default_BROKER_PORT = 1883; // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/energia/geracao/cmd"; // Tópico MQTT de escuta
const char* default_TOPICO_PUBLISH_1 = "/energia/geracao/valor"; // Tópico para publicar dados de energia
const char* default_TOPICO_PUBLISH_2 = "/energia/geracao/luminosidade"; // Tópico para publicar luminosidade
const char* default_ID_MQTT = "energia_001"; // ID MQTT

WiFiClient espClient;
PubSubClient MQTT(espClient);

char EstadoSaida = '0';

void initSerial() {
    Serial.begin(115200);
}

void initWiFi() {
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(default_SSID);
    Serial.println("Aguarde");
    reconectWiFi();
}

void initMQTT() {
    MQTT.setServer(default_BROKER_MQTT, default_BROKER_PORT);
    MQTT.setCallback(mqtt_callback);
}

void setup() {
    initSerial();
    initWiFi();
    initMQTT();
    delay(5000);
    MQTT.publish(default_TOPICO_PUBLISH_1, "s|on"); // Publica que o sistema foi iniciado
}

void loop() {
    VerificaConexoesWiFIEMQTT();
    handleEnergia(); // Nova função para lidar com dados de energia
    MQTT.loop();
}

void reconectWiFi() {
    if (WiFi.status() == WL_CONNECTED)
        return;
    WiFi.begin(default_SSID, default_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Conectado com sucesso na rede ");
    Serial.print(default_SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (int i = 0; i < length; i++) {
        char c = (char)payload[i];
        msg += c;
    }
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);
}

void VerificaConexoesWiFIEMQTT() {
    if (!MQTT.connected())
        reconnectMQTT();
    reconectWiFi();
}

void reconnectMQTT() {
    while (!MQTT.connected()) {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(default_BROKER_MQTT);
        if (MQTT.connect(default_ID_MQTT)) {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(default_TOPICO_SUBSCRIBE);
        } else {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Haverá nova tentativa de conexão em 2s");
            delay(2000);
        }
    }
}

void handleEnergia() {
    const int luminosidadePin = 34; // LDR ou outro sensor de luminosidade
    int sensorValue = analogRead(luminosidadePin);
    int luminosidade = map(sensorValue, 0, 4095, 0, 100); // Mapeia para uma escala de 0 a 100
    String mensagem = String(luminosidade);
    Serial.print("Valor da luminosidade (energia solar): ");
    Serial.println(mensagem.c_str());
    
    // Publica a luminosidade em um tópico
    MQTT.publish(default_TOPICO_PUBLISH_2, mensagem.c_str());
    
    // Publica os dados de energia no tópico principal (exemplo: produção solar ou eficiência)
    MQTT.publish(default_TOPICO_PUBLISH_1, ("Energia Solar: " + mensagem + "%").c_str());

}
