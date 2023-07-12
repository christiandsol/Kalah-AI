//
//  Player.cpp
//  Project3
//
//  Created by Christian Solano on 5/23/23.
//

#include "Player.h"

//Create a Player with the indicated name.

Player::Player(std::string name): m_name(name){}
//Return the name of the player.
std::string Player::name() const{ return m_name; }
//check interactivity
bool Player::isInteractive() const{
    return true; }
//Since this class is designed as a base class, it should have a virtual destructor.
Player::~Player(){};

//HumanPlayer
HumanPlayer::HumanPlayer(std::string name): Player(name){}

//check interactivity
bool HumanPlayer::isInteractive() const {
    return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const{
    int number;
    //ask to select hole until valid
    do {
        std::cout << "Select a hole " << name() <<":"<<std::endl;
        std::cin>>number;
        
        if (b.beans(s, number) == 0){
            std::cout<<"Hole must have beans"<< std::endl;
        }
        if (number <= 0 || number > b.holes())
            std::cout<<"The hole number must be from 1 to " << b.holes()<< std::endl;
    } while (number <=0 || number > b.holes() || b.beans(s, number)==0);
    return number;
}

BadPlayer::BadPlayer(std::string name): Player(name){}

bool BadPlayer::isInteractive() const {
    return false;
}
//choose earliest hole
int BadPlayer::chooseMove(const Board& b, Side s) const{
    int i = 1;
    for (; i <= b.holes(); i++){
        if (b.beans(s,i) > 0)
            return i;
    }
    return -1;
}



SmartPlayer::SmartPlayer(std::string name): Player(name){}

bool SmartPlayer::isInteractive() const {
    return false;
}
void SmartPlayer::chooseMoveHelper(JumpyTimer& timer,Side s, Board& b, int& bestHole, int& value, int depth,Side original,double timeLimit) const {
    //if timer is done evaluate
    if (timeLimit <= 0){
        bestHole = -1;
        value = evaluate(b,s);
        return;
    }
    //if turn is over move to pot and evaluate
    if (b.beansInPlay(s) == 0) {
        for (int i = 1; i <= b.holes(); i++){
            b.moveToPot(opponent(s), i, opponent(s));
        }
        bestHole = -1;
        value = evaluate(b, s);
        return;
    }
    //if depth reached, evaluate, even depth
    if (depth >= 12) {
        bestHole = -1;
        value = evaluate(b, s);
        return;
    }
    //choose correc tvaleu
    value = ((s == original) ? INT_MIN : INT_MAX);
    //for every poosible move
    for (int h = 1; h <= b.holes(); h++) {
        if (b.beans(s, h) > 0) {
            //set up timers
            double thisBranchTimeLimit = timeLimit / (b.holes() - h + 1);
            double startTime = timer.elapsed();
            //initialize values and copies
            Board newBoard(b);
            Side endSide;
            int endHole;
            //sow
            newBoard.sow(s, h, endSide, endHole);
            int h2, v2,h3;
            //if last hole is pot, go again
            if (endHole == 0 && endSide == s){
                chooseMoveHelper(timer,s, newBoard, h3, v2, depth,original,thisBranchTimeLimit);
            }
            //do capture
            else if (newBoard.beans(endSide, endHole) == 1 && endSide == s && newBoard.beans(opponent(s), endHole) > 0){
                newBoard.moveToPot(opponent(s), endHole, s);
                newBoard.moveToPot(s, endHole, s);
                chooseMoveHelper(timer, opponent(s), newBoard, h2, v2, depth + 1,original,thisBranchTimeLimit);
            }
            //opponents move
            else{
                chooseMoveHelper(timer,  opponent(s), newBoard, h2, v2, depth + 1,original,thisBranchTimeLimit);
            }
            //adjust timeLimit
            timeLimit-=(timer.elapsed()-startTime);
            //adjust bestHole and value accordingly
            if ((s == original && v2 >= value) || (s == opponent(original) && v2 <= value)) {
                bestHole = h;
                value = v2;
            }
            if (timeLimit <=0)
                timeLimit = 0;
        }
    }
    return;
}

int SmartPlayer::evaluate(Board b, Side s) const{
    //check worst/bestcases
    if (b.beansInPlay(s)== 0 && (b.beans(s, 0) > b.beans(opponent(s), 0))){
        return INT_MAX;
    }
    if (b.beansInPlay(s) == 0 && (b.beans(s, 0) < b.beans(opponent(s), 0))){
        return INT_MIN;
    }
    if (b.beansInPlay(s) == 0 && (b.beans(s, 0) == b.beans(opponent(s), 0))){
        return 0;
    }
    int value = 0;
    //difference in beans
//    for (int i = 1; i <= b.holes(); i++){
//        value += b.beans(s, i) - b.beans(opponent(s), i);
//    }
//    //adjust
//    value = value/2;
//    //add difference in pots
    value += (b.beans(s, 0) - b.beans(opponent(s), 0));
    return value;
}

int SmartPlayer::chooseMove(const Board& b, Side s) const{
    double timeLimit = 4990;
    JumpyTimer timer(1000);
    int bestHole;
    int value = INT_MIN;
    Board copy(b);
    // Call the helper function to start the minimax algorithm
    chooseMoveHelper(timer,s, copy, bestHole, value, 0,s,timeLimit);
    return bestHole;
}
