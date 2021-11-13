/*
 Name:		test_pin.ino
 Created:	10/20/2021 7:01:46 PM
 Author:	cristian
*/
#include <SPI.h>
namespace pins {
	enum {
		RX,
		TX,
		WD,
		WE,
		WF,
		LCD_DC,
		WG,
		T_CS = 9,
		WH,
		__MOSI,
		__MISO,
		__SCK,
		READ,
		VOLUME,
		__IRQ = 16,
		WC,
		WRITE = 22,
		WA = 24,
		RGB1_3,
		WB,
		RGB0_3,
		WI,
		RL,
		RK,
		RJ,
		RI,
		RA,
		RB,
		RC,
		LCD_CS,
		RH,
		RG,
		RF,
		RE,
		RD
	};
};
// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(pins::RA, OUTPUT);
	pinMode(pins::RB, OUTPUT);
	pinMode(pins::RC, OUTPUT);
	pinMode(pins::RD, OUTPUT);
	pinMode(pins::RE, OUTPUT);
	pinMode(pins::RF, OUTPUT);
	pinMode(pins::RG, OUTPUT);
	pinMode(pins::RH, OUTPUT);
	pinMode(pins::RI, OUTPUT);
	pinMode(pins::RJ, OUTPUT);
	pinMode(pins::RK, OUTPUT);
	pinMode(pins::RL, OUTPUT);
	pinMode(pins::WA, OUTPUT);
	pinMode(pins::WB, OUTPUT);
	pinMode(pins::WC, OUTPUT);
	pinMode(pins::WD, OUTPUT);
	pinMode(pins::WE, OUTPUT);
	pinMode(pins::WF, OUTPUT);
	pinMode(pins::WG, OUTPUT);
	pinMode(pins::WH, OUTPUT);
	pinMode(pins::WI, OUTPUT);
	pinMode(pins::WRITE, OUTPUT);
	pinMode(pins::READ, INPUT);
	pinMode(pins::VOLUME, INPUT);
	pinMode(pins::RGB1_3, OUTPUT);
	pinMode(pins::RGB0_3, OUTPUT);
}

// the loop function runs over and over again until power down or reset
#define DELAY_ON 100
#define DELAY_OFF 1
void loop() {
	Serial.println("RA");
	digitalWrite(pins::RA, HIGH);
	delay(DELAY_ON);
	digitalWrite(pins::RA, LOW);
	delay(DELAY_OFF);

	Serial.println("RB");
	digitalWrite(pins::RB, HIGH);
	delay(DELAY_ON);
	digitalWrite(pins::RB, LOW);
	delay(DELAY_OFF);

	Serial.println("RC");
	digitalWrite(pins::RC, HIGH);
	delay(DELAY_ON);
	digitalWrite(pins::RC, LOW);
	delay(DELAY_OFF);

	Serial.println("RD");
	digitalWrite(pins::RD, HIGH);
	delay(DELAY_ON);
	digitalWrite(pins::RD, LOW);
	delay(DELAY_OFF);

	Serial.println("RE");
	digitalWrite(pins::RE, HIGH);
	delay(DELAY_ON);
	digitalWrite(pins::RE, LOW);
	delay(DELAY_OFF);

	Serial.println("RF");
	digitalWrite(pins::RF, HIGH);
	delay(DELAY_ON);
	digitalWrite(pins::RF, LOW);
	delay(DELAY_OFF);

	Serial.println("RG");
	digitalWrite(pins::RG, HIGH);
	delay(DELAY_ON);
	digitalWrite(pins::RG, LOW);
	delay(DELAY_OFF);

	Serial.println("RH");
	digitalWrite(pins::RH, HIGH);
	delay(DELAY_ON);
	digitalWrite(pins::RH, LOW);
	delay(DELAY_OFF);

	Serial.println("RI");
	digitalWrite(pins::RI, HIGH);
	delay(DELAY_ON);
	digitalWrite(pins::RI, LOW);
	delay(DELAY_OFF);

	Serial.println("RJ");
	digitalWrite(pins::RJ, HIGH);
	delay(DELAY_ON);
	digitalWrite(pins::RJ, LOW);
	delay(DELAY_OFF);

	Serial.println("RK");
	digitalWrite(pins::RK, HIGH);
	delay(DELAY_ON);
	digitalWrite(pins::RK, LOW);
	delay(DELAY_OFF);

	Serial.println("RL");
	digitalWrite(pins::RL, HIGH);
	delay(DELAY_ON);
	digitalWrite(pins::RL, LOW);
	delay(DELAY_OFF);
}
