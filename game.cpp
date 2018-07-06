#include "game.h"
#include "utils.h"

#include <QJsonArray>
#include <stdexcept>
#include <cmath>
#include <exception>
#include <iostream>
#include <QtDebug>

Game::~Game() {
    // cleanup ya boi
    for (auto b : *m_balls) delete b;
    delete m_balls;
    delete m_table;
}

void Game::render(QPainter &painter) const {
    // table is rendered first, as its the lowest
    m_table->render(painter, m_screenshake);
//    std::string a = "Player: " + std::string(m_playerscore);
//    std::string b = "Computer:" + std::string(m_AIscore);
    QString score1 = QString::number(m_playerscore);
    QString score2 = QString::number(m_AIscore);
    QString a = "Player: "+ score1;
    QString b = "Computer: "+score2;
    painter.drawText(m_table->getWidth()+10, 10, a);
    painter.drawText(m_table->getWidth()+10, 50, b);

    if(m_win==2)
    {
        QFont font=painter.font();
        font.setPointSize(24);
        painter.setFont(font);
        painter.drawText(m_table->getWidth()/2, m_table->getHeight()/2, "YOU LOSE!");
    }
    else if(m_win==1)
    {
        QFont font=painter.font();
        font.setPointSize(24);
        painter.setFont(font);
        painter.drawText(m_table->getWidth()/2, m_table->getHeight()/2, "YOU WIN!");
    }


    // then render all the balls
    for (Ball* b : *m_balls) b->render(painter, m_screenshake);
//    if(m_playersNum>1)
//    {
//        painter.setPen(Qt::black);//draw the base power rectangle
//        painter.setBrush(QBrush(Qt::white));
//        painter.drawRect(m_table->getWidth(),0,50,m_table->getHeight());

//    }

}

void Game::animate(double dt) {
    //Using AI to change velocity of balls.
    if(AICanTakeAction())
    {
        qDebug() << "ai action";
//        if()
        if(this->getDifficulty()==0){
            m_logic = new ConcreteLogicB(this);
            m_logic->Algorithm(false);
        }
        else if(this->getDifficulty()==1)
        {
            m_logic = new ConcreteLogicA(this);
            m_logic->Algorithm(true);
        }
        else
        {
            m_logic = new ConcreteLogicB(this);
            m_logic->Algorithm(true);
        }

        m_allBallsStill = false;
//        SwitchTurn();
        //need to be implement
    }

    // keep track of the removed balls (they're set to nullptr during loop)
    // clean up afterwards
    std::vector<Ball*> toBeRemoved;
    // add these balls to the list after we finish
    std::vector<Ball*> toBeAdded;

    // (test) collide the ball with each other ball exactly once
    // to achieve this, balls only check collisions with balls "after them"
    for (auto it = m_balls->begin(); it != m_balls->end(); ++it) {
        Ball* ballA = *it;
        if (ballA == nullptr) continue;

        // correct ball velocity if colliding with table
        QVector2D tableBallDeltaV = resolveCollision(m_table, ballA);
        // test and resolve breakages with balls bouncing off table
        if (ballA->applyBreak(tableBallDeltaV, toBeAdded)) {
            // mark this ball to be deleted
            toBeRemoved.push_back(ballA);
            incrementShake();
            // nullify this ball
            *it = nullptr;

            continue;
        }
        // check whether ball should be swallowed
        if (m_table->sinks(ballA)) {
            // defer swallowing until later (messes iterators otherwise)
            if(ballA->isCue())
            {
                qDebug()<<ballA->getRadius();
                if(m_currentTurn==1)
                {
                    m_win = 2;
                }
                else if(m_currentTurn==2){
                    m_win = 1;
                }
            }
            toBeRemoved.push_back(ballA);
            // nullify this ball
            *it = nullptr;
            //2018.06.06 11.:04
            //calculate the score.
            //if cue ball in pocket,report the winner
//            for(Ball* b:toBeRemoved)
//            {
//                if(b->GetBallType()==BallType::DECORATORTYPE_CUEBALL)
//                {
//                    if(m_currentTurn==1)
//                    {
//                        QPainter painter;
//                        painter.drawText(m_table->getWidth()+10, m_table->getHeight()/2, "YOU LOSE!");
//                    }
//                    else if(m_currentTurn==2){
//                        QPainter painter;
//                        painter.drawText(m_table->getWidth()+10, m_table->getHeight()/2, "YOU WIN!");
//                    }
//                }
//                else
//                {
//                    continue;
//                }
//            }
                if(m_currentTurn==1)
                {
                    m_playerscore++;
                }
                else if(m_currentTurn==2)
                {
                    m_AIscore++;
                }


            SwitchTurn();

            continue;
        }

        // check collision with all later balls
        for (auto nestedIt = it + 1; nestedIt != m_balls->end(); ++nestedIt) {
            Ball* ballB = *nestedIt;
            if (ballB == nullptr) continue;
            if (isColliding(ballA, ballB)) {
                // retrieve the changes in velocities for each ball and resolve collision
                QVector2D ballADeltaV,ballBDeltaV;
                std::tie(ballADeltaV, ballBDeltaV) = resolveCollision(ballA, ballB);

                // add screenshake, remove ball, and add children to table vector if breaking
                if (ballA->applyBreak(ballADeltaV, toBeAdded)) {
                    toBeRemoved.push_back(ballA);
                    incrementShake();
                    // nullify this ball
                    *it = nullptr;
                    break;
                }
                // add screenshake, remove ball, and add children to table vector if breaking
                if (ballB->applyBreak(ballBDeltaV, toBeAdded)) {
                    toBeRemoved.push_back(ballB);
                    incrementShake();
                    // nullify this ball
                    *nestedIt = nullptr;
                    continue;
                }
            }
        }
        // we marked this ball as deleted, so skip
        if (*it == nullptr) continue;

        // move ball due to speed
        ballA->translate(ballA->getVelocity() * dt);
        // apply friction
        ballA->changeVelocity(-ballA->getVelocity() * m_table->getFriction() * dt);
    }

    // clean up them trash-balls
    for (Ball* b : toBeRemoved) {
        delete b;
        // delete all balls marked with nullptr
        m_balls->erase(std::find(m_balls->begin(), m_balls->end(), nullptr));
    }
    for (Ball* b: toBeAdded) m_balls->push_back(b);

    //Added By Jiang
    //If there are two players, keep stable
    if(m_playersNum==2 && !m_allBallsStill)
    {
        m_allBallsStill=true;
        for(Ball* ball:*m_balls)
        {
            if(ball->getVelocity().length()>0.001)
            {
                m_allBallsStill=false;
                break;
            }
        }
        if(m_allBallsStill)
        {
            qDebug() << "switch turn call";
            SaveGame();
            SwitchTurn();
        }
    }


    updateShake(dt);
}

