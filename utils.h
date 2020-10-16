#pragma once
#include "types.h"
#include <fstream>

class Utils
{
public:
    static std::fstream* openFile(char* path);
};
