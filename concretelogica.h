#ifndef CONCRETELOGICA_H
#define CONCRETELOGICA_H
#include "componentlogic.h"

class ConcreteLogicA: public ComponentLogic
{
public:
    ConcreteLogicA(Game* game) : ComponentLogic(game){}
    void Algorithm(bool hitrate);
};

#endif // CONCRETELOGICA_H
