#include "Globals.h"
#include "Character.h"

#pragma region Spells
/**************************************************************************
*	Purpose: Default constructor just sets some default values
**************************************************************************/
Spell::Spell()
{
	dmg = 0;				//Sets damage to 0
	baseCastChance = 0;		//Sets cast chance to 0
	hitChance = 0;			//Sets hit chance to 0
	manaCost = 0;			//Sets mana cost to 0
	heal = false;			//Sets heal to false
}

/**********************************************************************
*	Purpose: Constructor to set up important values
**********************************************************************/
Spell::Spell(string name, int bManaCost, int bDmg, int bHitChance, float bCastChance, Character* c)
{
	spellName = name;				//Sets the name of this spell
	baseManaCost = bManaCost;		//Sets the base mana cost of this spell
	baseDmg = bDmg;					//Sets the base damage of this spell
	baseHitChance = bHitChance;		//Sets the base chance to hit with this spell
	hitChance = baseHitChance;		//Sets the chance to hit with this spell
	baseCastChance = bCastChance;	//Sets the base chance to cast this spell
	castChance = bCastChance;		//Sets the chance to cast this spell
	caster = c;						//Sets who is casting this spell
	PrimeSpell();					//Primes the spell to be casted
}

/*****************************************************************
*	Purpose: Displays to the user why a spell failed to cast
*****************************************************************/
void Spell::DisplayFailedCast(bool noMana, bool missed)
{
	system("cls");																							//Clears screen
	if (noMana)																										//If the caster didn't have enough mana
		cout << caster->GetName() << " does not have the mana to cast " << spellName << "\n\n\n\n";						//Displays that the caster didn't have enough mana
	else if (!noMana && !missed)																					//If the caster failed to cast
		cout << caster->GetName() << " failed to cast " << spellName << "\n\n\n\n";										//Displays that the caster failed to cast the spell
	else if (!noMana && missed)																						//If the casters spell missed
		cout << caster->GetName() << "'s " << spellName << " missed\n\n\n\n";											//Displays that the caster's spell missed

	cout << "\n\n\n\n";		//Adds line breaks 
	system("pause");		//Waits for user confirmation
}

/*****************************************************************
*	Purpose: Displays what the spell did ie who attacked who for
*					how much or how much was healed
*****************************************************************/
void Spell::DisplayDamage()
{
	system("cls");				//Clears screen

#pragma region Dealt Damage
	{
		if (!heal)					//If the spell did damage rather than heal
		{
			cout << caster->GetName() << " attacked " << target->GetName() << " with " << spellName << " for " << dmg << " damage\n\n" << target->GetName();	//Display how much damage was done to the target and who attacked

			if (target->CheckDead())					
				cout << " is now dead";
			else
				cout << " is now at " << target->GetHealth() << " health";		//If the target died show that, otherwise show their new health value
		}
	}
#pragma endregion Dealt Damage

#pragma region Healed
	{
		if (heal)					//If the spell healed rather than dealt damage								
		{
			cout << caster->GetName() << " is healing ";			//Display that the caster is healing
			if (target == caster)
			{
				cout << "themself ";
			}
			else
			{
				cout << target->GetName() << " ";
			}
			cout << " for " << -dmg << "\n\n" << target->GetName() << " is now at " << target->GetHealth() << " health";		//Show who is being healed and for how much
		}
	}
#pragma endregion Healed

	cout << "\n\n\n\n";		//Adds line breaks
	system("pause");		//Waits for user confirmation
}

