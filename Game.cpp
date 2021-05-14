//------------------------------------------------------------------------------
// Game.cpp
//
// Group: Group 6.6, study assistant: Oberhuber M.
//
// Authors:
// Sarah Lang (01110645)
// Created: 26.03.2020
// Last change: 16.06.2020
//------------------------------------------------------------------------------
//

#include "Game.hpp"
using Oop::Game;
using Oop::Player;

//------------------------------------------------------------------------------
// string constants
//
const std::string Game::CREATURES_STRING = "Creatures";
const std::string Game::SPELLS_STRING = "Spells";
const std::string Game::NAME_STRING = "name";
const std::string Game::MANA_COST_STRING = "mana_cost";
const std::string Game::DAMAGE_POINTS_STRING = "damage_points";
const std::string Game::LIFE_POINTS_STRING = "life_points";
const std::string Game::SHIELD_STRING = "shield";
const std::string Game::MANA_DRAIN_STRING = "mana_drain";
const std::string Game::STRING_TO = "to";
const std::string Game::STRING_TO_SPACED = " to ";
const std::string Game::STRING_WITH = "with";
const std::string Game::STRING_WITH_SPACED = " with ";
const std::string Game::STRING_TEAM = "Team: ";
const std::string Game::STRING_TEAMNAME = "Theomachos";
const std::string Game::STRING_WHOAMI = "whoami";
const std::string Game::STRING_PLAY = "play";
const std::string Game::STRING_COMMAND = "Command: ";
const std::string Game::STRING_SPACE = " ";
const std::string Game::DIGITS = "0123456789";

//------------------------------------------------------------------------------
Game::Game(Oop::Interface &io) : io_(io)
{
}

//------------------------------------------------------------------------------
Game::~Game()
{
}

//------------------------------------------------------------------------------
  rapidjson::Document Game::parseJsonConfig(std::string config_file)
  {
    std::ifstream jsondata_ifstream { config_file };
    if ( !jsondata_ifstream.is_open() )
    {
      io_.error(Interface::ERROR_INVALID_CONFIG);
      exit(Game::CONFIG_PROBLEM);
    }

    rapidjson::IStreamWrapper json_in_streamwrapper { jsondata_ifstream };
    rapidjson::Document json_configfile {};
    json_configfile.ParseStream( json_in_streamwrapper );

    if ( json_configfile.HasParseError() )
    {
      io_.error(Interface::ERROR_INVALID_CONFIG);
      exit(Game::CONFIG_PROBLEM);
    }
    return json_configfile;
  }
//------------------------------------------------------------------------------
bool Game::loadConfig(std::string config_file)
{
  int failure = 0;
  try
  {
    rapidjson::Document json_configfile = parseJsonConfig(config_file);

    if ( json_configfile.HasMember(CREATURES_STRING.c_str())
         && json_configfile.HasMember(SPELLS_STRING.c_str())
         && json_configfile.MemberCount() == Game::TWO
         && json_configfile[SPELLS_STRING.c_str()].IsArray()
         && json_configfile[CREATURES_STRING.c_str()].IsArray()
        )
    {
      failure += testOverallValidityOfJsonConfig(json_configfile);
    }
    else
    {
      failure++;
    }

    if (!failure)
    {
      return true;
    }
    else
    {
      io_.error(Interface::ERROR_INVALID_CONFIG);
      return false;
    }
  }
  catch(...)
  {
    io_.error(Interface::ERROR_INVALID_CONFIG);
    return false;
  }
}
//------------------------------------------------------------------------------
void Game::clearBothPickUpStacks()
{
  for (size_t i = 0; i < pick_up_stack_one_.size(); i++)
  {
    delete pick_up_stack_one_[i];
  }

  for (size_t i = 0; i < pick_up_stack_two_.size(); i++)
  {
    delete pick_up_stack_two_[i];
  }
}
//------------------------------------------------------------------------------
int Game::testOverallValidityOfJsonConfig(rapidjson::Document &json_configfile)
{
  int config_invalid = 0;
  size_t card_count = 0;
  int error_in_spells = 0;

  const rapidjson::Value& array_of_spells = json_configfile\
                                            [SPELLS_STRING.c_str()];
  const rapidjson::Value& array_of_creatures = json_configfile\
                                               [CREATURES_STRING.c_str()];

  card_count += (array_of_spells.Size() + array_of_creatures.Size());

  if (card_count < static_cast<size_t>(Game::MINIMUM_CONFIG_CARDS))
  {
    return ConfigErrors::TOO_FEW_CARDS;
  }

  config_invalid += testConfigCreaturesValidity(array_of_creatures);

  for (rapidjson::SizeType i = 0; i < array_of_spells.Size(); i++)
  {
    const rapidjson::Value& current_spell = array_of_spells[i];
    error_in_spells += testConfigSpellValidity(current_spell);
  }
  config_invalid += error_in_spells;


  if (!config_invalid)
  {
    int problem = 0;
    problem += instantiateCreatureCards(array_of_creatures);
    problem += instantiateSpellCards(array_of_spells);

    if (problem)
    {
      clearBothPickUpStacks();
      return (config_invalid + problem);
    }

    Random& getInstance();

    Random::getInstance().shufflePickupstack(pick_up_stack_one_);
    Random::getInstance().shufflePickupstack(pick_up_stack_two_);
    return config_invalid;
  }
  else
  {
    clearBothPickUpStacks();
    return config_invalid;
  }
}
//------------------------------------------------------------------------------
int Game::testConfigCreaturesValidity
  (const rapidjson::Value& array_of_creatures)
{
  int error_in_creatures = 0;

  for (rapidjson::SizeType i = 0; i < array_of_creatures.Size(); i++)
  {
    const rapidjson::Value& current_creature = array_of_creatures[i];

    if (array_of_creatures[i].MemberCount() > Game::CREATURE_ARRAY_SIZE)
    {
      return ConfigErrors::TOO_MANY_ARRAY_MEMBERS;
    }

    error_in_creatures += checkIfCreatureNameValid(current_creature);
    error_in_creatures += checkIfCreatureManaCostValid(current_creature);
    error_in_creatures += checkIfCreatureDamagePointsValid(current_creature);
    error_in_creatures += checkIfCreatureLifePointsValid(current_creature);
    error_in_creatures += checkIfCreatureShieldValid(current_creature);
    error_in_creatures += checkIfCreatureManaDrainValid(current_creature);
  }
  error_in_creatures += testConfigCreatureNameDuplication(array_of_creatures);
  // this is placed outside of the for loop as to not pass the array 30 times

  if (!error_in_creatures)
  {
    return ConfigErrors::SUCCESS;
  }
  else
  {
    return ConfigErrors::FAIL;
  }
}

