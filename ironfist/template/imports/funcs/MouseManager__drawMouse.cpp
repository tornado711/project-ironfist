{
  Bitmap::blitTo(
    this->bitmap,
    HeroWindowManager::instance->screenBuffer,
    this->cursorTopLeftX,
    this->cursorTopLeftY,
    0,
    0,
    this->cursorWidth,
    this->cursorHeight);
}