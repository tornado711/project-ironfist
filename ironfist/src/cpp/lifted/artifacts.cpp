#include "tied/artifacts.h"

/*
 *
 * Also still unsupported:
 * 1) Random artifacts
 * 2) AI Artifact value tables
 *
 * FURTHERMORE
 *
 * For some strange reason, the assembly for the method UpdateHeroStatus bar
 * refers to table of primary-skill names by its offset relative the artifact events table.
 * Thus, there is some gibberish when you right-click a hero, and when two heroes trade.
 *
 * game::GetVictoryConditionText also does funny stuff in looking up artifact names  
 */

char *gArtifactNames[] = {
	"Ultimate Book of Knowledge",
	"Ultimate Sword of Dominion",
	"Ultimate Cloak of Protection",
	"Ultimate Wand of Magic",
	"Ultimate Shield",
	"Ultimate Staff",
	"Ultimate Crown",
	"Golden Goose",
	"Arcane Necklace of Magic",
	"Caster's Bracelet of Magic",
	"Mage's Ring of Power",
	"Witch's Broach of Magic",
	"Medal of Valor",
	"Medal of Courage",
	"Medal of Honor",
	"Medal of Distinction",
	"Fizbin of Misfortune",
	"Thunder Mace of Dominion",
	"Armored Gauntlets of Protection",
	"Defender Helm of Protection",
	"Giant Flail of Dominion",
	"Ballista of Quickness",
	"Stealth Shield of Protection",
	"Dragon Sword of Dominion",
	"Power Axe of Dominion",
	"Divine Breastplate of Protection",
	"Minor Scroll of Knowledge",
	"Major Scroll of Knowledge",
	"Superior Scroll of Knowledge",
	"Foremost Scroll of Knowledge",
	"Endless Sack of Gold",
	"Endless Bag of Gold",
	"Endless Purse of Gold",
	"Nomad Boots of Mobility",
	"Traveler's Boots of Mobility",
	"Lucky Rabbit's Foot",
	"Golden Horseshoe",
	"Gambler's Lucky Coin",
	"Four-Leaf Clover",
	"True Compass of Mobility",
	"Sailor's Astrolabe of Mobility",
	"Evil Eye",
	"Enchanted Hourglass",
	"Gold Watch",
	"Skullcap",
	"Ice Cloak",
	"Fire Cloak",
	"Lightning Helm",
	"Evercold Icicle",
	"Everhot Lava Rock",
	"Lightning Rod ",
	"Snake-Ring",
	"Ankh",
	"Book of Elements",
	"Elemental Ring",
	"Holy Pendant",
	"Pendant of Free Will",
	"Pendant of Life",
	"Serenity Pendant",
	"Seeing-eye Pendant",
	"Kinetic Pendant",
	"Pendant of Death",
	"Wand of Negation",
	"Golden Bow",
	"Telescope",
	"Statesman's Quill",
	"Wizard's Hat",
	"Power Ring",
	"Ammo Cart",
	"Tax Lien",
	"Hideous Mask",
	"Endless Pouch of Sulfur",
	"Endless Vial of Mercury",
	"Endless Pouch of Gems",
	"Endless Cord of Wood",
	"Endless Cart of Ore",
	"Endless Pouch of Crystal",
	"Spiked Helm",
	"Spiked Shield",
	"White Pearl",
	"Black Pearl",
	"Magic Book",
	"ERROR : Artifact 82",
	"ERROR : Artifact 83",
	"ERROR : Artifact 84",
	"ERROR : Artifact 85",
	"Spell Scroll",
	"Arm of the Martyr",
	"Breastplate of Anduran",
	"Broach of Shielding",
	"Battle Garb of Anduran",
	"Crystal Ball",
	"Heart of Fire",
	"Heart of Ice",
	"Helmet of Anduran",
	"Holy Hammer",
	"Legendary Scepter",
	"Masthead",
	"Sphere of Negation",
	"Staff of Wizardry",
	"Sword Breaker",
	"Sword of Anduran",
	"Spade of Necromancy",
	"Pandora's Box"
};

