//------------------------------------------------------------------------------
// Game.hpp
//
// Group: Group 6.6, study assistant: Oberhuber M.
//
// Authors:
// Sarah Lang (01110645)
// Created: 26.03.2020
// Last change: 16.06.2020
//------------------------------------------------------------------------------
//

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "Random.hpp"
#include "Player.hpp"

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <cctype> // for tolower()
#include <algorithm> // std::transform
#include <cmath> // for pow()


namespace Oop
{
  class SpellCard;
  class CreatureCard;
  class Card;
  class Player;
  class Interface;

  //---------------------------------------------------------------------------
  // Game Class
  //
  class Game
  {
  public:
    //--------------------------------------------------------------------------
    // string constants
    //
    static const std::string CREATURES_STRING;
    static const std::string SPELLS_STRING;
    static const std::string NAME_STRING;
    static const std::string MANA_COST_STRING;
    static const std::string DAMAGE_POINTS_STRING;
    static const std::string LIFE_POINTS_STRING;
    static const std::string SHIELD_STRING;
    static const std::string MANA_DRAIN_STRING;
    static const std::string STRING_TO;
    static const std::string STRING_TO_SPACED;
    static const std::string STRING_WITH;
    static const std::string STRING_WITH_SPACED;
    static const std::string STRING_TEAM;
    static const std::string STRING_TEAMNAME;
    static const std::string STRING_WHOAMI;
    static const std::string STRING_PLAY;
    static const std::string STRING_COMMAND;
    static const std::string STRING_SPACE;
    static const std::string DIGITS;

    //--------------------------------------------------------------------------
    // constants
    //
    static const int MINIMUM_CONFIG_CARDS = 10;
    static const int ZERO = 0;
    static const int ONE = 1;
    static const int TWO = 2;
    static const int THREE = 3;
    static const int FOUR = 4;
    static const int HAND_IS_FULL = 7;
    static const int INVALID = 99;
    static const int MAX_DAMAGE = 9;
    static const int MAX_LIFE = 9;
    static const int RUNNING_OUT_OF_LIFEPOINTS = 9;
    static const int CREATURE_ARRAY_SIZE = 6;
    static const int MAX_NAME_LENGTH = 8;
    static const int SUCCESSFUL_RETURN = 0;
    static const int CONFIG_PROBLEM = -3;

    //--------------------------------------------------------------------------
    // enums
    //
    enum Command { NON_EXISTING, HELP, ATTACK, SET, CAST,
                   SACRIFICE, STATE, FINISH, QUIT, WHOAMI, PLAY };

    // bool false is 0; true is one
    enum CurrentPlayer { PLAYER_ONE = false, PLAYER_TWO = true };

    enum Mode { NORMAL = true, AI = false };

    enum ConfigErrors { SUCCESS = 0, FAIL = 1, TOO_MANY_ARRAY_MEMBERS = 1,
                        CREATURE_NAME_INVALID = 1, CREATURE_MANA_INVALID = 1,
                        CREATURE_DAMAGE_INVALID = 1,
                        CREATURE_LIFEPOINTS_INVALID = 1,
                        CREATURE_SHIELD_INVALID = 1,
                        CREATURE_MANADRAIN_INVALID = 1,
                        SPELL_NAME_INVALID = 1, SPELL_ARRAY_INVALID = 1,
                        DUPLICATE_CREATURE_DOESNT_MATCH = 1,
                        FAILED_TO_CREATE_SPELLCARD = 1, TOO_FEW_CARDS = 1 };

    //--------------------------------------------------------------------------
    // Constructor
    //
    Game(Oop::Interface &io);

    //--------------------------------------------------------------------------
    // Copy Constructor
    // Makes a copy of another Game Object.
    // @param original Original to copy.
    //
    Game(const Game& original) = delete;

    //--------------------------------------------------------------------------
    // Assignment Operator
    // Used to assign one Game to another
    // @param original Original with values to copy.
    //
    Game& operator= (Game const& original) = delete;

