/*                  CODIGO RECEPTOR - JEEP WILLYS V2.0 - NRF24L01+
                   ***************************************************

      **************************CANAL ALDEIR MOREIRA****************************



  Para conferir as Vídeo Aulas Gratuítas desse projeto, acesse:
  https://www.youtube.com/AldeirMoreira


  Faça um Controle Remoto para controlar o que quiser:https://www.arduinoparamodelismo.com
  

  Atenção Siga atentamente as instruções das vídeos aulas e e-book para a montagem do hardware

  Desenvolvido por Aldeir de Souza Moreira - aldeirsm@gmail.com
  C 2021 - Todos os direitos reservados
*/

 
 // Aprenda Arduino para Modelismo de Forma Rápida, Simples e Divertida!:https://www.arduinoparamodelismo.com

// Importando Bibliotecas
#include <RF24.h>     // Biblioteca com as funções de controle do módulo NRF24.
#include <Servo.h>    // Biblioteca com as funções de controle do Servo Motor.

uint32_t PiscaAlerta, SetaDireita, SetaEsquerda, amostra_tempo; 

Servo myservo; // Criando um objeto do tipo Servo para controle do Servo Motor

RF24 radio(7, 8);  

struct MeuControleRemoto  {  
  boolean Botao_0;
  boolean Botao_1;
  boolean Botao_2;
  boolean Botao_3;
  boolean Botao_4;
  boolean Botao_5;
  boolean Botao_6;
  boolean Botao_7;
  boolean Botao_JoyA;
  boolean Botao_JoyB;
  byte Joystick_AX;
  byte Joystick_AY;
  byte Joystick_BX;
  byte Joystick_BY;
  byte Pot_A;
  byte Pot_B;
};

MeuControleRemoto ControleRemoto; 
int cont = 0;

// Variáveis Globais para tratar os dados recedidos dos botões do Controle

boolean Botao0  = LOW;
boolean AuxBotao0 = LOW;
boolean Botao1  = LOW;
boolean AuxBotao1 = LOW;
boolean Botao2  = LOW;
boolean AuxBotao2 = LOW;
boolean Botao3  = LOW;
boolean AuxBotao3 = LOW;
boolean Botao4  = LOW;
boolean AuxBotao4 = LOW;
boolean Botao5  = LOW;
boolean AuxBotao5 = LOW;
boolean Botao6  = LOW;
boolean AuxBotao6 = LOW;
boolean Botao7  = LOW;
boolean AuxBotao7 = LOW;
boolean BotaoJoyA  = LOW;
boolean AuxBotaoJoyA = LOW;
boolean BotaoJoyB  = LOW;
boolean AuxBotaoJoyB = LOW;


