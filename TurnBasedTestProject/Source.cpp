/**********************************************************************************************************
*	Author: Dylan Morgan
*
*	Project Name: Console Based Turn Based RPG (working title)
*
*	Start Date: 12/19/2016
*	Last Edited: 1/12/2017
*
*	Project Description: A turn based rpg that takes place by using only the command console. User explores
*						predefined map with potentially randomly generated dungeons. Goal is to have a game
*						like Zork but with more modern turn based elements and character development
**********************************************************************************************************/

#include "Character.h"
#include "InvalidInputFunctions.h"
#include <iomanip>
#include <conio.h>


const int BASIC_ATTACK_DAMAGE_LOW = 8;		//The minimum amount of damage a basic attack can deal
const int BASIC_ATTACK_DAMAGE_HIGH = 15;	//The max amount of damage a basic attack can deal

//This function will run and if wanted loop the entire program
void Run(bool &shouldRun);

//This function acts a turn in combat between two characters
void Turn(Character &currentPlayer, Character &targetPlayer);

//This function will display the battlefield to the user
void DisplayBattlefield(Character &playerCharacter, Character &otherCharacter001, char &choice);

//This function runs the intro of the game
void Intro(string &playerName);

//This function runs the tutorial fight of the game
void TutorialFight(Character &playerCharacter);

//Function to display stats to the player and allow them to distribute
void AssignPlayerStats(int numToDistribute, int stats[5]);



int main()
{
	bool shouldRun = true;

	do
	{
		srand(time(NULL));		//Seeds for random numbers
		system("cls");			//Clears the screen
		Run(shouldRun);			//Runs the program
	} while (shouldRun);

	//Map map;

	//map.LoadMapFromFile("Level01.txt");
	//map.DrawMap();

	return 0;
}

/****************************************************************
*	Purpose: Runs the intro to the game getting the name of the 
*		player
****************************************************************/
void Intro(string &playerName)
{
	system("cls");																		//Clears the screen
	cout << "Welcome adventurer what shall I call you?\n\nEnter Name: ";				//Prompts user for character name
	getline(cin, playerName);															//Gets a name from the player

	system("cls");
	cout << "Before we get into the heat of things you should set up your base stats\n\n"
		<< "When you are ready press any key to go to the stat distribution page...";
	ClearCin();
	_getch();

}

/*********************************************************************************
*	Purpose: Distributes a set number of stats to the player based on the player's
*			choice
*********************************************************************************/
void AssignPlayerStats(int numToDistribute, int stats[5])
{
	//Stat order stam, strength, dex, int, wis	
	bool distribute = true;											//Variable to determine if the points should be redistributed
	do																				//Loops if points need redistributed
	{
		int tempStats[5] = { stats[0], stats[1], stats[2], stats[3], stats[4] };				//Array of variables to hold the unconfirmed stat values initialized to the given stat values
		bool validInput = false;									//Variable to determine if input was valid or not
		do																			//Loops if a single value was given an incorrect input
		{
			int disCounter = numToDistribute;					//Variable to hold how many points can be distributed initialized to the given amount

			for (int i = 0; i < 5; i++)
			{
				tempStats[i] = stats[i];						//Intializes tempstats to stats( needed for loop)
			}

			for (int i = 0; i < 5; i++)									//Loops through the stats
			{
				system("cls");																																	//Clear screen
				cout << "Here you will distribute some initial stats, choose wisely as these will impact your character in various ways\n\n";					//Tel user what they will do here

				//This block displays what each stat does
				cout << "Stamina (determines maxiumum health)";
				cout << "\nStrength (determines physical damage bonus)";
				cout << "\nDexterity (determines physical hit chance bonus)";
				cout << "\nIntelligence (determines mana reduction and spell hit chance)";
				cout << "\nWisdom (determines magical effectiveness and spell hit chance)";

				//This block displays the current value of each stat
				cout << "\n\n\n";
				cout << "Stamina: " << tempStats[0];
				cout << "\nStrength: " << tempStats[1];
				cout << "\nDexterity: " << tempStats[2];
				cout << "\nIntelligence: " << tempStats[3];
				cout << "\nWisdom: " << tempStats[4];

				cout << "\n\nYou have " << disCounter << " points to distribute\n\n";	//Displays how many points user has to distribute

				int amount = 0;																//Variable to hold the number of points to put into a stat

				ClearCin();																	//Clears cin

				//This block prompts user to enter a amount to put into the current stat
				cout << "Enter the amount to put into ";										
				switch (i)
				{
				case 0:
					cout << "Stamina: ";
					break;
				case 1:
					cout << "Strength: ";
					break;
				case 2:
					cout << "Dexterity: ";
					break;
				case 3:
					cout << "Intelligence: ";
					break;
				case 4:
					cout << "Wisdom: ";
					break;
				}
				
				cin >> amount;																			//Gets the amount from the user
				CheckForAdvancedInvalidInput(validInput, amount, true, true, 0, disCounter);			//Makes sure it is valid

				if (validInput)																					//If the number entered was valid
				{
					tempStats[i] += amount;																		//Add the number to the current stat
					disCounter -= amount;																		//Subtract the number from the number of points available to distribute
				}
				else																							//If the number was not valid
				{
					i--;																						//Subtract one from i so the loop stays on the invalid stat until a valid value is entered
				}
				if (disCounter <= 0)																			//If at anypoint the number available to distribute is 0 break out of the loop to not prompt for more
					break;
				if (i == 4 && disCounter > 0)
					i--;
			}

		} while (!validInput);
		

	invalidChoice:										//Goes here if user entered a invalid confirmation choice
			
		//This block Displays to the user the new stat values
		system("cls");
		cout << "The new stat values are: \n\n";
		cout << "Stamina: " << tempStats[0];
		cout << "\nStrength: " << tempStats[1];
		cout << "\nDexterity: " << tempStats[2];
		cout << "\nIntelligence: " << tempStats[3];
		cout << "\nWisdom: " << tempStats[4];

		char choice = ' ';																//Variable to hold the choice of the user

		
		cout << "\n\n\n\nIs this correct? Y for yes, N for no: ";						//Prompts user to confirm or deny stats
		ClearCin();																		//Clears cin
		cin >> choice;																	//Gets users choice

		switch (choice)
		{
		case 'Y':
		case 'y':
			distribute = false;											
			for (int i = 0; i < 5; i++)
			{
				stats[i] = tempStats[i];			//If choice was yes then set the stats to the tempstats and set distribute to false to get out of loop
			}
			break;
		case 'N':
		case 'n':
			distribute = true;						//If choice was no the set distribute to true to reenter values
			break;
		default:
			DisplayInvalidNoVar();					//If choice was invalid display error message and goto reprompt
			goto invalidChoice;
		}
	} while (distribute);
	
}

