// Définition des pins digitales de sortie de contrôle PWM des moteurs
int azymut_RPWM_Output = 8; // Arduino PWM output pin 8; connect to IBT-2 pin 1 (RPWM)
int azymut_LPWM_Output = 6; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)*

int elev_RPWM_Output = 9; // Arduino PWM output pin 8; connect to IBT-2 pin 1 (RPWM)
int elev_LPWM_Output = 7; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)

// Définition des pins digitales des entrées des capteurs pour le contrôle automatique 
int azymut_sens0_A0 = A0; 
int azymut_sens0_A1 = A1; 
int azymut_sens1_A0 = A2; 
int azymut_sens1_A1 = A3; 

int elev_sens0_A0 = A4; 
int elev_sens0_A1 = A5; 
int elev_sens1_A0 = A6; 
int elev_sens1_A1 = A7; 

// Définition des pins digitales des entrées des capteurs pour le contrôle manuel
int TOR_high  = 15; 
int TOR_low   = 14;
int TOR_mode  = 13; 
int TOR_left  = 12; 
int TOR_right = 11; 
 

// Définition de variables servant au mode automatique
int val1, val2, diff, mean, tolerance;		// permet de choisir entre les deux ponts diviseurs du shadowbend 
int mean_low 	  = 300;
int mean_high 	= 700;

int motor_speed 	  = 100;		// vitesse moteur (valeur 8-bits: 0 à 255)
long int delay_ok   = 5000; 	// delai de rafraichissement du programme lorsque le shadowbend ne détecte pas d'ombre (lent  : 5s in ms)
int delay_diff  	  = 200;    // delai de rafraichissement du programme lorsque le shadowbend détecte une ombre      (rapide: 0.2s in ms)
bool dir          	= true;

#define MOTOR_ENABLE 1
#define PRINT_ENABLE 1  
#define MODE_AUTO 0

