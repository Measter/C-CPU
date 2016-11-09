#ifndef PIN_SETUP_H
#define PIN_SETUP_H
#include "InstructionMasks.h"

class Core
{
	enum class Status : unsigned char { Leave, Set, Clear };

	bool m_hadInvalidOpCode = false;

	void HadInvalidOpCode(unsigned int opcode);
	static unsigned int GetNextWord();

	static bool IsNegative(unsigned int val);
	static unsigned int Reverse(unsigned int b);
	static unsigned char Reverse(unsigned char b);

	static bool StatusTest(Condition condition);
	static void SetStatus(Status over, Status neg, Status zero);

	static void InstrJEQ(unsigned int instruction);
	static void InstrJNE(unsigned int instruction);
	static void InstrJLT(unsigned int instruction);
	static void InstrJGT(unsigned int instruction);
	static void InstrJC(unsigned int instruction);
	static void InstrJNC(unsigned int instruction);
	static void InstrJMP(unsigned int instruction);
		 
	static void InstrJCCHelper(Condition condition);
		  
	static void InstrADD(unsigned int instruction);
	static void InstrSUB(unsigned int instruction);
	static void InstrNOT(unsigned int instruction);
	static void InstrAND(unsigned int instruction);
	static void InstrOR(unsigned int instruction);
	static void InstrXOR(unsigned int instruction);
	static void InstrSHR(unsigned int instruction);
	static void InstrSHL(unsigned int instruction);
		   
	static void InstrPUSH(unsigned int instruction);
	static void InstrPOP(unsigned int instruction);
	static void InstrCMP(unsigned int instruction);

	static void InstrSETI(unsigned int instruction);
	static void InstrSETIL(unsigned int instruction);
	static void InstrSTR(unsigned int instruction);
	static void InstrLD(unsigned int instruction);
	static void InstrSET(unsigned int instruction);

public:
	void ExecuteNextInstruction();
	bool GetHadInvalidOpCode() const;
};

extern Core core;

#endif