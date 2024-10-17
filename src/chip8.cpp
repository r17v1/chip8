#include "Chip8.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <random>

Chip8::Chip8() : pc(0x200), opcode(0), memory{}, dataRegisters{}, addressRegister(0), memoryStack{},
stackPointer(0), delayTimer(0), soundTimer(0), display{}, keyboard{}, drawFlag(false), 
processorClockSpeed(700), timerPrecision(1000), fps(60), timerFrequency(60) {}

Chip8::~Chip8() {}

/**
 * Chip 8 had a 64x32 black and white display. We are storing the pixel data in a 64x32 array of booleans.
 * True will represent a white pixel, false will represent a black pixel. 
 *  
 * @returns the emulated display
 */
const bool (&Chip8::getDisplay() const)[64][32] {
    return display;
}

/**
 * Chip8 produced beep sound when the sound timer was non zero.
 * 
 * @returns true if a beep sound should be produced, false otherwise. 
 */
bool Chip8::shouldBeep() const {
    return soundTimer != 0;
}

/**
 * Whenever the display is updated, we will set the draw flag to true.
 * This allows us to then update the screen so that we dont update the renderer if there is no change.
 * It is assumed that the dislay has been updated when a true is returned, so the flag is set to false untill the next update. 
 * 
 * @return true if renderer should be updated false otherwise. 
 */
bool Chip8::getDrawFlag() {
    if (drawFlag) {
        drawFlag = false;
        return true;
    }
    return false;
};

uint16_t Chip8::getFPS() const {
    return fps;
}

void Chip8::setFPS(const uint16_t fps) {
    this -> fps  = fps;
};

uint16_t Chip8::getProcessorClockSpeed() const {
    return processorClockSpeed;
};

void Chip8::setProcessorClockSpeed(const uint16_t clockSpeed) {
    this -> processorClockSpeed = clockSpeed;
};

/**
 * Loads the file present in the provided file path.
 * 
 * @param - the provided file path
 */

void Chip8::loadFile(const char* filePath) {
    // The first section of the memory stores some font sprites (to render numbers and letters)
    const unsigned char fontSprites[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // Copy font sprites into memory starting at 0x050
    for (uint8_t i = 0; i < 80; ++i) {
        memory[0x050 + i] = fontSprites[i];
    }

    // Load the file into memory starting at 0x200. This is by convension.
    // Chip8 needed it because it stored the interpreter here, but for us it will be empty space (other than the font sprites at the start).
    std::uintmax_t fileSize = std::filesystem::file_size(filePath);
    if (fileSize > (4096 - 0x200)) {
        throw std::runtime_error("File size exceeds available memory.");
    }
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file!" << std::endl;
        throw std::runtime_error("Could not read file. Use --help to see instructions.");
    }

    file.read(reinterpret_cast<char*>(memory + 0x200), fileSize);

    if (!file) {
        throw std::runtime_error("Could not load file to memory");
    }
    file.close();
}

/**
 * The pc (program counter) points to the memory where the opcode is.
 * This function loads the opcode. Each memory element is 1 byte, however the opcode is 2 bytes. 
 * It is stored in big endian. Consider the opcode 1NNN.
 * We load the first byte, which is 001N
 * We shift it by 1 byte(8 bits), 001N << 8 = 1N00
 * Then we do a bitwise or with the second byte, which is 00NN
 *  1N00 | 00NN = 1NNN
 */
void Chip8::readOpcode() {
    // Opcodes are 2 bytes. Each memory block is 1 byte.
    opcode = memory[pc] << 8 | memory[pc+1];
    pc += 2;
}

/**
 * Sets every value of the display array to false (off)
 */
void Chip8::clearDisplay() {
    std::fill(&display[0][0], &display[0][0] + 64 * 32, false);
}

void Chip8::throwOpcodeNotRecognisedError(const uint16_t opcode) {
    std::ostringstream oss;
    oss << "Opcode not recognized: 0x" << std::hex << std::setw(4) << std::setfill('0') << opcode;
    throw std::runtime_error(oss.str());
}

/**
 * Generates a random number between 0 and 255
 */
uint8_t Chip8::getRandomNumber() {
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<> distribution(0, 255);
    return distribution(generator);
}


/**
 * Update emulated display signals
 * 
 * TODO: Add more documentation on how it works
 */
