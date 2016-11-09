#ifndef MEMORY_H
#define MEMORY_H

#define mem_max 511

class Memory {
private:
	unsigned int m_memory[mem_max + 1];
	long m_LastMemoryAddress;

public:
	Memory();

	void ZeroMemory();

	static unsigned int GetMemMax();

	void SetMemory(unsigned int address, unsigned int value, bool updateLastAddress);
	unsigned int GetMemory(unsigned int address, bool updateLastAddress);

	unsigned int GetLastMemoryAddress() const;

	void DumpMemory();
};

extern Memory ram;

#endif
