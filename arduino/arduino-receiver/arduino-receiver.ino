#include <VirtualWire.h>

//LCD shield
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup()
{
  
    Serial.begin(9600);  // Debugging only
    Serial.println("Receptor: setup");

    //LCD Shield
    lcd.begin(16, 2);              // Inicializar el LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RescueMe      RX");     // print a simple message
    

    // Se inicializa el RF
    vw_setup(1000);  // velocidad: Bits per segundo
    vw_set_rx_pin(3);    //Pin 2 como entrada del RF
    vw_set_ptt_pin(4);
    vw_rx_start();       // Se inicia como receptor
}

void loop()
{
  int backlight_pin = 10;
  pinMode(backlight_pin, OUTPUT);
  digitalWrite(10, HIGH);
  
  //HOME SCREEN
  lcd.setCursor(9,1);              // Cursor a linea 2, posicion 9
  lcd.print(millis()/1000);        // Imprime segundos
  
  lcd.setCursor(0,1);              // Cursor a linea 2, posicion 1
  
  
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  int dato1=0;
  float dato2=0.0;
  //verificamos si hay un dato valido en el RF
  if (vw_get_message((uint8_t *)buf,&buflen)) {
    int i;
    String  DatoCadena="";
    if((char)buf[0]=='i') //verificamos el inicio de trama
    {
        for (i = 1; i < buflen; i++)
        {
      DatoCadena.concat((char)buf[i]);
        }
        dato1=DatoCadena.toInt();
        Serial.print("Dato1 recivido: ");
        Serial.println(dato1);
        lcd.print(dato1);
    }
    else if((char)buf[0]=='f') //verificamos el inicio de trama
    {
        for (i = 1; i < buflen; i++)
        {
      DatoCadena.concat((char)buf[i]);
        }
        dato2=DatoCadena.toFloat();
        Serial.print("Dato2 recivido: ");
        Serial.println(dato2);
        //lcd.print(dato2);
    }
  }
}
