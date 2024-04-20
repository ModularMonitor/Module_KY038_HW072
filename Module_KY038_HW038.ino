void setup() {
  // Inicializa a comunicação serial
  Serial.begin(9600);

  // Define o pino do sensor como entrada
  pinMode(15, INPUT);
  pinMode(2, INPUT);
}

void loop() {
  // Lê a tensão do sensor
  int KY038 = analogRead(15);

  // Converte a tensão em dB
  float dbValue = 20 * log10(KY038 / 1023.0);

  // Exibe o valor em dB no Serial Monitor
  Serial.print("Intensidade do som: ");
  Serial.print(dbValue);
  Serial.println(" dB");

  int HW072 = analogRead(2);

  Serial.print("Analog reading = ");
  Serial.print(HW072);

  if (HW072 < 100) {
    Serial.println(" - Very bright");
  } else if (HW072 < 200) {
    Serial.println(" - Bright");
  } else if (HW072 < 500) {
    Serial.println(" - Light");
  } else if (HW072 < 800) {
    Serial.println(" - Dim");
  } else {
    Serial.println(" - Dark");
  }

  // Aguarda 1 segundo
  delay(1000);
}
