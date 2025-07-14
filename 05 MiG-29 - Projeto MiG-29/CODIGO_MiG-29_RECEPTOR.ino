
/*
  
                    *** PROJETO MIG-29 COM ARDUINO - CODIGO RECEPTOR ***
  
  
*** CANAL ARDUINO PARA MODELISMO https://www.youtube.com/ArduinoParaModelismo

*** CONHEÇA O CURSO DE ADUINO PARA MODELISMO: https://arduinoparamodelismo.com

*** CODIGO FONTE DO RECEPTOR NRF24L01 PARA TRANSMISSOR COM MÓDULO JOYSTICK (PLACA JLCPCB) OU FEITO PLACA PERFURADA
* 
*** VÍDEO COM A MONTAGEM DO MIG-29:
* 
*** VÍDEO COM A MONTAGEM DO CONTROLE: https://youtu.be/FMvNXXQfh0g

  Desenvolvido por Aldeir de Souza Moreira - aldeirsm@gmail.com / https://www.youtube.com/ArduinoParaModelismo
  C 2021 - Todos os direitos reservados

 *** Siga atentamente esquema elétrico para a montagem da eletrônica
*/

// Importando Bibliotecas
// Baixe aqui a biblioteca RF24: https://drive.google.com/drive/folders/12MaCubNG8izH3jl_uYXyyA-hCkALP-X8?usp=sharing
#include <RF24.h>     // Biblioteca com as funções de controle do módulo NRF24.

uint32_t amostra_tempo; // Variáveis necessárias para contagem de tempo em milis segundos para os leds de sinalização

RF24 radio(7, 8);  //Instanciando um objeto da classe RF24 e informando que os pinos CE e CSN do Módulo NRF24L01 estão ligados aos pinos digitais 9 e 10 do Arduino

