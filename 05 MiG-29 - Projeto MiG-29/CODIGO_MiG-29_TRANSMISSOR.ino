/*
  
                    *** PROJETO MIG-29 COM ARDUINO - CODIGO TRANSMISSOR ***
                    
                  (Atenção, este é o mesmo código do projeto Jeep Willys 2.0)
  
  
*** CANAL ARDUINO PARA MODELISMO https://www.youtube.com/ArduinoParaModelismo

*** CONHEÇA O CURSO DE ADUINO PARA MODELISMO: https://arduinoparamodelismo.com

*** CODIGO FONTE DO TRANSMISSOR NRF24L01 COM MÓDULO JOYSTICK (PLACA JLCPCB) OU FEITO PLACA PERFURADA
* 
*** VÍDEO COM A MONTAGEM DO MIG-29:
* 
*** VÍDEO COM A MONTAGEM DO CONTROLE: https://youtu.be/FMvNXXQfh0g

  Desenvolvido por Aldeir de Souza Moreira - aldeirsm@gmail.com / https://www.youtube.com/ArduinoParaModelismo
  C 2021 - Todos os direitos reservados
*/

// Aprenda Arduino para Modelismo de Forma Rápida, Simples e Divertida!:https://www.arduinoparamodelismo.com

// Importando Bibliotecas
// Baixe aqui a biblioteca RF24: https://drive.google.com/drive/folders/12MaCubNG8izH3jl_uYXyyA-hCkALP-X8?usp=sharing
#include <RF24.h>    // Biblioteca com as funções de controle do módulo NRF24.

RF24 radio(9, 10);

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

void setup() {


  //Serial.begin(115200);   //configurando a comunicação serial com velocidade de 115200bps.

  delay(100);// aguarda um tempo.

  Serial.println("*** CODIGO TRANSMISSOR - JEEP WILLYS V2.0 - NRF24L01+ ***");   

  radio.begin();
  radio.setAutoAck(false);
  radio.setChannel(100);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.powerUp();
  radio.openWritingPipe(0xE8E8F0F0E1LL);

  delay(100); // aguarda um tempo.

  // Definindo modo de operação dos pinos digitais 0 a 8 do Arduino onde foram ligados os botões ( push button do controle)
  pinMode(0, INPUT_PULLUP); // Define que o pino digital 0 do arduino será uma entrada com resistor pull up interno ativado. BOTÃO 0
  pinMode(1, INPUT_PULLUP); // Define que o pino digital 1 do arduino será uma entrada com resistor pull up interno ativado. BOTÃO 1
  pinMode(2, INPUT_PULLUP); // Define que o pino digital 2 do arduino será uma entrada com resistor pull up interno ativado. BOTÃO 2
  pinMode(3, INPUT_PULLUP); // Define que o pino digital 3 do arduino será uma entrada com resistor pull up interno ativado. BOTÃO 3
  pinMode(4, INPUT_PULLUP); // Define que o pino digital 4 do arduino será uma entrada com resistor pull up interno ativado. BOTÃO 4
  pinMode(5, INPUT_PULLUP); // Define que o pino digital 5 do arduino será uma entrada com resistor pull up interno ativado. BOTÃO 5
  pinMode(6, INPUT_PULLUP); // Define que o pino digital 6 do arduino será uma entrada com resistor pull up interno ativado. BOTÃO 6
  pinMode(7, INPUT_PULLUP); // Define que o pino digital 7 do arduino será uma entrada com resistor pull up interno ativado. BOTÃO 7
  pinMode(A2, INPUT_PULLUP); // Define que o pino analógico A2 do arduino será uma entrada digital com resistor pull up interno ativado. BOTÃO do joystick
  pinMode(A3, INPUT_PULLUP); // Define que o pino analógico A3 do arduino será uma entrada digital com resistor pull up interno ativado. BOTÃO do joystick
  pinMode(8, OUTPUT);        // Define que o pino digital 8 do arduino será uma saida para acionar o Buzzer

  // Emite um bip ao ligar o Jeep
  tone(8, 1000, 100);
  delay(100);
  tone(8, 800, 100);
  delay(100);
  tone(8, 600, 100);
  delay(100);
  tone(8, 400, 100);
  delay(100);
  tone(8, 200, 100);
  delay(100);
}

