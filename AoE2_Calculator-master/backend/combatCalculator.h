#pragma once
/** The libaries **/
#include "entity.h" // Using: Entity class
#include "player.h"
#include <QDebug>
#include <functional>
#include <iostream> // Using: cin, cout

enum class ActivePlayer : int {
  None    = 0B00,
  Player1 = 0B01,
  Player2 = 0B10,
  Both    = 0B11
};

std::ostream& operator<<(std::ostream& os, ActivePlayer player);

QDebug operator<<(QDebug d, ActivePlayer player);

constexpr ActivePlayer operator|(ActivePlayer lhs, ActivePlayer rhs)
{
  return static_cast<ActivePlayer>(
    static_cast<int>(lhs) | static_cast<int>(rhs));
}

constexpr ActivePlayer operator&(ActivePlayer lhs, ActivePlayer rhs)
{
  return static_cast<ActivePlayer>(
    static_cast<int>(lhs) & static_cast<int>(rhs));
}

class CombatCalculatorCallbacks {
public:
  explicit CombatCalculatorCallbacks(
    std::function<void(Player, bool)> onPlayerEntityDeath);

  const std::function<void(Player, bool)>& getOnPlayerEntityDeath() const;

private:
  std::function<void(Player, bool)> m_onPlayerEntityDeath;
};

class CombatCalculatorState {
public:
  CombatCalculatorState();

public:
  /** Values attained sometime during the running of the combat rounds **/

  int startingBattleParticipantQuantityP1, startingBattleParticipantQuantityP2;

  int startingAssistantQuantityP1, startingAssistantQuantityP2;

  // Int: Store the die roll input
  int d6DieRoll;

  // Bool: Store whether or not one of the unit has died
  bool aDeathHasOccured;

  // String: Store whether or not an entity is retreating
  std::string isRetreating;

  std::string player1Name;
  std::string player2Name;

  /** Values atteined from the function setCombatParticipants() **/
  // Struct: Declare the entities participating in the battle
  Entity p1BattleParticipant;
  Entity p2BattleParticipant;
  Entity p1BattleAssistant;
  Entity p2BattleAssistant;

  // Integer: Store modifiers to the attack dealt
  int roundAttackModifiersP1;
  int roundAttackModifiersP2;

  /** Values attained from the function setAdditionalValues() **/
  // Integer: Store the remaining damage
  float remainingDamageP1;
  float remainingDamageP2;
};

/** The combat calculator class **/
#ifndef COMBAT_CALCULATOR_H
#define COMBAT_CALCULATOR_H
class combatCalculator {
  // Reference: I do not need to access this data outside of this superclass and
  // the subclasses
protected:
  CombatCalculatorState*     m_state;
  CombatCalculatorCallbacks* m_callbacks;

  int &startingBattleParticipantQuantityP1,
    &startingBattleParticipantQuantityP2;

  int &startingAssistantQuantityP1, &startingAssistantQuantityP2;

  int& d6DieRoll;

  bool& aDeathHasOccured;

  std::string& isRetreating;

  std::string& player1Name;
  std::string& player2Name;

  Entity& p1BattleParticipant;
  Entity& p2BattleParticipant;
  Entity& p1BattleAssistant;
  Entity& p2BattleAssistant;

  int& roundAttackModifiersP1;
  int& roundAttackModifiersP2;

  float& remainingDamageP1;
  float& remainingDamageP2;

public:
  // Functions: The constructor and deconstructor
  explicit combatCalculator(
    CombatCalculatorState*     state,
    CombatCalculatorCallbacks* callbacks);
  ~combatCalculator();

  /** Set the values functions **/
  // Function: Set the player names
  void setPlayerNames(std::string& inputP1Name, std::string& inputP2Name);

  // Function: Set the battle participants
  void setCombatParticipants(
    Entity& inputP1BattleParticipant,
    Entity& inputP2BattleParticipant,
    Entity& inputP1AssistingMonkBattleParticipant,
    Entity& inputP2AssistingMonkBattleParticipant,
    int&    inputRoundAttackModifiersP1,
    int&    inputRoundAttackModifiersP2);

