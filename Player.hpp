//------------------------------------------------------------------------------
// Player.h
//
// Group: Group 6, study assistant: Oberhuber M.
//
// Authors: Sarah Lang (01110645)
// Created: 25.03.2020
// Last change: 15.06.2020
//------------------------------------------------------------------------------
//

#ifndef PLAYER_H
#define PLAYER_H

//#include <iostream>
//#include <stdexcept>
//#include <string>
//#include "Card.hpp"
#include <vector>
#include <array>

#include "CreatureCard.hpp"
#include "SpellCard.hpp"
#include "Interface.hpp"

namespace Oop
{
  //----------------------------------------------------------------------------
  // Player Class
  //
  // Class for player 1 and player 2
  //
  class Card;
  class SpellCard;
  class CreatureCard;
  class Interface;


  class Player
  {
  public:
    //--------------------------------------------------------------------------
    // constants
    //
    static const int ZERO_POINTS = 0;
    static const int ZERO = 0;
    static const int OPPONENT_PLACE = 0;
    static const int ONE_POINT = 1;

    static const int MINIMUM_ARRAY_POSITION = 1;
    static const int MAX_ARRAY_POSITION = 7;
    static const int SIZE_GAME_FIELD = 7;
    static const int START_LIFE_POINTS = 30;
    static const int MAX_MANA_POINTS = 15;

    static const int HEALER_MANA_COST = 5;
    static const int RELIEF_MANA_COST = 3;
    static const int REBIRTH_MANA_COST = 5;
    static const int DRACULA_MANA_COST = 2;
    static const int DRACULA_BOOST = 2;

    //--------------------------------------------------------------------------
    // enums
    //
    enum CommandError { SUCCESS, WRONG_PARAMETER, EXECUTION_IMPOSSIBLE,
                        NOT_ENOUGH_MANA, REBIRTH_UNSUCCESSFUL, SHIELD,
                        CHECK_IF_GAME_ENDS, CHECK_IF_PLAYER_LOSES };

    //--------------------------------------------------------------------------
    // constructors
    //
    Player(std::string name);
    Player();

    //--------------------------------------------------------------------------
    // Copy Constructor
    // Makes a copy of another Player Object.
    // @param original Original to copy.
    //
    Player(const Player& original) = delete;
    // deleted because it would be a shallow copy with the card pointers

    //--------------------------------------------------------------------------
    // Assignment Operator
    // Used to assign one Player to another
    // @param original Original with values to copy.
    //
    Player &operator=(Player const& original) = delete;

    //--------------------------------------------------------------------------
    // getter methods
    //
    std::string getName() const;
    int getLifePoints() const;
    int getManaPoints() const;
    bool graveyardIsEmpty() const;
    std::vector<Oop::Card*> getHandCards() const ;
    int getHandSize() const;
    Oop::CreatureCard* const* getGameField() const;
    int getPickUpStackSize() const;

    //--------------------------------------------------------------------------
    // setter methods
    //
    void setPlayerName(std::string name);
    void setPickUpStack(std::vector<Card*> pick_up_stack);

    //--------------------------------------------------------------------------
    // methods
    //

    //--------------------------------------------------------------------------
    // addLifePoints
    // Function to add to life_points_
    // @param The amount
    //
    void addLifePoints(int points);

    //--------------------------------------------------------------------------
    // reduceLifePoints
    // Function to reduce life_points_
    // @param The amount
    //
    void reduceLifePoints(int points);

    //--------------------------------------------------------------------------
    // addMana
    // Function to add to mana_
    // @param The amount
    //
    void addMana(int mana);

    //--------------------------------------------------------------------------
    // reduceMana
    // Function to reduce mana_
    // @param The amount
    //
    void reduceMana(int mana);

    //--------------------------------------------------------------------------
    // drainMana
    // Function to reduce mana_ to zero
    //
    void drainMana();

    //--------------------------------------------------------------------------
    // pickUpCardFromPickUpStack
    // Function to get the last card from pick_up_stack
    // and remove from pick_up_stack
    // @param current_player The enum value on who the current player is
    // @return Card the last card picked up from the pick_up_stack
    //
    void pickUpCardFromPickUpStack();

    //--------------------------------------------------------------------------
    // freePlayersObjects
    // Function for memory clean-up of card vectors
    // will delete all cards the vector points to
    // will not delete the vector itself
    //
    void freePlayersObjects();


