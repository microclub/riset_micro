#include <mega16.h>
#include <stdlib.h>
#include <lcd.h>
#include <delay.h>

int potensio1, potensio2, potensio3;
char temp[6];

// Alphanumeric LCD Module functions
#asm
.equ __lcd_port=0x15 ;PORTC
#endasm

#define ADC_VREF_TYPE 0x00
// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=0x40;
// Wait for the AD conversion to complete
while ((ADCSRA & 0x10)==0);
ADCSRA|=0x10;
return ADCW;
}

void main(void)
{
PORTD=0xff;
DDRD=0x07; //Bit PD2-0 sebagai output untuk menghidupkan LED

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

// ADC initialization
// ADC Clock frequency: 750.000 kHz
// ADC Voltage Reference: AREF pin
// ADC Auto Trigger Source: None
ADMUX=ADC_VREF_TYPE & 0xff;
ADCSRA=0x84;

// LCD module initialization
lcd_init(16);

while (1)
{
potensio1=read_adc(0);//baca ADC channel 0 (ADC0 atau PA0)
potensio2=read_adc(1);//baca ADC channel 1 (ADC1 atau PA1)
potensio3=read_adc(2);//baca ADC channel 2 (ADC2 atau PA2)

if (potensio1>potensio2 && potensio2>potensio3)
{PORTD=0b11111110;}//jika paling besar nilai potensio 1, nyalakan led1
if (potensio2>potensio1 && potensio2>potensio3)
{PORTD=0b11111101;}//jika paling besar nilai potensio 2, nyalakan led2
if (potensio1<potensio2 && potensio2<potensio3)
{PORTD=0b11111011;}//jika paling besar nilai potensio 3, nyalakan led3

lcd_clear( );
lcd_gotoxy(0,0);
lcd_putsf("Micro Club");
itoa(potensio1,temp);//menampilkan nilai ADC potensio1
lcd_gotoxy(0,1) ;
lcd_puts(temp);
itoa(potensio2,temp);//menampilkan nilai ADC potensio2
lcd_gotoxy(5,1) ;
lcd_puts(temp);
itoa(potensio3,temp);//menampilkan nilai ADC potensio3
lcd_gotoxy(10,1) ;
lcd_puts(temp);
delay_ms(300);
};
} 
