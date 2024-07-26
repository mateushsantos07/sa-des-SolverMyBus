#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal visor(0);

byte up[] = {
  B00000,
  B00100,
  B01110,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000
};

byte down[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B01110,
  B00100,
  B00000
};

const int PIN_ECHO = 5;
const int PIN_TRIGG = 6;
const int PIN_PING = 3;
const float VALOR_REFENCIA_HC = 0.01723;
const int NEXT = 4;
const int PREV = 2;

bool objetoPresente1 = false;
bool objetoPresente2 = false;
int qtaDoDia = 0;
int qtaAtualPessoas = 0;

int nextState = 0;
int prevState = 0;
int tela = 0;

int test = 0;

void setup(){
  	visor.begin(16,2);
	Serial.begin(9600);
  	pinMode(PIN_TRIGG, OUTPUT);
  	pinMode(PIN_ECHO, INPUT);
  	pinMode(2, INPUT);
  	pinMode(4, INPUT);
  	visor.createChar(0, up);
    visor.createChar(1, down);
	mostraSeta();
}

void loop(){
  nextState = digitalRead(NEXT);
  prevState = digitalRead(PREV);
  	
  if(nextState == HIGH){
  	tela++;
    if(tela >= 3){
		tela = 2;
    }
    visor.clear();
    mostraSeta();
    mostrar();
  }
  
  if(prevState == HIGH){
  	tela--;
    if(tela < 0){
		tela = 0;
    }
    visor.clear();
	mostraSeta();
    mostrar();
  }
  
  mostrar();
}

void mostraSeta() {
  if(tela == 0) {
    visor.setCursor(0, 1);
  	visor.write(byte(1));
  }else if(tela == 2) {
    visor.setCursor(0, 0);
  	visor.write(byte(0));
  }else {
    visor.setCursor(0, 0);
  	visor.write(byte(0));
     visor.setCursor(0, 1);
  	visor.write(byte(1));
  }
}

void mostrar(){
  if(tela == 0) {
    visor.setCursor(5,0);
    visor.print("TESTE 1");
  }else if(tela == 1){
    visor.setCursor(5,0);
    visor.print("TESTE 2");
  }else if(tela == 2) {
  	visor.setCursor(5,0);
    visor.print("TESTE 3");
  }
}	


void sensorHC(){
	digitalWrite(PIN_TRIGG, HIGH);
  	delayMicroseconds(10);
  	digitalWrite(PIN_TRIGG, LOW);
  	long duracao = pulseIn(PIN_ECHO, HIGH);
  		
  	float distancia = VALOR_REFENCIA_HC * duracao;
	
    if(distancia < 330&& objetoPresente2 == false) {
		objetoPresente2 = true;
    }else if(distancia >= 330 && objetoPresente2 == true) {
    	objetoPresente2 = false;
        qtaAtualPessoas--;
    }
}


void sensorPing(){
  	pinMode(PIN_PING, OUTPUT);
	digitalWrite(PIN_PING, HIGH);
  	delayMicroseconds(10);
  	digitalWrite(PIN_PING, LOW);
  	pinMode(PIN_PING, INPUT);
  	long duracao = pulseIn(PIN_PING, HIGH);
  		
  	float distancia = VALOR_REFENCIA_HC * duracao;
	
    if(distancia < 330&& objetoPresente1 == false) {
		objetoPresente1 = true;
    }else if(distancia >= 330 && objetoPresente1 == true) {
    	objetoPresente1 = false;
       	qtaAtualPessoas++;
      	qtaDoDia++;
    }
}







