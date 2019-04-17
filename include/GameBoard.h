/**
 * \file GameBoard.h
 * \author Shesan Bala
 * \brief Template module for a gameboard.
 */
#ifndef A3_GAME_BOARD_H_
#define A3_GAME_BOARD_H_

#include "CardTypes.h"
#include "CardStack.h"

#include <vector>

/**
 * \brief Describes a list of stack of cards.
 */
typedef std::vector<CardStackT> SeqCrdStckT;

/**
 * \brief Describes a list of cards.
 */
typedef std::vector<CardT> DckofCrds;


/**
 *  \brief Class representing a gameboard.
 *  \details Gameboard template that allows for instantiations of the gameboard.
 */
class BoardT 
{  
  private:
    /// Tableaus
    SeqCrdStckT T;
    /// Foundations
    SeqCrdStckT F;
    /// Deck
    CardStackT D;
    /// Waste pile
    CardStackT W;
    
    /**
     *  \brief Checks if there are two decks of cards.
     *  \param T Tableau stacks.
     *  \param F Foundation stacks.
     *  \param D Deck stack.
     *  \param W Waste stack.
     *  \return True or False based on whether there are two decks of cards.
     */
    bool two_decks(SeqCrdStckT T, SeqCrdStckT F, CardStackT D, CardStackT W);

    /**
     *  \brief Count the total number of cards currently in the game.
     *  \param T Tableau stacks.
     *  \param F Foundation stacks.
     *  \param D Deck stack.
     *  \param W Waste stack.
     *  \return Total number of cards in the game.
     */    
    unsigned int cnt_cards(SeqCrdStckT T, SeqCrdStckT F, CardStackT D, CardStackT W);  
    
    /**
     *  \brief Creates a sequence of given length consisting of empty stack of cards.
     *  \param n Length of the sequence.
     *  \return Sequence of empty stack of cards.
     */     
    SeqCrdStckT init_seq(nat n);

    /**
     *  \brief Creates a sequence of stack of cards for the tableau.
     *  \param c Deck of cards.
     *  \return Sequence of stack of cards for the tableau.
     */    
    SeqCrdStckT tab_deck(DckofCrds c);

    /**
     *  \brief Checks if the index of a stack of a given category of stack is a valid index.
     *  \param c Category of the stack.
     *  \param n Index of the stack.
     *  \return True or False based on whether the index is valid.
     */  
    bool is_valid_pos(CategoryT c, nat n);
  
  public:
    /**
     *  \brief Default constructor. 
     */    
    BoardT();

    /**
     *  \brief Constructs a gameboard.
     *  \exception Throws invalid_argument error if there are not two of each card. 
     *  \param c Startting 2 decks of cards.
     */
    BoardT(DckofCrds c);

    /**
     *  \brief Checks if a tab move is valid.
     *  \param c Category of the stack moving to.
     *  \param n0 Index of the current stack.
     *  \param n1 Index of the stack moving to.
     *  \exception Throws out_of_range error if the index of a stack of a category looking for is not in range.
     *  \return True or False based on whether the move is valid.
     */
    bool is_valid_tab_mv(CategoryT c, nat n0, nat n1);
    
    /**
     *  \brief Checks if a waste stack move is valid.
     *  \param c Category of the stack moving to.
     *  \param n Index of the stack moving to.
     *  \exception Throws invalid_argument error when checking for a waste move when the waste stack is empty.
     *  \exception Throws out_of_range error if the index of a stack of a category looking for is not in range.
     *  \return True or False based on whether the move is valid.
     */
    bool is_valid_waste_mv(CategoryT c, nat n);
    
    /**
     *  \brief Checks if the deck move is valid.
     *  \return True or False based on whether the move is valid.
     */
    bool is_valid_deck_mv();

    /**
     *  \brief Performs the tab move if valid.
     *  \param c Category of the stack moving to.
     *  \param n0 Index of the current stack.
     *  \param n1 Index of the stack moving to.
     *  \exception Throws invalid_argument error when tab move is not valid.
     */
    void tab_mv(CategoryT c, nat n0, nat n1);

    /**
     *  \brief Performs the waste stack move if valid.
     *  \param c Category of the stack moving to.
     *  \param n Index of the stack moving to.
     *  \exception Throws invalid_argument error when waste move is not valid.
     */
    void waste_mv(CategoryT c, nat n);

    /**
     *  \brief Performs the deck move if valid.
     *  \exception Throws invalid_argument error when deck move is not valid.
     */
    void deck_mv();

    /**
     *  \brief Gets a tableau stack from valid index.
     *  \param i Index of the tableau stack looking for.
     *  \exception Throws out_of_range error if the index of a stack of a category looking for is not in range.
     *  \return Stack of cards from the given index.
     */    
    CardStackT get_tab(nat i);

    /**
     *  \brief Gets a foundation stack from valid index.
     *  \param i Index of the foundation stack looking for.
     *  \exception Throws out_of_range error if the index of a stack of a category looking for is not in range.
     *  \return Stack of cards from the given index.
     */    
    CardStackT get_foundation(nat i);

    /**
     *  \brief Gets the waste pile stack.
     *  \return Stack of cards from waste pile.
     */      
    CardStackT get_deck();

    /**
     *  \brief Gets the deck stack.
     *  \return Stack of cards from deck pile.
     */     
    CardStackT get_waste();

    /**
     *  \brief Calculates whether any valid moves left.
     *  \return True or False based on whether any valid moves are available.
     */
    bool valid_mv_exists();

    /**
     *  \brief Calculates whether game is won.
     *  \return True or False based on whether game is at the win state.
     */
    bool is_win_state();    
};

#endif