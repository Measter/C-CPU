#include "Core.h"
#include "Registers.h"
#include "Pins.h"
#include <Arduino.h>
#include "InstructionMasks.h"
#include "debug.h"
#include "Memory.h"

Core core;

void Core::ExecuteNextInstruction() {
	unsigned int instruction = GetNextWord();

	switch (static_cast<OpCode>(instruction & OPCODE_MASK)) {

		case OpCode::JEQ:
			InstrJEQ(instruction);
			break;
		case OpCode::JNE:
			InstrJNE(instruction);
			break;
		case OpCode::JLT:
			InstrJLT(instruction);
			break;
		case OpCode::JGT:
			InstrJGT(instruction);
			break;
		case OpCode::JC:
			InstrJC(instruction);
			break;
		case OpCode::JNC:
			InstrJNC(instruction);
			break;
		case OpCode::JMP:
			InstrJMP(instruction);
			break;


		case OpCode::ADD:
			InstrADD(instruction);
			break;
		case OpCode::SUB:
			InstrSUB(instruction);
			break;
		case OpCode::NOT:
			InstrNOT(instruction);
			break;
		case OpCode::AND:
			InstrAND(instruction);
			break;
		case OpCode::OR:
			InstrOR(instruction);
			break;
		case OpCode::XOR:
			InstrXOR(instruction);
			break;
		case OpCode::SHR:
			InstrSHR(instruction);
			break;
		case OpCode::SHL:
			InstrSHL(instruction);
			break;


		case OpCode::PUSH:
			InstrPUSH(instruction);
			break;
		case OpCode::POP:
			InstrPOP(instruction);
			break;
		case OpCode::CMP:
			InstrCMP(instruction);
			break;


		case OpCode::SETI:
			InstrSETI(instruction);
			break;
		case OpCode::SETIL:
			InstrSETIL(instruction);
			break;
		case OpCode::STR:
			InstrSTR(instruction);
			break;
		case OpCode::LD:
			InstrLD(instruction);
			break;
		case OpCode::SET:
			InstrSET(instruction);
			break;


		default:
			HadInvalidOpCode(instruction);
			break;
	}
}

void Core::HadInvalidOpCode(unsigned int opcode) {
	m_hadInvalidOpCode = true;

	if (pins.IsDebugEnabled()) {
		Serial.print("CPU Crash! Invalid OpCode: ");
		PrintWord(opcode, true);

		registers.DumpRegisters();
		ram.DumpMemory();
	}
}

unsigned int Core::GetNextWord() {
	unsigned int word = ram.GetMemory(registers.GetPC(), false);
	registers.StepPC();

	return word;
}



bool Core::GetHadInvalidOpCode() const {
	return m_hadInvalidOpCode;
}

bool Core::IsNegative(unsigned int val) {
	// Abuse twos-compliment.
	return val & 0x8000;
}

unsigned int Core::Reverse(unsigned int b) {
	return (Reverse(b) << 8) | Reverse(b >> 8);
}
unsigned char Core::Reverse(unsigned char b) {
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}