  // Function: Pass the address of the remaining damage into the superclass
  // variable Reference: I tried storing the integer arrays but this is caused a
  // segmentation fault (core dump) that was hard to fix
  void setAdditionalValues(
    float& inputRemainingDamageP1,
    float& inputRemainingDamageP2);

  /** Dice rolling functions **/
  // Function: Generate d6 die input
  int generateD6DieInput();

  // Function: Check the randomness of the d6 dice roller
  void checkD6DiceSimulator();

  /** The remainder functions **/
  // Function: Calculate the non-rounded value (the remaining damage value)
  float calculateRemainingDamage(
    int roundedNumber,
    int inputAttackerDamage,
    int inputAttackerModifier,
    int inputDefenderHealth,
    int inputSetting);

  // Function: Check the remaining damage values for the effects of relentless
  // attack
  void checkRemainingDamage(int* inputP1Events, int* inputP2Events);

  // Function: Return the remaining damage
  float returnRemaningDamage(int inputPlayerNumber);

  /** Function: Output the entity information  **/
  void outputEntityInformation(std::string inputMessage);

  /** Battle ending functions **/
  // Function: Check if any of the entities have died before proceeding to the
  // next round of combat
  void checkIfDead();

  // Function: Check if the attacking ranged archer is retreating
  void checkIfRetreating(std::string roundType);

  /** Combat rounds functions **/
  // Function: Calculate the outcome of a battle
  virtual void roundOutcome(
    int          roundRunTimes,
    int*         inputP1Events,
    int*         inputP2Events,
    int*         inputP1Technologies,
    int*         inputP2Technologies,
    ActivePlayer activePlayer)
    = 0; // Abstract class with no implementation (overrided
         // by the subclasses)

  // Function: Make some final checks (after the end of the rounds of combat)
  void checkIfItCanBeHealed();

  void calculatingMonkRoundOutcomeForAnIndividualPlayer(
    // Both player names
    std::string& givenPlayerName,
    std::string& opposingPlayerName,
    // Both player battle participants
    Entity& givenPlayerBattleParticipant,
    Entity& givenPlayerBattleAssistant,
    Entity& opposingPlayerBattleParticipant,
    Entity& opposingPlayerBattleAssistant,
    // Both player events and technologies
    int* givenPlayerEvents,
    int* opposingPlayerEvents,
    int* givenPlayerTechnologies,
    // Given player monk related information
    bool&        givenPlayerHasAMonkPresent,
    bool&        givenPlayerMonkPowersActivated,
    std::string& givenPlayerIsHealingOrConverting,
    int&         givenPlayerConversionANDHealingRate,
    bool&        givenPlayerAssistingMonks,
    bool&        givenPlayerStandaloneMonks,
    int&         givenPlayerPointsGained,
    int&         givenPlayerEntitiesHealed);


  void calculatingArcherRoundOutcomeForAnIndividualPlayer(
    // Both player names
  std::string& givenPlayerName,
  std::string& opposingPlayerName,
  // Both player battle participants
  Entity& givenPlayerBattleParticipant,
  Entity& givenPlayerBattleAssistant,
  Entity& opposingPlayerBattleParticipant,
  Entity& opposingPlayerBattleAssistant,
  // Both player events and technologies
  int* givenPlayerEvents,
  int* opposingPlayerEvents,
  int* givenPlayerTechnologies,
  // Shared stuff
  const int roundDownBasedOnMultiplesOfThisNumber,
  // Round attack modifiers
  int givenPlayerRoundAttackModifiers,
  float& givenPlayerRemainingDamage,
  // Given player archer related information
  bool&        givenPlayerHasAArcherActivated,
  bool&        givenPlayerIsFightingBuilding,
    bool&        givenPlayerIsFightingUnit,
    bool&        givenPlayerRangedUnitCanAttackOpposingPlayerBuilding,
    bool&        givenPlayerRangedUnitCanAttackOpposingPlayerCavalry,
  bool&       opposingPlayerTakesNoDamageDueToShotsInTheBack,
  float& givenPlayerPointsGained,
  int&         givenPlayerRangedDamageDealt,
  int&         opposingPlayerEntityDeaths,
  int&         opposingPlayerBuildingDamage,
  int& opposingPlayerDamageDie,
  int& opposingPlayerStartingQuantity,
  int& opposingPlayerEndingQuantity);






