#include "ARC.hpp"
#include <string.h>
#include <algorithm>
#include <thread>

#include "utils.h"
#include "zpipe.h"

std::string arc::cur_file = std::string();
arc::Flags arc::flags;

arc::arc(const char* path)
{
    file.open(path, std::ios::in | std::ios::binary);
    if(file.is_open())
    {
        file.read(reinterpret_cast<char*>(&header), sizeof(header));
        findEndianess();

        if(flags.isArc)
        {
            if(flags.isBEndian) fixHeaderBE();
            if(flags.isVerbose)
            {
                printf("%s is arc file!\n", path);
                verboseARCHeader(header);
            }

            fixVersion();
            formatFilesList(header);
        }
    }
    else printf("cant open file\n");
}
arc::~arc(void)
{
    file.close();
}

void arc::findEndianess(void)
{
    flags.isLEndian = std::equal(ARC_MAGIC, ARC_MAGIC + 4, header.Magic);
    flags.isBEndian = std::equal(CRA_MAGIC, CRA_MAGIC + 4, header.Magic);

    if(flags.isLEndian || flags.isBEndian) flags.isArc = true;
}

void arc::formatFilesList(arc_s &header)
{
    u32 count = 1;

    file.seekg(std::ios::beg + FIX + sizeof(header));
    for(u32 i = 0; i < header.FilesNum; i++)
    {
        arc_file_s f;
        file.read(reinterpret_cast<char*>(&f), sizeof(f));

        fixArcPath(f);
        if(flags.isBEndian) fixFileHeaderBE(f);
        else fixFileHeader(f);

        if(flags.isVerbose)
            verboseFileInfo(f, count++);

        filesList.push_back(f);
    }

    if(flags.isExtract) extractAll();
}

bool arc::extract(u32 id)
{
    file.seekg(std::ios::beg + filesList.at(id).pZData);

    u32 sz = filesList.at(id).CompressedSize;

    v_in.resize(sz, 0);
    v_out.clear();

    file.read(v_in.data(), sz);

    inflate_mem(v_in, v_out);

    return writeFile(id);
}

bool arc::writeFile(u32 id)
{

    std::string path    = cur_file + fs::path::preferred_separator + filesList.at(id).Filename;
    std::string name    = path.substr(path.find_last_of('/') + 1);;
    std::string ext     = extractExt();

    path.resize(path.find_last_of("/"));
    fs::create_directories(path);

    std::fstream fout;
    fout.open(path + '/' + name + '.' + ext, std::ios::out | std::ios::binary);

    if(fout.is_open())
    {
        fout.write(v_out.data(), v_out.size());
        fout.close();

        return true;
    }

    return false;
}

std::string arc::extractExt(void)
{
    std::string out;
    char ext[5];
    std::copy(v_out.begin(), v_out.begin() + 4, ext);
    ext[4] = '\0';


    for(u32 i = 0; i < 4; i++)
    {
        if(isalpha(ext[i]))
            out.push_back(ext[i]);
        else
        {
            if(ext[i] > 0)
                out.append(std::to_string(ext[i]));
        }

    }

    if(flags.isBEndian) std::reverse(out.begin(), out.end());
    return out;
}

bool arc::extractAll(void)
{       
//    std::vector<std::thread*> threads;

//    for(u32 i = 0; i < header.FilesNum; i++)
//    {
//        std::thread t(&arc::extract, i);
//        threads.push_back(&t);
//        t.detach();
//    }

//    for(auto& t : threads)
//        t->join();
// maybe later

    for(u32 i = 0; i < header.FilesNum; i++)
        extract(i);

    return false;
}

void arc::verboseARCHeader(arc_s& header)
{
    printf("Magic:          %s\n", header.Magic);
    printf("Version:        %d\n", header.Version);
    printf("Num of Files:   %d\n", header.FilesNum);
}

void arc::verboseFileInfo(arc_file_s &f, u32 count)
{
    printf("\n");
    printf("File #%d\n", count);
    printf("Filepath:           %s\n", f.Filename);
    printf("Resource hash:      0x%X\n", f.ResourceHash);
    printf("Compressed size:    0x%X\n", f.CompressedSize);
    printf("Decompressed size:  0x%X\n", f.DecompressedSize);
    printf("Pointer to zdata:   0x%X\n", f.pZData);
}


////////////////////////////
/// Fixes

void arc::fixVersion(void)
{
    int buffer[4];
    file.seekg(std::ios::beg);
    file.read(reinterpret_cast<char*>(buffer), sizeof(buffer));

    if(buffer[2] == 0) FIX = 4; // true for version >= 17 i think
}

void arc::fixArcPath(arc_file_s& f)
{
    const char in   = '\\';
    const char out  = '/';
    char* pos = f.Filename;

    while(true)
    {
        pos = strchr(pos, in);

        if(pos == nullptr) break;
        *pos = out;
        pos++;
    }
}

void arc::fixFileHeader(arc_file_s& f)
{
    f.DecompressedSize &= ~(1 << 30); // 0x40000000
}

void arc::fixFileHeaderBE(arc_file_s& f)
{
    Utils::bswap32(f.ResourceHash);
    Utils::bswap32(f.CompressedSize);
    Utils::bswap32(f.DecompressedSize);
    Utils::bswap32(f.pZData);
}

void arc::fixHeaderBE(void)
{
    Utils::bswap16(header.Version);
    Utils::bswap16(header.FilesNum);
}
