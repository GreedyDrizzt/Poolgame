#include "menu.h"
#include "game.h"
#include "dialog.h"

Menu::Menu(Game *game, QWidget *parent):QMainWindow(parent), m_game(game)
{
    m_easy= new QPushButton("Easy", this);
    m_normal = new QPushButton("normal", this);
    m_hard = new QPushButton("hard",this);
    m_easy->setGeometry(QRect(QPoint(100,50),QSize(200,50)));
    m_normal->setGeometry(QRect(QPoint(100,50),QSize(200,50)));
    m_hard->setGeometry(QRect(QPoint(100,50),QSize(200,50)));
    connect(m_easy, SIGNAL(released()),this, SLOT(easyButton()));
    connect(m_normal, SIGNAL(released()),this, SLOT(normalButton()));
    connect(m_hard, SIGNAL(released()),this,SLOT(hardButton()));
}

void Menu::easyButton(){
    m_game->setDifficulty(0);
    m_easy->resize(100,100);
}
void Menu::normalButton(){
    m_game->setDifficulty(1);
    m_easy->resize(100,100);
}
void Menu::hardButton(){
    m_game->setDifficulty(2);
    m_easy->resize(100,100);
}
