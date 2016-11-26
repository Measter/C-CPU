#include "Pins.h"

IO pins;



// Needs to read from switches.
IO::IO() {
	is_debug = true;		// Hardcoded for now. Will have physical switch later.

	bool modes[8] = { false, false, false, false, false, false, true, false };
	Multiplexers.Buttons = MultiplexedButtons4051(MUX_BTN_ADDR0, MUX_BTN_ADDR1, MUX_BTN_ADDR2,
												  MUX_BTN_COMMON, true, modes, BTN_DEBOUNCE);

	data_input = 0;
	last_mem_out = 0;
	last_data_out = 0;

	m_memoryRegister.write(0);
}

void IO::updateInputs
() {
	Multiplexers.Buttons.updateState();
}

void IO::setMemoryLEDs(unsigned int value) const {
	m_memoryRegister.write(value & 0xFF, value >> 8);
}

void IO::setDataLEDs(unsigned int value) const {

}

bool IO::isDebugEnabled() const {
	return is_debug;
}

