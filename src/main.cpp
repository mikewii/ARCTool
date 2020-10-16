#include <string>
#include "ARC.hpp"
/*
 * argv[1] filename
*/

int main (int argc, char *argv[])
{
    if(argc < 2) {
        printf("Lack of arguments\nUse -h for usage hint\n");
        return -1;
    }

    arc::cur_file = argv[1];
    arc::cur_file =  arc::cur_file.substr(0, arc::cur_file.find_last_of('.')) + "_dearc";

    std::filesystem::path p = std::filesystem::current_path();
    p.append(argv[1]);

    arc a(p.string().c_str());

    return 0;
}
