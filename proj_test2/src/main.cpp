#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <uart.h> 
#include "timer.h"

// Choose two Arduino pins to use for software serial
int RXPin = 9;
int TXPin = 10;

//Default baud of NEO-6M is 9600
int GPSBaud = 9600;

// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin);

void setup()
{
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);

  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
}

/*void loop()
{
  // Displays information when new sentence is available.
  while (gpsSerial.available() > 0)
    Serial.write(gpsSerial.read());
}*/

int main(void)
{
    uart_init(UART_BAUD_SELECT(9600, F_CPU));

    TIM1_OVF_1SEC;
    TIM1_OVF_ENABLE;

    sei();

    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

ISR(TIMER1_OVF_vect)
{
    char string[20];
    static uint8_t no_of_values = 0;

    no_of_values++;

    while (gpsSerial.available() > 0)
    {
        itoa(gpsSerial.read(), string, 10);
        uart_puts("\r\n");
        uart_puts("DEGS: ");
        uart_puts (string);
        uart_puts("\r\n");  

        no_of_values = 0;
    }
}