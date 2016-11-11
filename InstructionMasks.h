#ifndef INSTRUCTION_MASKS_H
#define INSTRUCTION_MASKS_H

/////////////////////////
// Conditional Tests
/////////////////////////

enum class Condition : unsigned char {
	NotEqual, Equal,
	OverflowClear, Overflow,
	Plus, Minus,
	LessThan, GreaterThan
};

#define GetInstrRegA	((instruction & 0x700) >> 8)
#define GetInstrRegB	(instruction & 0x7)


/////////////////////////
// Instructions OpCodes
/////////////////////////

#define OPCODE_MASK		0xF800

enum class OpCode : unsigned int {
	JEQ		= 0x0000,
	JNE		= 0x0800,
	JLT		= 0x1000,
	JGT		= 0x1800,
	JC		= 0x2000,
	JNC		= 0x2800,
	JMP		= 0x3000,
	
	ADD		= 0x3800,
	SUB		= 0x4000,
	NOT		= 0x4800,
	AND		= 0x5000,
	OR		= 0x5800,
	XOR		= 0x6000,
	SHR		= 0x6800,
	SHL		= 0x7000,
				   
	PUSH	= 0x7800,
	POP		= 0x8000,
	CMP		= 0x8800,


	SETI	= 0x9000,			   
	SETIL	= 0x9800,
	STR		= 0xA000,
	LD		= 0xA800,
	SET		= 0xB000,
};

#endif