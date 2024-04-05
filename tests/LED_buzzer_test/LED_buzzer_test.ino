int B_LED = 2;
int G_LED = 1;
int R_LED = 4;
int buzzer1 = 22;
int buzzer2 = 22;

void setup() {
  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(B_LED, OUTPUT);

  pinMode(buzzer1, OUTPUT);
  pinMode(buzzer2, OUTPUT);

  Serial.println("Testing Buzzer 1...");
  delay(1000);
  tone(buzzer1, 500);
  delay(1000);
  noTone(buzzer1);

  Serial.println("Testing Buzzer 2...");
  delay(1000);
  tone(buzzer2, 1000);
  delay(1000);
  noTone(buzzer2);

  Serial.println("Testing Buzzers In Sync...");
  delay(1000);
  tone(buzzer1, 500);
  delay(10);
  tone(buzzer2, 1000);
  delay(1000);
  noTone(buzzer1);
  noTone(buzzer2);

  delay(1000);
  Serial.println("Testing LED...");
}

void loop() {
  digitalWrite(R_LED, LOW);
  digitalWrite(G_LED, LOW);
  digitalWrite(B_LED, HIGH); 
  delay(1000);

  digitalWrite(R_LED, LOW);
  digitalWrite(G_LED, HIGH);
  digitalWrite(B_LED, LOW);
  delay(1000);

  digitalWrite(R_LED, HIGH);
  digitalWrite(G_LED, LOW);
  digitalWrite(B_LED, LOW);
  delay(1000);

}
