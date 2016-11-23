#include "Memory.h"
#include "Pins.h"
#include "Debug.h"

#include <Arduino.h>

Memory ram;

Memory::Memory() {
	m_LastMemoryAddress = 0;
}

void Memory::zeroMemory() {
	long i;
	for (i = 0; i <= mem_max; i++) {
		m_memory[i] = 0;
	}
}

unsigned int Memory::getMemMax() {
	return mem_max;
}



void Memory::setMemory(unsigned int address, unsigned int value, bool updateLastAddress) {
	if (pins.isDebugEnabled()) {
		Serial.print("MemSet: ");
		printWord(address, false);
		Serial.print(": ");
		printWord(value, true);
	}

	if (updateLastAddress && m_LastMemoryAddress != address)
		m_LastMemoryAddress = address;

	m_memory[address & mem_max] = value;
}

unsigned int Memory::getMemory(unsigned int address, bool updateLastAddress) {
	if ( updateLastAddress && m_LastMemoryAddress != address)
		m_LastMemoryAddress = address;

	unsigned int value = m_memory[address];

	if (pins.isDebugEnabled())
	{
		Serial.print("MemGet: ");
		printWord(address, false);
		Serial.print(": ");
		printWord(value, true);
	}

	return value;
}


unsigned int Memory::getLastMemoryAddress() const {
	return m_LastMemoryAddress;
}



void Memory::dumpMemory() {
	Serial.println();
	Serial.println("Memory Dump:");

	Serial.print("       ");
	unsigned char j;
	for (j = 0; j < 16; j++) {
		printWord(j, false);
		Serial.print(" ");
	}

	Serial.println();
	Serial.print("       ");
	for (j = 0; j < 16; j++) {
		Serial.print("-----");
	}

	j = 16;
	for (unsigned int i = 0; i <= mem_max; i++, j++) {
		if (j == 16) {
			j = 0;
			Serial.print("\n ");
			printWord(i, false);
			Serial.print("| ");
		}

		printWord(m_memory[i], false);
		Serial.print(" ");
	}

	Serial.println();
	Serial.println();
}
