//------------------------------------------------------------------------------
// SpellCard.cpp
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
#include "SpellCard.hpp"
#include "Interface.hpp"

using Oop::SpellCard;

//------------------------------------------------------------------------------
SpellCard::SpellCard(SpellType type): Card("", SpellCard::ZERO,
                                           CardType::SPELL), spell_type_(type)
{
  switch (spell_type_)
  {
    case HEALER:
      this->mana_cost_ = SpellCard::MANA_COST_HEALER;
      this->name_ = Interface::STRING_HEALER;
      break;
    case RELIEF:
      this->mana_cost_ = SpellCard::MANA_COST_RELIEF;
      this->name_ = Interface::STRING_RELIEF;
      break;
    case REBIRTH:
      this->mana_cost_ = SpellCard::MANA_COST_REBIRTH;
      this->name_ = Interface::STRING_REBIRTH;
      break;
    case DRACULA:
      this->mana_cost_ = SpellCard::MANA_COST_DRACULA;
      this->name_ = Interface::STRING_DRACULA;
      break;
    case TRAITOR:
      this->mana_cost_ = SpellCard::MANA_COST_TRAITOR;
      this->name_ = Interface::STRING_TRAITOR;
      break;
  }
}
//------------------------------------------------------------------------------
SpellCard::SpellType SpellCard::getType() const
{
  return spell_type_;
}

//------------------------------------------------------------------------------
SpellCard::~SpellCard()
{
  // destructor
}

/*
--------------------------------------------------------------------------------
bool SpellCard::action(Game& game)
{
  return false;
}
according to newsgroup: if no errors in Interface, this is not needed
functionality implemented elsewhere
*/
