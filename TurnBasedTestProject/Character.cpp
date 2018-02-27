#include "Character.h"
#include "Globals.h"


Character::Character()
{
}

/****************************************************************
*	Purpose: Constructor for the Player class that sets
*			the name, whether or not it is a player character,
*			the starting health and mana of the character and
*			sets a bool saying the player is alive
****************************************************************/
Character::Character(string name, bool playerCharacter, int startHealth, int startMana)
{
	mName = name;				//Sets the name of the player
	mHealth = startHealth;		//Sets the starting health of the player
	mMana = startMana;			//Sets the starting mana of the player
	mHealthCap = startHealth;	//Sets the cap for the health
	mConstHPCap = startHealth;	//Sets the constant base healthcap
	mManaCap = startMana;		//Sets the cap for mana
	mConstMPCap = startMana;	//Sets the constant base mana cap
	mDead = false;				//Sets the player to be alive
	mPC = playerCharacter;		//Sets whether the player is controlled by the user

	mStamina = 0;				//Sets the character's stamina to a default value of 0
	mStrength = 0;				//Sets the character's strength to a default value of 0
	mDexterity = 0;				//Sets the character's dexterity to a default value of 0
	mIntelligence = 0;			//Sets the character's intelligence to a default value of 0
	mWisdom = 0;				//Sets the character's wisdom to a default value of 0

	mHealthMod = 0;				//Sets the character's health modifier to a default value of 0
	mManaMod = 0;				//Sets the character's mana modifier to a default value of 0
	mCastChanceMod = 0;			//Sets the character's casting chance modifier to a default value of 0
	mPhyDmgMod = 0;				//Sets the character's physical damage modifier to a default value of 0
	mMagDmgMod = 0;				//Sets the character's magic damage modifier to a default value of 0
	mHitMod = 0;				//Sets the character's hit chance modifier to a default value of 0

	mXp = 0;
	mXPToLevel = 100;
	mLevel = 1;

	spells = map<string, Spell>();	//Initializaes the character's spells to a empty map

	Spell heal("Heal", 20, -20, 100, 82, this);				//Creates a spell named heal that has a base cost of 13 mana, heals for 18, 100% hit chance, 82% cast chance, and is cast by this character
	Spell fireBall("Fire Ball", 35, 22, 88, 79, this);		//Creates a spell named fire ball that has a base cost of 30 mana, hits for 25, 88% hit chance, 79% cast chance, and is cast by this character

	PhysicalAbility basicAttack("Basic Attack", 8, 13, 100, this);
	PhysicalAbility slash("Slash", 19, 23, 80, this);
	PhysicalAbility blindRage("Blind Rage", 40, 60, 25, this);

	spells["Heal"] = heal;				//Adds the heal spell to this character's spells
	spells["Fire Ball"] = fireBall;		//Adds the fireball spell to this character's spells

	abilities["Basic Attack"] = basicAttack;
	abilities["Slash"] = slash;
	abilities["Blind Rage"] = blindRage;
}



void Character::SetAbilityTarget(string name, Character* t)
{
	abilities[name].target = t;
}

void Character::UseAbility(string name)
{
	abilities[name].UseAbility();
}

/**************************************************************
*	Purpose: Gets the mana cost of a spell given its name
**************************************************************/
int Character::GetManaCost(string name)
{
	return spells[name].manaCost;	//Returns the mana cost for this character to cast a spell with the name 'name'
}

/**************************************************************
*	Purpose: Sets the target of a spell given the name of the
*			spell and a pointer to the target, use before 
*			casting the spell		
**************************************************************/
void Character::SetSpellTarget(string name, Character* t)
{
	spells[name].target = t;	//Sets the target of spell 'name'
}

/**************************************************************
*	Purpose: Cast a spell given the name of it
**************************************************************/
void Character::CastSpell(string name)
{
	spells[name].CastSpell();	//Cast the spell 'name'
}

/**************************************************************
*	Purpose: Initializes stats given an array containing their
*				values
**************************************************************/
void Character::InitStats(int stats[5])
{
	mStamina = stats[0];		//Sets the character's stamina
	mStrength = stats[1];		//Sets the character's strength
	mDexterity = stats[2];		//Sets the character's dexterity
	mIntelligence = stats[3];	//Sets the character's intelligence
	mWisdom = stats[4];			//Sets the character's wisdom
}

