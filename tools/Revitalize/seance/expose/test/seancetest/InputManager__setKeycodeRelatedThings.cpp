signed int __thiscall InputManager::setKeycodeRelatedThings(InputManager *this)
{
  unsigned int v1; // eax@1
  __int16 v2; // dx@2
  signed int result; // eax@3

  v1 = 0;
  do
  {
    v2 = v1++;
    *((_WORD *)&this->field_74A + v1 + 1) = v2 << 8;
  }
  while ( v1 < 0x80 );
  this->keycodeRelated[0] = 0;
  this->keycodeRelated[1] = 27;
  this->keycodeRelated[2] = 49;
  this->keycodeRelated[3] = 50;
  this->keycodeRelated[4] = 51;
  this->keycodeRelated[5] = 52;
  this->keycodeRelated[6] = 53;
  this->keycodeRelated[7] = 54;
  this->keycodeRelated[8] = 55;
  this->keycodeRelated[9] = 56;
  this->keycodeRelated[10] = 57;
  this->keycodeRelated[11] = 48;
  this->keycodeRelated[13] = 61;
  this->keycodeRelated[14] = 127;
  this->keycodeRelated[15] = 9;
  this->keycodeRelated[16] = 81;
  this->keycodeRelated[17] = 87;
  this->keycodeRelated[18] = 69;
  this->keycodeRelated[19] = 82;
  this->keycodeRelated[20] = 84;
  this->keycodeRelated[21] = 89;
  this->keycodeRelated[22] = 85;
  this->keycodeRelated[23] = 73;
  this->keycodeRelated[24] = 79;
  this->keycodeRelated[25] = 80;
  this->keycodeRelated[26] = 91;
  this->keycodeRelated[27] = 93;
  this->keycodeRelated[28] = 10;
  this->keycodeRelated[29] = 7424;
  this->keycodeRelated[30] = 65;
  this->keycodeRelated[31] = 83;
  this->keycodeRelated[32] = 68;
  this->keycodeRelated[33] = 70;
  this->keycodeRelated[34] = 71;
  this->keycodeRelated[35] = 72;
  result = 45;
  this->keycodeRelated[36] = 74;
  this->keycodeRelated[37] = 75;
  this->keycodeRelated[38] = 76;
  this->keycodeRelated[41] = 10496;
  this->keycodeRelated[42] = 10752;
  this->keycodeRelated[43] = 92;
  this->keycodeRelated[44] = 90;
  this->keycodeRelated[45] = 88;
  this->keycodeRelated[46] = 67;
  this->keycodeRelated[47] = 86;
  this->keycodeRelated[48] = 66;
  this->keycodeRelated[49] = 78;
  this->keycodeRelated[50] = 77;
  this->keycodeRelated[51] = 44;
  this->keycodeRelated[52] = 46;
  this->keycodeRelated[53] = 47;
  this->keycodeRelated[54] = 13824;
  this->keycodeRelated[55] = 42;
  this->keycodeRelated[56] = 14336;
  this->keycodeRelated[57] = 32;
  this->keycodeRelated[12] = 45;
  this->keycodeRelated[39] = 39;
  this->keycodeRelated[40] = 39;
  this->keycodeRelated[58] = 14848;
  this->keycodeRelated[59] = 15104;
  this->keycodeRelated[60] = 15360;
  this->keycodeRelated[61] = 15616;
  this->keycodeRelated[62] = 15872;
  this->keycodeRelated[63] = 16128;
  this->keycodeRelated[64] = 16384;
  this->keycodeRelated[65] = 16640;
  this->keycodeRelated[66] = 16896;
  this->keycodeRelated[67] = 17152;
  this->keycodeRelated[68] = 17408;
  this->keycodeRelated[69] = 17664;
  this->keycodeRelated[70] = 17920;
  this->keycodeRelated[71] = 18176;
  this->keycodeRelated[72] = 18432;
  this->keycodeRelated[73] = 18688;
  this->keycodeRelated[75] = 19200;
  this->keycodeRelated[76] = 19456;
  this->keycodeRelated[77] = 19712;
  this->keycodeRelated[78] = 43;
  this->keycodeRelated[79] = 20224;
  this->keycodeRelated[80] = 20480;
  this->keycodeRelated[81] = 20736;
  this->keycodeRelated[82] = 20992;
  this->keycodeRelated[83] = 21248;
  this->keycodeRelated[84] = 21504;
  this->keycodeRelated[85] = 21760;
  this->keycodeRelated[86] = 22016;
  this->keycodeRelated[87] = 22272;
  this->keycodeRelated[88] = 22528;
  this->keycodeRelated[74] = 45;
  return result;
}