void setup() 
{  
  // Initialisation des entrées/sorties
  pinMode(azymut_RPWM_Output, OUTPUT);
  pinMode(azymut_LPWM_Output, OUTPUT);
  pinMode(elev_RPWM_Output, OUTPUT);
  pinMode(elev_LPWM_Output, OUTPUT);
  
  pinMode(azymut_sens0_A0, INPUT);
  pinMode(azymut_sens0_A1, INPUT);
  pinMode(azymut_sens1_A0, INPUT);
  pinMode(azymut_sens1_A1, INPUT);
  
  pinMode(elev_sens0_A0, INPUT);
  pinMode(elev_sens0_A1, INPUT);
  pinMode(elev_sens1_A0, INPUT);
  pinMode(elev_sens1_A1, INPUT);
  
  pinMode(TOR_mode , INPUT_PULLUP);
  pinMode(TOR_left , INPUT_PULLUP);
  pinMode(TOR_right, INPUT_PULLUP);
  pinMode(TOR_high , INPUT_PULLUP);
  pinMode(TOR_low  , INPUT_PULLUP);
  
  analogWrite(azymut_LPWM_Output, 0);
  analogWrite(azymut_RPWM_Output, 0);
  
  // Si besoin, initialisation du lien série arduino
  #if PRINT_ENABLE
    Serial.begin(9600);                 
  #endif

  delay(2000);                       
}  

 
void loop() 
{ 
  if (digitalRead(TOR_mode) == MODE_AUTO) {
  
	// Acquisition de la tension aux bornes du premier pont diviseur
  val1 = analogRead(azymut_sens0_A0); 
  val2 = analogRead(azymut_sens1_A0);
	  
  mean = (val1+val2)/2;

	// Transmission au moniteur série des valeurs obtenues
  #if PRINT_ENABLE  
	  Serial.println("MODE AUTO:");
    Serial.println("asservissement azymut : ");
	  Serial.print("azymut_sens0_A0 = ");
    Serial.println(val1);
	  Serial.print("azymut_sens1_A0 = ");
    Serial.println(val2);		  
	  Serial.print("mean = ");
    Serial.println(mean);
    Serial.println("");
	#endif 
  
	// Si une saturation est détectée alors on utilise le deuxième pont diviseur
	if (mean > mean_high or mean < mean_low){
		
	  // Acquisition de la tension aux bornes du deuxième pont diviseur
	  val1 = analogRead(azymut_sens0_A1);
	  val2 = analogRead(azymut_sens1_A1);
	  
	  mean = (val1+val2)/2;
	  
	  // Transmission au moniteur série des valeurs obtenues
    #if PRINT_ENABLE 	  
	    Serial.println("Saturation détectée - acquisition du deuxième pont diviseur:");
      Serial.print("azymut_sens0_A0 = ");
      Serial.println(val1);
	    Serial.print("azymut_sens1_A0 = ");
      Serial.println(val2);		  
	    Serial.print("mean = ");
      Serial.println(mean);
      Serial.println("");
	  #endif
	}
  
  // Pour éviter que le système oscille (alterne entre phase de movement et stable),
  // on défini une tolérance dynamique qui correspond à 20% d'écart à la moyenne entre les 2 tensions des 2 photorésistances
  tolerance = mean*0.2;  
  diff      = val2-val1;
  dir       = (diff >= tolerance);
  
  #if PRINT_ENABLE 	    
    Serial.print("diff = ");
    Serial.println(diff);
  #endif 
  
  //Si on détecte une ombre
  if(abs(diff)>tolerance){    
    #if PRINT_ENABLE 
      Serial.print("Ombre détectée sur l'azumut, le système va bouger vers la :");  
      if (diff > 0) {
        Serial.println("   GAUCHE");  
      }else{
        Serial.println("   DROITE");  
      }
    #endif 
    
    // alors on contrôle du moteur azymut par PWM
    #if MOTOR_ENABLE
      motor_control (true, dir, motor_speed);
    #endif
    

    
    // on attend un délai court de rafraichissement du programme,
    // pour être réactif pour stopper les moteurs lorsque il n'y a plus d'ombre
    delay(delay_diff); 
    
  // Sinon on ne détecte pas d'ombre
  } else {
    #if PRINT_ENABLE     
      Serial.println("Aucune ombre détectée en azymut !! ");
    #endif
    
    // On stoppe le moteur azumut
    #if MOTOR_ENABLE
      motor_control (true, true, 0);
    #endif
    
    // puis on stabilise l'elevation
    ////////////////////////////////////////////////////////////
    //           TODO: asservissement elevation               //
    ////////////////////////////////////////////////////////////
    // Acquisition de la tension aux bornes du premier pont diviseur
    val1 = analogRead(elev_sens0_A0); 
    val2 = analogRead(elev_sens1_A0);
      
    mean = (val1+val2)/2;

    // Transmission au moniteur série des valeurs obtenues
    #if PRINT_ENABLE  
      Serial.println("asservissement élévation : ");
      Serial.print("elev_sens0_A0 = ");
      Serial.println(val1);
      Serial.print("elev_sens1_A0 = ");
      Serial.println(val2);		  
      Serial.print("mean = ");
      Serial.println(mean);
      Serial.println("");
    #endif 
    
    // Si une saturation est détectée alors on utilise le deuxième pont diviseur
    if (mean > mean_high or mean < mean_low){
      
      // Acquisition de la tension aux bornes du deuxième pont diviseur
      val1 = analogRead(elev_sens0_A1);
      val2 = analogRead(elev_sens1_A1);
      
      mean = (val1+val2)/2;
      
      // Transmission au moniteur série des valeurs obtenues
      #if PRINT_ENABLE 	  
        Serial.println("Saturation détectée - acquisition du deuxième pont diviseur:");
        Serial.print("elev_sens0_A0 = ");
        Serial.println(val1);
        Serial.print("elev_sens1_A0 = ");
        Serial.println(val2);		  
        Serial.print("mean = ");
        Serial.println(mean);
        Serial.println("");
      #endif
    }
    
    // Pour éviter que le système oscille (alterne entre phase de movement et stable),
    // on défini une tolérance dynamique qui correspond à 20% d'écart à la moyenne entre les 2 tensions des 2 photorésistances
    tolerance = mean*0.2;  
    diff      = val2-val1;
    dir       = (diff >= tolerance);
    
    #if PRINT_ENABLE 	    
      Serial.print("diff = ");
      Serial.println(diff);
    #endif 
    
    //Si on détecte une ombre
    if(abs(diff)>tolerance){    
      #if PRINT_ENABLE 
        Serial.print("Ombre détectée sur l'azumut, le système va bouger vers le :");  
        if (diff > 0) {
          Serial.println("   HAUT");  
        }else{
          Serial.println("   BAS");  
        }
      #endif 
      
      // alors on contrôle du moteur elev par PWM
      #if MOTOR_ENABLE
        motor_control (false, dir, motor_speed);
      #endif
     
      // on attend un délai court de rafraichissement du programme,
      // pour être réactif pour stopper les moteurs lorsque il n'y a plus d'ombre
      delay(delay_diff); 
    } else {
    ////////////////////////////////////////////////////////////
    //           TODO: asservissement elevation               //
    ////////////////////////////////////////////////////////////
    
      // on attend un délai lent de rafraichissement du programme,
      // car le système est stabilisé et le soleil se déplace lentemment
      delay(delay_ok);
    }      
  }
 
  } else {
    #if PRINT_ENABLE  
      Serial.println("MODE MANUEL:");
    #endif
    // Controle manuel des moteurs :
    // Seul un bouton à la fois est pris en compte avec la priorité suivante:
    //   1. gauche
    //   2. droite
    //   3. haut
    //   4. bas
     
    // si le bouton gauche est appuyé
    if (not(digitalRead(TOR_left))) {
      motor_control (true, true, motor_speed);
  
    // si le bouton droite est appuyé
    } else if (not(digitalRead(TOR_right))) {
      motor_control (true, false, motor_speed);
      
    // si le bouton haut est appuyé
    } else if (not(digitalRead(TOR_high))) {
      motor_control (false, true, motor_speed);
      
    // si le bouton bas est appuyé
    } else if (not(digitalRead(TOR_low))) {
      motor_control (false, false, motor_speed);
    
    // sinon on ne fait rien
    } else {
      motor_control (true, true, 0);; 
    }
    
  }
  delay(delay_diff); 
}

