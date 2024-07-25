#include <Adafruit_LiquidCrystal.h>
Adafruit_LiquidCrystal visor(0);
int cont = 0;          // contador de capacidade
int quantPagantes = 0; // Variavel para calcular valor ganho
int quantPerd = 0;     // Variavel para calcular valor perdido ao final da linha
int chave = 13;
int valorChaveLigada = 0;
int ledAzul = 8;
int ledAmarelo = 9;
int ledVermelho = 10;
int valorGanho = 0;
int valorPerdido = 0;
int botaoDeExcesso = 0;

// porta 5 e 6 sensor de entrada || porta 3 e 4 sensor de saida
const int PIN_ECHO = 5;
const int PIN_TRIGG = 6;

const int PIN_ECHO1 = 3;
const int PIN_TRIGG1 = 4;
const float valReferencia = 0.01723; // valor referencia para sensor de proximidade

bool objPresente = false;
bool objPresente1 = false;

void setup(){
    Serial.begin(9600);
    pinMode(chave, INPUT);
    pinMode(ledAzul, OUTPUT);
    pinMode(ledAmarelo, OUTPUT);
    pinMode(ledVermelho, OUTPUT);
    pinMode(botaoDeExcesso, INPUT);

    pinMode(PIN_TRIGG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);

    pinMode(PIN_TRIGG1, OUTPUT);
    pinMode(PIN_ECHO1, INPUT);

    // visor lcd
    visor.begin(16, 2);
    visor.setBacklight(1);
    visor.print(0, 0);
    visor.print("Contagem");
    visor.setCursor(0, 1);
    visor.print(cont);
    visor.setCursor(9, 1);
    visor.print("CAP: 52");
}

void loop(){
    valorChaveLigada = digitalRead(chave);

    if (digitalRead(chave) == 1)
    {
        ledsCapacidade();
        sensorHC();
        delay(100);

        sensorSaidaHC();
        delay(100);

        visor.setCursor(0, 1);
        visor.print("     "); // Limpa a linha anterior
        visor.setCursor(0, 1);
        visor.print(cont);
    }

    //CORRIGIR AMANHA COM O PROFESSOR
    if(cont == 52){
        acimaDaCapacidade();
    }

    if (digitalRead(chave) == 0)
    {
       finalViagem();
    }
}

// Função para o sensor de entrada
void sensorHC(){
    digitalWrite(PIN_TRIGG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIGG, LOW);
    long duracao = pulseIn(PIN_ECHO, HIGH);

    // distancia = (velocidadeSomNoAr / 2) * duracao;
    float distancia = valReferencia * duracao;

    if (distancia < 336 && objPresente == false)
    {
        objPresente = true;
    }
    else if (distancia >= 336 && objPresente == true)
    {
        objPresente = false;
        cont++;
        quantPagantes++;
    }
}

// Função para o sensor de saida
void sensorSaidaHC(){
    digitalWrite(PIN_TRIGG1, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIGG1, LOW);
    long duracao = pulseIn(PIN_ECHO1, HIGH);

    float distancia = valReferencia * duracao;
    Serial.println(distancia);

    if (distancia < 336 && objPresente1 == false)
    {
        objPresente1 = true;
    }
    else if (distancia >= 336 && objPresente1 == true)
    {
        objPresente1 = false;
        cont--;
    }
}

// Função para controle dos leds

void ledsCapacidade(){
    if (cont <= 40)
    {
        digitalWrite(ledAzul, HIGH);
        digitalWrite(ledAmarelo, LOW);
        digitalWrite(ledVermelho, LOW);
    }
    else if (cont > 40 && cont < 52)
    {
        digitalWrite(ledAzul, LOW);
        digitalWrite(ledAmarelo, HIGH);
        digitalWrite(ledVermelho, LOW);
    }
    else
    {
        digitalWrite(ledAzul, LOW);
        digitalWrite(ledAmarelo, LOW);
        digitalWrite(ledVermelho, HIGH);

        visor.setCursor(0, 0);
        visor.print(0, 0);
        visor.print("Onibus Lotado");
        
    }
}
//valores finais de viagem
void finalViagem(){
     valorGanho = quantPagantes * 4;
     int vPerdido = valorPerdido * 4;

         visor.setCursor(0, 0);
        visor.print("       "); 
        visor.setCursor(8, 0);
        visor.print("      ");
        visor.setCursor(0, 1);
        visor.print("      ");
        visor.setCursor(8, 1);
        visor.print("      ");

        visor.setCursor(0, 0);
        visor.print("V.Ganho"); 
        visor.setCursor(8, 0);
        visor.print("V.Perd");
        visor.setCursor(0, 1);
        visor.print(valorGanho);
        visor.setCursor(8, 1);
        visor.print(vPerdido);
        
}

//Função botao de excesso de passageiros
//CORRIGIR AMANHA COM O PROFESSOR
void acimaDaCapacidade(){
  botaoDeExcesso = digitalRead(2);
  
  if (botaoDeExcesso == HIGH) {
   
    valorPerdido++;
  } 
  delay(10); 
}
