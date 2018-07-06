/**
  *     Written by Tim Burr
  *     2018/05/18
  */

#include "dialog.h"
#include "game.h"
#include "utils.h"
#include "gamebuilder.h"
#include "stagetwobuilder.h"
#include "stagethreebuilder.h"
#include "regulation.h"
#include "menu.h"
#include <QApplication>
#include <QFile>
#include <iostream>
#include <QString>
#include <QJsonObject>
#include <ctime>
#include <QJsonDocument>

QJsonObject loadConfig() {
    // load json from config file
    QFile conf_file(config_path);
    conf_file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = conf_file.readAll();
    conf_file.close();
    QJsonObject config = QJsonDocument::fromJson(content.toUtf8()).object();
    return config;
}

int main(int argc, char *argv[])
{
    QJsonObject conf = loadConfig();
    QApplication a(argc, argv);
    // seed our RNG
    srand(time(nullptr));

    // create our game based on our config
    GameDirector director(&conf);
    // use builder2 if we're stage two (defaults to false), otherwise no
    /*
    The only change to the config is the addition of a 'stage3' flag which functions identically to the 'stage2' flag from assignment 2.
    If both flags are present and true then the later stage flag takes precedent.
    If 'stage3' is true then it can also have as many other changes as you want to facilitate your extension(s).
    */
    int stage2Index = -1;
    int stage3Index = -1;
    int i=0;
    for(auto key:conf.keys())
    {
        if(key=="stage2")
        {
            stage2Index = i;
        }
        if(key=="stage3")
        {
            stage3Index = i;
        }
        i++;
    }
    if(conf.value("stage2").toBool()&&conf.value("stage3").toBool())
    {
        if(stage2Index>stage3Index)
        {
            director.setBuilder(new StageTwoBuilder());
        }
        else if(stage3Index>stage2Index)
        {
            director.setBuilder(new StageThreeBuilder());
        }
        else
        {
            director.setBuilder(new StageOneBuilder());
        }
    }
    else if(conf.value("stage2").toBool())
    {
        director.setBuilder(new StageTwoBuilder());
    }
    else if(conf.value("stage3").toBool())
    {
        director.setBuilder(new StageThreeBuilder());
    }
    else
    {
        director.setBuilder(new StageOneBuilder());
    }

//    if (conf.value("stage2").toBool(false) == true) {
//       director.setBuilder(new StageTwoBuilder());
//    } else {
//        // set and transfer ownership of this builder to the director
//        director.setBuilder(new StageOneBuilder());
//    }
    Game* game = director.createGame();

    Dialog w(game, stage3Index>stage2Index, nullptr);
    w.show();

    // display our dialog that contains our game and run
    return a.exec();
}
