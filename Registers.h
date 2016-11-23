#ifndef REGISTERS_H
#define REGISTERS_H

#define X_BIT 4
#define N_BIT 3
#define Z_BIT 2
#define V_BIT 1
#define C_BIT 0

class Registers {
#define REG_MAX 0x7
#define STATUS_OVERFLOW 2
#define STATUS_NEG 1
#define STATUS_ZERO 0
	union {
		unsigned int reg_arr[REG_MAX+1];
		struct {
			unsigned int r0;
			unsigned int r1;
			unsigned int r2;
			unsigned int r3;
			unsigned int r4;
			unsigned int r5;
			unsigned int r6;
			unsigned int r7;
		};
	};
	unsigned int pc = 0;
	unsigned char status = 0;
	unsigned int sp = 0;

public:
	void initRegisters();
	void dumpRegisters() const;
	unsigned int getRegister(unsigned char id) const;
	unsigned int getPC() const;
	unsigned char getStatus() const;
	unsigned int getSP() const;
	void setRegister(unsigned char id, unsigned int value);
	void setPC(unsigned int addr);
	void setStatus(unsigned char val);

	void incSP();
	void decSP();
	void stepPC();
};

extern Registers registers;

#endif
