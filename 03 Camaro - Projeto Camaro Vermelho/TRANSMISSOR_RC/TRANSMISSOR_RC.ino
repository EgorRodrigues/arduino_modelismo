/*
                               TRANSMISSOR
              **********************************************
                          PROJETOS COM ARDUINO
                          CANAL ALDEIR MOREIRA
              **********************************************


  Para conferir o Vídeo desse Projeto, acesse:
  https://youtu.be/PdIoELMgKRM

  Aprenda Arduino para modelismo:https://www.arduinoparamodelismo.com

   Atenção Siga atentamente o esquema elétrico para a montagem do hardware

  Código adaptado do Projeto Automodelo
  Desenvolvido por Aldeir de Souza Moreira - aldeirsm@gmail.com
  C 2021 - Todos os direitos reservados
*/

// Importando Bibliotecas
#include <SPI.h>   // Biblioteca para comunicação SPI entre o módulo NRF24 e o Arduino.
#include <RF24.h>  // Biblioteca com as funções de controle do módulo NRF24.


#define JOYSTICK_X A0  // Declarando constante joystick_x como sendo porta analogica A0 do Arduino
#define JOYSTICK_y A1  // Declarando constante joystick_y como sendo porta analogica A1 do Arduino


const uint64_t pipe = 0xE8E8F0F0E1LL;  //Configurando endereço de comunicação entre os MÓDULOS. "LL" no final do endereço define a constante como sendo do tipo "LongLong".


RF24 radio(9, 10);  //Instanciando uma classe RF24 e informando que os pinos CE e CSN do Módulo NRF24L01 estão ligados aos pinos digitais 9 e 10 do Arduino

struct Meujoystick  {  //Criando uma estrutura de dados que contem as variaveis do tipo byte para cada função do controle remoto, no codigo do receptor foi criada uma estrutura exatamente igual a esta para receber os dados.
  byte Motor;
  byte Re;
  byte Direcao;
  byte Buzzer;
  byte Farol;
  byte Seta_Direita;
  byte Seta_Esquerda;
  byte PiscaAlerta;
  byte Luzes;
  byte Sirene;
  byte Iluminacao;
};

Meujoystick joystick; //declarando uma variavel do tipo Meujoystick (Estrutura que contrem todas as variaveis que serão transmitidas para o receptor)

int VServo = 0;
int Acelera = 0;
int VDirecao = 0;
int VAcelera = 0;
int VRe = 0;
int auxAcelera = 0;
int VLuzes = 0;
int VSirene = 0;
int VIluminacao = 0;

// Declarando variaveis do tipo booleana e iniciando com o valor logico false (serão usadas como auxilio para saber o valor anterios das variaveis que controlam os botões)
boolean AuxVFarol = false;
boolean AuxVSetaD = false;
boolean AuxVSetaE = false;
boolean AuxVPiscaA = false;
boolean AuxVLuzes = false;
boolean AuxVSirene = false;
boolean AuxVIluminacao = false;

