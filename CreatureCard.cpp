//------------------------------------------------------------------------------
// CreatureCard.cpp
//
// Group: Group 6.6, study assistant: Oberhuber M.
//
// Authors: Beate Eberhard (00835175)
// Sarah Lang (01110645)
// Created: 26.03.2020
// Last change: 12.05.2020
//------------------------------------------------------------------------------
//

#include "Card.hpp"
#include "CreatureCard.hpp"

using Oop::CreatureCard;

//------------------------------------------------------------------------------
int CreatureCard::getLifePoints() const
{
  return life_points_;
}
//------------------------------------------------------------------------------
int CreatureCard::getDamagePoints() const
{
  return damage_points_;
}
//------------------------------------------------------------------------------
bool CreatureCard::getManaDrain() const
{
  return mana_drain_;
}
//------------------------------------------------------------------------------
bool CreatureCard::getShield() const
{
  return shield_;
}
//------------------------------------------------------------------------------
int CreatureCard::getCurrentLifePoints() const
{
  return current_life_points_;
}
//------------------------------------------------------------------------------
bool CreatureCard::getReadyToFight() const
{
  return ready_to_fight_;
}
//------------------------------------------------------------------------------
bool CreatureCard::getAlreadyAttacked() const
{
  return already_attacked_;
}
//------------------------------------------------------------------------------

// Bonustask getter
bool CreatureCard::getSpeedy() const
{
  return speedy_;
}
//------------------------------------------------------------------------------
void CreatureCard::setReadyToFight()
{
  ready_to_fight_ = true;
}
//------------------------------------------------------------------------------
void CreatureCard::setAlreadyAttacked()
{
  already_attacked_ = true;
}
//------------------------------------------------------------------------------
void CreatureCard::resetAlreadyAttacked()
{
  already_attacked_ = false;
}

//------------------------------------------------------------------------------
void CreatureCard::healCreature()
{
  int max_gain = current_life_points_ + \
                 CreatureCard::ONE_POINT;
  int healed = (max_gain > CreatureCard::MAX_LIFE_POINTS) ? \
               MAX_LIFE_POINTS : max_gain;
  current_life_points_ = healed;
}
//------------------------------------------------------------------------------
void CreatureCard::reduceCurrentLifePoints(int points)
{
  current_life_points_ -= points;
  if (current_life_points_ < CreatureCard::DEAD)
  {
    current_life_points_ = CreatureCard::DEAD;
  }
}
//------------------------------------------------------------------------------
void CreatureCard::restoreCreature()
{
  current_life_points_ = life_points_;
  already_attacked_ = false;
  ready_to_fight_ = false;
}
//------------------------------------------------------------------------------
bool CreatureCard::creatureIsDead()
{
  if (getCurrentLifePoints() == CreatureCard::DEAD)
  {
    return true;
  }
  else
  {
    return false;
  }
}
//------------------------------------------------------------------------------
CreatureCard::CreatureCard(std::string name, int mana_cost, \
  int damage_points, int life_points, bool shield, bool mana_drain, \
  bool speedy) :
  Card::Card(name, mana_cost, CardType::CREATURE),
  damage_points_(damage_points), life_points_(life_points), \
  shield_(shield), mana_drain_(mana_drain), speedy_(speedy), \
  ready_to_fight_(false), already_attacked_(false)
  {
    current_life_points_ = life_points_;
  }

//------------------------------------------------------------------------------
CreatureCard::~CreatureCard()
{
  // destructor
}
