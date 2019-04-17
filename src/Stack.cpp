#include "Stack.h"
#include "CardTypes.h"

#include <stdexcept>

template <class T>
Stack<T>::Stack(){}

template <class T>
Stack<T>::Stack(std::vector<T> s){
    this->s = s;
}

template <class T>
std::vector<T> Stack<T>::toSeq(){
    return this->s;
}

template <class T>
Stack<T> Stack<T>::push(T t){
    std::vector<T> temp(this->toSeq());
    temp.push_back(t);
    return Stack(temp);
}

template <class T>
Stack<T> Stack<T>::pop(){
    std::vector<T> temp(this->toSeq());
    if (temp.size() == 0){
        throw std::out_of_range("Stack is empty");
    }
    temp.pop_back();
    return Stack(temp);
}

template <class T>
T Stack<T>::top(){
    if (this->s.size() == 0){
        throw std::out_of_range("Stack is empty");
    }
    return this->s.back();
}

template <class T>
nat Stack<T>::size(){
    return this->s.size();
}

// Keep this at bottom
template class Stack<CardT>;