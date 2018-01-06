#include <Wire.h>        //Biblioteca para manipulação do protocolo I2C
#include <DS3231.h>      //Biblioteca para manipulação do DS3231 https://github.com/jarzebski/Arduino-DS3231
#include <LiquidCrystal_I2C.h>
 
DS3231 rtc;              //Criação do objeto do tipo DS3231
RTCDateTime dataehora;   //Criação do objeto do tipo RTCDateTime
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);

int rele = 3;
int botao = 2;
int manual = 4; 
long tempoManual = 0;
int estaManual = 0; //0 - Não, 1 - Sim
int ledManual = 5;
int bomba = LOW;
 
void setup()
{
  Serial.begin(9600);
  pinMode(rele, OUTPUT);
  pinMode(botao, INPUT);
  pinMode(manual, INPUT);
  pinMode(ledManual, OUTPUT);
  lcd.begin (16,2);
  rtc.begin();

  lcd.clear();
  lcd.setBacklight(LOW);
 
  //rtc.setDateTime(__DATE__, __TIME__); //habilitar somente para ajustar a hora, depois comentar e compilar de novo
}
 
void loop()
{  
  if (estaManual == 0 && digitalRead(manual) == HIGH && bomba == HIGH)
    ModoManual();
    
  if (digitalRead(botao) == HIGH)
    MostraInfo();
    
  dataehora = rtc.getDateTime();
  
  if ((dataehora.minute >= 0 && dataehora.minute <15) ||
      (dataehora.minute >= 30 && dataehora.minute <45) ||
      estaManual == 1)
    bomba = LOW; //Liga o rele
  else
    bomba = HIGH; //Desliga o rele
    
  digitalWrite(rele, bomba);

  if (estaManual == 1)
    digitalWrite(ledManual, HIGH);

  if (estaManual == 1 && abs(TempoAgora() - tempoManual) > 60) {
    estaManual = 0;
    digitalWrite(ledManual, LOW);
  }
}

void MostraInfo() {
  lcd.setBacklight(HIGH);
  lcd.clear();
  PrintHoraTemp();
  PrintBomba();
  delay(10000);
  lcd.clear();
  lcd.setBacklight(LOW);
}

void PrintHoraTemp() {
  lcd.setCursor(0,0);
  if (dataehora.hour < 10)
    lcd.print("0");
    
  lcd.print(dataehora.hour);
  lcd.print(":");

  if (dataehora.minute < 10)
    lcd.print("0");
    
  lcd.print(dataehora.minute);
  lcd.print(" - "); 
  lcd.print(rtc.readTemperature());
}

void PrintBomba() {
  lcd.setCursor(0,1);
  lcd.print("Bomba: ");

  if (bomba == LOW)
    lcd.print("ON ");
  else
    lcd.print("OFF");
}

void ModoManual() {
  estaManual = 1;
  tempoManual = TempoAgora();
}

int TempoAgora() {
  return dataehora.second + dataehora.minute * 60 + dataehora.hour * 3600;
}



