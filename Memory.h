#ifndef MEMORY_H
#define MEMORY_H

#define mem_max 511

class Memory {
private:
	unsigned int m_memory[mem_max + 1];
	long m_LastMemoryAddress;

public:
	Memory();

	void zeroMemory();

	static unsigned int getMemMax();

	void setMemory(unsigned int address, unsigned int value, bool updateLastAddress);
	unsigned int getMemory(unsigned int address, bool updateLastAddress);

	unsigned int getLastMemoryAddress() const;

	void dumpMemory();
};

extern Memory ram;

#endif
