//by D
//17.03.2020
//-----------пины-------------
#define tpin 7
#define prpin A0  //пин фоторезистооа
#define eCLK 2
#define eDT  3
#define SW   4  // кнопка энкодера
#define dCLK 5
#define dDIO 6
#define trans 8
//---константы---------
#define fadr 10 //первый адрес
//полезных данных в еепром
#define gam 0.67
#define r1 200000 //Сопротивление в темноте
#define max_mod 1
#define per_write 300000 //300k ms
#define per_update 200 //5гц
#include "GyverEncoder.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include "GyverTM1637.h"

/*
class replay
{
private:
unsigned long lt;
public:
void (*func)();
void opr(int per){
if(millis()-lt>per){
func();
lt=millis();
}
}

};
*/

LiquidCrystal_I2C lcd(0x3f,16,2); 
 // Устанавливаем дисплей
GyverTM1637 disp(dCLK, dDIO);
Encoder enc1(eCLK,eDT,SW,1); 
      // пин CLK, DT, SW,тип

int16_t up,dat;
unsigned long nextTime=0,lclik=0,uptime=0;
volatile byte son=0,mod=0;

void setup()
{ 
  pinMode(trans,OUTPUT);
  digitalWrite(trans,1);
  ini();
  Serial.begin(9600);
  Serial.print("hello");
  attachInterrupt(0, isr, CHANGE); // прерывание на 2 пине! CLK у энка
  uptime=millis();
}
void loop()
{
if(!son && (uptime+500)<millis())
{
pinMode(trans,OUTPUT);
digitalWrite(trans,1);
ini()

}
if(lclik<=millis())
{
son=1;
digitalWrite(trans,0);
pinMode(trans,INPUT);
}

if(nextTime<=millis())
 {
 dat = analogRead(A0);
 EEPROM.get(2,up);
 if(up<1024)
  {
  EEPROM.put(up,dat);
  dat = up+2;
  EEPROM.put(2,dat);
  nextTime=millis()+per_write;
  }
 
 if(!son) {
 disp.clear();
 switch (mod)
 case 0:
 disp.displayInt(Lux(analogRead(A0)));
 case 1:
 disp.displayInt(((dat-(2+fadr))/2);
 }
 
if(son)
{
 //delay что-ли?

}else{

if(Serial.available()>0)
	{
	lcd.setCursor(1, 0);
	//столбец(0-15), строка 0-1
	Serial.setTimeout(20);
  String e = Serial.readStringUntil(47);
  if( e.compareTo( String("read")))
  {
  Serial.flush();	
  Serial.print("send");
  delay(1);
  EEPROM.get(2,up);l
  for(int16_t i=fadr;i<=up;)
   {
   Serial.println(EEPROM.read(i),DEC);
   i=i+2;
   }
   Serial.println(up,DEC);
  }else if( e.compareTo( String("zero")))
  {
   dat=fadr;
   EEPROM.get(2,dat);
  }
 
	}//данные пришли
	if(uptime<=millis())
	 {
	  //обновление экрана
	  uptime=millis()+per_update;
  	}
 }//не сон 
}
void ini()
{
 lcd.init();                  
  lcd.backlight();
  // Включаем подсветку дисплея
  lcd.print("by D ");
  lcd.setCursor(8, 1);
  lcd.print("LCD 1602");
  disp.clear();//вкл второго
  disp.brightness(7); 
  disp.displayInt(8008);
}

int lux(int d)
{
float R=(float)((d*10000)/(1024-d));
 int Luxi = int(round(pow(10,(lg(r1/R)/gam))));
 return Luxi;
}

void isr() 
 {
  enc1.tick(); // отработка в прерывании
  if(enc1.isHolded() && son)
   {
    son=0;
   }
  if( enc1.isRight())
   {
   if(mod<=max_mod)
    {
    mod++;
    }
   }else if( enc1.isLeft() )
   {
   if(mod>=0)
    {
    mod--;
    }
   }
  lclik=millis()+per_write;
 }
