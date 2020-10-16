#pragma once
#include "types.h"
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>

static const char ARC_MAGIC[5] = "ARC\0";

class arc
{
private:
    struct arc_file_s {
        char    Filename[64];
        u32     ExtentionHash;
        u32 	CompressedSize;
        u32     DecompressedSize; // xor 20000000 + 40000000 if version is 17
        u32     pDataPosition; // 78 9C - zlib header
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
    arc_s header{0};
    u32 FIX = 0;
    u32 err_count = 0;

public:
    static std::string cur_file;

    arc(const char* path);
    ~arc();

    void versionFix(void);

    void verboseARCHeader(arc_s& header);
    void formatFilesList(arc_s& header);
    void verboseFileInfo(arc_file_s& f);

    void fixPath(arc_file_s& f);

    bool decompress(u32 id);
    bool decompressAll(void);

    std::string extractExt(void);

    bool writeFile(u32 id);
};
