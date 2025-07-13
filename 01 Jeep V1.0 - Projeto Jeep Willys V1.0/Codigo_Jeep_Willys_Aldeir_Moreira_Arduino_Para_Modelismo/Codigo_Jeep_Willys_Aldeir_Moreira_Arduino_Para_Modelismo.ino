/*
              **********************************************
                          PROJETO JEEP WILLYS
                      CANAL ARDUINO PARA MODELISMO
              **********************************************


  Para conferir as Vídeo Aulas Gratuítas desse projeto, acesse:
  https://www.youtube.com/ArduinoParaModelismo

  Playlist deste projeto:https://youtube.com/playlist?list=PLcWVEPpBZCFoA7HHRbi7hfrcuTXCfqtQz

  Aprenda Arduino para modelismo:https://www.arduinoparamodelismo.com

   Atenção Siga atentamente as instruções das vídeos aulas e e-book para a montagem do hardware
   
   Playlist com todos os vídeos deste projeto:https://youtube.com/playlist?list=PLcWVEPpBZCFoA7HHRbi7hfrcuTXCfqtQz
   
  Desenvolvido por Aldeir de Souza Moreira - aldeirsm@gmail.com
  C 2021 - Todos os direitos reservados
*/


#include <Servo.h>  // Incluindo biblioteca Servo

Servo servo; // Criando Objeto Servo, para controle do Servo Motor

uint32_t PiscaAlerta, bip_tensao, temp; // Variáveis necessárias para contagem de tempo em milis segundos (substitui a função delay) - (variáveis uint32_t são inteiros de 32 bits sem sinal)

int JoyAX = 255, JoyAY = 255; // Variáveis Globais para receberem os valores dos eixo X e Y do JoyStick A
int JoyBX = 90, JoyBY = 90;   // Variáveis Globais para receberem os valores dos eixo X e Y do JoyStick B
// As variáveis JoyAX e JoyBY não serão utilizadas neste código

// Variáveis Globais para auxiliar na identificação de quando um botão foi pressionado no celular
boolean pisca = 0, Sire = 0, SireR = 0;

float tensao = 0;

// Variável Global para armazenar os caracteres que o arduino receber do celular via módulo bluetooth
char DadosRecebidos;


