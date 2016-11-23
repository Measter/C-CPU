#ifndef PINS_H
#define PINS_H

#include "Button.h"
#include "Multiplexer4051.h"
#include "ShiftRegister595.h"

class IO {
#define BTN_DEBOUNCE 10
#define RESET_PC_PIN 7
#define DEPOSIT_PIN 8
#define STEP_PIN 9
#define MEMORY_DUMP_PIN 10

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
	void UpdateInputs();

	void SetMemoryLEDs(unsigned int value) const;
	void SetDataLEDs(unsigned int value) const;

	bool IsDebugEnabled() const;

	struct {
		Button Reset = Button(RESET_PC_PIN, BTN_DEBOUNCE, true, false);
		Button Deposit = Button(DEPOSIT_PIN, BTN_DEBOUNCE, true, false);
		Button Step = Button(STEP_PIN, BTN_DEBOUNCE, true, false);
		Button MemDump = Button(MEMORY_DUMP_PIN, BTN_DEBOUNCE, true, false);
	} Buttons;

	struct {
		Multiplexer4051<2> Memory = Multiplexer4051<2>(MUX_MEM_ADDR0, MUX_MEM_ADDR1, MUX_MEM_ADDR2, false, true, MUX_MEM_COMMON_HIGH, MUX_MEM_COMMON_LOW);
		Multiplexer4051<2> Data = Multiplexer4051<2>(MUX_DATA_ADDR0, MUX_DATA_ADDR1, MUX_DATA_ADDR2, false, true, MUX_DATA_COMMON_HIGH, MUX_DATA_COMMON_LOW);
	} Multiplexers;
};

extern IO pins;

#endif