void Game::updateShake(double dt) {
    // <3 code lovingly taken from here: <3
    // https://gamedev.stackexchange.com/a/47565
    // slightly modified..

    // update the screen shake per time step
    m_shakeRadius *= (1-dt)*0.9;
    m_shakeAngle += (150 + rand()%60);
    m_screenshake = QVector2D(sin(m_shakeAngle)*m_shakeRadius, cos(m_shakeAngle)*m_shakeRadius);
}

QVector2D Game::resolveCollision(const Table* table, Ball* ball) {
    QVector2D bPos = ball->getPosition();

    QVector2D startingVel = ball->getVelocity();

    // resulting multiplicity of direction. If a component is set to -1, it
    // will flip the velocity's corresponding component
    QVector2D vChange(1,1);

    // ball is beyond left side of table's bounds
    if (bPos.x() - ball->getRadius() <= 0) {
        // flip velocity if wrong dir
        if (ball->getVelocity().x() <= 0) vChange.setX(-1);
    // ball is beyond right side of table's bounds
    } else if (bPos.x() + ball->getRadius() >= 0 + table->getWidth()) {
        // flip velocity if wrong dir
        if (ball->getVelocity().x() >= 0) vChange.setX(-1);
    }
    // ball is above top of the table's bounds
    if (bPos.y() - ball->getRadius() <= 0) {
        // flip iff we're travelling in the wrong dir
        if (ball->getVelocity().y() <= 0) vChange.setY(-1);
    // ball is beyond bottom of table's bounds
    } else if (bPos.y() + ball->getRadius() >= 0 + table->getHeight()) {
        // if we're moving down (we want to let the ball bounce up if its heading back)
        if (ball->getVelocity().y() >= 0) vChange.setY(-1);
    }

    ball->multiplyVelocity(vChange);

    // return the change in velocity
    return ball->getVelocity() - startingVel;
}

void Game::aiMovment(){
    m_logic->Algorithm(isPlayersTurn);
}

