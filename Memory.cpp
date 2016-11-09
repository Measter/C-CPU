#include "Memory.h"
#include "Pins.h"
#include "Debug.h"

#include <Arduino.h>

Memory ram;

Memory::Memory() {
	m_LastMemoryAddress = 0;
}

void Memory::ZeroMemory() {
	long i;
	for (i = 0; i <= mem_max; i++) {
		m_memory[i] = 0;
	}
}

unsigned int Memory::GetMemMax() {
	return mem_max;
}



void Memory::SetMemory(unsigned int address, unsigned int value, bool updateLastAddress) {
	if (pins.IsDebugEnabled()) {
		Serial.print("MemSet: ");
		PrintWord(address, false);
		Serial.print(": ");
		PrintWord(value, true);
	}

	if (updateLastAddress && m_LastMemoryAddress != address)
		m_LastMemoryAddress = address;

	m_memory[address & mem_max] = value;
}

unsigned int Memory::GetMemory(unsigned int address, bool updateLastAddress) {
	if ( updateLastAddress && m_LastMemoryAddress != address)
		m_LastMemoryAddress = address;

	unsigned int value = m_memory[address];

	if (pins.IsDebugEnabled())
	{
		Serial.print("MemGet: ");
		PrintWord(address, false);
		Serial.print(": ");
		PrintWord(value, true);
	}

	return value;
}


unsigned int Memory::GetLastMemoryAddress() const {
	return m_LastMemoryAddress;
}



void Memory::DumpMemory() {
	Serial.println();
	Serial.println("Memory Dump:");

	Serial.print("       ");
	unsigned char j;
	for (j = 0; j < 16; j++) {
		PrintWord(j, false);
		Serial.print(" ");
	}

	Serial.print("       ");
	for (j = 0; j < 16; j++) {
		Serial.print("-----");
	}

	j = 16;
	for (unsigned int i = 0; i <= mem_max; i++, j++) {
		if (j == 16) {
			j = 0;
			Serial.print("\n ");
			PrintWord(i, false);
			Serial.print("| ");
		}

		PrintWord(m_memory[i], false);
		Serial.print(" ");
	}

	Serial.println();
	Serial.println();
}
