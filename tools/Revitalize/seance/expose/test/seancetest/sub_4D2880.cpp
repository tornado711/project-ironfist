char __thiscall sub_4D2880(void *this)
{
  int v2; // [sp+Ch] [bp-Ch]@1
  signed int i; // [sp+10h] [bp-8h]@1
  int v4; // [sp+14h] [bp-4h]@1

  v2 = (int)this;
  v4 = 0;
  for ( i = 0; i <= 7; ++i )
    v4 = 2 * v4 | sub_4D27E0(v2);
  return v4;
}