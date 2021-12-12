// C++ code
//Importando libreria
#include<LiquidCrystal.h> //añadiendo libreria
//Definiendo las constantes para filas y columnas
#define COLS 16 //Columnas del LCD
#define ROWS 2 //Filas del LCD
//Ya esta incluida la libreria, hay que definir los pines a usar
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //(RW, EN, d4, d5, d6, d7)
void setup() 
{
  Serial.begin(9600); //Iniciar la comunicacion serie
  lcd.begin(COLS, ROWS); //Se especifican las dimensiones de la LCD
}

void loop()
{
  //set the cursos to column 0, line 1
  lcd.clear(); //limpiamos la pantalla
  lcd.setCursor(0, 0); //situamos el cursos (C, F)
  lcd.print("Hola Mundo!!!!!!"); //Escribimos en la pantalla
  lcd.setCursor(0, 1); //situamos el cursos (C, F)
  lcd.print("Probando el LCD."); //Escribimos en pantalla
  delay(2000); //tiempo de espera
  lcd.clear(); //limpiamos la pantalla
  lcd.setCursor(0, 0); //situamos el cursor (C, F)
  lcd.print("FIN!!!!!!"); //escribimos en pantalla
  delay(2000); //tiempo de espera
}
