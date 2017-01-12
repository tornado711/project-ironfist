#include "base.h"
#include "adventure/adv.h"
#include "combat/combat.h"
#include "game/game.h"
#include "resource/resourceManager.h"
#include "artifacts.h"
#include "skills.h"
#include "sound/sound.h"
#include "spells.h"

#include "gui/dialog.h"

#include "scripting/callback.h"
#include <string>

extern int giNextAction;
extern signed char gbCombatSurrender;
extern signed char gbRetreatWin;
extern char *cMonFilename[]; // it's inside creature.cpp
extern char *cArmyFrameFileNames[]; // it's inside creature.cpp
extern char *cArmyProjectileFileNames[]; // it's inside creature.cpp
extern icon *gCurLoadedSpellIcon;

extern int gCurSpellEffectFrame;
extern int gCurLoadedSpellEffect;

unsigned __int8 giNumPowFrames[33] =
{
  10u,
  10u,
  10u,
  10u,
  10u,
  10u,
  10u,
  10u,
  10u,
  8u,
  8u,
  10u,
  10u,
  10u,
  10u,
  15u,
  10u,
  10u,
  10u,
  10u,
  10u,
  16u,
  16u,
  14u,
  19u,
  22u,
  10u,
  17u,
  10u,
  12u,
  11u,
  16u,
  7u
};

char *gCombatFxNames[33] =
{
  "",
  "magic01.icn",
  "magic02.icn",
  "magic03.icn",
  "magic04.icn",
  "magic05.icn",
  "magic06.icn",
  "magic07.icn",
  "magic08.icn",
  "rainbluk.icn",
  "cloudluk.icn",
  "moraleg.icn",
  "moraleb.icn",
  "reddeath.icn",
  "redfire.icn",
  "sparks.icn",
  "electric.icn",
  "physical.icn",
  "bluefire.icn",
  "icecloud.icn",
  "lichclod.icn",
  "bless.icn",
  "berzerk.icn",
  "shield.icn",
  "haste.icn",
  "paralyze.icn",
  "hypnotiz.icn",
  "dragslay.icn",
  "blind.icn",
  "curse.icn",
  "stonskin.icn",
  "stelskin.icn",
  "plasmblast.icn"
};

int squaresAroundCaster[2][3] = {
  {14,27,40},
  {11,24,37}
};

void combatManager::InitNonVisualVars() {
  combatManager::InitNonVisualVars_orig();
        
  ScriptCallback("OnBattleStart");

  for(int i = 0; i < 2; i++) {
    HandlePandoraBox(i);
  }
}

/*
* What happens when a hero wins a battle using Pandora's Box, but loses their
* main army? They walk around with stacks of 0 creatures, of course! 
* 0-creature stacks are still useful in combat though, since all attacks do at least 1 damage.
*
* On a related note, in the original game, what happens when a hero wins a battle
* using temporarily-resurrected creatures, but has no army left at the end? They
* walk around with no creatures, and instantly lose their next battle.
*
* Winning a battle with nothing but summoned elementals remaining works, however.
*
* TL, DR: There is a bug when winning a battle with nothing but temporary creatures left,
* but it's also present in the original game.
*/
void combatManager::HandlePandoraBox(int side) {
  if(this->heroes[side] && this->heroes[side]->HasArtifact(ARTIFACT_PANDORA_BOX)) {

    //The HoMM II code appears to lack a definition of creature tier. This deserves investigation.
    //We temporarily hardcode the tier-1 creatures
    int creatChoices[] = {
      CREATURE_PEASANT, CREATURE_SPRITE,CREATURE_HALFLING, CREATURE_GOBLIN,
      CREATURE_SKELETON, CREATURE_CENTAUR, CREATURE_ROGUE, CREATURE_BLOODSUCKER
    };
    int creat = creatChoices[SRandom(0, ELEMENTS_IN(creatChoices)-1)];

    int hex = -1;
    int poss = ELEMENTS_IN(squaresAroundCaster[side]);
    int tryFirst = SRandom(0, poss-1);
    for(int i = 0; i < poss; i++) {
      int square = squaresAroundCaster[side][(i+tryFirst)%poss];
      if(gMonsterDatabase[creat].creature_flags & TWO_HEXER) {
        int dir = side == 0 ? 1 : -1;
        if(this->combatGrid[square+dir].unitOwner != -1)
          continue;
      }
      if(this->combatGrid[square].unitOwner == -1)
        hex = square;
    }

    if(hex==-1)
      return;

    int amt = gpGame->GetRandomNumTroops(creat);
    AddArmy(side, creat, amt, hex, 0x8000, 0);

    hexcell* cell = &this->combatGrid[hex];
    this->creatures[cell->unitOwner][cell->stackIdx].temporaryQty = amt;
  }
}

void army::MoveTo(int hexIdx) {
  if(this->creature.creature_flags & FLYER) {
    this->targetHex = hexIdx;
    if(ValidFlight(this->targetHex, 0))
      FlyTo(this->targetHex);
  } else {
    WalkTo(hexIdx);
  }
}

void army::MoveAttack(int targHex, int x) {
  int startHex = this->occupiedHex;
  this->MoveAttack_orig(targHex, x);

  if( !(this->creature.creature_flags & DEAD) &&
    CreatureHasAttribute(this->creatureIdx, STRIKE_AND_RETURN)) {
    MoveTo(startHex);
  }
}

int __fastcall OppositeDirection(signed int hex) {
  int result;
  if (hex >= 6) {
    if (hex == 6)
      result = 7;
    else
      result = 6;
  } else {
    result = (hex + 3) % 6;
  }
  return result;
}

extern int gbGenieHalf;
extern int gbRemoteOn;

void DoAttackBattleMessage(army *attacker, army *target, int creaturesKilled, int damageDone) {
  char *attackingCreature;
  if (attacker->quantity <= 1)
    attackingCreature = GetCreatureName(attacker->creatureIdx);
  else
    attackingCreature = GetCreaturePluralName(attacker->creatureIdx);

  if (damageDone == -1) {
    sprintf(gText, "The mirror image is destroyed!");
  } else if (gbGenieHalf) {
    sprintf(gText,"%s %s half the enemy troops!", attackingCreature, (attacker->quantity > 1) ? "damage" : "damages");
  } else if (creaturesKilled <= 0) {
    sprintf(gText, "%s %s %d damage.", attackingCreature, (attacker->quantity > 1) ? "do" : "does", damageDone);
    gText[0] = toupper(gText[0]);
  } else {
    char *targetCreature;
    if (creaturesKilled <= 1)
      targetCreature = GetCreatureName(target->creatureIdx);
    else
      targetCreature = GetCreaturePluralName(target->creatureIdx);
    sprintf(
      gText,
      "%s %s %d damage.\n%d %s %s.",
      attackingCreature,
      (attacker->quantity > 1) ? "do" : "does",
      damageDone,
      creaturesKilled,
      targetCreature,
      (creaturesKilled > 1) ? "perish" : "perishes");
      gText[0] = toupper(gText[0]);
  }
  gpCombatManager->CombatMessage(gText, 1, 1, 0);
}

