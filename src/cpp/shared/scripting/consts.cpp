extern "C" {
#include "lua/src/lua.h"
#include "lua/src/lualib.h"
#include "lua/src/lauxlib.h"
}

#include "artifacts.h"
#include "adventure/adv.h"
#include "adventure/map.h"
#include "combat/creatures.h"
#include "spell/spells.h"
#include "scripting/lua_utils.h"
#include "scripting/register.h"
#include "town/town.h"
#include "gui/dialog.h"


/******************************* GUI *****************************************************/

void set_dialog_consts(lua_State *L) {
	lua_setconst(L, "DIALOG_YES_NO", DIALOG_YES_NO);
	lua_setconst(L, "DIALOG_OKAY", DIALOG_OKAY);
	lua_setconst(L, "DIALOG_OR", DIALOG_OR);
	lua_setconst(L, "DIALOG_EMPTY", DIALOG_EMPTY);
}

void set_barriercolor_consts(lua_State *L){
	lua_setconst(L, "AQUA_BARRIER", AQUA_BARRIER);
	lua_setconst(L, "BLUE_BARRIER", BLUE_BARRIER);
	lua_setconst(L, "RED_BARRIER", RED_BARRIER);
	lua_setconst(L, "ORANGE_BARRIER", ORANGE_BARRIER);
	lua_setconst(L, "BROWN_BARRIER", BROWN_BARRIER);
	lua_setconst(L, "PURPLE_BARRIER", PURPLE_BARRIER);
	lua_setconst(L, "GOLD_BARRIER", GOLD_BARRIER);
	lua_setconst(L, "GREEN_BARRIER", GREEN_BARRIER);
}
void set_messageboxgroups_consts(lua_State *L) {

	lua_setconst(L, "IMAGE_EMPTY", IMAGE_EMPTY);
	lua_setconst(L, "IMAGE_WOOD", IMAGE_WOOD);
	lua_setconst(L, "IMAGE_MERCURY", IMAGE_MERCURY);
	lua_setconst(L, "IMAGE_ORE", IMAGE_ORE);
	lua_setconst(L, "IMAGE_SULFUR", IMAGE_SULFUR);
	lua_setconst(L, "IMAGE_CRYSTALS", IMAGE_CRYSTALS);
	lua_setconst(L, "IMAGE_GEMS", IMAGE_GEMS);
	lua_setconst(L, "IMAGE_GOLD", IMAGE_GOLD);
	lua_setconst(L, "IMAGE_GROUP_ARTIFACTS", IMAGE_GROUP_ARTIFACTS);
	lua_setconst(L, "IMAGE_GROUP_SPELLS", IMAGE_GROUP_SPELLS);
	lua_setconst(L, "IMAGE_GROUP_PLAYERS", IMAGE_GROUP_PLAYERS);
	lua_setconst(L, "IMAGE_LUCK", IMAGE_LUCK);
	lua_setconst(L, "IMAGE_BADLUCK", IMAGE_BADLUCK);
	lua_setconst(L, "IMAGE_GOOD_MORALE", IMAGE_GOOD_MORALE);
	lua_setconst(L, "IMAGE_BAD_MORALE", IMAGE_BAD_MORALE);
	lua_setconst(L, "IMAGE_EXP", IMAGE_EXP);
	lua_setconst(L, "IMAGE_GROUP_HERO", IMAGE_GROUP_HERO);
	lua_setconst(L, "IMAGE_GROUP_SECONDARY_SKILLS", IMAGE_GROUP_SECONDARY_SKILLS);
	lua_setconst(L, "IMAGE_GROUP_UNIT", IMAGE_GROUP_UNIT);
	lua_setconst(L, "IMAGE_GROUP_PRIMARY_SKILLS", IMAGE_GROUP_PRIMARY_SKILLS);
}

void set_gui_consts(lua_State *L) {
  set_dialog_consts(L);
  set_messageboxgroups_consts(L);
}

/************************************************************************************/

