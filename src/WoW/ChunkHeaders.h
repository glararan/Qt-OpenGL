#ifndef CHUNKHEADERS_H
#define CHUNKHEADERS_H

#include <QtGlobal>

#define MAP_SIZE 64
#define CHUNKS_HORIZONTAL 16
#define CHUNKS_VERTICAL   16

/// Global
struct MverHeader
{
    quint32 version;
};

/// ADT
struct MhdrHeader
{
    quint32 flags;

    quint32 mcinOffset;
    quint32 mtexOffset;
    quint32 mmdxOffset;
    quint32 mmidOffset;
    quint32 mwmoOffset;
    quint32 mwidOffset;
    quint32 mddfOffset;
    quint32 modfOffset;
    quint32 mfboOffset;
    quint32 mh2oOffset;
    quint32 mtfxOffset;

    quint8 mamp_value;
    quint8 padding[3];

    quint32 unused1;
    quint32 unused2;
    quint32 unused3;
};

struct McinHeader
{
    struct Entry
    {
        quint32 offset;
        quint32 size;
        quint32 flags;
        quint32 asyncID;
    } entries[CHUNKS_HORIZONTAL * CHUNKS_VERTICAL];
};

struct MtexHeader
{
    char fileNames;
};

struct MmdxHeader
{
    quint32 fileNames;
};

struct MmidHeader
{
    quint32 offsets;
};

struct MwmoHeader // used also by WDT
{
    char fileNames;
};

struct MwidHeader // used also by WDL
{
    quint32 offsets;
};

struct MddfHeader
{
    quint32 mmidID;
    quint32 uniqueID;

    float position[3];
    float rotation[3];

    quint16 scale;
    quint16 flags;
};

struct ModfHeader // used also by WDT
{
    quint32 mwidID;
    quint32 uniqueID;

    float position[3];
    float rotation[3];
    float extents[6];

    quint16 flags;
    quint16 doodadSetIndex;
    quint16 nameSet;
    quint16 pad;
};

struct Mh2oHeader
{
    struct Liquid
    {
        quint32 instancesOffset;
        quint32 layerCount;
        quint32 attributesOffset;
    } liquid[CHUNKS_HORIZONTAL * CHUNKS_VERTICAL];

    /*struct Attributes
    {
        quint64 fishable;
        quint64 deep;
    } attributes;*/

    struct Instances
    {
        quint16 liquidType;
        quint16 liquidObject;

        float minHeightLevel;
        float maxHeightLevel;

        quint8 xOffset;
        quint8 yOffset;
        quint8 width;
        quint8 height;

        quint32 bitmapExistsOffset;
        quint32 vertexDataOffset;
    };

    struct HeightMask
    {
        float height[9][9];

        quint8 transparency[9][9];
    };

    struct Render
    {
        quint8 mask[8];
        quint8 fatigue[8]; // not sure about fatigue
    };
};

struct MfboHeader
{
    struct Plane
    {
        quint16 height[3][3];
    } minimum, maximum;
};

struct MtfxHeader
{
    quint32 unk1 : 1;
    quint32 unk2 : 3;
    quint32 unk3 : 28;
};

struct McnkHeader
{
    quint32 flags;
    quint32 indexX;
    quint32 indexY;
    quint32 layersCount;
    quint32 doodadRefsCount;
    quint32 mcvtOffset;
    quint32 mcnrOffset;
    quint32 mclyOffset;
    quint32 mcrfOffset;
    quint32 mcalOffset;
    quint32 mcalSize;
    quint32 mcshOffset;
    quint32 mcshSize;
    quint32 areaId;
    quint32 wmoRefsCount;
    quint32 holes;

    quint8 lowQualityTextureMap[CHUNKS_HORIZONTAL];

    quint32 predTex;
    quint32 effectDoodadCount;
    quint32 mcseOffset;
    quint32 nSndEmitters;
    quint32 mclqOffset;
    quint32 mclqSize;

    float posX;
    float posY;
    float posZ;

    quint32 mccvOffset;
    quint32 mclvOffset;
    quint32 unused;
};

// MCNK
struct McvtHeader
{
    float height[9 * 9 + 8 * 8];
};

struct MccvHeader
{
    struct Entry
    {
        quint8 blue;
        quint8 green;
        quint8 red;
        quint8 alpha;
    } entries[9 * 9 + 8 * 8];
};

struct McnrHeader
{
    struct Entry
    {
        qint8 normal[3];
    } entries[9 * 9 + 8 * 8];

    quint8 unk[3 * 3 + 2 * 2];
};

struct MclyHeader
{
    quint32 textureID;

    struct
    {
        quint32 animationRotation : 3;    // each tick is 45°
        quint32 animationSpeed : 3;
        quint32 animationEnabled : 1;
        quint32 overbright : 1;           // This will make the texture way brighter. Used for lava to make it "glow".
        quint32 useAlphaMap : 1;          // set for every layer after the first
        quint32 alphaMapCompressed : 1;   // see MCAL chunk description
        quint32 useCubeMapReflection : 1; // This makes the layer behave like its a reflection of the skybox. See below
        quint32 unk: 19;
    } flags;

    quint32 mcalOffset;
    quint32 effectID;
};

struct MclqHeader
{
    qint16 minimum;
    qint16 maximum;

    float height;
};

/// WDL
struct MaofHeader
{
    quint32 areaLowOffsets[MAP_SIZE][MAP_SIZE];
};

struct MareHeader
{
    quint16 outer[17 * 17];
    quint16 inner[16 * 16];
};

struct MahoHeader
{
    quint16 values[16];
};

/// WDT
struct MphdHeader
{
    quint32 flags;
    quint32 unk1;
    quint32 unk2[6];

    MphdHeader()
    : flags(0)
    , unk1(0)
    {
        for(qint8 i = 0; i < 6; ++i)
            unk2[i] = 0;
    }
};

struct MainHeader
{
    struct Data
    {
        quint32 exist;
        quint32 data1;

        Data()
        : exist(0)
        , data1(0)
        {
        }
    } ADTs[MAP_SIZE][MAP_SIZE];

    MainHeader()
    {
        for(qint8 x = 0; x < MAP_SIZE; ++x)
        {
            for(qint8 y = 0; y < MAP_SIZE; ++y)
                ADTs[x][y] = Data();
        }
    }
};

#endif // CHUNKHEADERS_H