  void outputtingMonkRoundOutcomeForAnIndividualPlayer(

    // The active player
    ActivePlayer activePlayer,
    std::string& givenPlayerName,
    int&         givenPlayerPointsAwarded,
    bool&        givenPlayerMonkPowersActivated,
    Entity&      givenPlayerBattleParticipant,
    Entity&      givenPlayerBattleAssistant);

  /** Return information functions **/
  // Function: Return the modified battle participants based on the input player
  // number
  Entity returnModifiedBattleParticipants(int inputPlayerNumber);
};
#endif // COMBAT_CALCULATOR_H

#ifndef COMBAT_CALCULATOR_MONK_ROUNDS_H
#define COMBAT_CALCULATOR_MONK_ROUNDS_H
class monkRounds : public combatCalculator {
public:
  using combatCalculator::combatCalculator;

  // Function: Calculate the outcome of a monk battle
  void roundOutcome(
    int          roundRunTimes,
    int*         inputP1Events,
    int*         inputP2Events,
    int*         inputP1Technologies,
    int*         inputP2Technologies,
    ActivePlayer activePlayer) override;
};
#endif // COMBAT_CALCULATOR_MONK_ROUNDS_H

#ifndef COMBAT_CALCULATOR_ARCHER_ROUNDS_H
#define COMBAT_CALCULATOR_ARCHER_ROUNDS_H
class archerRounds : public combatCalculator {
public:
  using combatCalculator::combatCalculator;

  // Function: Calculate the outcome of a ranged battle
  void roundOutcome(
    int          roundRunTimes,
    int*         inputP1Events,
    int*         inputP2Events,
    int*         inputP1Technologies,
    int*         inputP2Technologies,
    ActivePlayer activePlayer) override;
};
#endif // COMBAT_CALCULATOR_ARCHER_ROUNDS_H

#ifndef COMBAT_CALCULATOR_BOMBARDMENT_ROUNDS_H
#define COMBAT_CALCULATOR_BOMBARDMENT_ROUNDS_H
class bombardmentRounds : public combatCalculator {
public:
  using combatCalculator::combatCalculator;

  // Function: Calculate the outcome of a bombardment battle
  void roundOutcome(
    int          roundRunTimes,
    int*         inputP1Events,
    int*         inputP2Events,
    int*         inputP1Technologies,
    int*         inputP2Technologies,
    ActivePlayer activePlayer) override;
};
#endif // COMBAT_CALCULATOR_BOMBARDMENT_ROUNDS_H

#ifndef COMBAT_CALCULATOR_STANDARD_ROUNDS_H
#define COMBAT_CALCULATOR_STANDARD_ROUNDS_H
class standardRounds : public combatCalculator {
public:
  using combatCalculator::combatCalculator;

  // Function: Calculate the outcome of a standard battle
  void roundOutcome(
    int          roundRunTimes,
    int*         inputP1Events,
    int*         inputP2Events,
    int*         inputP1Technologies,
    int*         inputP2Technologies,
    ActivePlayer activePlayer) override;
};
#endif // COMBAT_CALCULATOR_STANDARD_ROUNDS_H

class FightMonksRounds : public combatCalculator {
public:
  enum class Kind { Ranged, Melee };

  FightMonksRounds(
    CombatCalculatorState*     state,
    CombatCalculatorCallbacks* callbacks,
    Kind                       kind);

  void roundOutcome(
    int          roundRunTimes,
    int*         inputP1Events,
    int*         inputP2Events,
    int*         inputP1Technologies,
    int*         inputP2Technologies,
    ActivePlayer activePlayer) override;

  Kind getKind() const;

private:
  int getDamageForKind(const Entity* entity) const;

  Kind m_kind;
};
