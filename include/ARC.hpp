#pragma once
#include "types.h"
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <typeinfo>

static const char ARC_MAGIC[5] = "ARC\0";
static const char CRA_MAGIC[5] = "\0CRA";

namespace fs = std::filesystem;

class arc
{
private:
    struct arc_file_s {
        char    Filename[64];
        u32     ResourceHash;
        u32 	CompressedSize;
        u32     DecompressedSize; // xor 20000000 + 40000000 if version is 17
        u32     pZData; // 78 9C - zlib header
    };

    struct arc_s {
        char        Magic[4];
        u16         Version;
        u16         FilesNum;
    };

    std::vector<arc_file_s> filesList;
    std::vector<char> v_in;
    std::vector<char> v_out;

    std::fstream file;
    arc_s header;
    u32 FIX = 0;
    u32 err_count = 0;

public:
    arc(const char* path);
    ~arc();

    void findEndianess(void);

    void formatFilesList(arc_s& header);

    void verboseARCHeader(arc_s& header);
    void verboseFileInfo(arc_file_s& f, u32 count);

    void fixVersion(void);
    void fixArcPath(arc_file_s& f);
    void fixFileHeader(arc_file_s& f);
    void fixFileHeaderBE(arc_file_s& f);
    void fixHeaderBE(void);

    bool extract(u32 id);
    bool extractAll(void);

    std::string extractExt(void);

    bool writeFile(u32 id);

    union Flags{
        u32 bitfield{0};
        struct{
            bool isArc : 1;
            bool isBEndian : 1;
            bool isLEndian : 1;

            bool isVerbose : 1;
            bool isNoAct : 1;
            bool isExtract : 1;
        };
    };

    static Flags flags;
    static std::string cur_file;
};
