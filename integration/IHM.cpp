#include "IHM.h"

IHM::IHM(){
  rgb = new ChainableLED(PIN_CLK,DATA_PIN,NUMBER_OF_LEDS);
  oled = new U8G2_SH1107_SEEED_128X128_1_SW_I2C(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
}

IHM :: ~IHM(){}

void IHM :: init_IHM(){
  rgb->init();  //led rgb
  oled -> begin();  //écran oled
  //Serial.begin(9600);
  Serial.begin(9600);
  pinMode(ROTARY_ANGLE_SENSOR, INPUT);  //poto
  pinMode(PUSHBUTTON, INPUT); //bouton poussoir
}


////////////////////// LED ////////////////////////////////


void IHM :: allumer_rouge(){
  rgb->setColorRGB(0,253,0,0);
}

void IHM :: allumer_vert(){
  rgb->setColorRGB(0,0,253,0);
}

void IHM :: allumer_bleu(){
  rgb->setColorRGB(0,0,0,253);
}

void IHM :: allumer_orange(){
  rgb->setColorRGB(0,255,140,0);
}

void IHM :: allumer_rouge_intense(){
  rgb->setColorRGB(0,253,0,0);
  rgb->setColorHSL(0,1,1,1);
}

void IHM :: eteindre(){
  rgb->setColorRGB(0,0,0,0);
}

void IHM :: attendre_2s(){
  delay(2000);
}

void IHM :: attendre_1s(){
  delay(1000);
}

void IHM :: change_color(float temp){   //à tester avec le cpateur
  if(temp >= 20 && temp < 22){
    this -> allumer_bleu();
  }else if(temp >= 22 && temp < 24){
    this -> allumer_orange();
  }else if(temp >= 24){
    this -> allumer_rouge();
  }
}


////////////////////// OLED ////////////////////////////////

void IHM :: afficher(u8g2_uint_t x, u8g2_uint_t y, const char* var){
  oled -> firstPage();
  do {
    oled -> setFont(u8g2_font_ncenB10_tr);
    oled -> drawStr(x,y,var);
  } while (oled -> nextPage());
}

void IHM :: welcome_page(){
  float angle = this -> poto();
  
  oled -> firstPage();
  do {
    oled -> setFont(u8g2_font_ncenB10_tr);
    oled -> drawStr(0,24,"BONJOUR");
    oled -> drawStr(0,50,"SELECT MODE :");
    oled -> drawStr(0,100, "Angle : ");
    oled -> drawStr(55,100,String(angle).c_str());
  } while (oled -> nextPage());
}



void IHM :: afficher_temp(float temp){
  oled -> firstPage();
    do {
      oled -> setFont(u8g2_font_ncenB10_tr);
      oled -> drawStr(50,50,String(temp).c_str());
    } while (oled -> nextPage());
}


void IHM :: afficher_vitesse(){
  float angle = this -> poto();

}

////////////////////// POTO ////////////////////////////////

float IHM :: poto(){
  
  float voltage;
  int sensor_value = analogRead(ROTARY_ANGLE_SENSOR);
  voltage = (float)sensor_value*ADC_REF/1023;
  float degrees = (voltage*FULL_ANGLE)/GROVE_VCC;
  //Serial.println("The angle between the mark and the starting position:");
  //Serial.println(degrees);

  return degrees;
}

float IHM :: get_speed(){
  return this -> poto();
}


////////////////////// POTO + OLED ////////////////////////////////

mode_utilisation IHM :: config_mode(){
   oled -> clearDisplay();  //effacer écran

   float angle = this->poto(); //récupérer valeur potentiomètre

   while(!this->button_state()){
    angle = this -> poto(); //récupérer valeur potentiomètre

    oled -> firstPage();
    do {
      oled -> setFont(u8g2_font_ncenB10_tr);
      oled -> drawStr(10,25,"Automatique");
      oled -> drawStr(10,80,"Manuel");
      if(angle < 150){                //en haut
        oled -> drawHLine(10,26,100); //ligne horizontale (x,y,longueur) qui souligne mode automatique
      }else{                          //en bas
        oled -> drawHLine(10,81,55);  //ligne horizontale (x,y,longueur) qui souligne mode manuel
      }
    } while (oled -> nextPage());

   }
   if (angle < 150){
      return Automatique;
   } else {
    return Manuel;
   }
   oled -> clearDisplay();
}

void IHM :: watch_speed(float vitesse){
  oled -> firstPage();
  do {
    oled -> drawRFrame(0,60,100,10,0); //(x,y,largeur,hauteur,arrondi des angles)
    if( vitesse < 10){
      oled -> drawBox(0,60,10,10);
    }else if(vitesse >= 10 && vitesse < 20){
      oled -> drawBox(0,60,20,10);
    }else if(vitesse >= 20 && vitesse < 30){
      oled -> drawBox(0,60,30,10);
    }else if(vitesse >= 30 && vitesse < 40){
      oled -> drawBox(0,60,40,10);
    }else if(vitesse >= 40 && vitesse < 50){
      oled -> drawBox(0,60,50,10);
    }else if(vitesse >= 50 && vitesse < 60){
      oled -> drawBox(0,60,60,10);
    }else if(vitesse >= 60 && vitesse < 70){
      oled -> drawBox(0,60,70,10);
    }else if(vitesse >= 70 && vitesse < 80){
      oled -> drawBox(0,60,80,10);
    }else if(vitesse >= 80 && vitesse < 90){
      oled -> drawBox(0,60,90,10);
    }else if(vitesse >= 90){
      oled -> drawBox(0,60,100,10);
    }
  }while (oled -> nextPage());
}

////////////////////// BOUTON POUSSOIR ////////////////////////////////
/*
int IHM :: button_state(){
  int state = digitalRead(PUSHBUTTON);    
}*/

////////////////////// BOUTON POUSSOIR ////////////////////////////////

bool IHM :: button_state(){
  bool state = false;
  if (digitalRead(PUSHBUTTON) == HIGH){
    state = true ;
  }
  //delay(1);
  return state;
}

////////////////// LED qui change avec temperature //////////////////

void IHM :: led_change_couleur(float temp){

  float temperature = temp;

  if( temperature > 0 && temperature < 18){
      allumer_bleu();
  }else if(temperature >= 18 && temperature < 25){
      allumer_orange();
  }else if(temperature >= 25 && temperature < 29){
      allumer_rouge();
  }else if(temperature > 29){
      allumer_rouge_intense();    
  }
}
