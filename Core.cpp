#include "Core.h"
#include "Registers.h"
#include "Pins.h"
#include <Arduino.h>
#include "InstructionMasks.h"
#include "debug.h"
#include "Memory.h"

Core core;

void Core::executeNextInstruction() {
	unsigned int instruction = getNextWord();

	switch (static_cast<OpCode>(instruction & OPCODE_MASK)) {

		case OpCode::JEQ:
			instrJEQ(instruction);
			break;
		case OpCode::JNE:
			instrJNE(instruction);
			break;
		case OpCode::JLT:
			instrJLT(instruction);
			break;
		case OpCode::JGT:
			instrJGT(instruction);
			break;
		case OpCode::JC:
			instrJC(instruction);
			break;
		case OpCode::JNC:
			instrJNC(instruction);
			break;
		case OpCode::JMP:
			instrJMP(instruction);
			break;


		case OpCode::ADD:
			instrADD(instruction);
			break;
		case OpCode::SUB:
			instrSUB(instruction);
			break;
		case OpCode::NOT:
			instrNOT(instruction);
			break;
		case OpCode::AND:
			instrAND(instruction);
			break;
		case OpCode::OR:
			instrOR(instruction);
			break;
		case OpCode::XOR:
			instrXOR(instruction);
			break;
		case OpCode::SHR:
			instrSHR(instruction);
			break;
		case OpCode::SHL:
			instrSHL(instruction);
			break;


		case OpCode::PUSH:
			instrPUSH(instruction);
			break;
		case OpCode::POP:
			instrPOP(instruction);
			break;
		case OpCode::CMP:
			instrCMP(instruction);
			break;


		case OpCode::SETI:
			instrSETI(instruction);
			break;
		case OpCode::SETIL:
			instrSETIL(instruction);
			break;
		case OpCode::STR:
			instrSTR(instruction);
			break;
		case OpCode::LD:
			instrLD(instruction);
			break;
		case OpCode::SET:
			instrSET(instruction);
			break;


		default:
			hadInvalidOpCode(instruction);
			break;
	}
}

void Core::hadInvalidOpCode(unsigned int opcode) {
	m_hadInvalidOpCode = true;

	if (pins.isDebugEnabled()) {
		Serial.print("CPU Crash! Invalid OpCode: ");
		printWord(opcode, true);

		registers.dumpRegisters();
		ram.dumpMemory();
	}
}

unsigned int Core::getNextWord() {
	unsigned int word = ram.getMemory(registers.getPC(), false);
	registers.stepPC();

	return word;
}



bool Core::getHadInvalidOpCode() const {
	return m_hadInvalidOpCode;
}

bool Core::isNegative(unsigned int val) {
	// Abuse twos-compliment.
	return val & 0x8000;
}

unsigned int Core::reverse(unsigned int b) {
	return (reverse(b) << 8) | reverse(b >> 8);
}
unsigned char Core::reverse(unsigned char b) {
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}


bool Core::statusTest(Condition condition) {
	unsigned char status = registers.getStatus();

	switch (condition) {
		case Condition::NotEqual:
			return !bitRead(status, STATUS_ZERO);
		case Condition::Equal:
			return bitRead(status, STATUS_ZERO);
		case Condition::OverflowClear:
			return !bitRead(status, STATUS_OVERFLOW);
		case Condition::Overflow:
			return bitRead(status, STATUS_OVERFLOW);
		case Condition::Plus:
			return !bitRead(status, STATUS_NEG);
		case Condition::Minus:
			return bitRead(status, STATUS_NEG);
		case Condition::LessThan:
			return (bitRead(status, STATUS_NEG) && !bitRead(status, STATUS_OVERFLOW))
				|| (!bitRead(status, STATUS_NEG) && bitRead(status, STATUS_OVERFLOW));
		case Condition::GreaterThan:
			return (bitRead(status, STATUS_NEG) && bitRead(status, STATUS_OVERFLOW) && !bitRead(status, STATUS_ZERO))
				|| (!bitRead(status, STATUS_NEG) && !bitRead(status, STATUS_OVERFLOW) && !bitRead(status, STATUS_ZERO));
		default:
			return false;
	}
}

void Core::setStatus(Status over, Status neg, Status zero) {
	unsigned char status = registers.getStatus();

	if (over == Status::Clear)
		bitClear(status, STATUS_OVERFLOW);
	else if (over == Status::Set)
		bitSet(status, STATUS_OVERFLOW);

	if (neg == Status::Clear)
		bitClear(status, STATUS_NEG);
	else if (neg == Status::Set)
		bitSet(status, STATUS_NEG);

	if (zero == Status::Clear)
		bitClear(status, STATUS_ZERO);
	else if (zero == Status::Set)
		bitSet(status, STATUS_ZERO);

	registers.setStatus(status);
}


void Core::instrJEQ(unsigned int instruction) {
	instrJCCHelper(Condition::Equal);
}

void Core::instrJNE(unsigned int instruction) {
	instrJCCHelper(Condition::NotEqual);
}

void Core::instrJLT(unsigned int instruction) {
	instrJCCHelper(Condition::LessThan);
}

void Core::instrJGT(unsigned int instruction) {
	instrJCCHelper(Condition::GreaterThan);
}

void Core::instrJC(unsigned int instruction) {
	instrJCCHelper(Condition::Overflow);
}

void Core::instrJNC(unsigned int instruction) {
	instrJCCHelper(Condition::OverflowClear);
}