void army::DoAttack(int isRetaliation) {
  army* primaryTarget = &gpCombatManager->creatures[gpCombatManager->combatGrid[targetHex].unitOwner][gpCombatManager->combatGrid[targetHex].stackIdx];
  if (gpCombatManager->combatGrid[targetHex].unitOwner < 0 || gpCombatManager->combatGrid[targetHex].stackIdx < 0)
    primaryTarget = this;
  ScriptCallback("OnBattleMeleeAttack", this, primaryTarget, isRetaliation);

  primaryTarget = 0;
  this->field_6 = 3;
  int creaturesKilled = 0;
  army *secondHexTarget = nullptr;
  int oldFacingRight = this->facingRight;
  if (isRetaliation)
    gpCombatManager->currentActionSide = 1 - gpCombatManager->currentActionSide;
  if (this->creatureIdx == CREATURE_HYDRA) {
    this->DoHydraAttack(isRetaliation);
  } else {
    int v18 = this->targetNeighborIdx;
    int targetHex = this->occupiedHex;
    if (this->creature.creature_flags & TWO_HEXER
      && (!this->facingRight && this->targetNeighborIdx >= 3
        || this->facingRight == 1 && (this->targetNeighborIdx <= 2 || this->targetNeighborIdx >= 6))) {
      if (oldFacingRight)
        targetHex = this->occupiedHex + 1;
      else
        targetHex = this->occupiedHex - 1;
    }
    targetHex = this->GetAdjacentCellIndex(targetHex, this->targetNeighborIdx);
    primaryTarget = &gpCombatManager->creatures[gpCombatManager->combatGrid[targetHex].unitOwner][gpCombatManager->combatGrid[targetHex].stackIdx];
    if (this->creature.creature_flags & TWO_HEX_ATTACKER) {
      int secondTargetHex = this->GetAdjacentCellIndex(targetHex, this->targetNeighborIdx);
      if (ValidHex(secondTargetHex)) {
        if (gpCombatManager->combatGrid[secondTargetHex].unitOwner >= 0
          && gpCombatManager->combatGrid[secondTargetHex].stackIdx >= 0
          && (gpCombatManager->combatGrid[secondTargetHex].unitOwner != primaryTarget->owningSide
            || gpCombatManager->combatGrid[secondTargetHex].stackIdx != primaryTarget->stackIdx))
          secondHexTarget = &gpCombatManager->creatures[gpCombatManager->combatGrid[secondTargetHex].unitOwner][gpCombatManager->combatGrid[secondTargetHex].stackIdx];
      }
    }
    gpCombatManager->ResetLimitCreature();
    int v2 = 80 * this->owningSide + 4 * this->stackIdx;
    ++*(signed int *)((char *)gpCombatManager->limitCreature[0] + v2);
    int v3 = 80 * primaryTarget->owningSide + 4 * primaryTarget->stackIdx;
    ++*(signed int *)((char *)gpCombatManager->limitCreature[0] + v3);
    if (secondHexTarget) {
      int v4 = 80 * secondHexTarget->owningSide + 4 * secondHexTarget->stackIdx;
      ++*(signed int *)((char *)gpCombatManager->limitCreature[0] + v4);
    }
    gpCombatManager->DrawFrame(0, 1, 0, 1, 75, 1, 1);
    int targetOldFacingRight = primaryTarget->facingRight;

    int shouldFaceRight;
    if (this->targetNeighborIdx > 2) {
      if (this->targetNeighborIdx > 5)
        shouldFaceRight = this->facingRight;
      else
        shouldFaceRight = 0;
    } else {
      shouldFaceRight = 1;
    }
    if (this->facingRight != shouldFaceRight) {
      this->facingRight = shouldFaceRight;
      if (this->creature.creature_flags & TWO_HEXER) {
        if (shouldFaceRight == 1)
          --this->occupiedHex;
        else
          ++this->occupiedHex;
      }
      primaryTarget->facingRight = 1 - this->facingRight;
      if (primaryTarget->facingRight != targetOldFacingRight && primaryTarget->creature.creature_flags & TWO_HEXER) {
        if (primaryTarget->facingRight == 1)
          --primaryTarget->occupiedHex;
        else
          ++primaryTarget->occupiedHex;
      }
    }
    this->CheckLuck();
    this->mightBeIsAttacking = 1;
    if (this->targetNeighborIdx != 6 && this->targetNeighborIdx != 5 && this->targetNeighborIdx) {
      if (this->targetNeighborIdx != 1 && this->targetNeighborIdx != 4)
        this->mightBeAttackAnimIdx = 24;
      else
        this->mightBeAttackAnimIdx = 20;
    } else {
      this->mightBeAttackAnimIdx = 16;
    }
    if (secondHexTarget)
      this->mightBeAttackAnimIdx += 2;
    gpSoundManager->MemorySample(this->combatSounds[1]);
    
    int damDone;
    int damageDone = 0;
    this->DamageEnemy(primaryTarget, &damDone, (int *)&creaturesKilled, 0, 0);
    int v13 = 0; // unused
    if (secondHexTarget)
      this->DamageEnemy(secondHexTarget, &damageDone, &v13, 0, 0);
    
    DoAttackBattleMessage(this, primaryTarget, creaturesKilled, damDone);
    
    int enemyIncapacitated = 0;
    switch (this->creatureIdx) {
      case CREATURE_CYCLOPS:
        if (primaryTarget->quantity > 0 && (!secondHexTarget || secondHexTarget->quantity > 0)) {
          if (SRandom(1, 100) >= 20) {
            if (SRandom(1, 100) < 20 && secondHexTarget && secondHexTarget->SpellCastWorks(SPELL_PARALYZE))
              secondHexTarget->spellEnemyCreatureAbilityIsCasting = SPELL_PARALYZE;
          } else if (primaryTarget && primaryTarget->SpellCastWorks(SPELL_PARALYZE)) {
            primaryTarget->spellEnemyCreatureAbilityIsCasting = SPELL_PARALYZE;
            enemyIncapacitated = 1;
          }
        }
        break;
      case CREATURE_UNICORN:
        if (SRandom(1, 100) < 20 && primaryTarget && primaryTarget->SpellCastWorks(SPELL_BLIND)) {
          primaryTarget->spellEnemyCreatureAbilityIsCasting = SPELL_BLIND;
          enemyIncapacitated = 1;
        }
        break;
      case CREATURE_MEDUSA:
        if (SRandom(1, 100) < 20 && primaryTarget && primaryTarget->SpellCastWorks(SPELL_MEDUSA_PETRIFY)) {
          primaryTarget->spellEnemyCreatureAbilityIsCasting = SPELL_MEDUSA_PETRIFY;
          enemyIncapacitated = 1;
        }
        break;
      case CREATURE_MUMMY:
        if (SRandom(1, 100) < 20)
          goto LABEL_97;
        break;
      case CREATURE_ROYAL_MUMMY:
        if (SRandom(1, 100) < 30) {
        LABEL_97:
          if (primaryTarget && primaryTarget->SpellCastWorks(SPELL_CURSE))
            primaryTarget->spellEnemyCreatureAbilityIsCasting = SPELL_CURSE;
        }
        break;
      case CREATURE_ARCHMAGE:
        if (SRandom(1, 100) < 20 && primaryTarget && primaryTarget->SpellCastWorks(SPELL_ARCHMAGI_DISPEL))
          primaryTarget->spellEnemyCreatureAbilityIsCasting = SPELL_ARCHMAGI_DISPEL;
        break;
      case CREATURE_GHOST:
        gpCombatManager->ghostAndVampireAbilityStrength[gpCombatManager->combatGrid[this->occupiedHex].unitOwner] = creaturesKilled;
        break;
      case CREATURE_VAMPIRE_LORD:
        gpCombatManager->ghostAndVampireAbilityStrength[gpCombatManager->combatGrid[this->occupiedHex].unitOwner] = creaturesKilled * primaryTarget->creature.hp;
        break;
      case CREATURE_SPRITE:
      case CREATURE_DWARF:
      case CREATURE_BATTLE_DWARF:
      case CREATURE_ELF:
      case CREATURE_GRAND_ELF:
      case CREATURE_DRUID:
      case CREATURE_GREATER_DRUID:
      case CREATURE_PHOENIX:
      case CREATURE_CENTAUR:
      case CREATURE_GARGOYLE:
      case CREATURE_GRIFFIN:
      case CREATURE_MINOTAUR:
      case CREATURE_MINOTAUR_KING:
      case CREATURE_HYDRA:
      case CREATURE_GREEN_DRAGON:
      case CREATURE_RED_DRAGON:
      case CREATURE_BLACK_DRAGON:
      case CREATURE_HALFLING:
      case CREATURE_BOAR:
      case CREATURE_IRON_GOLEM:
      case CREATURE_STEEL_GOLEM:
      case CREATURE_ROC:
      case CREATURE_MAGE:
      case CREATURE_GIANT:
      case CREATURE_TITAN:
      case CREATURE_SKELETON:
      case CREATURE_ZOMBIE:
      case CREATURE_MUTANT_ZOMBIE:
      case CREATURE_VAMPIRE:
      case CREATURE_LICH:
      case CREATURE_POWER_LICH:
      case CREATURE_BONE_DRAGON:
      case CREATURE_ROGUE:
      case CREATURE_NOMAD:
      case CREATURE_GENIE:
        break;
    }
    this->PowEffect(-1, 0, -1, -1);
    gpCombatManager->limitCreature[this->owningSide][this->stackIdx] = 1;
    if (this->creatureIdx == CREATURE_GHOST)
      this->quantity += gpCombatManager->ghostAndVampireAbilityStrength[gpCombatManager->combatGrid[this->occupiedHex].unitOwner];
    if (this->creatureIdx == CREATURE_VAMPIRE_LORD) {
      if (gpCombatManager->ghostAndVampireAbilityStrength[gpCombatManager->combatGrid[this->occupiedHex].unitOwner] >= this->damage) {
        int v5 = gpCombatManager->combatGrid[this->occupiedHex].unitOwner;
        gpCombatManager->ghostAndVampireAbilityStrength[v5] -= this->damage;
        this->damage = 0;
        int v11 = gpCombatManager->ghostAndVampireAbilityStrength[gpCombatManager->combatGrid[this->occupiedHex].unitOwner] / this->creature.hp;
        if (this->initialQuantity - this->quantity <= v11)
          this->quantity = this->initialQuantity;
        else
          this->quantity += v11;
      } else {
        this->damage -= gpCombatManager->ghostAndVampireAbilityStrength[gpCombatManager->combatGrid[this->occupiedHex].unitOwner];
      }
    }
    if (primaryTarget
      && primaryTarget->quantity > 0
      && !primaryTarget->effectStrengths[6]
      && !primaryTarget->effectStrengths[11]
      && (primaryTarget->creatureIdx == CREATURE_GRIFFIN || !(primaryTarget->creature.creature_flags & RETALIATED))
      && this->creatureIdx != CREATURE_ROGUE
      && this->creatureIdx != CREATURE_SPRITE
      && this->creatureIdx != CREATURE_VAMPIRE
      && this->creatureIdx != CREATURE_VAMPIRE_LORD
      && !enemyIncapacitated
      && !isRetaliation) {
      DelayMilli((signed __int64)(gfCombatSpeedMod[giCombatSpeed] * 150.0));
      primaryTarget->targetNeighborIdx = OppositeDirection(this->targetNeighborIdx);
      if (primaryTarget->creature.creature_flags & TWO_HEXER) {
        if (this->occupiedHex == this->GetAdjacentCellIndex(primaryTarget->occupiedHex, (unsigned int)(primaryTarget->facingRight - 1) < 1 ? 0 : 5))
          primaryTarget->targetNeighborIdx = 6;
        if (this->occupiedHex == this->GetAdjacentCellIndex(primaryTarget->occupiedHex, 3 - ((unsigned int)(primaryTarget->facingRight - 1) < 1)))
          primaryTarget->targetNeighborIdx = 7;
      }
      primaryTarget->DoAttack(1);
      primaryTarget->creature.creature_flags |= RETALIATED;
      if (gbRemoteOn
        && gpCombatManager->involvedInBadMorale[0]
        && gpCombatManager->involvedInBadMorale[1]
        && primaryTarget->creatureIdx == CREATURE_GHOST)
        primaryTarget->quantity += gpCombatManager->ghostAndVampireAbilityStrength[gpCombatManager->combatGrid[primaryTarget->occupiedHex].unitOwner];
    }
    if ((this->creatureIdx == CREATURE_WOLF
      || this->creatureIdx == CREATURE_PALADIN
      || this->creatureIdx == CREATURE_CRUSADER)
      && primaryTarget
      && primaryTarget->quantity > 0
      && !isRetaliation
      && !this->effectStrengths[6]
      && !this->effectStrengths[11]
      && !this->effectStrengths[2]
      && this->quantity > 0) {
      DelayMilli((signed __int64)(gfCombatSpeedMod[giCombatSpeed] * 100.0));
      int v16 = this->targetNeighborIdx;
      this->targetNeighborIdx = v18;
      this->DoAttack(1);
      this->targetNeighborIdx = v16;
    }
    if (this->facingRight != oldFacingRight) {
      if (!(this->creature.creature_flags & DEAD)) {
        this->facingRight = oldFacingRight;
        if (this->creature.creature_flags & TWO_HEXER) {
          if (oldFacingRight == 1)
            --this->occupiedHex;
          else
            ++this->occupiedHex;
        }
      }
      if (!(primaryTarget->creature.creature_flags & DEAD)) {
        if (primaryTarget->facingRight != targetOldFacingRight) {
          primaryTarget->facingRight = targetOldFacingRight;
          if (primaryTarget->creature.creature_flags & TWO_HEXER) {
            if (primaryTarget->facingRight == 1)
              --primaryTarget->occupiedHex;
            else
              ++primaryTarget->occupiedHex;
          }
        }
      }
    }
  }
  if (!isRetaliation && (this->effectStrengths[5] || this->effectStrengths[7])) {
    this->CancelSpellType(1);
    gpCombatManager->DrawFrame(1, 0, 0, 0, 75, 1, 1);
  }
  this->targetOwner = -1;
  if (isRetaliation)
    gpCombatManager->currentActionSide = 1 - gpCombatManager->currentActionSide;
}