    //--------------------------------------------------------------------------
    // Destructor
    //
    virtual ~Game();

    //--------------------------------------------------------------------------
    // methods
    //

    //--------------------------------------------------------------------------
    // loadConfig
    // Handles the loading and error checking of config
    // @param config_file The filename of config JSON file
    // @return bool indicating the success or failure
    //
    bool loadConfig(std::string config_file);

    //--------------------------------------------------------------------------
    // run()
    // makes sure that the startGame() method gets called initially
    // and that the game keeps prompting users for input
    //
    void run();

    //--------------------------------------------------------------------------
    // loadConfig helper methods
    //

    //--------------------------------------------------------------------------
    // parseJsonConfig
    // Function to parse a JSON config file using the rapidJSON library
    // @param config_file The filename of config JSON file
    // @return The config_file as rapidJSON Document
    //
    rapidjson::Document parseJsonConfig(std::string config_file);

    //--------------------------------------------------------------------------
    // checkIfCreatureNameValid
    // Function check if the name of a creature is valid
    // according to specification
    // @param creature The JSON info of one creature
    // @return An integer error value
    //
    int checkIfCreatureNameValid(const rapidjson::Value& creature);

    //--------------------------------------------------------------------------
    // checkIfCreatureManaCostValid
    // Function check if the mana_cost of a creature is valid
    // according to specification
    // @param creature The JSON info of one creature
    // @return An integer error value
    //
    int checkIfCreatureManaCostValid(const rapidjson::Value& creature);

    //--------------------------------------------------------------------------
    // checkIfCreatureDamagePointsValid
    // Function check if the damage_points of a creature are valid
    // according to specification
    // @param creature The JSON info of one creature
    // @return An integer error value
    //
    int checkIfCreatureDamagePointsValid(const rapidjson::Value& creature);

    //--------------------------------------------------------------------------
    // checkIfCreatureLifePointsValid
    // Function check if the life_points of a creature are valid
    // according to specification
    // @param creature The JSON info of one creature
    // @return An integer error value
    //
    int checkIfCreatureLifePointsValid(const rapidjson::Value& creature);

    //--------------------------------------------------------------------------
    // checkIfCreatureShieldValid
    // Function check if the shield attribute of a creature is valid (i.e.
    // bool)
    // @param creature The JSON info of one creature
    // @return An integer error value
    //
    int checkIfCreatureShieldValid(const rapidjson::Value& creature);

    //--------------------------------------------------------------------------
    // checkIfCreatureManaDrainValid
    // Function check if the mana_drain attribute of a creature
    // is valid (i.e. bool) according to specification
    // @param creature The JSON info of one creature
    // @return An integer error value
    //
    int checkIfCreatureManaDrainValid(const rapidjson::Value& creature);

    //--------------------------------------------------------------------------
    // testConfigCreaturesValidity
    // Function check handle checking if creature cards from config are valid
    // according to specification
    // @param array_of_creatures a JSON array with the creature cards info
    // @return An integer error value
    //
    int testConfigCreaturesValidity
      (const rapidjson::Value& array_of_creatures);

    //--------------------------------------------------------------------------
    // testConfigSpellValidity
    // Function check if the name of a spell is valid
    // according to specification
    // @param array_of_spells a JSON array with the spell cards info
    // @return An integer error value
    //
    int testConfigSpellValidity(const rapidjson::Value& spell);

    //--------------------------------------------------------------------------
    // testConfigCreatureNameDuplication
    // Function check if a creature name exists multiple times
    // if yes, if they all have the same attributes
    // @param array_of_creatures a JSON array with the creature cards info
    // @return An integer error value
    //
    int testConfigCreatureNameDuplication
      (const rapidjson::Value& array_of_creatures);

    //--------------------------------------------------------------------------
    // instantiateSpellCards
    // Function constructs spell cards from data from the config file
    // @param array_of_spells a JSON array with the spell cards info
    // @return An integer error value
    //
    int instantiateSpellCards(const rapidjson::Value& array_of_spells);

