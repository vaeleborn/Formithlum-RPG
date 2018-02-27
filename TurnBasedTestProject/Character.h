/********************************************************
*	NEXT THING TO DO: Stat array to be contained
	in a single struct
*****************************************************/

#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <map>


using namespace std;

struct Spell;
struct PhysicalAbility;


/***************************************
* Purpose: An instance of a character in game
***************************************/
class Character
{
private:
	string mName;		//Holds the character's name
	int mHealth;		//Holds the character's health
	int mHealthCap;		//Holds the character's health cap( can be modified )
	int mConstHPCap;		//Holds the character's health cap( can't be modified )
	int mManaCap;		//Holds the character's mana cap (can be changed)
	int mConstMPCap;		//Hold the character's mana cap (can't be changed)
	int mMana;			//Holds the character's mana
	bool mDead;			//Checks if character is dead
	bool mPC;			//Checks if character is controlled by the user 

	int mXp;			//Current experience of the character
	int mXPToLevel;		//Experience needed for the character to level

	int mLevel;			//Current level of the character


	//Character Stats
	int mStamina;			//Affects max health
	int mStrength;			//Affects max physical damage
	int mDexterity;			//Affects chance to hit
	int mIntelligence;		//Affects max magical damage
	int mWisdom;			//Affects mana costs
	int stats[5];

	int mHealthMod;			//Affects the health modifier( a set number of extra HP determined by users stamina )
	float mManaMod;			//Affects the mana modifier( what percentage of the base mana cost of a spell is needed to cast it affected by intelligence )
	float mCastChanceMod;	//Affects the casting chance modifier ( what percentage extra of the base percent cast chance is added to the cast chance affected by wisdom and intelligence )
	float mPhyDmgMod;		//Affects the physical damage modifier ( what percent of extra damage is done based on the base damage and the characters strength )
	float mMagDmgMod;		//Affects the magical damage modifier ( what percent of extra effectiveness a spell has based on wisdom )
	float mHitMod;			//Affects the hit chance modifier ( what percent extra of the base hit chance is added based on dexterity )
public:
	Character();		//Default Player constructor, sets stats to default values
	Character(string name, bool playerCharacter = false, int startHealth = 100, int startMana = 50);	//Constructor For Player, sets the name, if it is player controlled, starting health and mana


	map<string, Spell> spells;			//A map that contains all the spells known by this character
	map<string, PhysicalAbility> abilities;

	
	void CastSpell(string name);									//Function to cast a spell given it's name
	int GetManaCost(string name);									//Function to get the mana cost of a spell given it's name
	void UseAbility(string name);
	
	void InitStats(int stats[5]);									//Function to initialize the character's stats based on an array of integers
	void UpdateStats();												//Function to update the character's stats including the modifiers

	void TakeDamage(int dmg) { mHealth -= dmg; }					//Function to take damage
	void SubtractMana(int amount) { mMana -= amount; }				//Function to reduce the character's mana by a set amount, used for spells

	void Heal(int amount)											//Function that heals the character and makes sure their health does not exceed their cap
	{
		mHealth += amount;				//Adds the amount to heal to the character's health						
		if (mHealth > mHealthCap)			//If the character's health is greater than their max
			mHealth = mHealthCap;			//Set the health to their max
	}

	void BasicAttack(Character &target, int dmg);						//Function to deal damage to an enemy
	void Rest();													//Function to rest a turn restoring mana

	void SetSpellTarget(string name, Character* t);							//Function to set the target of a spell given the spells name and a pointer to the target
	void SetAbilityTarget(string name, Character* t);


	int GetXp() { return mXp; }
	int GetXpToLevel() {return mXPToLevel; }
	void AddXp(int amt) { mXp += amt; }
	void LevelUp() { 
		mXp = 0; 
		mLevel++;
	}
	int GetLevel() { return mLevel; }

	int* GetStats() {
		//int stats[5] = { mStamina, mStrength, mDexterity, mIntelligence, mWisdom };
		stats[0] = mStamina;
		stats[1] = mStrength;
		stats[2] = mDexterity;
		stats[3] = mIntelligence;
		stats[4] = mWisdom;
		return stats;
	}


	int GetHealth() { return mHealth; }								//Function to get the health of the character
	int GetMana() { return mMana; }									//Function to get the mana of the character
	int GetMaxHealth() { return mManaCap; }							//Function to get the health cap
	int GetMaxMana() { return mManaCap; }							//Function to get the mana cap
	string GetName() { return mName; }								//Function to get the name of the character

	int GetHealthMod() { return mHealthMod; }						//Function to get the health modifier of the character
	float GetManaMod() { return mManaMod; }							//Function to get the mana modifier of the character
	float GetPhyDmgMod() { return mPhyDmgMod; }						//Function to get the physical damage modifier of the character
	float GetMagDmgMod() { return mMagDmgMod; }						//Function to get the magical damage modifier of the character
	float GetHitMod() { return mHitMod; }							//Function to get the hit chance modifier of the character
	float GetCastChanceMod() { return mCastChanceMod; }				//Function to get the casting chance modifier of the character

	void ResetStats();												//Function to reset the character's stats

	bool CheckDead();												//Function to check if the character is dead
	bool CheckPlayerCharacter();									//Function to check if the character is controlled by the user

	~Character();
};

