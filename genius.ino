// LEDs
int verde = 13;
int vermelho = 10;
int amarelo = 6;
int azul = 3;

// Botões
int botaoVerde = 12;
int botaoVermelho = 8;
int botaoAmarelo = 4;
int botaoAzul = 2;

// Estado dos botões
int estadoBotaoVerde = 0;
int estadoBotaoVermelho = 0;
int estadoBotaoAmarelo = 0;
int estadoBotaoAzul = 0;

// Variáveis de debounce
unsigned long ultimoDebounceTempoVerde = 0;
unsigned long ultimoDebounceTempoVermelho = 0;
unsigned long ultimoDebounceTempoAmarelo = 0;
unsigned long ultimoDebounceTempoAzul = 0;
const unsigned long debounceDelay = 50; 

int buzzer = 11;
int turno = 1;
bool comecouAutoPlay = false;
int inputAtual = 0;

int* lista = nullptr;
int* listaInput = nullptr;

void setup() {
  // LEDs
  pinMode(verde, OUTPUT);
  pinMode(vermelho, OUTPUT);
  pinMode(amarelo, OUTPUT);
  pinMode(azul, OUTPUT);

  // Botões
  pinMode(botaoVermelho, INPUT);
  pinMode(botaoVerde, INPUT);
  pinMode(botaoAmarelo, INPUT);
  pinMode(botaoAzul, INPUT);
  
  pinMode(buzzer, OUTPUT);
  
  Serial.begin(9600);

  unsigned long seed = 0;
  for (int i = 0; i < 32; i++) {
    seed = (seed << 1) | (analogRead(0) & 0x01);
    delay(1);
  }
  randomSeed(seed);
}

void criarListaNumeros() {
    if (lista == nullptr) {
        lista = new int[turno];
        lista[0] = random(0, 4);
    } else {
        int* novaLista = new int[turno];
        for (int i = 0; i < turno - 1; i++) {
            novaLista[i] = lista[i];
        }
        novaLista[turno - 1] = random(0, 4);
        delete[] lista;
        lista = novaLista;
    }
}

void autoPlay() {
  comecouAutoPlay = true;
  criarListaNumeros();
  for (int i = 0; i < turno; i++) {
    switch (lista[i]) {
      case 0:
        digitalWrite(verde, HIGH);
        tone(buzzer, 256, 250);
        delay(500);
        digitalWrite(verde, LOW);
        break;
      case 1:
        digitalWrite(vermelho, HIGH);
        tone(buzzer, 330, 250);
        delay(500);
        digitalWrite(vermelho, LOW);
        break;
      case 2:
        digitalWrite(amarelo, HIGH);
        tone(buzzer, 396, 250);
        delay(500);
        digitalWrite(amarelo, LOW);
        break;
      case 3:
        digitalWrite(azul, HIGH);
        tone(buzzer, 495, 250);
        delay(500);
        digitalWrite(azul, LOW);
        break;
    }
    delay(250);
  }
}

bool verificarInput() {
  if (listaInput[inputAtual - 1] != lista[inputAtual - 1]) {
    tone(buzzer, 50, 500);
    delay(500);
    digitalWrite(buzzer, LOW);
    turno = 1;
    inputAtual = 0;
    comecouAutoPlay = false;
    delete[] listaInput;
    listaInput = new int[turno];
    delete[] lista;
    lista = nullptr;
    delay(1000); 
    return false;
  }
  
  if (inputAtual == turno) {
    turno++;
    inputAtual = 0;
    comecouAutoPlay = false;
    delete[] listaInput;
    listaInput = new int[turno];
    delay(1000); 
  }
  
  return true;
}

void loop() {
  if (!comecouAutoPlay) {
    autoPlay();
    listaInput = new int[turno];
  }
  
  int leituraAtualVerde = digitalRead(botaoVerde);
  int leituraAtualVermelho = digitalRead(botaoVermelho);
  int leituraAtualAmarelo = digitalRead(botaoAmarelo);
  int leituraAtualAzul = digitalRead(botaoAzul);
  
  unsigned long tempoAtual = millis();

  if ((tempoAtual - ultimoDebounceTempoVerde) > debounceDelay) {
    if (leituraAtualVerde == HIGH) {
      ultimoDebounceTempoVerde = tempoAtual;
      digitalWrite(verde, HIGH);
      tone(buzzer, 256, 250);
      listaInput[inputAtual] = 0;
      inputAtual++;
      delay(500);
      digitalWrite(verde, LOW);
      if (!verificarInput()) return;
    }
  }
  
  if ((tempoAtual - ultimoDebounceTempoVermelho) > debounceDelay) {
    if (leituraAtualVermelho == HIGH) {
      ultimoDebounceTempoVermelho = tempoAtual;
      digitalWrite(vermelho, HIGH);
      tone(buzzer, 330, 250);
      listaInput[inputAtual] = 1;
      inputAtual++;
      delay(500);
      digitalWrite(vermelho, LOW);
      if (!verificarInput()) return; // Verifica input imediatamente e reinicia se errar
    }
  }

  if ((tempoAtual - ultimoDebounceTempoAmarelo) > debounceDelay) {
    if (leituraAtualAmarelo == HIGH) {
      ultimoDebounceTempoAmarelo = tempoAtual;
      digitalWrite(amarelo, HIGH);
      tone(buzzer, 396, 250);
      listaInput[inputAtual] = 2;
      inputAtual++;
      delay(500);
      digitalWrite(amarelo, LOW);
      if (!verificarInput()) return; // Verifica input imediatamente e reinicia se errar
    }
  }

  if ((tempoAtual - ultimoDebounceTempoAzul) > debounceDelay) {
    if (leituraAtualAzul == HIGH) {
      ultimoDebounceTempoAzul = tempoAtual;
      digitalWrite(azul, HIGH);
      tone(buzzer, 495, 250);
      listaInput[inputAtual] = 3;
      inputAtual++;
      delay(500);
      digitalWrite(azul, LOW);
      if (!verificarInput()) return; 
    }
  }
  
  delay(15);
}
