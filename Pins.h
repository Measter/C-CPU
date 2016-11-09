#ifndef PINS_H
#define PINS_H

#include "Button.h"
#include "Multiplexer.h"
#include "ShiftRegister.h"

class IO {
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
	ShiftRegister m_memoryRegister = ShiftRegister(SHIFT_MEM_DATA, SHIFT_MEM_CLOCK, SHIFT_MEM_LATCH, false);

public:
	IO();
	void UpdateInputs();

	void SetMemoryLEDs(unsigned int value) const;
	void SetDataLEDs(unsigned int value) const;

	bool IsDebugEnabled() const;

	struct {
		Button Reset = Button(RESET_PC_PIN, true);
		Button Deposit = Button(DEPOSIT_PIN, true);
		Button Step = Button(STEP_PIN, true);
		Button MemDump = Button(MEMORY_DUMP_PIN, true);
	} Buttons;

	struct {
		Multiplexer<2> Memory = Multiplexer<2>(MUX_MEM_ADDR0, MUX_MEM_ADDR1, MUX_MEM_ADDR2, false, true, MUX_MEM_COMMON_HIGH, MUX_MEM_COMMON_LOW);
		Multiplexer<2> Data = Multiplexer<2>(MUX_DATA_ADDR0, MUX_DATA_ADDR1, MUX_DATA_ADDR2, false, true, MUX_DATA_COMMON_HIGH, MUX_DATA_COMMON_LOW);
	} Multiplexers;
};

extern IO pins;

#endif