void set_spell_consts(lua_State *L) {
  lua_setconst(L, "SPELL_FIREBALL", SPELL_FIREBALL);
  lua_setconst(L, "SPELL_FIREBLAST", SPELL_FIREBLAST);
  lua_setconst(L, "SPELL_LIGHTNING_BOLT", SPELL_LIGHTNING_BOLT);
  lua_setconst(L, "SPELL_CHAIN_LIGHTNING", SPELL_CHAIN_LIGHTNING);
  lua_setconst(L, "SPELL_TELEPORT", SPELL_TELEPORT);
  lua_setconst(L, "SPELL_CURE", SPELL_CURE);
  lua_setconst(L, "SPELL_MASS_CURE", SPELL_MASS_CURE);
  lua_setconst(L, "SPELL_RESURRECT", SPELL_RESURRECT);
  lua_setconst(L, "SPELL_RESURRECT_TRUE", SPELL_RESURRECT_TRUE);
  lua_setconst(L, "SPELL_HASTE", SPELL_HASTE);
  lua_setconst(L, "SPELL_MASS_HASTE", SPELL_MASS_HASTE);
  lua_setconst(L, "SPELL_SLOW", SPELL_SLOW);
  lua_setconst(L, "SPELL_MASS_SLOW", SPELL_MASS_SLOW);
  lua_setconst(L, "SPELL_BLIND", SPELL_BLIND);
  lua_setconst(L, "SPELL_BLESS", SPELL_BLESS);
  lua_setconst(L, "SPELL_MASS_BLESS", SPELL_MASS_BLESS);
  lua_setconst(L, "SPELL_STONESKIN", SPELL_STONESKIN);
  lua_setconst(L, "SPELL_STEELSKIN", SPELL_STEELSKIN);
  lua_setconst(L, "SPELL_CURSE", SPELL_CURSE);
  lua_setconst(L, "SPELL_MASS_CURSE", SPELL_MASS_CURSE);
  lua_setconst(L, "SPELL_HOLY_WORD", SPELL_HOLY_WORD);
  lua_setconst(L, "SPELL_HOLY_SHOUT", SPELL_HOLY_SHOUT);
  lua_setconst(L, "SPELL_ANTI_MAGIC", SPELL_ANTI_MAGIC);
  lua_setconst(L, "SPELL_DISPEL_MAGIC", SPELL_DISPEL_MAGIC);
  lua_setconst(L, "SPELL_MASS_DISPEL", SPELL_MASS_DISPEL);
  lua_setconst(L, "SPELL_MAGIC_ARROW", SPELL_MAGIC_ARROW);
  lua_setconst(L, "SPELL_BERZERKER", SPELL_BERZERKER);
  lua_setconst(L, "SPELL_ARMAGEDDON", SPELL_ARMAGEDDON);
  lua_setconst(L, "SPELL_ELEMENTAL_STORM", SPELL_ELEMENTAL_STORM);
  lua_setconst(L, "SPELL_METEOR_SHOWER", SPELL_METEOR_SHOWER);
  lua_setconst(L, "SPELL_PARALYZE", SPELL_PARALYZE);
  lua_setconst(L, "SPELL_HYPNOTIZE", SPELL_HYPNOTIZE);
  lua_setconst(L, "SPELL_COLD_RAY", SPELL_COLD_RAY);
  lua_setconst(L, "SPELL_COLD_RING", SPELL_COLD_RING);
  lua_setconst(L, "SPELL_DISRUPTING_RAY", SPELL_DISRUPTING_RAY);
  lua_setconst(L, "SPELL_DEATH_RIPPLE", SPELL_DEATH_RIPPLE);
  lua_setconst(L, "SPELL_DEATH_WAVE", SPELL_DEATH_WAVE);
  lua_setconst(L, "SPELL_DRAGON_SLAYER", SPELL_DRAGON_SLAYER);
  lua_setconst(L, "SPELL_BLOOD_LUST", SPELL_BLOOD_LUST);
  lua_setconst(L, "SPELL_ANIMATE_DEAD", SPELL_ANIMATE_DEAD);
  lua_setconst(L, "SPELL_MIRROR_IMAGE", SPELL_MIRROR_IMAGE);
  lua_setconst(L, "SPELL_SHIELD", SPELL_SHIELD);
  lua_setconst(L, "SPELL_MASS_SHIELD", SPELL_MASS_SHIELD);
  lua_setconst(L, "SPELL_SUMMON_EARTH_ELEMENTAL", SPELL_SUMMON_EARTH_ELEMENTAL);
  lua_setconst(L, "SPELL_SUMMON_AIR_ELEMENTAL", SPELL_SUMMON_AIR_ELEMENTAL);
  lua_setconst(L, "SPELL_SUMMON_FIRE_ELEMENTAL", SPELL_SUMMON_FIRE_ELEMENTAL);
  lua_setconst(L, "SPELL_SUMMON_WATER_ELEMENTAL", SPELL_SUMMON_WATER_ELEMENTAL);
  lua_setconst(L, "SPELL_EARTHQUAKE", SPELL_EARTHQUAKE);
  lua_setconst(L, "SPELL_VIEW_MINES", SPELL_VIEW_MINES);
  lua_setconst(L, "SPELL_VIEW_RESOURCES", SPELL_VIEW_RESOURCES);
  lua_setconst(L, "SPELL_VIEW_ARTIFACTS", SPELL_VIEW_ARTIFACTS);
  lua_setconst(L, "SPELL_VIEW_TOWNS", SPELL_VIEW_TOWNS);
  lua_setconst(L, "SPELL_VIEW_HEROES", SPELL_VIEW_HEROES);
  lua_setconst(L, "SPELL_VIEW_ALL", SPELL_VIEW_ALL);
  lua_setconst(L, "SPELL_IDENTIFY", SPELL_IDENTIFY);
  lua_setconst(L, "SPELL_SUMMON_BOAT", SPELL_SUMMON_BOAT);
  lua_setconst(L, "SPELL_DIMENSION_DOOR", SPELL_DIMENSION_DOOR);
  lua_setconst(L, "SPELL_TOWN_GATE", SPELL_TOWN_GATE);
  lua_setconst(L, "SPELL_TOWN_PORTAL", SPELL_TOWN_PORTAL);
  lua_setconst(L, "SPELL_VISIONS", SPELL_VISIONS);
  lua_setconst(L, "SPELL_HAUNT", SPELL_HAUNT);
  lua_setconst(L, "SPELL_SET_EARTH_GUARDIAN", SPELL_SET_EARTH_GUARDIAN);
  lua_setconst(L, "SPELL_SET_AIR_GUARDIAN", SPELL_SET_AIR_GUARDIAN);
  lua_setconst(L, "SPELL_SET_FIRE_GUARDIAN", SPELL_SET_FIRE_GUARDIAN);
  lua_setconst(L, "SPELL_SET_WATER_GUARDIAN", SPELL_SET_WATER_GUARDIAN);
}

