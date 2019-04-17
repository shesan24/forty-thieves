/**
 * \file Stack.h
 * \author Shesan Bala
 * \brief Generic template module for a stack.
 */
#ifndef A3_STACK_H_
#define A3_STACK_H_

#include <vector>

/**
 *  \brief Class representing a stack.
 *  \details Generic stack template that allows for instantiations of the stack with different types.
 */
template <class T>
class Stack
{
  private:
    /// Stack
    std::vector<T> s;

  public:

    /**
     *  \brief Default constructor. 
     */
    Stack();

    /**
     *  \brief Constructs a new stack. 
     *  \param s List of elements added to stack.
     */
    Stack(std::vector<T> s);

    /**
     *  \brief Push element of same type onto the stack.
     *  \param t Element to be added to the stack.
     *  \return A new stack with the element added onto it.
     */
    Stack<T> push(T t);

    /**
     *  \brief Pop an element from the stack.
     *  \exception Throws out_of_range error when calling pop while stack is empty.
     *  \return A new stack with the last element removed from it.
     */
    Stack<T> pop();

    /**
     *  \brief Get the last element from the stack (top of the stack).
     *  \exception Throws out_of_range error when calling top while stack is empty.   
     *  \return The last element from the stack.
     */
    T top();

    /**
     *  \brief Get the size of the stack.
     *  \return The size of the stack.
     */
    unsigned int size();

    /**
     *  \brief Get the whole stack.
     *  \return A list of all the elements in the stack.
     */
    std::vector<T> toSeq();
};

#endif