void setup()
{

  Serial.begin(9600);   //configurando a comunicação via porta serial à uma velocidade de 9600bps(baud).

  delay(1000);// aguardando um tempo em milissegundos.

  Serial.println("Codigo TRANSMISSOR Nrf24L01");   // Imprimindo na tela via serial o texto entre aspas

  radio.begin();   //Inicialidando o MÓDULO RF24l01 para comunicação.

  radio.setAutoAck(false);            // Desativando pacotes ACK (Pacote de Reconhecimento de Mensagem)
  radio.setDataRate(RF24_250KBPS);    // Configurando Módulo para operar em uma taxa de 250kbps (Menor taxa de dados possível para um maior alcance do rádio)
  radio.setPALevel(RF24_PA_MAX);      // Configurando Módulo para transmitir em potencia máxima
  radio.powerUp();                    // Ativando Módulo, caso entre em estado de baixo consumo.

  // Inicializando variaveis da struct Meujoystick com o valor 0
  joystick.Motor = 0;
  joystick.Re = 0;
  joystick.Direcao = 0;
  joystick.Buzzer = 0;
  joystick.Farol = 0;
  joystick.Seta_Direita = 0;
  joystick.Seta_Esquerda = 0;
  joystick.PiscaAlerta = 0;
  joystick.Luzes = 0;
  joystick.Sirene = 0;
  joystick.Iluminacao = 0;

  // declarando o modo de operação dos pinos digitais 0 a 7 do Arduino onde foram ligados os pushboton do controle
  pinMode(0, INPUT_PULLUP); // Define que o pino digital 0 do arduino será uma entrada com resistor pull up interno ativado. BOTÃO TRIANGULO
  pinMode(1, INPUT_PULLUP); // Define que o pino digital 1 do arduino será uma entrada com resistor pull up interno ativado. BOTÃO X
  pinMode(2, INPUT_PULLUP); // Define que o pino digital 2 do arduino será uma entrada com resistor pull up interno ativado. BOTÃO QUADRADO
  pinMode(3, INPUT_PULLUP); // Define que o pino digital 3 do arduino será uma entrada com resistor pull up interno ativado. BOTÃO CIRCULO
  pinMode(4, INPUT_PULLUP); // Define que o pino digital 4 do arduino será uma entrada com resistor pull up interno ativado. BOTÃO SETA PARA CIMA
  pinMode(5, INPUT_PULLUP); // Define que o pino digital 5 do arduino será uma entrada com resistor pull up interno ativado. BOTÃO SETA PARA BAIXO
  pinMode(6, INPUT_PULLUP); // Define que o pino digital 6 do arduino será uma entrada com resistor pull up interno ativado. BOTÃO SETA PARA ESQUERDA
  pinMode(7, INPUT_PULLUP); // Define que o pino digital 7 do arduino será uma entrada com resistor pull up interno ativado. BOTÃO SETA PARA DIREITA
  pinMode(8, OUTPUT);       // Define que o pino digital 8 do arduino será uma saida de sinal para acionar o Buzzer

  Sirene(); // Chamando função Sirene que faz o buzzer emitir um som de sirene (semelhante a ambulância) * Como esta fora do loop so ocorerá quando o Arduino for ligado - usada para saber que esta tudo certo com o Arduino
}

void loop()
{

  radio.stopListening();  // Interrompendo mecanismos de recpção "escuta" do Módulo
  TX();    // Chamando a função TX responsavel pela transmissão dos dados

  int Tensao_Bateria = readVcc(); // Chamando a função responsavel por fazer a leitura da Tensão que chega ao arduino.
  if (Tensao_Bateria < 3100) { // Aciona o buzzer se atensão estiver menor que 3,1 volts para indicar que a bateria esta fraca
    tone(8, 900, 400);
  }
}