void set_artifact_consts(lua_State* L) {
  lua_setconst(L, "ARTIFACT_ULTIMATE_BOOK_OF_KNOWLEDGE", ARTIFACT_ULTIMATE_BOOK_OF_KNOWLEDGE);
  lua_setconst(L, "ARTIFACT_ULTIMATE_SWORD_OF_DOMINION", ARTIFACT_ULTIMATE_SWORD_OF_DOMINION);
  lua_setconst(L, "ARTIFACT_ULTIMATE_CLOAK_OF_PROTECTION", ARTIFACT_ULTIMATE_CLOAK_OF_PROTECTION);
  lua_setconst(L, "ARTIFACT_ULTIMATE_WAND_OF_MAGIC", ARTIFACT_ULTIMATE_WAND_OF_MAGIC);
  lua_setconst(L, "ARTIFACT_ULTIMATE_SHIELD", ARTIFACT_ULTIMATE_SHIELD);
  lua_setconst(L, "ARTIFACT_ULTIMATE_STAFF", ARTIFACT_ULTIMATE_STAFF);
  lua_setconst(L, "ARTIFACT_ULTIMATE_CROWN", ARTIFACT_ULTIMATE_CROWN);
  lua_setconst(L, "ARTIFACT_GOLDEN_GOOSE", ARTIFACT_GOLDEN_GOOSE);
  lua_setconst(L, "ARTIFACT_ARCANE_NECKLACE_OF_MAGIC", ARTIFACT_ARCANE_NECKLACE_OF_MAGIC);
  lua_setconst(L, "ARTIFACT_CASTERS_BRACELET_OF_MAGIC", ARTIFACT_CASTERS_BRACELET_OF_MAGIC);
  lua_setconst(L, "ARTIFACT_MAGES_RING_OF_POWER", ARTIFACT_MAGES_RING_OF_POWER);
  lua_setconst(L, "ARTIFACT_WITCHS_BROACH_OF_MAGIC", ARTIFACT_WITCHS_BROACH_OF_MAGIC);
  lua_setconst(L, "ARTIFACT_MEDAL_OF_VALOR", ARTIFACT_MEDAL_OF_VALOR);
  lua_setconst(L, "ARTIFACT_MEDAL_OF_COURAGE", ARTIFACT_MEDAL_OF_COURAGE);
  lua_setconst(L, "ARTIFACT_MEDAL_OF_HONOR", ARTIFACT_MEDAL_OF_HONOR);
  lua_setconst(L, "ARTIFACT_MEDAL_OF_DISTINCTION", ARTIFACT_MEDAL_OF_DISTINCTION);
  lua_setconst(L, "ARTIFACT_FIZBIN_OF_MISFOURTUNE", ARTIFACT_FIZBIN_OF_MISFOURTUNE);
  lua_setconst(L, "ARTIFACT_THUNDER_MACE_OF_DOMINION", ARTIFACT_THUNDER_MACE_OF_DOMINION);
  lua_setconst(L, "ARTIFACT_ARMORED_GAUNTLETS_OF_PROTECTION", ARTIFACT_ARMORED_GAUNTLETS_OF_PROTECTION);
  lua_setconst(L, "ARTIFACT_DEFENDER_HELM_OF_PROTECTION", ARTIFACT_DEFENDER_HELM_OF_PROTECTION);
  lua_setconst(L, "ARTIFACT_GIANT_FLAIL_OF_DOMINION", ARTIFACT_GIANT_FLAIL_OF_DOMINION);
  lua_setconst(L, "ARTIFACT_BALLISTA_OF_QUICKNESS", ARTIFACT_BALLISTA_OF_QUICKNESS);
  lua_setconst(L, "ARTIFACT_STEALTH_SHIELD_OF_PROTECTION", ARTIFACT_STEALTH_SHIELD_OF_PROTECTION);
  lua_setconst(L, "ARTIFACT_DRAGON_SWORD_OF_DOMINION", ARTIFACT_DRAGON_SWORD_OF_DOMINION);
  lua_setconst(L, "ARTIFACT_POWER_AXE_OF_DOMINION", ARTIFACT_POWER_AXE_OF_DOMINION);
  lua_setconst(L, "ARTIFACT_DIVINE_BREASTPLATE_OF_PROTECTION", ARTIFACT_DIVINE_BREASTPLATE_OF_PROTECTION);
  lua_setconst(L, "ARTIFACT_MINOR_SCROLL_OF_KNOWLEDGE", ARTIFACT_MINOR_SCROLL_OF_KNOWLEDGE);
  lua_setconst(L, "ARTIFACT_MAJOR_SCROLL_OF_KNOWLEDGE", ARTIFACT_MAJOR_SCROLL_OF_KNOWLEDGE);
  lua_setconst(L, "ARTIFACT_SUPERIOR_SCROLL_OF_KNOWLEDGE", ARTIFACT_SUPERIOR_SCROLL_OF_KNOWLEDGE);
  lua_setconst(L, "ARTIFACT_FOREMOST_SCROLL_OF_KNOWLEDGE", ARTIFACT_FOREMOST_SCROLL_OF_KNOWLEDGE);
  lua_setconst(L, "ARTIFACT_ENDLESS_SACK_OF_GOLD", ARTIFACT_ENDLESS_SACK_OF_GOLD);
  lua_setconst(L, "ARTIFACT_ENDLESS_BAG_OF_GOLD", ARTIFACT_ENDLESS_BAG_OF_GOLD);
  lua_setconst(L, "ARTIFACT_ENDLESS_PURSE_OF_GOLD", ARTIFACT_ENDLESS_PURSE_OF_GOLD);
  lua_setconst(L, "ARTIFACT_NOMAD_BOOTS_OF_MOBILITY", ARTIFACT_NOMAD_BOOTS_OF_MOBILITY);
  lua_setconst(L, "ARTIFACT_TRAVELERS_BOOTS_OF_MOBILITY", ARTIFACT_TRAVELERS_BOOTS_OF_MOBILITY);
  lua_setconst(L, "ARTIFACT_LUCKY_RABBITS_FOOT", ARTIFACT_LUCKY_RABBITS_FOOT);
  lua_setconst(L, "ARTIFACT_GOLDEN_HORSESHOE", ARTIFACT_GOLDEN_HORSESHOE);
  lua_setconst(L, "ARTIFACT_GAMBLERS_LUCKY_COIN", ARTIFACT_GAMBLERS_LUCKY_COIN);
  lua_setconst(L, "ARTIFACT_FOUR_LEAF_CLOVER", ARTIFACT_FOUR_LEAF_CLOVER);
  lua_setconst(L, "ARTIFACT_TRUE_COMPASS_OF_MOBILITY", ARTIFACT_TRUE_COMPASS_OF_MOBILITY);
  lua_setconst(L, "ARTIFACT_SAILORS_ASTROLABE_OF_MOBILITY", ARTIFACT_SAILORS_ASTROLABE_OF_MOBILITY);
  lua_setconst(L, "ARTIFACT_EVIL_EYE", ARTIFACT_EVIL_EYE);
  lua_setconst(L, "ARTIFACT_ENCHANTED_HOURGLASS", ARTIFACT_ENCHANTED_HOURGLASS);
  lua_setconst(L, "ARTIFACT_GOLD_WATCH", ARTIFACT_GOLD_WATCH);
  lua_setconst(L, "ARTIFACT_SKULLCAP", ARTIFACT_SKULLCAP);
  lua_setconst(L, "ARTIFACT_ICE_CLOAK", ARTIFACT_ICE_CLOAK);
  lua_setconst(L, "ARTIFACT_FIRE_CLOAK", ARTIFACT_FIRE_CLOAK);
  lua_setconst(L, "ARTIFACT_LIGHTNING_HELM", ARTIFACT_LIGHTNING_HELM);
  lua_setconst(L, "ARTIFACT_EVERCOLD_ICICLE", ARTIFACT_EVERCOLD_ICICLE);
  lua_setconst(L, "ARTIFACT_EVERHOT_LAVA_ROCK", ARTIFACT_EVERHOT_LAVA_ROCK);
  lua_setconst(L, "ARTIFACT_LIGHTNING_ROD", ARTIFACT_LIGHTNING_ROD);
  lua_setconst(L, "ARTIFACT_SNAKE_RING", ARTIFACT_SNAKE_RING);
  lua_setconst(L, "ARTIFACT_ANKH", ARTIFACT_ANKH);
  lua_setconst(L, "ARTIFACT_BOOK_OF_ELEMENTS", ARTIFACT_BOOK_OF_ELEMENTS);
  lua_setconst(L, "ARTIFACT_ELEMENTAL_RING", ARTIFACT_ELEMENTAL_RING);
  lua_setconst(L, "ARTIFACT_HOLY_PENDANT", ARTIFACT_HOLY_PENDANT);
  lua_setconst(L, "ARTIFACT_PENDANT_OF_FREE_WILL", ARTIFACT_PENDANT_OF_FREE_WILL);
  lua_setconst(L, "ARTIFACT_PENDANT_OF_LIFE", ARTIFACT_PENDANT_OF_LIFE);
  lua_setconst(L, "ARTIFACT_SERENITY_PENDANT", ARTIFACT_SERENITY_PENDANT);
  lua_setconst(L, "ARTIFACT_SEEING_EYE_PENDANT", ARTIFACT_SEEING_EYE_PENDANT);
  lua_setconst(L, "ARTIFACT_KINETIC_PENDANT", ARTIFACT_KINETIC_PENDANT);
  lua_setconst(L, "ARTIFACT_PENDANT_OF_DEATH", ARTIFACT_PENDANT_OF_DEATH);
  lua_setconst(L, "ARTIFACT_WAND_OF_NEGATION", ARTIFACT_WAND_OF_NEGATION);
  lua_setconst(L, "ARTIFACT_GOLDEN_BOW", ARTIFACT_GOLDEN_BOW);
  lua_setconst(L, "ARTIFACT_TELESCOPE", ARTIFACT_TELESCOPE);
  lua_setconst(L, "ARTIFACT_STATESMANS_QUILL", ARTIFACT_STATESMANS_QUILL);
  lua_setconst(L, "ARTIFACT_WIZARDS_HAT", ARTIFACT_WIZARDS_HAT);
  lua_setconst(L, "ARTIFACT_POWER_RING", ARTIFACT_POWER_RING);
  lua_setconst(L, "ARTIFACT_AMMO_CART", ARTIFACT_AMMO_CART);
  lua_setconst(L, "ARTIFACT_TAX_LIEN", ARTIFACT_TAX_LIEN);
  lua_setconst(L, "ARTIFACT_HIDEOUS_MASK", ARTIFACT_HIDEOUS_MASK);
  lua_setconst(L, "ARTIFACT_ENDLESS_POUCH_OF_SULFUR", ARTIFACT_ENDLESS_POUCH_OF_SULFUR);
  lua_setconst(L, "ARTIFACT_ENDLESS_VIAL_OF_MERCURY", ARTIFACT_ENDLESS_VIAL_OF_MERCURY);
  lua_setconst(L, "ARTIFACT_ENDLESS_POUCH_OF_GEMS", ARTIFACT_ENDLESS_POUCH_OF_GEMS);
  lua_setconst(L, "ARTIFACT_ENDLESS_CORD_OF_WOOD", ARTIFACT_ENDLESS_CORD_OF_WOOD);
  lua_setconst(L, "ARTIFACT_ENDLESS_CART_OF_ORE", ARTIFACT_ENDLESS_CART_OF_ORE);
  lua_setconst(L, "ARTIFACT_ENDLESS_POUCH_OF_CRYSTAL", ARTIFACT_ENDLESS_POUCH_OF_CRYSTAL);
  lua_setconst(L, "ARTIFACT_SPIKED_HELM", ARTIFACT_SPIKED_HELM);
  lua_setconst(L, "ARTIFACT_SPIKED_SHIELD", ARTIFACT_SPIKED_SHIELD);
  lua_setconst(L, "ARTIFACT_WHITE_PEARL", ARTIFACT_WHITE_PEARL);
  lua_setconst(L, "ARTIFACT_BLACK_PEARL", ARTIFACT_BLACK_PEARL);
  lua_setconst(L, "ARTIFACT_MAGIC_BOOK", ARTIFACT_MAGIC_BOOK);
  lua_setconst(L, "ARTIFACT_SPELL_SCROLL", ARTIFACT_SPELL_SCROLL);
  lua_setconst(L, "ARTIFACT_ARM_OF_THE_MARTYR", ARTIFACT_ARM_OF_THE_MARTYR);
  lua_setconst(L, "ARTIFACT_BREASTPLATE_OF_ANDURAN", ARTIFACT_BREASTPLATE_OF_ANDURAN);
  lua_setconst(L, "ARTIFACT_BROACH_OF_SHIELDING", ARTIFACT_BROACH_OF_SHIELDING);
  lua_setconst(L, "ARTIFACT_BATTLE_GARB_OF_ANDURAN", ARTIFACT_BATTLE_GARB_OF_ANDURAN);
  lua_setconst(L, "ARTIFACT_CRYSTAL_BALL", ARTIFACT_CRYSTAL_BALL);
  lua_setconst(L, "ARTIFACT_HEART_OF_FIRE", ARTIFACT_HEART_OF_FIRE);
  lua_setconst(L, "ARTIFACT_HEART_OF_ICE", ARTIFACT_HEART_OF_ICE);
  lua_setconst(L, "ARTIFACT_HELMET_OF_ANDURAN", ARTIFACT_HELMET_OF_ANDURAN);
  lua_setconst(L, "ARTIFACT_HOLY_HAMMER", ARTIFACT_HOLY_HAMMER);
  lua_setconst(L, "ARTIFACT_LEGENDARY_SCEPTER", ARTIFACT_LEGENDARY_SCEPTER);
  lua_setconst(L, "ARTIFACT_MASTHEAD", ARTIFACT_MASTHEAD);
  lua_setconst(L, "ARTIFACT_SPHERE_OF_NEGATION", ARTIFACT_SPHERE_OF_NEGATION);
  lua_setconst(L, "ARTIFACT_STAFF_OF_WIZARDRY", ARTIFACT_STAFF_OF_WIZARDRY);
  lua_setconst(L, "ARTIFACT_SWORD_BREAKER", ARTIFACT_SWORD_BREAKER);
  lua_setconst(L, "ARTIFACT_SWORD_OF_ANDURAN", ARTIFACT_SWORD_OF_ANDURAN);
  lua_setconst(L, "ARTIFACT_SPADE_OF_NECROMANCY", ARTIFACT_SPADE_OF_NECROMANCY);
  lua_setconst(L, "ARTIFACT_PANDORA_BOX", ARTIFACT_PANDORA_BOX);
}