    //--------------------------------------------------------------------------
    // setCard
    // will perform the set command
    // @param position_hand The position on the hand to be taken from
    // @param position_gamefield The game_field_ position to be set to
    // @return the command error status
    //
    CommandError setCard(int position_hand, int position_gamefield);

    //--------------------------------------------------------------------------
    // runAttack
    // will perform the attack command
    // @param position_opponent The game_field_ position of the opponent
    //        to be attacked
    // @param position_self The position on the game_field indicating a
    //        creature to be chosen for attack
    // @param opponent The opponent
    // @return the command error status
    //
    CommandError runAttack(size_t position_opponent, size_t position_self, \
                           Player* opponent);

    //--------------------------------------------------------------------------
    // removeHandCardAfterUse
    // will remove a hand card after it was played
    // leaving the vector in correct order with no resulting gaps
    // @param the_card_position the index to remove from
    //
    void removeHandCardAfterUse(int the_card_position);

    //--------------------------------------------------------------------------
    // resetCardStatusAtNewTurn
    // resets the card status for the game field, at start of turn
    // i.e. ready_to_fight_ to true
    //
    void resetCardStatusAtNewTurn();

    //--------------------------------------------------------------------------
    // resetAlreadyAttackedValues
    // resets the card status for the game field, concerning already attacked
    // i.e. already_attacked_ are set to false
    //
    void resetAlreadyAttackedValues();

    //--------------------------------------------------------------------------
    // resetShieldCreauresAfterTurn
    // resets the shield creatures card status for the game field,
    // i.e. ready_to_fight_ to true if the creature has a shield
    //
    void resetShieldCreauresAfterTurn();

    //--------------------------------------------------------------------------
    // castRelief
    // casts a relief card
    // picking up 3 cards from pickup stack
    //
    void castRelief();

    //--------------------------------------------------------------------------
    // castHealer
    // casts a healer card
    // giving all gamefield creatures a boost of one life point
    //
    void castHealer();

    //--------------------------------------------------------------------------
    // castRebirth
    // casts a rebirth card
    // trying to revive the last creature from the graveyard_
    // @return bool on success or failure
    //
    bool castRebirth();

    //--------------------------------------------------------------------------
    // playCast
    // will perform the cast command, checking which spell it is and its cost
    // @param spell_card The spell card to be played
    // @param card_position The position of the card used
    // @param opponent The opponent
    // @return the command error status
    //
    CommandError playCast(SpellCard::SpellType spell_card, \
                          size_t card_position, Player* opponent);

    //--------------------------------------------------------------------------
    // playSacrifice
    // will perform the sacrifice command,
    // boosting player's life points by one and deleting the card if it was
    // a spell card or place on graveyard_ if it was a creature card
    // @param the_hand_position The position on the hand
    // @return the command error status
    //
    CommandError playSacrifice(size_t the_hand_position);

    //--------------------------------------------------------------------------
    // moveCreatureToGraveyard
    // moves a creature to the graveyard
    // will remove a game_field_ card
    // resetting its spot to nullptr
    // @param the_field_position The position on the field
    //
    void moveCreatureToGraveyard(size_t the_field_position);

    //--------------------------------------------------------------------------
    //destructor
    //
    virtual ~Player()
    {
      // intentionally left blank
    } ;

  private:
    //--------------------------------------------------------------------------
    // name of the player
    // string
    //
    std::string name_;

    //--------------------------------------------------------------------------
    // life points of the player - at game start: 30 points each player
    // int (no upper limit / if <1 point -> game ends)
    //
    int life_points_;

    //--------------------------------------------------------------------------
    // mana points of the player - at game start 0 points each player
    // int (between 0 - 15)
    //
    int mana_points_;

    //--------------------------------------------------------------------------
    // pick up stack
    // const vector Card*
    //
    std::vector<Card*> pick_up_stack_;

    //--------------------------------------------------------------------------
    // number of cards at hand stack
    // vector Card* (between 0 - 7)
    //
    std::vector<Card*> hand_;

    //--------------------------------------------------------------------------
    // creature graveyard of each player
    // vector
    //
    std::vector<CreatureCard*> graveyard_;

    //--------------------------------------------------------------------------
    // Game Field
    // pointer
    //
    std::array<CreatureCard*, Player::SIZE_GAME_FIELD> game_field_ = {};
  };

} // namespace Oop
#endif
