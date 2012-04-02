Icon *__thiscall Icon_constructor(Icon *this, int fileID)
{
  Icon *thisa; // edi@1
  int totalSize; // eax@1
  unsigned int totalSizea; // esi@1
  void *data; // eax@1

  thisa = this;
  AbstractResource_constructor((AbstractResource *)this, RESOURCE_TYPE_ICN, fileID, 1, NULL);
  thisa->vtable = &Icon_vtable;
  ResourceManager::pointToFile(resourceManager, fileID);
  thisa->numSprites = ResourceManager::readShort(resourceManager);
  totalSize = ResourceManager::readInt(resourceManager);
  totalSizea = totalSize;
  data = KBAlloc(totalSize, "F:\\h2xsrc\\Base\\ICON.CPP", 18);
  thisa->headersAndImageData = (ICNHeader *)data;
  ResourceManager::readFromCurrentFile(resourceManager, data, totalSizea);
  return thisa;
}