/*
void army::SpecialAttack() {
  army* primaryTarget = &gpCombatManager->creatures[gpCombatManager->combatGrid[targetHex].unitOwner][gpCombatManager->combatGrid[targetHex].stackIdx];
  ScriptSetSpecialVariableData("__attackingStack", this);
  ScriptSetSpecialVariableData("__targetStack", primaryTarget);
  std::string tmp = std::to_string(this->creatureIdx) + "," + std::to_string(primaryTarget->creatureIdx);
  ScriptSignal(SCRIPT_EVT_BATTLE_ATTACK_S, tmp);
  if (this->creatureIdx == CREATURE_MAGE) { // temporary creature. cyber behemoth attack
    gpCombatManager->CastSpell(SPELL_FIREBLAST, primaryTarget->occupiedHex, 1, 0);
  }
  else this->SpecialAttack_orig();
}
*/
// We don't actually change anything in sElevationOverlay, but the disasm was causing some problems

#pragma pack(push, 1)
struct SElevationOverlay {
  __int16 terrains;
  char coveredHexes[15];
};
#pragma pack(pop)

SElevationOverlay sElevationOverlay[25] =
{
  {
    0,
    {
      255,
      255,
      255,
      255,
      255,
      255,
      255,
      255,
      255,
      255,
      255,
      255,
      255,
      255,
      255
    }
  },
  { 2, { 30, 31, 32, 33, 47, 60, 255, 255, 255, 255, 255, 255, 255, 255, 255 } },
  {
    2,
    { 56, 57, 58, 59, 60, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 }
  },
  { 2, { 42, 55, 56, 57, 58, 59, 60, 48, 255, 255, 255, 255, 255, 255, 255 } },
  { 2, { 69, 70, 71, 72, 73, 60, 48, 255, 255, 255, 255, 255, 255, 255, 255 } },
  { 2, { 29, 30, 31, 32, 33, 34, 35, 81, 69, 70, 71, 72, 73, 74, 87 } },
  { 2, { 29, 17, 18, 19, 20, 21, 81, 95, 96, 97, 98, 99, 255, 255, 255 } },
  { 4, { 30, 31, 32, 33, 47, 60, 255, 255, 255, 255, 255, 255, 255, 255, 255 } },
  {
    4,
    { 56, 57, 58, 59, 60, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 }
  },
  { 4, { 42, 55, 56, 57, 58, 59, 47, 255, 255, 255, 255, 255, 255, 255, 255 } },
  { 4, { 69, 70, 71, 72, 73, 60, 48, 255, 255, 255, 255, 255, 255, 255, 255 } },
  { 4, { 18, 30, 43, 84, 85, 73, 60, 255, 255, 255, 255, 255, 255, 255, 255 } },
  { 4, { 21, 34, 48, 70, 83, 97, 98, 255, 255, 255, 255, 255, 255, 255, 255 } },
  {
    64,
    { 30, 31, 32, 33, 47, 60, 255, 255, 255, 255, 255, 255, 255, 255, 255 }
  },
  {
    64,
    { 56, 57, 58, 59, 60, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 }
  },
  { 64, { 42, 55, 56, 57, 58, 59, 60, 48, 255, 255, 255, 255, 255, 255, 255 } },
  { 64, { 69, 70, 71, 72, 73, 60, 48, 255, 255, 255, 255, 255, 255, 255, 255 } },
  { 64, { 29, 30, 31, 32, 33, 34, 35, 81, 69, 70, 71, 72, 73, 74, 87 } },
  { 64, { 29, 17, 18, 19, 20, 21, 81, 95, 96, 97, 98, 99, 255, 255, 255 } },
  {
    128,
    { 30, 31, 32, 33, 47, 60, 255, 255, 255, 255, 255, 255, 255, 255, 255 }
  },
  {
    128,
    { 56, 57, 58, 59, 60, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 }
  },
  { 128, { 42, 55, 56, 57, 58, 59, 60, 48, 255, 255, 255, 255, 255, 255, 255 } },
  {
    128,
    { 69, 70, 71, 72, 73, 60, 48, 255, 255, 255, 255, 255, 255, 255, 255 }
  },
  {
    128,
    { 43, 30, 18, 84, 85, 73, 60, 255, 255, 255, 255, 255, 255, 255, 255 }
  },
  {
    128,
    { 21, 34, 48, 70, 83, 97, 98, 255, 255, 255, 255, 255, 255, 255, 255 }
  }
};

