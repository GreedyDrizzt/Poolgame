#ifndef COMPONENTLOGIC_H
#define COMPONENTLOGIC_H
#include <vector>
class Game;
class Ball;
class Table;
class ComponentLogic
{
public:
    ComponentLogic(Game* game);

    void ComponentStrategy(int level);

    virtual void Algorithm(bool hit)=0;
protected:
    Game* m_game;
    bool m_hitrate;
};
#endif // COMPONENTLOGIC_H
