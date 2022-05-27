/*
 * helpers.h
 *
 * Created: 22.05.2022 21:01:23
 *  Author: Heinrich
 */ 


#ifndef HELPERS_H_
#define HELPERS_H_

#include <stdint.h>
#include <stdbool.h>

static __inline uint16_t __bswap_16(uint16_t __x)
{
	return (__x << 8)|(__x >> 8);
}

static __inline uint16_t __make_u16t(uint8_t __x1, uint8_t __x2)
{
	// return (uint16_t)((__x2 << 8) + (__x1 & 0x00ff));
	return ((uint16_t) __x1 << 8) | __x2;
}

static __inline float __adc2voltage(uint16_t __v)
{
	return ((float) __v * 5UL / 1024UL );
}

static __inline uint16_t __adc2voltage_millis(uint16_t __v)
{
	return ((uint16_t) __v * 5000UL / 1024UL );
}

static __inline uint16_t __voltage2dac_millis (uint16_t millis)
{
	return (uint16_t) (millis * 1024UL / 5000UL);
}

/*
static __inline bool __isSet(uint8_t __x1, uint8_t __x2)
{
	return (__x1 & ( 1 << __x2));
}
*/

static __inline uint8_t __low(uint16_t __x)
{
	return (uint8_t) ((__x) & 0xFF);
}

static __inline uint8_t __high(uint16_t __x)
{
	return (uint8_t) (((__x)>>8) & 0xFF);
}

static __inline uint16_t __target_voltage_duty_cycle (uint16_t targetVoltageMillis, uint16_t supplyVoltageMillis)
{
	return (uint16_t) (((float)targetVoltageMillis / (float) supplyVoltageMillis)*1024 -1);
}

#define bswap_16(x) __bswap_16(x)
#define make_u16t(x1,x2) __make_u16t(x1,x2)
#define adc2voltage(adc) __adc2voltage(adc)
#define adc2voltage_millis(adc) __adc2voltage_millis(adc)
#define voltage2dac_millis(millis) __voltage2dac_millis(millis)
#define target_voltage_duty_cycle(targetVoltage,supplyVoltage) __target_voltage_duty_cycle(targetVoltage,supplyVoltage)

// #define isSet(x1, x2) __isSet(x1, x2)
#define low(x)	__low(x)
#define high(x)	__high(x)

#define BIT_SET(a,b) ((a) |= (1U<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1U<<(b)))
#define BIT_CHECK(var,pos) !!((var) & (1U<<(pos)))
#define BIT_TOGGLE(var,pos) ((var)^= 1UL << (pos))

#endif /* HELPERS_H_ */