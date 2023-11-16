#include <avr/io.h>

#ifndef F_CPU
# define F_CPU 16000000 /**< @brief CPU frequency in Hz required TWI_BIT_RATE_REG */
#endif

#define GPS_TXD_PIN 4
#define GPS_RXD_PIN 3

