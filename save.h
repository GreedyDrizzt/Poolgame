#ifndef SAVE_H
#define SAVE_H

#include <vector>
#include "table.h"
class Game;//circular reference
class Ball;
class Save
{
public:
    Save();
    ~Save(){
        if(m_game)
            delete m_game;
    }
//    const std::vector<Ball*>& getBalls() const {return m_balls;}
//    const StageTwoTable& getTable() const {return m_table;}
    void setGame(Game* game);
    Game *getGame();
//    addBall(Ball *ball);
private:
    Game *m_game;
};

#endif // SAVE_H