void set_luck_consts(lua_State *L) {
	lua_setconst(L, "IMAGE_LUCK", IMAGE_LUCK);
	lua_setconst(L, "IMAGE_BADLUCK", IMAGE_BADLUCK);

}


void set_town_consts(lua_State* L) {
  lua_setconst(L, "BUILDING_MAGE_GUILD", BUILDING_MAGE_GUILD);
  lua_setconst(L, "BUILDING_THIEVES_GUILD", BUILDING_THIEVES_GUILD);
  lua_setconst(L, "BUILDING_TAVERN", BUILDING_TAVERN);
  lua_setconst(L, "BUILDING_DOCK", BUILDING_DOCK);
  lua_setconst(L, "BUILDING_WELL", BUILDING_WELL);
  lua_setconst(L, "BUILDING_TENT", BUILDING_TENT);
  lua_setconst(L, "BUILDING_CASTLE", BUILDING_CASTLE);
  lua_setconst(L, "BUILDING_STATUE", BUILDING_STATUE);
  lua_setconst(L, "BUILDING_LEFT_TURRET", BUILDING_LEFT_TURRET);
  lua_setconst(L, "BUILDING_RIGHT_TURRET", BUILDING_RIGHT_TURRET);
  lua_setconst(L, "BUILDING_MARKET", BUILDING_MARKET);
  lua_setconst(L, "BUILDING_SPECIAL_GROWTH", BUILDING_SPECIAL_GROWTH);
  lua_setconst(L, "BUILDING_MOAT", BUILDING_MOAT);
  lua_setconst(L, "BUILDING_SPECIAL", BUILDING_SPECIAL);
  lua_setconst(L, "BUILDING_BOAT", BUILDING_BOAT);
  lua_setconst(L, "BUILDING_CAPTAIN", BUILDING_CAPTAIN);
  lua_setconst(L, "BUILDING_DWELLING_1", BUILDING_DWELLING_1);
  lua_setconst(L, "BUILDING_DWELLING_2", BUILDING_DWELLING_2);
  lua_setconst(L, "BUILDING_DWELLING_3", BUILDING_DWELLING_3);
  lua_setconst(L, "BUILDING_DWELLING_4", BUILDING_DWELLING_4);
  lua_setconst(L, "BUILDING_DWELLING_5", BUILDING_DWELLING_5);
  lua_setconst(L, "BUILDING_DWELLING_6", BUILDING_DWELLING_6);
  lua_setconst(L, "BUILDING_UPGRADE_1", BUILDING_UPGRADE_1);
  lua_setconst(L, "BUILDING_UPGRADE_2", BUILDING_UPGRADE_2);
  lua_setconst(L, "BUILDING_UPGRADE_3", BUILDING_UPGRADE_3);
  lua_setconst(L, "BUILDING_UPGRADE_4", BUILDING_UPGRADE_4);
  lua_setconst(L, "BUILDING_UPGRADE_5", BUILDING_UPGRADE_5);
  lua_setconst(L, "BUILDING_UPGRADE_5B", BUILDING_UPGRADE_5B);
}

