/*
              **********************************************
                          PROJETO JEEP WILLYS
                          CANAL ALDEIR MOREIRA
              **********************************************


  Para conferir as Vídeo Aulas Gratuítas desse projeto, acesse:
  https://www.youtube.com/AldeirMoreira

  Aprenda Arduino para modelismo:https://www.arduinoparamodelismo.com

   Atenção Siga atentamente as instruções das vídeos aulas e e-book para a montagem do hardware

  Desenvolvido por Aldeir de Souza Moreira - aldeirsm@gmail.com
  C 2021 - Todos os direitos reservados
*/


#include <Servo.h>  // Incluindo biblioteca Servo

Servo servo; // Criando Objeto Servo, para controle do Servo Motor

uint32_t amostra_tempo, PiscaAlerta, bip_tensao, temp;

int JoyAX = 255, JoyAY = 255;
int JoyBX = 90, JoyBY = 90;


// Variáveis Globais para auxiliar na identificação de quando um botão foi pressionado no celular
boolean  SetaD = false, SetaE = false, pisca = false, Sire = false, SireR = false;

float tensao = 0;

// Variável Global para armazenar os caracteres que o arduino receber do celular via módulo bluetooth
char DadosRecebidos;


void setup() {

  Serial.begin(9600); // Inicializando comunicação Serial Padrão nas portas 0 e 1 do arduino

  servo.attach(9);    // Informando que o servo motor está conectado ao pino digital 9 do Arduino

  pinMode(2, OUTPUT);  // Leds - Farol
  pinMode(3, OUTPUT);  // Leds - Lanterna/Luz Freio
  pinMode(4, OUTPUT);  // Buzzer - Buzinha
  pinMode(5, OUTPUT);  // Ponte H
  pinMode(6, OUTPUT);  // Ponte H

  pinMode(A0, OUTPUT);  // Leds - Seta Direita
  pinMode(A1, OUTPUT);  // Leds - Seta Esquerda

  // Emite um bip ao ligar o Jeep
  tone(4, 1000, 100);
  delay(100);
  tone(4, 800, 100);
  delay(100);
  tone(4, 600, 100);
  delay(100);
  tone(4, 400, 100);
  delay(100);
  tone(4, 200, 100);
  delay(100);
}

