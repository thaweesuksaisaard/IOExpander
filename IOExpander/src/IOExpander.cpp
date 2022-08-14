#include <IOExpander.h>

bool IOEXPANDER::init(uint8_t sda, uint8_t scl, uint32_t freq)
{
	_return = false;
	
	if(Wire.begin(sda, scl, freq)){
		clearAllConfigure();
		_return = true;
	}
	
	return _return;
}


bool IOEXPANDER::init(uint8_t sda, uint8_t scl, uint32_t freq, uint8_t iodira, uint8_t iodirb)
{
	_return = false;
	
	if(Wire.begin(sda, scl, freq)){
		_return = true;
	}
	/*
	 // configure ICON use BANK 0
    Wire.beginTransmission(0x20);
    Wire.write(0x05);               
    Wire.write(0b00000000);       
    Wire.endTransmission();
	delay(100);
	
	// Port A
	// set I/O pins IODIRA
	Wire.beginTransmission(0x20);
	Wire.write(0x00);             	// Address value Port A
	Wire.write(iodira);				// define input or output
	//Wire.write(0x00);
	Wire.endTransmission();
	delay(100);
	// Port B
	// set I/O pins IODIRB
	Wire.beginTransmission(0x20);
	Wire.write(0x01);             	// Address value Port B
	Wire.write(iodirb);				// define input or output
	//Wire.write(0xFF);
	Wire.endTransmission();
	delay(100);
	// check input port 
	if(iodirb != 0x00)
	{
		Wire.beginTransmission(0x20);
		Wire.write(0x0D);               
		Wire.write(iodirb);       
		Wire.endTransmission();
		delay(100);
	}
	// Port A
	// clear rigister
	Wire.beginTransmission(0x20);
	Wire.write(0x12);
	Wire.write(0x00);
	Wire.endTransmission();
	delay(100);
	// Port B
	// clear rigister
	Wire.beginTransmission(0x20);
	Wire.write(0x13);
	Wire.write(0x00);
	Wire.endTransmission();
	delay(100);
	*/
	Wire.beginTransmission(0x20);
    Wire.write(0x05);               
    Wire.write(0b00000000);       
    Wire.endTransmission();
    delay(100);
    // Direction
    Wire.beginTransmission(0x20);
    Wire.write(0x01);               
    Wire.write(0x00);       
    Wire.endTransmission();
    delay(100);
    // enable pullup
    Wire.beginTransmission(0x20);
    Wire.write(0x0D);               
    Wire.write(0xFF);       
    Wire.endTransmission();
    delay(100);
    //  inverted
    //Wire.beginTransmission(0x20);
    //Wire.write(0x03);               
    //Wire.write(0x00);       
    //Wire.endTransmission();
    //delay(100);
    // Port A
    // Direction
    Wire.beginTransmission(0x20);
    Wire.write(0x00);               
    Wire.write(0x00);       
    Wire.endTransmission();
    delay(100);
	
	return _return;
}

uint8_t IOEXPANDER::pinMode(uint8_t pin, uint8_t mode)
{
	_dir = IODIR(pin);					// return 0x00 pin is 0-7, return 0x01 pin is 8-15
	_value = getRegister(_dir);
	
	_logic = (mode == OUTPUT) ? 0:1;
	if(pin<8)
		_pin = pin;
	else
		_pin = pin%8;
	
	_dataWrite = checkvalue(_pin, _logic, _value);
	
	// write direction
	writeRegister(_dir, _dataWrite);
	
	// check input pullup	
	_logic = (mode == INPUT_PULLUP) ? 1:0;
	_pullup = GPPU(pin);
	_value = getRegister(_pullup);
	_dataWrite = checkvalue(_pin, _logic, _value);
	writeRegister(_pullup, _dataWrite);
	
	return 0x01;
}

uint8_t IOEXPANDER::digitalRead(uint8_t pin)
{
	_gpio = GPIO(pin);
	_value = getRegister(_gpio);
	
	if(pin<8)
		_pin = pin;
	else
		_pin = pin%8;
	
	return  ((((_value>>_pin)&0x01) == 0) ? LOW : HIGH);
}

void IOEXPANDER::digitalWrite(uint8_t pin, uint8_t val)
{
	_gpio = GPIO(pin);
	_value = getRegister(_gpio);
	
	_logic = (val == LOW) ? 0:1;
	if(pin<8)
		_pin = pin;
	else
		_pin = pin%8;
	
	_dataWrite = checkvalue(_pin, _logic, _value);
	
	// write GPIO
	writeRegister(_gpio, _dataWrite);
}