void set_faction_consts(lua_State* L) {
  lua_setconst(L, "FACTION_KNIGHT", 0);
  lua_setconst(L, "FACTION_BARBARIAN", 1);
  lua_setconst(L, "FACTION_SORCERESS", 2);
  lua_setconst(L, "FACTION_WARLOCK", 3);
  lua_setconst(L, "FACTION_WIZARD", 4);
  lua_setconst(L, "FACTION_NECROMANCER", 5);
  lua_setconst(L, "FACTION_MULTIPLE", 6);
  lua_setconst(L, "FACTION_RANDOM", 7);
}


void set_skill_consts(lua_State* L) {
  lua_setconst(L, "PRIMARY_SKILL_ATTACK", PRIMARY_SKILL_ATTACK);
  lua_setconst(L, "PRIMARY_SKILL_DEFENSE", PRIMARY_SKILL_DEFENSE);
  lua_setconst(L, "PRIMARY_SKILL_SPELLPOWER", PRIMARY_SKILL_SPELLPOWER);
  lua_setconst(L, "PRIMARY_SKILL_KNOWLEDGE", PRIMARY_SKILL_KNOWLEDGE);
  lua_setconst(L, "SECONDARY_SKILL_PATHFINDING", SECONDARY_SKILL_PATHFINDING);
  lua_setconst(L, "SECONDARY_SKILL_ARCHERY", SECONDARY_SKILL_ARCHERY);
  lua_setconst(L, "SECONDARY_SKILL_LOGISTICS", SECONDARY_SKILL_LOGISTICS);
  lua_setconst(L, "SECONDARY_SKILL_SCOUTING", SECONDARY_SKILL_SCOUTING);
  lua_setconst(L, "SECONDARY_SKILL_DIPLOMACY", SECONDARY_SKILL_DIPLOMACY);
  lua_setconst(L, "SECONDARY_SKILL_NAVIGATION", SECONDARY_SKILL_NAVIGATION);
  lua_setconst(L, "SECONDARY_SKILL_LEADERSHIP", SECONDARY_SKILL_LEADERSHIP);
  lua_setconst(L, "SECONDARY_SKILL_WISDOM", SECONDARY_SKILL_WISDOM);
  lua_setconst(L, "SECONDARY_SKILL_MYSTICISM", SECONDARY_SKILL_MYSTICISM);
  lua_setconst(L, "SECONDARY_SKILL_LUCK", SECONDARY_SKILL_LUCK);
  lua_setconst(L, "SECONDARY_SKILL_BALLISTICS", SECONDARY_SKILL_BALLISTICS);
  lua_setconst(L, "SECONDARY_SKILL_EAGLE_EYE", SECONDARY_SKILL_EAGLE_EYE);
  lua_setconst(L, "SECONDARY_SKILL_NECROMANCY", SECONDARY_SKILL_NECROMANCY);
  lua_setconst(L, "SECONDARY_SKILL_ESTATES", SECONDARY_SKILL_ESTATES);
}

