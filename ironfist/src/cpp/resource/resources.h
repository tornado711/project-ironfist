
#ifndef RESOURCE
#define RESOURCE
#include "../input.h"
#include<winsock.h>


enum RESOURCE_TYPE : short
{
  RESOURCE_TYPE_BITMAP = 0x0,
  RESOURCE_TYPE_ICN = 0x1,
  RESOURCE_TYPE_PALETTE = 0x2,
  RESOURCE_TYPE_TILESET = 0x3,
  RESOURCE_TYPE_FONT = 0x5,
  RESOURCE_TYPE_SAMPLE = 0x6,
};

struct AbstractResource;

#pragma pack(push,1)
struct ResourceVtable
{
  int (__fastcall *cleanUp)(AbstractResource *, char);
};
#pragma pack(pop)

#pragma pack(push,1)
struct AbstractResource
{
  ResourceVtable *vtable;
  RESOURCE_TYPE resourceType;
  __int16 referenceCount;
  unsigned int fileID;
  AbstractResource *next;
};
#pragma pack(pop)

#pragma pack(push,1)
struct Bitmap
{
  ResourceVtable *vtable;
  RESOURCE_TYPE resourceType;
  __int16 referenceCount;
  int fileID;
  int next;
  __int16 setInConstructor;
  __int16 width;
  __int16 height;
  unsigned __int8 *contents;
};
#pragma pack(pop)

#pragma pack(push,1)
struct ICNHeader
{
  __int16 offsetX;
  __int16 offsetY;
  __int16 width;
  __int16 height;
  char someSortOfLength;
  int off;
};
#pragma pack(pop)

#pragma pack(push,1)
class Icon
{
public:

  ResourceVtable *vtable;
  RESOURCE_TYPE resourceType;
  __int16 referenceCount;
  int fileID;
  AbstractResource *next;
  __int16 numSprites;
  ICNHeader *headersAndImageData;
  int drawToScreen( int  x, int  y, int  spriteIdx, int  mirror);
};
#pragma pack(pop)


#pragma pack(push,1)
struct Sample
{
  ResourceVtable *vtable;
  __int16 resourceType;
  __int16 referenceCount;
  int fileID;
  AbstractResource *next;
  int field_10;
  void *contents;
  int nbytes;
  int codeThing;
  unsigned int playbackRate;
  unsigned int sampleType;
  unsigned int field_28;
  int loopCount;
};
#pragma pack(pop)

#pragma pack(push,1)
struct Sequence
{
  ResourceVtable *vtable;
  __int16 resourceType;
  __int16 referenceCount;
  int fileID;
  AbstractResource *next;
  void *contents;
};
#pragma pack(pop)

#pragma pack(push,1)
struct Tileset
{
  ResourceVtable *vtable;
  RESOURCE_TYPE resourceType;
  __int16 referenceCount;
  int fileID;
  AbstractResource *next;
  __int16 numTiles;
  __int16 field_12;
  __int16 field_14;
  void *contents;
};
#pragma pack(pop)

#pragma pack(push,1)
struct RGB
{
  char red;
  char green;
  char blue;
};
#pragma pack(pop)

#pragma pack(push,1)
struct PaletteContents
{
  RGB colors[256];
};
#pragma pack(pop)

#pragma pack(push,1)
struct Palette
{
  ResourceVtable *vtable;
  __int16 resourceType;
  __int16 referenceCount;
  int fileID;
  int next;
  PaletteContents *contents;
};
#pragma pack(pop)

#pragma pack(push,1)
struct Font
{
  ResourceVtable *vtable;
  __int16 resourceType;
  __int16 referenceCount;
  int fileID;
  int next;
  int lineHeight;
  int whetherFirstShortIsMoreThan13;
  int field_18;
  Icon *icon;
};
#pragma pack(pop)

struct AbstractManager;

#pragma pack(push,1)
struct ManagerVtable
{
  int (__fastcall *initialize)(AbstractManager *, int, int);
  void (__fastcall *cleanUp)(AbstractManager *, int);
  int (__fastcall *handleInput)(AbstractManager *, int, InputEvent *);
};
#pragma pack(pop)

#pragma pack(push,1)
struct AbstractManager
{
  ManagerVtable *vtable;
  AbstractManager *next;
  AbstractManager *prev;
  int type;
  int idx;
  char name[30];
  int ready;
};
#pragma pack(pop)

#pragma pack(push,1)
enum MANAGER_TYPE
{
  MANAGER_TYPE_HERO_WINDOW_MANAGER = 0x20,
  MANAGER_TYPE_RESOURCE_MAMANGER = 0x80,
  MANAGER_TYPE_SWAP_MANAGER = 0x100,
  MANAGER_TYPE_COMBAT_MANAGER = 0x200,
};
#pragma pack(pop)

#pragma pack(push,1)
enum MANAGER_RETURN_CODE
{
  MANAGER_SUCCESS = 0,
  MANAGER_FINISHED = 2,
  MANAGER_FAILED = 3,
};
#pragma pack(pop)

#pragma pack(push,1)
struct FileInAGGInfo
{
  int id;
  int off;
  int length;
};
#pragma pack(pop)


#pragma pack(push,1)
enum RESOURCEMGR_CONSTANTS
{
  NUM_AGG_FILES = 3,
  NUM_GOOD_EVIL_ICONS = 37,
};
#pragma pack(pop)


#pragma pack(push,1)
class ResourceManager
{
public:

  ManagerVtable *vtable;
  AbstractManager *next;
  AbstractManager *prev;
  MANAGER_TYPE type;
  int idx;
  char name[30];
  int ready;
  AbstractResource *loadedFileLinkedList;
  int numOpenAGGFiles;
  int curHandleIdx;
  int fileDescriptors[NUM_AGG_FILES];
  FileInAGGInfo *aggContentInfo[NUM_AGG_FILES];
  int numberOfFilesInAGG[NUM_AGG_FILES];
  int isFreeingAllResources;
  int field_5E;
  char resourceToLoad[60];
  int fileID;
  MANAGER_RETURN_CODE addAGG( const char * filename);
  void dumpImageToScreen( const char * filename, Bitmap * screenBuf, int  isICN);
  AbstractResource * findLoadedFile( unsigned int  fileHash);
  void freeAllResources();
  Bitmap * getBitmapByFilename( const char * filename);
  int getFileSize( int  fileHash);
  Font * getFontByFilename( const char * filename);
  Icon * getIconByFileID( int  fileID);
  Icon * getIconByFilename( const char * filename);
  Palette * getPal( const char * file);
  Sample * getSampleByFilename( char * filename);
  Sequence * getSequenceByFilename( char * filename);
  Tileset * getTilesetByFilename( char * filename);
  MANAGER_RETURN_CODE handleInput( InputEvent * a1);
  MANAGER_RETURN_CODE initialize( int  idx);
  signed int pointToFile( int  id);
  void popFileAndOffsetOffStack();
  void prependFileLinkedListNode( AbstractResource * res);
  void pushCurrentFileAndOffsetOntoStack();
  void readFromCurrentFile( void * buf, DWORD  ntoread);
  int readInt();
  void readNexDOSFilename( char * buf);
  __int16 readShort();
  void reduceReferenceCountToResource( AbstractResource * res);
  void removeFileFromList( AbstractResource * fil);
  unsigned int setResource( const char * filename, int  useEvilersion);
};
#pragma pack(pop)

ResourceManager *ResourceManager_constructor(ResourceManager*);

#endif