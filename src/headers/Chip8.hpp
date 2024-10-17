#ifndef CHIP8_HPP
#define CHIP8_HPP
#include <iostream>
#include <chrono>

class Chip8 {
public:
    Chip8 ();
    ~Chip8();
    void loadFile(const char* filePath);
    void executeOneCycle();
    void executeFrame();
    bool keyboard[16];

    bool shouldBeep() const;
    const bool (&getDisplay() const)[64][32];

    bool getDrawFlag();

    uint16_t getFPS() const;
    void setFPS(const uint16_t fps);

    uint16_t getProcessorClockSpeed() const;
    void setProcessorClockSpeed(const uint16_t clockSpeed);



private:
    uint16_t pc;
    uint16_t opcode;
    uint8_t memory[4096];
    uint8_t dataRegisters[16];
    uint16_t addressRegister;
    uint16_t memoryStack[48];
    uint8_t stackPointer;
    uint16_t delayTimer;
    uint16_t soundTimer;
    uint8_t timerPrecision;
    uint16_t processorClockSpeed;
    uint16_t fps;
    uint8_t timerFrequency;
    bool drawFlag;
    bool display[64][32];

    
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