//------------------------------------------------------------------------------
int Game::checkIfCreatureNameValid(const rapidjson::Value& creature)
{
  if (creature.HasMember(NAME_STRING.c_str()) && \
      creature[NAME_STRING.c_str()].IsString())
  {
    std::string name = creature[NAME_STRING.c_str()].GetString();
    if ((name.length() >= Game::ONE) && \
        (name.length() <= Game::MAX_NAME_LENGTH))
    {
      return ConfigErrors::SUCCESS;
    }
  }
  return ConfigErrors::CREATURE_NAME_INVALID;
}

//------------------------------------------------------------------------------
int Game::checkIfCreatureManaCostValid(const rapidjson::Value& creature)
{
  if (creature.HasMember(MANA_COST_STRING.c_str()) && \
      creature[MANA_COST_STRING.c_str()].IsInt())
  {
    int mana_cost = creature[MANA_COST_STRING.c_str()].GetInt();
    if ( (mana_cost >= Game::ONE) && (mana_cost <= Interface::MAX_MANA))
    {
      return ConfigErrors::SUCCESS;
    }
  }
  return ConfigErrors::CREATURE_MANA_INVALID;
}

//------------------------------------------------------------------------------
int Game::checkIfCreatureDamagePointsValid(const rapidjson::Value& creature)
{
  if (creature.HasMember(DAMAGE_POINTS_STRING.c_str()) && \
      creature[DAMAGE_POINTS_STRING.c_str()].IsInt())
  {
    int damage_points = creature[DAMAGE_POINTS_STRING.c_str()].GetInt();
    if ( (damage_points >= Game::ZERO) && (damage_points <= Game::MAX_DAMAGE))
    {
      return ConfigErrors::SUCCESS;
    }
  }
  return ConfigErrors::CREATURE_DAMAGE_INVALID;
}

//------------------------------------------------------------------------------
int Game::checkIfCreatureLifePointsValid(const rapidjson::Value& creature)
{
  if (creature.HasMember(LIFE_POINTS_STRING.c_str()) && \
      creature[LIFE_POINTS_STRING.c_str()].IsInt())
  {
    int life_points = creature[LIFE_POINTS_STRING.c_str()].GetInt();
    if ( (life_points >= Game::ONE) && (life_points <= Game::MAX_LIFE))
    {
      return ConfigErrors::SUCCESS;
    }
  }
  return ConfigErrors::CREATURE_LIFEPOINTS_INVALID;
}

//------------------------------------------------------------------------------
int Game::checkIfCreatureShieldValid(const rapidjson::Value& creature)
{
  if (creature.HasMember(SHIELD_STRING.c_str()) && \
      creature[SHIELD_STRING.c_str()].IsBool())
  {
    return ConfigErrors::SUCCESS;
  }
  return ConfigErrors::CREATURE_SHIELD_INVALID;
}

//------------------------------------------------------------------------------
int Game::checkIfCreatureManaDrainValid(const rapidjson::Value& creature)
{
  if (creature.HasMember(MANA_DRAIN_STRING.c_str()) && \
      creature[MANA_DRAIN_STRING.c_str()].IsBool())
  {
    return ConfigErrors::SUCCESS;
  }
  return ConfigErrors::CREATURE_MANADRAIN_INVALID;
}

