#ifndef IOEXPANDER_H
#define IOEXPANDER_H

#include <Arduino.h>
#include <Wire.h>

// register address
#define IODIRA 	0x00
#define IODIRB 	0x01
#define IPOLA  	0x02
#define IPOLB 	0x03
#define GPINTENA 0x04
#define GPINTENB 0x05
#define DEFVALA	0x06
#define DEFVALB 0x07
#define INTCONA 0x08
#define INTCONB 0x09
#define IOCONA	0x0A
#define IOCONB 	0x0B
#define GPPUA   0x0C
#define GPPUB 	0x0D
#define INTFA	0x0E
#define INTFB 	0x0F
#define INTCAPA	0x10
#define INTCAPB 0x11
#define GPIOA	0x12
#define GPIOB  	0x13
#define OLATA	0x14
#define OLATB	0x15

#define HIGH 0x1
#define LOW  0x0

#define MCP_PORT(pin) ((pin < 8) ? 0 : 1) //!< Determine port from pin number
#define IODIR(pin)	((pin < 8) ? IODIRA  : IODIRB);
#define GPPU(pin) ((pin < 8) ? GPPUA  : GPPUB);
#define GPIO(pin) ((pin < 8) ? GPIOA  : GPIOB);
class IOEXPANDER{
	public :
		bool init(uint8_t sda, uint8_t scl, uint32_t freq);
		bool init(uint8_t sda, uint8_t scl, uint32_t freq, uint8_t iodira, uint8_t iodirb);
		void digitalWrites(uint8_t pin, uint8_t val);
  
		uint8_t pinMode(uint8_t pin, uint8_t mode);
		uint8_t digitalRead(uint8_t pin);
		void digitalWrite(uint8_t pin, uint8_t value);
		void showAllConfigure();
		void clearAllConfigure();
	private:
		bool _return;
		uint8_t getRegister(uint8_t baseAddress);
		uint8_t checkvalue(uint8_t pin, uint8_t val, uint8_t statusPortA);
		void writeRegister(uint8_t address, uint8_t data);
		uint8_t _value;
		uint8_t _valuereturn;
		uint8_t _statusPortA;
		uint8_t _statusPortB;
		uint8_t _mode;
		uint8_t _logic;
		uint8_t _pin;
		uint8_t _dataWrite;
		uint8_t _dir;
		uint8_t _pullup;
		uint8_t _gpio;
};

#endif