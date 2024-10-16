#include "SDLWrapper.hpp"
#include "Chip8.hpp"
#include "utils.hpp"
#include <iostream>
#include <thread>

int main(int argc, char* argv[]) {
    auto args = utils::parseArguments(argc, argv);
    if (args.find(utils::CONSTANTS::HELP_KEY) != args.end()) {
        utils::printHelp(argv);
        return 0;
    }

    Chip8 chip8;
    if (args.find(utils::CONSTANTS::FILE_PATH_KEY) != args.end()) {
        chip8.loadFile(args[utils::CONSTANTS::FILE_PATH_KEY].c_str());
    } else {
        chip8.loadFile(utils::CONSTANTS::DEFAULT_FILE_PATH);
    }

     if (args.find(utils::CONSTANTS::FPS_KEY) != args.end()) {
        chip8.setFPS(std::stoi(args[utils::CONSTANTS::FPS_KEY]));
     } else {
        chip8.setFPS(utils::CONSTANTS::DEFAULT_FPS);
     }

     if (args.find(utils::CONSTANTS::CLOCK_SPEED_KEY) != args.end()) {
        chip8.setProcessorClockSpeed(std::stoi(args[utils::CONSTANTS::CLOCK_SPEED_KEY]));
     } else {
        chip8.setProcessorClockSpeed(utils::CONSTANTS::DEFAULT_CLOCK_SPEED);
     }

    SDLWrapper sdlWrapper(argv[1], 64, 32, 20);
    Uint32 frameStart;
    Uint32 frameTime;
    Uint32 frameDelay = 1000 / chip8.getFPS();

    while(sdlWrapper.checkRunning()) {
        frameStart = SDL_GetTicks();
        sdlWrapper.handleEvents();
        sdlWrapper.setKeyState(chip8.keyboard);
        if (chip8.getDrawFlag()) {
            sdlWrapper.render(chip8.getDisplay());
            chip8.resetDrawFlag();
        }
        if (chip8.shouldBeep()) {
            SDL_PauseAudioDevice(sdlWrapper.deviceId, 0); 
        } else {
            SDL_PauseAudioDevice(sdlWrapper.deviceId, 1);
        }
        chip8.executeFrame(); 
        
        frameTime = SDL_GetTicks() - frameStart;
        
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    return 0;
}
 