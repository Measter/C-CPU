#ifndef PINS_H
#define PINS_H

#include "Multiplexer4051.h"
#include "ShiftRegister595.h"
#include "MultiplexedButtons4051.h"

class IO {
#define BTN_DEBOUNCE 10
#define BTN_RESET_PC_ADDR 5
#define BTN_DEPOSIT_ADDR 2
#define BTN_STEP_ADDR 7
#define BTN_MEMORY_DUMP_ADDR 4
#define BTN_RUN_ADDR 6

#define MUX_BTN_ADDR0 2
#define MUX_BTN_ADDR1 3
#define MUX_BTN_ADDR2 4
#define MUX_BTN_COMMON 7

#define MUX_DATA_ADDR0 2
#define MUX_DATA_ADDR1 3
#define MUX_DATA_ADDR2 4
#define MUX_DATA_COMMON_HIGH 5
#define MUX_DATA_COMMON_LOW 6


#define MUX_MEM_ADDR0 2
#define MUX_MEM_ADDR1 3
#define MUX_MEM_ADDR2 4
#define MUX_MEM_COMMON_HIGH 5
#define MUX_MEM_COMMON_LOW 6

#define SHIFT_MEM_DATA 11
#define SHIFT_MEM_LATCH 13
#define SHIFT_MEM_CLOCK 12

private:
	unsigned int data_input;
	unsigned int last_mem_out;
	unsigned int last_data_out;
	bool is_debug;
	ShiftRegister595 m_memoryRegister = ShiftRegister595(SHIFT_MEM_DATA, SHIFT_MEM_CLOCK, SHIFT_MEM_LATCH, false);

public:
	IO();
	void updateInputs();

	void setMemoryLEDs(unsigned int value) const;
	void setDataLEDs(unsigned int value) const;

	bool isDebugEnabled() const;

	struct {
		Multiplexer4051<2> Memory = Multiplexer4051<2>(MUX_MEM_ADDR0, MUX_MEM_ADDR1, MUX_MEM_ADDR2, false, true, MUX_MEM_COMMON_HIGH, MUX_MEM_COMMON_LOW);
		Multiplexer4051<2> Data = Multiplexer4051<2>(MUX_DATA_ADDR0, MUX_DATA_ADDR1, MUX_DATA_ADDR2, false, true, MUX_DATA_COMMON_HIGH, MUX_DATA_COMMON_LOW);
		MultiplexedButtons4051 Buttons;
	} Multiplexers;
};

extern IO pins;

#endif
