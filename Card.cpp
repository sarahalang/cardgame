//------------------------------------------------------------------------------
// Card.cpp
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

using Oop::Card;

//------------------------------------------------------------------------------
std::string Card::getName() const
{
  return name_;
}
//------------------------------------------------------------------------------

Card::CardType Card::getType() const
{
  return type_;
}
//------------------------------------------------------------------------------

int Card::getManaCost() const
{
  return mana_cost_;
}
//------------------------------------------------------------------------------
Card::Card(std::string name, int mana_cost, Card::CardType type) :
                                                  name_(name), \
                                                  mana_cost_(mana_cost), \
                                                  type_(type)
{}

//------------------------------------------------------------------------------
Card::~Card()
{
  // destructor
}