/**********************************************************************
*	Purpose: Primes the spell for use by setting its mana cost, chance
*			to cast successfully, chance to hit, damage done or health
*			healed
***********************************************************************/
void Spell::PrimeSpell()
{
	manaCost = baseManaCost - baseManaCost * (float)(caster->GetManaMod() / 100.f);					//Sets the mana cost of the spell based on formula (see formulas.txt to get modifier equation)		
	castChance = baseCastChance + baseCastChance * (float)(caster->GetCastChanceMod() / 100.f);		//Sets the chance to cast the spell based on formula (see formulas.txt to get modifier equation)

	if (baseHitChance < 100)																	//If the base hit chance is not 100%
		hitChance = baseHitChance + (baseHitChance * (float)(caster->GetHitMod() / 100));			//Calculate the hit chance based on formula (see formulas.txt to get modifier equation)

	dmg = baseDmg + (baseDmg * (float)(caster->GetMagDmgMod() / 100));			//Calculates the damage of the spell based on formula (see formulas.txt to get modifier equation)

	if (baseDmg < 0)			//If the base damage is negative 
	{
		heal = true;				//Mark this spell as a healing spell
	}
	else						//Otherwise
	{
		heal = false;				//Mark it as a damage spell
	}
}

/***************************************************************
*	Purpose: Casts the spell by doing what it has been told to
*			do be it heal or deal damage, also has chances to
*			fail
***************************************************************/
void Spell::CastSpell()
	{
	//PrimeSpell();

		if (manaCost > caster->GetMana())							//If the character does not have enough mana to cast the spell
		{	
			DisplayFailedCast(true);									//Display to the user that the character failed to cast due to lack of mana
		}
		else														//Otherwise
		{	
			int testNum = rand() % 100 + 1;								//Get a random number between 1 and 100 inclusively

			if (testNum > castChance || castChance == 0)					//If the random number fails the spell cast chance percent 
			{	
				DisplayFailedCast(false, true);									//Display to user that character failed to cast	
			}
			else															//Otherwise
			{

				int testNum = rand() % 100 + 1;								//Get a new random number

				if (testNum <= hitChance && hitChance != 0)						//If it passes the hit chance check
				{
					caster->SubtractMana(manaCost);								//Subtract mana from the caster
					if (heal)													//If it is a healing spell
					{
						target->Heal(-dmg);										//Heal the target
					}
					else														//Otherwise
					{
						target->TakeDamage(dmg);								//Deal damage to the target
					}

					DisplayDamage();											//Display to the user what happened
				}
				else															//If the spell missed
				{
					DisplayFailedCast();										//Display to user that the character missed their spell cast
				}
			}
		}
	}
#pragma endregion 

#pragma region Physical Abilities
PhysicalAbility::PhysicalAbility()
{
	abilityName = "";
	minBaseDamage = 0;
	maxBaseDamage = 0;
	baseHitChance = 0;
	hitChance = 0;
}

PhysicalAbility::PhysicalAbility(string name, int minDmg, int maxDmg, float bHitChance, Character* atkChar)
{
	abilityName = name;
	minBaseDamage = minDmg;
	maxBaseDamage = maxDmg;
	baseHitChance = bHitChance;
	hitChance = 0;
	attacker = atkChar;
}

void PhysicalAbility::DisplayDamage()
{
	system("cls");				//Clears screen

	
		
	cout << attacker->GetName() << " attacked " << target->GetName() << " with " << abilityName << " for " << damage << " damage\n\n" << target->GetName();	//Display how much damage was done to the target and who attacked

	if (target->CheckDead())
		cout << " is now dead";
	else
		cout << " is now at " << target->GetHealth() << " health";		//If the target died show that, otherwise show their new health value
}

void PhysicalAbility::PrimeAbility()
{
	if (baseHitChance < 100)
		hitChance = baseHitChance + baseHitChance * (float)(attacker->GetHitMod() / 100.0f);
	else
		hitChance = baseHitChance;

	int tempDmg = rand() % (maxBaseDamage + minBaseDamage + 1) + minBaseDamage;
	damage = tempDmg + tempDmg * attacker->GetPhyDmgMod() / 100.0f;	
}

void PhysicalAbility::UseAbility()
{
	system("cls");
	int testNum = rand() % 100 + 1;

	if (testNum <= hitChance && hitChance != 0)
	{
		target->TakeDamage(damage);
		DisplayDamage();
	}
	else
	{
		cout << attacker->GetName() << "'s " << abilityName << " missed";											//Displays that the caster's spell missed
	}

	cout << "\n\n\n\n";
	system("pause");
}
#pragma endregion 