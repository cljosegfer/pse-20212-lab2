#include <LiquidCrystal_I2C.h>      // usando a biblioteca LiquidCrystal

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

//Constantes do NTC
#define T0 298.15 // define constante igual a 298.15 Kelvin
#define R_div 10000 // Resistor do divisor de tensao
#define R0 10000 // Valor da resistencia inicial do NTC
#define T1 273.15 // [K] in datasheet 0º C
#define T2 373.15 // [K] in datasheet 100° C
#define RT1 35563 // [ohms] resistencia in T1
#define RT2 549 // [ohms] resistencia in T2

// Definições do conversor AD
#define sensorPin A0  // define entrada analógica A0
#define CHECK_BIT(reg,n) ((reg) & (1<<(n))) // verificar estado do bit n no registrador

// Confirgurações do LCD
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7; // definicao dos pinos do Display
LiquidCrystal_I2C lcd(0x27, 16, 2);                // configurando os pinos

// Variaveis globais
float sensorValue = 0; 
float tensao; 
float temperatura; 
float beta = 0.0; // parametros iniciais [K]
float R_inferior = 0.0; // parametros iniciais [ohm]
float R_NTC = 0.0; // Rout in A0

float leituraAnalogica(){
  // talvez poderia passar o pino como argumento e fazer config de MUX[3:0] adequada
  unsigned short x;

  ADCSRA |= BIT7; // ativa adc, ADEN = 1

  float media = 0;
  for (int i = 0; i < 10; i++){
    ADCSRA |= BIT6; // single conversion, ADSC = 1
    while(CHECK_BIT(ADCSRA, 6)){ // espera até ADSC = 0, q significa q a conversao terminou
      asm("");
    }
    x = ADC;
    media += (float)x;
  }
  media /= 10.0;
  ADCSRA &= ~(BIT7); // desliga adc
  return media;
}

void mostrar_temp(){
  R_NTC = (R_div * tensao / (5 - tensao)); // calculo de Rout
  temperatura = (beta / log(R_NTC / R_inferior)) - 273.15; // calculo da temp. em Celsius

  lcd.setCursor(0, 0);            // selecionando coluna 0 e linha 0
  lcd.print("Temperatura:");      // print da mensagem
  lcd.setCursor(2, 1);            // selecionando coluna 2 e linha 1
  lcd.print(temperatura));
}

void setup() {
  // configuracao adc
  ADMUX |= BIT6; // REFS[1:0] = 01, high reference como avcc
  // config 10bits default (ADMUX), ADLAR = 0
  // entrada A0 default (ADMUX), MUX[3:0] = 0000

  beta = (log(RT1 / RT2)) / ((1 / T1) - (1 / T2)); // calculo de beta
  R_inferior = R0 * exp(-beta / T0); // calculo de Rinf

  lcd.init();                      // inicializa LCD
  lcd.backlight();                 // ativa led de backlight
  
}

void loop() {
  sensorValue = leituraAnalogica();
  tensao = sensorValue * (5.0 / 1024); // cálculo da tensão
  mostrar_temp();
}