uint8_t IOEXPANDER::checkvalue(uint8_t pin, uint8_t val, uint8_t statusPortA)
{
	switch(pin)
	{
		case 0x00 :
			if((statusPortA&0b00000001) == 0b00000001)
			{
				if(val == 0x00)
					_valuereturn = statusPortA & 0b11111110;
				else 
					_valuereturn = statusPortA & 0b11111111;
			}
			else if((statusPortA&0b00000001) == 0b00000000)
				_valuereturn = statusPortA | val;
			break;
		case 0x01 :
			if((statusPortA&0b00000010) == 0b00000010)
			{
				if(val == 0x00)
					_valuereturn = statusPortA & 0b11111101;
				else 
					_valuereturn = statusPortA & 0b11111111;
			}
			else if((statusPortA&0b00000010) == 0b00000000)
				_valuereturn = statusPortA | val<<1;
			break;
		case 0x02 :
			if((statusPortA&0b00000100) == 0b00000100)
			{
				if(val == 0x00)
					_valuereturn = statusPortA & 0b11111011;
				else 
					_valuereturn = statusPortA & 0b11111111;
			}
			else if((statusPortA&0b00000100) == 0b00000000)
				_valuereturn = statusPortA | val<<2;
			break;
		case 0x03 :
			if((statusPortA&0b00001000) == 0b00001000)
			{
				if(val == 0x00)
					_valuereturn = statusPortA & 0b11110111;
				else 
					_valuereturn = statusPortA & 0b11111111;
			}
			else if((statusPortA&0b00001000) == 0b00000000)
				_valuereturn = statusPortA | val<<3;
			break;
		case 0x04 :
			if((statusPortA&0b00010000) == 0b00010000)
			{
				if(val == 0x00)
					_valuereturn = statusPortA & 0b11101111;
				else 
					_valuereturn = statusPortA & 0b11111111;
			}
			else if((statusPortA&0b00010000) == 0b00000000)
				_valuereturn = statusPortA | val<<4;
			break;
		case 0x05 :
			if((statusPortA&0b00100000) == 0b00100000)
			{
				if(val == 0x00)
					_valuereturn = statusPortA & 0b11011111;
				else 
					_valuereturn = statusPortA & 0b11111111;
			}	
			else if((statusPortA&0b00100000) == 0b00000000)
				_valuereturn = statusPortA | val<<5;
			break;
		case 0x06 :
			if((statusPortA&0b01000000) == 0b01000000)
			{
				if(val == 0x00)
					_valuereturn = statusPortA & 0b10111111;
				else 
					_valuereturn = statusPortA & 0b11111111;
			}	
			else if((statusPortA&0b01000000) == 0b00000000)
				_valuereturn = statusPortA | val<<6;
			break;
		case 0x07 :
			if((statusPortA&0b10000000) == 0b10000000)
			{
				if(val == 0x00)
					_valuereturn = statusPortA & 0b01111111;
				else 
					_valuereturn = statusPortA & 0b11111111;
			}	
			else if((statusPortA&0b10000000) == 0b00000000)
				_valuereturn = statusPortA | val<<7;
			break;
	}
	
	return _valuereturn;
}

uint8_t IOEXPANDER::getRegister(uint8_t baseAddress)
{
	Wire.beginTransmission(0x20);
	Wire.write(baseAddress);
	Wire.endTransmission();
	Wire.requestFrom(0x20, 1);
	return Wire.read();
}

void IOEXPANDER::writeRegister(uint8_t address, uint8_t data)
{
	Wire.beginTransmission(0x20);
	Wire.write(address);
	Wire.write(data);
	Wire.endTransmission();
}

void IOEXPANDER::showAllConfigure()
{
	for(uint8_t i = 0 ; i <= 0x15;i++)
	{
		switch(i)
		{
			case 0x00 :
				Serial.print("IODIRA   0x00 : ");
				break;
			case 0x01 :
				Serial.print("IODIRB   0x01 : ");
				break;
			case 0x02 :
				Serial.print("IPOLA    0x02 : ");
				break;
			case 0x03 :
				Serial.print("IPOLB    0x03 : ");
				break;
			case 0x04 :
				Serial.print("GPINTENA 0x04 : ");
				break;
			case 0x05 :
				Serial.print("GPINTENB 0x05 : ");
				break;
			case 0x06 :
				Serial.print("DEFVALA  0x06 : ");
				break;
			case 0x07 :
				Serial.print("DEFVALB  0x07 : ");
				break;
			case 0x08 :
				Serial.print("INTCONA  0x08 : ");
				break;
			case 0x09 :
				Serial.print("INTCONB  0x09 : ");
				break;
			case 0x0A :
				Serial.print("IOCONA   0x0A : ");
				break;
			case 0x0B :
				Serial.print("IOCONB   0x0B : ");
				break;
			case 0x0C :
				Serial.print("GPPUA    0x0C : ");
				break;
			case 0x0D :
				Serial.print("GPPUB    0x0D : ");
				break;
			case 0x0E :
				Serial.print("INTFA    0x0E : ");
				break;
			case 0x0F :
				Serial.print("INTFB    0x0F : ");
				break;
			case 0x10 :
				Serial.print("INTCAPA  0x10 : ");
				break;
			case 0x11 :
				Serial.print("INTCAPB  0x11 : ");
				break;
			case 0x12 :
				Serial.print("GPIOA    0x12 : ");
				break;
			case 0x13 :
				Serial.print("GPIOB    0x13 : ");
				break;
			case 0x14 :
				Serial.print("OLATA    0x14 : ");
				break;
			case 0x15 :
				Serial.print("OLATB    0x15 : ");
				break;
		}
		Serial.println(getRegister(i), BIN);
	}
}

void IOEXPANDER::clearAllConfigure()
{
	for(uint8_t i = 0; i <= 1; i++)
	{
		writeRegister(i, 0xFF);
	}
	
	for(uint8_t i = 2; i <= 5; i++)
	{
		writeRegister(i, 0x00);
	}
	
	for(uint8_t i = 0x0C; i <= 0x15; i++)
	{
		writeRegister(i, 0x00);
	}
}