bool Core::StatusTest(Condition condition) {
	unsigned char status = registers.GetStatus();

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

void Core::SetStatus(Status over, Status neg, Status zero) {
	unsigned char status = registers.GetStatus();

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

	registers.SetStatus(status);
}


void Core::InstrJEQ(unsigned int instruction) {
	InstrJCCHelper(Condition::Equal);
}

void Core::InstrJNE(unsigned int instruction) {
	InstrJCCHelper(Condition::NotEqual);
}

void Core::InstrJLT(unsigned int instruction) {
	InstrJCCHelper(Condition::LessThan);
}

void Core::InstrJGT(unsigned int instruction) {
	InstrJCCHelper(Condition::GreaterThan);
}

void Core::InstrJC(unsigned int instruction) {
	InstrJCCHelper(Condition::Overflow);
}

void Core::InstrJNC(unsigned int instruction) {
	InstrJCCHelper(Condition::OverflowClear);
}

void Core::InstrJMP(unsigned int instruction) {
	registers.SetPC(ram.GetMemory(registers.GetPC(), false));
}


void Core::InstrJCCHelper(Condition condition) {
	unsigned int addr = GetNextWord();

	if (StatusTest(condition))
		registers.SetPC(addr);
}




void Core::InstrADD(unsigned int instruction) {
	unsigned int a = registers.GetRegister(GetInstrRegA);
	unsigned int b = registers.GetRegister(GetInstrRegB);

	unsigned int res = a + b;

	registers.SetRegister(GetInstrRegA, res);

	SetStatus(res < a ? Status::Set : Status::Clear,
			  IsNegative(res) ? Status::Set : Status::Clear,
			  res == 0 ? Status::Set : Status::Clear);
}

void Core::InstrSUB(unsigned int instruction) {
	unsigned int a = registers.GetRegister(GetInstrRegA);
	unsigned int b = registers.GetRegister(GetInstrRegB);

	unsigned int res = a - b;

	registers.SetRegister(GetInstrRegA, res);

	SetStatus(res > a ? Status::Set : Status::Clear,
			  IsNegative(res) ? Status::Set : Status::Clear,
			  res == 0 ? Status::Set : Status::Clear);
}

void Core::InstrNOT(unsigned int instruction) {
	unsigned int a = registers.GetRegister(GetInstrRegA);
	unsigned int res = ~a;

	registers.SetRegister(a, res);

	SetStatus(res < a ? Status::Set : Status::Clear,
			  IsNegative(res) ? Status::Set : Status::Clear,
			  res == 0 ? Status::Set : Status::Clear);
}

void Core::InstrAND(unsigned int instruction) {
	unsigned int a = registers.GetRegister(GetInstrRegA);
	unsigned int b = registers.GetRegister(GetInstrRegB);

	unsigned int res = a & b;

	registers.SetRegister(GetInstrRegA, res);

	SetStatus(res < a ? Status::Set : Status::Clear,
			  IsNegative(res) ? Status::Set : Status::Clear,
			  res == 0 ? Status::Set : Status::Clear);
}

void Core::InstrOR(unsigned int instruction) {
	unsigned int a = registers.GetRegister(GetInstrRegA);
	unsigned int b = registers.GetRegister(GetInstrRegB);

	unsigned int res = a | b;

	registers.SetRegister(GetInstrRegA, res);

	SetStatus(res < a ? Status::Set : Status::Clear,
			  IsNegative(res) ? Status::Set : Status::Clear,
			  res == 0 ? Status::Set : Status::Clear);
}

void Core::InstrXOR(unsigned int instruction) {
	unsigned int a = registers.GetRegister(GetInstrRegA);
	unsigned int b = registers.GetRegister(GetInstrRegB);

	unsigned int res = a ^ b;

	registers.SetRegister(GetInstrRegA, res);

	SetStatus(res < a ? Status::Set : Status::Clear,
			  IsNegative(res) ? Status::Set : Status::Clear,
			  res == 0 ? Status::Set : Status::Clear);
}

void Core::InstrSHR(unsigned int instruction) {
	unsigned int a = registers.GetRegister(GetInstrRegA);
	unsigned int res = a >> 1;
	
	registers.SetRegister(a, res);
	
	SetStatus(res < a ? Status::Set : Status::Clear,
			  IsNegative(res) ? Status::Set : Status::Clear,
			  res == 0 ? Status::Set : Status::Clear);	
}

void Core::InstrSHL(unsigned int instruction) {
	unsigned int a = registers.GetRegister(GetInstrRegA);

	unsigned long res = static_cast<unsigned long>(a) << 1;

	registers.SetRegister(GetInstrRegA, res);

	SetStatus(res < a ? Status::Set : Status::Clear,
			  IsNegative(res) ? Status::Set : Status::Clear,
			  res == 0 ? Status::Set : Status::Clear);
}



void Core::InstrPUSH(unsigned int instruction) {
	unsigned int a = registers.GetRegister(GetInstrRegA);
	registers.DecSP();

	ram.SetMemory(registers.GetSP(), a, true);
}

void Core::InstrPOP(unsigned int instruction) {
	unsigned int val = ram.GetMemory(registers.GetSP(), true);
	registers.IncSP();

	registers.SetRegister(GetInstrRegA, val);
}

void Core::InstrCMP(unsigned int instruction) {
	unsigned int a = registers.GetRegister(GetInstrRegA);
	unsigned int b = registers.GetRegister(GetInstrRegB);

	unsigned int res = a - b;

	SetStatus(res > a ? Status::Set : Status::Clear,
			  IsNegative(res) ? Status::Set : Status::Clear,
			  res == 0 ? Status::Set : Status::Clear);
}



void Core::InstrSETI(unsigned int instruction) {
	unsigned char imm = instruction & 0xFF;
	registers.SetRegister(GetInstrRegA, imm);
}

void Core::InstrSETIL(unsigned int instruction) {
	unsigned int imm = GetNextWord();
	registers.SetRegister(GetInstrRegA, imm);
}

void Core::InstrSTR(unsigned int instruction) {
	unsigned int a = registers.GetRegister(GetInstrRegA);
	unsigned int b = registers.GetRegister(GetInstrRegB);

	ram.SetMemory(b, a, true);
}

void Core::InstrLD(unsigned int instruction) {
	unsigned int b = registers.GetRegister(GetInstrRegB);
	registers.SetRegister(GetInstrRegA, ram.GetMemory(b, true));
}

void Core::InstrSET(unsigned int instruction) {
	unsigned int b = registers.GetRegister(GetInstrRegB);
	registers.SetRegister(GetInstrRegA, b);
}

