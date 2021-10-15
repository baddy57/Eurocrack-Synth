#ifndef __ADDRESS_H__
#define __ADDRESS_H__

#include "HardwareCfg.h"
#include <cstdint>


//partial address for a module slot
class Address{
	protected:
		uint_fast8_t pin;
		bool sel[6]; //module slot selectors
	public:
		unsigned int _id;
		Address(uint_fast8_t);
		Address(const Address&);
		bool getSel(uint_fast8_t i) const{return sel[i];};
		void setForReading() const;
		virtual void setForWriting() const{};
		uint_fast8_t getPin() const{return this->pin;};
		uint_fast8_t toInt()const{return _id;};
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
		uint_fast8_t getPin() const{return this-> pin;};
};


class IdMuxAddress : public Address {
	private:
		bool sel2[3];
	public:
		IdMuxAddress(const Address&);
		void setForReading() const;
};

#endif