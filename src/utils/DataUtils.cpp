#include "DataUtils.h"

namespace DataUtils
{
    std::string trim(const std::string &str)
    {
        size_t start = str.find_first_not_of(" \t");
        size_t end = str.find_last_not_of(" \t");

        if (start == std::string::npos)
        {
            return ""; // String is all whitespace
        }

        return str.substr(start, end - start + 1);
    }
};