void SpecialAttackBattleMessage(army *attacker, army *target, int creaturesKilled, int damageDone) {
  char *attackingCreature;
  if (creaturesKilled <= 0) {
    if (attacker->quantity <= 1)
      attackingCreature = GetCreatureName(attacker->creatureIdx);
    else
      attackingCreature = GetCreaturePluralName(attacker->creatureIdx);
    sprintf(gText, "%s %s %d damage.", attackingCreature, (attacker->quantity > 1) ? "do" : "does", damageDone);
    gText[0] = toupper(gText[0]);
  }
  else {
    if (damageDone == -1) {
      sprintf(gText, "The mirror image is destroyed!");
    } else {
      char *targetCreature;
      if (creaturesKilled <= 1)
        targetCreature = GetCreatureName(target->creatureIdx);
      else
        targetCreature = GetCreaturePluralName(target->creatureIdx);
      if (attacker->quantity <= 1)
        attackingCreature = GetCreatureName(attacker->creatureIdx);
      else
        attackingCreature = GetCreaturePluralName(attacker->creatureIdx);
      sprintf(
        gText,
        "%s %s %d damage.\n%d %s %s.",
        attackingCreature,
        (attacker->quantity > 1) ? "do" : "does",
        damageDone,
        creaturesKilled,
        targetCreature,
        (creaturesKilled > 1) ? "perish" : "perishes");
      gText[0] = toupper(gText[0]);
    }
  }
  gpCombatManager->CombatMessage(gText, 1, 1, 0);
}

void OccupyHexes(army *a) {
  if (a->facingRight == 1)
    a->occupiedHex--;
  else
    a->occupiedHex++;
}

void ProcessSecondAttack(army *attacker, army *target) {
  if (!bSecondAttack && target->quantity > 0) {
    bSecondAttack = 1;
    attacker->SpecialAttack();
    bSecondAttack = 0;
  }
}

