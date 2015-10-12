#ifndef MOBRADARWIDGET_HPP
#define MOBRADARWIDGET_HPP

#include <QWidget>
#include <QMap>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QColor>

#include <QDebug>

namespace Ui {
class MobRadarWidget;
}

enum class MobType
{
    Creeper      = 50,
    Skeleton     = 51,
    Spider       = 52,
    GiantZombie  = 53,
    Zombie       = 54,
    Slime        = 55,
    Ghast        = 56,
    ZombiePigman = 57,
    Enderman     = 58,
    CaveSpider   = 59,
    Silverfish   = 60,
    Blaze        = 61,
    MagmaCube    = 62,
    EnderDragon  = 63,
    Wither       = 64,
    Bat          = 65,
    Witch        = 66,
    Endermite    = 67,
    Guardian     = 68,
    Shulker      = 69,
    Pig          = 90,
    Sheep        = 91,
    Cow          = 92,
    Chicken      = 93,
    Squid        = 94,
    Wolf         = 95,
    Mooshroom    = 96,
    Snowman      = 97,
    Ocelot       = 98,
    IronGolem    = 99,
    Horse        = 100,
    Rabbit       = 101,
    Villager     = 120,

    Undefined    = -1
};

//----------------------------------------------------------------------------//

class MobInfo
{
public:
    MobInfo(int id, MobType mobType, int x, int y, QGraphicsScene * scene);
    ~MobInfo();

    MobInfo(const MobInfo & other) = delete;

    QPointF getPos() const;
    void setPos(QPointF pos);

private:
    void updateLabel();

private:
    static const QMap<MobType, QColor> m_mobColors;

    int                    m_id;
    QGraphicsEllipseItem * m_circle;
    QGraphicsTextItem    * m_label;

    QGraphicsScene       * m_scene;
};

//----------------------------------------------------------------------------//

class MouseEventCatcher : public QObject
{
    Q_OBJECT

public:
    MouseEventCatcher(QGraphicsView * view, QObject * parent = nullptr);

private:
    virtual bool eventFilter(QObject *target, QEvent *event) override;

private:
    QGraphicsView * m_view;
};

//----------------------------------------------------------------------------//

class MobRadarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MobRadarWidget(int centerX, int centerZ, int pixelsPerCube, QWidget *parent = nullptr);
    ~MobRadarWidget();

public slots:
    void onEntitySpawned(int entityId, int mobType, int x, int y, int z);
    void onEntityDestroyed(int entityId);
    void onEntityPositionChanged(int entityId, int x, int y, int z, bool isRelative);

private slots:
    void onTimer();

    void on_btnZoomIn_clicked();
    void on_btnZoomOut_clicked();

private:
    Ui::MobRadarWidget     * ui;
    QGraphicsScene         * m_graphicsScene;

    // Game coordinates in widget center
    int m_centerX;
    int m_centerZ;
    // Scale
    int m_pixelsPerCube;

    QMap<int, MobInfo *>  m_mobs;

    QTimer m_timer;
};

#endif // MOBRADARWIDGET_HPP
