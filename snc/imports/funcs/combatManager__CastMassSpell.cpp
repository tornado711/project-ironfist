{
  int v3; // edx@1
  combatManager *thisb; // [sp+14h] [bp-4Ch]@1
  int othSide; // [sp+18h] [bp-48h]@2
  int thisSide; // [sp+18h] [bp-48h]@8
  signed int side0; // [sp+18h] [bp-48h]@14
  signed int side1; // [sp+18h] [bp-48h]@23
  signed int side2; // [sp+18h] [bp-48h]@35
  signed int othSidee; // [sp+18h] [bp-48h]@44
  signed int othSidef; // [sp+18h] [bp-48h]@53
  signed int othSideg; // [sp+18h] [bp-48h]@64
  signed int affectedSide; // [sp+18h] [bp-48h]@74
  army *thisa; // [sp+1Ch] [bp-44h]@80
  int isDamageSpell; // [sp+20h] [bp-40h]@1
  signed int damage; // [sp+24h] [bp-3Ch]@23
  signed int damagea; // [sp+24h] [bp-3Ch]@53
  signed int anyoneAffected; // [sp+28h] [bp-38h]@64
  STACK_MODIFYING_EFFECT effect; // [sp+2Ch] [bp-34h]@87
  int animIdx; // [sp+30h] [bp-30h]@1
  int i; // [sp+34h] [bp-2Ch]@2
  int j; // [sp+34h] [bp-2Ch]@8
  int k; // [sp+34h] [bp-2Ch]@16
  int l; // [sp+34h] [bp-2Ch]@25
  int m; // [sp+34h] [bp-2Ch]@37
  int n; // [sp+34h] [bp-2Ch]@46
  int ii; // [sp+34h] [bp-2Ch]@55
  int jj; // [sp+34h] [bp-2Ch]@66
  int kk; // [sp+34h] [bp-2Ch]@76
  char stackAffected[2][20]; // [sp+38h] [bp-28h]@1

  thisb = this;
  animIdx = gsSpellInfo[spell].creatureEffectAnimationIdx;
  isDamageSpell = 0;
  gpWindowManager->field_56 = 0;
  combatManager::ShowSpellMessage(this, 0, spell, 0);
  memset(stackAffected, 0, 40u);
  switch ( spell )
  {
    case SPELL_MASS_SLOW:
    case SPELL_MASS_CURSE:
      othSide = 1 - thisb->currentActionSide;
      for ( i = 0; thisb->numCreatures[othSide] > i; ++i )
      {
        if ( army::SpellCastWorks(&thisb->creatures[othSide][i], spell) )
          stackAffected[othSide][i] = 1;
      }
      break;
    case SPELL_MASS_CURE:
    case SPELL_MASS_HASTE:
    case SPELL_MASS_BLESS:
    case SPELL_MASS_SHIELD:
      thisSide = thisb->currentActionSide;
      for ( j = 0; thisb->numCreatures[thisSide] > j; ++j )
      {
        if ( army::SpellCastWorks(&thisb->creatures[thisSide][j], spell) )
          stackAffected[thisSide][j] = 1;
      }
      break;
    case SPELL_MASS_DISPEL:
      for ( side0 = 0; side0 < 2; ++side0 )
      {
        for ( k = 0; thisb->numCreatures[side0] > k; ++k )
        {
          if ( army::SpellCastWorks(&thisb->creatures[side0][k], spell) )
            stackAffected[side0][k] = 1;
        }
      }
      break;
    case SPELL_HOLY_WORD:
    case SPELL_HOLY_SHOUT:
      isDamageSpell = 1;
      damage = spellpower * ((unsigned int)(spell - SPELL_HOLY_WORD) < 1 ? 10 : 20);
      for ( side1 = 0; side1 < 2; ++side1 )
      {
        for ( l = 0; ; ++l )
        {
          v3 = l;
          if ( thisb->numCreatures[side1] <= l )
            break;
          if ( HIBYTE(thisb->creatures[side1][l].creature.creature_flags) & ATTR_UNDEAD
            && army::SpellCastWorks(&thisb->creatures[side1][l], spell) )
            stackAffected[side1][l] = 1;
        }
      }
      if ( spell == SPELL_HOLY_WORD )
        combatManager::Blur(thisb, v3, 0, -2, -2);
      else
        combatManager::Blur(thisb, v3, 0, -4, -4);
      for ( side2 = 0; side2 < 2; ++side2 )
      {
        for ( m = 0; thisb->numCreatures[side2] > m; ++m )
        {
          if ( stackAffected[side2][m] )
            army::Damage(&thisb->creatures[side2][m], damage, SPELL_NONE);
        }
      }
      sprintf(gText, "The %s spell does %d damage\nto all undead creatures.", gSpellNames[spell], damage);
      combatManager::CombatMessage(thisb, gText, 1, 1, 0);
      break;
    case SPELL_DEATH_RIPPLE:
    case SPELL_DEATH_WAVE:
      isDamageSpell = 1;
      for ( othSidee = 0; othSidee < 2; ++othSidee )
      {
        for ( n = 0; ; ++n )
        {
          v3 = n;
          if ( thisb->numCreatures[othSidee] <= n )
            break;
          if ( !(HIBYTE(thisb->creatures[othSidee][n].creature.creature_flags) & ATTR_UNDEAD)
            && army::SpellCastWorks(&thisb->creatures[othSidee][n], spell) )
            stackAffected[othSidee][n] = 1;
        }
      }
      combatManager::Ripple(thisb, v3, 2 - ((unsigned int)(spell - SPELL_DEATH_RIPPLE) < 1));
      damagea = spellpower * ((unsigned int)(spell - SPELL_DEATH_RIPPLE) < 1 ? 5 : 10);
      for ( othSidef = 0; othSidef < 2; ++othSidef )
      {
        for ( ii = 0; thisb->numCreatures[othSidef] > ii; ++ii )
        {
          if ( stackAffected[othSidef][ii] )
            army::Damage(&thisb->creatures[othSidef][ii], damagea, SPELL_NONE);
        }
      }
      sprintf(gText, "The Death spell does %d damage\nto all living creatures.", damagea);
      combatManager::CombatMessage(thisb, gText, 1, 1, 0);
      break;
    default:
      break;
  }
  if ( !gbNoShowCombat )
  {
    anyoneAffected = 0;
    for ( othSideg = 0; othSideg < 2; ++othSideg )
    {
      for ( jj = 0; thisb->numCreatures[othSideg] > jj; ++jj )
      {
        if ( stackAffected[othSideg][jj] )
          anyoneAffected = 1;
      }
    }
    if ( anyoneAffected )
      combatManager::ShowMassSpell(thisb, (char (*)[2][20])stackAffected, animIdx, isDamageSpell);
  }
  for ( affectedSide = 0; affectedSide < 2; ++affectedSide )
  {
    for ( kk = 0; thisb->numCreatures[affectedSide] > kk; ++kk )
    {
      if ( stackAffected[affectedSide][kk] )
      {
        thisa = &thisb->creatures[affectedSide][kk];
        switch ( spell )
        {
          case SPELL_MASS_CURSE:
            army::SetSpellInfluence(thisa, EFFECT_CURSE, spellpower);
            break;
          case SPELL_MASS_SLOW:
            army::SetSpellInfluence(thisa, EFFECT_SLOW, spellpower);
            break;
          case SPELL_MASS_HASTE:
            army::SetSpellInfluence(thisa, EFFECT_HASTE, spellpower);
            break;
          case SPELL_MASS_BLESS:
            army::SetSpellInfluence(thisa, EFFECT_BLESS, spellpower);
            break;
          case SPELL_MASS_SHIELD:
            army::SetSpellInfluence(thisa, EFFECT_SHIELD, spellpower);
            break;
          case SPELL_MASS_CURE:
            army::Cure(thisa, spellpower);
            break;
          case SPELL_MASS_DISPEL:
            for ( effect = 0; (signed int)effect < 15; ++effect )
              army::CancelIndividualSpell(thisa, effect);
            break;
          case SPELL_DEATH_RIPPLE:
          case SPELL_DEATH_WAVE:
            continue;
        }
      }
    }
  }
  combatManager::DrawFrame(thisb, 1, 0, 0, 0, 75, 1, 1);
  gpWindowManager->field_56 = 1;
}