    //--------------------------------------------------------------------------
    // instantiateCreatureCards
    // Function constructs creature cards from data from the config file
    // @param array_of_creatures a JSON array with the creature cards info
    // @return An integer error value
    //
    int instantiateCreatureCards(const rapidjson::Value& array_of_creatures);

    //--------------------------------------------------------------------------
    // testOverallValidityOfJsonConfig
    // Function to test the overall validity of a correctly parsed JSON config
    // file, i.e. number of cards isn't too small, appropirate arrays exist,etc.
    // according to specification
    // @param json_configfile The JSON file as rapidJSON Document type
    // @return An integer error value
    //
    int testOverallValidityOfJsonConfig(rapidjson::Document &json_configfile);

    //--------------------------------------------------------------------------
    // parseCommandToEnum
    // Function to parse a command from the user prompt
    // @param command_string A string with the command entered
    // (just the 1st word)
    // @return A Command enum value
    //
    Command parseCommandToEnum(std::string command_string);

    //--------------------------------------------------------------------------
    // parseCommandInput
    // Function to parse a command from the user prompt,
    // handles prompting the user for input,
    // will keep asking until it has received some input
    // will warn if input was not a known command
    // else will pass the line on for further parsing.
    // This function handles one command input at a time.
    // In order for this game process of prompting to keep going,
    // there is an infinite loop in Game::run() which can only be broken out of
    // by using the "quit" command or by winning/losing the game
    // @return the command as a string
    //
    std::string parseCommandInput();

    //--------------------------------------------------------------------------
    // getCurrentPlayer
    // Function to get the current player (one or two) whose turn it is
    // @return Player& reference (player_one_ or player_two_ who's turn it is)
    //
    Player& getCurrentPlayer();

    //--------------------------------------------------------------------------
    // getCurrentOpponent
    // Function to get the current opponent
    // @return Player& reference
    // (player_one_ or player_two_, the current opponent)
    //
    Player& getCurrentOpponent();

    //--------------------------------------------------------------------------
    // startGame
    // asks players for names, sets up their already shuffled pickup stacks
    // deals each player three cards from their respective pickup stack
    //
    void startGame();

    //--------------------------------------------------------------------------
    // newTurn
    // starts a new turn, printing the map
    // if necessary a new round and counting up the round count,
    //
    void newTurn();

    //--------------------------------------------------------------------------
    // endOfGameDialog
    // prints the winner dialog - it is always the opponent of the caller
    //
    void endOfGameDialog();

    //--------------------------------------------------------------------------
    // checkIfPlayerLoses
    // checks if a player has lost all of their life points
    // if yes, initiates the end of game dialog
    // @param player The player to check
    //
    void checkIfPlayerLoses(Player& player_to_check);

    //--------------------------------------------------------------------------
    // changeCurrentPlayer
    // changes the value of the current player bool
    //
    void changeCurrentPlayer();

    //--------------------------------------------------------------------------
    // checkIfBufferEmpty
    // checks if there are leftover elements in buffer after the commands
    // @param &inputted_line_leftovers what's left of the stringstream
    // after comand parsing
    // @return int indicating success or failure (fail is if leftovers present)
    //
    int checkIfBufferEmpty(std::istringstream& inputted_line_leftovers);

    //--------------------------------------------------------------------------
    // displayCurrentGameState
    // display current game map for state command
    //
    void displayCurrentGameState();

    //--------------------------------------------------------------------------
    // clearBothPickUpStacks
    // frees all the already instantiated cards from the Game class'
    // pick_up_stack_one_ and pick_up_stack_two_
    //
    void clearBothPickUpStacks();

    //--------------------------------------------------------------------------
    // calculateManaGain
    // calculates the mana gain depending on the current rounds counter
    // @return int new_mana
    //
    int calculateManaGain();

    //--------------------------------------------------------------------------
    // carryOutManaGain
    // carries out the mana gain at the start of a turn
    //
    void carryOutManaGain();