void SpecialAttackGraphics(army *attacker, army *target) {
  gpCombatManager->ResetLimitCreature();
  gpCombatManager->limitCreature[attacker->owningSide][attacker->stackIdx]++;
  gpCombatManager->DrawFrame(0, 1, 0, 1, 75, 1, 1);

  int targMidX = target->MidX();
  int targMidY = target->MidY();
  if (attacker->creatureIdx == CREATURE_LICH || attacker->creatureIdx == CREATURE_POWER_LICH)  {
    targMidX = gpCombatManager->combatGrid[target->occupiedHex].centerX;
    targMidY = gpCombatManager->combatGrid[target->occupiedHex].occupyingCreatureBottomY - 17;
  }
  int projStartX;
  if (attacker->facingRight == 1)
    projStartX = attacker->frameInfo.projectileStartOffset[1][0] + gpCombatManager->combatGrid[attacker->occupiedHex].centerX;
  else
    projStartX = gpCombatManager->combatGrid[attacker->occupiedHex].centerX - attacker->frameInfo.projectileStartOffset[1][0];
  int projStartY = attacker->frameInfo.projectileStartOffset[1][1] + gpCombatManager->combatGrid[attacker->occupiedHex].occupyingCreatureBottomY;
  int totXDiff = targMidX - projStartX;
  char firingLeft = 0;
  if (targMidX - projStartX < 0) {
    firingLeft = 1;
    totXDiff = -totXDiff;
  }
  int yDiff = targMidY - projStartY;

  float angleDeg;
  int spriteIdx;
  if (totXDiff) {
    float slope = (double)-yDiff / (double)totXDiff;
    angleDeg = atan(slope) * 180.0 / 3.14159;
    int i;
    for (i = 1;  attacker->frameInfo.numMissileDirs > i &&
      (*(float *)((char *)&attacker->frameInfo.projectileStartOffset[i + 2] + 1) +
        attacker->frameInfo.projDirAngle[i]) / 2.0 >= angleDeg;  ++i)
      ;
    if (attacker->frameInfo.numMissileDirs <= i)
      spriteIdx = attacker->frameInfo.numMissileDirs - 1;
    else
      spriteIdx = i - 1;
  } else {
    if (yDiff <= 0)
      spriteIdx = 0;
    else
      spriteIdx = attacker->frameInfo.numMissileDirs - 1;
    angleDeg = (double)(yDiff <= 0 ? 90 : -90);
  }

  int attackDirectionAnimationIdx;
  if (angleDeg <= 25.0) {
    if (angleDeg <= -25.0) {
      attacker->animationType = ANIMATION_TYPE_RANGED_ATTACK_DOWNWARDS;
      attackDirectionAnimationIdx = 2;
    }
    else {
      attacker->animationType = ANIMATION_TYPE_RANGED_ATTACK_FORWARDS;
      attackDirectionAnimationIdx = 1;
    }
  } else {
    attacker->animationType = ANIMATION_TYPE_RANGED_ATTACK_UPWARDS;
    attackDirectionAnimationIdx = 0;
  }
  for (attacker->animationFrame = 0;
    attacker->frameInfo.animationLengths[attacker->animationType] > attacker->animationFrame;
    ++attacker->animationFrame)
  {
    if (attacker->frameInfo.animationLengths[attacker->animationType] - 1 == attacker->animationFrame)
      gpCombatManager->DrawFrame(0, 1, 0, 0, 75, 1, 1);
    else
      gpCombatManager->DrawFrame(1, 1, 0, 0, 75, 1, 1);
    glTimers = (signed __int64)((double)KBTickCount()
      + (double)attacker->frameInfo.shootingTime
      * gfCombatSpeedMod[giCombatSpeed]
      / (double)attacker->frameInfo.animationLengths[attacker->animationType]);
  }
  attacker->animationFrame = attacker->frameInfo.animationLengths[attacker->animationType] - 1;
  int v27 = 25;
  int v18 = 25;
  int v35 = 31;
  int v22 = 25;
  if (attacker->creatureIdx == CREATURE_LICH || attacker->creatureIdx == CREATURE_POWER_LICH) {
    v35 = 26;
    v22 = 7;
    v27 = 10;
    v18 = 10;
  }
  if (attacker->creatureIdx == CREATURE_CYBER_BEHEMOTH) {
    v27 = 100;
    v18 = 100;
    v35 = 31;
    v22 = 25;
  }
  int v20 = 0;
  int offsetX = 639;
  int v15 = 0;
  int offsetY = 479;
  int startX;
  if (attacker->facingRight == 1)
    startX = attacker->frameInfo.projectileStartOffset[attackDirectionAnimationIdx][0] + gpCombatManager->combatGrid[attacker->occupiedHex].centerX;
  else
    startX = gpCombatManager->combatGrid[attacker->occupiedHex].centerX - attacker->frameInfo.projectileStartOffset[attackDirectionAnimationIdx][0];
  
  int startY = attacker->frameInfo.projectileStartOffset[attackDirectionAnimationIdx][1] + gpCombatManager->combatGrid[attacker->occupiedHex].occupyingCreatureBottomY;

  int endX = target->MidX();
  int endY = target->MidY();
  int diffX = endX - startX;
  int diffY = endY - startY;
  int distance = (signed __int64)sqrt((double)(diffY * diffY + diffX * diffX));
  int v52 = (distance + (v35 / 2)) / v35;

  if (attacker->creatureIdx == CREATURE_MAGE || attacker->creatureIdx == CREATURE_ARCHMAGE) {
    gpWindowManager->UpdateScreenRegion(giMinExtentX, giMinExtentY, giMaxExtentX - giMinExtentX + 1, giMaxExtentY - giMinExtentY + 1);
    DelayMilli((signed __int64)(gfCombatSpeedMod[giCombatSpeed] * 115.0));
    gpCombatManager->DoBolt(1, startX, startY, endX, endY, 0, 0, 5, 4, 302, 0, 0, distance / 15 + 15, 1, 0, 10, 0);
  } else if (attacker->creatureIdx == CREATURE_CYBER_BEHEMOTH) {
    gpCombatManager->ArcShot(attacker->missileIcon, startX, startY, endX, endY);
  } else {  
    int v37;
    int v43;
    if (v52 <= 1) {
      v43 = diffX;
      v37 = diffY;
    } else {
      v43 = diffX / (v52 - 1);
      v37 = diffY / (v52 - 1);
    }
    int v44 = startX;
    int v38 = startY;
    //from = (bitmap *)operator new(26);
    bitmap *from = nullptr;
    from = new bitmap(33, 2 * v27, 2 * v18);
    from->GrabBitmapCareful(gpWindowManager->screenBuffer, v44 - v27, v38 - v18);
    int v59 = v44;
    int v53 = v38;
    int x = 0;
    int y = 0;
    for (int i = 0; i < v52; ++i) {
      if (v59 - v27 < offsetX)
        offsetX = v59 - v27;
      if (offsetX < 0)
        offsetX = 0;
      if (v27 + v59 > v20)
        v20 = v27 + v59;
      if (v20 > 639)
        v20 = 639;
      if (v53 - v18 < offsetY)
        offsetY = v53 - v18;
      if (offsetY < 0)
        offsetY = 0;
      if (v18 + v53 > v15)
        v15 = v18 + v53;
      if (v15 > 442)
        v15 = 442;
      if (i) {
        from->DrawToBufferCareful(x, y);
      } else {
        if (giMinExtentX > offsetX)
          giMinExtentX = offsetX;
        if (v20 > giMaxExtentX)
          giMaxExtentX = v20;
        if (offsetY < giMinExtentY)
          giMinExtentY = offsetY;
        if (v15 > giMaxExtentY)
          giMaxExtentY = v15;
      }
      x = v44 - v27;
      if (v44 - v27 < 0)
        x = 0;
      if (x + (signed int)from->width > 640)
        x = 640 - from->width;
      y = v38 - v18;
      if (v38 - v18 < 0)
        y = 0;
      if (y + (signed int)from->height > 640)
        y = 640 - from->height;
      from->GrabBitmapCareful(gpWindowManager->screenBuffer, x, y);
      attacker->missileIcon->DrawToBuffer(v44, v38, spriteIdx, firingLeft);
      if (i) {
        DelayTil(&glTimers);
        gpWindowManager->UpdateScreenRegion(offsetX, offsetY, v20 - offsetX + 1, v15 - offsetY + 1);
      } else {
        gpWindowManager->UpdateScreenRegion(giMinExtentX, giMinExtentY, giMaxExtentX - giMinExtentX + 1, giMaxExtentY - giMinExtentY + 1);
      }

      glTimers = (signed __int64)((double)KBTickCount() + (double)v22 * gfCombatSpeedMod[giCombatSpeed]);
      v59 = v44;
      v53 = v38;
      v44 += v43;
      v38 += v37;
      offsetX = v44 - v27;
      v20 = v27 + v44;
      offsetY = v38 - v18;
      v15 = v18 + v38;
    }
    from->DrawToBuffer(x, y);
    gpWindowManager->UpdateScreenRegion(v59 - v27, v53 - v18, 2 * v27, 2 * v18);
    if (from)
      from->~bitmap();
  }    
}

