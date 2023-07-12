
#include <vector>
#include "Board.h"
#include "Side.h"
Board::Board(int nHoles, int nInitialBeansPerHole) : m_holes(nHoles), m_initialBeansPerHole(nInitialBeansPerHole){
    //check validity
    if (nHoles <= 0)
        m_holes = 1;
    if (nInitialBeansPerHole <= 0)
        m_initialBeansPerHole = 0;
    //initialize "grid" of vectors for representation of board
        m_board = std::vector<std::vector<int>>(NSIDES, std::vector<int>(m_holes + 1, m_initialBeansPerHole));
    //set pots to 0;
    m_board[NORTH][POT] = 0;
    m_board[SOUTH][POT] = 0;
    //set total beans
    m_totalBeans = m_initialBeansPerHole * m_holes * NSIDES;
}

int Board::holes() const{
    return m_holes;
}
//beans getter
int Board::beans(Side s, int hole) const{
    if( hole > m_holes || hole < 0){
        return -1;
    }
    return m_board[s][hole];
}
//beans on a side
int Board::beansInPlay(Side s) const{
    int count = 0;
    for (int i = 1; i < m_holes + 1; i++){
        count += m_board[s][i];
    }
    return count;
}


int Board::totalBeans() const{
    return m_totalBeans;
}
bool Board::sow(Side s, int hole, Side& endSide, int& endHole){
    //check validity
    if (beans(s, hole)<=0 || hole == 0){
        return false;
    }
    // Store initial values
   endSide = s;
   endHole = hole;
   int beansToSow = m_board[s][hole];
   m_board[s][hole] = 0;
   // Sow the beans counterclockwise
   while (beansToSow > 0) {
       if (endSide == NORTH){
           // if at hole 1 and at own pot
           if (hole == 1 && s == NORTH){
               hole--;
               //increment
               m_board[NORTH][hole]++;
               //change index to 1 and move to SOUTH
               endSide = SOUTH;
           }
           else if (hole == 1 && s == SOUTH){
               //set endSide and hole
               endSide = SOUTH;
               beansToSow++;
               hole = 0;
           }
           else{
               //decrement hole
               hole--;
               m_board[NORTH][hole]++;
           }
       }
       else {
           //if at end and south
           if (hole == m_holes && s == SOUTH){
               //increment hole and pot
               hole++;
               m_board[SOUTH][0]++;
               //change side
               endSide = NORTH;
           }
           else if (hole == m_holes && s == NORTH){
               //if at end and north increment hole and switch side
               hole++;
               endSide = NORTH;
               //increment to account for decrement
               beansToSow++;
           }
           else{
            //increment next hole
               hole++;
               m_board[SOUTH][hole]++;
           }
       }
       beansToSow--; 
    }
    //set endHole to hole
    endHole = hole; 
    return true;
    }
bool Board::moveToPot(Side s, int hole, Side potOwner){
    //check validity
    if (beans(s,hole)==-1 || hole == 0){
        return false;
    }
    //move to pot
    else{
        int temp = m_board[s][hole];
        m_board[s][hole] = 0;
        m_board[potOwner][0] += temp;
        return true;
    }
}
bool Board::setBeans(Side s, int hole, int beans){
    if (this->beans(s,hole)==-1 || beans < 0){
        return false;
    }
    m_totalBeans = m_totalBeans + (beans - this->beans(s,hole));
    m_board[s][hole] = beans;
    return true;
}