void loop() {

  DadosRecebidos = Serial.read(); // Variável DadosRecebidos armazena os dados recebidos da comunicação serial entre o celular e o Módulo Bluetooth

  if (DadosRecebidos > 0) {
    // Serial.println(DadosRecebidos);
  }

  // JOYSTICK A ACELERAÇÃO

  if (DadosRecebidos == 'A') {
    JoyAX = Serial.parseInt();
    while (DadosRecebidos != 'B') {
      if (Serial.available()) {
        DadosRecebidos = Serial.read();
        if (DadosRecebidos == 'Y') {
          JoyAY = Serial.parseInt();
        }
      }
    }
    // Serial.print("JoyAX: "); // Mostra no Monitor Serial
    // Serial.println(JoyAX); // Mostra no Monitor Serial os valores de JoyAX
    // Serial.print("JoyAY: "); // Mostra no Monitor Serial
    // Serial.println(JoyAY); // Mostra no Monitor Serial os valores de JoyAY
  }

  // JOYSTICK B DIREÇÃO

  if (DadosRecebidos == 'C') {
    JoyBX = Serial.parseInt();
    while (DadosRecebidos != 'D') {
      if (Serial.available()) {
        DadosRecebidos = Serial.read();
        if (DadosRecebidos == 'Y') {
          JoyBY = Serial.parseInt();
        }
      }
    }
    //Serial.print("JoyBX: "); // Mostra no Monitor Serial
    //Serial.println(JoyBX); // Mostra no Monitor Serial os valores de JoyBX
    //Serial.print("JoyBY: "); // Mostra no Monitor Serial
    //Serial.println(JoyBY); // Mostra no Monitor Serial os valores de JoyBY
  }




  // CONTROLE DE DIREÇÃO (SERVO MOTOR)
  servo.write(JoyBX);



  // CONTROLE DE ACELERAÇÃO, FRENTE, RÉ E PARADO


  int aux = 0;

  if (JoyAY <= 250) {                  // FRENTE
    aux = map(JoyAY, 250, 0, 50, 255);
    // Controle da Ponte H
    analogWrite(5, LOW);
    analogWrite(6, aux);
    Serial.print(" Frente PWM: ");
    Serial.println(aux);
    // Luz de Freio
    digitalWrite(3, digitalRead(2));

  } else if (JoyAY >= 260) {
    aux = map(JoyAY, 260, 510, 50, 255);
    // Controle da Ponte H
    analogWrite(5, aux);
    analogWrite(6, LOW);
    //Serial.print(" Re PWM: ");
    //Serial.println(aux);

    // Luz de Freio
    digitalWrite(3, HIGH);

  } else {               // MOTOR PARADO
    analogWrite(5, LOW); // desativa as duas portas da ponte h
    analogWrite(6, LOW); // desativa as duas portas da ponte h

    digitalWrite(3, digitalRead(2));
  }


  // FAROL e LANTERNA
  if (DadosRecebidos == 'F') {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    Serial.println(" Botão Farol Pressionado");
  }
  if (DadosRecebidos == 'f') {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
  }


  // BUZINA
  if (DadosRecebidos == 'E') {
    tone(4, 600);
    Serial.println("Botão Buzina Pressionado");
  } else if (DadosRecebidos == 'e') {
    noTone(4);
  }


  // SIRENE
  if (DadosRecebidos == 'S') {
    Sire = HIGH;
    Serial.println("Botão Sirene Pressionado");
  }

  if (Sire == HIGH) {
    Sirene();
  }
  if (DadosRecebidos == 's') {
    Sire = LOW;
    noTone(4);
  }


  // SIRENE RAPIDA
  if (DadosRecebidos == 'R') {
    SireR = HIGH;
    Serial.println("Botão Sirene Rapida Pressionado");
  }

  if (SireR == HIGH) {
    SireneRapida();
  }
  if (DadosRecebidos == 'r') {
    noTone(4);
    SireR = LOW;
  }

  // SETA DIREITA

  if (DadosRecebidos == 'H') {
    SetaD = !SetaD;
    digitalWrite(A0, LOW);
    SetaE = LOW;
    digitalWrite(A1, LOW);
  }

  if (SetaD == HIGH) {
    if (millis() - amostra_tempo > 500) {
      digitalWrite(A0, !digitalRead(A0));
      tone(4, 30, 30);
      amostra_tempo = millis();
    }
  }


  // SETA ESQUERDA

  if (DadosRecebidos == 'G') {
    SetaE = !SetaE;
    digitalWrite(A1, LOW);
    SetaD = LOW;
    digitalWrite(A0, LOW);
  }

  if (SetaE == HIGH) {
    if (millis() - amostra_tempo > 500) {
      digitalWrite(A1, !digitalRead(A1));
      tone(4, 30, 30);
      amostra_tempo = millis();
    }
  }

  // PISCA-ALERTA

  if (DadosRecebidos == 'P') {
    pisca = HIGH;
    SetaD = LOW;
    SetaE = LOW;
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);

  } else if (DadosRecebidos == 'p') {
    pisca = LOW;
    digitalWrite(A0, LOW);
    analogWrite(A1, LOW);

  }
  if (pisca == HIGH) {
    if (millis() - PiscaAlerta > 500) {
      digitalWrite(A0, !digitalRead(A0));
      digitalWrite(A1, !digitalRead(A1));
      tone(4, 30, 30);
      PiscaAlerta = millis();
    }
  }

  //Envia dados para o Celular

  if (millis() - temp > 2000) {
    readVcc();
    Serial.print("*T" + String(tensao) + "*");

    int bateria = map((tensao * 10), 34, 42, 0, 100);
    Serial.print("*Z" + String(bateria) + "*");
    temp = millis();
  }

  if (tensao < 3.6) {
    if (millis() - bip_tensao > 10000) {
      Serial.print("*W*");
      Serial.println("Tensao baixa, carregue a bateria");
      tone(4, 500, 100); // Emite um bip de alerta de tensão baixa
      delay(100);
      tone(4, 200, 200);
      delay(200);
      tone(4, 900, 100);
      bip_tensao = millis();
    }
  }


}

