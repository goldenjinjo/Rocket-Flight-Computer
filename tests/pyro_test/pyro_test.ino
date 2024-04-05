// Defining Pins
int pyro1 = 2; // Right Terminal
int pyro2 = 3; // Left Terminal
int RED = 8;
int BLU = 14;
int GRN = 15;
int buzzer1 = 6;
int buzzer2 = 7;


void setup() {

  Serial.begin(9600);
  
  pinMode(pyro1, OUTPUT);
  pinMode(pyro2, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BLU, OUTPUT);
  pinMode(GRN, OUTPUT);
  pinMode(buzzer1, OUTPUT);

  
  digitalWrite(pyro1, LOW);
 
  Serial.println("Triggering Pyro 1 in  ~10 seconds");
  
  int x = 20;
  while(x > 1) 
  {
    delay(500);
    x = x - 1;
    digitalWrite(RED, HIGH);
    tone(buzzer1, 300);
    Serial.println("..."+String(x));
    delay(500);
    digitalWrite(RED, LOW);
    noTone(buzzer1);

  }

  delay(1000);

  
  Serial.println("PYRO TRIGGER");
  tone(buzzer1, 1500);
  tone(buzzer2, 1500);
  digitalWrite(BLU, HIGH);
  digitalWrite(pyro1, HIGH);
  delay(500);
  digitalWrite(BLU, LOW);
  delay(10);
  digitalWrite(pyro1, LOW);
  noTone(buzzer1);
  noTone(buzzer2);
  
 

  

}

// the loop routine runs over and over again forever:
void loop() {

  digitalWrite(RED, HIGH);
  delay(200);
  digitalWrite(RED, LOW);
  delay(200);
                 
}