char *gArtifactDesc[] =
{
	"{Ultimate Book\n(+12 Knowledge)}\n\nThe Ultimate Book of Knowledge increases your knowledge by 12.",
	"{Ultimate Sword\n(+12 Attack)}\n\nThe Ultimate Sword of Dominion increases your attack skill by 12.",
	"{Ultimate Cloak\n(+12 Defense)}\n\nThe Ultimate Cloak of Protection increases your defense skill by 12.",
	"{Ultimate Wand\n(+12 Spell Power)}\n\nThe Ultimate Wand of Magic increases your spell power by 12.",
	"{Ultimate Shield}\n\nThe Ultimate Shield increases your attack and defense skills by 6 each.",
	"{Ultimate Staff}\n\nThe Ultimate Staff increases your spell power and knowledge by 6 each.",
	"{Ultimate Crown}\n\nThe Ultimate Crown increases each of your basic skills by 4 points.",
	"{Golden Goose}\n\nThe Golden Goose brings in an income of 10,000 gold per turn.",
	"{Arcane Necklace\n(+4 Spell Power)}\n\nThe Arcane Necklace of Magic increases your spell power by 4.",
	"{Caster's Bracelet\n(+2 Spell Power)}\n\nThe Caster's Bracelet of Magic increases your spell power by 2.",
	"{Mage's Ring\n(+2 Spell Power)}\n\nThe Mage's Ring of Power increases your spell power by 2.",
	"{Witches Broach\n(+3 Spell Power)}\n\nThe Witch's Broach of Magic increases your spell power by 3.",
	"{Medal}\n\nThe Medal of Valor increases your morale.",
	"{Medal}\n\nThe Medal of Courage increases your morale.",
	"{Medal}\n\nThe Medal of Honor increases your morale.",
	"{Medal}\n\nThe Medal of Distinction increases your morale.",
	"{Fizbin}\n\nThe Fizbin of Misfortune greatly decreases your morale.",
	"{Thunder Mace\n(+1 Attack)}\n\nThe Thunder Mace of Dominion increases your attack skill by 1.",
	"{Armored Gauntlets\n(+1 Defense)}\n\nThe Armored Gauntlets of Protection increase your defense skill by 1.",
	"{Defender Helm\n(+1 Defense)}\n\nThe Defender Helm of Protection increases your defense skill by 1.",
	"{Giant Flail\n(+1 Attack)}\n\nThe Giant Flail of Dominion increases your attack skill by 1.",
	"{Ballista}\n\nThe Ballista of Quickness lets your catapult fire twice per combat round.",
	"{Stealth Shield\n(+2 Defense)}\n\nThe Stealth Shield of Protection increases your defense skill by 2.",
	"{Dragon Sword\n(+3 Attack)}\n\nThe Dragon Sword of Dominion increases your attack skill by 3.",
	"{Power Axe\n(+2 Attack)}\n\nThe Power Axe of Dominion increases your attack skill by 2.",
	"{Divine Breastplate\n(+3 Defense)}\n\nThe Divine Breastplate of Protection increases your defense skill by 3.",
	"{Minor Scroll\n(+2 Knowledge)}\n\nThe Minor Scroll of Knowledge increases your knowledge by 2.",
	"{Major Scroll\n(+3 Knowledge)}\n\nThe Major Scroll of Knowledge increases your knowledge by 3.",
	"{Superior Scroll\n(+4 Knowledge)}\n\nThe Superior Scroll of Knowledge increases your knowledge by 4.",
	"{Foremost Scroll\n(+5 Knowledge)}\n\nThe Foremost Scroll of Knowledge increases your knowledge by 5.",
	"{Endless Sack}\n\nThe Endless Sack of Gold provides you with 1000 gold per day.",
	"{Endless Bag}\n\nThe Endless Bag of Gold provides you with 750 gold per day.",
	"{Endless Purse}\n\nThe Endless Purse of Gold provides you with 500 gold per day.",
	"{Nomad Boots}\n\nThe Nomad Boots of Mobility increase your movement on land.",
	"{Traveler's Boots}\n\nThe Traveler's Boots of Mobility increase your movement on land.",
	"{Rabbit's Foot}\n\nThe Lucky Rabbit's Foot increases your luck in combat.",
	"{Horseshoe}\n\nThe Golden Horseshoe increases your luck in combat.",
	"{Coin}\n\nThe Gambler's Lucky Coin increases your luck in combat.",
	"{Clover}\n\nThe Four-Leaf Clover increases your luck in combat.",
	"{Compass}\n\nThe True Compass of Mobility increases your movement on land and sea.",
	"{Astrolabe}\n\nThe Sailors' Astrolabe of Mobility increases your movement on sea.",
	"{Evil Eye}\n\nThe Evil Eye reduces the casting cost of curse spells by half.",
	"{Enchanted hourglass}\n\nThe Enchanted Hourglass extends the duration of all your spells by 2 turns.",
	"{Gold watch}\n\nThe Gold Watch doubles the effectiveness of your hypnotize spells.",
	"{Skullcap}\n\nThe Skullcap halves the casting cost of all mind influencing spells.",
	"{Ice cloak}\n\nThe Ice Cloak halves all damage your troops take from cold spells.",
	"{Fire cloak}\n\nThe Fire Cloak halves all damage your troops take from fire spells.",
	"{Lightning helm}\n\nThe Lightning Helm halves all damage your troops take from lightning spells.",
	"{Evercold icicle}\n\nThe Evercold Icicle causes your cold spells to do 50% more damage to enemy troops.",
	"{Everhot lava rock}\n\nThe Everhot Lava Rock causes your fire spells to do 50% more damage to enemy troops.",
	"{Lightning rod }\n\nThe Lightning Rod causes your lightning spells to do 50% more damage to enemy troops.",
	"{Snake-ring}\n\nThe Snake Ring halves the casting cost of all your bless spells.",
	"{Ankh}\n\nThe Ankh doubles the effectiveness of all your resurrect and animate spells.",
	"{Book of Elements}\n\nThe Book of Elements doubles the effectiveness of all your summoning spells.",
	"{Elemental ring}\n\nThe Elemental Ring halves the casting cost of all summoning spells.",
	"{Holy Pendant}\n\nThe Holy Pendant makes all your troops immune to curse spells.",
	"{Pendant of free will}\n\nThe Pendant of Free Will makes all your troops immune to hypnotize spells.",
	"{Pendant of life}\n\nThe Pendant of Life makes all your troops immune to death spells.",
	"{Serenity pendant}\n\nThe Serenity Pendant makes all your troops immune to berserk spells.",
	"{Seeing-eye pendant}\n\nThe Seeing-eye Pendant makes all your troops immune to blindness spells.",
	"{Kinetic pendant}\n\nThe Kinetic Pendant makes all your troops immune to paralyze spells.",
	"{Pendant of death}\n\nThe Pendant of Death makes all your troops immune to holy spells.",
	"{Wand of negation}\n\nThe Wand of Negation protects your troops from the Dispel Magic spell.",
	"{Golden bow}\n\nThe Golden Bow eliminates the 50% penalty for your troops shooting past obstacles. (e.g. castle walls)",
	"{Telescope}\n\nThe Telescope increases the amount of terrain your hero reveals when adventuring by 1 extra square.",
	"{Statesman's quill}\n\nThe Statesman's Quill reduces the cost of surrender to 10% of the total cost of troops you have in your army.",
	"{Wizard's hat}\n\nThe Wizard's Hat increases the duration of your spells by 10 turns!",
	"{Power ring}\n\nThe Power Ring returns 2 extra spell points/turn to your hero.",
	"{Ammo cart}\n\nThe Ammo Cart provides endless ammunition for all your troops that shoot.",
	"{Tax lien}\n\nThe Tax Lien costs you 250 gold pieces/turn.",
	"{Hideous mask}\n\nThe Hideous Mask prevents all 'wandering' armies from joining your hero.",
	"{Endless pouch of sulfur}\n\nThe Endless Pouch of Sulfur provides 1 unit of sulfur per day.",
	"{Endless vial of mercury}\n\nThe Endless Vial of Mercury provides 1 unit of mercury per day.",
	"{Endless pouch of gems}\n\nThe Endless Pouch of Gems provides 1 unit of gems per day.",
	"{Endless cord of wood}\n\nThe Endless Cord of Wood provides 1 unit of wood per day.",
	"{Endless cart of ore}\n\nThe Endless Cart of Ore provides 1 unit of ore per day.",
	"{Endless pouch of crystal}\n\nThe Endless Pouch of Crystal provides 1 unit of crystal/day.",
	"{Spiked helm}\n\nThe Spiked Helm increases your attack and defense skills by 1 each.",
	"{Spiked shield}\n\nThe Spiked Shield increases your attack and defense skills by 2 each.",
	"{White pearl}\n\nThe White Pearl increases your spell power and knowledge by 1 each.",
	"{Black pearl}\n\nThe Black Pearl increases your spell power and knowledge by 2 each.",
	"{Magic Book}\n\nThe Magic Book enables you to cast spells.",
	"{ERROR}\n\nArtifact 82.",
	"{ERROR}\n\nArtifact 83.",
	"{ERROR}\n\nArtifact 84.",
	"{ERROR}\n\nArtifact 85.",
	"{Spell Scroll}\n\nThis Spell Scroll gives your hero the ability to cast the '%s' spell.",
	"{Arm of the Martyr}\n\nThe Arm of the Martyr increases your spell power by 3 but adds the undead morale penalty.",
	"{Breastplate of Anduran}\n\nThe Breastplate increases your defense by 5.",
	"{Broach of Shielding}\n\nThe Broach of Shielding provides 50% protection from Armageddon and Elemental Storm, but decreases spell power by 2.",
	"{Battle Garb}\n\nThe Battle Garb of Anduran combines the powers of the three Anduran artifacts.  It provides maximum luck and morale for your troops and gives you the Town Portal spell.",
	"{Crystal Ball}\n\nThe Crystal Ball lets you get more specific information about monsters, enemy heroes, and castles nearby the hero who holds it.",
	"{Heart of Fire}\n\nThe Heart of Fire provides 50% protection from fire, but doubles the damage taken from cold.",
	"{Heart of Ice}\n\nThe Heart of Ice provides 50% protection from cold, but doubles the damage taken from fire.",
	"{Helmet of Anduran}\n\nThe Helmet increases your spell power by 5.",
	"{Holy Hammer}\n\nThe Holy Hammer increases your attack skill by 5.",
	"{Legendary Scepter}\n\nThe Legendary Scepter adds 2 points to all attributes.",
	"{Masthead}\n\nThe Masthead boosts your luck and morale by 1 each in sea combat.",
	"{Sphere of Negation}\n\nThe Sphere of Negation disables all spell casting, for both sides, in combat.",
	"{Staff of Wizardry}\n\nThe Staff of Wizardry boosts your spell power by 5.",
	"{Sword Breaker}\n\nThe Sword Breaker increases your defense by 4 and attack by 1.",
	"{Sword of Anduran}\n\nThe Sword increases your attack skill by 5.",
	"{Spade of Necromancy}\n\nThe Spade gives you increased necromancy skill.",
	"{Pandora's Box}\n\nPandora's Box gives your hero's army a random stack of level 1 creatures for the duration of each battle.",
};