void setup() {

  //****** Para uma melher performance comente a linha abaixo antes de carregar o código final para o Arduino

  //Serial.begin(115200);   //configurando a comunicação via porta serial à uma velocidade de 115200bps.

  delay(100);   

  Serial.println("*** CODIGO RECEPTOR - JEEP WILLYS V2.0 - NRF24L01+ ***"); 

  // Configurando Pinos do Arduino
  //pinMode(0, OUTPUT);   // Não está sendo usada neste projeto (PORTAS 0 E 1 NÃO DEVEM SER USADAS CASO QUEIRA UTILIZAR O MONITOR SERIAL)
  //pinMode(1, OUTPUT);   // Não está sendo usada neste projeto (PORTAS 0 E 1 NÃO DEVEM SER USADAS CASO QUEIRA UTILIZAR O MONITOR SERIAL)
  pinMode(2, OUTPUT);     // FAROL 
  pinMode(3, OUTPUT);     // LUZ DE FREIO 
  pinMode(4, OUTPUT);     // BUZINA 
  pinMode(5, OUTPUT);     // PONTE H 
  pinMode(6, OUTPUT);     // PONTE H 
  // pinMode(10, OUTPUT);  // Não está sendo usada neste projeto (PORTA DIGITAL 10 SEMPRE DEVE SER USADA COMO SAÍDA QUANDO USAMOS O MÓDULO NRF24L01)
  pinMode(A0, OUTPUT);    // SETA DIREITA 
  pinMode(A1, OUTPUT);    // SETA ESQUERDA 
  //pinMode(A2, OUTPUT);  // Não está sendo usada neste projeto
  //pinMode(A3, OUTPUT);  // Não está sendo usada neste projeto
  //pinMode(A4, OUTPUT);  // Não está sendo usada neste projeto
  //pinMode(A5, OUTPUT);  // Não está sendo usada neste projeto

  myservo.attach(9);  // SERVO MOTOR DIREÇÃO 

  radio.begin();                            
  radio.setAutoAck(false);                 
  radio.setChannel(100);                   
  radio.setDataRate(RF24_250KBPS);         
  radio.setPALevel(RF24_PA_HIGH);         
  radio.powerUp();                          
  radio.openReadingPipe(1, 0xE8E8F0F0E1LL); 
  radio.startListening();                  

  // Emite um bip ao ligar o receptor
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

  delay(15); 

  if (radio.available()) { 

    radio.read( &ControleRemoto, sizeof(ControleRemoto) );  

    // TRATANDO DADOS RECEBIDOS DO BOTÃO 0

    // USADO PARA PISCA-ALERTA
    if (ControleRemoto.Botao_0 == LOW) { 
      if (AuxBotao0 == LOW) {     
        AuxBotao0 = HIGH;         
        Botao0 = ! Botao0;       
        Serial.print("       Botao 0 Pressionado: ");
        Serial.println(Botao0);     
        digitalWrite(A0, LOW);  
        digitalWrite(A1, LOW);  

      }
    } else { 
      AuxBotao0 = LOW; 
    }

    if (Botao0 == HIGH) { 
      Botao1 = LOW;  
      Botao2 = LOW; 
      if (millis() -  PiscaAlerta > 500) {  
        digitalWrite(A0, !digitalRead(A0));
        digitalWrite(A1, !digitalRead(A1)); 
        tone(4, 80, 50);
        PiscaAlerta = millis();
      }
    }


    // TRATANDO DADOS RECEBIDOS DO BOTÃO 1
    // USADO PARA SETA PARA DIREITA

    if (ControleRemoto.Botao_1 == LOW) {
      if (AuxBotao1 == LOW) {
        AuxBotao1 = HIGH;
        Botao1 = ! Botao1;
        Serial.print("       Botao 1 Pressionado: ");
        Serial.println(Botao1);
        digitalWrite(A0, LOW);  
        digitalWrite(A1, LOW);  
      }
    } else {
      AuxBotao1 = LOW;
    }

    if (Botao1 == HIGH) {
      Botao2 = LOW;   
      if (millis() -  SetaDireita > 500) {
        digitalWrite(A0, !digitalRead(A0));
        tone(4, 80, 50);
        SetaDireita = millis();
      }
    }


    //   TRATANDO DADOS RECEBIDOS DO BOTÃO 2
    // **USADO PARA SETA ESQUERDA

    if (ControleRemoto.Botao_2 == LOW) {
      if (AuxBotao2 == LOW) {
        AuxBotao2 = HIGH;
        Botao2 = ! Botao2;
        Serial.print("       Botao 2 Pressionado: ");
        Serial.println(Botao2);
        digitalWrite(A0, LOW);  
        digitalWrite(A1, LOW);  
      }
    } else {
      AuxBotao2 = LOW;
    }


    if (Botao2 == HIGH) {
      Botao1 = LOW;   
      if (millis() -  SetaEsquerda > 500) {
        digitalWrite(A1, !digitalRead(A1));
        tone(4, 80, 50);
        SetaEsquerda = millis();
      }
    }



    // TRATANDO DADOS RECEBIDOS DO BOTÃO 3
    // NÃO ESTÁ SENDO USADO


    if (ControleRemoto.Botao_3 == LOW) {
      if (AuxBotao3 == LOW) {
        AuxBotao3 = HIGH;
        Botao3 = ! Botao3;
        Serial.print("       Botao 3 Pressionado: ");
        Serial.println(Botao3);
      }
    } else {
      AuxBotao3 = LOW;
    }



    // TRATANDO DADOS RECEBIDOS DO BOTÃO 4
    // USADO PARA FAROL E LANTERNA TRAZEIRA


    if (ControleRemoto.Botao_4 == LOW) {
      if (AuxBotao4 == LOW) {
        AuxBotao4 = HIGH;
        Botao4 = ! Botao4;
        Serial.print("       Botao 4 Pressionado: ");
        Serial.println(Botao4);
        digitalWrite(2, LOW);       
        digitalWrite(3, LOW);       
      }
    } else {
      AuxBotao4 = LOW;
    }

    if (Botao4 == HIGH) {
      digitalWrite(2, HIGH);       
      digitalWrite(3, HIGH);      
    }




    // TRATANDO DADOS RECEBIDOS DO BOTÃO 5
    // USADO PARA SIRENE


    if (ControleRemoto.Botao_5 == LOW) {
      if (AuxBotao5 == LOW) {
        AuxBotao5 = HIGH;
        Botao5 = ! Botao5;
        Serial.print("       Botao 5 Pressionado: ");
        Serial.println(Botao5);
        digitalWrite(2, LOW);       
        digitalWrite(3, LOW);        
        Sirene(); 
      }
    } else {
      AuxBotao5 = LOW;
    }


    // TRATANDO DADOS RECEBIDOS DO BOTÃO 6
    // USADO PARA SIRENE RAPIDA


    if (ControleRemoto.Botao_6 == LOW) {
      if (AuxBotao6 == LOW) {
        AuxBotao6 = HIGH;
        Botao6 = ! Botao6;
        Serial.print("       Botao 6 Pressionado: ");
        Serial.println(Botao6);
        digitalWrite(2, LOW);       
        digitalWrite(3, LOW);       
      }
    } else {
      AuxBotao6 = LOW;
    }

    if (Botao6 == HIGH) {
      SireneRapida();
    }



    // TRATANDO DADOS RECEBIDOS DO BOTÃO 7
    // BUZINA, é acionada ao pressionar o botão 7 no controle remoto


    if (ControleRemoto.Botao_7 == LOW) { 
      tone(4, 800); 
      Serial.print("       Botao 7 Pressionado: ");
      Serial.println(ControleRemoto.Botao_7);
    } else {
      noTone(4); 
    }



    // TRATANDO DADOS RECEBIDOS DO BOTÃO JOYSTICK A

    if (ControleRemoto.Botao_JoyA == LOW) {
      if (AuxBotaoJoyA == LOW) {
        AuxBotaoJoyA = HIGH;
        BotaoJoyA = ! BotaoJoyA;
        Serial.print("       Botao Joystick A Pressionado: ");
        Serial.println(BotaoJoyA);
      }
    } else {
      AuxBotaoJoyA = LOW;
    }


    // TRATANDO DADOS RECEBIDOS DO BOTÃO JOYSTICK B

    if (ControleRemoto.Botao_JoyB == LOW) {
      if (AuxBotaoJoyB == LOW) {
        AuxBotaoJoyB = HIGH;
        BotaoJoyB = ! BotaoJoyB;
        Serial.print("       Botao Joystick B Pressionado: ");
        Serial.println(BotaoJoyB);
      }
    } else {
      AuxBotaoJoyB = LOW;
    }


    //*************************************************************************************************************
    // TRATANDO DADOS DOS JOYSTICKS ANALÓGICOS PARA CONTROLE DO SERVO MOTOR E PONTE H


    int aux = 0; 

    // CONTROLE DE DIREÇÃO - controla a posição Do braço do servo motor de acordo com os movimentos no joystick no controle remoto
    aux = map(ControleRemoto.Joystick_BX, 0, 255, 40, 140); 
    myservo.write(aux); 

    //Serial.print("Direcao:     "); 
    //Serial.println(aux); 


    // Tratando valores recebidos dos joysticks para controle de frente ré e aceleração do motor
    // Os valores recebidos do joystick vão de 0 a 255, os ifs abaixo dividem este valor para controlar frente ré e parado.

    if (ControleRemoto.Joystick_AY >= 130) {
      aux = map(ControleRemoto.Joystick_AY, 130, 255, 20, 255); 
      analogWrite(5, aux);  
      analogWrite(6, LOW);     

      Serial.print("Frente: "); 
      Serial.println(aux);  

      digitalWrite(3, digitalRead(2));   

    }
    else if (ControleRemoto.Joystick_AY <= 125) { 
      aux = map(ControleRemoto.Joystick_AY, 125, 0, 20, 255);
      analogWrite(5, LOW);  
      analogWrite(6, aux);  

      Serial.print("Re:     "); 
      Serial.println(aux);  

      digitalWrite(3, HIGH);              

    }
    else {
     
      analogWrite(5, LOW);   
      analogWrite(6, LOW);   

      //Serial.println("PARADO "); 

      digitalWrite(3, digitalRead(2)); 

    }

    cont = 0;
  }
  else { 
    cont ++;
    if (cont > 10) { 
      cont = 11;
      Serial.println("Sem Sinal do Radio: Verifique se o transmissor esta ligado");  
      digitalWrite(5, LOW);  
      digitalWrite(6, LOW);  

      if (millis() - amostra_tempo > 5000) {
        tone(4, 700, 100);
        delay(100);
        tone(4, 1300, 100);
        amostra_tempo = millis();
      }
    }
  }
}
// FIM DO LOOP


// Funções usadas no Programa


void Sirene() {    // Função Sirene_Lenta que faz com que o buzzer emita um som de sirene (semelhante a ambulância)
  int frequencia = 0;

  analogWrite(5, LOW);              
  analogWrite(6, LOW);              

  
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);

  for (frequencia = 80; frequencia < 2200; frequencia += 1) {
    tone(4, frequencia, 10);
    delay(1);
  }
  for (frequencia = 2200 ; frequencia > 200; frequencia -= 1) {
    tone(4, frequencia, 10);
    delay(1);
  }
  // Desativando leds dos faroes e lanternas para efeito visual
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
}



void SireneRapida() {
  int frequencia = 0;

  // Ativando leds dos faroes e lanterna para efeito visual
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);

  for (frequencia = 150; frequencia < 1800; frequencia += 40) {
    tone(4, frequencia, 10);
    delay(1);
  }
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);

  for (frequencia = 1800 ; frequencia > 150; frequencia -= 40) {
    tone(4, frequencia, 10);
    delay(1);
  }
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
}
// FIM DO CÓDIGO
// FIM
