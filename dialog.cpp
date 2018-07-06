#include "dialog.h"
#include "ui_dialog.h"
#include <QPainter>
#include <QTimer>
#include <iostream>
#include <QMouseEvent>
#include <QVBoxLayout>
#include "utils.h"

Dialog::Dialog(Game *game, bool isStageThree, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    m_game(game)
{
    ui->setupUi(this);

    if(isStageThree){
        m_gameStarted = false;
        // menu buttons
        QLayout *layout = new QVBoxLayout(this);
        setLayout(layout);
        m_easy = new QPushButton("Easy", this);
        m_normal = new QPushButton("normal", this);
        m_hard = new QPushButton("hard",this);
        connect(m_easy, QPushButton::released,[this](){this->startGame(0);});
        connect(m_normal, QPushButton::released,[this](){this->startGame(1);});
        connect(m_hard, QPushButton::released,[this](){this->startGame(2);});
        layout->addWidget(m_easy);
        layout->addWidget(m_normal);
        layout->addWidget(m_hard);

        // for animating (i.e. movement, collision) every animFrameMS
        aTimer = new QTimer(this);
        connect(aTimer, SIGNAL(timeout()), this, SLOT(nextAnim()));

        // for drawing every drawFrameMS milliseconds
        dTimer = new QTimer(this);
        connect(dTimer, SIGNAL(timeout()), this, SLOT(tryRender()));
    }else{

        // for animating (i.e. movement, collision) every animFrameMS
        aTimer = new QTimer(this);
        connect(aTimer, SIGNAL(timeout()), this, SLOT(nextAnim()));
        aTimer->start(animFrameMS);

        // for drawing every drawFrameMS milliseconds
        dTimer = new QTimer(this);
        connect(dTimer, SIGNAL(timeout()), this, SLOT(tryRender()));
        dTimer->start(drawFrameMS);
    }
    // set the window size to be at least the table size
/*if()*/
    if(game->m_playersNum>1)
    {
        this->resize(game->getMinimumWidth()+100,game->getMinimumHeight());
    }else
        this->resize(game->getMinimumWidth(), game->getMinimumHeight());
}

Dialog::~Dialog()
{
    delete aTimer;
    delete dTimer;
    delete m_game;
    delete ui;
}

void Dialog::tryRender() {
    this->update();
}
void Dialog::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_R)
    {

    }
}
void Dialog::keyReleaseEvent(QKeyEvent*event){
    if(event->key() == Qt::Key_R)
    {
        Game* lastgame = m_game->readGame();
        if(lastgame!=nullptr)
        {
            lastgame->m_currentTurn=1;
            lastgame->m_allBallsStill=true;//still got problem
//            lastgame->m_allBallsStill = !lastgame->m_allBallsStill;
            Game * tmp = m_game;
            lastgame->SetPlayersNum(m_game->m_playersNum);
            m_game = lastgame;
            delete tmp;
        }
    }
}
void Dialog::nextAnim() {
    m_game->animate(1.0/(double)animFrameMS);
}

void Dialog::paintEvent(QPaintEvent *)
{
    if(m_gameStarted){
        QPainter painter(this);
        m_game->render(painter);
    }
}

void Dialog::mousePressEvent(QMouseEvent* event) {
    evalAllEventsOfTypeSpecified(MouseEventable::EVENTS::MouseClickFn, event);
}

void Dialog::mouseReleaseEvent(QMouseEvent* event) {
    evalAllEventsOfTypeSpecified(MouseEventable::EVENTS::MouseRelFn, event);
    m_game->m_allBallsStill = false;
}
void Dialog::mouseMoveEvent(QMouseEvent* event) {
    evalAllEventsOfTypeSpecified(MouseEventable::EVENTS::MouseMoveFn, event);
}

void Dialog::evalAllEventsOfTypeSpecified(MouseEventable::EVENTS t, QMouseEvent *event) {

    if(m_game->PlayerCanTakeAction())
    {
        // handle all the clicky events, and remove them if they've xPIRED
        MouseEventable::EventQueue& Qu = m_game->getEventFns();
        qDebug() << Qu.size();
        for (ssize_t i = Qu.size()-1; i >= 0; i--) {
            if (auto spt = (Qu.at(i)).lock()) {
                if (spt->second == t) {
//                    if(spt->second == MouseEventable::EVENTS::MouseRelFn){
//                        m_game->SwitchTurn();
//                    }
                    spt->first(event);
                }
            } else {
                // remove this element from our vector
                Qu.erase(Qu.begin() + i);
            }
        }

    }


}

void Dialog::startGame(int difficulty){
    m_game->setDifficulty(difficulty);
    delete m_easy;
    delete m_normal;
    delete m_hard;
    aTimer->start(animFrameMS);
    dTimer->start(drawFrameMS);
    m_gameStarted = true;
}

void Dialog::drawMenu()
{

}
