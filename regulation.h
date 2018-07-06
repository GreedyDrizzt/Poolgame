#ifndef REGULATION_H
#define REGULATION_H
#include "game.h"
#include "gamebuilder.h"

class Regulation
{
public:
    Regulation();
    void setIsPlayersTurn(bool turn){ isPlayersTurn = turn; }
    bool getIsPlayersTurn(){return isPlayersTurn; }
    Game* createGame(GameDirector director){ return director.createGame(); }
private:
    bool isPlayersTurn;
    Game* m_game;

};

#endif // REGULATION_H
