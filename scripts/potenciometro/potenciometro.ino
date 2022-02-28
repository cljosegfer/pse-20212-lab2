// Exemplo 1 - Usando potenciometro para fazer leituras analógicas
// Apostila Eletrogate - KIT START

#define sensorPin A0  // define entrada analógica A0
#define CHECK_BIT(reg,n) ((reg) & (1<<(n))) // verificar estado do bit n no registrador

int sensorValue = 0; // variável inteiro igual a zero
float voltage; // variável numero fracionario

int leituraAnalogica(){
  // talvez poderia passar o pino como argumento e fazer config de MUX[3:0] adequada
  unsigned short x;

  ADCSRA |= 0x80; // ativa adc, ADEN = 1

  ADCSRA |= 0x40; // single conversion, ADSC = 1
  while(CHECK_BIT(ADCSRA, 6)){ // espera até ADSC = 0, q significa q a conversao terminou
    asm("");
  }
  x = ADC;

  // ADCSRA &= ~(0x80); // desliga adc?

  return (int)x;
}

void setup() {
  // configuracao adc
  ADMUX |= 0x40; // REFS[1:0] = 01, high reference como avcc
  // ADMUX |= 0x20; // ADLAR = 1, configuracao 8bits
  // config 10bits default (ADMUX), ADLAR = 0
  // entrada A0 default (ADMUX), MUX[3:0] = 0000
  // prescaler 2 default (ADCSRA), ADPS[2:0] = 000

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
