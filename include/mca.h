#pragma once
#include "types.h"

static const char MCA_MAGIC[5] = "MADP";

// https://github.com/losnoco/vgmstream/blob/2125433cd5fc362e679373cce7777110f7b5fba0/src/meta/mca.c
// https://github.com/jackoalan/audacity/blob/gc-dspadpcm/src/export/ExportDSPADPCM.cpp
class mca
{
private:
    struct mca_s {
        char    Magic[4];
        u32     Version;
        u16     ChannelCount; // u8 ?
        u16     InterleaveBlockSize; // what?
        u32     SamplesNum;
        u32     SampleRate; // pointer\size
        u32     SampleLoopStart;
        u32     SampleLoopEnd;
        u32     HeaderSize;
        u32     DataSize;

    };

    // all data in this structure is in BIG ENDIAN FORMAT!!!!
    struct DSPADPCM_s {
        // for header generation during decode
        u32 num_samples; // total number of RAW samples
        u32 num_adpcm_nibbles; // number of ADPCM nibbles (including frame headers)
        u32 sample_rate; // Sample rate, in Hz

        // DSP addressing and decode context
        u16 loop_flag; // 1=LOOPED, 0=NOT LOOPED
        u16 format; // Always 0x0000, for ADPCM
        u32 sa; // Start offset address for looped samples (zero for non-looped)
        u32 ea; // End offset address for looped samples
        u32 ca; // always zero
        u16 coef[16]; // decode coefficients (eight pairs of 16-bit words)

        // DSP decoder initial state
        u16 gain; // always zero for ADPCM
        u16 ps; // predictor/scale
        u16 yn1; // sample history
        u16 yn2; // sample history

        // DSP decoder loop context
        u16 lps; // predictor/scale for loop context
        u16 lyn1; // sample history (n-1) for loop context
        u16 lyn2; // sample history (n-2) for loop context
        u16 pad[11]; // reserved
    }; // Header is 96 bytes long
};
