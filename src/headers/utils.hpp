#ifndef UTILS_HPP
#define UTILS_HPP
#include <map>
#include <string>
#include <cstdint>

class utils {
    public:
       static  std::map<std::string, std::string> parseArguments(int argc, char* argv[]) {
        std::map<std::string, std::string> args;

        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            if (arg.substr(0, 2) == "--") {
                std::string key;
                std::string value;
                auto pos = arg.find('=');
                if (pos != std::string::npos) {
                    key = arg.substr(2, pos - 2);
                    value = arg.substr(pos + 1);
                } else {
                    key = arg.substr(2);
                    if (i + 1 < argc && argv[i + 1][0] != '-') {
                        value = argv[++i];
                    } else {
                        value = "";
                    }
                }

                args[key] = value;
            }
        }
        return args;
    }

    struct CONSTANTS {
        static inline constexpr const char* FILE_PATH_KEY = "file_path";
        static inline constexpr const char* CLOCK_SPEED_KEY  = "clock_speed";
        static inline constexpr const char* FPS_KEY = "fps";
        static inline constexpr const char* HELP_KEY = "help";
        
        static inline constexpr const char* DEFAULT_FILE_PATH = "../ROMS/BRIX.ch8";
        static const uint16_t DEFAULT_CLOCK_SPEED = 700;
        static const uint8_t DEFAULT_FPS = 60;
    };

    static void printHelp(char* argv[]) {
        std::cout << "Usage:" << std::endl << argv[0] << " --OPTIONAL FLAG=value" << std::endl << "Optional Flags:" << std::endl
                <<  "--" << CONSTANTS::FILE_PATH_KEY << "=path/to/rom" << std::endl
                <<  "--" << CONSTANTS::CLOCK_SPEED_KEY << "=clock speed // recomended to keep it below 1500" << std::endl
                << "--" << CONSTANTS::FPS_KEY << "=FPS // max 255" << std::endl
                << "Example : " << argv[0] << " --" << CONSTANTS::FILE_PATH_KEY << "=" <<  CONSTANTS::DEFAULT_FILE_PATH 
                << " --" << CONSTANTS::FPS_KEY << "=" <<  (int) CONSTANTS::DEFAULT_FPS
                << " --" << CONSTANTS::CLOCK_SPEED_KEY << "=" << (int) CONSTANTS::DEFAULT_CLOCK_SPEED << std::endl
                << "Controls: 1 2 3 4 q w e r a s d f z x c v";
    }
    
};
#endif