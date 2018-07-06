#ifndef CONCRETELOGICB_H
#define CONCRETELOGICB_H
#include "componentlogic.h"
class ConcreteLogicB:public ComponentLogic
{
public:
    ConcreteLogicB(Game* game):ComponentLogic(game){}
    void Algorithm(bool hitrate);
};

#endif // CONCRETELOGICB_H
