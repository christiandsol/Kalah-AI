//
//  main.cpp
//  Project3
//
//  Created by Christian Solano on 5/16/23.
//

#include <iostream>
#include "Game.h"
#include <cassert>
void doGameTests()
{
    BadPlayer bp1("Bart");
    BadPlayer bp2("Homer");
    Board b(3, 0);
    b.setBeans(SOUTH, 1, 2);
    b.setBeans(NORTH, 2, 1);
    b.setBeans(NORTH, 3, 2);
    Game g(b, &bp1, &bp2);
    bool over;
    bool hasWinner;
    Side winner;
      //    Homer
      //   0  1  2
      // 0         0
      //   2  0  0
      //    Bart
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 0 &&
    g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 2 &&
    g.beans(SOUTH, 1) == 2 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);

    g.move(SOUTH);
//         0  1  0
//       0         3
//         0  1  0
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
    g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 0 &&
    g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

    g.move(NORTH);
      //   1  0  0
      // 0         3
      //   0  1  0
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
    g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
    g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

    g.move(SOUTH);
      //   1  0  0
      // 0         3
      //   0  0  1
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
    g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
    g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 1);

    g.move(NORTH);
      //   0  0  0
      // 1         4
      //   0  0  0
    g.status(over, hasWinner, winner);
    assert(over && g.beans(NORTH, POT) == 1 && g.beans(SOUTH, POT) == 4 &&
    g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
    g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
    assert(hasWinner && winner == SOUTH);
}

void doBoardTests()
    {
        Board b(3, 2);
        assert(b.holes() == 3  &&  b.totalBeans() == 12  &&
                        b.beans(SOUTH, POT) == 0  &&  b.beansInPlay(SOUTH) == 6);
        b.setBeans(SOUTH, 1, 1);
        b.moveToPot(SOUTH, 2, SOUTH);
        assert(b.totalBeans() == 11  &&  b.beans(SOUTH, 1) == 1  &&
                    b.beans(SOUTH, 2) == 0  &&  b.beans(SOUTH, POT) == 2  &&
            b.beansInPlay(SOUTH) == 3);
        Side es;
        int eh;
        b.sow(SOUTH, 3, es, eh);
        assert(es == NORTH  &&  eh == 3  &&  b.beans(SOUTH, 3) == 0  &&
                    b.beans(NORTH, 3) == 3  &&  b.beans(SOUTH, POT) == 3  &&
            b.beansInPlay(SOUTH) == 1  &&  b.beansInPlay(NORTH) == 7);
    }
void doPlayerTests()
{
    HumanPlayer hp("Marge");
    assert(hp.name() == "Marge"  &&  hp.isInteractive());
    BadPlayer bp("Homer");
    assert(bp.name() == "Homer"  &&  !bp.isInteractive());
    SmartPlayer sp("Lisa");
    assert(sp.name() == "Lisa"  &&  !sp.isInteractive());
    Board b(3, 2);
    b.setBeans(SOUTH, 2, 0);
    std::cout << "=========" << std::endl;
    int n = hp.chooseMove(b, SOUTH);
    std::cout << "=========" << std::endl;
    assert(n == 1  ||  n == 3);
    n = bp.chooseMove(b, SOUTH);
    assert(n == 1  ||  n == 3);
    n = sp.chooseMove(b, SOUTH);
    assert(n == 1  ||  n == 3);
}

using namespace std;
int main() {
    Board b1(6,4);
    Board b2(-1,0);
    assert(b2.holes()==1);
    assert(b2.beansInPlay(NORTH)==0);
    assert(b2.beans(NORTH, 0)==0);
    assert(b2.beans(SOUTH, 0)==0);
    for (int i = 1; i < b2.holes() + 1; i++){
        assert(b2.beans(NORTH, i)==0);
    }
    assert(b1.holes()==6);
    assert(b1.beansInPlay(NORTH)==24);
    assert(b1.beansInPlay(SOUTH)==24);
    assert(b1.totalBeans()==48);
    assert(b1.beans(NORTH, 0)==0);
    assert(b1.beans(SOUTH, 0)==0);
    assert(b2.setBeans(NORTH, 5, 8)==false);
    assert(b1.setBeans(NORTH, 5, 8));
    assert(b1.beans(NORTH, 5)==8);
    assert(b1.beansInPlay(NORTH)==28);
    assert(b1.totalBeans()==(48+4));
    assert(!b1.moveToPot(NORTH, 0, NORTH));
    assert(b1.moveToPot(NORTH, 5, SOUTH));
    assert(b1.beans(SOUTH, 0)==8);
    assert(b1.beansInPlay(NORTH)==(20));
    assert(b1.beansInPlay(SOUTH)==(24));
    assert(b1.totalBeans()==52);
    assert(b1.moveToPot(NORTH, 5, NORTH));
    assert(b1.beans(NORTH, 0)==0);
    Side endSide;
    int endHole;
    Board b3(4,2);
    assert(b3.sow(SOUTH, 4, endSide, endHole)==true);
    assert(endHole == 4 && endSide == NORTH);
    assert(b3.beans(NORTH, 4)==3);
    assert(b3.beans(SOUTH, 4)==0);
    assert(b3.beans(NORTH,0)==0);
    assert(b3.beans(SOUTH,0)==1);
    Board b4(4, 16);
    assert(b4.sow(NORTH,2,endSide, endHole)==true);
    assert(endSide == NORTH && endHole ==4);
    for (int i = 1; i < 4; i++){
        assert(b4.beans(SOUTH, i)==18);
    }
    assert(b4.beans(NORTH, POT)==2);
    assert(b4.beans(NORTH, 1)==18);
    assert(b4.beans(NORTH, 3)==17);
    assert(b4.beans(NORTH, 4)==18);
    assert(b4.beans(NORTH, 2)==1);
    assert(b4.beansInPlay(NORTH)==(18*2 + 1 + 17));
    assert(b4.beansInPlay(SOUTH)== (18*4));



    //test Player
    Board b5(6,4);
    HumanPlayer p1("test");
    assert(p1.name()=="test");
    assert(p1.isInteractive());

    BadPlayer bp1("Bad");
    assert(bp1.name()=="Bad");
    assert(!bp1.isInteractive());
    int number = bp1.chooseMove(b5, NORTH);
    assert(number == 1);
    b5.setBeans(NORTH, 1, 0);
    number = bp1.chooseMove(b5, NORTH);
    assert(number == 2);
    number = bp1.chooseMove(b5, SOUTH);
    assert(number == 1);
    b5.setBeans(SOUTH, 1, 0);
    number = bp1.chooseMove(b5, SOUTH);
    assert(number == 2);
    for (int i = 2; i < 7; i++){
        b5.setBeans(SOUTH, i, 0);
    }
    assert(bp1.chooseMove(b5, SOUTH)==-1);

    //test GAME
    Board gameBoard(6,4);
    BadPlayer player1("Christian");
    BadPlayer player2("Kim");
    //test smartPlayer
    SmartPlayer Kim("Kim");
    HumanPlayer Christian("Christian");
    Player* pKim = &Kim;
    Player* pChristian = &Christian;
    //south goes again
    Game game(gameBoard, pChristian, pKim);
    game.play();
    doBoardTests();
    doPlayerTests();
    doGameTests(); 
    return 0;
}
