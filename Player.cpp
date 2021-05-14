//------------------------------------------------------------------------------
// Player.cpp
//
// Group: Group 6, study assistant: Oberhuber M.
//
// Authors: Sarah Lang (01110645)
// Created: 25.03.2020
// Last change: 16.06.2020
//------------------------------------------------------------------------------
//

#include "Player.hpp"


using Oop::Player;

Player::Player(std::string name)
        : name_(name), life_points_(Player::START_LIFE_POINTS),
          mana_points_(Player::ZERO_POINTS)
    {
    }

   Player::Player()
        : name_(""), life_points_(Player::START_LIFE_POINTS),
          mana_points_(Player::ZERO_POINTS)
    {
    }


//------------------------------------------------------------------------------
std::string Player::getName() const
{
  return name_;
}
//------------------------------------------------------------------------------
int Player::getLifePoints() const
{
  return life_points_;
}
//------------------------------------------------------------------------------
bool Player::graveyardIsEmpty() const
{
  if (graveyard_.empty())
  {
    return true;
  }
  return false;
}
//------------------------------------------------------------------------------
int Player::getPickUpStackSize() const
{
  return static_cast<int>(pick_up_stack_.size());
}
//------------------------------------------------------------------------------
void Player::setPlayerName(std::string name)
{
  name_ = name;
}
//------------------------------------------------------------------------------
void Player::setPickUpStack(std::vector<Card*> pick_up_stack)
{
  for (auto &card : pick_up_stack)
  {
    pick_up_stack_.push_back(card);
  }
}
//------------------------------------------------------------------------------
void Player::pickUpCardFromPickUpStack()
{
  if ((getHandSize() >= Interface::MAX_HAND_CARDS) || pick_up_stack_.empty())
  {
    reduceLifePoints(Player::ONE_POINT);
  }
  else
  {
    try
    {
      hand_.push_back(pick_up_stack_.back());
      pick_up_stack_.pop_back();
    }
    catch (...)
    {
      return;
    }
  }
}
//------------------------------------------------------------------------------
void Player::castRelief()
{
  reduceMana(Player::RELIEF_MANA_COST);
  pickUpCardFromPickUpStack();
  pickUpCardFromPickUpStack();
  pickUpCardFromPickUpStack();
}
//------------------------------------------------------------------------------
bool Player::castRebirth()
{
  reduceMana(Player::REBIRTH_MANA_COST);
  bool success = false;
  if (graveyard_.empty())
  {
    return success;
  }
  for (size_t position = 0; position < game_field_.size(); position++)
  {
    if (game_field_[position] == nullptr) // nullptr means field is empty
    {
      game_field_[position] = graveyard_.back();
      success = true;
      graveyard_.pop_back();
      game_field_[position]->restoreCreature();
      break;
    }
  }
  return success;
}
//------------------------------------------------------------------------------
void Player::castHealer()
{
  reduceMana(Player::HEALER_MANA_COST);
  for (size_t position = 0; position < game_field_.size(); position++)
  {
    if (game_field_[position] != nullptr)
    {
      game_field_[position]->healCreature();
    }
  }
}

//------------------------------------------------------------------------------
Player::CommandError Player::playCast(SpellCard::SpellType spell_type, \
  size_t card_position, Player* opponent)
{
  int mana_cost = 0;
  switch (spell_type)
  {
    case SpellCard::SpellType::HEALER:
      mana_cost = SpellCard::MANA_COST_HEALER;
      break;
    case SpellCard::SpellType::RELIEF:
      mana_cost = SpellCard::MANA_COST_RELIEF;
      break;
    case SpellCard::SpellType::REBIRTH:
      mana_cost = SpellCard::MANA_COST_REBIRTH;
      break;
    case SpellCard::SpellType::DRACULA:
      mana_cost = SpellCard::MANA_COST_DRACULA;
      break;
    case SpellCard::SpellType::TRAITOR:
      mana_cost = SpellCard::MANA_COST_TRAITOR;
      break;
  }
  if (mana_cost > getManaPoints())
  {
    return CommandError::NOT_ENOUGH_MANA;
  }
  removeHandCardAfterUse(card_position + \
                         static_cast<size_t>(Player::ONE_POINT));

  switch (spell_type)
  {
    case SpellCard::SpellType::HEALER:
      castHealer();
      break;
    case SpellCard::SpellType::RELIEF:
      castRelief();
      return CommandError::CHECK_IF_PLAYER_LOSES;
      break;
    case SpellCard::SpellType::REBIRTH:
      {
        bool success = false;
        success = castRebirth();
        if (!success)
        {
          return CommandError::REBIRTH_UNSUCCESSFUL;
        }
      }
      break;
    case SpellCard::SpellType::DRACULA:
      {
        mana_cost = SpellCard::MANA_COST_DRACULA;
        reduceMana(mana_cost);
        addLifePoints(Player::DRACULA_BOOST);
        opponent->reduceLifePoints(Player::DRACULA_BOOST);
        return CommandError::CHECK_IF_GAME_ENDS;
      }
    case SpellCard::SpellType::TRAITOR:
      mana_cost = SpellCard::MANA_COST_TRAITOR;
      // not implemented
      break;
  }
  return CommandError::SUCCESS;
}
//------------------------------------------------------------------------------
Player::CommandError Player::playSacrifice(size_t the_hand_position)
{
  // the value is greater than the [array style] position by one
  int position = static_cast<int>(the_hand_position + Player::ONE_POINT);

  if ( !( (position > Player::ZERO) && (position <= Interface::MAX_HAND_CARDS)))
  {
    return CommandError::WRONG_PARAMETER;
  }
  // if value not in this range, position doesn't exist / is empty
  if ( !( (position > Player::ZERO) && (position <= getHandSize()) ))
  {
    return CommandError::EXECUTION_IMPOSSIBLE;
  }

  if (hand_.at(the_hand_position)->getType() == Card::CardType::CREATURE)
  {
    graveyard_.push_back((dynamic_cast<CreatureCard*>\
                                               (hand_.at(the_hand_position))));
  }
  if (hand_.at(the_hand_position)->getType() == Card::CardType::SPELL)
  {
    // card will just be deleted, not put to graveyard
  }
  removeHandCardAfterUse(the_hand_position + Player::ONE_POINT);
  addLifePoints(Player::ONE_POINT);
  return CommandError::SUCCESS;
}

