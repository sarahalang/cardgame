//------------------------------------------------------------------------------
// SpellCard.hpp
//
// Group: Group 6.6, study assistant: Oberhuber M.
//
// Authors: Beate Eberhard (00835175)
// Sarah Lang (01110645)
// Created: 26.03.2020
// Last change: 12.05.2020
//------------------------------------------------------------------------------
//

#ifndef SPELLCARD_H_INCLUDED
#define SPELLCARD_H_INCLUDED

#include "Card.hpp"

namespace Oop
{
  class Game;

  //----------------------------------------------------------------------------
  // SpellCard Class
  // Derived class from Card with type SpellCard
  //
  class SpellCard : public Card
  {
  public:
    //--------------------------------------------------------------------------
    // enums
    //
    enum SpellType {HEALER, RELIEF, REBIRTH, DRACULA, TRAITOR};

    //--------------------------------------------------------------------------
    // constants
    //
    static const int MANA_COST_HEALER = 5;
    static const int MANA_COST_RELIEF = 3;
    static const int MANA_COST_REBIRTH = 5;
    static const int MANA_COST_DRACULA = 2;
    static const int MANA_COST_TRAITOR = 9;
    static const int ZERO = 0;

    //--------------------------------------------------------------------------
    // Constructor
    //
    SpellCard(SpellType type);

    //--------------------------------------------------------------------------
    // Copy Constructor
    // Makes a copy of another SpellCard Object.
    // @param original Original to copy.
    //
    SpellCard(const SpellCard& original) = default;

    //--------------------------------------------------------------------------
    // Assignment Operator
    // Used to assign one SpellCard to another
    // @param original Original with values to copy.
    //
    SpellCard& operator= (SpellCard const& original) = default;

    //--------------------------------------------------------------------------
    // Destructor
    //
    virtual ~SpellCard();

    //--------------------------------------------------------------------------
    // getter methods
    //
    SpellType getType() const;

    //--------------------------------------------------------------------------
    // methods
    //
    bool action(Game& game);

  private:
    //--------------------------------------------------------------------------
    // Defines the type of spell.
    //
    SpellType spell_type_;
  };
}

#endif // SPELLCARD_H_INCLUDED
