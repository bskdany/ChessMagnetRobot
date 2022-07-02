#include <ezButton.h>

// Dichiaro le variabili 
int PinElettroMagnete = 10; //  Pin del magnete. 
int PinMotoreXEnable    = 2;
int PinMotoreXDirection = 3;
int PinMotoreXPuls      = 4;
int PinMotoreYEnable    = 11;
int PinMotoreYDirection = 12;
int PinMotoreYPuls      = 13;
float xSeconds; // quanti secondi servono per far muovere motore x
float ySeconds; // quanti secondi servono per far muovere motore y
int directionY; // direzione motore y
int directionX; // direzione motore x
int Stop1 = 1;
int Stop2 = 1;
float coordX;
float coordY;

//coordinate caselle sapendo che la scacchiera ha 1000 x 1000 coordinate. 
float casellaA1[2] = {62.5,62.5};
float casellaA2[2] = {62.5,187.5};


unsigned long startTime;
unsigned long executionTime;
unsigned long stopTime;

ezButton limitSwitch(7); // bottone asse x
ezButton limitSwitch1(8); // bottone asse y 

void setup() {
  Serial.begin(9600); // ? 
  pinMode(PinMotoreXEnable,    OUTPUT); // motori inizializzati 
  pinMode(PinMotoreXDirection, OUTPUT);
  pinMode(PinMotoreXPuls,      OUTPUT);
  pinMode(PinMotoreYEnable,    OUTPUT);
  pinMode(PinMotoreYDirection, OUTPUT);
  pinMode(PinMotoreYPuls,      OUTPUT);
  pinMode(PinElettroMagnete,   OUTPUT); // inizializzo elettromagnete
  limitSwitch.setDebounceTime(50);
  limitSwitch1.setDebounceTime(50);

  //variabili tempo
  float calibrationTime;
  float singleCoordX;
  float singleCoordY;

// calibrazione: saltata perchè abbiamo trovato il tempo max. 
  
  //calibrationTime = calibrationX();
  //singleCoordX = calibrationTime /1000;
  singleCoordX = 11;
  //Serial.println(singleCoordX);

  //calibrationTime = calibrationY();
  //singleCoordY = calibrationTime /1000;
  //Serial.println(singleCoordY);
  singleCoordY = 11;
  
  coordX = 0;
  coordY = 0;

  float start[2] = {300,150}; // coordinate di partenza
  float destination[2] = {700,600};  // coordinate di arrivo
  delay(2000);
  
  //movement(singleCoordX, singleCoordY, start ,destination);
  path( singleCoordX, singleCoordY,start,destination);
  calibrationTime = calibrationX();
  calibrationTime = calibrationY();
}

//no
void loop() {
}



void path(float singleCoordX, float singleCoordY, float start[], float destination[]){
  float tempCoord[2] = {0,0};
  movement(singleCoordX,singleCoordY,tempCoord,start);// porta il magnete dalla posizione (0,0)alla posizione della prima casella. 
  accendiMagnete();
  delay(500);
  Serial.println(start[0],start[1]);
  float deltaX = destination[0] - start [0]; // prendo il primo valore dell'array con 0. 
  float deltaY = destination[1] - start [1];
  float mezzo = 62.5;
  Serial.println(deltaX);
  
  int testNumber;
  if (deltaX >= 0){ 
    testNumber = 1;
  }
  else{
    testNumber = -1;
  }
  float tempCoord1[2] = {start[0]+(mezzo*testNumber),start[1]}; // coordinate prima uscita dalla casella
  movement(singleCoordX,singleCoordY,start,tempCoord1); // Do come destinazione start + mezzo in x, vado da
  Serial.println(tempCoord1[0],tempCoord1[1]);
  delay(500);
  
  float tempCoord2[2] = {tempCoord1[0],tempCoord1[1]+(deltaY-mezzo)};
  movement(singleCoordX,singleCoordY,tempCoord1,tempCoord2);
  Serial.println(tempCoord2[0],tempCoord2[1]);
  delay(500);
  
  float tempCoord3[2] = {tempCoord2[0]+deltaX-(mezzo*testNumber),tempCoord2[1]};
  movement(singleCoordX,singleCoordY,tempCoord2,tempCoord3);
  Serial.println(tempCoord3[0],tempCoord3[1]);
  delay(500);
  
  float tempCoord4[2] = {tempCoord3[0],tempCoord3[1]+mezzo};
  movement(singleCoordX,singleCoordY,tempCoord3,tempCoord4);
  Serial.println(tempCoord4[0],tempCoord4[1]);
  delay(500);
  spegniMagnete();
}