void army::SpecialAttack() {
  int damageDone = 0;
  int creaturesKilled = 0;
  this->field_125 = 0;
  army *target = &gpCombatManager->creatures[this->targetOwner][this->targetStackIdx];
  char targetColumnHex = target->occupiedHex % 13;
  char targetRowHex = target->occupiedHex / 13; // unused
  char sourceColumnHex = this->occupiedHex % 13;
  char sourceRowHex = this->occupiedHex / 13;
  int tmpFacingRight = this->facingRight;
  this->facingRight = targetColumnHex > sourceColumnHex || !(sourceRowHex & 1) && targetColumnHex == sourceColumnHex;

  if (this->facingRight != tmpFacingRight) {
    if (this->creature.creature_flags & TWO_HEXER) {
      OccupyHexes(this);
    }
    gpCombatManager->DrawFrame(1, 0, 0, 0, 75, 1, 1);
  }

  this->CheckLuck();
  gpSoundManager->MemorySample(this->combatSounds[3]);

  SpecialAttackGraphics(this, target);

  // Decrease the number of shots left
  if (!gpCombatManager->heroes[this->owningSide] || !gpCombatManager->heroes[this->owningSide]->HasArtifact(ARTIFACT_AMMO_CART))
    --this->creature.shots;

  int animIdx = -1;
  int a4 = -1;
  int a5 = -1;
    
  if (this->creatureIdx == CREATURE_LICH || this->creatureIdx == CREATURE_POWER_LICH) {
    int possibleTarget;
    gpCombatManager->ClearEffects();
    for (int i = 0; i < 7; ++i)  {
      if (i >= 6)
        possibleTarget = target->occupiedHex;
      else
        possibleTarget = target->GetAdjacentCellIndex(target->occupiedHex, i);
      if (possibleTarget != -1) {
        hexcell *targetHex = &gpCombatManager->combatGrid[possibleTarget];
        if (targetHex->unitOwner != -1) {
          army *targ = &gpCombatManager->creatures[targetHex->unitOwner][targetHex->stackIdx];
          if (!gArmyEffected[targ->owningSide][targ->stackIdx]) {
            if (target != targ || i == 6) {
              gArmyEffected[targ->owningSide][targ->stackIdx] = 1;
              this->DamageEnemy(targ, &damageDone, &creaturesKilled, 1, 0);
            }
          }
        }
      }
    }
    this->field_FA = 0;
    animIdx = 20;
    a4 = gpCombatManager->combatGrid[possibleTarget].centerX;
    a5 = gpCombatManager->combatGrid[possibleTarget].occupyingCreatureBottomY - 17;
    gpSoundManager->MemorySample(combatSounds[5]);
  } else if (CreatureHasAttribute(this->creatureIdx, PLASMA_BLAST)) {

    int cyberBehemothAttackMask[] = {
      -27,-26,-25,
      -14,-13,-12,-11,
      -2,-1,1,2,
      12,13,14,15,
      25,26,27
    };

    gpCombatManager->ClearEffects();

    int possibleTarget;
    for (int j = 0; j < 18; j++) {
      possibleTarget = target->occupiedHex + cyberBehemothAttackMask[j];
      if (possibleTarget >= 0 && possibleTarget < 116) {
        hexcell *targetHex = &gpCombatManager->combatGrid[possibleTarget];
        if (targetHex->unitOwner != -1) {
          army *targ = &gpCombatManager->creatures[targetHex->unitOwner][targetHex->stackIdx];
          if (!gArmyEffected[targ->owningSide][targ->stackIdx]) {
            if (target != targ) {
              gArmyEffected[targ->owningSide][targ->stackIdx] = 1;
              this->DamageEnemy(targ, &damageDone, &creaturesKilled, 1, 0);
            }
          }
        }
      }
    }
    possibleTarget = target->occupiedHex;
    if (possibleTarget != -1) {
      hexcell *targetHex = &gpCombatManager->combatGrid[possibleTarget];
      if (targetHex->unitOwner != -1) {
        army *targ = &gpCombatManager->creatures[targetHex->unitOwner][targetHex->stackIdx];
        if (!gArmyEffected[targ->owningSide][targ->stackIdx]) {
          gArmyEffected[targ->owningSide][targ->stackIdx] = 1;
          this->DamageEnemy(targ, &damageDone, &creaturesKilled, 1, 0);
        }
      }
    }
    this->field_FA = 0;
    animIdx = 32;
    a4 = gpCombatManager->combatGrid[target->occupiedHex].centerX;
    a5 = gpCombatManager->combatGrid[target->occupiedHex].occupyingCreatureBottomY - 17;
    //gpSoundManager->MemorySample(combatSounds[5]);
  } else {
    this->DamageEnemy(target, &damageDone, &creaturesKilled, 1, 0);
  }

  SpecialAttackBattleMessage(this, target, creaturesKilled, damageDone);

  if (this->creatureIdx == CREATURE_ARCHMAGE) {
    if (SRandom(1, 100) < 20) {
      if (target)  {
        if (target->SpellCastWorks(SPELL_ARCHMAGI_DISPEL))
          target->spellEnemyCreatureAbilityIsCasting = 102;
      }
    }
  }

  this->PowEffect(animIdx, 0, a4, a5);

  if (this->facingRight != tmpFacingRight) {
    if (this->creature.creature_flags & TWO_HEXER) {
      OccupyHexes(this);
    }
    this->facingRight = tmpFacingRight;
  }

  if (this->creatureIdx == CREATURE_ELF || this->creatureIdx == CREATURE_GRAND_ELF || this->creatureIdx == CREATURE_RANGER)
    ProcessSecondAttack(this, target);
  
  // Block mind spells
  if (this->effectStrengths[5] || this->effectStrengths[7]) {
    this->CancelSpellType(1);
    gpCombatManager->DrawFrame(1, 0, 0, 0, 75, 1, 1);
  }
}

void army::LoadResources() {
  if (!gbNoShowCombat) {
    int creatureID = this->creatureIdx;
    int formFileID = gpResourceManager->MakeId(cArmyFrameFileNames[creatureID], 1);
    gpResourceManager->PointToFile(formFileID);
    gpResourceManager->ReadBlock((signed char*)&this->frameInfo, 821u);
    ModifyFrameInfo(&this->frameInfo, (CREATURES)creatureID);
    this->field_B2 = this->frameInfo.stepTime;

    std::string shortName = this->creature.short_name;
    this->combatSounds[0] = gpResourceManager->GetSample(shortName + "move.82M");
    this->combatSounds[1] = gpResourceManager->GetSample(shortName + "attk.82M");
    this->combatSounds[2] = gpResourceManager->GetSample(shortName + "wnce.82M");
    this->combatSounds[4] = gpResourceManager->GetSample(shortName + "kill.82M");
    if (this->creature.creature_flags & SHOOTER) {
      this->combatSounds[3] = gpResourceManager->GetSample(shortName + "shot.82M");
    }
    switch (creatureID) {
      case CREATURE_VAMPIRE: case CREATURE_VAMPIRE_LORD: {
        this->combatSounds[5] = gpResourceManager->GetSample(shortName + "ext1.82M");
        this->combatSounds[6] = gpResourceManager->GetSample(shortName + "ext2.82M");
        break;
      }
      case CREATURE_LICH: case CREATURE_POWER_LICH: {
        this->combatSounds[5] = gpResourceManager->GetSample(shortName + "expl.82M");
      }
    }
    
    this->creatureIcon = gpResourceManager->GetIcon(cMonFilename[creatureID]);

    // Loading projectiles
    if (this->creature.creature_flags & SHOOTER) {
      if(!strlen(cArmyProjectileFileNames[creatureID]))
        this->missileIcon = gpResourceManager->GetIcon("elf__msl.icn");
      else
        this->missileIcon = gpResourceManager->GetIcon(cArmyProjectileFileNames[creatureID]);
    } else {
      this->combatSounds[3] = 0;
      this->missileIcon = 0;
    }

    for (int i = 0; i < 5; ++i) {
      if (this->combatSounds[i]) {
        this->combatSounds[i]->field_28 = 64;
        this->combatSounds[i]->codeThing = 3;
        this->combatSounds[i]->loopCount = 1;
      }
    }
  }
}

