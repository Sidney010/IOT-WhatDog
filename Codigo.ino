#include <WiFi.h>
#include <HTTPClient.h> 
#include <WiFiClientSecure.h>

// Credenciaias de rede
const char* ssid      = "b08-iot";
// const char* password  = "password";

const int led_internet_conectada = 13;
const int led_internet_desconectada = 12;


// URL de test (JSONPlaceholder é um serviço comum para testes de API)
const char* serverName = "https://jsonplaceholder.typicode.com/todos/1";

void setup() {
  // put your setup code here, to run once:
  // Led
  pinMode(led_internet_conectada, OUTPUT);
  pinMode(led_internet_desconectada, OUTPUT);

  Serial.begin(115200);    
  WiFi.begin(ssid);
  Serial.print("Conectando ao Wi-Fi: ");
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }
  Serial.print(tentativas);
  Serial.print("\n Conectado com sucesso");

}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() == WL_CONNECTED) {
    // Criamos o objeto cliente seguro
    WiFiClientSecure *client = new WiFiClientSecure;
    if (client) {
      // Em um cenario ral, voce usuario client->setCACertificate);
      // Para este exemplo simples, vamos ignorae a validação do certificado:
      client->setInsecure();

      HTTPClient https;

      Serial.print("[HTTPS] Iniciando requesição...\n");

      if (https.begin(*client, serverName)){

        // Realiza a GET
        int httpCode = https.GET();


        // httpCode será positivo se o servidor responder
        if (httpCode > 0) {
          Serial.printf("[HTTPS] Código de resposta: %d\n", httpCode);
          if (httpCode == HTTP_CODE_OK) {
            digitalWrite(led_internet_conectada, HIGH);
            digitalWrite(led_internet_desconectada, LOW);
            
            String payload = https.getString();
            Serial.println("Conteúdo recebido: ");
            Serial.println(payload);
          }
        } else {
          Serial.printf("[HTTPS] Falha na requesição, erro: %s\n", https.errorToString(httpCode).c_str());
          digitalWrite(led_internet_desconectada, HIGH);
          digitalWrite(led_internet_conectada, LOW);
        }

        https.end();
      } else {
        digitalWrite(led_internet_desconectada, HIGH);
        digitalWrite(led_internet_conectada, LOW);
        Serial.print("[HTTPS] Não foi possivel conectar ao servidor\n");
      }
      delete client; // liberar memória
    }
  }
  // Aguarde para a próxima leitura
  delay(10000);
}
