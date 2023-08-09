int RPWM_Output = 8; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output = 6; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)

int sens1_A0 = A0; 
int sens1_A1 = A1; 
int sens2_A0 = A2; 
int sens2_A1 = A3; 


int tolerance = 40;// constante à définir pour ne pas déclencher le moteur sous ce seuil
int val1, val2, diff, mean;// définition de variables (photorésistance 1, photorésistance 2, différence entre les deux. 

int motor_speed = 60;
int delay_ok    = 3*60*1000;
int delay_diff  = 500;
bool dir        = true;

#define MOTOR_ENABLE 1


void setup() 
{  
  pinMode(RPWM_Output, OUTPUT);
  pinMode(LPWM_Output, OUTPUT);
  
  pinMode(sens1_A0, INPUT);
  pinMode(sens1_A1, INPUT);
  pinMode(sens2_A0, INPUT);
  pinMode(sens2_A1, INPUT);
  
  delay(2000); // a 2 secondes de délai 
  Serial.begin(9600); // vitesse de communication
  analogWrite(LPWM_Output, 0);
  analogWrite(RPWM_Output, 0);
}  

 
void loop() 
{ 
  val1 = analogRead(sens1_A0); // lire la valeur analogique en sortie de la photorésistance 1
  val2 = analogRead(sens2_A0); // lire la valeur analogique en sortie de la photorésistance 2
  Serial.print("(sens1_a0, sens2_a0)  = ");
  Serial.print(val1);// on écrit la différence dans le moniteur séries
  Serial.print(" , ");
  Serial.println(val2);// on écrit la différence dans le moniteur séries
  mean = (val1+val2)/2;
  
  if (mean > 700 or mean < 300){
    val1 = analogRead(sens1_A1); // lire la valeur analogique en sortie de la photorésistance 1
    val2 = analogRead(sens2_A1); // lire la valeur analogique en sortie de la photorésistance 2
      
    Serial.println("switch to A1:");
    Serial.print("(sens1_a1, sens2_a1)  = ");
    Serial.print(val1);// on écrit la différence dans le moniteur séries
    Serial.print(" , ");
    Serial.println(val2);// on écrit la différence dans le moniteur séries
  }
  

  diff = val2-val1;// on calcule la différence
  dir  = (diff >= tolerance);

  Serial.print("diff = ");
  Serial.println(diff);// on écrit la différence dans le moniteur série
  
  if(abs(diff)>tolerance){    //Si la différence est en dessous du seuil
    if (diff > 0) {
      
      Serial.print("=> Left");
#if MOTOR_ENABLE
      analogWrite(RPWM_Output, 0);
      analogWrite(LPWM_Output, motor_speed);
#endif
    }else{
       Serial.print("=> Right");
#if MOTOR_ENABLE
      analogWrite(LPWM_Output, 0);
      analogWrite(RPWM_Output, motor_speed);
#endif
    }
    delay(delay_diff); 
  } else {                    //Sinon entre -tolerance et +tolerance
    Serial.print("=> OOOOK");
#if MOTOR_ENABLE
    analogWrite(RPWM_Output, 0);
    analogWrite(LPWM_Output, 0);
#endif
    delay(delay_ok);  
  }
                  //On met un délai pour laisser le temps aux commandes

}