void army::PowEffect(int animIdx, int a3, int a4, int a5)
{
  int specialCaseOverlapWeirdness = 1;
  if (this->creatureIdx == CREATURE_PALADIN || this->creatureIdx == CREATURE_CRUSADER)
    specialCaseOverlapWeirdness = 0;
  if (this->creatureIdx == CREATURE_DWARF || this->creatureIdx == CREATURE_BATTLE_DWARF)
    specialCaseOverlapWeirdness = 2;

  bool doCreatureEffect = false;
  
  if (a4 == -1) {
    if (animIdx != -1) {
      for (int i = 0; i < 2; i++)  {
        for (int j = 0; gpCombatManager->numCreatures[i] > j; j++) {
          if (gpCombatManager->creatures[i][j].probablyIsNeedDrawSpellEffect)
            doCreatureEffect = true;
        }
      }
    }
  } else {
    doCreatureEffect = true;
  }

  if (!gbNoShowCombat && animIdx != -1 && doCreatureEffect && animIdx != gCurLoadedSpellEffect) {
    gpResourceManager->Dispose((resource *)gCurLoadedSpellIcon);
    gCurLoadedSpellIcon = gpResourceManager->GetIcon(gCombatFxNames[animIdx]);
    gCurLoadedSpellEffect = animIdx;
  }

  int oneWayAnimLen = 0;
  int fromAnimLen = 0;
  int toAnimLen = 0;
  int maxOneWayAnimLen = 0;
  int maxFromAnimLen = 0;
  int maxToAnimLen = 0;
  int creatureEffectNumFrames = 0;
  if (doCreatureEffect)
    creatureEffectNumFrames = giNumPowFrames[gCurLoadedSpellEffect];

  for (int i = 0; i < 2; i++)  {
    for (int j = 0; gpCombatManager->numCreatures[i] > j; j++) {
      army *othstack = &gpCombatManager->creatures[i][j];
      if (othstack->mightBeIsAttacking) {
        toAnimLen = othstack->frameInfo.animationLengths[this->mightBeAttackAnimIdx];
        fromAnimLen = othstack->frameInfo.animationLengths[this->mightBeAttackAnimIdx + 1] + 1;
      } else if (othstack->dead) {
        oneWayAnimLen = othstack->frameInfo.animationLengths[13];
      } else if (othstack->damageTakenDuringSomeTimePeriod) {
        oneWayAnimLen = othstack->frameInfo.animationLengths[15] + othstack->frameInfo.animationLengths[14] + 1;
      }
      if (maxToAnimLen <= toAnimLen)
        maxToAnimLen = toAnimLen;
      if (maxFromAnimLen <= fromAnimLen)
        maxFromAnimLen = fromAnimLen;
      if (maxOneWayAnimLen <= oneWayAnimLen)
        maxOneWayAnimLen = oneWayAnimLen;
    }
  }
  int maxAnyCreatureAnimLen = maxOneWayAnimLen + maxToAnimLen - specialCaseOverlapWeirdness;
  if (maxAnyCreatureAnimLen <= maxFromAnimLen + maxToAnimLen)
    maxAnyCreatureAnimLen = maxFromAnimLen + maxToAnimLen;

  int maxAnyCreatureAnimLena = maxOneWayAnimLen;
  if (maxAnyCreatureAnimLena <= maxAnyCreatureAnimLen)
    maxAnyCreatureAnimLena = maxAnyCreatureAnimLen;

  int maxAnimLen = creatureEffectNumFrames;
  if (maxAnimLen <= maxAnyCreatureAnimLena)
    maxAnimLen = maxAnyCreatureAnimLena;
  
  if (a3)
    gpCombatManager->ResetLimitCreature();

  for (int i = 0; i < 2; i++) {
    for (int j = 0; gpCombatManager->numCreatures[i] > j; j++) {
      army *creature = &gpCombatManager->creatures[i][j];
      int animType = creature->animationType;
      if(animType == ANIMATION_TYPE_RANGED_ATTACK_UPWARDS || animType == ANIMATION_TYPE_RANGED_ATTACK_FORWARDS || animType == ANIMATION_TYPE_RANGED_ATTACK_DOWNWARDS)
        creature->animatingRangedAttack = true;
      else
        creature->animatingRangedAttack = false;
      if ((creature->damageTakenDuringSomeTimePeriod || creature->mightBeIsAttacking || creature->animatingRangedAttack) && !gpCombatManager->limitCreature[i][j])
        gpCombatManager->limitCreature[i][j]++;
    }
  }
  gpCombatManager->DrawFrame(0, 1, 0, 1, 75, 1, 1);
  if (a4 != -1) {
    for (int i = 0; gCurLoadedSpellIcon->numSprites > i; i++) {
      IconEntry *animICNHeader = &gCurLoadedSpellIcon->headersAndImageData[i];
      giMinExtentX = a4 + animICNHeader->offsetX;
      if (giMinExtentX >= giMinExtentX)
        giMinExtentX = giMinExtentX;
      giMinExtentY = a5 + animICNHeader->offsetY;
      if (giMinExtentY >= giMinExtentY)
        giMinExtentY = giMinExtentY;
      giMaxExtentX = a4 + animICNHeader->offsetX + animICNHeader->width - 1;
      if (giMaxExtentX <= giMaxExtentX)
        giMaxExtentX = giMaxExtentX;
      giMaxExtentY = a5 + animICNHeader->height + animICNHeader->offsetY - 1;
      if (giMaxExtentY <= giMaxExtentY)
        giMaxExtentY = giMaxExtentY;
    }
    if (giMinExtentX <= 0)
      giMinExtentX = 0;
    if (giMinExtentY <= 0)
      giMinExtentY = 0;
    if (giMaxExtentX >= 639)
      giMaxExtentX = 639;
    if (giMaxExtentY >= 442)
      giMaxExtentY = 442;
  }
  for (int i = 0; i < 2; i++) {
    for (int j = 0; gpCombatManager->numCreatures[i] > j; j++) {
      army *creature = &gpCombatManager->creatures[i][j];
      creature->field_3 = -1;
      creature->field_4 = -1;
      creature->effectStrengths[15] = 0;
      if (creature->damageTakenDuringSomeTimePeriod || creature->mightBeIsAttacking) {
        if (creature->mightBeIsAttacking) {
          creature->field_3 = this->mightBeAttackAnimIdx;
          creature->field_4 = this->mightBeAttackAnimIdx + 1;
        } else if (creature->dead) {
          creature->field_3 = ANIMATION_TYPE_DYING;
        } else {
          creature->field_3 = ANIMATION_TYPE_WINCE;
          creature->field_4 = ANIMATION_TYPE_WINCE_RETURN;
        }
        if (creature->field_3 == 13)
          creature->field_5 = creature->frameInfo.animationLengths[13];
        else
          creature->field_5 = creature->frameInfo.animationLengths[creature->field_3]
          + creature->frameInfo.animationLengths[creature->field_3 + 1];
        if (creature->field_3 == creature->animationType)
          --creature->field_5;
        if (this->field_6 < 2)
          this->field_6 = 2;
      }
    }
  }
  for (int k = 0; maxAnimLen > k; k++) {
    for (int i = 0; i < 2; i++) {
      for (int j = 0; gpCombatManager->numCreatures[i] > j; j++) {
        army *creature = &gpCombatManager->creatures[i][j];
        
        if (creature->animatingRangedAttack) {
          if (creature->animationType != ANIMATION_TYPE_RANGED_ATTACK_UPWARDS
            && creature->animationType != ANIMATION_TYPE_RANGED_ATTACK_FORWARDS
            && creature->animationType != ANIMATION_TYPE_RANGED_ATTACK_DOWNWARDS) {
            if (creature->animationType != ANIMATION_TYPE_STANDING) {
              if (creature->frameInfo.animationLengths[creature->animationType] <= creature->animationFrame + 1) {
                creature->animationType = ANIMATION_TYPE_STANDING;
                creature->animationFrame = 0;
              } else {
                ++creature->animationFrame;
              }
            }
          } else {
            ++creature->animationType;
            creature->animationFrame = 0;
          }
        }
        if (creature->field_3 != -1
          && !creature->effectStrengths[15]
          && (creature->mightBeIsAttacking
            || creature->field_5 >= maxAnimLen - k - 1
            || maxToAnimLen && maxToAnimLen - 1 <= k
            || !maxToAnimLen
            && creature->animationType != ANIMATION_TYPE_WINCE_RETURN
            && (creature->animationType != ANIMATION_TYPE_WINCE
              || creature->frameInfo.animationLengths[creature->animationType] > creature->animationFrame + 1))) {
          if (creature->field_3 == creature->animationType || creature->field_4 == creature->animationType) {
            if (creature->frameInfo.animationLengths[creature->animationType] <= creature->animationFrame + 1) {
              if (creature->field_4 == creature->animationType || creature->field_4 == -1) {
                if (creature->animationType != ANIMATION_TYPE_STANDING && creature->animationType != ANIMATION_TYPE_DYING) {
                  creature->animationType = ANIMATION_TYPE_STANDING;
                  creature->animationFrame = 0;
                  creature->effectStrengths[15] = 1;
                }
              } else {
                creature->animationType = creature->field_4;
                creature->animationFrame = 0;
              }
            } else {
              creature->animationFrame++;
            }
          } else {
            if (!gbNoShowCombat && creature->field_3 == 14)
              gpSoundManager->MemorySample(creature->combatSounds[2]);
            if (!gbNoShowCombat && creature->field_3 == 13)
              gpSoundManager->MemorySample(creature->combatSounds[4]);
            creature->animationType = creature->field_3;
            creature->animationFrame = 0;
          }
        }
      }
    }
    glTimers = (signed __int64)((double)KBTickCount() + (double)120 * gfCombatSpeedMod[giCombatSpeed]);
    if (doCreatureEffect && giNumPowFrames[gCurLoadedSpellEffect] > k)
      gCurSpellEffectFrame = k;
    gpCombatManager->DrawFrame(0, 0, 0, 0, 75, 1, 1);
    if (a4 != -1 && giNumPowFrames[gCurLoadedSpellEffect] > k)
      gCurLoadedSpellIcon->CombatClipDrawToBuffer(a4,  a5 + this->field_FA, gCurSpellEffectFrame, &this->effectAnimationBounds, 0, 0, 0, 0);
    gpWindowManager->UpdateScreenRegion(0, 0, INTERNAL_WINDOW_WIDTH, INTERNAL_WINDOW_HEIGHT);
    DelayTil(&glTimers);
  }

  for (int i = 0; i < 2; i++) {
    for (int j = 0; gpCombatManager->numCreatures[i] > j; j++) {
      army *creature = &gpCombatManager->creatures[i][j];
      if (creature->damageTakenDuringSomeTimePeriod && creature->spellEnemyCreatureAbilityIsCasting != -1 && creature->spellEnemyCreatureAbilityIsCasting != 101) {
        gpCombatManager->CastSpell((Spell)creature->spellEnemyCreatureAbilityIsCasting, creature->occupiedHex, 1, -1);
        creature->spellEnemyCreatureAbilityIsCasting = -1;
      }
    }
  }

  bool v41 = true;
  while (v41)
  {
    v41 = false;
    for (int i = 0; i < 2; i++) {
      for (int j = 0; gpCombatManager->numCreatures[i] > j; j++) {
        army *thisf = &gpCombatManager->creatures[i][j];
        int animType = thisf->animationType;
        if (animType != ANIMATION_TYPE_WINCE
          && animType != ANIMATION_TYPE_MELEE_ATTACK_UPWARDS
          && animType != ANIMATION_TYPE_MELEE_ATTACK_FORWARDS
          && animType != ANIMATION_TYPE_MELEE_ATTACK_DOWNWARDS
          && animType != ANIMATION_TYPE_TWO_HEX_ATTACK_UPWARDS
          && animType != ANIMATION_TYPE_TWO_HEX_ATTACK_FORWARDS
          && animType != ANIMATION_TYPE_TWO_HEX_ATTACK_DOWNWARDS
          && animType != ANIMATION_TYPE_RANGED_ATTACK_UPWARDS
          && animType != ANIMATION_TYPE_RANGED_ATTACK_FORWARDS
          && animType != ANIMATION_TYPE_RANGED_ATTACK_DOWNWARDS) {
          if (animType == ANIMATION_TYPE_DYING
            || animType == ANIMATION_TYPE_WINCE_RETURN
            || animType == ANIMATION_TYPE_MELEE_ATTACK_UPWARDS_RETURN
            || animType == ANIMATION_TYPE_MELEE_ATTACK_FORWARDS_RETURN
            || animType == ANIMATION_TYPE_MELEE_ATTACK_DOWNWARDS_RETURN
            || animType == ANIMATION_TYPE_TWO_HEX_ATTACK_UPWARDS_RETURN
            || animType == ANIMATION_TYPE_TWO_HEX_ATTACK_FORWARDS_RETURN
            || animType == ANIMATION_TYPE_TWO_HEX_ATTACK_DOWNWARDS_RETURN
            || animType == ANIMATION_TYPE_RANGED_ATTACK_UPWARDS_RETURN
            || animType == ANIMATION_TYPE_RANGED_ATTACK_FORWARDS_RETURN
            || animType == ANIMATION_TYPE_RANGED_ATTACK_DOWNWARDS_RETURN) {
            if (thisf->frameInfo.animationLengths[animType] <= thisf->animationFrame + 1) {
              if (animType != ANIMATION_TYPE_DYING) {
                thisf->animationType = ANIMATION_TYPE_STANDING;
                thisf->animationFrame = 0;
                v41 = true;
              }
            } else {
              thisf->animationFrame++;
              v41 = true;
            }
          }
        } else {
          thisf->animationType++;
          thisf->animationFrame = 0;
          v41 = true;
        }
      }
    }
    if (v41) {
      glTimers = (signed __int64)((double)KBTickCount() + (double)120 * gfCombatSpeedMod[giCombatSpeed]);
      gpCombatManager->DrawFrame(1, 1, 0, 0, 75, 1, 1);
    }
  }
  if (a3)
    gpCombatManager->ResetLimitCreature();
  memset(gpCombatManager->shouldVanish, 0, 0x28u);
  gpCombatManager->anyStacksShouldVanish = 0;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; gpCombatManager->numCreatures[i] > j; j++) {
      if (gpCombatManager->creatures[i][j].dead)
        gpCombatManager->creatures[i][j].ProcessDeath(0);
    }
  }
  if (gpCombatManager->anyStacksShouldVanish)
    gpCombatManager->MakeCreaturesVanish();
  for (int i = 0; i < 2; i++) {
    for (int j = 0; gpCombatManager->numCreatures[i] > j; j++) {
      army *creature = &gpCombatManager->creatures[i][j];
      if (creature->damageTakenDuringSomeTimePeriod && creature->spellEnemyCreatureAbilityIsCasting == 101) {
        gpCombatManager->CastSpell((Spell)creature->spellEnemyCreatureAbilityIsCasting, creature->occupiedHex, 1, -1);
        creature->spellEnemyCreatureAbilityIsCasting = -1;
      }
      creature->probablyIsNeedDrawSpellEffect = 0;
      creature->damageTakenDuringSomeTimePeriod = 0;
      creature->hasTakenLosses = 0;
      creature->field_6 = 1;
      creature->mightBeIsAttacking = 0;
      creature->previousQuantity = -1;
    }
  }
  gpCombatManager->DrawFrame(1, 0, 0, 0, 75, 1, 1);
}

