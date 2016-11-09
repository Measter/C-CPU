#ifndef DEBUG_H
#define DEBUG_H

void PrintByte(unsigned char value, bool newLine);
void PrintWord(unsigned int value, bool newLine);
void PrintByteBin(unsigned char value, bool newLine);

bool UpdateDebugInput();
unsigned int GetDebugInput();

#endif