char* gArtifactEvents[] =
{
	"After rescuing a Sorceress from a cursed tomb, she rewards your heroism with an exquisite jeweled necklace.",
	"While searching through the rubble of a caved-in mine, you free a group of trapped Dwarves.  Grateful, the leader gives you a golden bracelet.",
	"A cry of pain leads you to a Centaur, caught in a trap.  Upon setting the creature free, he hands you a small pouch.  Emptying the contents, you find a dazzling jeweled ring.",
	"Alongside the remains of a burnt witch lies a beautiful broach, intricately designed.  Approaching the corpse with caution, you add the broach to your inventory.",
	"Freeing a virtuous maiden from the clutches of an evil overlord, you are granted a Medal of Valor by the King's herald.",
	"After saving a young boy from a vicious pack of Wolves, you return him to his father's manor.  The grateful nobleman awards you with a Medal of Courage.",
	"After freeing a princess of a neighboring kingdom from the evil clutches of despicable slavers, she awards you with a Medal of Honor.",
	"Ridding the countryside of the hideous Minotaur who made a sport of eating noblemen's Knights, you are honored with the Medal of Distinction.",
	"You stumble upon a medal lying alongside the empty road.  Adding the medal to your inventory, you become aware that you have acquired the undesirable Fizbin of Misfortune, greatly decreasing your army's morale.",
	"During a sudden storm, a bolt of lightning strikes a tree, splitting it.  Inside the tree you find a mysterious mace.",
	"You encounter the infamous Black Knight!  After a grueling duel ending in a draw, the Knight, out of respect, offers you a pair of armored gauntlets.",
	"A glint of golden light catches your eye.  Upon further investigation, you find a golden helm hidden under a bush.",
	"A clumsy Giant has killed himself with his own flail.  Knowing your superior skill with this weapon, you confidently remove the spectacular flail from the fallen Giant.",
	"Walking through the ruins of an ancient walled city, you find the instrument of the city's destruction, an elaborately crafted ballista.",
	"A stone statue of a warrior holds a silver shield.  As you remove the shield, the statue crumbles into dust.",
	"As you are walking along a narrow path, a nearby bush suddenly bursts into flames.  Before your eyes the flames become the image of a beautiful woman.  She holds out a magnificent sword to you.",
	"You see a silver axe embedded deeply in the ground.  After several unsuccessful attempts by your army to remove the axe, you tightly grip the handle of the axe and effortlessly pull it free.",
	"A gang of Rogues is sifting through the possessions of dead warriors.  Scaring off the scavengers, you note the Rogues had overlooked a beautiful breastplate.",
	"Before you appears a levitating glass case with a scroll, perched upon a bed of crimson velvet.  At your touch, the lid opens and the scroll floats into your awaiting hands.",
	"Visiting a local wiseman, you explain the intent of your journey.  He reaches into a sack and withdraws a yellowed scroll and hands it to you.",
	"You come across the remains of an ancient Druid.  Bones, yellowed with age, peer from the ragged folds of her robe.  Searching the robe, you discover a scroll hidden in the folds.",
	"Mangled bones, yellowed with age, peer from the ragged folds of a dead Druid's robe.  Searching the robe, you discover a scroll hidden within.",
	"A little leprechaun dances gleefully around a magic sack.  Seeing you approach, he stops in mid-stride.  The little man screams and stamps his foot ferociously, vanishing into thin air.  Remembering the old leprechaun saying 'Finders Keepers', you grab the sack and leave.",
	"A noblewoman, separated from her traveling companions, asks for your help.  After escorting her home, she rewards you with a bag filled with gold.",
	"In your travels, you find a leather purse filled with gold that once belonged to a great warrior king who had the ability to transform any inanimate object into gold.",
	"A Nomad trader seeks protection from a tribe of Goblins.  For your assistance, he gives you a finely crafted pair of boots made from the softest leather.  Looking closely, you see fascinating ancient carvings engraved on the leather.",
	"Discovering a pair of beautifully beaded boots made from the finest and softest leather, you thank the anonymous donor and add the boots to your inventory.",
	"A traveling merchant offers you a rabbit's foot, made of gleaming silver fur, for safe passage.  The merchant explains the charm will increase your luck in combat.",
	"An ensnared Unicorn whinnies in fright.  Murmuring soothing words, you set her free.  Snorting and stamping her front hoof once, she gallops off.  Looking down you see a golden horseshoe.",
	"You have captured a mischievous imp who has been terrorizing the region.  In exchange for his release, he rewards you with a magical coin.",
	"In the middle of a patch of dead and dry vegetation, to your surprise you find a healthy green four-leaf clover.",
	"An old man claiming to be an inventor asks you to try his latest invention.  He then hands you a compass.",
	"An old sea captain is being tortured by Ogres.  You save him, and in return he rewards you with a wondrous instrument to measure the distance of a star.",
	"While venturing into a decrepit hut you find the Skeleton of a long dead witch.  Investigation of the remains reveals a glass eye rolling around inside an empty skull.",
	"A surprise turn in the landscape finds you in the midst of a grisly scene:  Vultures picking at the aftermath of a terrible battle.  Your cursory search of the remains turns up an enchanted hourglass.",
	"In reward for helping his cart out of a ditch, a traveling potion salesman gives you a \"magic\" gold watch.  Unbeknownst to him, the watch really is magical.",
	"A brief stop at an improbable rural inn yields an exchange of money, tales, and accidentally, luggage.  You find a magical skullcap in your new backpack.",
	"Responding to the panicked cries of a damsel in distress, you discover a young woman fleeing from a hungry bear.  You slay the beast in the nick of time, and the grateful Sorceress weaves a magic cloak from the bear's hide.",
	"You've come upon a fight between a Necromancer and a Paladin.  The Necromancer blasts the Paladin with a fire bolt, bringing him to his knees.  Acting quickly, you slay the evil one before the final blow.  The grateful Paladin gives you the fire cloak that saved him.",
	"A traveling tinker in need of supplies offers you a helm with a thunderbolt design on its top in exchange for food and water.  Curious, you accept, and later find out that the helm is magical.",
	"An icicle withstanding the full heat of the noonday sun attracts your attention.  Intrigued, you break it off, and find that it does not melt in your hand.",
	"Your wanderings bring you into contact with a tribe of ape-like beings using a magical lava rock that never cools to light their fires.  You take pity on them and teach them to make fire with sticks.  Believing you to be a god, the apes give you their rock.",
	"While waiting out a storm, a lighting bolt strikes a nearby cottage's lightning rod, which melts and falls to the ground.  The tip of the rod, however, survives intact and makes your hair stand on end when you touch it.  Hmm...",
	"You've found an oddly shaped ring on the finger of a long dead traveler.  The ring looks like a snake biting its own tail.",
	"A fierce windstorm reveals the entrance to a buried tomb.  Your investigation reveals that the tomb has already been looted, but the thieves overlooked an ankh on a silver chain in the dark.",
	"You come across a conjurer who begs to accompany you and your army awhile for safety.  You agree, and he offers as payment a copy of the book of the elements.",
	"While pausing to rest, you notice a bobcat climbing a short tree to get at a crow's nest.  On impulse, you climb the tree yourself and scare off the cat.  When you look in the nest, you find a collection of shiny stones and a ring.",
	"In your wanderings you come across a hermit living in a small, tidy hut.  Impressed with your mission, he takes time out from his meditations to bless and give you a charm against curses.",
	"Responding to cries for help, you find river Sprites making a sport of dunking an old man.  Feeling vengeful, you rescue the man and drag a Sprite onto dry land for awhile.  The Sprite, uncomfortable in the air, gives you a magic pendant to let him go.",
	"A brief roadside encounter with a small caravan and a game of knucklebones wins a magic pendant.  Its former owner says that it protects from Necromancers' death spells.",
	"The sounds of combat draw you to the scene of a fight between an old Barbarian and an eight-headed Hydra.  Your timely intervention swings the battle in favor of the man, and he rewards you with a pendant he used to use to calm his mind for battle.",
	"You come upon a very old woman, long blind from cataracts and dying alone.  You tend to her final needs and promise a proper burial.  Grateful, she gives you a magic pendant emblazoned with a stylized eye.  It lets you see with your eyes closed.",
	"You come across a golem wearing a glowing pendant and blocking your way.   Acting on a hunch, you cut the pendant from its neck.  Deprived of its power source, the golem breaks down, leaving you with the magical pendant.",
	"A quick and deadly battle with a Necromancer wins you his magical pendant.  Later, a Wizard tells you that the pendant protects undead under your control from holy word spells.",
	"You meet an old Wizard friend of yours traveling in the opposite direction.  He presents  you with a gift:  A wand that prevents the use of the dispel magic spell on your allies.",
	"A chance meeting with a famous Archer finds you in a game of knucklebones pitting his bow against your horse.  You win.",
	"A merchant from far away lands trades you a new invention of his people for traveling supplies.  It makes distant objects appear closer, and he calls it...\n\na telescope.",
	"You pause to help a diplomat with a broken axle fix his problem.  In gratitude, he gives you a writing quill with magical properties which he says will \"help people see things your way\".",
	"You see a Wizard fleeing from a Griffin and riding like the wind.  The Wizard opens a portal and rides through, getting his hat knocked off by the edge of the gate.  The Griffin follows; the gate closes.  You pick the hat up, dust it off, and put it on.",
	"You find a small tree that closely resembles the great Warlock Carnauth with a ring around one of its twigs.  Scraps of clothing and rotting leather lead you to suspect that it IS Carnauth, transformed.  Since you can't help him, you take the magic ring.",
	"An ammunition cart in the middle of an old battlefield catches your eye.  Inspection shows it to be in good working order, so  you take it along.",
	"Your big spending habits have earned you a massive tax bill that you can't hope to pay.  The tax man takes pity and agrees to only take 250 gold a day from your account for life.  Check here if you want one dollar to go to the presidential campaign election fund.",
	"Your looting of the grave of Sinfilas Gardolad, the famous shapeshifting Warlock, unearths his fabled mask.  Trembling, you put it on and it twists your visage into an awful grimace!  Oh no!  It's actually the hideous mask of Gromluck Greene, and you are stuck with it.",
	"You visit an alchemist who, upon seeing your army, is swayed by the righteousness of your cause.  The newly loyal subject gives you his endless pouch of sulfur to help with the war effort.",
	"A brief stop at a hastily abandoned Wizard's tower turns up a magical vial of mercury that always has a little left on the bottom.  Recognizing a treasure when you see one, you cap it and slip it in your pocket.",
	"A short rainstorm brings forth a rainbow...and you can see the end of it.  Riding quickly, you seize the pot of gold you find there.  The leprechaun who owns it, unable to stop you from taking it, offers an endless pouch of gems for the return of his gold.  You accept.",
	"Pausing to rest and light a cook fire, you pull wood out of a nearby pile of dead wood.  As you keep pulling wood from the pile, you notice that it doesn't shrink.  You realize to your delight that the wood is enchanted, so you take it along.",
	"You've found a Goblin weapon smithy making weapons for use against humans.  With a tremendous yell you and your army descend upon their camp and drive them away.  A search finds a magic ore cart that never runs out of iron.",
	"Taking shelter from a storm in a small cave,  you notice a small patch of crystal in one corner.  Curious, you break a piece off and notice that the original crystal grows the lost piece back.  You decide to stuff the entire patch into a pouch and take it with you.",
	"Your army is ambushed by a small tribe of wild (and none too bright) Orcs.  You fend them off easily and the survivors flee in all directions.  One of the Orcs was wearing a polished spiked helm.  Figuring it will make a good souvenir, you take it.",
	"You come upon a bridge spanning a dry gully.  Before you can cross, a Troll steps out from under the bridge and demands payment before it will permit you to pass.  You refuse, and the Troll charges, forcing you to slay it.  You take its spiked shield as a trophy.",
	"A walk across a dry saltwater lake bed yields an unlikely prize:  A white pearl amidst shattered shells and debris.",
	"Rumors of a Griffin of unusual size preying upon the countryside lead you to its cave lair.  A quick, brutal fight dispatches the beast, and a search of its foul nest turns up a huge black pearl.",
	"",
	"ERROR : Artifact event 82.",
	"ERROR : Artifact event 83.",
	"ERROR : Artifact event 84.",
	"ERROR : Artifact event 85.",
	"You find an elaborate container which houses an old vellum scroll. The runes on the container are very old, and the artistry with which it was put together is stunning. As you pull the scroll out, you feel imbued with magical power.",
	"One of the less intelligent members of your party picks up an arm off of the ground.  Despite its missing a body, it is still moving.  Your troops find the dismembered arm repulsive, but you cannot bring yourself to drop it: it seems to hold some sort of magical power that influences your decision making.",
	"You come upon a sign.  It reads: \"Here lies the body of Anduran.  Bow and swear fealty, and you shall be rewarded.\"  You decide to do as it says.  As you stand up, you feel a coldness against your skin.  Looking down, you find that you are suddenly wearing a gleaming, ornate breastplate.",
	"A kindly Sorceress thinks that your army's defenses could use a magical boost.  She offers to enchant the Broach that you wear on your cloak, and you accept.",
	"Out of pity for a poor peasant, you purchase a chest of old junk they are hawking for too much gold. Later, as you search through it, you find it contains the 3 pieces of the legendary battle garb of Anduran!",
	"You come upon a caravan of gypsies who are feasting and fortifying their bodies with mead.  They call you forward and say \"If you prove that you can dance the Rama-Buta, we will reward you.\"  You don't know it, but try anyway.  They laugh hysterically, but admire your bravery, giving you a Crystal Ball.",
	"You enter a recently burned glade and come upon a Fire Elemental sitting atop a rock.  It looks up, its flaming face contorted in a look of severe pain.  It then tosses a glowing object at you.  You put up your hands to block it, but it passes right through them and sears itself into your chest.",
	"Suddenly, a biting coldness engulfs your body.  You seize up, falling from your horse.  The pain subsides, but you still feel as if your chest is frozen.  As you pick yourself up off of the ground, you hear hearty laughter.  You turn around just in time to see a Frost Giant run off into the woods and disappear.",
	"You spy a gleaming object poking up out of the ground.   You send a member of your party over to investigate.  He comes back with a golden helmet in his hands.  You realize that it must be the helmet of the legendary Anduran, the only man who was known to wear solid gold armor.",
	"You come upon a battle where a Paladin has been mortally wounded by a group of Zombies.  He asks you to take his hammer and finish what he started.  As you pick it up, it begins to hum, and then everything becomes a blur.  The Zombies lie dead, the hammer dripping with blood.  You strap it to your belt.",
	"Upon cresting a small hill, you come upon a ridiculous looking sight.  A Sprite is attempting to carry a Scepter that is almost as big as it is.  Trying not to laugh, you ask, \"Need help?\"  The Sprite glares at you and answers: \"You think this is funny?  Fine.  You can carry it.  I much prefer flying anyway.\"",
	"An old seaman tells you a tale of an enchanted masthead that he used in his youth to rally his crew during times of trouble.  He then hands you a faded map that shows where he hid it.  After much exploring, you find it stashed underneath a nearby dock.",
	"You stop to help a Peasant catch a runaway mare.  To show his gratitude, he hands you a tiny sphere.  As soon as you grasp it, you feel the magical energy drain from your limbs...",
	"While out scaring up game, your troops find a mysterious staff levitating about three feet off of the ground.  They hand it to you, and you notice an inscription.  It reads: \"Brains best brawn and magic beats might.  Heed my words, and you'll win every fight.\"",
	"A former Captain of the Guard admires your quest and gives you the enchanted Sword Breaker that he relied on during his tour of duty.",
	"A Troll stops you and says: \"Pay me 5,000 gold, or the Sword of Anduran will slay you where you stand.\"  You refuse.  The troll grabs the sword hanging from its belt, screams in pain, and runs away.  Picking up the fabled sword, you give thanks that half-witted Trolls tend to grab the wrong end of sharp objects.",
	"A dirty shovel has been thrust into a dirt mound nearby. Upon investigation, you discover it to be the enchanted shovel of the Gravediggers, long thought lost by mortals.",
	"Pandora's Box acquired"
};