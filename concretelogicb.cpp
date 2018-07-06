#include "concretelogicb.h"
#include "game.h"
#include "ball.h"
#include <QtDebug>
void ConcreteLogicB::Algorithm(bool hitrate)
{
    qDebug() << "Algo B";
    std::vector<Ball*>& gameBalls=*(m_game->m_balls);
    Table* gameTable=m_game->m_table;
    Ball *cue=nullptr;
    for (Ball *b:gameBalls)
    {
        if(b->isCue()){
            cue = b;
            break;
        }
//        cue = dynamic_cast<CueBall *>(b);
//        if(cue){
//            break;
//        }
//        else
//           continue;
    }
    if(cue == nullptr){
        return;
    }
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
    if(hitrate)
    {
        QVector2D p_pos = a->getPosition();
        QVector2D routeToPocket = p_pos - a->getPosition();
        double deltaHeight = (a->getRadius()) * routeToPocket.normalized().y();
        double deltaWidth = (a->getRadius()) * routeToPocket.normalized().x();
        QVector2D hitPosition = a->getPosition() - QVector2D(deltaWidth, deltaHeight) - (routeToPocket*(cue->getRadius()));//已得到撞击点

        QVector2D movDirection = hitPosition - (cue->getPosition());
        cue->setVelocity(movDirection.normalized()*gameTable->getWidth()*0.5);
    }
    else
    {
        cue->setVelocity((a->getPosition()-cue->getPosition()).normalized()*gameTable->getWidth() * 0.1);
    }

}
