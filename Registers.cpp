#include "Registers.h"
#include "Debug.h"
#include <Arduino.h>
#include "Memory.h"

Registers registers;

void Registers::initRegisters() {
	r0 = r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;

	pc = 0x0;
	status = 0x0;
	sp = ram.getMemMax()+1;
}

void Registers::dumpRegisters() const {
	Serial.println();
	Serial.println("Register Dump:");

	for( unsigned char i = 0; i <= REG_MAX; i++ ) {
		Serial.print("  R");
		Serial.print(i);
		Serial.print(": ");
		printWord(reg_arr[i], false);

		if( i == 3 ) 
			Serial.println();
	}

	Serial.println();
	Serial.print("  PC: ");
	printWord(pc, false);
	Serial.print("  SP: ");
	printWord(sp, true);

	Serial.println("               ONZ");
	Serial.print("  Status: ");
	printByteBin(status, true);
}

unsigned int Registers::getPC() const {
	return pc;
}

unsigned char Registers::getStatus() const {
	return status;
}

unsigned int Registers::getSP() const {
	return sp;
}


unsigned int Registers::getRegister(unsigned char id) const {
	return reg_arr[id & REG_MAX];
}

void Registers::setPC(unsigned int addr) {
	pc = addr;
}

void Registers::setRegister(unsigned char id, unsigned int value) {
	reg_arr[id & REG_MAX] = value;
}

void Registers::setStatus(unsigned char val) {
	status = val;
}

void Registers::incSP() {
	sp++;
}

void Registers::decSP() {
	sp--;
}

void Registers::stepPC() {
	pc++;
}

