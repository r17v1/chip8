#include "SDLWrapper.hpp"
#include "Chip8.hpp"
#include <iostream>
#include <thread>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    Chip8 chip8;
    chip8.loadFile(argv[1]);

    SDLWrapper sdlWrapper(argv[1], 64, 32, 20);

    while(sdlWrapper.checkRunning()) {
        frameStart = SDL_GetTicks();
        sdlWrapper.handleEvents();
        sdlWrapper.setKeyState(chip8.keyboard);
        if (chip8.drawFlag) {
            sdlWrapper.render(chip8.getDisplay());
            chip8.drawFlag = false;
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
