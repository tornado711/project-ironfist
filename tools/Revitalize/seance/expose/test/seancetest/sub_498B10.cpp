signed int __thiscall sub_498B10(Hero *this, int a2)
{
  signed int i; // [sp+10h] [bp-4h]@1

  for ( i = 0; i < 14; ++i )
  {
    if ( this->ordinalSkillWasLearned[i] == a2 + 1 )
      return i;
  }
  return -1;
}