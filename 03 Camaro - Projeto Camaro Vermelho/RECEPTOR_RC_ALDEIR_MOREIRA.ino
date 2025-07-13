/*
 *                             RECEPTOR
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
#include <SPI.h>      // Biblioteca para comunicação SPI entre o módulo NRF24 e o Arduino.
#include <RF24.h>     // Biblioteca com as funções de controle do módulo NRF24.
#include <Servo.h>    // Biblioteca com as funções de controle do Servo Motor.

uint32_t amostra_tempo; // Declarando variável Global para armazenar o tempo da função millis

Servo myservo; // Criando um objeto do tipo Servo para controle do Servo Motor


const uint64_t pipe = 0xE8E8F0F0E1LL;  //Configurando endereço de comunicação entre os MÓDULOS. "LL" no final do endereço define a constante como sendo do tipo "LongLong".


RF24 radio(9, 10);  //Instanciando uma classe RF24 e informando que os pinos CE e CSN do Módulo NRF24L01 estão ligados aos pinos digitais 9 e 10 do Arduino


struct Meujoystick  {  //Criando uma estrutura de dados que contem as variaveis do tipo byte para cada função do controle remoto, no codigo do transmissor foi criada uma estrutura exatamente igual a esta para transmitir os dados.
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


Meujoystick joystick;   //declarando uma variavel do tipo Meujoystick (Estrutura que contrem todas as variaveis transmitidas pelo controle remoto )

// Declarando variaveis e niciando variaveis com valor 0, estas variáveis receberão os valores enviados pelo controle remto. 
byte VMotor = 0;                // Variavel que aciona o motor, seu valor vai de 0 a 255 de acordo com a posição do joystick de aceleração
byte VRe = 0;                   // Variavel que controla a Ré do veículo atravez da ponte H, seu valor é 0 ou 1, Ré ou Frente
byte VDirecao = 0;              // Variavel que controla o servo motor da direção, seu valor ficar entre 40 e 140
byte VBuzzer = 0;               // Variavel que controla o buzzer, buzina do veículo, seu valor é 0 ou 1
byte VFarol = 0;                // Variavel que controla os Leds do farol, seu valor e 0 ou 1
byte VSeta_Direita = 0;         // Variavel que controla os Leds da seta direita, seu valor e 0 ou 1
byte VSeta_Esquerda = 0;        // Variavel que controla os Leds da seta esquerda, seu valor e 0 ou 1
byte VPiscaAlerta = 0;          // Variavel que controla os Leds das setas direita e esquerda ao mesmo tempo, seu valor e 0 ou 1
byte VLuzes = 0;                // Variavel que controla todos os Leds piscando-os auternadamente, seu valor e 0 ou 1
byte VSirene = 0;               //Variavel que controla o buzzer, emitindo som de sirene, seu valor é 0 ou 1
int VIluminacao = 0;            // Variavel que controla os Leds de iluminação do veículo, seu valor e 0 ou 1
//int Temp = 0;

void setup()
{
  Serial.begin(9600);   //configurando a comunicação via porta serial à uma velocidade de 9600bps(baud).

  delay(1000);   //aguardando um tempo em milissegundos.

  Serial.println("Codigo RECEPTOR Nrf24L01"); //Imprimindo na tela via serial o texto entre aspas


  pinMode(2, OUTPUT);   // configurando o pino Digital 2 do Arduino como saida de dados, ele irá acender os Leds dos Faróis
  pinMode(3, OUTPUT);   // configurando o pino Digital 3 do Arduino como saida de dados, ele irá controlar o Servo Motor
  pinMode(4, OUTPUT);   // configurando o pino Digital 4 do Arduino como saida de dados, ele irá controlar o Buzzer
  pinMode(5, OUTPUT);   // configurando o pino Digital 5 do Arduino como saida de dados, ele irá controlar uma das entrada da Ponte H
  pinMode(6, OUTPUT);   // configurando o pino Digital 6 do Arduino como saida de dados, ele irá controlar a outra entrada da Ponte H
  pinMode(7, OUTPUT);   // configurando o pino Digital 7 do Arduino como saida de dados, ele irá controlar os Leds da seta direita
  pinMode(8, OUTPUT);   // configurando o pino Digital 8 do Arduino como saida de dados, ele irá controlar os Leds da seta esquerda
  pinMode(A0, OUTPUT);  // configurando o pino Analogico A0 do Arduino como saida de dados, nele deve ser ligado os leds responsaveis pela iluminação do veículo (as portas analógicas também podem ser utilizada para acesnder leds) )

  radio.begin();    //Inicialidando o MÓDULO RF24l01 para comunicação.

  radio.setAutoAck(false);    // Desativando pacotes ACK (Pacote de Reconhecimento de Mensagem)
  radio.setDataRate(RF24_250KBPS);   // Configurando Módulo para operar em uma taxa de 250kbps (Menor taxa de dados possível para um maior alcance do rádio)
  radio.setPALevel(RF24_PA_MAX);     // Configurando Módulo para transmitir em potencia máxima (a titulo de conhecimento, pois aqui o radio esta configurado apenas como receptor)
  radio.powerUp();                   // Ativando Módulo, caso entre em estado de baixo consumo. 
  radio.startListening();            //Inicializando o MÓDULO para 'escutar' as requisições enviadas pelo transmissor.

  Sirene_Lenta();  // Chamando função Sirene_Lenta que faz o buzzer emitir um som de sirene (semelhante a ambulância) * Como esta fora do loop so ocorerá quando o Arduino for ligado - usada para saber que esta tudo certo com o Arduino 
  for (int t = 0; t < 15; t ++) {
    Sirene_Rapida();  // Chamando função Sirene_Rapida que faz com que o buzzer emita um som de sirene (semelhante a carro de polícia) A função dentro de um "for" para que repita 15 fezes este som
  }
  digitalWrite(2, HIGH);           // as linas abaixo fazem com que os leds dos faróis e iluminação pisquem ao ligar o Arduino
  digitalWrite(A0, HIGH);
  delay(300);
  digitalWrite(2, LOW);
  digitalWrite(A0, LOW);
  delay(300);
  digitalWrite(2, HIGH);
  digitalWrite(A0, HIGH);
  delay(300);
  digitalWrite(2, LOW);
  digitalWrite(A0, LOW);
}

void loop()
{
  radio.startListening();  // Reforçando a inicialização de 'escuta'
  myservo.attach(3);  // Atribou servo ao pino digital 03 do arduino
  RX();   // Chamando a função RX responsavel pela recpção dos dados

}

void RX()   // função criada para recepção dos dados enviados pelo transmissor
{
  Serial.println(" Iniciando Recepcao Nrf24L01");   // Imprimindo na tela via serial o texto entre aspas
  radio.openReadingPipe(1, pipe); // abrindo o meio de comunicação entre transmissor e recptor com o mesmo endeço definido no ínicio do programa

  
  if ( radio.available() )  //verificando se há dados enviados pelo tranmissor.
  {

    boolean sinal = false;
    while (!sinal)  // Enquanto a variavel sinal for diferente de falso significa que os dados estão sendo recebidos corretamente
    {

      sinal = radio.read( &joystick, sizeof(joystick) );  // Recebendo dados enviado pelo transmissor
      VMotor = joystick.Motor;                   // As variaveis declaradas acima estão recebendo os dados enviado pelo tranmissor e que vieram encapsulados na struct Meujoystick
      VRe = joystick.Re;
      VDirecao = joystick.Direcao;               
      VBuzzer = joystick.Buzzer;                 
      VFarol = joystick.Farol;                   
      VSeta_Direita = joystick.Seta_Direita;     
      VSeta_Esquerda = joystick.Seta_Esquerda;   
      VPiscaAlerta = joystick.PiscaAlerta;
      VLuzes = joystick.Luzes;
      VSirene = joystick.Sirene;
      VIluminacao = joystick.Iluminacao;


      // Frente, Ré e Aceleração, controla a ponte H invertendo suas duas entradas de acordo com o valor da variável VRe
      if (VRe == 1) {
        analogWrite(5, VMotor);
        analogWrite(6, LOW);
      }
      else {
        analogWrite(5, LOW);
        analogWrite(6, VMotor);
      }

      // Direção, controla a posição que o braço de servo motor vai ficar de acordo com os valores da variavel VDireção
      myservo.write(VDirecao);

      // Buzina, aciona o buzzer produzindo som de acordo com o valor da variavel VBuzzer
      if (VBuzzer != 1) {
        tone(4, 600);
      } else {
        noTone(4);
      }
      // Farol, acende e apagar os leds dos faróis de acordo com o valor da variavel VFarol
      if (VFarol == 1) {
        digitalWrite(2, HIGH);
      }
      else {
        digitalWrite(2, LOW);
      }

      // Iluminação, acende e apagar os leds de iluminação do veículo de acordo com o valor da variavel VIluminacao
      if (VIluminacao == 1) {
        digitalWrite(A0, HIGH);
      }
      else {
        digitalWrite(A0, LOW);
      }

      // Seta Direita, faz os leds da seta direita piscarem ou não, de acordo com o valor da variavel VSeta_Direita
      digitalWrite(7, LOW);
      if (VSeta_Direita == 1) {      
        if (millis() - amostra_tempo > 400) {    // função millis no if faz acontagem de tempo que a seta deve ficar acesa 
          digitalWrite(7, HIGH);
        }
        if (millis() - amostra_tempo > 750) {   // função millis no if faz acontagem de tempo que a seta deve ficar apagada
          digitalWrite(7, LOW);
          amostra_tempo = millis();
        }
      }

      // Seta Esquerda, faz os leds da seta esquerda piscarem ou não, de acordo com o valor da variavel VSeta_Esquerda
      digitalWrite(8, LOW);
      if (VSeta_Esquerda == 1) {      
        if (millis() - amostra_tempo > 400) {   // função millis no if faz acontagem de tempo que a seta deve ficar acesa 
          digitalWrite(8, HIGH);
        }
        if (millis() - amostra_tempo > 750) {   // função millis no if faz acontagem de tempo que a seta deve ficar apagada
          digitalWrite(8, LOW);
          amostra_tempo = millis();
        }
      }

      // Pisca Alerta, faz os leds das setas direita e esquerda piscarem juntos ou não, de acordo com o valor da variavel VPiscaAlerta
      if (VPiscaAlerta == 1) {       // Pisca Alerta

        if (millis() - amostra_tempo > 400) {   // função millis no if faz acontagem de tempo que as setas devem ficar acesas
          digitalWrite(7, HIGH);
          digitalWrite(8, HIGH);
        }
        if (millis() - amostra_tempo > 750) {  // função millis no if faz acontagem de tempo que as setas devem ficar apagadas
          digitalWrite(7, LOW);
          digitalWrite(8, LOW);
          amostra_tempo = millis();
        }
      }


      // Luzes, faz todos os leds do veículo piscarem ou não, de acordo com o valor da variavel VLuzes
      if (VLuzes == 1) {      
        Luzes () ;
      }

      // Sirene, aciona o buzzer produzindo som de sirena de acordo com o valor da variavel VSirene
      if (VSirene == 1) {       // Sirene 2
        Sirene_Rapida ();
      }

// Abaixo seque um seria de prints que serviram para debug, você deve descomentar (retirar as //) das duas linhas responsaveis por imprimir no monitor serial o valor da variavel. 
      //Serial.print("       Motor: ");
      //Serial.println(VMotor);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug
      
      //Serial.print("          Re: ");
      //Serial.println(VRe);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug
      
      //Serial.print("     Direcao: ");
      //Serial.println(VDirecao);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug
      
      //Serial.print("      Buzzer: ");
      //Serial.println(VBuzzer);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug
      
      //Serial.print("       Farol: ");
      //Serial.println(VFarol);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug
     
      //Serial.print("Seta_Direita: ");
      //Serial.println(VSeta_Direita);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug
      
      //Serial.print("Seta_Esquerda: ");
      //Serial.println(VSeta_Esquerda);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug
     
      //Serial.print("  PiscaAlerta: ");
      //Serial.println(VPiscaAlerta);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug
     
      //Serial.print("       Luzes: ");
      //Serial.println(VLuzes);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug
     
      //Serial.print("       Sirene: ");
      //Serial.println(VSirene);      // Escreve na tela via serial o valor recebido. Pode ser usado para debug

    }
  }
  else  // Se nao existir dados enviados pelo transmissor ou o receptor por algum problema não conseguir recebelos
  {
    radio.stopListening();  // Interrompendo mecanismos de recpção "escuta" do Módulo
    Serial.println("Sem Sinal do Radio: Verifique se o transmissor esta ligado");   // Imprimindo na tela via serial o texto entre aspas

    if (millis() - amostra_tempo > 2000) { // Se passarem mais de 3 segundos sem conexão com o transmissor os motores são desligados automaticamente para evitar que o veículo ande sem controle
      tone(4, 300, 100);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      amostra_tempo = millis();
    }
  }
}

void Sirene_Lenta() {    // Função Sirene_Lenta que faz com que o buzzer emita um som de sirene (semelhante a ambulância)
  int frequencia = 0;

  digitalWrite(7, LOW);
  digitalWrite(8, LOW);

  for (frequencia = 100; frequencia < 2000; frequencia += 1) {
    tone(4, frequencia, 10);
    //delay(1);
  }
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);

  for (frequencia = 2000 ; frequencia > 100; frequencia -= 1) {
    tone(4, frequencia, 10);
    //delay(1);
  }
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
}
void Sirene_Rapida() {      //Função Sirene_Rapida que faz com que o buzzer emita um som de sirene (semelhante a um carro de polícia)
  int frequencia = 0;

  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);

  for (frequencia = 150; frequencia < 1800; frequencia += 40) {
    tone(4, frequencia, 10);
    delay(1);
  }
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);

  for (frequencia = 1800 ; frequencia > 150; frequencia -= 40) {
    tone(4, frequencia, 10);
    delay(1);
  }
}

void Luzes() {     // A função Luzes faz todas as luzes do veículo piscarem auternadamente
  if (millis() - amostra_tempo > 200) {
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(A0, LOW);
    digitalWrite(2, LOW);
  }
  if (millis() - amostra_tempo > 400) {
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(A0, HIGH);
    digitalWrite(2, HIGH);
  }
  if (millis() - amostra_tempo > 600) {
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(A0, LOW);
    digitalWrite(2, LOW);
    amostra_tempo = millis();
  }
}


//FIM
