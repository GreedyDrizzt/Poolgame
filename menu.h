#ifndef MENU_H
#define MENU_H
#include <QMainWindow>
#include <QPushButton>
//#include "game.h"
class Game;

namespace Ui {
    class Menu;
 }
class Menu : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Menu(Game *game, QWidget *parent=0);

    protected:

    #ifndef QT_NO_CONTEXTMENU
        //TODO:因为报错我把这句注释掉了
        //void contextMenuEvent(QContextMenuEvent *event) override;
    #endif // QT_NO_CONTEXTMENU
    private slots:
        void easyButton();
        void normalButton();
        void hardButton();
    private:
        QPushButton *m_button;
        QPushButton *m_easy;
        QPushButton *m_normal;
        QPushButton *m_hard;
        Game* m_game;
};
#endif // MENU_H