//------------------------------------------------------------------------------
int Game::testConfigSpellValidity(const rapidjson::Value& spell)
{
  if (spell.MemberCount() == Game::ONE && spell.HasMember(NAME_STRING.c_str()) \
      && spell[NAME_STRING.c_str()].IsString())
  {
    std::string spell_name = spell[NAME_STRING.c_str()].GetString();

    if (spell_name == Interface::STRING_HEALER ||
      spell_name == Interface::STRING_REBIRTH ||
      spell_name == Interface::STRING_DRACULA ||
      spell_name == Interface::STRING_RELIEF)
      {
         return ConfigErrors::SUCCESS;
      }
    return ConfigErrors::SPELL_NAME_INVALID;
  }
  return ConfigErrors::SPELL_ARRAY_INVALID;
}

//------------------------------------------------------------------------------
int Game::testConfigCreatureNameDuplication
  (const rapidjson::Value& array_of_creatures)
{
  for (rapidjson::SizeType i = 0; i < array_of_creatures.Size(); i++)
  {
    const rapidjson::Value& current_creature = array_of_creatures[i];
    std::string current_name = current_creature\
                               [NAME_STRING.c_str()].GetString();

    // test against each other creature name, in case of multiple duplication
    for (rapidjson::SizeType creature = 0; \
         creature < array_of_creatures.Size(); creature++)
    {
      std::string testing_name = array_of_creatures[creature]\
                                 [NAME_STRING.c_str()].GetString();

      if(current_name == testing_name)
      {
        int current_mana = current_creature\
                           [MANA_COST_STRING.c_str()].GetInt();
        int testing_mana = array_of_creatures[creature]\
                           [MANA_COST_STRING.c_str()].GetInt();

        int current_damage = current_creature\
                             [DAMAGE_POINTS_STRING.c_str()].GetInt();
        int testing_damage = array_of_creatures[creature]\
                             [DAMAGE_POINTS_STRING.c_str()].GetInt();

        int current_life = current_creature\
                           [LIFE_POINTS_STRING.c_str()].GetInt();
        int testing_life = array_of_creatures[creature]\
                           [LIFE_POINTS_STRING.c_str()].GetInt();

        bool current_shield = current_creature\
                              [SHIELD_STRING.c_str()].GetBool();
        bool testing_shield = array_of_creatures[creature]\
                              [SHIELD_STRING.c_str()].GetBool();

        bool current_mana_drain = current_creature\
                                  [MANA_DRAIN_STRING.c_str()].GetBool();
        bool testing_mana_drain = array_of_creatures[creature]\
                                  [MANA_DRAIN_STRING.c_str()].GetBool();

        if (current_mana != testing_mana || \
            current_damage != testing_damage || \
            current_life != testing_life || \
            current_shield != testing_shield || \
            current_mana_drain != testing_mana_drain)
        {
          return ConfigErrors::DUPLICATE_CREATURE_DOESNT_MATCH;
        }
      }
    }
  }
  return ConfigErrors::SUCCESS;
}

//------------------------------------------------------------------------------
int Game::instantiateSpellCards(const rapidjson::Value& array_of_spells)
{
  for (rapidjson::SizeType i = 0; i < array_of_spells.Size(); i++)
  {
    const rapidjson::Value& current_spell = array_of_spells[i];
    std::string spell_name = current_spell[NAME_STRING.c_str()].GetString();

    if (spell_name == Interface::STRING_HEALER)
    {
      pick_up_stack_one_.push_back(new SpellCard(\
                                   SpellCard::SpellType::HEALER));
      pick_up_stack_two_.push_back(new SpellCard(\
                                   SpellCard::SpellType::HEALER));
    }

    else if (spell_name == Interface::STRING_DRACULA)
    {
      pick_up_stack_one_.push_back(new SpellCard(\
                                   SpellCard::SpellType::DRACULA));
      pick_up_stack_two_.push_back(new SpellCard(\
                                   SpellCard::SpellType::DRACULA));
    }
    else if (spell_name == Interface::STRING_RELIEF)
    {
      pick_up_stack_one_.push_back(new SpellCard(\
                                   SpellCard::SpellType::RELIEF));
      pick_up_stack_two_.push_back(new SpellCard(\
                                   SpellCard::SpellType::RELIEF));
    }
     else if (spell_name == Interface::STRING_REBIRTH)
    {
      pick_up_stack_one_.push_back(new SpellCard(\
                                   SpellCard::SpellType::REBIRTH));
      pick_up_stack_two_.push_back(new SpellCard(\
                                   SpellCard::SpellType::REBIRTH));
    }
    else // ergo the type was not matched
    {
      return ConfigErrors::FAILED_TO_CREATE_SPELLCARD;
    }
  }
  return ConfigErrors::SUCCESS;
}