std::pair<QVector2D, QVector2D> Game::resolveCollision(Ball* ballA, Ball* ballB) {
    // SOURCE : ASSIGNMENT SPEC

    // if not colliding (distance is larger than radii)
    QVector2D collisionVector = ballB->getPosition() - ballA->getPosition();
    if (collisionVector.length() > ballA->getRadius() + ballB->getRadius()) {
       throw std::logic_error("attempting to resolve collision of balls that do not touch");
    }
    collisionVector.normalize();

    QVector2D ballAStartingVelocity = ballA->getVelocity();
    QVector2D ballBStartingVelocity = ballB->getVelocity();

    float mr = ballB->getMass() / ballA->getMass();
    double pa = QVector2D::dotProduct(collisionVector, ballA->getVelocity());
    double pb = QVector2D::dotProduct(collisionVector, ballB->getVelocity());

    if (pa <= 0 && pb >= 0) return std::make_pair(QVector2D(0,0), QVector2D(0,0));

    double a = -(mr + 1);
    double b = 2*(mr * pb + pa);
    double c = -((mr - 1)*pb*pb + 2*pa*pb);
    double disc = sqrt(b*b - 4*a*c);
    double root = (-b + disc)/(2*a);
    if (root - pb < 0.01) {
        root = (-b - disc)/(2*a);
    }

    ballA->changeVelocity(mr * (pb - root) * collisionVector);
    ballB->changeVelocity((root-pb) * collisionVector);

    // return the change in velocities for the two balls
    return std::make_pair(ballA->getVelocity() - ballAStartingVelocity, ballB->getVelocity() - ballBStartingVelocity);
}

void Game::SwitchTurn()
{
    if(m_currentTurn==1)
    {
        m_currentTurn=2;
    }
    else if(m_currentTurn==2)
    {
        m_currentTurn=1;
    }
}
bool Game::PlayerCanTakeAction()
{
    if(m_playersNum==1)
    {
        return true;
    }
    else if(m_playersNum==2)
    {
        if(m_currentTurn==1 && m_allBallsStill)
        {
            return true;
        }
    }
    return false;
}

bool Game::AICanTakeAction()
{
    if(m_playersNum==2 && m_currentTurn==2 && m_allBallsStill)
    {
        return true;
    }
    return false;
}

//TODO:
void Game::SaveGame()
{
    qDebug() << "savegame";
    delete lastgame;
    lastgame = new Save();
    lastgame->setGame(this->clone());
}
//2018.06.05  17:02:00
Game* Game::readGame()
{
//    Game* previous = new Game();
//    previous->m_balls = new std::vector<Ball *>();
//    previous->m_table = new StageTwoTable();
//    for(auto b: *m_balls)
//    {
//        delete b;
//    }
//    m_balls->clear();

//    for(auto b: *lastgame->getGame()->m_balls)
//    {
//        Ball* newBall = nullptr;
//        switch (b->GetBallType()) {
//        case BallType::BALLTYPE_STAGEONEBALL:
//            newBall = new StageOneBall(*dynamic_cast<StageOneBall*>(b));
//            break;
//        case BallType::BALLTYPE_COMPOSITEBALL:
//            newBall = new CompositeBall(*dynamic_cast<CompositeBall*>(b));
//            break;
//        }

//        if(b->GetBallType()&BallType::DECORATORTYPE_CUEBALL)
//        {
//            newBall = new CueBall(newBall);
//        }
//        if(b->GetBallType()&BallType::DECORATORTYPE_BALLSPARKLE)
//        {
//            newBall = new BallSparkleDecorator(newBall);
//        }
//        if(b->GetBallType()&BallType::DECORATORTYPE_BALLSMASH)
//        {
//            newBall = new BallSmashDecorator(newBall);
//        }
//        if(newBall!=nullptr)
//        {
//            m_balls->push_back(newBall);
//        }
//    }
//    delete m_table;
//    m_table = new StageTwoTable(*dynamic_cast<StageTwoTable*>(lastgame->getGame()->m_table));

    return lastgame->getGame();

//    return previous;
}

Game* Game::clone(){
    Game* newGame = new Game();
    newGame->m_balls = new std::vector<Ball *>();
    for(Ball* ball : *(this->m_balls)){
        newGame->m_balls->push_back(ball->clone());
        Ball* newBall = newGame->m_balls->back();
        if(newBall->isCue()){
            qDebug() << "Adding new mouse functions";
            newGame->addMouseFunctions(dynamic_cast<CueBall *>(newBall)->getEvents());
        }
    }
    newGame->m_table = m_table->clone();
    newGame->m_playerscore = this->m_playerscore;
    newGame->m_AIscore = this->m_AIscore;
    return newGame;
}
