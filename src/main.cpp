#include "SDLWrapper.hpp"
#include "Chip8.hpp"
#include "utils.hpp"
#include <iostream>
#include <thread>

int main(int argc, char* argv[]) {
    auto args = utils::parseArguments(argc, argv);

    // Parse the arguments and store them in a map so that they can be handled more easily
    if (args.find(utils::CONSTANTS::HELP_KEY) != args.end()) {
        utils::printHelp(argv);
        return 0;
    }

    Chip8 chip8;

    // If file path is provided, load that file. Otherwise load the default file
    if (args.find(utils::CONSTANTS::FILE_PATH_KEY) != args.end()) {
        chip8.loadFile(args[utils::CONSTANTS::FILE_PATH_KEY].c_str());
    } else {
        chip8.loadFile(utils::CONSTANTS::DEFAULT_FILE_PATH);
    }
    
    // If FPS is provided, set that FPS. Otherwise set the default FPS
     if (args.find(utils::CONSTANTS::FPS_KEY) != args.end()) {
        chip8.setFPS(std::stoi(args[utils::CONSTANTS::FPS_KEY]));
     } else {
        chip8.setFPS(utils::CONSTANTS::DEFAULT_FPS);
     }

    // If clock speed is provided, set that clock speed. Otherwise set the default clock speed 
     if (args.find(utils::CONSTANTS::CLOCK_SPEED_KEY) != args.end()) {
        chip8.setProcessorClockSpeed(std::stoi(args[utils::CONSTANTS::CLOCK_SPEED_KEY]));
     } else {
        chip8.setProcessorClockSpeed(utils::CONSTANTS::DEFAULT_CLOCK_SPEED);
     }

    // Initi SDL so that we can render + play audio
    SDLWrapper sdlWrapper(
        argv[1], 
        64,   // Chip 8 display is 64x32
        32,  
        20,   // Since 64x32 is too small, we will multiply everything by 20
        3000, // Audio anmplitude
        450   // Audio frequency
    );
    
    // Needed for delay calculation so that FPS can be maintained
    Uint32 frameStart;
    Uint32 frameTime;

    // FPS is frames per second, but the delay expects milliseconds 
    Uint32 frameDelay = 1000 / chip8.getFPS();  

    while(sdlWrapper.checkRunning()) {
        // Needed for delay calculation to maintain the desired FPS
        frameStart = SDL_GetTicks();

        // Needed to handle trhe close event triggered when clicking the close button
        sdlWrapper.handleEvents();
        
        // Get keyboard input, and set the state of the emulated keboard
        sdlWrapper.setKeyState(chip8.keyboard);
        
        // Get the emulated display data and convert it to actual graphics
        // Only render again when the emulated display data has changed
        if (chip8.getDrawFlag()) {
            sdlWrapper.render(chip8.getDisplay());
        }
        
        // Play beep audio when the emulated audio signals it to be played
        sdlWrapper.playAudio(chip8.shouldBeep());

        // Executes one frame. It will execute processorClockSpeed/fps instructions
        chip8.executeFrame(); 
        
        // Delay so that the desired FPS can be maintained
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    return 0;
}
 