//------------------------------------------------------------------------------
int Game::instantiateCreatureCards(const rapidjson::Value& array_of_creatures)
{
  for (rapidjson::SizeType i = 0; i < array_of_creatures.Size(); i++)
  {
    const rapidjson::Value& current_creature = array_of_creatures[i];

    std::string name = current_creature[NAME_STRING.c_str()].GetString();
    int mana_cost = current_creature[MANA_COST_STRING.c_str()].GetInt();
    int damage_points = current_creature[DAMAGE_POINTS_STRING.c_str()].GetInt();
    int life_points = current_creature[LIFE_POINTS_STRING.c_str()].GetInt();
    bool shield = current_creature[SHIELD_STRING.c_str()].GetBool();
    bool mana_drain = current_creature[MANA_DRAIN_STRING.c_str()].GetBool();

    pick_up_stack_one_.push_back(new CreatureCard(name, mana_cost, \
                                                  damage_points, life_points, \
                                                  shield, mana_drain, false));

    pick_up_stack_two_.push_back(new CreatureCard(name, mana_cost, \
                                                 damage_points, life_points, \
                                                 shield, mana_drain, false));
  }
  return ConfigErrors::SUCCESS;
}

//------------------------------------------------------------------------------
Player& Game::getCurrentPlayer()
{
  return (current_player_ ? player_two_ : player_one_);
}
//------------------------------------------------------------------------------
Player& Game::getCurrentOpponent()
{
  return (current_player_ ? player_one_ : player_two_);
}
//------------------------------------------------------------------------------
void Game::displayCurrentGameState()
{
  Player &player = getCurrentPlayer();
  Player &opponent = getCurrentOpponent();
  io_.out(&player, &opponent);
}
//------------------------------------------------------------------------------
int Game::calculateManaGain()
{
  int new_mana = std::pow(Game::TWO, rounds);
  // this round's new mana gain is 2^(rounds)
  // while rounds < 3: new_mana = pow(2, rounds), else: 8
  new_mana = (new_mana > Interface::MAX_MANA_GAIN) ? \
              Interface::MAX_MANA_GAIN : new_mana;
  //
  return new_mana;
}
//------------------------------------------------------------------------------
void Game::carryOutManaGain()
{
  int new_mana = calculateManaGain();
  int previous_mana = getCurrentPlayer().getManaPoints();
  int mana_to_add = 0;
  int max_potential_mana = previous_mana + new_mana;

  if (previous_mana < Interface::MAX_MANA)
  {
    mana_to_add = (max_potential_mana < Interface::MAX_MANA) ?  \
                   max_potential_mana : Interface::MAX_MANA;
    mana_to_add -= previous_mana;
    getCurrentPlayer().addMana(mana_to_add);
  }
  // if mana is already max, do nothing
}
//------------------------------------------------------------------------------
void Game::newTurn()
{
  std::string current_player_name = "";

  if (current_player_ == CurrentPlayer::PLAYER_ONE)
  {
    player_one_.resetShieldCreauresAfterTurn();
    changeCurrentPlayer();
    carryOutManaGain();
    current_player_name = player_two_.getName();
    player_two_.resetCardStatusAtNewTurn();
    // since this happens before the draw, in each round only cards
    // are resetted which are at least one round old
    player_two_.pickUpCardFromPickUpStack();
    player_two_.resetAlreadyAttackedValues();

    io_.out(Interface::INFO, Interface::INFO_CURRENT_PLAYER + \
            current_player_name);
    displayCurrentGameState();

    checkIfPlayerLoses(player_two_);
    return;
  }
  if (current_player_ == CurrentPlayer::PLAYER_TWO)
  // if player 2 says finish, new round starts
  {
    player_two_.resetShieldCreauresAfterTurn();
    changeCurrentPlayer();
    rounds++;
    carryOutManaGain();
    current_player_name = player_one_.getName();
    player_one_.resetCardStatusAtNewTurn();
    // since this happens before the draw, in each round only cards
    // are resetted which are at least one round old
    player_one_.pickUpCardFromPickUpStack();
    player_one_.resetAlreadyAttackedValues();

    io_.out(Interface::INFO, Interface::INFO_ROUND + std::to_string(rounds));
    io_.out(Interface::INFO, Interface::INFO_CURRENT_PLAYER + \
            current_player_name);
    displayCurrentGameState();

    checkIfPlayerLoses(player_one_);
    return;
  }
}
//------------------------------------------------------------------------------
void Game::startGame()
{
  std::string player_one_name = io_.readPlayerName(current_player_);
  player_one_.setPlayerName(player_one_name);
  player_one_.setPickUpStack(pick_up_stack_one_);
  changeCurrentPlayer();

  std::string player_two_name = io_.readPlayerName(current_player_);
  player_two_.setPlayerName(player_two_name);
  player_two_.setPickUpStack(pick_up_stack_two_);
  changeCurrentPlayer();

  // at the start of the game, both players get dealt 3 cards
  // before the rounds even start
  player_one_.pickUpCardFromPickUpStack();
  player_one_.pickUpCardFromPickUpStack();
  player_one_.pickUpCardFromPickUpStack();

  player_two_.pickUpCardFromPickUpStack();
  player_two_.pickUpCardFromPickUpStack();
  player_two_.pickUpCardFromPickUpStack();

  // in the first round, the counter should remain round 0
  // however, mana-gain and pick up a card should still happen like always
  // thus, the newTurn() cannot be triggered to achieve this
  carryOutManaGain();
  player_one_.pickUpCardFromPickUpStack();

  io_.out(Interface::INFO, Interface::INFO_ROUND + std::to_string(rounds));
  io_.out(Interface::INFO, Interface::INFO_CURRENT_PLAYER + \
          player_one_name);
  io_.out(&getCurrentPlayer(), &getCurrentOpponent());
}
//------------------------------------------------------------------------------
void Game::changeCurrentPlayer()
{
  if (current_player_ == CurrentPlayer::PLAYER_ONE)
  {
    current_player_ = CurrentPlayer::PLAYER_TWO;
    return;
  }
  if (current_player_ == CurrentPlayer::PLAYER_TWO)
  {
    current_player_ = CurrentPlayer::PLAYER_ONE;
    return;
  }
}
//------------------------------------------------------------------------------
void Game::checkIfPlayerLoses(Player& player_to_check)
{
  if (player_to_check.getLifePoints() < Game::ONE)
  {
    endOfGameDialog();
  }
}
//------------------------------------------------------------------------------

