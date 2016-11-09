#include "Registers.h"
#include "Debug.h"
#include <Arduino.h>
#include "Memory.h"

Registers registers;

void Registers::InitRegisters() {
	r0 = r1 = r2 = r3 = 0;

	pc = 0x0;
	status = 0x0;
	sp = ram.GetMemMax();
}

void Registers::DumpRegisters() const {
	Serial.println();
	Serial.println("Register Dump:");

	for( unsigned char i = 0; i <= REG_MAX; i++ ) {
		Serial.print("  R");
		Serial.print(i);
		Serial.print(": ");
		PrintWord(reg_arr[i], false);

		if( i == 3 ) 
			Serial.println();
	}

	Serial.println();
	Serial.print("  PC: ");
	PrintWord(pc, false);
	Serial.print("  SP: ");
	PrintWord(sp, true);

	Serial.println("               ONZ");
	Serial.print("  Status: ");
	PrintByteBin(status, true);
}

unsigned int Registers::GetPC() const {
	return pc;
}

unsigned char Registers::GetStatus() const {
	return status;
}

unsigned int Registers::GetSP() const {
	return sp;
}


unsigned int Registers::GetRegister(unsigned char id) const {
	return reg_arr[id & REG_MAX];
}

void Registers::SetPC(unsigned int addr) {
	pc = addr;
}

void Registers::SetRegister(unsigned char id, unsigned int value) {
	reg_arr[id & REG_MAX] = value;
}

void Registers::SetStatus(unsigned char val) {
	status = val;
}

void Registers::IncSP() {
	sp++;
}

void Registers::DecSP() {
	sp--;
}

void Registers::StepPC() {
	pc++;
}

