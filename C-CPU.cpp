#include <Arduino.h>
#include "Registers.h"
#include "Pins.h"
#include "Memory.h"
#include "Core.h"
#include "debug.h"

void setup()
{
	Serial.begin(56000);

	if (pins.isDebugEnabled()) {
		Serial.println("C-CPU Emulator");
		Serial.print("Zeroing RAM...");
		ram.zeroMemory();
		Serial.println("Done");
	}

	registers.initRegisters();
}

void loop()
{
	pins.updateInputs();

	if (pins.isDebugEnabled() && updateDebugInput()) {
		ram.setMemory(registers.getPC(), getDebugInput(), true);
		registers.stepPC();
	}

	if (pins.Buttons.Reset.wasPressed()) {
		registers.setPC(pins.Multiplexers.Memory.read());
		if (pins.isDebugEnabled()) {
			Serial.print("PC Set: ");
			printWord(registers.getPC(), true);
		}
	}

	if (pins.Buttons.Deposit.wasPressed()) {
		ram.setMemory(registers.getPC(), pins.Multiplexers.Data.read(), true);
		registers.stepPC();
	}

	if (pins.Buttons.Step.wasPressed()) {
		core.executeNextInstruction();
	}

	if (pins.Buttons.MemDump.wasPressed() && pins.isDebugEnabled()) {
		registers.dumpRegisters();
		ram.dumpMemory();
	}

	pins.setMemoryLEDs(ram.getLastMemoryAddress());

	delay(1);
}