void Game::endOfGameDialog()
{
  std::string winner_name = "";
  // the winner is always the current opponent
  winner_name = getCurrentOpponent().getName();

  io_.out(Interface::INFO, Interface::ENDLINE_PART_ONE + \
    winner_name + Interface::ENDLINE_PART_TWO);

  player_one_.freePlayersObjects();
  player_two_.freePlayersObjects();

  exit(Game::SUCCESSFUL_RETURN);
}
//------------------------------------------------------------------------------
int Game::checkIfBufferEmpty(std::istringstream& inputted_line_leftovers)
{
  std::string buffer = "";
  inputted_line_leftovers >> buffer;
  if (!buffer.empty())
  {
    return Player::WRONG_PARAMETER;
  }
  return Player::SUCCESS;
}
//------------------------------------------------------------------------------
void Game::parseCommandsHelpStateQuitFinish(Game::Command command, \
                                            std::istringstream& input_line)
{
  std::string buffer = "";
  int buffer_trash_present = 0;

  buffer_trash_present = checkIfBufferEmpty(input_line);
  if (buffer_trash_present)
  {
    io_.out(Interface::INFO, Interface::WARNING_WRONG_PARAM_COUNT);
    return;
  }
  if (command == Command::HELP)
  {
    for (auto message : Interface::INFO_HELP_MSGS)
    {
      io_.out(Interface::INFO, message);
    }
    return;
  }
  if (command == Command::STATE)
  {
    displayCurrentGameState();
    return;
  }
  if (command == Command::FINISH)
  {
    newTurn();
    return;
  }
  if (command == Command::QUIT)
  {
    endOfGameDialog();
    return;
  }
}
//------------------------------------------------------------------------------
void Game::parseCommandsWhoamiPlay(Game::Command command, \
                                            std::istringstream& input_line)
{
  std::string buffer = "";
  int buffer_trash_present = 0;
  std::string ai_command = "";

  buffer_trash_present = checkIfBufferEmpty(input_line);
  if (buffer_trash_present)
  {
    io_.out(Interface::INFO, Interface::WARNING_WRONG_PARAM_COUNT);
    return;
  }
  if (command == Command::WHOAMI)
  {
    std::string message = Game::STRING_TEAM + Game::STRING_TEAMNAME;
    io_.out(Interface::INFO, message);
    return;
  }
  if (command == Command::PLAY)
  {
    mode_ = Mode::AI; // setting mode to ai
    while ((mode_ == Mode::AI) && (ai_error_ == false))
    {
      ai_command = parseCommandInput();
      if (ai_command.empty())
      {
        break;
      }
    }
    return;
  }
}
//------------------------------------------------------------------------------
void Game::parseCommandsCastSacrifice(Game::Command command, \
                                      std::istringstream& input_line)
{
  std::string buffer = "";
  int buffer_trash_present = 0;
  int first_value = 0;
  Player::CommandError error_type;

  if (input_line.eof())
  {
    io_.out(Interface::INFO, Interface::WARNING_WRONG_PARAM_COUNT);
    return;
  }

  input_line >> first_value;
  if (!input_line.fail()) // checks if integer!
  {
    // is int; do nothing.
  }
  else
  {
    // not int;
    io_.out(Interface::INFO, Interface::WARNING_WRONG_PARAMETER);
    return;
  }

  buffer_trash_present = checkIfBufferEmpty(input_line);
  if (buffer_trash_present)
  {
    io_.out(Interface::INFO, Interface::WARNING_WRONG_PARAM_COUNT);
    return;
  }
  // the position to target is the provided value minus one
  size_t target_value = static_cast<size_t>(first_value - Game::ONE);

  if (command == Command::CAST)
  {
    if (getCurrentPlayer().getHandCards().at(target_value)->getType() \
        == Card::CardType::CREATURE)
    {
      error_type = Player::CommandError::EXECUTION_IMPOSSIBLE;
      printCommandErrorOnInterface(error_type);
      return;
    }

    SpellCard::SpellType the_card_type = dynamic_cast<SpellCard*>\
      (getCurrentPlayer().getHandCards().at(target_value))->getType();
    // card needs to be removed first before spell is cast
    Player &opponent = getCurrentOpponent();
    error_type = getCurrentPlayer().playCast(the_card_type,
                                             target_value, &opponent);
    printCommandErrorOnInterface(error_type);
    return;
  }

  if (command == Command::SACRIFICE)
  {
    error_type = getCurrentPlayer().playSacrifice(target_value);
    printCommandErrorOnInterface(error_type);
    return;
  }
}
/*
Comment for the tutor (Ass1):
We are aware that the following function will not correctly parse all the
testcases. If we fix it for one of
set 1 1
attack a b c
set 1 to 0,1
the others won't give the correct error message anymore.
In order to solve this, major changes in the whole command parsing would
have been necessary which we sadly cannot implement anymore before the
deadline. We decided thus to give you the best of our versions for this.
*/
//------------------------------------------------------------------------------
void Game::parseCommandsAttackSet(Game::Command command, \
                                  std::istringstream& input_line)
{
  int first_value = 0;
  int second_value = 0;
  std::string word = "";
  std::string buffer = "";
  int buffer_trash_present = 0;
  Player::CommandError error_type;
  if (input_line.eof())
  {
    io_.out(Interface::INFO, Interface::WARNING_WRONG_PARAM_COUNT);
    return;
  }
  input_line >> first_value >> word >> second_value;
  if (!input_line.fail())
  {
    // is int; do nothing;
  }
  else
  {
    // mistake is here: will fail whether second_value is empty or not int
    // giving the same error message. Checking for a placeholder value
    // doesn't work either because if it fails, second_value is unchanged
    io_.out(Interface::INFO, Interface::WARNING_WRONG_PARAMETER);
    return;
  }
  std::transform (word.begin(), word.end(), word.begin(), ::tolower);
  buffer_trash_present = checkIfBufferEmpty(input_line);
  if (buffer_trash_present)
  {
    io_.out(Interface::INFO, Interface::WARNING_WRONG_PARAM_COUNT);
    return;
  }

  if (command == Command::SET)
  {
    if (word != Game::STRING_TO)
    {
     io_.out(Interface::INFO, Interface::WARNING_WRONG_PARAMETER);
      return;
    }
    error_type = getCurrentPlayer().setCard(first_value, second_value);
    printCommandErrorOnInterface(error_type);
    return;
  }

  if (command == Command::ATTACK)
  {
    if (word != Game::STRING_WITH)
    {
      io_.out(Interface::INFO, Interface::WARNING_WRONG_PARAMETER);
      return;
    }
    Player& opponent = getCurrentOpponent();
    error_type = getCurrentPlayer().runAttack(static_cast<size_t>\
                (first_value), static_cast<size_t>(second_value), &opponent);
    printCommandErrorOnInterface(error_type);
    changeCurrentPlayer();
    checkIfPlayerLoses(opponent);
    changeCurrentPlayer();
    return;
  }
}
//------------------------------------------------------------------------------
void Game::printCommandErrorOnInterface(Player::CommandError error_type)
{
  switch (error_type)
  {
    case Player::CommandError::SUCCESS:
      // all good
      break;
    case Player::CommandError::WRONG_PARAMETER:
      io_.out(Interface::INFO, Interface::WARNING_WRONG_PARAMETER);
      break;
    case Player::CommandError::EXECUTION_IMPOSSIBLE:
      io_.out(Interface::INFO, Interface::WARNING_EXECUTION_NOT_POSSIBLE);
      break;
    case Player::CommandError::NOT_ENOUGH_MANA:
      io_.out(Interface::INFO, Interface::WARNING_NOT_ENOUGH_MANA);
      break;
    case Player::CommandError::REBIRTH_UNSUCCESSFUL:
      io_.out(Interface::INFO, Interface::WARNING_REBIRTH_UNSUCCESSFUL);
      break;
    case Player::CommandError::SHIELD:
      io_.out(Interface::INFO, Interface::WARNING_SHIELD_MONSTER);
      break;
    case Player::CommandError::CHECK_IF_GAME_ENDS:
    {
      Player& opponent = getCurrentOpponent();
      changeCurrentPlayer(); // needs to be changed in case of game ending
      // because the end of game dialog always prints the opponent as winning
      // in case of DRACULA, this needs to be switched for the check
      checkIfPlayerLoses(opponent);
      changeCurrentPlayer();
      break;
    }
    case Player::CommandError::CHECK_IF_PLAYER_LOSES:
      {
        checkIfPlayerLoses(getCurrentPlayer());
      }
      break;
  }
}
//------------------------------------------------------------------------------
std::string Game::parseCommandInput()
{
  std::string line = "";
  std::string command = "";
  io_.printCommandPrompt(getCurrentPlayer().getName());

  if (mode_ == Game::Mode::NORMAL)
  {
    line = io_.in();
  }
  else // mode is AI (Ass 2)
  {
    if (ai_error_ == false)
    {
      line = getAICommand(); // never returns empty
      if (line == last_ai_command_) // then we are looping
      {
        ai_error_ = true;
        return command;
      }
    }
    if (ai_error_ == true)
    {
      command == Interface::COMMAND_FINISH;
      last_ai_command_ = Interface::COMMAND_HELP;
      ai_error_ = false;
      mode_ = Mode::NORMAL;
      return command;
    }

    io_.out(Interface::INFO, (Game::STRING_COMMAND + line));
    last_ai_command_ = line;
  }

  std::istringstream inputted_line(line);
  inputted_line >> command;
  if (command == "")
  {
    return command;
  }

  std::transform (command.begin(), command.end(), command.begin(), ::tolower);
  Command chosen_command = parseCommandToEnum(command);

  if (chosen_command == Command::HELP || chosen_command == Command::STATE
   || chosen_command == Command::FINISH || chosen_command == Command::QUIT)
  {
    parseCommandsHelpStateQuitFinish(chosen_command, inputted_line);
  }
  else if  (chosen_command == Command::CAST
        || chosen_command ==  Command::SACRIFICE)
  {
    parseCommandsCastSacrifice(chosen_command, inputted_line);
  }
  else if  (chosen_command == Command::ATTACK || chosen_command == Command::SET)
  {
    parseCommandsAttackSet(chosen_command, inputted_line);
  }
  else if  (chosen_command == Command::NON_EXISTING)
  {
    io_.out(Interface::INFO, Interface::WARNING_UNKNOWN_COMMAND);
  }
  else if (chosen_command == Command::WHOAMI || chosen_command == Command::PLAY)
  {
    parseCommandsWhoamiPlay(chosen_command, inputted_line);
  }
  inputted_line.clear();

  return command;
}

