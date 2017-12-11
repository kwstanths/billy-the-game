#ifndef __CodeReminder_hpp__
#define __CodeReminder_hpp__


#include "Console.hpp"


#define CodeReminder(text) debug_tools::Console(debug_tools::CRITICAL, std::string(#text) + ":" + std::string(__FILE__) + ":" + std::to_string(__LINE__));

#define CodeReminderFatal(text) \
    { \
        debug_tools::Console(debug_tools::FATAL, std::string(#text) + ":" + std::string(__FILE__) + ":" + std::to_string(__LINE__)); \
        debug_tools::WaitInput(); \
        exit(-1); \
    }


#endif