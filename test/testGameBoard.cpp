#include "catch.h"
#include "CardTypes.h"
#include "CardStack.h"
#include "GameBoard.h"

#include <stdexcept>
#include <iostream>

//Helper functions for testing
void swap_cards(CardT &crd1, CardT &crd2){
    CardT tmpCrd = crd1;
    crd1 = crd2;
    crd2 = tmpCrd;
}

void print_card_stack(CardStackT m){
    std::cout << "------" << "\n";
    std::vector<CardT> x = m.toSeq();
    for(CardT n : x){
        std::cout << n.s << n.r << "\n";
    }
}

// print_card_stack(tmpBoard.get_tab(0));
// print_card_stack(tmpBoard.get_tab(1));
// print_card_stack(tmpBoard.get_tab(2));
// print_card_stack(tmpBoard.get_foundation(0));

//GameBoard tests
TEST_CASE("tests for GameBoard" , "[BoardT]"){

    //Setup Before Every Test
    DckofCrds fullDck;
    for(nat suit = 0; suit < 4; suit++){
        for(RankT rank = ACE; rank <= KING; rank++){        
            CardT tmpCrd = { static_cast<SuitT>(suit), rank };
            fullDck.push_back(tmpCrd);
            fullDck.push_back(tmpCrd);
        }
    }

    SECTION("Testing constructor"){ 
        //Setup for testing        
        BoardT tmpBoard(fullDck);              
        
        nat wasteSize = 0;
        nat deckSize = 64;
        bool foundationEmpty = true;
        bool tableauFull = true;
        
        for(nat i = 0; i < 7; i++){
            if(tmpBoard.get_foundation(i).size() != 0){
                foundationEmpty = false;
            }
        }
        
        for(nat i = 0; i < 9; i++){
            if(tmpBoard.get_tab(i).size() != 4){
                foundationEmpty = false;
            }
        }

        //Testing normal cases       
        REQUIRE((
                tmpBoard.get_waste().size() == wasteSize
            &&  tmpBoard.get_deck().size() == deckSize
            &&  foundationEmpty
            &&  tableauFull
        ));

        //Testing for errors
        CardT tmpCrd = { static_cast<SuitT>(1), 1 };
        fullDck[0] =  tmpCrd;               
        
        REQUIRE_THROWS_AS(BoardT(fullDck), std::invalid_argument);
    }   

    SECTION("Testing is_valid_tab_mv"){ 
        //Setup for testing
        swap_cards(fullDck[4], fullDck[6]);
        swap_cards(fullDck[9], fullDck[11]);
        
        BoardT tmpBoard(fullDck); 
        
        tmpBoard.tab_mv(Tableau, 1, 2);
        tmpBoard.tab_mv(Tableau, 1, 2);
        tmpBoard.tab_mv(Tableau, 0, 1);
        tmpBoard.tab_mv(Tableau, 0, 2);
        tmpBoard.tab_mv(Tableau, 0, 2);  

        //Testing normal cases       
        REQUIRE((
                !tmpBoard.is_valid_tab_mv(Deck, 0, 0) 
            &&  !tmpBoard.is_valid_tab_mv(Waste, 0, 0) 
            &&  !tmpBoard.is_valid_tab_mv(Tableau, 0, 2) 
            &&  tmpBoard.is_valid_tab_mv(Tableau, 0, 1) 
        ));
        
        tmpBoard.tab_mv(Foundation, 0, 1);
        
        REQUIRE((
                !tmpBoard.is_valid_tab_mv(Tableau, 0, 1)  
            &&  tmpBoard.is_valid_tab_mv(Tableau, 1, 0)
            &&  !tmpBoard.is_valid_tab_mv(Foundation, 0, 0) 
            &&  !tmpBoard.is_valid_tab_mv(Foundation, 1, 0)
            &&  !tmpBoard.is_valid_tab_mv(Foundation, 2, 1)
            &&  tmpBoard.is_valid_tab_mv(Foundation, 1, 1)
        ));

        //Testing for errors
        REQUIRE_THROWS_AS(tmpBoard.is_valid_tab_mv(Tableau, 0, 10), std::out_of_range);
        REQUIRE_THROWS_AS(tmpBoard.is_valid_tab_mv(Foundation, 0, 8), std::out_of_range);       
    }
    
    SECTION("Testing is_valid_waste_mv"){
        //Setup for testing
        swap_cards(fullDck[4], fullDck[6]);
        swap_cards(fullDck[9], fullDck[11]);
        swap_cards(fullDck[101], fullDck[103]);
        swap_cards(fullDck[79], fullDck[100]);
        swap_cards(fullDck[81], fullDck[101]);        
        
        BoardT tmpBoard(fullDck); 
        
        tmpBoard.tab_mv(Tableau, 1, 2);
        tmpBoard.tab_mv(Tableau, 1, 2);
        tmpBoard.tab_mv(Tableau, 0, 1);
        tmpBoard.tab_mv(Tableau, 0, 2);
        tmpBoard.tab_mv(Tableau, 0, 2); 
        tmpBoard.tab_mv(Tableau, 0, 1);

        //Testing for errors
        REQUIRE_THROWS_AS(tmpBoard.is_valid_waste_mv(Tableau, 0), std::invalid_argument);
        
        tmpBoard.deck_mv();
        tmpBoard.deck_mv();
        
        REQUIRE_THROWS_AS(tmpBoard.is_valid_waste_mv(Tableau, -1), std::out_of_range); 
        REQUIRE_THROWS_AS(tmpBoard.is_valid_waste_mv(Foundation, -1), std::out_of_range); 
        
        //Testing normal cases
        REQUIRE((
                !tmpBoard.is_valid_waste_mv(Tableau, 1)  
            &&  tmpBoard.is_valid_waste_mv(Tableau, 0)            
        ));
        
        tmpBoard.waste_mv(Tableau, 0);
        
        REQUIRE((
                !tmpBoard.is_valid_waste_mv(Foundation, 0)  
            &&  tmpBoard.is_valid_waste_mv(Tableau, 0)            
        ));
        
        tmpBoard.waste_mv(Tableau, 0);
        tmpBoard.deck_mv();
        tmpBoard.deck_mv();
        
        REQUIRE(tmpBoard.is_valid_waste_mv(Foundation, 0));         
        
        tmpBoard.waste_mv(Foundation, 0);
        
        REQUIRE(tmpBoard.is_valid_waste_mv(Foundation, 0)); 
    }

    SECTION("Testing is_valid_deck_mv"){
        //Setup for testing
        BoardT tmpBoard(fullDck);

        //Testing normal cases 
        REQUIRE(tmpBoard.is_valid_deck_mv());
        
        for(int i = 0; i < 64; i++){
            tmpBoard.deck_mv();
        }  

        REQUIRE(!tmpBoard.is_valid_deck_mv());        
    }

    SECTION("Testing deck_mv"){
        //Setup for testing
        BoardT tmpBoard(fullDck); 
        
        CardT crd1 = { static_cast<SuitT>(3), 13 };        
        tmpBoard.deck_mv();        
        CardT crd2 = tmpBoard.get_waste().top();
        
        //Testing normal cases
        REQUIRE((
                crd1.s == crd2.s
            &&  crd1.r == crd2.r
        ));

        for(int i = 1; i < 64; i++){
            tmpBoard.deck_mv();
        }     

        //Testing for errors
        REQUIRE_THROWS_AS(tmpBoard.deck_mv(), std::invalid_argument);         
    }

    SECTION("Testing tab_mv"){ 
        //Setup for testing
        swap_cards(fullDck[4], fullDck[6]);
        swap_cards(fullDck[9], fullDck[11]);
        
        BoardT tmpBoard(fullDck); 
        
        tmpBoard.tab_mv(Tableau, 1, 2);
        tmpBoard.tab_mv(Tableau, 1, 2);
        tmpBoard.tab_mv(Tableau, 0, 1);
        tmpBoard.tab_mv(Tableau, 0, 2);
        tmpBoard.tab_mv(Tableau, 0, 2);
        
        //Temporary cards used to verify tab moves
        CardT crd1 = { static_cast<SuitT>(0), 1 };  
        CardT crd2 = { static_cast<SuitT>(0), 2 }; 
        CardT tmpCrd;     
        
        //Start of testing

        // Moving card from tableau to tableau with cards but unacceptable move
        REQUIRE_THROWS_AS(tmpBoard.tab_mv(Tableau, 0, 2), std::invalid_argument);
        
        // Moving card from tableau to tableau with cards and acceptable move
        tmpBoard.tab_mv(Tableau, 0, 1);
        tmpCrd = tmpBoard.get_tab(1).top();
        REQUIRE((
                tmpCrd.s == crd1.s
            &&  tmpCrd.r == crd1.r
        ));

        // Moving card from empty tableau to another tableau
        REQUIRE_THROWS_AS(tmpBoard.tab_mv(Tableau, 0, 1), std::invalid_argument);

        // Moving card from empty tableau to foundation
        REQUIRE_THROWS_AS(tmpBoard.tab_mv(Foundation, 0, 0), std::invalid_argument);

        // Moving card from tableau to empty tableau 
        tmpBoard.tab_mv(Tableau, 1, 0);
        tmpCrd = tmpBoard.get_tab(0).top();
        REQUIRE((
                tmpCrd.s == crd1.s
            &&  tmpCrd.r == crd1.r
        ));

        // Moving card from tableau to empty foundation but unacceptable move
        REQUIRE_THROWS_AS(tmpBoard.tab_mv(Foundation, 1, 0), std::invalid_argument); 

        // Moving card from tableau to empty foundation and acceptable move
        tmpBoard.tab_mv(Foundation, 2, 0);
        tmpCrd = tmpBoard.get_foundation(0).top();
        REQUIRE((
                tmpCrd.s == crd1.s
            &&  tmpCrd.r == crd1.r
        ));

         // Moving card from tableau to foundation with cards but unacceptable move
        REQUIRE_THROWS_AS(tmpBoard.tab_mv(Foundation, 0, 0), std::invalid_argument); 

        // Moving card from tableau to foundation with cards and acceptable move
        tmpBoard.tab_mv(Foundation, 2, 0);
        tmpCrd = tmpBoard.get_foundation(0).top();
        REQUIRE((
                tmpCrd.s == crd2.s
            &&  tmpCrd.r == crd2.r
        ));          
    }

    SECTION("Testing waste_mv"){
        //Setup for testing
        swap_cards(fullDck[4], fullDck[6]);
        swap_cards(fullDck[9], fullDck[11]);
        swap_cards(fullDck[101], fullDck[103]);
        swap_cards(fullDck[79], fullDck[100]);
        swap_cards(fullDck[81], fullDck[101]);        
        
        BoardT tmpBoard(fullDck); 
        
        tmpBoard.tab_mv(Tableau, 1, 2);
        tmpBoard.tab_mv(Tableau, 1, 2);
        tmpBoard.tab_mv(Tableau, 0, 1);
        tmpBoard.tab_mv(Tableau, 0, 2);
        tmpBoard.tab_mv(Tableau, 0, 2); 
        tmpBoard.tab_mv(Tableau, 0, 1);

        // Temporary cards used to verify tab moves
        CardT crd1 = { static_cast<SuitT>(3), 1 };  
        CardT crd2 = { static_cast<SuitT>(3), 2 }; 
        CardT crd3 = { static_cast<SuitT>(3), 12 };  
        CardT crd4 = { static_cast<SuitT>(3), 13 }; 
        CardT tmpCrd;
        
        //Start of testing

        // Moving card from empty waste stack to tableau
        REQUIRE_THROWS_AS(tmpBoard.waste_mv(Tableau, 0), std::invalid_argument);
        
        // Moving card from empty waste stack to foundation
        REQUIRE_THROWS_AS(tmpBoard.waste_mv(Foundation, 0), std::invalid_argument);

        tmpBoard.deck_mv();
        tmpBoard.deck_mv();
        
        // Moving card from waste stack to empty tableau
        tmpBoard.waste_mv(Tableau, 0);
        tmpCrd = tmpBoard.get_tab(0).top();
        REQUIRE((
                tmpCrd.s == crd4.s
            &&  tmpCrd.r == crd4.r
        ));
        
        // Moving card from waste stack to tableau with cards but unacceptable move
        REQUIRE_THROWS_AS(tmpBoard.waste_mv(Tableau, 1), std::invalid_argument); 
        
        // Moving card from waste stack to tableau with cards and acceptable move
        tmpBoard.waste_mv(Tableau, 0);
        tmpCrd = tmpBoard.get_tab(0).top();
        REQUIRE((
                tmpCrd.s == crd3.s
            &&  tmpCrd.r == crd3.r
        ));
        
        tmpBoard.deck_mv();
        tmpBoard.deck_mv();

        // Moving card from waste stack to empty foundation and acceptable move
        tmpBoard.waste_mv(Foundation, 0);
        tmpCrd = tmpBoard.get_foundation(0).top();
        REQUIRE((
                tmpCrd.s == crd1.s
            &&  tmpCrd.r == crd1.r
        ));

        // Moving card from waste stack to empty foundation but unacceptable move
        REQUIRE_THROWS_AS(tmpBoard.waste_mv(Foundation, 1), std::invalid_argument); 

        // Moving card from waste stack to foundation with cards and acceptable move
        tmpBoard.waste_mv(Foundation, 0);
        tmpCrd = tmpBoard.get_foundation(0).top();
        REQUIRE((
                tmpCrd.s == crd2.s
            &&  tmpCrd.r == crd2.r
        ));

        tmpBoard.deck_mv();

        // Moving card from waste stack to foundation with cards but unacceptable move
        REQUIRE_THROWS_AS(tmpBoard.waste_mv(Foundation, 0), std::invalid_argument); 
    }

    SECTION("Testing get_tab"){
        //Setup for testing
        BoardT tmpBoard(fullDck);         

        //Testing normal cases       
        CardT crd1 = { static_cast<SuitT>(0), 2};
        CardT tmpCrd = tmpBoard.get_tab(0).top();
        REQUIRE((
                tmpCrd.s == crd1.s
            &&  tmpCrd.r == crd1.r
        ));

        //Testing for errors
        REQUIRE_THROWS_AS(tmpBoard.get_tab(10), std::out_of_range);        
    }

    SECTION("Testing get_foundation"){
        //Setup for testing
        BoardT tmpBoard(fullDck);         

        //Testing normal cases   
        REQUIRE(tmpBoard.get_foundation(0).size() == 0);
        
        //Testing for errors
        REQUIRE_THROWS_AS(tmpBoard.get_foundation(8), std::out_of_range);      
    }

    SECTION("Testing get_deck"){
        //Setup for testing
        BoardT tmpBoard(fullDck);        
        
        //Testing normal cases       
        CardT crd1 = { static_cast<SuitT>(3), 13};
        CardT tmpCrd = tmpBoard.get_deck().top();
        REQUIRE((
                tmpCrd.s == crd1.s
            &&  tmpCrd.r == crd1.r
        ));       
    }

    SECTION("Testing get_waste"){
        //Setup for testing
        BoardT tmpBoard(fullDck);       
        tmpBoard.deck_mv();
    
        //Testing normal cases       
        CardT crd1 = { static_cast<SuitT>(3), 13};
        CardT tmpCrd = tmpBoard.get_waste().top();
        REQUIRE((
                tmpCrd.s == crd1.s
            &&  tmpCrd.r == crd1.r
        ));       
    }    

    SECTION("Testing is_win_state"){
        //Setup for testing        
        DckofCrds tmpDck;
        for(RankT rank = ACE; rank <= KING; rank++){
            for(nat suit = 0; suit < 4; suit++){        
                CardT tmpCrd = { static_cast<SuitT>(suit), rank };
                tmpDck.push_back(tmpCrd);
                tmpDck.push_back(tmpCrd);
            }
        }

        BoardT tmpBoard(tmpDck); 
        
        for(nat i = 0; i < 5; i++){
            tmpBoard.tab_mv(Foundation, i*2, 2);
            tmpBoard.tab_mv(Foundation, i*2, 3);
            tmpBoard.tab_mv(Foundation, i*2, 0);
            tmpBoard.tab_mv(Foundation, i*2, 1);
            tmpBoard.tab_mv(Foundation, i*2+1, 6);
            tmpBoard.tab_mv(Foundation, i*2+1, 7);
            tmpBoard.tab_mv(Foundation, i*2+1, 4);
            tmpBoard.tab_mv(Foundation, i*2+1, 5);
        }

        for(nat i = 0; i < 64; i++){
            tmpBoard.deck_mv();
        }
        
        //Testing normal cases
        REQUIRE(!tmpBoard.is_win_state());
        
        for(int i = 0; i < 8; i++){
            for(nat i = 0; i < 8; i++){
            tmpBoard.waste_mv(Foundation, i);
            }
        }   
        
        REQUIRE(tmpBoard.is_win_state());
    }

    SECTION("Testing valid_mv_exists"){
        //Setup for testing
        swap_cards(fullDck[0], fullDck[40]);
        swap_cards(fullDck[47], fullDck[27]);
        
        BoardT tmpBoard(fullDck);  

        //Start of testing

        //Deck move exists
        REQUIRE(tmpBoard.valid_mv_exists());

        //No more moves from deck 
        for(nat i = 0; i < 64; i++){
            tmpBoard.deck_mv();
        }

        //Moving card from waste stack to foundation exists
        REQUIRE(tmpBoard.valid_mv_exists());

        //No more moves from waste stack
        tmpBoard.waste_mv(Foundation, 0);

        //Moving card from tableau(T[0]) to foundation(F[0]) exists
        REQUIRE(tmpBoard.valid_mv_exists());

        //No more moves from tableau 
        tmpBoard.tab_mv(Foundation, 0, 0);

        //No more moves available at all
        REQUIRE(!tmpBoard.valid_mv_exists());
    }
}