void loop() {

  readVcc(); 

  //********* LENDO Botões

  ControleRemoto.Botao_0 = digitalRead(0);
  ControleRemoto.Botao_1 = digitalRead(1);
  ControleRemoto.Botao_2 = digitalRead(2);
  ControleRemoto.Botao_3 = digitalRead(3);
  ControleRemoto.Botao_4 = digitalRead(4);
  ControleRemoto.Botao_5 = digitalRead(5);
  ControleRemoto.Botao_6 = digitalRead(6);
  ControleRemoto.Botao_7 = digitalRead(7);
  ControleRemoto.Botao_JoyA = digitalRead(A2);
  ControleRemoto.Botao_JoyB = digitalRead(A3);

  //********* LENDO JOYSTICKS

  ControleRemoto.Joystick_AX = map (analogRead(A1), 0, 1023, 0, 255);  
  ControleRemoto.Joystick_AY = map (analogRead(A0), 0, 1023, 0, 255);  
  ControleRemoto.Joystick_BX = map (analogRead(A4), 0, 1023, 0, 255);  
  ControleRemoto.Joystick_BY = map (analogRead(A5), 0, 1023, 0, 255);  

  //********* LENDO POTENCIÔMETROS
  ControleRemoto.Pot_A = map (analogRead(A6), 0, 1023, 0, 255);  
  ControleRemoto.Pot_B = map (analogRead(A7), 0, 1023, 0, 255);  

  // Transmitindo dados

  for (int i = 0; i < 10; i++) { // Laço de repetição
    radio.write( &ControleRemoto, sizeof(ControleRemoto) ); // comando que envia os dados
  }

  // Abaixo uma sequencia de prints para mostrar no Monitor Serial o valor lido nos joysticks e botões
  // Retire as barras // para visualizar no monitor serial o valor da variável que você deseja analisar

  //Serial.print("       Joystick_AX: ");
  //Serial.println(ControleRemoto.Joystick_AX);      

  //Serial.print("       Joystick_AY: ");
  //Serial.println(ControleRemoto.Joystick_AY);     

  //Serial.print("       Joystick_BX: ");
  //Serial.println(ControleRemoto.Joystick_BX);      

  //Serial.print("       Joystick_BY: ");
  //Serial.println(ControleRemoto.Joystick_BY);      

  //Potenciômetros

  //Serial.print("       Pot_A: ");
  //Serial.println(ControleRemoto.Pot_A);      

  //Serial.print("       Pot_B: ");
  //Serial.println(ControleRemoto.Pot_B);      


  // **ATENÇÃO! OS BOTÕES 0 e 1 ESTÃO LIGADOS NAS PORTAS 0 e 1 DO ARDUINO QUE SÃO USADAS TAMBÉM PELA COMUNICAÇÃO SERIAL, POR ISSO HAVERÁ PROBLEMAS AO VISUALIZAR O VALOR DESTES BOTÕES NA SERIAL (A SERIAL PODE TRAVAR AO PRESSIONAR O BOTÃO)

  //Serial.print("           Botao_0: ");
  //Serial.println(ControleRemoto.Botao_0);      

  //Serial.print("           Botao_1: ");
  //Serial.println(ControleRemoto.Botao_1);      

  //Serial.print("           Botao_2: ");
  //Serial.println(ControleRemoto.Botao_2);     

  //Serial.print("           Botao_3: ");
  //Serial.println(ControleRemoto.Botao_3);      

  //Serial.print("           Botao_4: ");
  //Serial.println(ControleRemoto.Botao_4);      

  //Serial.print("           Botao_5: ");
  //Serial.println(ControleRemoto.Botao_5);      

  //Serial.print("           Botao_6: ");
  //Serial.println(ControleRemoto.Botao_6);     

  //Serial.print("           Botao_7: ");
  //Serial.println(ControleRemoto.Botao_7);      

  //Serial.print("           Botao_JoyA: ");
  //Serial.println(ControleRemoto.Botao_JoyA);     

  //Serial.print("           Botao_JoyB: ");
  //Serial.println(ControleRemoto.Botao_JoyB);      
}


// FIM DO LOOP


//INÍCIO DAS FUNÇÕES


void readVcc()
{
  // Ler referência interna 1.1V
  // Ajusta a referência ao Vcc e a medição de referência interna para 1.1V
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Aguarde
  ADCSRA |= _BV(ADSC); // Inicia a conversão
  while (bit_is_set(ADCSRA, ADSC)); // Medindo
  uint8_t low = ADCL; // Vai ler ADCL primeiro - Então trava ADCH
  uint8_t high = ADCH; // Desbloqueia
  int result = (high << 8) | low;
  result = 1125300L / result; // Calcular Vcc em milivolts; 1125300 = 1.1*1023*1000

  if (result < 3100) { // Aciona o buzzer se a tensão da bateria estiver menor que 3,1 volts (para indicar que a bateria esta fraca)
    tone(8, 900, 100);
  }
}

// FIM