    //--------------------------------------------------------------------------
    // printCommandErrorOnInterface
    // prints appropriate error message on interface depending on the error
    // @param the error returned from running the comand
    //
    void printCommandErrorOnInterface(Player::CommandError error_type);

    //--------------------------------------------------------------------------
    // parseCommandsAttackSet
    // parse commands of the type: attack, set
    // @param command The Command-Enum indicating the type of attack
    // @param input_line A stringstream with the rest of the input_line after
    // the command
    //
    void parseCommandsAttackSet(Command command, \
                                std::istringstream& input_line);

    //--------------------------------------------------------------------------
    // parseCommandsCastSacrifice
    // parse commands of the type: cast, sacrifice
    // @param command The Command-Enum indicating the type of attack
    // @param input_line A stringstream with the rest of the input_line after
    // the command
    //
    void parseCommandsCastSacrifice(Command command, \
                                    std::istringstream& input_line);

    //--------------------------------------------------------------------------
    // parseCommandsHelpStateQuitFinish
    // parse commands of the type: help, state, quit, finish
    // @param command The Command-Enum indicating the type of attack
    // @param input_line A stringstream with the rest of the input_line after
    // the command
    //
    void parseCommandsHelpStateQuitFinish(Command command, \
                                          std::istringstream &input_line);

    //--------------------------------------------------------------------------
    // parseCommandsWhoamiPlay
    // parse commands of the type: whoami, play (Ass2)
    // @param command The Command-Enum indicating the type of attack
    // @param input_line A stringstream with the rest of the input_line after
    // the command
    //
    void parseCommandsWhoamiPlay(Command command, \
                                 std::istringstream &input_line);

    //--------------------------------------------------------------------------
    // getAICommand
    // gets one single command via the rules of the AI
    // @return the first card to set 
    //
    std::string getAICommand();

    //--------------------------------------------------------------------------
    // getShieldCardToSetForAI
    // first check if there is a shield which can be set
    // @return the command as a string formatted like required
    //
    std::string getShieldCardToSetForAI();

    //--------------------------------------------------------------------------
    // getAttackingCreatureForAI
    // gets a possible attacker creature, returns zero if none
    // @return an integer denoting a creature position
    //
    int getAttackingCreatureForAI();

    //--------------------------------------------------------------------------
    // getAttackTargetCreatureForAI
    // gets a possible target creature, returns zero if none
    // @return an integer denoting a creature position
    //
    int getAttackTargetCreatureForAI();

    //--------------------------------------------------------------------------
    // manageHandCardforAI
    // manages setting and casting of one handcard
    // @param the reference to the currently handeled handcard
    // @param the current hand_counter value
    // @return a command string
    //
    std::string manageHandCardforAI(Card* card, int hand_counter);

  private:
    //--------------------------------------------------------------------------
    // A reference to the used interface
    //
    Oop::Interface& io_;

    //--------------------------------------------------------------------------
    // Two pick up stack vectors where the cards for each player
    // can be created from config and shuffled before the player objects
    // get their names, etc.
    //
    std::vector<Card*> pick_up_stack_one_;
    std::vector<Card*> pick_up_stack_two_;

    //--------------------------------------------------------------------------
    // Two Player objects
    //
    Player player_one_;
    Player player_two_;

    //--------------------------------------------------------------------------
    // A boolean containing the info on whose turn it is
    //
    bool current_player_ = CurrentPlayer::PLAYER_ONE;

    //--------------------------------------------------------------------------
    // A boolean containing the info on what game mode we're in (Ass2)
    //
    bool mode_ = Mode::NORMAL;

    //--------------------------------------------------------------------------
    // A boolean indicating error state of AI (Ass2)
    //
    bool ai_error_ = false;

    //--------------------------------------------------------------------------
    // A string containing the last command to avoid infinite looping (Ass2)
    // initiailized to help so it's never empty
    //
    std::string last_ai_command_ = Interface::COMMAND_HELP;


    //--------------------------------------------------------------------------
    // A counter of rounds (int)
    //
    int rounds = 0;
  };
}

#endif // GAME_H_INCLUDED
