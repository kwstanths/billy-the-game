#include "Levels.hpp"


namespace debug_tools{

    std::pair<std::string, Color> GetLevelInfo(Level level){
        switch (level) {
            case Level::INFO:
                {
                    return std::make_pair<std::string, Color>("Info", Color::DARK_GREEN);
                }
            case Level::WARNING:
                {
                    return std::make_pair<std::string, Color>("Warning", Color::DARK_YELLOW);
                }
            case Level::CRITICAL:
                {
                    return std::make_pair<std::string, Color>("Critical", Color::YELLOW);
                }
            case Level::FATAL:
                {
#ifdef _WIN32
                    return std::make_pair<std::string, Color>("Fatal", Color::RED);
#endif // _WIN32

                    return std::make_pair<std::string, Color>("Fatal", Color::DARK_RED);
                }
        }

        return std::make_pair<std::string, Color>("Unknown error", Color::RED);
    }
}
