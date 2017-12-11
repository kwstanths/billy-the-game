#include "Console.hpp"

namespace debug_tools{

    void Console(Level level, std::string text, Color color){
        std::pair<std::string, Color> level_info = GetLevelInfo(level);
        if (level == Level::INFO) {
            std::cout << GetStringTimestamp() << " [";
            CustomPrint(std::cout, level_info.first, level_info.second);
            std::cout << "] : ";
            CustomPrint(std::cout, text, color);
            std::cout << std::endl;
        } else {
            std::cerr << GetStringTimestamp() << " [";
            CustomPrint(std::cerr, level_info.first, level_info.second);
            std::cerr << "] : ";
            CustomPrint(std::cout, text, color);
            std::cout << std::endl;
        }

    }

    void CustomPrint(std::ostream & os, std::string text, Color fg_color){

        os.precision(16);

        ChangeConsoleColor(fg_color);

        os << text;

        RestoreConsoleColor();

    } 

    void CustomPrintHex(std::ostream & os, int number, Color color){

        ChangeConsoleColor(color);

        os << std::hex << number << std::dec << std::endl;

        RestoreConsoleColor();
    } 

    void CustomPrintHex(std::ostream & os, double number, Color color){

        ChangeConsoleColor(color);

        os << std::hexfloat << number << std::defaultfloat << std::endl;

        RestoreConsoleColor();
    }

    void ChangeConsoleColor(Color fg_color){
#ifdef _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color);
#endif
#ifdef __linux__
        std::cout << "\033[0;" << fg_color << "m" << std::flush;        
#endif
    }

    void RestoreConsoleColor(){
#ifdef _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color::DEF_FG);
#endif
#ifdef __linux__
        std::cout << "\033[0;" << Color::DEF_FG << "m" << std::flush;
#endif 
    }

}