/************************************************************************
*	Purpose: Runs the tutorial fight against a premade ai cultist
************************************************************************/
void TutorialFight(Character &playerCharacter)
{
	system("cls");																						//Clears the screen then shows welcome message
	cout << "Alright " << playerCharacter.GetName() << ", let's get you aquainted with how fighting works.\n\n\n"
		<< "When in combat you will see basic information about the enemy, below that you will see advanced information about yourself.\n\n"
		<< "This information includes your current health and mana.\n\n"
		<< "You are then presented with what you may do for that turn\n\nPay special attention to mana costs since selecting something that costs more than you have will essentially waste a turn\n\n"
		<< "After you do what you want to the enemy will take their turn then you will be presented with your choices again.\n\n\n\n"
		<< "If you think you are ready press any key to experience your first fight...";
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
	_getch();											//Waits for user confirmation
	system("cls");										//Clears the screen

	bool runTutorialFight = false;						//Determines if fight needs to be ran again

	do
	{
		Character otherCharacter001("Cultist");				//Creates the ai character
		int stats[5] = { 4, 4, 4, 4, 4 };					//Stats to be used by the ai character
		otherCharacter001.InitStats(stats);					//Initializes the ai's stats
		otherCharacter001.UpdateStats();					//Updates the ai's stats
		otherCharacter001.ResetStats();						//Resets the ai's stats( health and mana to cap to fully heal and rest )

		do
		{
			Turn(playerCharacter, otherCharacter001);					//Player character takes a turn
			if (!otherCharacter001.CheckDead())												//If the ai character is not dead
				Turn(otherCharacter001, playerCharacter);									//Ai character takes a turn
		} while (!playerCharacter.CheckDead() && !otherCharacter001.CheckDead());	//Loop turns until one character is dead

		if (!playerCharacter.CheckDead())		//If the player is not the character that died
		{	
			system("cls");								//Clear the screen
			playerCharacter.AddXp(100);
			cout << playerCharacter.GetName() << " gained " << 100 << "xp\n\n";
			if (playerCharacter.GetXp() >= playerCharacter.GetXpToLevel())
			{
				playerCharacter.LevelUp();
				cout << playerCharacter.GetName() << " leveled up to level" << playerCharacter.GetLevel() << "\n\n\n\n";
				system("pause");
				AssignPlayerStats(3, playerCharacter.GetStats());
				system("cls");
			}
			
			runTutorialFight = false;					//Set tutorial fight to not run again then display congratulatory message to user
			cout << "Now that wasn't so bad was it?\n\nNow that you know the basics of combat we should go over how to move about in this world.\nWhen you are ready press any key...";
		}
		else									//If the player did die
		{
			system("cls");								//Clear the screen
			runTutorialFight = true;					//Set fight to run again and display an encouraging message to user
			cout << "Okay so maybe that didn't go your way, don't feel too bad though there's always next time.\nLet me just heal you up and you can go again...";
			
		}
		playerCharacter.ResetStats();				//Replenishes players health and mana
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
		_getch();											//Waits for user input
	} while (runTutorialFight);
	
}

