// Exemplo 1 - Usando potenciometro para fazer leituras analógicas
// Apostila Eletrogate - KIT START

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

#define sensorPin A0  // define entrada analógica A0
#define CHECK_BIT(reg,n) ((reg) & (1<<(n))) // verificar estado do bit n no registrador

int sensorValue = 0; // variável inteiro igual a zero
float voltage; // variável numero fracionario

int leituraAnalogica(){
  // talvez poderia passar o pino como argumento e fazer config de MUX[3:0] adequada
  unsigned short x;

  ADCSRA |= BIT7; // ativa adc, ADEN = 1

  int media = 0;
  for (int i = 0; i < 10; i++){
    ADCSRA |= BIT6; // single conversion, ADSC = 1
    while(CHECK_BIT(ADCSRA, 6)){ // espera até ADSC = 0, q significa q a conversao terminou
      asm("");
    }
    x = ADC;
    media += (int)x; //talvez seja ruim somar dps dividir, mas tenho medo de dividir um 10bit
  }
  media /= 10;
  
  ADCSRA &= ~(BIT7); // desliga adc
  return media;
}

void setup() {
  // configuracao adc
  ADMUX |= BIT6; // REFS[1:0] = 01, high reference como avcc
  // config 10bits default (ADMUX), ADLAR = 0
  // entrada A0 default (ADMUX), MUX[3:0] = 0000

  Serial.begin(9600); // monitor serial - velocidade 9600 Bps
  delay(100); // atraso de 100 milisegundos
}

void loop() {
// sensorValue = analogRead(sensorPin); // leitura da entrada analógica A0
sensorValue = leituraAnalogica();
voltage = sensorValue * (5.0 / 1024); // cálculo da tensão

Serial.print("Tensão do potenciometro: "); // imprime no monitor serial
Serial.print(voltage); // imprime a tensão
Serial.print("  Valor: "); // imprime no monitor serial
Serial.println(sensorValue); // imprime o valor
delay(500); // atraso de 500 milisegundos
}