void TX()  // função criada para transmissão dos dados para o receptor
{
  Serial.println("Iniciando TRANSMICAO Nrf24L01"); // Imprimindo na tela via serial o texto entre aspas

  radio.openWritingPipe(pipe);   // abrindo o meio de comunicação entre transmissor e recptor com o mesmo endeço definido no ínicio do programa

  VServo = analogRead(JOYSTICK_X);  // Variavel VServo recebendo os valores analogicos do potenciometro Eixo X do JoyStick da Direita (Controle de direção)

  Acelera = analogRead(JOYSTICK_y);  // Variavel Acelera recebendo os valores analogicos do potenciometro Eixo Y do JoyStick da Esquerda (Controle de Aceleração)

  VDirecao = map (VServo, 0, 1023, 140, 40); // Convertendo valores analogicos recebidos do Joystick entre 0 e 1023 para entre 40 e 140 para controle da Direção
  joystick.Direcao = VDirecao; // Transferindo valores de direção já tratados para ser enviados ao receptor

  VAcelera = map (Acelera, 0, 1023, 0, 510); // Convertendo valores analogicos recebidos do Joystick entre 0 e 1023 para entre 0 e 510 para controle da Aceleração

  // Controlando Aceleração e sentido de rotação do motor  de acordo com a movimentação do eixo Y do JoyStick da Esquerda.
  if (VAcelera < 230) {
    VRe = 1;
    auxAcelera = 255 - VAcelera;

    if (auxAcelera > 245) {
      auxAcelera = 255;
    }
  }

  else if (VAcelera > 275) {
    VRe = 0;
    auxAcelera = VAcelera - 255;

    if (auxAcelera > 245) {
      auxAcelera = 255;
    }
  }
  else {
    auxAcelera = 0;
  }

  joystick.Re = VRe;             // Transferindo indicador Ré ou Frente para ser enviado ao receptor
  joystick.Motor = auxAcelera;  // Transferindo valores de aceleração já tratados para ser enviados ao receptor

  //********* Tratando botoes ***********

  // Farol
  int VFarol = digitalRead(0); //Verifica se o botão "X" do joystick ligado a porta digital 0 do arduino foi pressionado

  if (VFarol == LOW) {
    if (AuxVFarol == false) {
      AuxVFarol = true;
      joystick.Farol = joystick.Farol + 1;
      if (joystick.Farol > 1) {
        joystick.Farol = 0;
      }
    }
  }
  else {
    AuxVFarol = false;
  }

  //Iluminação
  int VIluminacao = digitalRead(5); //Verifica se o botão Seta para Baixo do joystick ligado a porta digital 5 do arduino foi pressionado

  if (VIluminacao == LOW) {
    if (AuxVIluminacao == false) {
      AuxVIluminacao = true;
      joystick.Iluminacao = joystick.Iluminacao + 1;
      if (joystick.Iluminacao > 1) {
        joystick.Iluminacao = 0;
      }
    }
  }
  else {
    AuxVIluminacao = false;
  }

  //Seta Direita
  int VSetaD = digitalRead(3); // Verifica se o botão Circulo do joystick ligado a porta digital 3 do arduino foi pressionado

  if (VSetaD == LOW) {
    if (AuxVSetaD == false) {
      AuxVSetaD = true;
      joystick.Seta_Direita = joystick.Seta_Direita + 1;
      joystick.Seta_Esquerda = 0;  // Se seta direita acende 1, seta esquerda a paga 0
      if (joystick.Seta_Direita > 1) {
        joystick.Seta_Direita = 0;
      }
    }
  }
  else {
    AuxVSetaD = false;
  }
  //Seta Esquerda
  int VSetaE = digitalRead(2); // Verifica se o botão Quadrado do joystick ligado a porta digital 2 do arduino foi pressionado

  if (VSetaE == LOW) {
    if (AuxVSetaE == false) {
      AuxVSetaE = true;
      joystick.Seta_Esquerda = joystick.Seta_Esquerda + 1;
      joystick.Seta_Direita = 0;  // Se seta esquerda acende 1, seta direita a paga 0
      if (joystick.Seta_Esquerda > 1) {
        joystick.Seta_Esquerda = 0;
      }
    }
  }
  else {
    AuxVSetaE = false;
  }

  //Pisca Alerta
  int VPiscaA = digitalRead(4); // Verifica se o botão Seta para Cima do joystick ligado a porta digital 4 do arduino foi pressionado

  if (VPiscaA == LOW) {
    if (AuxVPiscaA == false) {
      AuxVPiscaA = true;
      joystick.PiscaAlerta = joystick.PiscaAlerta + 1;
      joystick.Seta_Esquerda = 0;  // Se seta PiscaAlerta acende 1, seta esquerda apaga 0
      joystick.Seta_Direita = 0;  // Se seta PiscaAlerta acende 1, seta direita apaga 0
      if (joystick.PiscaAlerta > 1) {
        joystick.PiscaAlerta = 0;
      }
    }
  }
  else {
    AuxVPiscaA = false;
  }

  //Luzes
  VLuzes = digitalRead(6); // Verifica se o botão Seta para Esquerda do joystick ligado a porta digital 6 do arduino foi pressionado

  if (VLuzes == LOW) {
    if (AuxVLuzes == false) {
      AuxVLuzes = true;
      joystick.Luzes = joystick.Luzes + 1;
      if (joystick.Luzes > 1) {
        joystick.Luzes = 0;
      }
    }
  }
  else {
    AuxVLuzes = false;
  }

  //Sirene
  VSirene = digitalRead(7); // Verifica se o botão Seta para Direita do joystick ligado a porta digital 7 do arduino foi pressionado

  if (VSirene == LOW) {
    if (AuxVSirene == false) {
      AuxVSirene = true;
      joystick.Sirene = joystick.Sirene + 1;
      if (joystick.Sirene > 1) {
        joystick.Sirene = 0;
      }
    }
  }
  else {
    AuxVSirene = false;
  }

  //Buzzer
  int VBuzzer = digitalRead(1); // Verifica se o botão Triangulo do joystick, ligado a porta digital 1 do arduino foi pressionado
  joystick.Buzzer = VBuzzer;

  // Abaixo uma sequencia de linhas de código para imprimir na tela o valor da variaveis que serão enviadas para o veículo, você pode retirar as barras // para imprimir o valor da variavel que você tenha alguma dúvida

  Serial.print("       Motor: ");
  Serial.println(joystick.Motor);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  Serial.print("          Re: ");
  Serial.println(joystick.Re);          // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  Serial.print("     Direcao: ");
  Serial.println(joystick.Direcao);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  Serial.print("      Buzzer: ");
  Serial.println(joystick.Buzzer);       // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  Serial.print("       Farol: ");
  Serial.println(joystick.Farol);        // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  Serial.print("Seta_Direita: ");
  Serial.println(joystick.Seta_Direita);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  Serial.print("Seta_Esquerda: ");
  Serial.println(joystick.Seta_Esquerda);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  Serial.print("  PiscaAlerta: ");
  Serial.println(joystick.PiscaAlerta);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  Serial.print("       Luzes: ");
  Serial.println(joystick.Luzes);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

  Serial.print("       Sirene: ");
  Serial.println(joystick.Sirene);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug


  // Transmitindo variaveis, este comando foi repetido 30 vezes para corrigir erros que possam ocorer na transmição desta forma consigui-se um maior alcance com maior confiabilidade (esta repetição é necessária pois o ACK foi desativado)
  for (int i = 0; i < 30; i++) {
    radio.write( &joystick, sizeof(joystick) );
  }

}

