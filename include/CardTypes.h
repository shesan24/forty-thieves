/**
 * \file CardTypes.h
 * \author Shesan Bala
 * \brief Library exporting card types.
 */
#ifndef A3_CARD_TYPES_H_
#define A3_CARD_TYPES_H_

/**
 * \brief Describes the rank of a card.
 */
typedef unsigned short int RankT;

/**
 * \brief Describes a natural number.
 */
typedef unsigned int nat;

/**
 * \brief RankT for an Ace.
 */
#define ACE 1

/**
 * \brief RankT for an Jack.
 */
#define JACK 11

/**
 * \brief RankT for a Queen.
 */
#define QUEEN 12

/**
 * \brief RankT for a King.
 */
#define KING 13

/**
 * \brief Represents suits of the cards.
 */
enum SuitT
{
    Heart,
    Diamond,
    Club,
    Spade
};

/**
 * \brief Represents different set of stacks used in forty thieves.
 */
enum CategoryT
{
    Tableau,
    Foundation,
    Deck,
    Waste
};

/**
 * \brief Represents a card.
 */
struct CardT
{
    SuitT s;
    RankT r;
};

#endif