//------------------------------------------------------------------------------
Game::Command Game::parseCommandToEnum(std::string command_string)
{
  if (command_string == Interface::COMMAND_HELP)
  {
    return Command::HELP;
  }
  if (command_string == Interface::COMMAND_ATTACK)
  {
    return Command::ATTACK;
  }
  if (command_string == Interface::COMMAND_SET)
  {
    return Command::SET;
  }
  if (command_string == Interface::COMMAND_CAST)
  {
    return Command::CAST;
  }
  if (command_string == Interface::COMMAND_SACRIFICE)
  {
    return Command::SACRIFICE;
  }
  if (command_string == Interface::COMMAND_STATE)
  {
    return Command::STATE;
  }
  if (command_string == Interface::COMMAND_FINISH)
  {
    return Command::FINISH;
  }
  if (command_string == Interface::COMMAND_QUIT)
  {
    return Command::QUIT;
  }
  if (command_string == Game::STRING_PLAY)
  {
    return Command::PLAY;
  }
  if (command_string == Game::STRING_WHOAMI)
  {
    return Command::WHOAMI;
  }
  else
  {
    return Command::NON_EXISTING;
  }
}
//------------------------------------------------------------------------------
void Game::run()
{
  bool playing = true;
  current_player_ = CurrentPlayer::PLAYER_ONE;
  mode_ = Mode::NORMAL;
  startGame();

  // this loop will prompt users for new input until someone quits
  while (playing)
  {
    parseCommandInput();
  }
}

