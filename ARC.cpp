#include "ARC.hpp"
#include <string.h>
#include "zpipe.h"

std::string arc::cur_file = std::string();

arc::arc(const char* path)
{
    file.open(path, std::ios::in | std::ios::binary);
    if(file.is_open())
    {
        file.read(reinterpret_cast<char*>(&header), sizeof(header));
        if(std::equal(ARC_MAGIC, ARC_MAGIC + 4, header.Magic))
        {
            printf("%s is arc file!\n", path);
            versionFix();

            verboseARCHeader(header);
            formatFilesList(header);
        }
    }
    else printf("cant open file\n");
}
arc::~arc(void)
{
    file.close();
}

void arc::versionFix(void)
{
    int buffer[4];
    file.seekg(std::ios::beg);
    file.read(reinterpret_cast<char*>(buffer), sizeof(buffer));

    if(buffer[2] == 0) FIX = 4; // true for version >= 17 i think
}

void arc::fixPath(arc_file_s& f)
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

void arc::formatFilesList(arc_s &header)
{
    file.seekg(std::ios::beg + FIX + sizeof(header));
    for(u32 i = 0; i < header.FilesNum; i++)
    {
        arc_file_s f;
        file.read(reinterpret_cast<char*>(&f), sizeof(f));
        //printf("File[%02d]: %s\n", count, f.Filename);

        fixPath(f);

        verboseFileInfo(f);

        filesList.push_back(f);
    }

    for(u32 i = 0; i < header.FilesNum; i++)
        decompress(i);
}

bool arc::decompress(u32 id)
{
    file.seekg(std::ios::beg + filesList.at(id).pDataPosition);

    u32 sz = filesList.at(id).CompressedSize;

    //char temp[sz];

    v_in.resize(sz, 0);
    v_out.clear();

    file.read(v_in.data(), sz);

    inflate_mem(v_in, v_out);

    return writeFile(id);
}

bool arc::writeFile(u32 id)
{

    std::string path    = cur_file + '/' + filesList.at(id).Filename;
    std::string name    = path.substr(path.find_last_of("/") + 1);;
    std::string ext     = extractExt();

    path.resize(path.find_last_of("/"));
    std::filesystem::create_directories(path);

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

    ext[0] = v_out.at(0);
    ext[1] = v_out.at(1);
    ext[2] = v_out.at(2);
    ext[3] = v_out.at(3);
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

    return out;
}

bool arc::decompressAll(void)
{
    return false;
}

void arc::verboseARCHeader(arc_s& header)
{
    printf("Magic:          %s\n", header.Magic);
    printf("Version:        %d\n", header.Version);
    printf("Num of Files:   %d\n", header.FilesNum);
}

void arc::verboseFileInfo(arc_file_s &f)
{
    float dec = __bswap_constant_32(f.DecompressedSize);

    printf("\n");
    printf("Filepath:           %s\n", f.Filename);
    printf("Ext hash:           0x%X\n", f.ExtentionHash);
    printf("Compressed size:    0x%X\n", f.CompressedSize);
    printf("Decompressed size:  0x%X | Float: %f.f\n", f.DecompressedSize, dec);
    printf("Data position:      0x%X\n", f.pDataPosition);
}
