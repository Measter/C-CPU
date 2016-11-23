#ifndef PIN_SETUP_H
#define PIN_SETUP_H
#include "InstructionMasks.h"

class Core
{
	enum class Status : unsigned char { Leave, Set, Clear };

	bool m_hadInvalidOpCode = false;

	void hadInvalidOpCode(unsigned int opcode);
	static unsigned int getNextWord();

	static bool isNegative(unsigned int val);
	static unsigned int reverse(unsigned int b);
	static unsigned char reverse(unsigned char b);

	static bool statusTest(Condition condition);
	static void setStatus(Status over, Status neg, Status zero);

	static void instrJEQ(unsigned int instruction);
	static void instrJNE(unsigned int instruction);
	static void instrJLT(unsigned int instruction);
	static void instrJGT(unsigned int instruction);
	static void instrJC(unsigned int instruction);
	static void instrJNC(unsigned int instruction);
	static void instrJMP(unsigned int instruction);
		 
	static void instrJCCHelper(Condition condition);
		  
	static void instrADD(unsigned int instruction);
	static void instrSUB(unsigned int instruction);
	static void instrNOT(unsigned int instruction);
	static void instrAND(unsigned int instruction);
	static void instrOR(unsigned int instruction);
	static void instrXOR(unsigned int instruction);
	static void instrSHR(unsigned int instruction);
	static void instrSHL(unsigned int instruction);
		   
	static void instrPUSH(unsigned int instruction);
	static void instrPOP(unsigned int instruction);
	static void instrCMP(unsigned int instruction);

	static void instrSETI(unsigned int instruction);
	static void instrSETIL(unsigned int instruction);
	static void instrSTR(unsigned int instruction);
	static void instrLD(unsigned int instruction);
	static void instrSET(unsigned int instruction);

public:
	void executeNextInstruction();
	bool getHadInvalidOpCode() const;
};

extern Core core;

#endif