void movement(int singleCoordX, int singleCoordY, float start[] ,float destination[]){ 
  float xMovement; // Do tempo singola coordinata, array partenza (x,y), arrey arrivo (x,y)
  float yMovement;
  float xTime;
  float yTime;
  int xDirection;  
  int yDirection;
  xMovement = destination[0] - start[0]; // coordinata iniz. (0,100) coordinata fin.(500,400)  
  yMovement = destination[1] - start[1];// tutto il blocco ne ha 1000 per convenzione
  

  xTime = xMovement * singleCoordX; // il tempo da muovere è il deltax per il tempo di una singola coordinata. 
  yTime = yMovement * singleCoordY;

  // cerco la direzione x
  if(destination[0] > start[0] ){ // allora: se il xfin-xiniz > 0 --> allora muovo in positivo, al contratio se é negativo 
    xDirection = 1;
    
  }
  // se invece la direzione è negativa, inverti il senso del tempo 
  else{
    xDirection = 0;
    xTime = - xTime;
  }


  // cerco la direzione y
  if(destination[1] > start[1] ){ // allora: se il xfin-xiniz > 0 --> allora muovo in positivo, al contratio se é negativo 
    yDirection = 1;
    
  }
  // se invece la direzione è negativa, inverti il senso del tempo 
  else{
    yDirection = 0;
    yTime = - yTime;
  }
 

  //movimento dei motori
  motoreY(yTime,yDirection);
  motoreX(xTime,xDirection);
  
}


int calibrationX() {

 delay(1000);
  
  limitSwitch.loop(); // MUST call the loop() function first

  int state = limitSwitch.getState();
  

  // Abilita Driver
  digitalWrite(PinMotoreXEnable, LOW);
  digitalWrite(PinMotoreXDirection, HIGH);
  startTime = millis();
  while (true)
  {
    digitalWrite(PinMotoreXPuls, HIGH);
    delayMicroseconds(500);
    digitalWrite(PinMotoreXPuls, LOW);
    delayMicroseconds(500);
    
    if (limitSwitch.getStateRaw() == LOW) {
      
      stopTime = millis();
      executionTime = stopTime - startTime;
     
      digitalWrite(PinMotoreXEnable, HIGH);
      digitalWrite(PinMotoreYEnable, HIGH);
      return executionTime;
      break;
    }
  }

}
int calibrationY() {
 delay(1000);
  
  limitSwitch1.loop(); // MUST call the loop() function first

  int state = limitSwitch1.getState();
  

  // Abilita Driver
  digitalWrite(PinMotoreYEnable, LOW);
  digitalWrite(PinMotoreYDirection, HIGH);
  startTime = millis();
  while (true)
  {
    digitalWrite(PinMotoreYPuls, HIGH);
    delayMicroseconds(500);
    digitalWrite(PinMotoreYPuls, LOW);
    delayMicroseconds(500);
    
    if (limitSwitch1.getStateRaw() == LOW) {
      
      stopTime = millis();
      executionTime = stopTime - startTime;
     
      digitalWrite(PinMotoreXEnable, HIGH);
      digitalWrite(PinMotoreYEnable, HIGH);
      return executionTime;
      break;
    }


  }

}

void motoreX(int xSeconds, int directionX) {
  
  digitalWrite(PinMotoreXEnable, LOW);
  digitalWrite(PinMotoreXDirection, HIGH);
  if (directionX == 0) {
    digitalWrite(PinMotoreXDirection, HIGH);
  }
  else {
    digitalWrite(PinMotoreXDirection, LOW);
  }
  float Index2;
  Index2 = 0; // numero che parte da 0 e viene incrementato di +1 finchè non raggiunge x secondi --> si stoppa. 
  while (Index2 < xSeconds)    // il motore va finché il numero è minore di index 
  {
    digitalWrite(PinMotoreXPuls, HIGH);
    delayMicroseconds(500);
    digitalWrite(PinMotoreXPuls, LOW);
    delayMicroseconds(500);
    Index2++; // index aumenta di +1. 
  }

  // Spegni Driver
  digitalWrite(PinMotoreXEnable, HIGH);
  Index2 = 0;
}

int motoreY(float ySeconds, float directionY) {
  digitalWrite(PinMotoreYEnable, LOW);
  if (directionY == 0) {
    digitalWrite(PinMotoreYDirection, HIGH);
  }
  else {
    digitalWrite(PinMotoreYDirection, LOW);
  }
  float Index;
  Index = 0;
  while (Index < ySeconds)
  {
    digitalWrite(PinMotoreYPuls, HIGH);
    delayMicroseconds(500);
    digitalWrite(PinMotoreYPuls, LOW);
    delayMicroseconds(500);
    Index++;
  }

  // Spegni Driver
  digitalWrite(PinMotoreYEnable, HIGH);
  Index = 0;
}
 
 // Funzione per l'elettromagnete.
 void accendiMagnete(){
digitalWrite(PinElettroMagnete, HIGH);
 }

void spegniMagnete(){
  digitalWrite(PinElettroMagnete, LOW);
}