//------------------------------------------------------------------------------
void Player::resetCardStatusAtNewTurn()
{
  for (size_t i = 0; i < static_cast<size_t>(Player::SIZE_GAME_FIELD); i++)
  {
    if (game_field_[i] != nullptr)
    {
      if (!(game_field_[i]->getReadyToFight() == true))
      {
        game_field_[i]->setReadyToFight();
      }
    }
  }
}

//------------------------------------------------------------------------------
void Player::resetAlreadyAttackedValues()
{
  for (size_t i = 0; i < static_cast<size_t>(Player::SIZE_GAME_FIELD); i++)
  {
    if (game_field_[i] != nullptr)
    {
      game_field_[i]->resetAlreadyAttacked();
    }
  }
}
//------------------------------------------------------------------------------
void Player::resetShieldCreauresAfterTurn()
{
  // starting from 1 because the player is on position 0
  for (size_t i = 0; i < static_cast<size_t>(Player::SIZE_GAME_FIELD); i++)
  {
    if ( (game_field_[i] != nullptr) && (game_field_[i]->getShield() == true) )
    {
      if (!(game_field_[i]->getReadyToFight() == true))
      {
        game_field_[i]->setReadyToFight();
      }
    }
  }
}
//------------------------------------------------------------------------------
void Player::removeHandCardAfterUse(int the_card_position)
{
  int position_to_delete = (the_card_position - Player::ONE_POINT);
  hand_.erase(hand_.begin() + position_to_delete);
}
//------------------------------------------------------------------------------
Player::CommandError Player::runAttack(size_t position_opponent, \
  size_t position_self, Player* opponent)
{
  int damage = 0;
  int opponent_target = static_cast<int>(position_opponent);
  // opponent_target is not "normalized" (-1): to check if opponent is attacked
  position_self -= static_cast<size_t>(Player::ONE_POINT);
  position_opponent -= static_cast<size_t>(Player::ONE_POINT);
  bool opponent_has_shield = false;

  for (auto iterator = opponent->game_field_.begin(); \
       iterator != opponent->game_field_.end(); iterator++)
  {
    if ((*iterator) != nullptr)
    {
      if ((*iterator)->getShield() == true)
      {
        opponent_has_shield = true;
      }
    }
  }
  if ( (opponent_target < Player::OPPONENT_PLACE) ||
       (opponent_target > Player::MAX_ARRAY_POSITION) ||
       (static_cast<int>(position_self) < Player::ZERO) ||
       (static_cast<int>(position_self) >= Player::MAX_ARRAY_POSITION) )
  {
    return CommandError::WRONG_PARAMETER;
  }
  if ( (game_field_[position_self] == nullptr) ||
       (game_field_[position_self]->getAlreadyAttacked() == true) ||
       (game_field_[position_self]->getReadyToFight() == false) )
  {
    return CommandError::EXECUTION_IMPOSSIBLE;
  }
  damage = game_field_[position_self]->getDamagePoints();
  // attacking the opponent directly - only possible if no shield
  if (opponent_target < Player::MINIMUM_ARRAY_POSITION)
  {
    if (opponent_has_shield)
    {
      return CommandError::SHIELD;
    }
    if (game_field_[position_self]->getManaDrain() == true)
    {
      opponent->drainMana();
    }
    opponent->reduceLifePoints(damage);
    game_field_[position_self]->setAlreadyAttacked();
    return CommandError::SUCCESS;
  }
  // you can only attack shield creatures if the opponent has a shield
  if ((opponent_has_shield) &&
      (opponent->game_field_[position_opponent]->getShield() == false))
  {
    return CommandError::SHIELD;
  }
  if ( (opponent->game_field_[position_opponent] == nullptr) ||
       (opponent->game_field_[position_opponent]->getReadyToFight() == false) )
  {
    return CommandError::EXECUTION_IMPOSSIBLE;
  }
  opponent->game_field_[position_opponent]->reduceCurrentLifePoints(damage);
  if (opponent->game_field_[position_opponent]->creatureIsDead())
  {
    opponent->moveCreatureToGraveyard(position_opponent);
  }
  game_field_[position_self]->setAlreadyAttacked();
  return CommandError::CHECK_IF_GAME_ENDS;
}
//------------------------------------------------------------------------------
void Player::moveCreatureToGraveyard(size_t the_field_position)
{
  graveyard_.push_back(game_field_[the_field_position]);
  game_field_[the_field_position] = nullptr;
}
//------------------------------------------------------------------------------
Player::CommandError Player::setCard(int position_hand, int position_gamefield)
{
  if ( (position_hand > Interface::MAX_HAND_CARDS) ||
       (position_gamefield > Player::SIZE_GAME_FIELD) ||
       (position_hand < Player::MINIMUM_ARRAY_POSITION) ||
       (position_gamefield < Player::MINIMUM_ARRAY_POSITION) )
  {
    return CommandError::WRONG_PARAMETER;
  }

  // game_field[0] is the opponent, so the target value does not need to be
  // "normalized" by one, but the hand_ value does: starts with 0
  size_t own = static_cast<size_t>(position_hand - Player::ONE_POINT);
  size_t enemy = static_cast<size_t>(position_gamefield - Player::ONE_POINT);

  if ( !((position_hand > Player::ZERO) &&
       (position_hand < getHandSize() + Player::ONE_POINT) ) ||
       (game_field_[enemy] != nullptr) )
  {
    return CommandError::EXECUTION_IMPOSSIBLE;
  }


  if (hand_.at(own)->getType() == Card::CardType::CREATURE)
  {
    if (hand_.at(own)->getManaCost() > getManaPoints())
    {
      return CommandError::NOT_ENOUGH_MANA;
    }
    else
    {
      reduceMana(hand_.at(own)->getManaCost());
      game_field_[enemy] = dynamic_cast<CreatureCard*>(hand_.at(own));
      removeHandCardAfterUse(position_hand);
      return CommandError::SUCCESS;
    }
  }
  else
  {
    return CommandError::EXECUTION_IMPOSSIBLE;
  }
}

