#include <Adafruit_LiquidCrystal.h>
Adafruit_LiquidCrystal visor(0);
// Configuração das setas
byte up[] = {
    B00000,
    B00100,
    B01110,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000};

byte down[] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B01110,
    B00100,
    B00000};

int cont = 0;             // contador de capacidade
int quantPagantes = 0;    // Variavel para calcular valor ganho
int quantNaoPagantes = 0; // Para calcular os isentos
int quantPerd = 0;        // Variavel para calcular valor perdido ao final da linha
int chave = 13;
int valorChaveLigada = 0;
int ledAzul = 8;
int ledAmarelo = 9;
int ledVermelho = 10;
int valorGanho = 0;
int valorPerdido = 0;
int botaoDeExcesso = 2;
int botaoNaoPagantes = 7;
int nextState = 0; // estado da seta
int prevState = 0;
int tela = 0;
int test = 0;

const int capacidadeMaxima = 52;
// porta 5 e 6 sensor de entrada || porta 3 e 4 sensor de saida
const int PIN_ECHO = 5;
const int PIN_TRIGG = 6;

const int PIN_ECHO1 = 3;
const int PIN_TRIGG1 = 4;
const float valReferencia = 0.01723; // valor referencia para sensor de proximidade
const int NEXT = 11;
const int PREV = 12;

bool objPresente = false;
bool objPresente1 = false;

void setup()
{
    Serial.begin(9600);
    pinMode(chave, INPUT);
    pinMode(ledAzul, OUTPUT);
    pinMode(ledAmarelo, OUTPUT);
    pinMode(ledVermelho, OUTPUT);
    pinMode(botaoDeExcesso, INPUT);
    pinMode(botaoNaoPagantes, INPUT);

    pinMode(PIN_TRIGG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);

    pinMode(PIN_TRIGG1, OUTPUT);
    pinMode(PIN_ECHO1, INPUT);

    pinMode(NEXT, INPUT);
    pinMode(PREV, INPUT);

    mostraSeta();

    // visor lcd
    visor.begin(16, 2);
    visor.createChar(0, up);
    visor.createChar(1, down);
    visor.setBacklight(1);
}

void loop()
{
    valorChaveLigada = digitalRead(chave);

    if (digitalRead(chave) == 1)
    {
        ledsCapacidade();
        sensorHC();
        sensorSaidaHC();
        naoPagantes();
    }

    if (cont == capacidadeMaxima)
    {
        acimaDaCapacidade();
    }

    nextState = digitalRead(NEXT);
    prevState = digitalRead(PREV);

    if (nextState == HIGH)
    {
        tela++;
        if (tela >= 3)
        {
            tela = 2;
        }
        visor.clear();
        mostraSeta();
        mostrar();
    }

    if (prevState == HIGH)
    {
        tela--;
        if (tela < 0)
        {
            tela = 0;
        }
        visor.clear();
        mostraSeta();
        mostrar();
    }

    mostrar();
}

// Função para o sensor de entrada
void sensorHC()
{
    digitalWrite(PIN_TRIGG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIGG, LOW);
    long duracao = pulseIn(PIN_ECHO, HIGH);

    // distancia = (velocidadeSomNoAr / 2) * duracao;
    float distancia = valReferencia * duracao;
    Serial.println(distancia);

    if (distancia < 336 && objPresente == false)
    {
        objPresente = true;
    }
    else if (distancia >= 336 && objPresente == true)
    {
        objPresente = false;
        if (cont < capacidadeMaxima)
        {
            cont++;
            quantPagantes++;
            valorGanho = (quantPagantes * 4) - (quantNaoPagantes * 4);
        }
    }
}

// Função para o sensor de saida
void sensorSaidaHC()
{
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

void ledsCapacidade()
{
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
        visor.print("Onibus Lotado");
    }
}
// configurações da tela
void mostraSeta()
{
    Serial.println(tela);
    if (tela == 0)
    {
        visor.setCursor(0, 1);
        visor.write(0);
    }
    else
    {
        visor.setCursor(0, 0);
        visor.write(byte(0));
        visor.setCursor(0, 1);
        visor.write(byte(1));
    }
}

void mostrar()
{

    if (tela == 0)
    {
        visor.setCursor(0, 0);
        visor.print("Contagem");
        visor.setCursor(1, 1);
        visor.print(cont);
        visor.setCursor(9, 1);
        visor.print("CAP: 52");
    }
    else if (tela == 1)
    {

        visor.setCursor(1, 0);
        visor.print("V.Ganho");
        visor.setCursor(8, 0);
        visor.print("V.Perd");
        visor.setCursor(1, 1);
        visor.print(valorGanho);
        visor.setCursor(8, 1);
        visor.print(valorPerdido);
    }
    else if (tela == 2)
    {
        visor.setCursor(1, 0);
        visor.print("N.Pagantes");
        visor.setCursor(13, 0);
        visor.print(quantNaoPagantes);
    }
}

void acimaDaCapacidade()
{
    int botao = digitalRead(botaoDeExcesso);

    if (botao == HIGH)
    {

        quantPerd++;
        delay(10);
    }
}

void naoPagantes()
{
    int botao = digitalRead(botaoNaoPagantes);

    if (botao == HIGH)
    {

        quantNaoPagantes++;
        delay(10);
    }
}