/********************************************************************
*	Purpose: Runs the core game by calling on other functions in 
*			proper order
********************************************************************/
void Run(bool &shouldRun)
{
	string playerName = "";				//The players name variable

	Intro(playerName);								//Run the games intro

	//Stat order stam, strength, dex, int, wis						Side Note: will add function for user to distribute starting stats
	int playerStats[5] = {1, 1, 1, 1, 1 };						//Player character starting stats initialized to 1

	AssignPlayerStats(20, playerStats);

	Character playerCharacter(playerName, true);		//Initializes the players character
	playerCharacter.InitStats(playerStats);				//Initializes the players characters stats
	playerCharacter.UpdateStats();						//Updates the players stats
	playerCharacter.ResetStats();						//Replenishes players health and mana

	TutorialFight(playerCharacter);					//Run the games tutorial fight

}

/*****************************************************************
*	Purpose: Serves as one turn for a single character with a
*		single target, character can be ai or player controlled
*****************************************************************/
void Turn(Character &currentPlayer, Character &targetPlayer)
{
	if (!currentPlayer.CheckPlayerCharacter())															//If it is an ai characters turn
	{
		/* Call Player AI functions here by first constructing logic for what to call
		*  Possibly add random numbers to determine some factors*/

		currentPlayer.UpdateStats();						//Update the characters stats
		int healThreshold = rand() % 30 + 41;						//Set a random healing threshold percentage to test at what percent health they have a chance to try and heal
		int willHeal = rand() % 100 + 1;							//A random number to determine if the character will heal if they are at or past the threshold
		
		if (willHeal <= 68 && currentPlayer.GetHealth() <= (float)(healThreshold / 100.f ) * currentPlayer.GetMaxHealth() && currentPlayer.GetMana() >= currentPlayer.GetManaCost("Heal"))
		{																					//If random checks pass and character has the mana
			currentPlayer.SetSpellTarget("Heal", &currentPlayer);								//Sets their heal to target themself
			currentPlayer.CastSpell("Heal");												//Casts heal
		}
		else																				//IF the character is not healing
		{
			int restThresholdPercent = rand() % 21 + 20;																//Sets the rest mana threshold to 20-40%
			int willRest = rand() % 100 + 1;																			//Determines if chance character will rest if they are past the threshold 

			int chanceToCastFireBall = rand() % (80 - 60 + 1) + 60;														//Chance the character will cast a fireball
			int randomNum = rand() % 100 + 1;																			//A random number to be used

			if (currentPlayer.GetMana() <= (restThresholdPercent * currentPlayer.GetMaxMana()) / 100 && willRest <= 75)	
			{								//If the character is past the mana threshold and meets the rest check
				currentPlayer.Rest();		//The character rests
			}
			else if(currentPlayer.GetMana() >= currentPlayer.GetManaCost("Fire Ball") && randomNum <= chanceToCastFireBall)
			{															//If the character has enough mana to cast fire ball and meets the cast check
				currentPlayer.SetSpellTarget("Fire Ball", &targetPlayer);	//Sets fire ball target to the target
				currentPlayer.CastSpell("Fire Ball");					//Casts fire ball
			}
			else
			{																													//If all special attacks did not pass their checks
				//int dmg = rand() % (BASIC_ATTACK_DAMAGE_HIGH - BASIC_ATTACK_DAMAGE_LOW + 1) + BASIC_ATTACK_DAMAGE_LOW;			//Set dmg for a basic attack
				//currentPlayer.BasicAttack(targetPlayer, dmg);		
				//Deal a basic attack

				int attackNum = rand() % 100;
				int enemyHealthThreshold = rand() % 16 + 10;

				if (targetPlayer.GetHealth() >= enemyHealthThreshold && attackNum <= 80)
				{
					currentPlayer.SetAbilityTarget("Basic Attack", &targetPlayer);
					currentPlayer.UseAbility("Basic Attack");
				}
				else if (attackNum <= 15)
				{
					currentPlayer.SetAbilityTarget("Blind Rage", &targetPlayer);
					currentPlayer.UseAbility("Blind Rage");
				}
				else if (attackNum <= 90)
				{
					currentPlayer.SetAbilityTarget("Slash", &targetPlayer);
					currentPlayer.UseAbility("Slash");
				}
				else
				{
					currentPlayer.SetAbilityTarget("Basic Attack", &targetPlayer);
					currentPlayer.UseAbility("Basic Attack");
				}
				
				
			}
			
		}
	}
	else																			//If the character is controlled by the user
	{
		badInput:																							//Place to jump to if there is bad input
		char choice = ' ';																					//Char to hold users choice
		DisplayBattlefield(currentPlayer, targetPlayer, choice);												//Displays the battlefield to the user, prompts and gets a choice for what they want to do

		int dmg = 0;																										//Variable used for basic attack base damage
		switch (choice)
		{
		case '1':																													//Player chose basic attack
			//dmg = rand() % (BASIC_ATTACK_DAMAGE_HIGH - BASIC_ATTACK_DAMAGE_LOW + 1) + BASIC_ATTACK_DAMAGE_LOW;						//Randomizes base basic attack damage within a range
			//currentPlayer.BasicAttack(targetPlayer, dmg);																			//Player attacks target with basic attack
			currentPlayer.SetAbilityTarget("Basic Attack", &targetPlayer);
			currentPlayer.UseAbility("Basic Attack");
			break;
		case '2':																													//Player chose to heal
			currentPlayer.SetSpellTarget("Heal", &currentPlayer);																		//Sets the target of heal to themself
			currentPlayer.CastSpell("Heal");																						//Casts the heal spell
			break;
		case '3':																													//Player chose to cast fireball
			currentPlayer.SetSpellTarget("Fire Ball", &targetPlayer);																	//Sets the target of fireball to the target character
			currentPlayer.CastSpell("Fire Ball");																					//Casts fire ball spell
			break;
		case '4':
			currentPlayer.SetAbilityTarget("Slash", &targetPlayer);
			currentPlayer.UseAbility("Slash");
			break;
		case '5':
			currentPlayer.SetAbilityTarget("Blind Rage", &targetPlayer);
			currentPlayer.UseAbility("Blind Rage");
			break;
		case '6':																													//Player chose to rest and regen mana
			currentPlayer.Rest();																									//Player rests restoring their mana
			break;
		default:																													//There was an invalid choice
			system("cls");																											//Clear the screen
			cout << "Invalid Input, try again\n\n\n\n";																				//Display choice was invalid
			system("pause");																										//Wait for user confirmation
			goto badInput;
		}
	}
}

