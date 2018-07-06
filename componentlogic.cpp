#include "componentlogic.h"
#include "game.h"

ComponentLogic::ComponentLogic(Game* game)
{
    m_game=game;
    m_hitrate = false;
}
void ComponentLogic::ComponentStrategy(int level)
{
    int random_num = rand() & 100 + 1;
    if (random_num<level)
    {
        m_hitrate = true;
        Algorithm(m_hitrate);
    }
    else
    {
        m_hitrate =false;
        Algorithm(m_hitrate);
    }

}