//------------------------------------------------------------------------------
std::string Game::getShieldCardToSetForAI()
{
  Player &player = getCurrentPlayer();
  std::string command = "";
  int shield_position = 0;
  bool shield_was_found_which_can_be_set = false;

  // check if there is a shield card for which we have enough mana points
  for (auto &card : player.getHandCards())
  {
    Card::CardType the_card_type = card->getType();
    if (the_card_type == Card::CardType::CREATURE)
    {
      bool has_shield = dynamic_cast<CreatureCard*>(card)->getShield();
      if ((has_shield == true) &&
          (card->getManaCost() <= player.getManaPoints()) )
      {
        shield_was_found_which_can_be_set = true;
        break;
      }
    }
    shield_position++;
  }
  if (!shield_was_found_which_can_be_set)
  {
    return command;
  }
  shield_position += Game::ONE;

  // playing SET
  // a shield creature card was found for which we have enough mana to set
  for (size_t position = 0; position < Player::SIZE_GAME_FIELD; position++)
  {
    if (player.getGameField()[position] == nullptr)
    {
      int set_card_to = (static_cast<int>(position) + Game::ONE);
      command = Interface::COMMAND_SET + Game::STRING_SPACE + \
                std::to_string(shield_position) + Game::STRING_TO_SPACED + \
                std::to_string(set_card_to);
      return command;
    }
  }
  // no free spot to set to on gamefield
  return command;
}

