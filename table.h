#pragma once

#include <QColor>
#include <QPainter>

#include "pocket.h"

class Ball;

class Table {
protected:
    int m_width;
    int m_height;
    QBrush m_brush;
    double m_friction;
public:
    virtual ~Table() {}
    Table(){}
    Table(const Table& T)
    {
        m_width = T.m_width;
        m_height = T.m_height;
        m_friction = T.m_friction;
        m_brush = T.m_brush;
    }
    Table(int width, int height, QColor colour, double friction) :
        m_width(width), m_height(height),
        m_brush(colour), m_friction(friction) {}
    /**
     * @brief render - draw the table to screen using the specified painter
     * @param painter - painter to use
     */
    virtual void render(QPainter& painter, const QVector2D& offset) = 0;

    int getWidth() const { return m_width; }
    void setWidth(int width){ m_width = width;}
    int getHeight() const { return m_height; }
    void setHeight(int height){ m_height = height;}
    double getFriction() const { return m_friction; }
    void setFriction(double friction){m_friction = friction;}
    virtual std::vector<Pocket*> getPocket() const = 0;
    virtual void setPocket(std::vector<Pocket*> pocket)=0;
    void setBrush(QBrush brush){m_brush = brush;}
    QBrush getBrush() const {return m_brush;}
    virtual bool sinks(Ball*) { return false; }
    Table& operator=(const Table &lastTable);
    virtual Table* clone() = 0;
};

class StageOneTable : public Table
{
public:
    StageOneTable(int width, int height, QColor colour, double friction) :
        Table(width, height, colour, friction) {}
    StageOneTable() {}

    StageOneTable(const StageOneTable& other) : Table(other) {}
    /**
     * @brief render - draw the stageonetable to screen using the specified painter
     * @param painter - painter to use
     */
    void render(QPainter &painter, const QVector2D& offset) override;

    //Added By Jiang
    virtual std::vector<Pocket*> getPocket() const {return std::vector<Pocket*>();}
    virtual void setPocket(std::vector<Pocket*> pocket) {}

    virtual Table *clone(){
        return new StageOneTable(*this);
    }
};

class StageTwoTable : public Table {
protected:
    std::vector<Pocket*> m_pockets;

public:
    StageTwoTable(){}
    StageTwoTable(int width, int height, QColor colour, double friction) :
        Table(width, height, colour, friction) {}

    StageTwoTable(const StageTwoTable& other) :
        Table(other), m_pockets() {
        for (auto pocket : other.m_pockets)
        {
            addPocket(pocket->clone());
        }
    }
    ~StageTwoTable();

    /**
     * @brief render - draw the stageonetable to screen using the specified painter
     * @param painter - painter to use
     */
    void render(QPainter &painter, const QVector2D& offset) override;

    // sinky winky ball
    virtual bool sinks(Ball* b) override;

    /* self explanatory */
    void setPocket(std::vector<Pocket*> pocket){m_pockets=pocket;}
    void addPocket(Pocket* p) { m_pockets.push_back(p); }
    std::vector<Pocket*> getPocket() const { return m_pockets;}
    StageTwoTable& operator =(const StageTwoTable& table);
    virtual Table* clone() override {
        return new StageTwoTable(*this);
    }
};
