void setup() {
  pinMode(13, OUTPUT);  // Pino 13 é geralmente conectado ao LED embutido
  Serial.begin(9600);  // Inicializa a comunicação serial a 9600 bps
}

void loop() {
  digitalWrite(13, HIGH);        // Liga o LED
  Serial.println("Ligado");
  delay(500);                    // Espera 500 ms
  digitalWrite(13, LOW);         // Desliga o LED
  Serial.println("DesLigado"); 
  delay(500);                    // Espera 500 ms
}
