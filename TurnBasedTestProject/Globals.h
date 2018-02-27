#pragma once
#include <iostream>
#include <string>

using namespace std;

class Character;

/*******************************************************************
* Purpose: A structure which can be used to design spells that deal
*		damage or heal a single target
*
*		Note: Could be adjusted to target multiple things in the
*				future
*******************************************************************/
struct Spell
{
private:
	//This function will display an appropriate message letting the user know their cast failed and why
	void DisplayFailedCast(bool noMana = false, bool missed = false);

	//This function will display the damage done...or healed from the cast
	void DisplayDamage();

public:
	string spellName;	//Contains the name of this spell

	Character* caster;		//A pointer to the character who is casting the spell
	Character* target;		//A pointer to the character who is the target of the spell

	int baseManaCost;		//The base amount of mana this spell will cost
	int baseDmg;			//The base amount of damage this spell will deal or heal
	int baseHitChance;		//The base percent chance this spell has to hit
	int baseCastChance;		//The base percent chance this spell has to be cast

	bool heal;			//Used to see if this spell heals or does damage
	int dmg;			//The amount of damage this spell will deal or heal after calculating with characters stats
	int hitChance;		//The chance this spell has to hit after calculating with characters stats
	float castChance;	//The chance this spell has to be cast after calculating with characters stats
	int manaCost;		//The amount of mana this spell will cost after calculating with characters stats

	//Constructor that sets up base values
	Spell(string name, int bManaCost, int bDmg, int bHitChance, float bCastChance, Character* c);

	//Default Constructor
	Spell();

	//Function to set up the costs of the spell and what it will do
	void PrimeSpell();

	//Function to cast the spell
	void CastSpell();
};

struct PhysicalAbility
{
private:
	//This function will display the damage done
	void DisplayDamage();

public:
	string abilityName;

	Character* attacker;
	Character* target;

	int minBaseDamage;
	int maxBaseDamage;
	float baseHitChance;

	int damage;
	float hitChance;

	//Constructor that suts up base values
	PhysicalAbility(string name, int minDmg, int maxDmg, float bHitChance, Character* atkChar);

	//Default Construct
	PhysicalAbility();

	//Function to set up effects of ability
	void PrimeAbility();

	//Function to use the ability
	void UseAbility();
};

/************************************
*	Struct to hold a x, y coordinate
*************************************/
struct Coordinate
{
	int x;
	int y;
};