/*************************************************************
*	Purpose: Updates the characters modifiers and primes their
*			spells
*************************************************************/
void Character::UpdateStats()
{
	mHealthMod = 0;			//Resets the health modifier to be refound
	mManaMod = 0;			//Resets the mana modifier to be refound
	mCastChanceMod = 0;		//Resets the casting chance modifier to be refound
	mPhyDmgMod = 0;			//Resets the physical damage modifier to be refound
	mMagDmgMod = 0;			//Resets the magical damage modifier to be refound
	mHitMod = 0;			//Resets the hit chance modifier to be refound


	/********************************************
	*	Special Note: In the following sections
	*		to view the equations used to determine
	*		values see formulas.txt
	********************************************/

#pragma region stamina
	//Sets the Health modifier
	{
		mHealthCap = mConstHPCap;	//Resets health cap to its base value

		if (mStamina > 99)		
			mStamina = 99;			//Caps the character's stamina at 99

		int tempStaminaValue = mStamina;	//Temporary stamina value set to the character's stamina used to loop through each tier of the equation to determine health bonus

		if (tempStaminaValue > 72)						//If the temp stamina and therefore the starting stamina is > 72
		{
			int modValue = tempStaminaValue - 72;		//Set a modifier value to the tempStaminaValue - 72 ( so 99 becomes 27, there are 27 levels in this tier )
			tempStaminaValue -= modValue;				//Subtracts the mod value from the tempStamina value ( this will bring temp stamina down to the previous tier to calculate it too)
			mHealthMod += modValue;						//Adds the modValue to the health mod( this tier increased HP by 1 per level so level 72-99 gives 27 HP total )
		}

		if (tempStaminaValue > 50)						//If the temp stamina is > 50 & <= 72
		{
			int modValue = tempStaminaValue - 50;		//Set a modifier value to the tempStaminaValue - 50 ( so 72 becomes 22, there are 22 levels in this tier )
			tempStaminaValue -= modValue;				//Subtract mod value from temp stamina to proceed to lower tier after calculations and levels in this tier
			mHealthMod += modValue * 5;					//Add 5 times the mod value to the health modifier ( each level in this tier gives 5HP so 50-72 gives 110 HP total )
		}

		if (tempStaminaValue > 24)						//If the temp stamina is > 24 & <= 50
		{
			int modValue = tempStaminaValue - 24;		//Set a modifier value to the tempStaminaValue - 24 ( so 50 becomes 26, there are 26 levels in this tier )
			tempStaminaValue -= modValue;				//Subtract mod value from temp stamina to proceed to lower tier after calculations and levels in this tier
			mHealthMod += modValue * 10;				//Add 10 times the mod value to the health modifier ( each level in this tier gives 10HP so 24-50 gives  260 HP total )
		}

		if (tempStaminaValue > 0)						//If the temp stamina is > 0 & <= 24
		{
			int modValue = tempStaminaValue;			//Set a modifier value to the tempStaminaValue ( so 24 becomes 24 and 0 will be 0, there are 24 levels in this tier )
			tempStaminaValue -= modValue;				//Subtract mod value from tempstamina to go through the levels in this tier
			mHealthMod += modValue * 15;				//Add 15 times the mod value to the health modifier ( each level in this tier gives 15HP so 0-24 gives  360 HP total )
		}

		mHealthCap += mHealthMod;						//Add the health modifier to this characters health cap
	}
#pragma endregion stamina

#pragma region strength
	//Physical damage modifier
	{
		if (mStrength > 99)
			mStrength = 99;					//Caps the character's strength at 99

		float tempStrengthValue = mStrength;	//Temporary strength value set to the character's strength used to loop through each tier of the equation to determine physical damage bonus			

		if (tempStrengthValue > 72.f)						//If the temp stamina is > 72
		{
			float modValue = tempStrengthValue - 72.f;		//Set a modifier value to the temp strength value - 72 ( so 99 becomes 27, there are 27 levels in this tier )
			tempStrengthValue -= modValue;					//Subtract mod value from tempstamina to go through the levels in this tier
			mPhyDmgMod += modValue * 1.5f;					//Add 1.5 times the mod value to the physical damage bonus percentage ( each level in this tier gives a 1.5% bonus for a total of 40.5%
		}

		if (tempStrengthValue > 50.f)						//If the temp stamina is > 50 & <= 72
		{
			float modValue = tempStrengthValue - 50.f;		//Set a modifier value to the temp strength value - 50 ( so 72 becomes 22, there are 22 levels in this tier )
			tempStrengthValue -= modValue;					//Subtract mod value from tempstamina to go through the levels in this tier
			mPhyDmgMod += 2.72f * modValue;					//Add 2.72 times the mod value to the physical damage bonus percentage ( each level in this tier gives a 2.72% bonus for a total of 59.84%
		}

		if (tempStrengthValue > 24.f)
		{
			float modValue = tempStrengthValue - 24.f;		//Set a modifier value to the temp strength value - 24 (so 50 becomes 26, there are 26 levels in this tier)
			tempStrengthValue -= modValue;					//Subtract mod value from tempstamina to go through the levels in this tier
			mPhyDmgMod += modValue * 3.85f;					//Add 3.85 times the mod value to the physical damage bonus percentage ( each level in this tier gives a 3.85% bonus for a total of 100.1%
		}

		if (tempStrengthValue > 0.f)
		{
			float modValue = tempStrengthValue;				//Set a modifier value to the temp strength value (so 0 becomes 0, there are 24 levels in this tier)
			tempStrengthValue -= modValue;					//Subtract mod value from tempstamina to go through the levels in this tier
			mPhyDmgMod += 8.3f * modValue;					//Add 8.3 times the mod value to the physical damage bonus percentage ( each level in this tier gives a 8.3% bonus for a total of 199.2%
		}
	}
#pragma endregion strength

#pragma region dexterity
	//Hit chance modifier
	{
		if (mDexterity > 99)
			mDexterity = 99;			//Caps the characters dexterity at 99

		float tempDexValue = mDexterity;				//Temporary dexterity value used to loop through the levels and tiers for the modifier

		if (tempDexValue > 72.f)						//If the temp value is > 72
		{
			float modValue = tempDexValue - 72.f;		//Set a mod value to the temp value - 72 ( so 99 becomes 27, there are 27 levels in this tier )
			tempDexValue -= modValue;					//Subtracts the mod value from the temp value to loop to the next tier
			mHitMod += modValue * 0.26f;				//Adds 0.26 * the mod value to the hit chance modifier( each level gives .26% more than the base for a total of 6.76% more this tier)
		}
		if (tempDexValue > 50.f)						//If the temp value is > 50 & <= 72
		{
			float modValue = tempDexValue - 50.f;		//Set a mod value to the temp value - 50 ( so 72 becomes 22, there are 22 levels in this tier )
			tempDexValue -= modValue;					//Subtracts the mod value from the temp value to loop to the next tier
			mHitMod += modValue * 1.28f;				//Adds 1.28 * the mod value to the hit chance modifier( each level gives 1.28% more than the base for a total of 28.16% more this tier)
		}
		if (tempDexValue > 24.f)						//If the temp value is > 24 & <= 50
		{
			float modValue = tempDexValue - 24.f;		//Set a mod value to the temp value - 24 ( so 50 becomes 26, there are 26 levels in this tier )
			tempDexValue -= modValue;					//Subtracts the mod value from the temp value to loop to the next tier
			mHitMod += modValue * 1.54f;				//Adds 1.54 * the mod value to the hit chance modifier( each level gives 1.54% more than the base for a total of 40.04% more this tier)
		}
		if (tempDexValue > 0.f)							//If the temp value is > 0 and <= 24
		{
			float modValue = tempDexValue;				//Set a mod value to the temp value ( so 0 is 0, there are 24 levels this tier )
			tempDexValue -= modValue;					//Subtracts the mod value from the temp value to loop to the next tier
			mHitMod += modValue * 2.08f;				//Adds 2.08 * the mod value to the hit chance modifier( each level gives 2.08% more than the base for a total of 49.92% more this tier)
		}

	}
#pragma endregion dexterity

#pragma region intelligence 
	//Mana Mod
	{
		if (mIntelligence > 99)
			mIntelligence = 99;					//Caps the characters intelligence at 99

		mManaCap = mConstMPCap;							//Resets the mana cap to the base
		int tempManaMod = 0;							//Variable to add to the mana capS

		int tempIntValue = mIntelligence;
		if (tempIntValue > 72)							//If the temp value is > 72
		{
			int modValue = tempIntValue - 72;			//Set a mod value to the temp value - 72 ( so 99 becomes 27, there are 27 levels in this tier )
			tempIntValue -= modValue;					//Subtracts the mod value from the temp value to loop to the next tier
			mManaMod += 0.37f * modValue;				//Adds .37 * the mod value to the mana cost modifier( each level this tier reduces the mana cost by .37% for a total of 9.99% ) 
			tempManaMod += modValue;					//Adds the modvalue to the temp mana mod ( each level this tier increases max mana by 1 for a total 27 )
		}
		if (tempIntValue > 50)							//If the temp value is > 50 & <= 72
		{
			int modValue = tempIntValue - 50;			//Set a mod value to the temp value - 50 ( so 72 becomes 22, there are 22 levels in this tier )
			tempIntValue -= modValue;					//Subtracts the mod value from the temp value to loop to the next tier
			mManaMod += modValue * 0.68f;				//Adds .68 * the mod value to the mana cost modifier( each level this tier reduces the mana cost by .68% for a total of 14.95% ) 
			tempManaMod += 5 * modValue;				//Adds the modvalue to the temp mana mod ( each level this tier increases max mana by 5 for a total 110 )
		}
		if (tempIntValue > 24)							//If the temp value is > 24 & <= 50
		{
			int modValue = tempIntValue - 24;			//Set a mod value to the temp value - 24 ( so 50 becomes 26, there are 26 levels in this tier )
			tempIntValue -= modValue;					//Subtracts the mod value from the temp value to loop to the next tier
			mManaMod += modValue * 0.96f;				//Adds .96 * the mod value to the mana cost modifier( each level this tier reduces the mana cost by .96% for a total of 24.96% ) 
			tempManaMod += 10 * modValue;				//Adds the modvalue to the temp mana mod ( each level this tier increases max mana by 10 for a total 260 )
		}
		if (tempIntValue > 0)							//If the temp value is > 0 and <= 24
		{
			int modValue = tempIntValue;				//Set a mod value to the temp value ( so 0 is 0, there are 24 levels this tier )
			tempIntValue -= modValue;					//Subtracts the mod value from the temp value to loop to the next tier
			mManaMod += modValue * 1.67f;				//Adds 1.64 * the mod value to the mana cost modifier( each level this tier reduces the mana cost by 1.64% for a total of 39.36% ) 
			tempManaMod += 20 * modValue;				//Adds the modvalue to the temp mana mod ( each level this tier increases max mana by 20 for a total 480 )
		}
		mManaCap += tempManaMod;						//Adds the tempManaMod to the manacap
	}
#pragma endregion intelligence

#pragma region wisdom
	//Magic damage mod
	{
		if (mWisdom > 99)
			mWisdom = 99;									//Caps the characters wisdom at 99
		float tempWisdomValue = mWisdom;

		if (tempWisdomValue > 72.f)							//If the temp value is > 72
		{
			float modValue = tempWisdomValue - 72.f;		//Set a mod value to the temp value - 72 ( so 99 becomes 27, there are 27 levels in this tier )
			tempWisdomValue -= modValue;					//Subtracts the mod value from the temp value to loop to the next tier
			mMagDmgMod += modValue * 1.5f;					//Adds 1.5 * modvalue to the magic damage modifier ( each level this tier adds 1.5% base damage for a total of 40.5% )
		}

		if (tempWisdomValue > 50.f)							//If the temp value is > 50 & <= 72
		{
			float modValue = tempWisdomValue - 50.f;		//Set a mod value to the temp value - 50 ( so 72 becomes 22, there are 22 levels in this tier )
			tempWisdomValue -= modValue;					//Subtracts the mod value from the temp value to loop to the next tier
			mMagDmgMod += 2.72f * modValue;					//Adds 2.72 * modvalue to the magic damage modifier ( each level this tier adds 2.72% base damage for a total of 59.84% )
		}

		if (tempWisdomValue > 24.f)							//If the temp value is > 24 & <= 50
		{
			float modValue = tempWisdomValue - 24.f;		//Set a mod value to the temp value - 24 ( so 50 becomes 26, there are 26 levels in this tier )
			tempWisdomValue -= modValue;					//Subtracts the mod value from the temp value to loop to the next tier
			mMagDmgMod += modValue * 3.85f;					//Adds 3.85 * modvalue to the magic damage modifier ( each level this tier adds 3.85% base damage for a total of 100.1% )
		}

		if (tempWisdomValue > 0.f)							//If the temp value is > 0 and <= 24
		{
			float modValue = tempWisdomValue;				//Set a mod value to the temp value ( so 0 is 0, there are 24 levels this tier )
			tempWisdomValue -= modValue;					//Subtracts the mod value from the temp value to loop to the next tier
			mMagDmgMod += 8.3f * modValue;					//Adds8.3 * modvalue to the magic damage modifier ( each level this tier adds 8.3% base damage for a total of 199.2% )
		}
	}
#pragma endregion wisdom

#pragma region Cast Chance
	{
		float tempCastChanceVal = (mIntelligence + mWisdom) / 1.2f;			//Sets a max value based on intelligence and wisdom added together and divided by 1.2

		if (tempCastChanceVal > 86.2f)						//If the temp value is >= 86.2
		{
			float modValue = tempCastChanceVal - 86.2f;		//Set a mod value to temp value minus 86.2 so ( 198 becomes 111.8 )
			tempCastChanceVal -= modValue;					//Subtracts the mod value from the temp value to loop to the next tier
			mCastChanceMod += modValue * 0.09f;				//Adds .09 * mod value to casting chance
		}
		if (tempCastChanceVal > 50.f)						//If the temp value is > 50 & < 86.2
		{
			float modValue = tempCastChanceVal - 50.f;		//Set a mod value to temp value minus 50 so ( 86.2 becomes 36.2  )
			tempCastChanceVal -= modValue;					//Subtracts the mod value from the temp value to loop to the next tier
			mCastChanceMod += modValue * 0.77f;				//Adds 0.77 * mod value to the casting chance
		}
		if (tempCastChanceVal > 28.8f)						//If the temp value is > 28.8 & <= 50
		{
			float modValue = tempCastChanceVal - 28.8f;		//Set a mod value to temp value minus 28.8 so ( 50 becomes 21.2 )
			tempCastChanceVal -= modValue;					//Subtracts the mod value from the temp value to loop to the next tier
			mCastChanceMod += modValue * 1.89f;				//Adds 1.89 * mod value to the casting chance percentage
		}
		if (tempCastChanceVal > 0.f)						//If the temp value is > 0 and <= 28.8
		{
			float modValue = tempCastChanceVal;				//Set a mod value to temp valu( 0 becomes 0 )
			tempCastChanceVal -= modValue;					//Subtracts the mod value from the temp value to loop to the next tier
			mCastChanceMod += modValue * 1.73f;				//Adds 1.73 * modvalue to the casting chance
		}
	}
#pragma endregion Cast Chance

	std::map<std::string, Spell>::iterator it = spells.begin();	//An iterator to loop through the character's spells
	while (it != spells.end())			//Loops through the character's spells priming each one
	{
		it->second.PrimeSpell();
		it++;
	}

	std::map<std::string, PhysicalAbility>::iterator iter = abilities.begin();	//An iterator to loop through the character's spells
	while (iter != abilities.end())			//Loops through the character's spells priming each one
	{
		iter->second.PrimeAbility();
		iter++;
	}

}

