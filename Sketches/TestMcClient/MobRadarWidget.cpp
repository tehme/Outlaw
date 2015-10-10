#include "MobRadarWidget.hpp"
#include "ui_MobRadarWidget.h"
#include <QPainter>
#include <QDebug>


MobRadarWidget::MobRadarWidget(int centerX, int centerZ, int pixelsPerCube, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MobRadarWidget),
    m_graphicsScene(new QGraphicsScene(this)),
    m_centerX(centerX),
    m_centerZ(centerZ),
    m_pixelsPerCube(pixelsPerCube)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(m_graphicsScene);
    ui->graphicsView->scale(1, 1);
}

MobRadarWidget::~MobRadarWidget()
{
    delete ui;
}

void MobRadarWidget::onEntitySpawned(int entityId, int x, int y, int z)
{
    if(m_mobs.size() >= 5)
        return;

    m_mobs.insert(entityId, MobPosition(entityId, x, y, z));

    MobPosition & mob = m_mobs[entityId];

    mob.m_position = m_graphicsScene->addEllipse(x, z, 5, 5);
    mob.m_label = m_graphicsScene->addText("BLANK LABEL");
    mob.updateLabel(); // todo: create if none

    // Scale back units
    m_mobs[entityId].m_position->setScale(1);
    m_mobs[entityId].m_label->setScale(1);

    qDebug() << "Entity spawned! Id:" << entityId << "; coodrinates:"<< x << y << z;
}

void MobRadarWidget::onEntityPositionChanged(int entityId, int x, int y, int z, bool isRelative)
{
    if(!m_mobs.contains(entityId))
    {
        qDebug() << "Trying to change position of entity" << entityId << "that is not in the list.";
        return;
    }

    MobPosition & currentPosition = m_mobs[entityId];

    if(!isRelative)
    {
        currentPosition.m_x = x;
        currentPosition.m_y = y;
        currentPosition.m_z = z;
        qDebug() << "Changing position of entity" << entityId << "absolutely to" << x << y << z;
    }
    else
    {
        currentPosition.m_x += x;
        currentPosition.m_y += y;
        currentPosition.m_z += z;

        qDebug() << "Changing position of entity" << entityId << "relatively to" << x << y << z;
    }

    if(currentPosition.m_position)
    {
        currentPosition.m_position->setPos(currentPosition.m_x, currentPosition.m_z);
        qDebug() << "Setting visual position for entity" << entityId << "to" << currentPosition.m_x << currentPosition.m_z;
        currentPosition.updateLabel();
        qDebug() << "Entity position changed! Id:" << entityId;
    }
    else
    {
        qDebug() << "Trying to set pos for entity without graphics item!";
    }


}

void MobRadarWidget::onEntityDestroyed(int entityId)
{
    if(m_mobs.contains(entityId))
    {
        m_graphicsScene->removeItem(m_mobs[entityId].m_position);
        delete m_mobs[entityId].m_position;

        m_graphicsScene->removeItem(m_mobs[entityId].m_label);
        delete m_mobs[entityId].m_label;

        m_mobs.remove(entityId);
        qDebug() << "Entity destroyed! Id:" << entityId;
    }
    else
    {
        qDebug() << "Trying to destroy entity" << entityId << "that is not in the list.";
    }
}
