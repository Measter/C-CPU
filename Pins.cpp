#include "Pins.h"

IO pins;



// Needs to read from switches.
IO::IO() {
	is_debug = true;		// Hardcoded for now. Will have physical switch later.

	data_input = 0;
	last_mem_out = 0;
	last_data_out = 0;

	m_memoryRegister.Write(0);
}

void IO::UpdateInputs
() {
	Buttons.Reset.UpdateState();
	Buttons.Deposit.UpdateState();
	Buttons.MemDump.UpdateState();
	Buttons.Step.UpdateState();
}

void IO::SetMemoryLEDs(unsigned int value) const {
	m_memoryRegister.Write(value & 0xFF, value >> 8);
}

void IO::SetDataLEDs(unsigned int value) const {

}

bool IO::IsDebugEnabled() const {
	return is_debug;
}

