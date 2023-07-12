//
//  Player.hpp
//  Project3
//
//  Created by Christian Solano on 5/23/23.
//

#ifndef Player_hpp
#define Player_hpp
#include <iostream>
#include <string>
#include "Board.h"
#include "Side.h"
#include <climits>
class Player {
public: 
    Player(std::string name);
    //Create a Player with the indicated name.
    std::string name() const;
    //Return the name of the player.
    virtual bool isInteractive() const;
    //Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
    virtual int chooseMove(const Board& b, Side s) const = 0;
    //Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
    virtual ~Player();
    //Since this class is designed as a base class, it should have a virtual destructor.
private:
    std::string m_name;
};

class HumanPlayer : public Player {
public:
    HumanPlayer(std::string name);
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
};

class BadPlayer : public Player {
public:
    BadPlayer(std::string name);
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
};


//Smart Player class
class SmartPlayer : public Player {
public:
    SmartPlayer(std::string name);
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
private:
    int evaluate(Board b,Side s) const;
    int evaluate(Board b) const;
    void chooseMoveHelper(JumpyTimer& timer,  Side player, Board& board, int& bestHole, int&value, int depth, Side original,double timeLimit) const;
    void makeMove(Board& b,int hole,Side s);
};
#endif /* Player_hpp */
