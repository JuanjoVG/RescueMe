//RF
#include <VirtualWire.h>

//LCD
#include <LiquidCrystal.h>
#define BACKLIGHT_PIN 10
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Keypad
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

#define INTERRUPTOR_PIN 15

//Modes
#define TX_MODE   0
#define RX_MODE   1
#define ALERT_MODE 2
int mode = RX_MODE;

//Device info
int arduino_id = 1234;

//Buffers
uint8_t buf2[VW_MAX_MESSAGE_LEN];
uint8_t buf2len = VW_MAX_MESSAGE_LEN;

//vars
#define ACK_NOT_RECEIVED 0
#define ACK_RECEIVED 1
int ACK = 0;
int ALERT = 0;
int SEND_ALERT = 1;
String gpsPosition_lat = "41.389224";   //segundo dato para enviar
String gpsPosition_long = "2.113059";   //segundo dato para enviar

String peer_gpsPosition_lat = "0";   //segundo dato para enviar
String peer_gpsPosition_long = "0";   //segundo dato para enviar

//BEEP
#define BEEP_PIN 14
#define BEEP_FREQ 2000

//BT
char data = 0;

/*********** Functions ************/
int read_LCD_buttons() { 
  adc_key_in = analogRead(0);      // Leemos A0
  // Mis botones dan:  0, 145, 329,507,743
  // Y ahora los comparamos con un margen comodo
  if (adc_key_in > 900) return btnNONE;     // Ningun boton pulsado 
  if (adc_key_in < 50)   return btnRIGHT; 
  if (adc_key_in < 250)  return btnUP;
  if (adc_key_in < 450)  return btnDOWN;
  if (adc_key_in < 650)  return btnLEFT;
  if (adc_key_in < 850)  return btnSELECT; 

  return btnNONE;  // Por si todo falla
}

void sendPosition() {
  //defs
  char buf[VW_MAX_MESSAGE_LEN]; // Cadena para enviar
  String str=""; 

  //body
  //Serial.print("TX - LAT:");
  //Serial.print(gpsPosition_lat);
  //Serial.print(" LON:");
  //Serial.println(gpsPosition_long);
  str="x"+gpsPosition_lat+"y"+gpsPosition_long; //convertimos el entero a String y agramos un inicio de trama
  str.toCharArray(buf,sizeof(buf)); //convertimos el String en un array
  vw_send((uint8_t *)buf, strlen(buf)); //Enviamos el array
  vw_wait_tx(); //Esperamos hasta que el mensaje se envie
}

void listenRF() {
  if (vw_get_message((uint8_t *)buf2,&buf2len)) {
    int i;
    peer_gpsPosition_lat = "";
    peer_gpsPosition_long = "";
    if((char)buf2[0]=='x') { //verificamos el inicio de trama
      int IS_LATITUDE = 1;
      for (i = 1; i < buf2len; i++) {
        if((char)buf2[i]=='y') IS_LATITUDE = 0;
        else if(IS_LATITUDE) peer_gpsPosition_lat.concat((char)buf2[i]);
        else peer_gpsPosition_long.concat((char)buf2[i]);
      }
      ALERT = 0;
      //Serial.print("RX - LAT:");
      //Serial.print(peer_gpsPosition_lat);
      //Serial.print(" LON:");
      //Serial.println(peer_gpsPosition_long);
    }
  }
}

void readCoordinates() {
  String str = "";
  if(Serial.available() > 0) { // Send data only when you receive data:
    str = Serial.readStringUntil('#');//Read the incoming data & store into data      
    Serial.print(str);
    int commaIndex = str.indexOf('/');
    gpsPosition_lat = str.substring(0, commaIndex);
    gpsPosition_long = str.substring(commaIndex + 1);
  }
}