void Chip8::draw(uint8_t Vx, uint8_t Vy, uint8_t n) {
    drawFlag = true;
    dataRegisters[0xF] = 0;
    for (uint8_t i = 0; i < n; i++) {
        uint8_t row = memory[addressRegister + i];
        for (int8_t  j = 7; j >= 0; j--) {
            uint8_t x = (Vx + 7 - j) % 64;
            uint8_t y = (Vy + i) % 32;
            if (row & (1 << j)) {
                dataRegisters[0xF] |= display[x][y] & 1;
                display[x][y] ^= 1;
            }
        }
    }
}

/**
 * Read the first x data registers and store them in memory starting from addressed pointed by address register i
 */
void Chip8::registerDump(uint8_t x) {
    for (uint8_t i=0; i <= x; i++) {
        memory[addressRegister + i] = dataRegisters[i];
    }
}

/** 
 * Read x bytes from the memory starting from address pointed by address register i, and store them in first x data registers. 
 */
void Chip8::registerLoad(uint8_t x) {
    for (uint8_t i=0; i <= x; i++) {
        dataRegisters[i] = memory[addressRegister + i];
    }
}

/**
 * Store the pressed key into the data register x
 * 
 * @param uint8_t x - the index of the data register where the pressed key will be stored
 */
void Chip8::storeKey(uint8_t x) {
    bool keyPressed = false;
    for (uint8_t i =0; i < 16; i++) {
        if (keyboard[i]) {
            dataRegisters[x] = i;
            keyPressed = true;
            break;
        }
    }

    if (!keyPressed) {
        pc -= 2;
    }
    return;
}

/**
 * Chip8 timers ticked at a frequency of 60 hz.This function is called once every frame. 
 * Since FPS is frames per second, frequency/fps is the amount we want to tick. However it might be less than 1.
 * timerPrecision is used as a multiplier so that we can update the timer bit by bit instead of reducing by 1 every 1/60 second,
 * this makes the tick greater than 1. Doubles/floats could also be used instead. 
 */
void Chip8::updateTimers() {
    uint16_t ticks = (timerPrecision * timerFrequency) / fps;
    if (delayTimer > 0) {
       delayTimer = delayTimer >= ticks ? delayTimer - ticks : 0;
    }   
   
    if (soundTimer > 0) {
       soundTimer = soundTimer >= ticks ? soundTimer - ticks : 0;
    }
}

/**
 * Execute 1 frame. If there are fps frames in 1 second, and processor clock speed is processorClockSpeed
 * processorClockSpeed/fps cycles needs to be executed.
 * Update the timers after the executon.
 */
void Chip8::executeFrame() {
    for(uint32_t i = 0; i < processorClockSpeed/fps ; i++) {
        executeOneCycle();
    }
    updateTimers();
}

/**
 * Execute 1 processor cycle. 
 * 
 * Todo: add documentation on opcodes. 
 */
