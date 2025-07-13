
/*                  CODIGO TRANSMISSOR - PROJETO AEROBARCO RC - NRF24L01+
                    ***************************************************

      **************************CANAL ARDUINO PARA MODELISMO****************************

  https://www.youtube.com/ArduinoParaModelismo


  Aprende a fazer Controle Remoto para controlar o que quiser:https://www.arduinoparamodelismo.com

  Também nós siga no Instagram: 
  https://www.instagram.com/arduinoparamodelismo


  #Atenção Siga atentamente as instruções das vídeos aulas e esquema eletríco para a montagem do hardware

  Desenvolvido por Aldeir de Souza Moreira - aldeirsm@gmail.com
  C 2021 - Todos os direitos reservados
*/


// Aprenda Arduino para Modelismo de Forma Rápida, Simples e Divertida!:https://www.arduinoparamodelismo.com

// Importando Bibliotecas
#include <RF24.h>     // Biblioteca com as funções de controle do módulo NRF24.
#include <Servo.h>    // Biblioteca com as funções de controle do Servo Motor.

uint32_t Temp; // Variável para armazenar contagem de tempo em milis segundos usada para piscar os leds do barco para indicar falta de conexão com o controle remoto.

Servo myservo; // Instanciando objeto do tipo Servo para controle do Servo Motor

RF24 radio(7, 8); //Instanciando um objeto da classe RF24 e informando que os pinos CE e CSN do Módulo NRF24L01 estão ligados aos pinos digitais 7 e 8 do Arduino

struct MeuControleRemoto  { //Criando uma estrutura de dados que contém 16 variáveis do tipo boolean e byte para cada função do controle remoto, no código do Transmissor também foi criada uma estrutura exatamente igual a esta para enviar os dados.
  boolean Botao_0;
  boolean Botao_1;
  boolean Botao_2;
  boolean Botao_3;
  boolean Botao_4;
  boolean Botao_5;
  boolean Botao_6;
  boolean Botao_7;
  boolean Botao_JoyE;
  boolean Botao_JoyD;
  byte Joystick_EX;
  byte Joystick_EY;
  byte Joystick_DX;
  byte Joystick_DY;
  byte Pot_E;
  byte Pot_D;
};

MeuControleRemoto ControleRemoto;  //Declarando uma variável do tipo MeuControleRemoto (Estrutura que contém todas as variáveis que serão transmitidas para o receptor)
int cont = 0;

// Criando algumas variáveis Globais para tratar os dados recedidos dos botões do Controle

boolean BotaoJoyD  = LOW;
boolean AuxBotaoJoyD = LOW;


void setup() {

  //****** Para uma melhor performance comente a linha abaixo antes de carregar o código final para o Arduino (Retire os comentários apenas se precisar visualizar os valores no monitor serial)
  Serial.begin(115200);   //configurando a comunicação via porta serial à uma velocidade de 115200bps.

  delay(100);

  Serial.println("*** CODIGO RECEPTOR - PROJETO AEROBARCO RC - NRF24L01+ ***");

  //Configurando Pinos do Arduino

  pinMode(2, OUTPUT);     // LEDS DE ILUMINAÇÃO
  pinMode(5, OUTPUT);     // LIGADO AO MOSFET PARA ACELERAÇÃO DO MOTOR
  pinMode(A3, OUTPUT);    // LIGADO AO SERVO MOTOR RESPONSÁVEL PELA MOVIMENTAÇÃO DO LEME

  myservo.attach(A3);  // Vincula o Servo Motor à porta analógica A3 do Arduino

  radio.begin();
  radio.setAutoAck(false);
  radio.setChannel(100);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.powerUp();
  radio.openReadingPipe(1, 0xE8E8F0F0E1LL);
  radio.startListening();

}

