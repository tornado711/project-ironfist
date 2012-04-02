
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
  int fileDescriptors[2];
  FileInAGGInfo *aggContentInfo[2];
  int numberOfFilesInAGG[2];
  int isFreeingAllResources;
  int field_5E;
  char resourceToLoad[60];
  int fileID;
  signed int addAGG( const char * filename);
  void dumpImageToScreen( const char * filename, Bitmap * screenBuf, int  isICN);
  AbstractResource * findLoadedFile( int  fileID);
  void freeAllResources();
  Bitmap * getBitmapByFilename( const char * filename);
  int getFileSize( int  fileID);
  Font * getFontByFilename( const char * filename);
  Icon * getIconByFileID( int  fileID);
  Icon * getIconByFilename( const char * filename);
  Palette * getPal( const char * file);
  Sample * getSampleByFilename( const char * filename);
  Palette * getPal( const char * file);
  Sample * getSampleByFilename( const char * filename);
  Sequence * getSequenceByFilename( const char * filename);
  Tileset * getTilesetByFilename( const char * filename);
  int handleInput( InputEvent * a1);
  signed int initialize( int  idx);
  __int32 pointToFile( int  id);
  __int32 popFileAndOffsetOffStack();
  void prependFileLinkedListNode( AbstractResource * res);
  int pushCurrentFileAndOffsetOntoStack();
  void readFromCurrentFile( void * buf, unsigned int  nbytes);
  int readInt();
  void readNexDOSFilename( void * buf);
  __int16 readShort();
  void reduceReferenceCountToResource( AbstractResource * res);
  void removeFileFromList( AbstractResource * fil);
  unsigned int setResource( const char * filename, int  useEvilersion);
  ResourceManager * constructor();
  void destructor();
}

ResourceManager * ResourceManager::constructor()
{
  MACRO_SEEK i; // [sp+10h] [bp-4h]@1

  AbstractManager_constructor((AbstractManager *)this);
  this->vtable = &ResourceManager_vtable;
  this->ready = 0;
  this->loadedFileLinkedList = NULL;
  this->isFreeingAllResources = 0;
  strcpy(this->resourceToLoad, unk_51F1F8);
  this->fileID = 0;
  for ( i = 0; i < SEEK_END; ++i )
  {
    this->fileDescriptors[i] = -1;
    this->aggContentInfo[i] = NULL;
    this->numberOfFilesInAGG[i] = 0;
  }
  this->numOpenAGGFiles = 0;
  this->curHandleIdx = 0;
  return this;
}

void ResourceManager::destructor()
{
  ResourceManager *thisa; // [sp+Ch] [bp-8h]@1
  signed int i; // [sp+10h] [bp-4h]@2

  thisa = this;
  if ( this->ready == 1 )
  {
    ResourceManager::freeAllResources(this);
    thisa->loadedFileLinkedList = 0;
    for ( i = 0; i < 2; ++i )
    {
      if ( thisa->aggContentInfo[i] )
        KBFree(thisa->aggContentInfo[i], (int)"F:\\h2xsrc\\Base\\RESMGR.CPP", 474);
      if ( thisa->fileDescriptors[i] != -1 )
      {
        _close(thisa->fileDescriptors[i]);
        thisa->fileDescriptors[i] = -1;
      }
    }
    thisa->numOpenAGGFiles = 0;
    thisa->ready = 0;
  }
}

unsigned int ResourceManager::setResource( const char * filename, int  useEvilersion)
{
  unsigned int result; // eax@8
  ResourceManager *v4; // [sp+Ch] [bp-Ch]@1
  signed int i; // [sp+10h] [bp-8h]@3

  v4 = this;
  strcpy(this->resourceToLoad, filename);
  if ( useEvilBorders && useEvilersion )
  {
    for ( i = 0; i < 37; ++i )
    {
      if ( !_strcmpi(v4->resourceToLoad, goodEvilIcons[i][0]) )
        strcpy(v4->resourceToLoad, goodEvilIcons[i][1]);
    }
  }
  result = computeFileID(v4->resourceToLoad);
  v4->fileID = result;
  return result;
}

signed int ResourceManager::addAGG( const char * filename)
{
  signed int result; // eax@2
  size_t v3; // ST1C_4@5
  ResourceManager *v4; // [sp+Ch] [bp-10h]@1
  __int16 buf; // [sp+14h] [bp-8h]@5
  int v6; // [sp+18h] [bp-4h]@3

  v4 = this;
  if ( this->numOpenAGGFiles < SEEK_END )
  {
    v6 = _open(filename, O_BINARY);
    if ( v6 == -1 )
    {
      sprintf(globBuf, "Can't open file: %s", filename);
      terminate(globBuf);
      result = 3;
    }
    else
    {
      v4->curHandleIdx = v4->numOpenAGGFiles++;
      v4->fileDescriptors[v4->curHandleIdx] = v6;
      _read(v4->fileDescriptors[v4->curHandleIdx], &buf, 2u);
      v4->numberOfFilesInAGG[v4->curHandleIdx] = buf;
      v3 = 12 * v4->numberOfFilesInAGG[v4->curHandleIdx];
      v4->aggContentInfo[v4->curHandleIdx] = (FileInAGGInfo *)KBAlloc(v3, "F:\\h2xsrc\\Base\\RESMGR.CPP", 542);
      _read(v4->fileDescriptors[v4->curHandleIdx], v4->aggContentInfo[v4->curHandleIdx], v3);
      result = 0;
    }
  }
  else
  {
    sprintf(globBuf, "Only %d .AGG files can be used at once.", SEEK_END);
    terminate(globBuf);
    result = 3;
  }
  return result;
}