void azymut_motor_control ( bool is_left, int speed) {
  // Si on va à gauche :
  if (is_left) {
    analogWrite(azymut_RPWM_Output, 0);     // d'abords on arrête la pwm de droite
    analogWrite(azymut_LPWM_Output, speed); // puis on lance la pwm gauche
    
  // Sinon on va à droite :
  } else {
    analogWrite(azymut_LPWM_Output, 0);     // d'abords on arrête la pwm de gauche
    analogWrite(azymut_RPWM_Output, speed); // puis on lance la pwm droite
  }
}

void elevation_motor_control ( bool is_up, int speed) {
  // Si on monte les mirroirs :
  if (is_up) {
    analogWrite(elev_RPWM_Output, 0);     // d'abords on arrête la pwm de du bas
    analogWrite(elev_LPWM_Output, speed); // puis on lance la pwm du haut
    
  // Sinon on descend les mirroirs :
  } else {
    analogWrite(elev_LPWM_Output, 0);     // d'abords on arrête la pwm de du haut
    analogWrite(elev_RPWM_Output, speed); // puis on lance la pwm du haut
  }
}

void motor_control ( bool is_azymut, bool dir, int speed) {
  
  // les moteurs azymut ou élévation sont activés indépendemment,
  // on arrête l'autre si on veut activé l'un
  if (is_azymut) {
    elevation_motor_control(true, 0);
    azymut_motor_control(dir, speed);
  } else {
    azymut_motor_control(true, 0);
    elevation_motor_control(dir, speed);
  }
}