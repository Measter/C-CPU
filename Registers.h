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
	void InitRegisters();
	void DumpRegisters() const;
	unsigned int GetRegister(unsigned char id) const;
	unsigned int GetPC() const;
	unsigned char GetStatus() const;
	unsigned int GetSP() const;
	void SetRegister(unsigned char id, unsigned int value);
	void SetPC(unsigned int addr);
	void SetStatus(unsigned char val);

	void IncSP();
	void DecSP();
	void StepPC();
};

extern Registers registers;

#endif
