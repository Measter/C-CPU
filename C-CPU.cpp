#include <Arduino.h>
#include "Registers.h"
#include "Pins.h"
#include "Memory.h"
#include "Core.h"
#include "debug.h"

void setup()
{
	Serial.begin(56000);

	if (pins.IsDebugEnabled()) {
		Serial.println("C-CPU Emulator");
		Serial.print("Zeroing RAM...");
		ram.ZeroMemory();
		Serial.println("Done");
	}

	registers.InitRegisters();
}

void loop()
{
	pins.UpdateInputs();

	if (pins.IsDebugEnabled() && UpdateDebugInput()) {
		ram.SetMemory(registers.GetPC(), GetDebugInput(), true);
		registers.StepPC();
	}

	if (pins.Buttons.Reset.wasPressed()) {
		registers.SetPC(pins.Multiplexers.Memory.read());
		if (pins.IsDebugEnabled()) {
			Serial.print("PC Set: ");
			PrintWord(registers.GetPC(), true);
		}
	}

	if (pins.Buttons.Deposit.wasPressed()) {
		ram.SetMemory(registers.GetPC(), pins.Multiplexers.Data.read(), true);
		registers.StepPC();
	}

	if (pins.Buttons.Step.wasPressed()) {
		core.ExecuteNextInstruction();
	}

	if (pins.Buttons.MemDump.wasPressed() && pins.IsDebugEnabled()) {
		registers.DumpRegisters();
		ram.DumpMemory();
	}

	pins.SetMemoryLEDs(ram.GetLastMemoryAddress());

	delay(1);
}