void combatManager::ArcShot(icon *icn, int fromX, int fromY, int targX, int targY)
{
  bool firingLeft = false;
  if (fromX > targX)
    firingLeft = true;
  float boulderX = fromX;
  float boulderY = fromY;
  float amplitude = 0.01282;
  float v33 = (double)((targX + fromX) / 2.);
  float stepX = (v33 - (double)fromX) / 12.5;
  float v32 = (double)targY - (double)(targX - fromX) * 0.3 - (double)targY * 0.35;
  if(firingLeft)
      v32 = (double)targY - (double)(fromX - targX) * 0.3 - (double)targY * 0.35;
  float stepY = (v32 - (double)fromY) * amplitude;
  int oldX = -1;
  int oldY = -1;
  int imageIdx = 0; // changes the sprite when its angle changes

  // temporarily save the screen so we can clear it from the projectile sprite later
  bitmap *savedscreen = new bitmap(0, INTERNAL_WINDOW_WIDTH, INTERNAL_WINDOW_HEIGHT);
  gpWindowManager->screenBuffer->CopyTo(savedscreen, 0, 0, 0, 0, INTERNAL_WINDOW_WIDTH, INTERNAL_WINDOW_HEIGHT);

  const int NUM_CYCLES = 24; // equals to the number of frames for the whole arc path
  for (int i = 0; i <= NUM_CYCLES; i++) {
    if (i == (NUM_CYCLES / 2)) // reached the peak height
      stepY = (v32 - (double)targY) * amplitude;
    if (i % 3 == 0) // every 3rd cycle
      if(imageIdx < (icn->numSprites - 1))
        imageIdx++; // changes the sprite of projectile

    savedscreen->CopyTo(gpWindowManager->screenBuffer, 0, 0, 0, 0, INTERNAL_WINDOW_WIDTH, INTERNAL_WINDOW_HEIGHT); // clear the screen from the previous projectile sprite
    icn->CombatClipDrawToBuffer((int)boulderX, (int)boulderY, imageIdx, NULL, firingLeft, 0, 0, 0);
    gpWindowManager->UpdateScreenRegion(0, 0, INTERNAL_WINDOW_WIDTH, INTERNAL_WINDOW_HEIGHT);
    
    oldX = (int)boulderX;
    oldY = (int)boulderY;
    boulderX += stepX;
    boulderY += (double)((NUM_CYCLES / 2) - i) * stepY;
    
    glTimers = (signed __int64)((double)KBTickCount() + (double)40 * gfCombatSpeedMod[giCombatSpeed]);
    DelayTil(&glTimers);
  }
  delete savedscreen;
  this->DrawFrame(1, 0, 0, 0, 1, 1, 1); // clear screen from the projectile
}