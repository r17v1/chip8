#include "SDLWrapper.hpp"
#include <iostream>


SDLWrapper::SDLWrapper(const char* title, const int screenWidth, const int screenHeight, const int screenMultiplier)
    : window(nullptr), renderer(nullptr), isRunning(false), SCREEN_WIDTH(screenWidth), 
    SCREEN_HEIGHT(screenHeight),SCREEN_MULTIPLIER(screenMultiplier), AUDIO_AMPLITUDE(3000), AUDIO_FREQUENCY(450) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    } else {
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH*SCREEN_MULTIPLIER, SCREEN_HEIGHT*SCREEN_MULTIPLIER, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        } else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == nullptr) {
                std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            } else {
                isRunning = true;
            }
        }
    }
    
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
    }

    desiredSpec.freq = 44100;
    desiredSpec.format = AUDIO_S16SYS;
    desiredSpec.channels = 1;
    desiredSpec.samples = 512;
    desiredSpec.callback = SDLWrapper::audio_callback;
    desiredSpec.userdata = this;
    deviceId =  SDL_OpenAudioDevice(NULL, 0, &desiredSpec, &obtainedSpec, 0);

    if (deviceId == 0) {
        std::cerr << "Failed to open audio: " << SDL_GetError() << std::endl;
    }
}


SDLWrapper::~SDLWrapper() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_CloseAudioDevice(deviceId);;
    SDL_Quit();
}

void SDLWrapper::audio_callback(void* userdata, Uint8* stream, int len) {
    SDLWrapper* wrapper = static_cast<SDLWrapper*>(userdata);

    static int phase = 0;
    int16_t* buffer = (int16_t*)stream;
    int length = len / 2; // 2 bytes per sample for AUDIO_S16SYS


    for (int i = 0; i < length; ++i) {
        buffer[i] = (phase < wrapper-> AUDIO_FREQUENCY / 2) ? (wrapper -> AUDIO_AMPLITUDE) : - (wrapper -> AUDIO_AMPLITUDE);
        phase = (phase + 1) % (wrapper-> AUDIO_FREQUENCY);
    }
}

const bool SDLWrapper::checkRunning() {
    return isRunning;
}

void SDLWrapper::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
    }
}

void SDLWrapper::render(const bool display[64][32]) {
    clear();
    for (int i=0; i<32; i++) {
        for (int j=0; j<64; j++) {
            if (display[j][i]) {
                SDL_Rect rect = { j*SCREEN_MULTIPLIER, i*SCREEN_MULTIPLIER, SCREEN_MULTIPLIER, SCREEN_MULTIPLIER };
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void SDLWrapper::clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void SDLWrapper::setKeyState(bool* chip8Keyboard) {
    const Uint8 *state = SDL_GetKeyboardState(0);
    uint8_t i = 0;
    chip8Keyboard[i++] = state[SDL_SCANCODE_X];
    chip8Keyboard[i++] = state[SDL_SCANCODE_1];
    chip8Keyboard[i++] = state[SDL_SCANCODE_2];
    chip8Keyboard[i++] = state[SDL_SCANCODE_3];
    chip8Keyboard[i++] = state[SDL_SCANCODE_Q];
    chip8Keyboard[i++] = state[SDL_SCANCODE_W];
    chip8Keyboard[i++] = state[SDL_SCANCODE_E];
    chip8Keyboard[i++] = state[SDL_SCANCODE_A];
    chip8Keyboard[i++] = state[SDL_SCANCODE_S];
    chip8Keyboard[i++] = state[SDL_SCANCODE_D];
    chip8Keyboard[i++] = state[SDL_SCANCODE_Z];
    chip8Keyboard[i++] = state[SDL_SCANCODE_C];
    chip8Keyboard[i++] = state[SDL_SCANCODE_4];
    chip8Keyboard[i++] = state[SDL_SCANCODE_R];
    chip8Keyboard[i++] = state[SDL_SCANCODE_F];
    chip8Keyboard[i++] = state[SDL_SCANCODE_V];
}