// Inspire de https://wiki.dfrobot.com/Incremental_Photoelectric_Rotary_Encoder_-_400P_R_SKU__SEN0230#target_5
/***************************************************
  Two phase quadrature encoder(Incremental)
* ****************************************************
   To determine motor with encode （CW OR CCW）

  @author Dong
  @version  V1.0
  @date  2016-5-26
  All above must be included in any redistribution
* ****************************************************/

//-----------------Encodeur chassis --------------------------//
// Entrées
#define  A_PHASE 2            // Phase A sur le pin n°2
#define  B_PHASE 3            // Phase B sur le pin n°3
// Sorties
#define  MOTOR 13             // petit moteur test

// Déclaration des variables
int raf = 50;                 // Temps de raffraichissement (ms) du serial
int pos = 0;                  // Position angulaire 1600/tour
int oneturn = 1600;           // Un tour complet vaut valeur 1600 --> ! Dépend de l'encodeur rotatif choisi
float degree = 0 ;            // La valeur sortie en degrés
// Test moteur
  // fourchette d'angle où le moteur ne s'allume pas  
float min_degree = 0 ;        // La valeur sortie en degrés
float max_degree = 120 ;      // La valeur sortie en degrés

// 
void setup() {
  pinMode(A_PHASE, INPUT);    // Déclaration phase A en entrée 3
  pinMode(B_PHASE, INPUT);    // Déclaration phase B en entrée 2
  pinMode(MOTOR, OUTPUT);     // Déclaration phase B en entrée 2
  Serial.begin(9600);         //Serial Port Baudrate: 9600 (vitesse de communication USB UART)
  attachInterrupt(digitalPinToInterrupt( A_PHASE), interrupt_A, CHANGE); //Interrupt trigger mode: CHANGE
  attachInterrupt(digitalPinToInterrupt( B_PHASE), interrupt_B, CHANGE); //Interrupt trigger mode: CHANGE
}

void loop() {
  degree = to_degree(pos);

  if ( (degree > min_degree) && (degree < max_degree) ) {
    digitalWrite(MOTOR, HIGH);
  } else {
    digitalWrite(MOTOR, LOW);
  }
  Serial.println(degree);
  delay(raf);     
}

void interrupt_A()    // Interrupt function
{ char i; 
  char j;
  i = digitalRead( B_PHASE);
  j = digitalRead( A_PHASE);
  if (i ^ j ) {
    pos ++;
  } else {
    pos--; 
  }
}

void interrupt_B()  // Interrupt function
{ char i; 
  char j;
  i = digitalRead( B_PHASE);
  j = digitalRead( A_PHASE);
  if (i ^ j ) {
    pos --;
  } else {
    pos++; 
  }  
}

float to_degree(float my_pos) // Change position en pulse/tour en degres
{ 
  return ((my_pos*360/oneturn) );
}