//------------------------------------------------------------------------------
std::string Game::getAICommand()
{
  Player &player = getCurrentPlayer();
  std::string command = "";
  int hand_counter = 0;
  std::string card_to_use = "";
  int attacker_position = 0;
  int target_to_attack = 0;

  // first check if we have a shield card to set (which is a priority)
  command = getShieldCardToSetForAI();
  if (!command.empty())
  {
    return command;
  }

  // then check if there's anything else we can set/use
  // if enough mana points and gamefield space free, set the next possible card
  for (auto &card : player.getHandCards())
  {
    if (card->getManaCost() > player.getManaPoints())
    {
      hand_counter++; // not enough points to set this card
      continue;
    }
    else
    {
      command = manageHandCardforAI(card, hand_counter);
      if (!command.empty())
      {
        return command;
      }
    }
  }

  // after we checked if we can set or cast anything, we perform attacks with
  // gamefield creatures who can attack, targeting shields, if any.
  // If not, attack opponent directly.
  attacker_position = getAttackingCreatureForAI();
  target_to_attack = getAttackTargetCreatureForAI();

  if ((attacker_position != Game::ZERO) && // if 0, there was no own creature
      (target_to_attack != Game::INVALID) &&
      (ai_error_ == false))
  {
    command = Interface::COMMAND_ATTACK + Game::STRING_SPACE + \
              std::to_string(target_to_attack) + \
              Game::STRING_WITH_SPACED + std::to_string(attacker_position);
    return command;
  }
  else // in case none of the above apply (no set, cast or attack possible):
  {
    command = Interface::COMMAND_FINISH;
    mode_ = Mode::NORMAL;
    last_ai_command_ = Interface::COMMAND_HELP;
    ai_error_ = false;
    return command;
  }
}
//------------------------------------------------------------------------------
int Game::getAttackingCreatureForAI()
{
  Player &player = getCurrentPlayer();
  int attacker_position = 0;
  for (size_t position = 0; position < Player::SIZE_GAME_FIELD; position++)
  {
    if ((player.getGameField()[position] != nullptr) &&
        (player.getGameField()[position]->getReadyToFight() == true) &&
        (player.getGameField()[position]->getAlreadyAttacked() == false))
    {
      attacker_position = static_cast<int>(position) + Game::ONE;
    }
  }
  return attacker_position;
}

//------------------------------------------------------------------------------
int Game::getAttackTargetCreatureForAI()
{
  int target_to_attack = 0;
  Player &opponent = getCurrentOpponent();
  for (size_t position = 0; position < Player::SIZE_GAME_FIELD; position++)
  {
    if ((opponent.getGameField()[position] != nullptr) &&
        (opponent.getGameField()[position]->getShield() == true))
    {
      if (opponent.getGameField()[position]->getReadyToFight() == true)
      {
        target_to_attack = static_cast<int>(position) + Game::ONE;
        return target_to_attack;
      }
      else
      {
        target_to_attack = Game::INVALID;
        return target_to_attack;
      }
    }
    // else target_to_attack is already set to zero, i.e. the opponent
  }
  return target_to_attack;
}
//------------------------------------------------------------------------------
std::string Game::manageHandCardforAI(Card* card, int hand_counter)
{
  Player &player = getCurrentPlayer();
  std::string command = "";
  std::string card_to_use = "";
  bool free_space_to_set = false;
  int set_card_to = 0;

  card_to_use = std::to_string(hand_counter + Game::ONE);
  Card::CardType the_card_type = card->getType();

  // playing CAST
  if (the_card_type == Card::CardType::SPELL)
  {
    SpellCard::SpellType the_spell_type = dynamic_cast<SpellCard*>\
      (card)->getType();
    if ((the_spell_type == SpellCard::SpellType::RELIEF) &&
        (player.getPickUpStackSize() < Game::THREE) &&
        (player.getHandSize() > Game::FOUR))
    {
      return command; // don't cast if relief would cause loss of points
    }
    if ((the_spell_type == SpellCard::SpellType::REBIRTH) &&
        (player.graveyardIsEmpty()))
    {
      return command; // don't cast if rebirth impossible
    }
    command = Interface::COMMAND_CAST + Game::STRING_SPACE + card_to_use;
    return command;
  }

  // playing SET
  // a creature card was found for which we have enough mana to set
  for (size_t position = 0; position < Player::SIZE_GAME_FIELD; position++)
  {
    if (player.getGameField()[position] == nullptr) // nullptr = field empty
    {
      set_card_to = (static_cast<int>(position) + Game::ONE);
      free_space_to_set = true;
      if (free_space_to_set)
      {
        command = Interface::COMMAND_SET + Game::STRING_SPACE + card_to_use + \
                  Game::STRING_TO_SPACED + std::to_string(set_card_to);
        return command;
      }
    }
  }

  // playing SACRIFICE in case hand is still too full after setting
  if (player.getHandSize() == Game::HAND_IS_FULL)
  {
    if ((the_card_type == Card::CardType::CREATURE) &&
        (dynamic_cast<CreatureCard*>(card)->getShield() == false))
    {
      // don't sacrifice if it's a valuable shield card
      return command;
    }
    else if (player.getLifePoints() < Game::RUNNING_OUT_OF_LIFEPOINTS)
    {
      // only start sacrifices when life points run out
      return command;
    }
    command = Interface::COMMAND_SACRIFICE + Game::STRING_SPACE + card_to_use;
    return command;
  }

  return command;
}
