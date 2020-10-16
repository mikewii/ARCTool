#pragma once
#include "types.h"

struct mod_s {
    char    Magic[4];
    u16     Version;
    u16     MeshCount;
    u16     Unk1;
    u16     Unk2;
    u32     Unk3[13];
};
