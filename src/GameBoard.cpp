#include "GameBoard.h"
#include <stdexcept>
#include <iostream>

BoardT::BoardT(){}

BoardT::BoardT(DckofCrds c){
    DckofCrds tmpDck;
    CardStackT tmpStck(tmpDck);
    CardStackT stckCrds(c);
    
    if(!two_decks(init_seq(10), init_seq(8), stckCrds, tmpDck)){
        throw std::invalid_argument("Two decks error");
    }
   
    this->T = tab_deck(c);
    
    this->F = init_seq(8);

    DckofCrds dck;    
    for(int i = 40; i < 104; i++){
        dck.push_back(c[i]);
    }
    CardStackT dckStck(dck);
    this->D = dckStck;

    DckofCrds wstDck;
    CardStackT wstStck(wstDck);
    this->W = wstStck;  
}

bool BoardT::is_valid_tab_mv(CategoryT c, unsigned int n0, unsigned int n1){
    if(c == Deck || c == Waste) return false;

    if(c == Tableau){
        if(n0 < 0 || n0 > 9 || n1 < 0 || n1 > 9)    throw std::out_of_range("Not a valid tableau position");

        CardStackT stack0 = this->T[n0];
        CardStackT stack1 = this->T[n1];
        
        if(stack0.size() == 0)  return false;

        if(stack1.size() == 0)  return true;
        
        if(stack0.top().s == stack1.top().s && stack0.top().r == stack1.top().r - 1)    return true;
    }

    if(c == Foundation){
        if(n0 < 0 || n0 > 9 || n1 < 0 || n1 > 7)    throw std::out_of_range("Not a valid foundation position");

        CardStackT stack0 = this->T[n0];
        CardStackT stack1 = this->F[n1];
        
        if(stack0.size() == 0)  return false;

        if(stack1.size() == 0){
            if(stack0.top().r == ACE) return true;
            else return false;
        } 
        
        if(stack0.top().s == stack1.top().s && stack0.top().r == stack1.top().r + 1)    return true;
    }

    return false;
}

bool BoardT::is_valid_waste_mv(CategoryT c, unsigned int n){
    if(c == Deck || c == Waste) return false;

    if(this->W.size() == 0) throw std::invalid_argument("Waste stack is empty");

    if(c == Tableau){
        if(n < 0 || n > 9)  throw std::out_of_range("Not a valid tableau position");

        CardStackT stack1 = this->T[n];        

        if(stack1.size() == 0)  return true;
        
        if(this->W.top().s == stack1.top().s && this->W.top().r == stack1.top().r - 1)  return true;
    }

    if(c == Foundation){
        if(n < 0 || n > 7) throw std::out_of_range("Not a valid foundation position");

        CardStackT stack1 = this->F[n];        

        if(stack1.size() == 0){
            if(this->W.top().r == ACE) return true;
            else return false;
        } 
        
        if(this->W.top().s == stack1.top().s && this->W.top().r == stack1.top().r + 1)  return true;
    }

    return false;
}

bool BoardT::is_valid_deck_mv(){
    return this->D.size() > 0;
}

void BoardT::tab_mv(CategoryT c, nat n0, nat n1){
    if(!is_valid_tab_mv(c, n0, n1)){
        throw std::invalid_argument("Move not possible");
    }

    if(c == Tableau){
        this->T[n1] = this->T[n1].push(this->T[n0].top());
        this->T[n0] = this->T[n0].pop();
    } else if(c == Foundation){
        this->F[n1] = this->F[n1].push(this->T[n0].top());
        this->T[n0] = this->T[n0].pop();
    }
}

void BoardT::waste_mv(CategoryT c, nat n){
    if(!is_valid_waste_mv(c, n)) throw std::invalid_argument("Move not possible");

    if(c == Tableau){
        this->T[n] = this->T[n].push(this->W.top());
        this->W = this->W.pop();
    }else if(c == Foundation){
        this->F[n] = this->F[n].push(this->W.top());
        this->W = this->W.pop();
    }
}

