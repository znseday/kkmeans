#pragma once

#include <iostream>
#include <fstream>

#include <iterator>
#include <algorithm>
#include <memory>

#include <iostream>

//#include <dlib/clustering.h>

#if ((defined WIN32) || (defined WIN64))
#include "C:/dlib-19.19/dlib-19.19/dlib/clustering.h"
#else
    #include <dlib/clustering.h>
    #include <dlib/rand.h>
#endif


// __FUNCSIG__ is for VS, but Qt (mingw) works with __PRETTY_FUNCTION__
#if ((defined WIN32) || (defined WIN64)) && (defined _MSC_VER)
#define MY_P_FUNC __FUNCSIG__
#else
#define MY_P_FUNC __PRETTY_FUNCTION__
#endif

extern bool IsDebugOutput;

#define MY_DEBUG_ONLY(x) { if(IsDebugOutput) {x}  }

//#pragma pack(1)
#pragma pack(push, 1)
struct bmpHeaderType
{
    // header file
    unsigned short Type;   // signature = BM 0x4D42
    uint32_t  Size;        // file size = 3*H*W+54
    uint32_t  Reserved;    // 0
    uint32_t  OffsetBits;  // 54
    // header info
    uint32_t  SizeH;          // header info size = 40
    int32_t   Width;
    int32_t   Height;
    uint16_t  Planes;         // 1
    uint16_t  BitCount;       // 24
    uint32_t  Compression;    // 0
    uint32_t  SizeImage;      // image size = 3*W*H
    int32_t   XPelsPerMeter;  // 0
    int32_t   YPelsPerMeter;  // 0
    uint32_t  ColorUsed;      // 0
    uint32_t  ColorImportant; // 0
};
#pragma pack(pop)

class Task
{
private:
    std::vector<std::tuple<double,double>> data;

public:
    Task() = default;

    void GenDemoData(const std::string &fn);
    void GenRealData(const std::string &fn);

    void LoadDataFromInputStream();

    void TrainAndDo(int nClusters, double g, double tolerance, unsigned long d_size);
};

void ClusterToRGB(int iCluster, int nClusters, unsigned char &r, unsigned char &g, unsigned char &b);



