#pragma once
#include <fstream>
#include <vector>

int inf(FILE *source, FILE *dest);
int inflate_mem(std::vector<char> &source, std::vector<char> &dest);
