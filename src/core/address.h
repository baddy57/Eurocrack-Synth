#pragma once

#include <cstdint>

#include <Wire.h>

#include "../configuration.h"
#include "../hardware_setup/motherboard.h"

//partial address for a module slot
class Address{
	protected:
		uint_fast8_t arduinoPin;
		bool sel[6]; //module slot selectors
		unsigned int _id;
	public:
		Address(uint_fast8_t);
		Address(const Address&);
		bool getSel(uint_fast8_t i) const{return sel[i];};
		void setForReading() const;
		virtual void setForWriting() const{};
		uint_fast8_t getArduinoPin() const{return this->arduinoPin;};
		uint_fast8_t getId() const{return _id;};
		uint_fast8_t getModulePin() const {return _id/64;};
};

//address of a switch, a button, a pot or an inputSocket
class ControlAddress : public Address {
	private:
		bool sel2[6]; //module mux's selectors
	public:
		ControlAddress(const Address&, uint_fast8_t);
		ControlAddress(uint_fast8_t);
		void setForReading()const;
};

//address of an OutputSocket, has different pins
class OutputSocketAddress : public Address {
	private:
		bool sel2[3]; //socket mux selectors
	public:
		OutputSocketAddress(const Address&, uint_fast8_t);
		void setForWriting() const;
		uint_fast8_t getArduinoPin() const{return this-> arduinoPin;};
};


class IdMuxAddress : public Address {
	private:
		bool sel2[3];
	public:
		IdMuxAddress(const Address&);
		void setForReading() const;
};
