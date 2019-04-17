#include "catch.h"
#include "CardTypes.h"
#include "CardStack.h"

#include <stdexcept>

TEST_CASE("tests for CardStackT" , "[Stack]") {

    //Setup Before Every Test
    std::vector<CardT> tstDck;
    CardT crd1 = { static_cast<SuitT>(0), 1 };
    CardT crd2 = { static_cast<SuitT>(1), 1 };
    tstDck.push_back(crd1);
    tstDck.push_back(crd2);
    CardStackT tstCrdStckT(tstDck); 

    SECTION( "Testing constructor & toSeq" ){   
        std::vector<CardT> tmpDck = tstCrdStckT.toSeq();
        REQUIRE((   
                tmpDck[0].s == 0 
            &&  tmpDck[0].r == 1
            &&  tmpDck[1].s == 1
            &&  tmpDck[1].r == 1
        ));
    }

    SECTION( "Testing size" ){   
        nat tmpSize = 2;        
        REQUIRE(tmpSize == tstCrdStckT.size());
    }

    SECTION( "Testing push" ){   
        CardT crd3 = { static_cast<SuitT>(2), 1 };
        tstCrdStckT = tstCrdStckT.push(crd3);
        std::vector<CardT> tmpDck = tstCrdStckT.toSeq();
        REQUIRE((
                tmpDck[2].s == 2 
            &&  tmpDck[2].r == 1
        ));
    }    

    SECTION( "Testing pop" ){
        nat tmpSize = 1;        
        tstCrdStckT = tstCrdStckT.pop();
        REQUIRE(tmpSize == tstCrdStckT.size());
        tstCrdStckT = tstCrdStckT.pop();
        REQUIRE_THROWS_AS(tstCrdStckT.pop(), std::out_of_range);
    }

    SECTION( "Testing top" ){       
        CardT tmpCrd = tstCrdStckT.top();
        REQUIRE((   
                tmpCrd.s == 1 
            &&  tmpCrd.r == 1
        ));
        tstCrdStckT = tstCrdStckT.pop();
        tmpCrd = tstCrdStckT.top();
        REQUIRE((   
                tmpCrd.s == 0 
            &&  tmpCrd.r == 1
        ));
        tstCrdStckT = tstCrdStckT.pop();
        REQUIRE_THROWS_AS(tstCrdStckT.top(), std::out_of_range);
    }
}