#include "table.h"
#include "ball.h"
#include "pocket.h"
#include <iostream>

void StageOneTable::render(QPainter &painter, const QVector2D& offset) {
    // our table colour
    painter.setBrush(m_brush);
    // draw table
    painter.drawRect(offset.x(), offset.y(), this->getWidth(), this->getHeight());
}

void StageTwoTable::render(QPainter &painter, const QVector2D& offset) {
    // our table colour
    painter.setBrush(m_brush);
    // draw table
    painter.drawRect(offset.x(), offset.y(), this->getWidth(), this->getHeight());

    // render the pockets relative to this table
    for (Pocket* p : m_pockets) {
        p->render(painter, offset);
    }
}
Table& Table::operator=(const Table& lastTable){
    this->setHeight(lastTable.getHeight());
    this->setBrush(lastTable.getBrush());
    this->setWidth(lastTable.getWidth());
    this->setFriction(lastTable.getFriction());
    this->setPocket(lastTable.getPocket());
    //Added By Jiang
    return *this;
}
StageTwoTable::~StageTwoTable() {
    for (Pocket* p : m_pockets) delete p;
}

bool StageTwoTable::sinks(Ball *b) {
    QVector2D absPos = b->getPosition();
    double radius = b->getRadius();
    // check whether any pockets consumes this ball
    for (Pocket* p : m_pockets) {
        // you sunk my scrabbleship
        if (p->contains(absPos, radius)) {
            p->incrementSunk();
            return true;
        }
    }
    return false;
}

StageTwoTable& StageTwoTable::operator=(const StageTwoTable& lastTable){
    this->setHeight(lastTable.getHeight());
    this->setBrush(lastTable.getBrush());
    this->setWidth(lastTable.getWidth());
    this->setFriction(lastTable.getFriction());
    this->setPocket(lastTable.getPocket());
    //Added By Jiang
    for(auto pocket:m_pockets)
    {
        delete pocket;
    }
    this->m_pockets.clear();
    for(auto pocket:lastTable.m_pockets)
    {
        Pocket* newPocket=new Pocket(*pocket);
        this->m_pockets.push_back(newPocket);
    }
    return *this;
}
