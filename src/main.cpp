#include <string.h>
#include "ARC.hpp"
/*
 * argv[1] filename
*/

bool setUsageFlags(const char* arg);
static const char* usage =
"Usage: arctool [option] <filename>\n\n\
List of options:\n\
  -v verbose\n\
  -n no action, include verbose\n\
  -e extract\n\n\
Example:\n\
  arctool -xv file.arc\n\
";

int main (int argc, char *argv[])
{
    if(argc < 2) {
        printf("Use -h for usage hints\n");
        return -1;
    }

    if(!setUsageFlags(argv[1])) return 0;


    arc::cur_file = argv[2];
    arc::cur_file = arc::cur_file.substr(0, arc::cur_file.find_last_of('.')) + "_dearc";

    fs::path p = fs::current_path();
    p.append(argv[2]);

    arc a(p.string().c_str());

    return 0;
}

bool setUsageFlags(const char* arg)
{
    if(strcmp(arg, "-h") == 0)
    {
        printf("%s", usage);
        return false;
    }

    if(strcmp(arg, "-v") == 0) arc::flags.isVerbose = true;
    if(strcmp(arg, "-n") == 0) {
        arc::flags.isVerbose = true;
        arc::flags.isNoAct = true;
    }
    if(strcmp(arg, "-e") == 0) arc::flags.isExtract = true;
    if(strcmp(arg, "-ev") || strcmp(arg, "-ve")){
        arc::flags.isExtract = true;
        arc::flags.isVerbose = true;
    }

    return true;
}
