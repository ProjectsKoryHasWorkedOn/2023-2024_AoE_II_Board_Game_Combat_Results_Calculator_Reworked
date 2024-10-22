#pragma once
/** The libaries **/
#include "entity.h" // Using: Entity class
#include <iostream> // Using: cin, cout

/** The modifiers calculator class **/
class Database;

class modifiersCalculator {
protected:
  Database* m_database;

  // Behaviour: Store information about the entities
  Entity p1BattleParticipant;
  Entity p2BattleParticipant;
  Entity p1BattleAssistant;
  Entity p2BattleAssistant;

  // Behaviour: Store information about the player
  int  playerNumber;
  int  playerAge;
  int* playerTechnologies;
  int* playerEvents;

  // Behaviour: Store information about whether to return a BattleParticipant or
  // AssistingMonkParticipant
  int returnMode;

public:
  // Functions: The constructor and deconstructor
  explicit modifiersCalculator(Database* database);
  ~modifiersCalculator();

  // Function: Set the battle participants
  void setEntities(
    Entity& inputBattleParticipantP1,
    Entity& inputBattleParticipantP2,
    Entity& inputAssistingMonkParticipantP1,
    Entity& inputAssistingMonkParticipantP2);

  // Function: Set the additional values
  void setAdditionalValues(
    int& inputPlayerNumber,
    int& inputPlayerAge,
    int* inputTechnologies,
    int* inputEvents);

  // Function: Apply the effects of just the attack bonuses
  void applyRockPaperScissorsEffects();

  // Function: Apply the effects of just the technologies
  void applyTechnologyEffects();

  // Function: Apply the effects of just the events
  void applyEventCardEffects();

  // Function: Apply the effects of 1) attack bonuses, 2) technologies, 3) the
  // relevant event cards, and 4) quantity for player 1's battle participant
  Entity applyAllModifiers(int inputReturnMode);
};