/***************************************************
*	Purpose: Resets the stats of the character
***************************************************/
void Character::ResetStats()
{
	mHealth = mHealthCap;		//Resets health to max
	mMana = mManaCap;			//Resets mana to max
}

/***************************************************************
*	Purpose: Restores the characters mana to it's cap
***************************************************************/
void Character::Rest()
{
	system("cls");																//Clears the screen
	mMana = mManaCap;																//Sets the current mana to the max mana
	
	cout << mName << " has rested, restoring their mana to " << mMana;				//Displays that the character has restored their mana

	cout << "\n\n\n\n";															//Adds line breaks
	system("pause");															//Waits for user input
}

/***************************************************************
*	Purpose: Attacks another player for a set amount of damage
*			and checks if it killed that player or not
***************************************************************/
void Character::BasicAttack(Character &target, int damage)
{
	damage += damage * (mPhyDmgMod/100);					//Adds damage modifier to basic damage
	target.TakeDamage(damage);																		//Calls the take damage function of the target to deal damage 
	system("cls");																						//Clears the screen
	cout << GetName() << " attacked " << target.GetName() << " for " << damage << " damage\n";			//Says that this character attacked the target and for how much
	if (target.CheckDead())																		//If the target died
	{	
		cout << "\n" << target.GetName() << " is now dead";										//Display that the target is now dead
	}
	else																						//Otherwise
	{
		cout << "\n" << target.GetName() << " is now at " << target.GetHealth() << " health";	//Show what health the target is now at
	}

	cout << "\n\n\n\n";										//Add line breaks 
	system("pause");										//Wait for user to continue
}

/******************************************************************
*	Purpose: Checks if this player is dead 
*******************************************************************/
bool Character::CheckDead()
{
	bool dead = false;	//Bool to be returned

	if (mHealth <= 0)		//If the health is less than or equal to zero set dead to true
	{
		dead = true;
	}
	else					//Otherwise set dead to false
	{
		dead = false;		
	}
	return dead;			//Return the value of the dead bool
}

/*****************************************************************
*	Purpose: Checks if the character is controlled by the user
*****************************************************************/
bool Character::CheckPlayerCharacter()
{
	bool pc = false;	//Variable to be returned

	if (mPC)			//If it is a Character character set the bool to true
		pc = true;
	else				//Otherwise set the bool to false
		pc = false;

	return pc;			//Return the value of the pc bool
}

Character::~Character()
{
}