/*******************************************************************************************
*	Purpose: Displays the battlefield to the user, this includes basic enemy info such
*				as name and health. More advance player info (name, health, and mana),
*				and presents the player with choices for the turn setting a choice variable
********************************************************************************************/
void DisplayBattlefield(Character &playerCharacter, Character &otherCharacter001, char &choice)
{
	playerCharacter.UpdateStats();																								//Updates the players stats ( modifiers and primes all spells )

	//This block displays the enemies information
	system("cls");																														//Clears the screen
	cout.setf(ios::right);
	cout.fill('-');
	cout << setw(65) << "-" << "Enemy" << setw(65) << "-";	
	cout.setf(ios::left);
	cout.fill(' ');
	cout << "\n" << otherCharacter001.GetName() << "\nHealth: " << otherCharacter001.GetHealth() << "\n\n\n\n\n\n";												

	//This block displays the players information
	cout.setf(ios::right);
	cout.fill('-');
	cout << setw(65) << "-" << "Player" << setw(64) << "-";
	cout.setf(ios::left);
	cout.fill(' ');																													

	//This block prompts the player for what they wish to do and sets their choice
	cout << "\n" << playerCharacter.GetName() << "\nHealth: " << playerCharacter.GetHealth() << "\nMana: " << playerCharacter.GetMana() << "\n\nAbilities:\n"
		<< "1) Basic Attack(deals low physical damage, will always hit)\n"
		<< "2) Heal(cost " << playerCharacter.GetManaCost("Heal") << " mana, restores health)\n" 
		<< "3) Fire Ball(cost " << playerCharacter.GetManaCost("Fire Ball") << " mana, deals magical fire damage)\n"
		<< "4) Slash(deals moderate physical damage)\n"
		<< "5) Blind Rage(deals high physical damage but has a very low hit chance)\n"
		<< "6) Rest(restores mana)\n"
		<< "\n\n" << "Enter choice: ";
	cin >> choice;
}