void setup() {

  Serial.begin(9600); // Inicializando comunicação Serial Padrão nas portas 0 e 1 do arduino

  servo.attach(7);    // Informando que o servo motor está conectado ao pino digital 7 do Arduino

  pinMode(2, OUTPUT);  // Leds - Farol
  pinMode(3, OUTPUT);  // Leds - Lanterna/Luz Freio
  pinMode(4, OUTPUT);  // Buzzer - Buzinha
  pinMode(5, OUTPUT);  // Ponte H
  pinMode(6, OUTPUT);  // Ponte H

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

  if (DadosRecebidos > 0) { // Se a variável DadosRecebidos for maior que 0, signigica que alguma informação esta chegando na Serial
    // Serial.println(DadosRecebidos); // Mostra no Monitor Serial os dados recebidos do celular via Bluetooth
  }

  // JOYSTICK A ACELERAÇÃO
  //  JoyAX = 255;
  //  JoyAY = 255;
  // Neste trecho de código vamos tratar os dados recebidos via Bluetooth para do joystick virtual A (vamos usar apenas o eixo Y para controle de aceleração do motor)
  // Formato de dados enviados pelo Aplicativo: AX222,Y222B
  if (DadosRecebidos == 'A') {           // Verifica se o caractere recebido do celular é igual a 'A' Maiúsculo
    JoyAX = Serial.parseInt();           // Se sim, faz aconversão dos caractéres enviados após o 'A' para inteiro (converte os números de char para int) e armazena na variável JoyBX
    while (DadosRecebidos != 'B') {      // Enquanto os caracteres recebidos não forem iguais a 'B'
      if (Serial.available()) {          // Se houver atividade na comunicação serial (se o celular estiver enviado dodos para o Arduino)
        DadosRecebidos = Serial.read();  // Variável DadosRecebidos armazena os próximos dados enviados pelo celular via bluetooth
        if (DadosRecebidos == 'Y') {     // Se o caractere recebido do celular for igual a 'Y' Maiúsculo
          JoyAY = Serial.parseInt();     // A variável JoyBY recebe a aconversão dos caractéres enviados após o 'Y' para inteiro (converte os números de char para int)
        }
      }
    }
    // Serial.print("JoyAX: "); // Mostra no Monitor Serial
    // Serial.println(JoyAX); // Mostra no Monitor Serial os valores de JoyAX
    // Serial.print("JoyAY: "); // Mostra no Monitor Serial
    // Serial.println(JoyAY); // Mostra no Monitor Serial os valores de JoyAY
  }

  // JOYSTICK B DIREÇÃO
  //  JoyBX = 90;
  //  JoyBY = 90;
  // Neste trecho de código vamos tratar os dados recebidos via Bluetooth para o joystick virtual 2 (vamos usar apenas o eixo X para controle do servo de direção)
  // Formato de dados enviados pelo Aplicativo: CX22,Y22D
  if (DadosRecebidos == 'C') {          // Verifica se o caractere recebido do celular é igual a 'C' Maiúsculo
    JoyBX = Serial.parseInt();          // Se sim, faz aconversão dos caractéres enviados após o 'C' para inteiro (converte os números de char para int) e armazena na variável JoyBX
    while (DadosRecebidos != 'D') {     // Enquanto os caracteres recebidos não forem iguais a 'D'
      if (Serial.available()) {         // Se houver atividade na comunicação serial (se o celular estiver enviado dados para o Arduino)
        DadosRecebidos = Serial.read(); // Variável DadosRecebidos armazena os próximos dados enviados pelo celular via bluetooth
        if (DadosRecebidos == 'Y') {    // Se o caractere recebido do celular for igual a 'Y' Maiúsculo
          JoyBY = Serial.parseInt();    // A variável JoyBY recebe a aconversão dos caractéres enviados após o 'Y' para inteiro (converte os números de char para int)
        }
      }
    }
    //Serial.print("JoyBX: "); // Mostra no Monitor Serial
    //Serial.println(JoyBX); // Mostra no Monitor Serial os valores de JoyBX
    //Serial.print("JoyBY: "); // Mostra no Monitor Serial
    //Serial.println(JoyBY); // Mostra no Monitor Serial os valores de JoyBY
  }




  // CONTROLE DE DIREÇÃO (SERVO MOTOR)
  servo.write(JoyBX); // Configura a posição do Servo de acordo com a posição do joystick (valor da variável JoyBX)



  // CONTROLE DE ACELERAÇÃO, FRENTE, RÉ E PARADO
  // Para a elaboração do controle de aceleração, frente e ré foram levados em consideração as configurações feitas no aplicativo,
  // que envia o valor minimo de 0 e o valor máximo 510, tendo o valor central do envio em 255

  int aux = 0; // variável para auxiliar na converasão dos valores do joystick para o padrão PWM de 0 a 255

  if (JoyAY <= 250) {                  // FRENTE - quando o bastão do joystick virtual é empurado para cima (posição central é = 255, todo para cima = 0)
    aux = map(JoyAY, 250, 0, 50, 255); // Converte os valores do joystick para o padrão PWM de 0 a 255 (neste caso iniciei em 20 pois valores abaixo disso não conseguem acionar o motor)
    // Controle da Ponte H
    analogWrite(5, LOW);               // ativa pino ponte H com valor LOW para que o veículo se movimente para frente
    analogWrite(6, aux);               // ativa pino ponte H com o valor da variável Pot (controle de aceleração)
    Serial.print(" Frente PWM: "); // Mostra no Monitor Serial
    Serial.println(aux); // Mostra no Monitor Serial os valores de aux
    // Luz de Freio
    digitalWrite(3, digitalRead(2));   // Se o motor for acionada para Frente, verifica se o farol (D2) esta aceso, se estiver mantém a Lanterna (D3) acesa, se não apaga

  } else if (JoyAY >= 260) {             // RÉ - quando o bastão do joystick virtual é empurado para baixo (posição central é = 255, todo para baixo = 510)
    aux = map(JoyAY, 260, 510, 50, 255); // Converte os valores do joystick para o padrão PWM de 0 a 255 (neste caso iniciei em 20 pois valores abaixo disso não conseguem acionar o motor)
    // Controle da Ponte H
    analogWrite(5, aux);                 // ativa pino ponte H com o valor da variável Pot (controle de aceleração)
    analogWrite(6, LOW);                 // ativa pino ponte H com valor LOW para que o veículo se movimente para trás
    //Serial.print(" Re PWM: "); // Mostra no Monitor Serial
    //Serial.println(aux); // Mostra no Monitor Serial os valores de aux

    // Luz de Freio
    digitalWrite(3, HIGH);               // Acende Lanterna Trazeira (D3) ao acionar a Ré

  } else {               // MOTOR PARADO
    analogWrite(5, LOW); // desativa as duas portas da ponte h
    analogWrite(6, LOW); // desativa as duas portas da ponte h

    digitalWrite(3, digitalRead(2)); // Se o motor estiver parado, verifica se o farol (D2) esta aceso, se estiver mantém a Lanterna (D3) acesa, se não apaga
  }


  // FAROL e LANTERNA
  if (DadosRecebidos == 'F') {   // Verifica se o caractere enviado do celular, e recebido pelo módulo Bluetooth, é igual a letra "F" maiúsculo
    digitalWrite(2, HIGH);        // Apaga Farol
    digitalWrite(3, HIGH);        // Apaga Lanterna
    Serial.println(" Botão Farol Pressionado");
  }
  if (DadosRecebidos == 'f') {           // Se Farol for igual a HIGH entra no if ( HIGH, true, 1 significa verdadeirao o ativo já LOW, fase, 0 significar falso ou desativado) você pode usar qualuer uma das três opções que funcionará
    digitalWrite(2, LOW);       // Acende Farol
    digitalWrite(3, LOW);       // Acende lanterna traseira
  }


  // BUZINA
  if (DadosRecebidos == 'E') {         // Verifica se o caractere enviado do celular e recebido pelo módulo Bluetooth é igual a letra L maiúsculo
    tone(4, 600);                      // Gera um tom com 600 Hz na porta  4 onde está ligado o Buzzer enquanto o botão estver sendo precionado
    Serial.println("Botão Buzina Pressionado");
  } else if (DadosRecebidos == 'e') {  // Verifica se o caractere enviado do celular e recebido pelo módulo Bluetooth é igual a letra f minúsculo
    noTone(4);                         // Faz parar todos os tons gerados na porta  4 onde está ligado o Buzzer
  }


  // SIRENE
  if (DadosRecebidos == 'S') {         // Verifica se o caractere enviado do celular e recebido pelo módulo Bluetooth é igual a letra S maiúsculo
    Sire = HIGH;                      // A variável booleana Sire recebe o contrario de seu valor atual (se era 0 recebe 1 e vice-versa)
    Serial.println("Botão Sirene Pressionado");
  }

  if (Sire == HIGH) {                  // Se Sire for igual a HIGH entra no if ( HIGH, true, 1 significa verdadeirao o ativo já LOW, fase, 0 significar falso ou desativado) você pode usar qualuer uma das três opções que funcionará
    Sirene();                          // Chamada a função Sirene() implementada no final do código
  }
  if (DadosRecebidos == 's') {  // Verifica se o caractere enviado do celular e recebido pelo módulo Bluetooth é igual a letra f minúsculo
    Sire = LOW;
    noTone(4); // Faz parar todos os tons gerados na porta  4 onde está ligado o Buzzer
  }


  // SIRENE RAPIDA
  if (DadosRecebidos == 'R') {         // Verifica se o caractere enviado do celular e recebido pelo módulo Bluetooth é igual a letra R maiúsculo
    SireR = HIGH;
    Serial.println("Botão Sirene Rapida Pressionado");
  }

  if (SireR == HIGH) {
    SireneRapida();
  }
  if (DadosRecebidos == 'r') {  // Verifica se o caractere enviado do celular e recebido pelo módulo Bluetooth é igual a letra f minúsculo
    noTone(4); // Faz parar todos os tons gerados na porta  4 onde está ligado o Buzzer
    SireR = LOW;
  }



  // PISCA-ALERTA

  if (DadosRecebidos == 'P') { // Verifica se o caractere enviado do celular, e recebido pelo módulo Bluetooth, é igual a letra "E" maiúsculo
    pisca = HIGH;
  } else if (DadosRecebidos == 'p') {
    pisca = LOW;
    digitalWrite(2, LOW);   // Apaga Farol
    analogWrite(3, LOW);    // Apaga Lanterna

  }
  if (pisca == HIGH) {
    if (millis() - PiscaAlerta > 500) { // função millis no if faz a contagem de tempo que a seta deve ficar acesa, neste caso 500 milissegundos (Acende/Apaga led a cada meio segundo)
      digitalWrite(2, !digitalRead(2));
      digitalWrite(3, !digitalRead(3));
      tone(4, 30, 30);
      PiscaAlerta = millis();           // variável PiscaAlerta recebe o tempo atual
    }
  }

  //Envia dados para o Celular

  if (millis() - temp > 2000) {
    readVcc(); // Chamada a função que lê a tensão da bateria
    Serial.print("*T" + String(tensao) + "*");  // Envia via bluetooth para o celular a Tensão da Bateria, em um formato adequado para a leitura do aplicativo.

    int bateria = map((tensao * 10), 34, 42, 0, 100); // Converte a tensão da bateria em porcentagem (3 volts igual a 0% da bateria 4.2 volts igual a 100% da bateria).
    Serial.print("*Z" + String(bateria) + "*"); // Envia via bluetooth para o celular o valor útil em porcentagem da bateria, em um formato adequado para a leitura do aplicativo.
    temp = millis(); // variável bip_tensao recebe o tempo atual
  }

  if (tensao < 3.6) { // verifica se a tensão da bateria é menor que
    if (millis() - bip_tensao > 10000) {// função millis no if faz a contagem de tempo para que seja emitido um bip de alerta para tensão baixa da bateria, neste caso 5000 milissegundos (emite um bip no buzzer a cada 5 segundo)
      Serial.print("*W*");
      Serial.println("Tensao baixa, carregue a bateria");
      tone(4, 500, 100); // Emite um bip de alerta de tensão baixa
      delay(100);
      tone(4, 200, 200);
      delay(200);
      tone(4, 900, 100);
      bip_tensao = millis(); // variável bip_tensao recebe o tempo atual
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
