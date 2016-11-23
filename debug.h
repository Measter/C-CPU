#ifndef DEBUG_H
#define DEBUG_H

void printByte(unsigned char value, bool newLine);
void printWord(unsigned int value, bool newLine);
void printByteBin(unsigned char value, bool newLine);

bool updateDebugInput();
unsigned int getDebugInput();

#endif
