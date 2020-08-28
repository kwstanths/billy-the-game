#ifndef __BasicFunctions_hpp__
#define __BasicFunctions_hpp__

#include <vector>
#include <string>

namespace game_engine {
namespace utility {

    /** Split a string using a delimeter 
        @param str Input string
        @param delimeter
        @return A vector of strings 
    */
    std::vector<std::string> split(const std::string & str, const std::string & delim);

}
}

#endif