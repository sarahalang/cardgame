//------------------------------------------------------------------------------
// CreatureCard.hpp
//
// Group: Group 6.6, study assistant: Oberhuber M.
//
// Authors: Beate Eberhard (00835175)
// Sarah Lang (01110645)
// Created: 26.03.2020
// Last change: 12.05.2020
//------------------------------------------------------------------------------
//

#ifndef CREATURECARD_H_INCLUDED
#define CREATURECARD_H_INCLUDED

#include "Card.hpp"

namespace Oop
{
  //----------------------------------------------------------------------------
  // CreatureCard Class
  // Derived class from Card with type CreatureCreature
  //
  class CreatureCard : public Card
  {
  public:
    //--------------------------------------------------------------------------
    // constants
    //
    static const int ONE_POINT = 1;
    static const int DEAD = 0;
    static const int MAX_LIFE_POINTS = 9;

    //--------------------------------------------------------------------------
    // Constructor
    //
    CreatureCard(std::string name, int mana_cost, int damage_points, \
      int life_points, bool shield, bool mana_drain, bool speedy);

    //--------------------------------------------------------------------------
    // Copy Constructor
    // Makes a copy of another CreatureCard Object.
    // @param original Original to copy.
    //
    CreatureCard(const CreatureCard& original) = default;

    //--------------------------------------------------------------------------
    // Assignment Operator
    // Used to assign one CreatureCard to another
    // @param original Original with values to copy.
    //
    CreatureCard& operator= (CreatureCard const& original) = default;

    //--------------------------------------------------------------------------
    // getter methods
    //
    int getLifePoints() const;
    int getDamagePoints() const;
    bool getManaDrain() const;
    bool getShield() const;
    int getCurrentLifePoints() const;
    bool getReadyToFight() const;
    bool getAlreadyAttacked() const;
    bool getSpeedy() const;

    //--------------------------------------------------------------------------
    // setter methods
    //
    void setReadyToFight();
    void setAlreadyAttacked();
    void resetAlreadyAttacked();

    //--------------------------------------------------------------------------
    // methods
    //

    //--------------------------------------------------------------------------
    // healCreature
    // heals the creature it is effected on by adding one life point
    //
    void healCreature();

    //--------------------------------------------------------------------------
    // reduceCurrentLifePoints
    // reduces the creature's current life points by point amount in param
    // @param points The points to take away
    //
    void reduceCurrentLifePoints(int points);

    //--------------------------------------------------------------------------
    // restoreCreature
    // restores the creature to initial values after rebirth
    //
    void restoreCreature();

    //--------------------------------------------------------------------------
    // creatureIsDead
    // checks if the creature is dead, i.e. has no more life points
    // return bool true if it is dead
    //
    bool creatureIsDead();

    //--------------------------------------------------------------------------
    // Destructor
    //
    virtual ~CreatureCard();

  private:
    //--------------------------------------------------------------------------
    // Damage points this creature deals when attaching
    // const int (between 0-9)
    //
    const int damage_points_;

    //--------------------------------------------------------------------------
    // Life points of the creature at dealing of card
    // const int (between 0-9)
    //
    const int life_points_;

    //--------------------------------------------------------------------------
    // Life points this creature currently has
    // Int (between 0-9)
    // if current_life_points_ == 0, the creatures dies and goes to cementary
    //
    int current_life_points_;

    //--------------------------------------------------------------------------
    // Holds the information whether the creature has a shield or not
    // const bool
    //
    const bool shield_;

    //--------------------------------------------------------------------------
    // Holds the information whether creature will decrease foe's mana
    // upon attacking.
    // Only upon direct attach a non-shild creature will lose all mana
    // bool
    //
    bool mana_drain_;

    //--------------------------------------------------------------------------
    // Holds the information whether the creature is 'speedy'
    // meaning that it can attack already when the card is dealt
    // without waiting one round
    // bool
    //
    bool speedy_ = false;

    //--------------------------------------------------------------------------
    // Holds the information whether the creature is ready to attack
    // bool
    //
    bool ready_to_fight_;

    //--------------------------------------------------------------------------
    // Holds the information whether the creature has already attacked
    // bool
    //
    bool already_attacked_;
  };
}

#endif // CREATURECARD_H_INCLUDED
