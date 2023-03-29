int motor=2;// déclaration du moteur sur le pin digital 2
int sens1 = A0; // photoresistor 1 pin analog 1 mis en série avec résistance de 10kohm
int sens2 = A1; //photoresistor 2 pin analog 2 mis en série avec résistance de 10kohm
int tolerance = 20;// constante à définir pour ne pas déclencher le moteur sous ce seuil
int val1, val2, diff;// définition de variables (photorésistance 1, photorésistance 2, différence entre les deux. 

void setup() 
{  
  pinMode(sens1, INPUT);//la photorésistance1 envoie un signal analogique compris entre 0 et 1023
  pinMode(sens2, INPUT);//la photorésistance2 envoie un signal analogique compris entre 0 et 1023
  pinMode(motor, OUTPUT);//Signal envoyé au moteur
  delay(2000); // a 2 secondes de délai 
  Serial.begin(9600); // vitesse de communication
}  

 
void loop() 
{ 
  val1 = analogRead(sens1); // lire la valeur analogique en sortie de la photorésistance 1
  val2 = analogRead(sens2); // lire la valeur analogique en sortie de la photorésistance 2
  diff = val2-val1;// on calcule la différence
  
  Serial.println(diff);// on écrit la différence dans le moniteur série
  
  if(diff<-tolerance){           //Si la différence est en dessous du seuil
    analogWrite(motor,255);      //alors faire tourner le moteur très vite
  } else if (diff>tolerance) {   //Si la différence est au dessus du seuil
    analogWrite(motor,150);      //alors faire tourner le moteur très lentement
  } else {                       //Sinon entre -tolerance et +tolerance
    analogWrite(motor,0);        //On éteind le moteur
  }

  delay(100);                    //On met un délai pour laisser le temps aux commandes

}