//------------------------------------------------------------------------------
void Player::freePlayersObjects()
{
  for (auto &card : pick_up_stack_)
  {
    delete card;
  }
  pick_up_stack_.clear();

  for (auto &card : hand_)
  {
    delete card;
  }
  hand_.clear();

  for (auto &card : graveyard_)
  {
    delete card;
  }
  graveyard_.clear();

  for (auto iterator = game_field_.begin(); iterator != game_field_.end(); \
       iterator++)
  {
    if ((*iterator) != nullptr)
    {
      delete *iterator;
      *iterator = nullptr;
    }
  }
}
//------------------------------------------------------------------------------
void Player::addLifePoints(int points)
{
  life_points_ += points;
}
//------------------------------------------------------------------------------
void Player::reduceLifePoints(int points)
{
  if (points > Player::ZERO_POINTS)
  {
    life_points_ -= points;
  }
  if (life_points_ < Player::ZERO_POINTS)
  {
    life_points_ = Player::ZERO_POINTS;
  }
}
//------------------------------------------------------------------------------
int Player::getManaPoints() const
{
  return mana_points_;
}
//------------------------------------------------------------------------------
void Player::addMana(int mana)
{
  if (mana < Player::ZERO_POINTS)
  {
    mana = Player::ZERO_POINTS;
  }
  if (mana <= Player::MAX_MANA_POINTS)
  {
    mana_points_ += mana;
  }
  if (mana_points_ > Player::MAX_MANA_POINTS)
  {
    mana_points_ = Player::MAX_MANA_POINTS;
  }
}
//------------------------------------------------------------------------------
void Player::reduceMana(int mana)
{
  // only reduce mana if value is valid
  if (mana > Player::ZERO_POINTS)
  {
    mana_points_ -= mana;
  }
  // if the mana_points_ were reduced to below zero, resize to zero
  if (mana_points_ < Player::ZERO_POINTS)
  {
    mana_points_ = Player::ZERO_POINTS;
  }
}
//------------------------------------------------------------------------------
void Player::drainMana()
{
  mana_points_ = ZERO_POINTS;
}
//------------------------------------------------------------------------------
std::vector<Oop::Card*> Player::getHandCards() const
{
  return hand_;
}
//------------------------------------------------------------------------------
int Player::getHandSize() const
{
  return hand_.size();
}
//------------------------------------------------------------------------------
Oop::CreatureCard* const* Oop::Player::getGameField() const
{
  return game_field_.data();
}
