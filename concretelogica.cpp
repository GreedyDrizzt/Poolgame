#include "concretelogica.h"
#include "game.h"
#include "ball.h"

#include <QtDebug>

void ConcreteLogicA::Algorithm(bool hitrate)
{
    qDebug() << "Algo A";
    std::vector<Ball*>& gameBalls=*(m_game->m_balls);
    Table* gameTable=m_game->m_table;
    Ball *cue=nullptr;
    for (Ball *b:gameBalls)
    {
        if(b->isCue())
        {
            cue =b;
            break;
        }
    }
    if(cue == nullptr)
        return;
    Ball* a=nullptr;
    for (Ball *b : gameBalls)
    {
        double distance = sqrt((gameTable->getWidth())*(gameTable->getWidth())+(gameTable->getHeight())*(gameTable->getHeight()));

        if (b != cue)
        {
            double length = (b->getPosition()-cue->getPosition()).length();
            if(length<distance)
            {
                distance = length;
                a=b;
            }
        }
    }
    if(hitrate){
        for(Pocket* p: gameTable->getPocket())
        {
            QVector2D p_pos = p->getPosition();
            QVector2D routeToPocket = (p_pos - a->getPosition()).normalized();
            double deltaHeight = (a->getRadius()) * routeToPocket.normalized().y();
            double deltaWidth = (a->getRadius()) * routeToPocket.normalized().x();
            QVector2D hitPosition = a->getPosition() - QVector2D(deltaWidth, deltaHeight) - (routeToPocket*cue->getRadius());//已得到撞击点
            QVector2D movDirection = (hitPosition - (cue->getPosition())).normalized();
            for (Ball* b: gameBalls)
            {
                if(b->GetBallType()!=BallType::DECORATORTYPE_CUEBALL)
                {
                    double distance = b->getPosition().distanceToLine(cue->getPosition(),movDirection*gameTable->getWidth());
                    if (distance>((b->getRadius())+(cue->getRadius())))
                    {
                        qDebug()<<"in the if";
                        cue->setVelocity(movDirection*gameTable->getWidth()*0.5);
                        return;
                    }
                    else
                        continue;
                }
                else
                    continue;
            }
        }
    }
    else
    {
        cue->setVelocity((a->getPosition()-cue->getPosition()).normalized()*gameTable->getWidth()*0.5);
    }
}