void loop() {

  delay(5);

  if (radio.available()) {

    radio.read( &ControleRemoto, sizeof(ControleRemoto) );

    // TRATANDO DADOS RECEBIDOS DO BOTÃO JOYSTICK B (BOTÃO PRESENTE NO PRÓPRIO JOYSTICK)

    // BOTÃO USADO PARA ACENDER O LED LIGADO NA PORTA 2 DO ARDUINO (LEDS DE ILUMINAÇÃO DO BARCO)

    if (ControleRemoto.Botao_JoyD == LOW) {
      if (AuxBotaoJoyD == LOW) {
        AuxBotaoJoyD = HIGH;
        BotaoJoyD = ! BotaoJoyD;
        Serial.print("       Botao Joystick B Pressionado: ");
        Serial.println(BotaoJoyD);
      }
    } else {
      AuxBotaoJoyD = LOW;
    }


    if (BotaoJoyD == HIGH) { // Se o botõa no Joystick B foi pressiondao
      digitalWrite(2, HIGH); // Energisa porta digital 2 onde está ligado o led (Acende led)
    } else {                // Se NÃO, Se o botõa no Joystick B NÃO foi pressiondao
      digitalWrite(2, LOW); // Desenergisa porta digital 2 onde está ligado o led (Apaga led)
    }


    //*************************************************************************************************************
    // TRATANDO DADOS DOS JOYSTICKS ANALÓGICOS PARA CONTROLE DO SERVO MOTOR E PONTE H

    // CONTROLE DE LEME DO BARCO - controla a posição Do braço do servo motor de acordo com os movimentos no joystick no controle remoto


    // Tratando Ajuste de Trimagem do Servo Motor do Leme (Posição Central)usando Potenciômetro da Direita no controle

    int Pot = ControleRemoto.Pot_D; // Variável auxiliar do tipo int para rececer os valores do Potenciômetro da Direita no controle
    int Joy = ControleRemoto.Joystick_DX; // Variável auxiliar do tipo int para rececer os valores do Joystick eixo X da Direita no controle

    // Os ifs abaixo ajustam a posição central do servo motor onde esta ligado o leme do barco para mudar de acordo com os valores do Potenciômetro da Direita no controle
    if (Pot < 127) {
      Pot = map(Pot, 126, 0, 0, 126);
      Joy = Joy - Pot;
    }
    if (Pot >= 127) {
      Pot = map(Pot, 127, 255, 0, 128);
      Joy = Joy + Pot;
    }

    if (Joy >= 254) {
      Joy = 255;
    }

    if (Joy <= 1) {
      Joy = 0;
    }


    // Tratando Ajuste de Trimagem do Sevo do Leme (Amplitude de movimento)usando Potenciômetro da Esquerda

    int ValMin, ValMax; // Variáveis do tipo inteiro para auxiliar na trimagem da amplitude de movimento do servo motor

    // usando a função map para converter os valores de 0 a 255 do potenciometro para valores entre 0 e 180 para ajuste do funcionamento do servo motor do leme
    ValMin = map(ControleRemoto.Pot_E, 0, 255, 0, 90);
    ValMax = map(ControleRemoto.Pot_E, 0, 255, 180, 90);
    //    Serial.print("ValMin: ");
    //    Serial.println(ValMin);
    //    Serial.print("ValMax: ");
    //    Serial.println(ValMax);

    Joy = map(Joy, 0, 255, ValMin, ValMax); // usando a função map para converter o valores de 0 a 255 recebidos do joystick do controle para os valores de acordo com o movimento do potenciômetro da esquerda no controle, esse valores podem se de 0 a 180 para o melhor funcionamento o LEME do barco

    myservo.write(Joy); // Aciona o servo motor usado para o Leme do barco de acordo com os valores contidos na variável joy

    Serial.print("Leme:     "); // Imprime no Monitor Serial o texto entre aspas
    Serial.println(Joy);   // Imprime no Monitor Serial o valor da variável 'Joy'. Pode ser usado para debug


    // Tratando valores recebidos dos joysticks para controle de aceleração do motor
    // Os valores recebidos do joystick vão de 0 a 255, como o joystick fica posicionado no meio do percurso, os ifs abaixo fazem com que a aceleração aconteca com o movimento do bastão do meio para cima

    if (ControleRemoto.Joystick_EX >= 130) { // se o valor for maior que 130 (de 130 a 255), estamos empurando o joystick de aceleração para frente)
      ControleRemoto.Joystick_EX = map(ControleRemoto.Joystick_EX, 130, 255, 0, 255); // usando a função map para converter o valor de 130 a 255 para o padrão pwm de 0 a 255
      analogWrite(5, ControleRemoto.Joystick_EX); // aplicando sinal pwm na porta  digital 5 onde está ligado o mosfet que irá acionar o motor. O motor terá a velocidade proporcional ao valor da variável ControleRemoto.Joystick_EX

      Serial.print("Acelera: "); // Imprime no Monitor Serial o texto entre aspas
      Serial.println(ControleRemoto.Joystick_EX);   // Imprime no Monitor Serial o valor da variável 'aux'. Pode ser usado para debug

    } else { // Se o valor for menor que 130 significa que o bastão do joystick não foi empurado e o motor irá receber aceleração 0
      analogWrite(5, LOW); // Para o motor
    }

    cont = 0; //inciando a variável cont com o valor 0

  } else { // Se nao existir dados enviados pelo transmissor ou o receptor por algum problema não conseguir recebelos
    cont ++; // incrementa a variável cont
    if (cont > 20) {  // Se o Receptor ficar por 20 ciclos sem conexão com o transmissor, o motor é desligado automaticamente para evitar que o barco navegue desgovernado
      cont = 21; // mantem o valor de cont em 21 para evitar overflow
      Serial.println("Sem Sinal do Radio: Verifique se o transmissor esta ligado"); // Imprimindo no Monitor Serial o texto entre aspas
      digitalWrite(5, LOW); // Para o motor

      // Pisca o led de meio em meio segundo, indicando que não há conexão com o controle remoto
      if (millis() -  Temp > 500) {
        digitalWrite(2, !digitalRead(2));
        Temp = millis();
      }
    }
  }
}
// FIM DO CÓDIGO
