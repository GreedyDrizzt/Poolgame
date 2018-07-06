#include "save.h"
#include "ball.h"
#include "game.h"
Save::Save()
{
//    m_balls = new std::vector<CompositeBall>;
//    m_table = new StageTwoTable();
}
void Save::setGame(Game* game){
    m_game = game;
//    for(Ball* ball:*(game->m_balls))
//    {
//        addBall(ball);
//    }
//    m_table = *dynamic_cast<StageTwoTable*>((game->m_table));
}

Game *Save::getGame(){
    Game *ret = m_game;
    m_game = nullptr;
    return ret;
}
//Save::addBall(Ball *ball){
//    Ball * newBall = nullptr;
//    switch (ball->GetBallType())
//    {
//    case BallType::BALLTYPE_STAGEONEBALL:
//        newBall = new StageOneBall(*dynamic_cast<StageOneBall*>(ball));
//        break;
//    case BallType::BALLTYPE_COMPOSITEBALL:
//        newBall = new CompositeBall(*dynamic_cast<CompositeBall*>(ball));
//        break;
//    }

//    if (newBall != nullptr)
//    {
//        m_balls.push_back(newBall);
//    }
//}
