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

	if (pins.Multiplexers.Buttons.wasPressed(BTN_RESET_PC_ADDR)) {
		registers.setPC(pins.Multiplexers.Memory.read());
		if (pins.isDebugEnabled()) {
			Serial.print("PC Set: ");
			printWord(registers.getPC(), true);
		}
	}

	if (pins.Multiplexers.Buttons.wasPressed(BTN_DEPOSIT_ADDR)) {
		ram.setMemory(registers.getPC(), pins.Multiplexers.Data.read(), true);
		registers.stepPC();
	}

	if (pins.Multiplexers.Buttons.wasPressed(BTN_STEP_ADDR)) {
		core.executeNextInstruction();
	}

	if (pins.Multiplexers.Buttons.wasPressed(BTN_MEMORY_DUMP_ADDR) && pins.isDebugEnabled()) {
		registers.dumpRegisters();
		ram.dumpMemory();
	}

	pins.setMemoryLEDs(ram.getLastMemoryAddress());

	delay(1);
}