void BoardT::deck_mv(){
    if(!is_valid_deck_mv()) throw std::invalid_argument("Move not possible");
    this->W = this->W.push(this->D.top());
    this->D = this->D.pop();    
}

CardStackT BoardT::get_tab(nat i){    
    if(is_valid_pos(Tableau, i)) return this->T[i];
    throw std::out_of_range("Not a valid tableau position");
}

CardStackT BoardT::get_foundation(nat i){
    if(is_valid_pos(Foundation, i)) return this->F[i];
    throw std::out_of_range("Not a valid foundation position");
}

CardStackT BoardT::get_waste(){
    return this->W;
}

CardStackT BoardT::get_deck(){
    return this->D;
}

bool BoardT::valid_mv_exists(){
    //Checking for deck moves
    if(is_valid_deck_mv()) return true;

    //Checking for waste moves
    for(nat m = 0; m < 10; m++){
        if(is_valid_waste_mv(Tableau, m)) return true;
    }
    for(nat n = 0; n < 8; n++){
        if(is_valid_waste_mv(Foundation, n)) return true;
    }  
    
    //Checking for tableau moves
    for(nat i = 0; i < 10; i++){
        for(nat j = 0; j < 10; j++){
            if(is_valid_tab_mv(Tableau, i, j))  return true;
        }
        for(nat k = 0; k < 8; k++){
            if(is_valid_tab_mv(Foundation, i, k))   return true;
        }
    }

    return false;
}

bool BoardT::is_win_state(){    
    for(nat i = 0; i < 7; i++){
        if(this->F[i].size() > 0 && this->F[i].top().r == KING) continue;
        return false;
    }

    return true;
}



//Private Methods

bool BoardT::two_decks(std::vector<CardStackT> T, std::vector<CardStackT> F, CardStackT D, CardStackT W){
    unsigned int count = 0;
    
    for (RankT rank = ACE; rank <= KING; rank++) {
        for (unsigned int suit = 0; suit < 4; suit++) {
            CardT c1 = { static_cast<SuitT>(suit), rank };
            
            for(CardStackT m : T){
                std::vector<CardT> x = m.toSeq();
                for(CardT n : x){
                    if(n.s == c1.s && n.r == c1.r)  count++;
                }
            }

            for(CardStackT m : F){
                std::vector<CardT> x = m.toSeq();
                for(CardT n : x){
                    if(n.s == c1.s && n.r == c1.r)  count++;
                }
            }

            for(CardT n : D.toSeq()){
                if(n.s == c1.s && n.r == c1.r)  count++;
            }

            for(CardT n : W.toSeq()){
                if(n.s == c1.s && n.r == c1.r)  count++;                    
            }

            if(count != 2)  return false;

            count = 0;
        }
    }

    return true;
}

unsigned int BoardT::cnt_cards(std::vector<CardStackT> T, std::vector<CardStackT> F, CardStackT D, CardStackT W){
    unsigned int count = 0;
    
    for(CardStackT m : T){
        count += m.size();
    }
    
    for(CardStackT m : F){
        count += m.size();
    }
    
    count += D.size() + W.size();
    
    return count;
}

SeqCrdStckT BoardT::init_seq(nat n){
    SeqCrdStckT vect1;
    
    for(nat i = 0; i < n; i++){
        std::vector<CardT> d;
        CardStackT stack1(d);
        vect1.push_back(stack1);
    }
   
    return vect1;
}

SeqCrdStckT BoardT::tab_deck(DckofCrds c)
{
    SeqCrdStckT tmpCrdStck;
    
    for(int i = 0; i < 40; i+=4){
        DckofCrds tmpDck;
        
        tmpDck.push_back(c[i]);
        tmpDck.push_back(c[i+1]);
        tmpDck.push_back(c[i+2]);
        tmpDck.push_back(c[i+3]);
        
        CardStackT tmpDckStck(tmpDck);
        tmpCrdStck.push_back(tmpDckStck);
    }

    return tmpCrdStck;
}

bool BoardT::is_valid_pos(CategoryT c, nat n){
    if(c == Tableau && n >= 0 && n < 10) return true;
    else if(c == Foundation && n >= 0 && n < 8) return true;
    return false;
}