int readVcc() {
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Aguarde
  ADCSRA |= _BV(ADSC); // Inicia a conversão
  while (bit_is_set(ADCSRA, ADSC)); // Medindo
  uint8_t low = ADCL; // Vai ler ADCL primeiro - Então trava ADCH
  uint8_t high = ADCH; // Desbloqueia
  tensao = (high << 8) | low;
  tensao = 1125300L / tensao; // Calcular Vcc em milivolts; 1125300 = 1.1*1023*1000
  tensao = tensao / 1000;
  if (tensao > 4.2) {
    tensao = 4.2;
  }
  //Serial.println(tensao);
}



void Sirene() {    // Função Sirene faz com que o buzzer emita um som de sirene (semelhante a ambulância)
  int frequencia = 0;

  // Desativando motor enquanto a sirene estiver acionada, isso é feito para evitar que o Jeep Willys fique descontrolado, pois enquanto a função sirene é executado existe um atraso para o arduino ler as informações recebidas pelo módulo bluetooth
  analogWrite(5, LOW);              // desativa as duas portas da ponte h
  analogWrite(6, LOW);              // desativa as duas portas da ponte h

  // Ativando leds dos faroes e lanternas para efeito visual
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);

  for (frequencia = 100; frequencia < 2200; frequencia += 1) { // este laço for incrementa a variável frequencia, que é usada para definir o tom reproduzido pele buzzer de 100HZ a 2200Hz
    tone(4, frequencia, 10);
    delay(1);
    // comandos para interromper a execução da sirene
    DadosRecebidos = Serial.read(); // Variável DadosRecebidos armazena os dados recevidos da comunicação serial entre o celular e o Módulo Bluetooth
    if (DadosRecebidos == 's') {
      Sire = LOW;
      frequencia = 2200;
    }
  }

  for (frequencia = 2200 ; frequencia > 200; frequencia -= 1) {
    tone(4, frequencia, 10);
    delay(1);
    DadosRecebidos = Serial.read(); // Variável DadosRecebidos armazena os dados recevidos da comunicação serial entre o celular e o Módulo Bluetooth
    if (DadosRecebidos == 's') {
      Sire = LOW;
      frequencia = 200;
    }
  }
  // Desativando leds dos faroes e lanternas para efeito visual
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
}



void SireneRapida() {  // Função SireneRapida faz com que o buzzer emita um som de sirene (semelhante a de uma viatura de polícia)
  int frequencia = 0;

  //  Desativando motor enquanto a sirene estiver acionada
  analogWrite(5, LOW);              // desativa as duas portas da ponte h
  analogWrite(6, LOW);              // desativa as duas portas da ponte h

  for (int rep = 0; rep < 15; rep++) {


    for (frequencia = 80; frequencia < 2300; frequencia += 40) { // este laço for incrementa a variável frequencia de 40 em 40, que é usada para definir o tom reproduzido pele buzzer de 80HZ a 2300Hz
      tone(4, frequencia, 10);
      delay(1);
      DadosRecebidos = Serial.read(); // Variável DadosRecebidos armazena os dados recevidos da comunicação serial entre o celular e o Módulo Bluetooth
      if (DadosRecebidos == 'r') {
        SireR = LOW;
        frequencia = 1800;
        rep = 15;

      }
    }
    // Ativando leds dos faroes e lanternas para efeito visual
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);

    for (frequencia = 2300 ; frequencia > 80; frequencia -= 40) {
      tone(4, frequencia, 10);
      delay(1);
      DadosRecebidos = Serial.read(); // Variável DadosRecebidos armazena os dados recevidos da comunicação serial entre o celular e o Módulo Bluetooth
      if (DadosRecebidos == 'r') {
        SireR = LOW;
        frequencia = 150;
        rep = 15;
      }
    }
    // Desativando leds dos faroes e lanternas para efeito visual
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
  }
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
}
// FIM DO CÓDIGO
