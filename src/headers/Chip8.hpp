#ifndef CHIP8_HPP
#define CHIP8_HPP
#include <iostream>
#include <chrono>

class Chip8 {
public:
    Chip8 ();
    ~Chip8();
    const bool (&getDisplay() const)[64][32];
    bool shouldBeep();
    void loadFile(const char* filePath);
    void executeOneCycle();
    void executeFrame();
    bool keyboard[16];
    bool drawFlag;

private:
    uint16_t pc;
    uint16_t opcode;
    uint8_t memory[4096];
    uint8_t dataRegisters[16];
    uint16_t addressRegister;
    uint16_t memoryStack[48];
    uint8_t stackPointer;
    uint8_t delayTimer;
    uint8_t soundTimer;
    bool display[64][32];
    int processorClockSpeed;
    
    void readOpcode();
    void clearDisplay();
    void throwOpcodeNotRecognisedError(const uint16_t opcode);
    uint8_t getRandomNumber();
    void draw(uint8_t Vx, uint8_t Vy, uint8_t n);
    void storeKey(uint8_t x);
    void registerDump(uint8_t x);
    void registerLoad(uint8_t x);
    void updateTimers();
};
#endif