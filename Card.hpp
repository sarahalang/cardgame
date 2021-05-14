//------------------------------------------------------------------------------
// Card.hpp
//
// Group: Group 6.6, study assistant: Oberhuber M.
//
// Authors: Beate Eberhard (00835175)
// Sarah Lang (01110645)
// Created: 26.03.2020
// Last change: 12.05.2020
//------------------------------------------------------------------------------
//

#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED

#include <string>
#include <iostream>

namespace Oop
{
  //----------------------------------------------------------------------------
  // Card Class
  // base class for play cards SpellCard and CreatureCard
  //
  class Card
  {
  public:
    //--------------------------------------------------------------------------
    // Enumeration of all possible Card Types
    //
    enum CardType {CREATURE, SPELL};

    //--------------------------------------------------------------------------
    // Constructor
    //
    Card(std::string name, int mana_cost, CardType type);

    //--------------------------------------------------------------------------
    // Deleted Copy Constructor
    // Makes a copy of another Card Object.
    // @param original Original to copy.
    //
    Card(const Card& original) = default;

    //--------------------------------------------------------------------------
    // Deleted Assignment Operator
    // Used to assign one Card to another
    // @param original Original with values to copy.
    //
    Card& operator= (Card const& original) = default;

    //--------------------------------------------------------------------------
    // getter methods
    //
    std::string getName() const;
    CardType getType() const;
    int getManaCost() const;

    //--------------------------------------------------------------------------
    // Destructor
    //
    virtual ~Card() = 0;

  protected:
    //--------------------------------------------------------------------------
    // Name of the card as string, coming from config file, max. 8 chars.
    std::string name_;

    //--------------------------------------------------------------------------
    // Mana Cost of the card as int (values 1-15 incl.)
    // This attribute can be changed after initialization
    //
    int mana_cost_;

    //--------------------------------------------------------------------------
    // Type of the card, as per the CardType enum
    // const enum
    //
    const CardType type_;
  };
}

#endif // CARD_H_INCLUDED
