#include "Pins.h"

IO pins;



// Needs to read from switches.
IO::IO() {
	is_debug = true;		// Hardcoded for now. Will have physical switch later.

	data_input = 0;
	last_mem_out = 0;
	last_data_out = 0;

	m_memoryRegister.write(0);
}

void IO::UpdateInputs
() {
	Buttons.Reset.updateState();
	Buttons.Deposit.updateState();
	Buttons.MemDump.updateState();
	Buttons.Step.updateState();
}

void IO::SetMemoryLEDs(unsigned int value) const {
	m_memoryRegister.write(value & 0xFF, value >> 8);
}

void IO::SetDataLEDs(unsigned int value) const {

}

bool IO::IsDebugEnabled() const {
	return is_debug;
}