void Chip8::executeOneCycle() {
    readOpcode();
    
    switch (opcode >> 12) {
        case 0x0:
            switch (opcode & 0x0FFF) {
                case 0x0E0:
                    clearDisplay();
                    break;
                case 0x0EE:
                    if (stackPointer == 0) {
                        throw std::runtime_error("Stack is empty, cannot return!");
                    }
                    --stackPointer;
                    pc = memoryStack[stackPointer];
                    break;
                default:
                    throwOpcodeNotRecognisedError(opcode);
                    break;
            }
            break;
        case 0x1: 
            pc = opcode & 0x0FFF;
            break;
        case 0x2: 
            memoryStack[stackPointer] = pc;
            ++stackPointer;
            pc = opcode & 0x0FFF;
            break;
        case 0x3: 
            if (dataRegisters[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
                pc += 2; // skips next instruction.
                         // each instructions are 2 bytes. 
            }
            break;
        case 0x4:
            if (dataRegisters[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
                pc += 2; // skips next instruction.
                         // each instructions are 2 bytes. 
            }
            break;
        case 0x5: 
            switch (opcode & 0x000F) {
                case 0x0 :
                    if (dataRegisters[(opcode & 0x0F00) >> 8] == dataRegisters[(opcode & 0x00F0) >> 4]) {
                        pc += 2;
                    }
                    break;
                default:
                    throwOpcodeNotRecognisedError(opcode);
                    break;
            }
            break;
        case 0x6: 
            dataRegisters[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            break;
        case 0x7:
            dataRegisters[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            break;
        case 0x8: {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            switch (opcode & 0x000F) {
                case 0x0:
                    dataRegisters[x] = dataRegisters[y];
                    break;
                case 0x1:
                    dataRegisters[x] |= dataRegisters[y];
                    break;
                case 0x2:
                    dataRegisters[x] &= dataRegisters[y];
                    break;
                case 0x3:
                    dataRegisters[x] ^= dataRegisters[y];
                    break;
                case 0x4: {
                    uint16_t sum = dataRegisters[x] + dataRegisters[y];
                    dataRegisters[0xF] = (sum > 0xFF);
                    dataRegisters[x] = sum & 0xFF;
                    break;
                }
                case 0x5:
                    dataRegisters[0xF] = dataRegisters[x] >= dataRegisters[y];
                    dataRegisters[x] -= dataRegisters[y];
                    break;
                case 0x6:
                    dataRegisters[0xF] = dataRegisters[x] & 0x1;
                    dataRegisters[x] >>= 1;
                    break;
                case 0x7:
                    dataRegisters[0xF] = dataRegisters[y] >= dataRegisters[x];
                    dataRegisters[x] = dataRegisters[y] - dataRegisters[x];
                    break;
                case 0xE:
                    dataRegisters[0xF] = dataRegisters[x] >> 7;
                    dataRegisters[x] <<= 1;
                    break;
                default:
                    throwOpcodeNotRecognisedError(opcode);
                    break;
            }
        }
            break;
        case 0x9: 
            if (opcode & 0x000F != 0) {
                throwOpcodeNotRecognisedError(opcode);
            }
            if (dataRegisters[((opcode & 0x0F00) >> 8)] != dataRegisters[((opcode & 0x00F0) >> 4)]) {
                pc += 2;
            }
            break;
        case 0xA: 
            addressRegister = opcode & 0x0FFF;
            break;
        case 0xB: 
            pc = dataRegisters[0] + (opcode & 0x0FFF);
            break;
        case 0xC: 
            dataRegisters[(opcode & 0x0F00) >> 8] = getRandomNumber() & (opcode & 0x00FF);
            break;
        case 0xD: 
                draw(dataRegisters[(opcode & 0x0F00) >> 8], dataRegisters[(opcode & 0x00F0) >> 4], opcode & 0x000F);
            break;
        case 0xE: 
            switch (opcode & 0x00FF) {
                case 0x9E:
                    if (keyboard[dataRegisters[(opcode & 0x0F00) >> 8]]) {
                        pc += 2;
                    }
                    break;
                case 0xA1:
                    if (!keyboard[dataRegisters[(opcode & 0x0F00) >> 8]]) {
                        pc += 2;
                    }
                    break;
                default:
                    throwOpcodeNotRecognisedError(opcode);
                    break;
            }   
            break;
        case 0xF: 
            switch (opcode & 0x00FF) {
                case 0x07:
                    dataRegisters[(opcode & 0x0F00) >> 8] = delayTimer / timerPrecision;
                    break;
                case 0x0A:
                    storeKey((opcode & 0x0F00) >> 8);
                    break;
                case 0x15:
                    delayTimer = dataRegisters[(opcode & 0x0F00) >> 8] * timerPrecision;
                    break;
                case 0x18:
                    soundTimer = dataRegisters[(opcode & 0x0F00) >> 8] * timerPrecision;
                    break;
                case 0x1E:
                    addressRegister += dataRegisters[(opcode & 0x0F00) >> 8];
                    break;
                case 0x29:
                    addressRegister = 0x050 + dataRegisters[(opcode & 0x0F00) >> 8] * 5;
                    break;
                case 0x33: {
                    uint8_t vx = dataRegisters[(opcode & 0x0F00) >> 8];
                    memory[addressRegister] = vx / 100;
                    memory[addressRegister + 1] = (vx / 10) % 10; 
                    memory[addressRegister + 2] = vx % 10;    
                    break;
                }
                case 0x55:
                    registerDump((opcode & 0x0F00) >> 8);
                    break;
                case 0x65:
                    registerLoad((opcode & 0x0F00) >> 8);
                    break;
                default:
                    throwOpcodeNotRecognisedError(opcode);
                    break;
            }   
            break;
    }
    return;
}