void set_creature_consts(lua_State* L) {
  lua_setconst(L, "CREATURE_PEASANT", CREATURE_PEASANT);
  lua_setconst(L, "CREATURE_ARCHER", CREATURE_ARCHER);
  lua_setconst(L, "CREATURE_RANGER", CREATURE_RANGER);
  lua_setconst(L, "CREATURE_PIKEMAN", CREATURE_PIKEMAN);
  lua_setconst(L, "CREATURE_VETERAN_PIKEMAN", CREATURE_VETERAN_PIKEMAN);
  lua_setconst(L, "CREATURE_SWORDSMAN", CREATURE_SWORDSMAN);
  lua_setconst(L, "CREATURE_MASTER_SWORDSMAN", CREATURE_MASTER_SWORDSMAN);
  lua_setconst(L, "CREATURE_CAVALRY", CREATURE_CAVALRY);
  lua_setconst(L, "CREATURE_CHAMPION", CREATURE_CHAMPION);
  lua_setconst(L, "CREATURE_PALADIN", CREATURE_PALADIN);
  lua_setconst(L, "CREATURE_CRUSADER", CREATURE_CRUSADER);
  lua_setconst(L, "CREATURE_GOBLIN", CREATURE_GOBLIN);
  lua_setconst(L, "CREATURE_ORC", CREATURE_ORC);
  lua_setconst(L, "CREATURE_ORC_CHIEF", CREATURE_ORC_CHIEF);
  lua_setconst(L, "CREATURE_WOLF", CREATURE_WOLF);
  lua_setconst(L, "CREATURE_OGRE", CREATURE_OGRE);
  lua_setconst(L, "CREATURE_OGRE_LORD", CREATURE_OGRE_LORD);
  lua_setconst(L, "CREATURE_TROLL", CREATURE_TROLL);
  lua_setconst(L, "CREATURE_WAR_TROLL", CREATURE_WAR_TROLL);
  lua_setconst(L, "CREATURE_CYCLOPS", CREATURE_CYCLOPS);
  lua_setconst(L, "CREATURE_SPRITE", CREATURE_SPRITE);
  lua_setconst(L, "CREATURE_DWARF", CREATURE_DWARF);
  lua_setconst(L, "CREATURE_BATTLE_DWARF", CREATURE_BATTLE_DWARF);
  lua_setconst(L, "CREATURE_ELF", CREATURE_ELF);
  lua_setconst(L, "CREATURE_GRAND_ELF", CREATURE_GRAND_ELF);
  lua_setconst(L, "CREATURE_DRUID", CREATURE_DRUID);
  lua_setconst(L, "CREATURE_GREATER_DRUID", CREATURE_GREATER_DRUID);
  lua_setconst(L, "CREATURE_UNICORN", CREATURE_UNICORN);
  lua_setconst(L, "CREATURE_PHOENIX", CREATURE_PHOENIX);
  lua_setconst(L, "CREATURE_CENTAUR", CREATURE_CENTAUR);
  lua_setconst(L, "CREATURE_GARGOYLE", CREATURE_GARGOYLE);
  lua_setconst(L, "CREATURE_GRIFFIN", CREATURE_GRIFFIN);
  lua_setconst(L, "CREATURE_MINOTAUR", CREATURE_MINOTAUR);
  lua_setconst(L, "CREATURE_MINOTAUR_KING", CREATURE_MINOTAUR_KING);
  lua_setconst(L, "CREATURE_HYDRA", CREATURE_HYDRA);
  lua_setconst(L, "CREATURE_GREEN_DRAGON", CREATURE_GREEN_DRAGON);
  lua_setconst(L, "CREATURE_RED_DRAGON", CREATURE_RED_DRAGON);
  lua_setconst(L, "CREATURE_BLACK_DRAGON", CREATURE_BLACK_DRAGON);
  lua_setconst(L, "CREATURE_HALFLING", CREATURE_HALFLING);
  lua_setconst(L, "CREATURE_BOAR", CREATURE_BOAR);
  lua_setconst(L, "CREATURE_IRON_GOLEM", CREATURE_IRON_GOLEM);
  lua_setconst(L, "CREATURE_STEEL_GOLEM", CREATURE_STEEL_GOLEM);
  lua_setconst(L, "CREATURE_ROC", CREATURE_ROC);
  lua_setconst(L, "CREATURE_MAGE", CREATURE_MAGE);
  lua_setconst(L, "CREATURE_ARCHMAGE", CREATURE_ARCHMAGE);
  lua_setconst(L, "CREATURE_GIANT", CREATURE_GIANT);
  lua_setconst(L, "CREATURE_TITAN", CREATURE_TITAN);
  lua_setconst(L, "CREATURE_SKELETON", CREATURE_SKELETON);
  lua_setconst(L, "CREATURE_ZOMBIE", CREATURE_ZOMBIE);
  lua_setconst(L, "CREATURE_MUTANT_ZOMBIE", CREATURE_MUTANT_ZOMBIE);
  lua_setconst(L, "CREATURE_MUMMY", CREATURE_MUMMY);
  lua_setconst(L, "CREATURE_ROYAL_MUMMY", CREATURE_ROYAL_MUMMY);
  lua_setconst(L, "CREATURE_VAMPIRE", CREATURE_VAMPIRE);
  lua_setconst(L, "CREATURE_VAMPIRE_LORD", CREATURE_VAMPIRE_LORD);
  lua_setconst(L, "CREATURE_LICH", CREATURE_LICH);
  lua_setconst(L, "CREATURE_POWER_LICH", CREATURE_POWER_LICH);
  lua_setconst(L, "CREATURE_BONE_DRAGON", CREATURE_BONE_DRAGON);
  lua_setconst(L, "CREATURE_ROGUE", CREATURE_ROGUE);
  lua_setconst(L, "CREATURE_NOMAD", CREATURE_NOMAD);
  lua_setconst(L, "CREATURE_GHOST", CREATURE_GHOST);
  lua_setconst(L, "CREATURE_GENIE", CREATURE_GENIE);
  lua_setconst(L, "CREATURE_MEDUSA", CREATURE_MEDUSA);
  lua_setconst(L, "CREATURE_EARTH_ELEMENTAL", CREATURE_EARTH_ELEMENTAL);
  lua_setconst(L, "CREATURE_AIR_ELEMENTAL", CREATURE_AIR_ELEMENTAL);
  lua_setconst(L, "CREATURE_FIRE_ELEMENTAL", CREATURE_FIRE_ELEMENTAL);
  lua_setconst(L, "CREATURE_WATER_ELEMENTAL", CREATURE_WATER_ELEMENTAL);
  lua_setconst(L, "CREATURE_KOBOLD", CREATURE_KOBOLD);
  lua_setconst(L, "CREATURE_BLOODSUCKER", CREATURE_BLOODSUCKER);
  lua_setconst(L, "CREATURE_HARPY", CREATURE_HARPY);
  lua_setconst(L, "CREATURE_BLACK_KNIGHT", CREATURE_BLACK_KNIGHT);
  lua_setconst(L, "CREATURE_CATOBLEBA", CREATURE_CATOBLEBA);
  lua_setconst(L, "CREATURE_TREANT", CREATURE_TREANT);
  lua_setconst(L, "CREATURE_CYBER_KOBOLD_SPEARMAN", CREATURE_CYBER_KOBOLD_SPEARMAN);
  lua_setconst(L, "CREATURE_CYBER_PLASMA_BERSERKER", CREATURE_CYBER_PLASMA_BERSERKER);
  lua_setconst(L, "CREATURE_CYBER_INDIGO_PANTHER", CREATURE_CYBER_INDIGO_PANTHER);
  lua_setconst(L, "CREATURE_CYBER_SHADOW_ASSASSIN", CREATURE_CYBER_SHADOW_ASSASSIN);
  lua_setconst(L, "CREATURE_CYBER_BEHEMOTH", CREATURE_CYBER_BEHEMOTH);
}

void set_resources_consts(lua_State *L) {
  lua_setconst(L, "RESOURCE_WOOD", RESOURCE_WOOD);
  lua_setconst(L, "RESOURCE_MERCURY", RESOURCE_MERCURY);
  lua_setconst(L, "RESOURCE_ORE", RESOURCE_ORE);
  lua_setconst(L, "RESOURCE_SULFUR", RESOURCE_SULFUR);
  lua_setconst(L, "RESOURCE_CRYSTALS", RESOURCE_CRYSTAL);
  lua_setconst(L, "RESOURCE_GEMS", RESOURCE_GEMS);
  lua_setconst(L, "RESOURCE_GOLD", RESOURCE_GOLD);
}