struct MeuControleRemoto  {  //Criando uma estrutura de dados que contém as variáveis do tipo byte para cada função do controle remoto  
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

MeuControleRemoto ControleRemoto; //declarando uma variável do tipo MeuControleRemoto (Estrutura que contém todas as variáveis que são enviadas pelo controle remoto)

int cont = 0;

void setup() {

  //****** Para uma melhor performance comente a linha abaixo antes de carregar o código final para o Arduino

  //Serial.begin(115200);   //configurando a comunicação via porta serial à uma velocidade de 115200bps.

  delay(100);   //aguardando um tempo em milissegundos.

  Serial.println("*** CODIGO RECEPTOR PROJETO AEROINO TUTORIAL - CURSO ARDUINO PARA MODELISMO ***"); //Imprimindo no Monitor Serial o texto entre aspas
  Serial.println("CODIGO RECEPTOR PARA TRANSMISSOR COM MÓDULO JOYSTICK E PLACA PERFURADA");
  // Configurando Pinos do Arduino
  pinMode(3, OUTPUT);     // MOTOR DIREITO  - configurando o pino Digital 3 do Arduino como saida de dados, nele será ligado o Mosfet para o Motor direito
  pinMode(6, OUTPUT);     // MOTOR ESQUERDO - configurando o pino Digital 6 do Arduino como saida de dados, nele será ligado o Mosfet para o Motor esquerdo
  

  radio.begin();                            // Inicialidando o MÓDULO RF24l01 para comunicação.
  radio.setAutoAck(false);                  // Desativando pacotes ACK (Pacote de Reconhecimento de Mensagem)
  radio.setChannel(100);                    // Configurando canal de transmissão do rádio (você pode escolher um canal de 0 a 127)
  radio.setDataRate(RF24_250KBPS);          // Configurando Módulo para operar em uma taxa de 250kbps (Menor taxa de dados possível para um maior alcance do rádio)
  radio.setPALevel(RF24_PA_HIGH);           // Configurando Módulo para transmitir em potência máxima
  radio.powerUp();                          // Ativando Módulo, caso entre em estado de baixo consumo.
  radio.openReadingPipe(1, 0xE8E8F0F0E1LL); // Abrindo o meio de comunicação entre transmissor e recptor com o mesmo endeço definido no ínicio do programa
  radio.startListening();                   // Configurando MÓDULO como receptor, para receber os dados enviados pelo transmissor.
}

void loop() {


  delay(15); // Necessário para a comunicação entre NRF24 e Arduino

  if (radio.available()) { //verificando se há dados enviados pelo tranmissor.

    radio.read( &ControleRemoto, sizeof(ControleRemoto) );  // Recebendo dados enviado pelo transmissor


    //*************************************************************************************************************
    // TRATANDO DADOS DOS JOYSTICKS ANALÓGICOS PARA CONTROLE DOS MOTORES DIREITO E ESQUERDO

    // Primeiro vamos tratar dos valores de aceleração do dois motores

    if (ControleRemoto.Joystick_BY < 130) { // Como vamos usar o joystick apenas do metade para cima, temos que garantir que se você puxar o joystick para baixo o seu valor nunca será menor que 130
      ControleRemoto.Joystick_BY = 130;
    }

    int aux = map(ControleRemoto.Joystick_BY, 130, 255, 0, 255); // usando a função map para converter o valor de 130 a 255 para o padrão pwm de 0 a 255
    int Mdireito = aux;
    int Mesquerdo = aux;


    // Agora vamos fazer com que ao movimentar o joystick direito, que fará o controle de direção, um motor receba mais aceleração que o outro, possibilitando as curvas.

    //*** Variável de Ajuste para curvas
    int Ajuste = 50; // Aumente o valor para curvas mais agrecivas, e diminua o valor para curvas mais suaves


    if (ControleRemoto.Joystick_AX >= 130) {

      // Tratando valores para o Aeromodelo virar para a Esquerda (Motor Direito acelera mais que o Motor Esquerdo)
      int auxMD = map(ControleRemoto.Joystick_AX, 130, 255, 0, Ajuste);

      Mdireito = Mdireito + auxMD;
      Mesquerdo = Mesquerdo - auxMD;

      // Sequencia de ifs para garantir que os valores não saiam do padrão PWM de 0 a 255
      if (Mdireito < 0) {
        Mdireito = 0;
      }
      if (Mdireito > 255) {
        Mdireito = 255;
      }

      if (Mesquerdo < 0) {
        Mesquerdo = 0;
      }
      if (Mesquerdo > 255) {
        Mesquerdo = 255;
      }
    }


    // Tratando valores para o Aeromodelo virar para a Direita(Motor Esquerdo acelera mais que o Motor Direito)
    if (ControleRemoto.Joystick_AX <= 125) {
      int auxME = map(ControleRemoto.Joystick_AX, 125, 0, 0, Ajuste);

      Mdireito = Mdireito - auxME;
      Mesquerdo = Mesquerdo + auxME;

      // Sequencia de ifs para garantir que os valores não saiam do padrão PWM de 0 a 255
      if (Mdireito < 0) {
        Mdireito = 0;
      }
      if (Mdireito > 255) {
        Mdireito = 255;
      }

      if (Mesquerdo < 0) {
        Mesquerdo = 0;
      }
      if (Mesquerdo > 255) {
        Mesquerdo = 255;
      }
    }

    // Aplicando valor PWM tratado nas portas digitais 3 e 6
    analogWrite(3, Mdireito);
    analogWrite(6, Mesquerdo);

    Serial.print("Motor Direito:     ");
    Serial.println(Mdireito);

    Serial.print("Motor Esquerdo:     ");
    Serial.println(Mesquerdo);

    cont = 0;
  }
  else { // Se não existir dados enviados pelo transmissor ou o receptor por algum problema não conseguir recebê-los
    cont ++;
    if (cont > 10) { // Se o Receptor ficar por 10 ciclos sem conexão com o transmissor, os motores são desligados automaticamente para evitar que o Aeromodelo continue voando desgovernado.
      cont = 11;
      Serial.println("Sem Sinal do Radio: Verifique se o transmissor esta ligado");   // Imprimindo no Monitor Serial o texto entre aspas
      digitalWrite(3, LOW);  // Desativa porta 3 (Desliga Motor)
      digitalWrite(6, LOW);  // Desativa porta 6 (Desliga Motor)
    }
  }
}
// FIM DO LOOP