/*********** Setup ************/
void setup() {
    //default mode
    mode = RX_MODE;
    //pinMode(14, INPUT);
    pinMode(INTERRUPTOR_PIN, INPUT);
  
    //LCD Shield
    pinMode(BACKLIGHT_PIN, OUTPUT); //backlight
    lcd.begin(16, 2);              // Inicializar el LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RescueMe      RX");     // print a simple message
    
    //Serial Debug + BT
    Serial.begin(9600);    // Debugging only

    // Se inicializa el RF
    vw_setup(2000); // velocidad: Bits per segundo
    vw_set_tx_pin(2); //Pin 2 como salida para el RF
    vw_set_rx_pin(3); //Pin 3 como entrada del RF
    vw_set_ptt_pin(4); //pushtotalk a otro puerto, para evitar colision de libs
    vw_rx_start();

    //HOME SCREEN
    //lcd.setCursor(0,1);              // Cursor a linea 2, posicion 1
    //lcd.print("ID:");
    //lcd.print(arduino_id);

    //beep
    pinMode(BEEP_PIN, OUTPUT);
}

/*********** Main Loop ************/
void loop() {
  //Backlight encendido
  digitalWrite(BACKLIGHT_PIN, HIGH);

  //Get pushed Key
  //lcd_key = read_LCD_buttons();
  //if( lcd_key != btnNONE) {
  if(digitalRead(INTERRUPTOR_PIN)==HIGH) {
    lcd.setCursor(14,0);
    if(mode==TX_MODE) {mode=RX_MODE; lcd.print("RX");}
    else {mode=TX_MODE; lcd.print("TX");}
  }
    //else if ( lcd_key == btnNONE)
    //  lcd.print("NONE  ");

  //MODE switch
  if (mode==TX_MODE) { //Transmit mode
    //Serial.println("TX_MODE");
    //tone(BEEP_PIN, BEEP_FREQ, 100);
    sendPosition();
    lcd.setCursor(14,0);
    lcd.print("TX");
    lcd.setCursor(10,1);
    lcd.print("DANGER");
    //send alert to BT
    if(SEND_ALERT) {
      Serial.print("-ALERT");
      SEND_ALERT=0;
      readCoordinates();
    }
    lcd.setCursor(0,0);
    lcd.print(gpsPosition_lat);
    lcd.setCursor(0,1);
    lcd.print(gpsPosition_long);
    /*
    startMillis = millis();
    ACK = ACK_NOT_RECEIVED;
    int i = 0;
    int tempMillis=0;
    while (ACK==ACK_NOT_RECEIVED && tempMillis < 3000) {
      i=i+1;
      if (i%100==0) tempMillis = millis()-startMillis;
      listenRF();
    }
    int distance = millis()-startMillis;
    Serial.print("DISTANCE:");
    Serial.println(distance);
    */
    delay(100);
  } 
  else if (mode==RX_MODE) {//Receive mode
    SEND_ALERT=1;
    //Serial.println("RX_MODE");
    //clear info
    
    //peer_gpsPosition_lat="0";
    //peer_gpsPosition_long="0";
    
    listenRF();
    lcd.setCursor(14,0);
    lcd.print("RX");
    if (ALERT < 15) {
      tone(BEEP_PIN, BEEP_FREQ, 100);
      ALERT=ALERT+1;
      lcd.setCursor(10,1);
      lcd.print("ALERT!");
      lcd.setCursor(0,0);
      lcd.print(peer_gpsPosition_lat);
      lcd.setCursor(0,1);
      lcd.print(peer_gpsPosition_long);  
    } else if (ALERT==15) {
      lcd.setCursor(10,1);
      lcd.print("------");
      lcd.setCursor(0,0);
      lcd.print("              ");
      lcd.setCursor(0,1);
      lcd.print("nobody in danger");  
    }
    delay(200);
  } else if (ALERT_MODE) {
    
  }
  
  /*
  str="f"+String(dato2); //convertimos el float a String y agramos un inicio de trama
  str.toCharArray(buf,sizeof(buf)); //convertimos el String en un array
  vw_send((uint8_t *)buf, strlen(buf)); ////Enviamos el array
  vw_wait_tx(); //Esperamos hasta que el mensaje se envie
  HOLAAA
  */
  //lcd.blink();
}
