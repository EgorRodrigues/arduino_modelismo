

/*                  CODIGO TRANSMISSOR - PROJETO AEROBARCO RC - NRF24L01+
                    ***************************************************

      **************************CANAL ARDUINO PARA MODELISMO****************************

  https://www.youtube.com/ArduinoParaModelismo


  Aprende a fazer Controle Remoto para controlar o que quiser:https://www.arduinoparamodelismo.com
  
  Também nós siga no Instagram: 
  https://www.instagram.com/arduinoparamodelismo

  #Atenção! Siga atentamente as instruções das vídeos aulas e esquema elétrico para a montagem do hardware

  Desenvolvido por Aldeir de Souza Moreira - aldeirsm@gmail.com
  C 2021 - Todos os direitos reservados
*/


// Aprenda Arduino para Modelismo de Forma Rápida, Simples e Divertida!:https://www.arduinoparamodelismo.com

// Importando Bibliotecas
#include <RF24.h>    // Biblioteca com as funções de controle do módulo NRF24.

RF24 radio(9, 10);

struct MeuControleRemoto  {  //Criando uma estrutura de dados que contém as variáveis do tipo byte para cada função do controle remoto, no código do receptor foi criada uma estrutura exatamente igual a esta para receber os dados.
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
  byte Joystick_AY;
  byte Joystick_AX;
  byte Joystick_BX;
  byte Joystick_BY;
  byte Pot_A;
  byte Pot_B;
};

MeuControleRemoto ControleRemoto;

void setup() {

  //****** Para uma melhor performance comente a linha abaixo antes de carregar o código final para o Arduino (Retire os comentários apenas se precisar visualizar os valores no monitor serial)
  //Serial.begin(115200);   //configurando a comunicação via porta serial à uma velocidade de 115200bps.

  delay(100); // Pequno delay para incicialização da comunicação serial

  Serial.println("*** CODIGO TRANSMISSOR - PROJETO AEROBARCO RC - NRF24L01+ ***");

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
  pinMode(8, OUTPUT);        // Define que o pino digital 8 do arduino será uma saida de sinal para acionar o Buzzer

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

  readVcc(); // Função responsável por fazer a leitura da Tensão que chega ao arduino.

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

  ControleRemoto.Joystick_AX = map (analogRead(A1), 0, 1023, 0, 255);  //  Lendo os valores analógicos do potenciometro Eixo X do JoyStick A ligado na porta A1
  ControleRemoto.Joystick_AY = map (analogRead(A0), 0, 1023, 0, 255);  //  Lendo os valores analógicos do potenciometro Eixo Y do JoyStick A ligado na porta A0
  ControleRemoto.Joystick_BX = map (analogRead(A4), 0, 1023, 0, 255);  //  Lendo os valores analógicos do potenciometro Eixo X do JoyStick B ligado na porta A4
  ControleRemoto.Joystick_BY = map (analogRead(A5), 0, 1023, 0, 255);  //  Lendo os valores analógicos do potenciometro Eixo X do JoyStick B ligado na porta A5

  //********* LENDO POTENCIÔMETROS
  ControleRemoto.Pot_A = map (analogRead(A6), 0, 1023, 0, 255);  //  Lendo os valores analógicos do potenciometro A ligado na porta A6
  ControleRemoto.Pot_B = map (analogRead(A7), 0, 1023, 0, 255);  //  Lendo os valores analógicos do potenciometro B ligado na porta A7

  // Transmitindo variáveis, este comando foi repetido 10 vezes para corrigir erros que possam ocorrer na transmição.
  // Desta forma é possível um maior alcance com maior confiabilidade (esta repetição é necessária pois o ACK foi desativado)

  for (int i = 0; i < 10; i++) { // Laço de repetição
    radio.write( &ControleRemoto, sizeof(ControleRemoto) ); // comando que envia os dados
  }

  // Abaixo uma sequencia de prints para mostrar no Monitor Serial o valor lido nos joysticks e botões
  // Retire as barras // para visualizar no monitor serial o valor da variável que você deseja analisar

  //Serial.print("       Joystick_AX: ");
  //Serial.println(ControleRemoto.Joystick_AX);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  //Serial.print("       Joystick_AY: ");
  //Serial.println(ControleRemoto.Joystick_AY);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  //Serial.print("       Joystick_BX: ");
  //Serial.println(ControleRemoto.Joystick_BX);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  //Serial.print("       Joystick_BY: ");
  //Serial.println(ControleRemoto.Joystick_BY);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  //Potenciômetros

  //Serial.print("       Pot_A: ");
  //Serial.println(ControleRemoto.Pot_A);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  //Serial.print("       Pot_B: ");
  //Serial.println(ControleRemoto.Pot_B);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug


  // **ATENÇÃO! OS BOTÕES 0 e 1 ESTÃO LIGADOS NAS PORTAS 0 e 1 DO ARDUINO QUE SÃO USADAS TAMBÉM PELA COMUNICAÇÃO SERIAL, POR ISSO HAVERÁ PROBLEMAS AO VISUALIZAR O VALOR DESTES BOTÕES NA SERIAL

  //Serial.print("           Botao_0: ");
  //Serial.println(ControleRemoto.Botao_0);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  //Serial.print("           Botao_1: ");
  //Serial.println(ControleRemoto.Botao_1);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  //Serial.print("           Botao_2: ");
  //Serial.println(ControleRemoto.Botao_2);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  //Serial.print("           Botao_3: ");
  //Serial.println(ControleRemoto.Botao_3);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  //Serial.print("           Botao_4: ");
  //Serial.println(ControleRemoto.Botao_4);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  //Serial.print("           Botao_5: ");
  //Serial.println(ControleRemoto.Botao_5);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  //Serial.print("           Botao_6: ");
  //Serial.println(ControleRemoto.Botao_6);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  //Serial.print("           Botao_7: ");
  //Serial.println(ControleRemoto.Botao_7);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  //Serial.print("           Botao_JoyA: ");
  //Serial.println(ControleRemoto.Botao_JoyA);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  //Serial.print("           Botao_JoyB: ");
  //Serial.println(ControleRemoto.Botao_JoyB);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug
}


// FIM DO LOOP


//INÍCIO DAS FUNÇÕES

// Função responsavel por fazer a leitura da Tensão que chega ao arduino. (Só funciona quando o arduino é alimentado de 5,5 volts para baixo, neste caso a alimentação deve ser feita na porta 5v do Uno ou Nano e na porta vcc do pro mini)
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