void set_location_consts(lua_State *L) {
  lua_setconst(L, "LOCATION_ALCHEMIST_LAB", LOCATION_ALCHEMIST_LAB);
  lua_setconst(L, "LOCATION_SIGN", LOCATION_SIGN);
  lua_setconst(L, "LOCATION_BUOY", LOCATION_BUOY);
  lua_setconst(L, "LOCATION_SKELETON", LOCATION_SKELETON);
  lua_setconst(L, "LOCATION_DAEMON_CAVE", LOCATION_DAEMON_CAVE);
  lua_setconst(L, "LOCATION_TREASURE_CHEST", LOCATION_TREASURE_CHEST);
  lua_setconst(L, "LOCATION_FAERIE_RING", LOCATION_FAERIE_RING);
  lua_setconst(L, "LOCATION_CAMPFIRE", LOCATION_CAMPFIRE);
  lua_setconst(L, "LOCATION_FOUNTAIN", LOCATION_FOUNTAIN);
  lua_setconst(L, "LOCATION_GAZEBO", LOCATION_GAZEBO);
  lua_setconst(L, "LOCATION_ANCIENT_LAMP", LOCATION_ANCIENT_LAMP);
  lua_setconst(L, "LOCATION_GRAVEYARD", LOCATION_GRAVEYARD);
  lua_setconst(L, "LOCATION_ARCHERS_HOUSE", LOCATION_ARCHERS_HOUSE);
  lua_setconst(L, "LOCATION_GOBLIN_HUT", LOCATION_GOBLIN_HUT);
  lua_setconst(L, "LOCATION_DWARF_COTTAGE", LOCATION_DWARF_COTTAGE);
  lua_setconst(L, "LOCATION_PEASANT_HUT", LOCATION_PEASANT_HUT);
  lua_setconst(L, "LOCATION_LOG_CABIN", LOCATION_LOG_CABIN);
  lua_setconst(L, "LOCATION_ROAD", LOCATION_ROAD);
  lua_setconst(L, "LOCATION_EVENT", LOCATION_EVENT);
  lua_setconst(L, "LOCATION_DRAGON_CITY", LOCATION_DRAGON_CITY);
  lua_setconst(L, "LOCATION_LIGHTHOUSE", LOCATION_LIGHTHOUSE);
  lua_setconst(L, "LOCATION_WATERWHEEL", LOCATION_WATERWHEEL);
  lua_setconst(L, "LOCATION_MINE", LOCATION_MINE);
  lua_setconst(L, "LOCATION_ARMY_CAMP", LOCATION_ARMY_CAMP);
  lua_setconst(L, "LOCATION_OBELISK", LOCATION_OBELISK);
  lua_setconst(L, "LOCATION_OASIS", LOCATION_OASIS);
  lua_setconst(L, "LOCATION_RESOURCE", LOCATION_RESOURCE);
  lua_setconst(L, "LOCATION_SAWMILL", LOCATION_SAWMILL);
  lua_setconst(L, "LOCATION_ORACLE", LOCATION_ORACLE);
  lua_setconst(L, "LOCATION_SHRINE_FIRST_ORDER", LOCATION_SHRINE_FIRST_ORDER);
  lua_setconst(L, "LOCATION_SHIPWRECK", LOCATION_SHIPWRECK);
  lua_setconst(L, "LOCATION_SEA_CHEST", LOCATION_SEA_CHEST);
  lua_setconst(L, "LOCATION_DESERT_TENT", LOCATION_DESERT_TENT);
  lua_setconst(L, "LOCATION_TOWN", LOCATION_TOWN);
  lua_setconst(L, "LOCATION_STONE_LITHS", LOCATION_STONE_LITHS);
  lua_setconst(L, "LOCATION_WAGON_CAMP", LOCATION_WAGON_CAMP);
  lua_setconst(L, "LOCATION_WELL", LOCATION_WELL);
  lua_setconst(L, "LOCATION_WHIRLPOOL", LOCATION_WHIRLPOOL);
  lua_setconst(L, "LOCATION_WINDMILL", LOCATION_WINDMILL);
  lua_setconst(L, "LOCATION_ARTIFACT", LOCATION_ARTIFACT);
  lua_setconst(L, "LOCATION_HERO", LOCATION_HERO);
  lua_setconst(L, "LOCATION_BOAT", LOCATION_BOAT);
  lua_setconst(L, "LOCATION_RANDOM_ARTIFACT", LOCATION_RANDOM_ARTIFACT);
  lua_setconst(L, "LOCATION_RANDOM_RESOURCE", LOCATION_RANDOM_RESOURCE);
  lua_setconst(L, "LOCATION_RANDOM_MONSTER", LOCATION_RANDOM_MONSTER);
  lua_setconst(L, "LOCATION_RANDOM_TOWN", LOCATION_RANDOM_TOWN);
  lua_setconst(L, "LOCATION_RANDOM_CASTLE", LOCATION_RANDOM_CASTLE);
  lua_setconst(L, "LOCATION_RANDOM_MONSTER_WEAK", LOCATION_RANDOM_MONSTER_WEAK);
  lua_setconst(L, "LOCATION_RANDOM_MONSTER_MEDIUM", LOCATION_RANDOM_MONSTER_MEDIUM);
  lua_setconst(L, "LOCATION_RANDOM_MONSTER_STRONG", LOCATION_RANDOM_MONSTER_STRONG);
  lua_setconst(L, "LOCATION_RANDOM_MONSTER_VERY_STRONG", LOCATION_RANDOM_MONSTER_VERY_STRONG);
  lua_setconst(L, "LOCATION_RANDOM_HERO", LOCATION_RANDOM_HERO);
  lua_setconst(L, "LOCATION_NOTHING_SPECIAL", LOCATION_NOTHING_SPECIAL);
  lua_setconst(L, "LOCATION_WATCH_TOWER", LOCATION_WATCH_TOWER);
  lua_setconst(L, "LOCATION_TREE_HOUSE", LOCATION_TREE_HOUSE);
  lua_setconst(L, "LOCATION_TREE_CITY", LOCATION_TREE_CITY);
  lua_setconst(L, "LOCATION_RUINS", LOCATION_RUINS);
  lua_setconst(L, "LOCATION_FORT", LOCATION_FORT);
  lua_setconst(L, "LOCATION_TRADING_POST", LOCATION_TRADING_POST);
  lua_setconst(L, "LOCATION_ABANDONED_MINE", LOCATION_ABANDONED_MINE);
  lua_setconst(L, "LOCATION_DWARF_CABIN", LOCATION_DWARF_CABIN);
  lua_setconst(L, "LOCATION_STANDING_STONES", LOCATION_STANDING_STONES);
  lua_setconst(L, "LOCATION_IDOL", LOCATION_IDOL);
  lua_setconst(L, "LOCATION_TREE_OF_KNOWLEDGE", LOCATION_TREE_OF_KNOWLEDGE);
  lua_setconst(L, "LOCATION_WITCH_DOCTORS_HUT", LOCATION_WITCH_DOCTORS_HUT);
  lua_setconst(L, "LOCATION_TEMPLE", LOCATION_TEMPLE);
  lua_setconst(L, "LOCATION_HILL_FORT", LOCATION_HILL_FORT);
  lua_setconst(L, "LOCATION_HALFLING_HOLE", LOCATION_HALFLING_HOLE);
  lua_setconst(L, "LOCATION_MERCENARY_CAMP", LOCATION_MERCENARY_CAMP);
  lua_setconst(L, "LOCATION_SHRINE_SECOND_ORDER", LOCATION_SHRINE_SECOND_ORDER);
  lua_setconst(L, "LOCATION_SHRINE_THIRD_ORDER", LOCATION_SHRINE_THIRD_ORDER);
  lua_setconst(L, "LOCATION_PYRAMID", LOCATION_PYRAMID);
  lua_setconst(L, "LOCATION_CITY_OF_DEAD", LOCATION_CITY_OF_DEAD);
  lua_setconst(L, "LOCATION_EXCAVATION", LOCATION_EXCAVATION);
  lua_setconst(L, "LOCATION_SPHINX", LOCATION_SPHINX);
  lua_setconst(L, "LOCATION_WAGON", LOCATION_WAGON);
  lua_setconst(L, "LOCATION_TAR_PIT", LOCATION_TAR_PIT);
  lua_setconst(L, "LOCATION_ARTESIAN_SPRING", LOCATION_ARTESIAN_SPRING);
  lua_setconst(L, "LOCATION_TROLL_BRIDGE", LOCATION_TROLL_BRIDGE);
  lua_setconst(L, "LOCATION_WATERING_HOLE", LOCATION_WATERING_HOLE);
  lua_setconst(L, "LOCATION_WITCH_HUT", LOCATION_WITCH_HUT);
  lua_setconst(L, "LOCATION_XANADU", LOCATION_XANADU);
  lua_setconst(L, "LOCATION_CAVE", LOCATION_CAVE);
  lua_setconst(L, "LOCATION_LEAN_TO", LOCATION_LEAN_TO);
  lua_setconst(L, "LOCATION_MAGELLANS_MAPS", LOCATION_MAGELLANS_MAPS);
  lua_setconst(L, "LOCATION_FLOTSAM", LOCATION_FLOTSAM);
  lua_setconst(L, "LOCATION_DERELICT_SHIP", LOCATION_DERELICT_SHIP);
  lua_setconst(L, "LOCATION_SHIPWRECK_SURVIVOR", LOCATION_SHIPWRECK_SURVIVOR);
  lua_setconst(L, "LOCATION_BOTTLE", LOCATION_BOTTLE);
  lua_setconst(L, "LOCATION_MAGIC_WELL", LOCATION_MAGIC_WELL);
  lua_setconst(L, "LOCATION_MAGIC_GARDEN", LOCATION_MAGIC_GARDEN);
  lua_setconst(L, "LOCATION_OBSERVATION_TOWER", LOCATION_OBSERVATION_TOWER);
  lua_setconst(L, "LOCATION_FREEMANS_FOUNDRY", LOCATION_FREEMANS_FOUNDRY);
  lua_setconst(L, "LOCATION_STREAM", LOCATION_STREAM);
  lua_setconst(L, "LOCATION_TREES", LOCATION_TREES);
  lua_setconst(L, "LOCATION_MOUNTAINS", LOCATION_MOUNTAINS);
  lua_setconst(L, "LOCATION_VOLCANO", LOCATION_VOLCANO);
  lua_setconst(L, "LOCATION_FLOWERS", LOCATION_FLOWERS);
  lua_setconst(L, "LOCATION_ROCK", LOCATION_ROCK);
  lua_setconst(L, "LOCATION_LAKE", LOCATION_LAKE);
  lua_setconst(L, "LOCATION_MANDRAKE", LOCATION_MANDRAKE);
  lua_setconst(L, "LOCATION_DEAD_TREE", LOCATION_DEAD_TREE);
  lua_setconst(L, "LOCATION_STUMP", LOCATION_STUMP);
  lua_setconst(L, "LOCATION_CRATER", LOCATION_CRATER);
  lua_setconst(L, "LOCATION_CACTUS", LOCATION_CACTUS);
  lua_setconst(L, "LOCATION_MOUND", LOCATION_MOUND);
  lua_setconst(L, "LOCATION_DUNE", LOCATION_DUNE);
  lua_setconst(L, "LOCATION_LAVA_POOL", LOCATION_LAVA_POOL);
  lua_setconst(L, "LOCATION_SHRUB", LOCATION_SHRUB);
  lua_setconst(L, "LOCATION_HOLE", LOCATION_HOLE);
  lua_setconst(L, "LOCATION_OUTCROPPING", LOCATION_OUTCROPPING);
  lua_setconst(L, "LOCATION_RANDOM_ARTIFACT_TREASURE", LOCATION_RANDOM_ARTIFACT_TREASURE);
  lua_setconst(L, "LOCATION_RANDOM_ARTIFACT_MINOR", LOCATION_RANDOM_ARTIFACT_MINOR);
  lua_setconst(L, "LOCATION_RANDOM_ARTIFACT_MAJOR", LOCATION_RANDOM_ARTIFACT_MAJOR);
  lua_setconst(L, "LOCATION_BARRIER", LOCATION_BARRIER);
  lua_setconst(L, "LOCATION_TRAVELLER_TENT", LOCATION_TRAVELLER_TENT);
  lua_setconst(L, "LOCATION_EXPANSION_DWELLING", LOCATION_EXPANSION_DWELLING);
  lua_setconst(L, "LOCATION_ALCHEMIST_TOWER", LOCATION_ALCHEMIST_TOWER);
  lua_setconst(L, "LOCATION_JAIL", LOCATION_JAIL);
}

void set_map_cell_consts(lua_State *L) {
  lua_setconst(L, "MAP_CELL_NO_FLIP", MAP_CELL_NO_FLIP);
  lua_setconst(L, "MAP_CELL_FLIP_VERTICALLY", MAP_CELL_FLIP_VERTICALLY);
  lua_setconst(L, "MAP_CELL_FLIP_HORIZONTALLY", MAP_CELL_FLIP_HORIZONTALLY);
  lua_setconst(L, "MAP_CELL_FLIP_DIAGONALLY", MAP_CELL_FLIP_DIAGONALLY);
}

void set_map_consts(lua_State *L) {
  set_location_consts(L);
  set_map_cell_consts(L);
}

/*************************************************************************************/

void set_scripting_consts(lua_State* L) {
  set_gui_consts(L);
  set_spell_consts(L);
  set_artifact_consts(L);
  set_town_consts(L);
  set_faction_consts(L);
  set_skill_consts(L);
  set_creature_consts(L);
  set_location_consts(L);  
  set_barriercolor_consts(L);
  set_resources_consts(L); 
  set_map_consts(L);
}
