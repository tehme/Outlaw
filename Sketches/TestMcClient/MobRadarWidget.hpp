#ifndef MOBRADARWIDGET_HPP
#define MOBRADARWIDGET_HPP

#include <QWidget>
#include <QMap>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include <QDebug>

namespace Ui {
class MobRadarWidget;
}

struct MobPosition
{
    MobPosition(int entityId = 0, int x = 0, int y = 0, int z = 0) :
        m_entityId(entityId),
        m_x(x),
        m_y(y),
        m_z(z),
        m_position(nullptr),
        m_label(nullptr)
    {
    }

    void updateLabel()
    {
        if(m_label)
        {
            const QString mobLabel = QString("Id %1: %2 %3 %4").arg(m_entityId).arg(m_x).arg(m_y).arg(m_z);
            m_label->setPlainText(mobLabel);
            m_label->setPos(m_x + 5, m_z - 10);
            qDebug() << "Setting label position for entity" << m_entityId << "to" << (m_x + 5) << (m_z - 10);
        }
        else
        {
//            qDebug() << "Entity" << m_entityId << "has no label!";
        }
    }

    int                    m_entityId;
    int                    m_x;
    int                    m_y;
    int                    m_z;
    QGraphicsEllipseItem * m_position;
    QGraphicsTextItem    * m_label;
};

class MobRadarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MobRadarWidget(int centerX, int centerZ, int pixelsPerCube, QWidget *parent = nullptr);
    ~MobRadarWidget();

public slots:
    void onEntitySpawned(int entityId, int x, int y, int z);
    void onEntityDestroyed(int entityId);
    void onEntityPositionChanged(int entityId, int x, int y, int z, bool isRelative);


private:
    Ui::MobRadarWidget     * ui;
    QGraphicsScene         * m_graphicsScene;

    // Game coordinates in widget center
    int m_centerX;
    int m_centerZ;
    // Scale
    int m_pixelsPerCube;

    QMap<int, MobPosition>  m_mobs;
};

#endif // MOBRADARWIDGET_HPP
