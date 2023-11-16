#ifndef F_CPU
# define F_CPU 16000000  // CPU frequency in Hz required for UART_BAUD_SELECT
#endif


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <uart.h>           // Peter Fleury's UART library
#include <stdlib.h>         // C library. Needed for number conversions
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

volatile uint8_t new_gps_data = 0;

int main(void)
{
    char string[2];
    // Initialize USART to asynchronous, 8N1, 9600
    uart_init(UART_BAUD_SELECT(9600, F_CPU));
    
    // Configure 16-bit Timer/Counter1 to transmit UART data
    // Set prescaler to 262 ms and enable overflow interrupt
    TIM1_OVF_1SEC;
    TIM1_OVF_ENABLE;
    // Enables interrupts by setting the global interrupt mask
    sei();

    // Put strings to ringbuffer for transmitting via UART

    // Infinite loop
    while (1) {
        if (new_gps_data == 1) {
          itoa(dht12.temp_int, string, 10);
          uart_puts(string);
          uart_puts(".");
          itoa(dht12.temp_dec, string, 10);
          uart_puts(string);
          uart_puts("\t\t");

          new_gps_data == 0;
        }
    }

    // Will never reach this
    return 0;
}



ISR(TIMER1_OVF_vect)
{
    static uint8_t n_ovfs = 0;

    TinyGPSPlus gps;

    SoftwareSerial ss(4, 3);

    n_ovfs++;
    // Read the data every 5 secs
    if (n_ovfs >= 5) {
        n_ovfs = 0;

        // Read values from Temp/Humid sensor
        twi_start();
        if (twi_write((SENSOR_ADR<<1) | TWI_WRITE) == 0) {
            // Set internal memory location
            twi_write(SENSOR_HUM_MEM);
            twi_stop();

            // Read data from internal memory
            twi_start();
            twi_write((SENSOR_ADR<<1) | TWI_READ);
            dht12.hum_int = twi_read(TWI_ACK);
            dht12.hum_dec = twi_read(TWI_ACK);
            dht12.temp_int = twi_read(TWI_ACK);
            dht12.temp_dec = twi_read(TWI_ACK);
            dht12.checksum = twi_read(TWI_NACK);
            twi_stop();

            new_gps_data = 1;
        }
        else {
            twi_stop();
        }
    }
}