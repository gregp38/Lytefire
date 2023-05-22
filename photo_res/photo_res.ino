int E1=2;// déclaration du moteur sur le pin digital 2
int M1=3;// déclaration sens de rotation du moteur sur le pin digital 3
int sens1 = A6; // photoresistor 1 pin analog 6 mis en série avec résistance de 10kohm
int sens2 = A5; //photoresistor 2 pin analog 5 mis en série avec résistance de 10kohm
int tolerance = 40;// constante à définir pour ne pas déclencher le moteur sous ce seuil
int val1, val2, diff;// définition de variables (photorésistance 1, photorésistance 2, différence entre les deux. 

int delay_ok   = 1000;
int delay_diff = 100;
bool dir       = true;

void setup() 
{  
  pinMode(sens1, INPUT);//la photorésistance1 envoie un signal analogique compris entre 0 et 1023
  pinMode(sens2, INPUT);//la photorésistance2 envoie un signal analogique compris entre 0 et 1023
  pinMode(E1, OUTPUT);//Signal envoyé au moteur
  pinMode(M1, OUTPUT);//Signal envoyé au moteur
  delay(2000); // a 2 secondes de délai 
  Serial.begin(9600); // vitesse de communication
}  

 
void loop() 
{ 
  val1 = analogRead(sens1); // lire la valeur analogique en sortie de la photorésistance 1
  val2 = analogRead(sens2); // lire la valeur analogique en sortie de la photorésistance 2
  diff = val2-val1;// on calcule la différence
  dir  = (diff >= tolerance);
  
  Serial.println(diff);// on écrit la différence dans le moniteur série
  
  if(abs(diff)>tolerance){    //Si la différence est en dessous du seuil
    digitalWrite(M1,dir);     //alors faire tourner le moteur très vite
    analogWrite(E1,255);      //alors faire tourner le moteur très lentement
    delay(delay_diff); 
  } else {                    //Sinon entre -tolerance et +tolerance
    analogWrite(E1,0);        //On éteind le moteur
    delay(delay_ok);  
  }
                  //On met un délai pour laisser le temps aux commandes

}