void Core::instrJMP(unsigned int instruction) {
	registers.setPC(ram.getMemory(registers.getPC(), false));
}


void Core::instrJCCHelper(Condition condition) {
	unsigned int addr = getNextWord();

	if (statusTest(condition))
		registers.setPC(addr);
}




void Core::instrADD(unsigned int instruction) {
	unsigned int a = registers.getRegister(getInstrRegA);
	unsigned int b = registers.getRegister(getInstrRegB);

	unsigned int res = a + b;

	registers.setRegister(getInstrRegA, res);

	setStatus(res < a ? Status::Set : Status::Clear,
			  isNegative(res) ? Status::Set : Status::Clear,
			  res == 0 ? Status::Set : Status::Clear);
}

void Core::instrSUB(unsigned int instruction) {
	unsigned int a = registers.getRegister(getInstrRegA);
	unsigned int b = registers.getRegister(getInstrRegB);

	unsigned int res = a - b;

	registers.setRegister(getInstrRegA, res);

	setStatus(res > a ? Status::Set : Status::Clear,
			  isNegative(res) ? Status::Set : Status::Clear,
			  res == 0 ? Status::Set : Status::Clear);
}

void Core::instrNOT(unsigned int instruction) {
	unsigned int a = registers.getRegister(getInstrRegA);
	unsigned int res = ~a;

	registers.setRegister(a, res);

	setStatus(res < a ? Status::Set : Status::Clear,
			  isNegative(res) ? Status::Set : Status::Clear,
			  res == 0 ? Status::Set : Status::Clear);
}

void Core::instrAND(unsigned int instruction) {
	unsigned int a = registers.getRegister(getInstrRegA);
	unsigned int b = registers.getRegister(getInstrRegB);

	unsigned int res = a & b;

	registers.setRegister(getInstrRegA, res);

	setStatus(res < a ? Status::Set : Status::Clear,
			  isNegative(res) ? Status::Set : Status::Clear,
			  res == 0 ? Status::Set : Status::Clear);
}

void Core::instrOR(unsigned int instruction) {
	unsigned int a = registers.getRegister(getInstrRegA);
	unsigned int b = registers.getRegister(getInstrRegB);

	unsigned int res = a | b;

	registers.setRegister(getInstrRegA, res);

	setStatus(res < a ? Status::Set : Status::Clear,
			  isNegative(res) ? Status::Set : Status::Clear,
			  res == 0 ? Status::Set : Status::Clear);
}

void Core::instrXOR(unsigned int instruction) {
	unsigned int a = registers.getRegister(getInstrRegA);
	unsigned int b = registers.getRegister(getInstrRegB);

	unsigned int res = a ^ b;

	registers.setRegister(getInstrRegA, res);

	setStatus(res < a ? Status::Set : Status::Clear,
			  isNegative(res) ? Status::Set : Status::Clear,
			  res == 0 ? Status::Set : Status::Clear);
}

void Core::instrSHR(unsigned int instruction) {
	unsigned int a = registers.getRegister(getInstrRegA);
	unsigned int res = a >> 1;
	
	registers.setRegister(a, res);
	
	setStatus(res < a ? Status::Set : Status::Clear,
			  isNegative(res) ? Status::Set : Status::Clear,
			  res == 0 ? Status::Set : Status::Clear);	
}

void Core::instrSHL(unsigned int instruction) {
	unsigned int a = registers.getRegister(getInstrRegA);

	unsigned long res = static_cast<unsigned long>(a) << 1;

	registers.setRegister(getInstrRegA, res);

	setStatus(res < a ? Status::Set : Status::Clear,
			  isNegative(res) ? Status::Set : Status::Clear,
			  res == 0 ? Status::Set : Status::Clear);
}



void Core::instrPUSH(unsigned int instruction) {
	unsigned int a = registers.getRegister(getInstrRegA);
	registers.decSP();

	ram.setMemory(registers.getSP(), a, true);
}

void Core::instrPOP(unsigned int instruction) {
	unsigned int val = ram.getMemory(registers.getSP(), true);
	registers.incSP();

	registers.setRegister(getInstrRegA, val);
}

void Core::instrCMP(unsigned int instruction) {
	unsigned int a = registers.getRegister(getInstrRegA);
	unsigned int b = registers.getRegister(getInstrRegB);

	unsigned int res = a - b;

	setStatus(res > a ? Status::Set : Status::Clear,
			  isNegative(res) ? Status::Set : Status::Clear,
			  res == 0 ? Status::Set : Status::Clear);
}



void Core::instrSETI(unsigned int instruction) {
	unsigned char imm = instruction & 0xFF;
	registers.setRegister(getInstrRegA, imm);
}

void Core::instrSETIL(unsigned int instruction) {
	unsigned int imm = getNextWord();
	registers.setRegister(getInstrRegA, imm);
}

void Core::instrSTR(unsigned int instruction) {
	unsigned int a = registers.getRegister(getInstrRegA);
	unsigned int b = registers.getRegister(getInstrRegB);

	ram.setMemory(b, a, true);
}

void Core::instrLD(unsigned int instruction) {
	unsigned int b = registers.getRegister(getInstrRegB);
	registers.setRegister(getInstrRegA, ram.getMemory(b, true));
}

void Core::instrSET(unsigned int instruction) {
	unsigned int b = registers.getRegister(getInstrRegB);
	registers.setRegister(getInstrRegA, b);
}

