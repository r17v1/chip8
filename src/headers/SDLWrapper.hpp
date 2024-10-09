#ifndef SDLWRAPPER_HPP
#define SDLWRAPPER_HPP

#include <SDL2/SDL.h>

class SDLWrapper {
public:
    SDLWrapper(const char* title, const int screenWidth, const int screenHeight, const int screenMultiplier);
    const bool checkRunning();
    ~SDLWrapper();
    void handleEvents();
    void render(const bool display[64][32]);
    void clear();
    const bool (&getKeyState() const)[16];
    void setKeyState(bool* chip8Keyboard);
    SDL_AudioDeviceID deviceId;

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;   
    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;
    const int SCREEN_MULTIPLIER;
    const int AUDIO_AMPLITUDE;
    const int AUDIO_FREQUENCY;
    SDL_AudioSpec desiredSpec;
    SDL_AudioSpec obtainedSpec;
    static void audio_callback(void* userdata, Uint8* stream, int len);
};

#endif