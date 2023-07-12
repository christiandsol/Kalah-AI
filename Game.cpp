//
//  Game.cpp
//  Project3
//
//  Created by Christian Solano on 5/23/23.
//
#include "Player.h"
#include "Game.h"
#include "Side.h"

//Construct a Game to be played with the indicated players on a copy of the board b. The player on the south side always moves first.
Game::Game(const Board& b, Player* south, Player* north): m_board(b), m_south(south), m_north(north){
    //no copy constructor needed, only need to copy data members?
}
void Game::display() const{
    //space out game
    int space = m_board.holes();
    //North name
  for (int i = 0; i < space; i++){
    if(i < space/2){
      std::cout<<" ";
    }
  }
    //north beans
  std::cout<< m_north->name()<<std::endl;
  for (int i =1; i < space + 1; i++){
    std::cout<<" "<<m_board.beans(NORTH,i);
  }
  std::cout<<std::endl;
    //Middle
  std::cout<<m_board.beans(NORTH,0);
  for (int i = 0; i < space * 2; i++){
    std::cout << " ";
  }
  std::cout<<m_board.beans(SOUTH,0)<<std::endl;
    //South beans
   for (int i =1; i < space + 1; i++){
    std::cout<<" "<<m_board.beans(SOUTH,i);
  }
  std::cout<<std::endl;
    for (int i = 0; i < space; i++){
    if(i < space/2){
      std::cout<<" ";
    }
  }
    //South name
  std::cout<< m_south->name()<<std::endl;
}
void Game::status(bool& over, bool& hasWinner, Side& winner) const{
    //set winner to avoid bad access
    winner = NORTH;
    if (m_board.beansInPlay(winner)==0){
        //set over true
        over = true;
        int northBeans = m_board.beans(NORTH, POT);
        int southBeans = m_board.beans(SOUTH, POT);
        //compare beans and set winnder
        if (southBeans > northBeans){
            hasWinner = true;
            over = true;
            winner = SOUTH;
        }
        else if (northBeans > southBeans){
            hasWinner = true;
            winner = NORTH;
        }
        else {
            hasWinner = false;
        }
    }
    else{
        over = false;
    }
}

bool Game::move(Side s){
    
    //if side has no beans to play, pick up opponents and return false
    if (m_board.beansInPlay(s)==0){
        for (int i = 1; i <= m_board.holes(); i++){
            m_board.moveToPot(opponent(s), i, opponent(s));
        }
        return false;
    }
    Side endSide;
    int endHole;
    int holeOfChoice;
    //choose hole
    if (s==SOUTH){
        holeOfChoice = m_south->chooseMove(m_board, s);
        if (!m_south->isInteractive()){
            std::cout<<m_south->name() << " chooses hole "<< holeOfChoice<< std::endl;
        }
    }
    else{
        holeOfChoice = m_north->chooseMove(m_board, s);
        if (!m_north->isInteractive()){
            std::cout<<m_north->name() << " chooses hole "<< holeOfChoice<<std::endl;
        }
    }
    //sow
    m_board.sow(s, holeOfChoice, endSide, endHole);
    //check for capture
    if (m_board.beans(endSide, endHole) == 1 && endSide == s && m_board.beans(opponent(s), endHole) > 0){
        //move beans to pot
        m_board.moveToPot(s, endHole, s);
        m_board.moveToPot(opponent(endSide), endHole, s);
    }

    //check if you landed in your own pot, check endSide is opponent because you change to opposite within code
    if ((endHole == POT || endHole == m_board.holes()+1) && endSide == opponent(s)){
        if (s == SOUTH){
            std::cout<< m_south->name() << " landed in Their Pot, "<< m_south->name()<<" Goes Again!"<<std::endl;
                display();
                return move(s);

        }
        //repeat for north
        else if (s == NORTH) {
            std::cout<< m_north-> name() << " landed in Their Pot, " << m_north->name()<<" Goes Again!"<<std::endl;
                display();
                return move(s);
        }
    }
    return true;
}

//Attempt to make a complete move for the player playing side s. "Complete" means that the player sows the seeds from a hole and takes any additional turns required or completes a capture. Whenever the player gets an additional turn, you should display the board so someone looking at the screen can follow what's happening. If the move can be completed, return true; if not, because the move is not yet completed but side s has no holes with beans to pick up and sow, sweep any beans in s's opponent's holes into that opponent's pot and return false.
void Game::play(){
    //display
    display();
    //create variables
    Side winner;
    bool over;
    bool hasWinner = false;
    Side s = SOUTH;
    //continue to perform a move until you can't
    while (move(s)){
        display();
        //check interactivity
        if(!m_south->isInteractive() && !m_north->isInteractive()){
            std::cout<< "Press Enter to Continue: " << std::endl;
            std::cin.ignore(10000, '\n');
        }
        //change s to opponent
        s = opponent(s);
    }
    //move returned false, meaning a side couldn't perform a move
    if (s == NORTH){
        std::cout<< m_north->name() << " has no beans left to sow" <<std::endl<< "The remaining beans go in " << m_south->name() << "'s pot" <<std::endl;
    }
    else {
        std::cout<< m_south->name() << " has no beans left to sow" <<std::endl<< "The remaining beans go in " << m_north->name() << "'s pot" <<std::endl;
    }
    display();
    status(over, hasWinner, winner);
    // if it's over and has winner
    if (hasWinner && over){
        if (winner == NORTH){
            std::cout<< "the Winner is " << m_north->name() << std::endl;
        }
        else {
            std::cout<< "the Winner is " << m_south->name() << std::endl;

        }
    }
    // if it's over and doesn't have winner
    else if(!hasWinner){
        std::cout<<"Tie!"<<std::endl;
    }
}

int Game::beans(Side s, int hole) const{
    
    if( hole > m_board.holes() || hole < 0){
        return -1;
    }
    return m_board.beans(s, hole);}
//Return the number of beans in the indicated hole or pot of the game's board, or −1 if the hole number is invalid. This function exists so that we and you can more easily test your program.

