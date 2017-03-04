#include <VirtualWire.h>

//LCD shield
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup()
{
    //LCD Shield
    lcd.begin(16, 2);              // Inicializar el LCD
    lcd.setCursor(0,0);
    lcd.print("RescueMe RX");     // print a simple message
    Serial.begin(9600);    // Debugging only
    Serial.println("setup");

    // Se inicializa el RF
    vw_setup(1000); // velocidad: Bits per segundo
    vw_set_tx_pin(2); //Pin 2 como salida para el RF en realidad es el 3 XD
}

void loop()
{
  //HOME SCREEN
  lcd.setCursor(9,1);              // Cursor a linea 2, posicion 9
  lcd.setCursor(13, 0);            // Cursor a linea 1, posicion 13
  lcd.setCursor(0,1);              // Cursor a linea 2, posicion 1
  
    char buf[VW_MAX_MESSAGE_LEN]; // Cadena para enviar
    int dato1=millis()/1000; //variable con el tiempo en segundos
    float dato2=3.14;   //segundo dato para enviar
    String str="";  

    lcd.print(dato1);
    
    str="i"+String(dato1); //convertimos el entero a String y agramos un inicio de trama
    str.toCharArray(buf,sizeof(buf)); //convertimos el String en un array
    vw_send((uint8_t *)buf, strlen(buf)); //Enviamos el array
    vw_wait_tx(); //Esperamos hasta que el mensaje se envie
    
    str="f"+String(dato2); //convertimos el float a String y agramos un inicio de trama
    str.toCharArray(buf,sizeof(buf)); //convertimos el String en un array
    vw_send((uint8_t *)buf, strlen(buf)); ////Enviamos el array
    vw_wait_tx(); //Esperamos hasta que el mensaje se envie
    
    delay(200);
}