// Função responsavel por fazer a leitura da Tensão que chega ao arduino.
int readVcc()
{
  // Ler referência interna 1.1V
  // Ajusta a referência ao Vcc e a medição de referência interna para 1.1V
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif
  delay(2); // Aguarde
  ADCSRA |= _BV(ADSC); // Inicia a conversão
  while (bit_is_set(ADCSRA, ADSC)); // Medindo
  uint8_t low = ADCL; // Vai ler ADCL primeiro - Então trava ADCH
  uint8_t high = ADCH; // Desbloqueia
  int result = (high << 8) | low;
  result = 1125300L / result; // Calcular Vcc em milivolts; 1125300 = 1.1*1023*1000
  return result; // Vcc em milivolts
}



void Sirene() {  // Função Sirene faz com que o buzzer emita um som de sirene (semelhante a ambulância)

  int frequencia = 0;
  for (frequencia = 150; frequencia < 1800; frequencia += 1) {
    tone(8, frequencia, 10);
    delay(1);
  }
  for (frequencia = 1800 ; frequencia > 100; frequencia -= 1) {
    tone(8, frequencia, 10);
    delay(1);
  }
  for (int temp = 0; temp < 15; temp ++) {
    for (frequencia = 150; frequencia < 1800; frequencia += 40) {
      tone(8, frequencia, 10);
      delay(1);
    }
    for (frequencia = 1800 ; frequencia > 150; frequencia -= 40) {
      tone(8, frequencia, 10);
      delay(1);
    }
  }
}
// FIM
