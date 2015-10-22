#include "MobRadarWidget.hpp"
#include "ui_MobRadarWidget.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

//----------------------------------------------------------------------------//

const QMap<MobType, QColor> MobInfo::m_mobColors =
{
    {MobType::Zombie,   QColor(0x62, 0x84, 0x50)},
    {MobType::Creeper,  QColor(0x5d, 0xcd, 0x4d)},
    {MobType::Skeleton, QColor(0x99, 0x99, 0x99)},
};


MobInfo::MobInfo(int id, MobType mobType, int x, int y, QGraphicsScene * scene) :
    m_id(id),
    m_circle(new QGraphicsEllipseItem(0, 0, 5, 5)),
    m_label(new QGraphicsTextItem(m_circle)),
    m_scene(scene)
{
    m_label->setPos(5, -5);

    if(m_mobColors.contains(mobType))
    {
        m_circle->setBrush(QBrush(m_mobColors[mobType]));
    }

    // Do not scale when view is zoomed
    m_circle->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);

    m_scene->addItem(m_circle);

    setPos(QPointF(x, y));
}

MobInfo::~MobInfo()
{
    qDebug() << "Item" << m_id << "dtor!";
    m_scene->removeItem(m_circle);
    delete m_circle;
    m_circle = nullptr;
    m_label = nullptr;
    m_scene = nullptr;
}

QPointF MobInfo::getPos() const
{
    if(m_circle)
    {
        return m_circle->pos();
    }

    return QPointF();
}

void MobInfo::setPos(QPointF pos)
{
    if(m_circle)
    {
        m_circle->setPos(pos);
        updateLabel();
    }
}

void MobInfo::updateLabel()
{
    if(m_label)
    {
        QString labelText = QString("ID %1; x: %2; y: ?; z: %3").arg(m_id).arg(m_circle->x()).arg(m_circle->y());
        m_label->setPlainText(labelText);
    }
}

//----------------------------------------------------------------------------//

MouseEventCatcher::MouseEventCatcher(QGraphicsView * view, QObject * parent) :
    QObject(parent),
    m_view(view)
{
    m_view->installEventFilter(this);
}

bool MouseEventCatcher::eventFilter(QObject * target, QEvent * event)
{
    if(target == m_view && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent * mouseEvent = dynamic_cast<QMouseEvent *>(event);
        m_view->centerOn(m_view->mapToScene(mouseEvent->x(), mouseEvent->y()));

        return true;
    }

    return false;
}

//----------------------------------------------------------------------------//

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
    MouseEventCatcher * mouseEventCatcher = new MouseEventCatcher(ui->graphicsView, ui->graphicsView);
//    ui->graphicsView->scale(2, 2);

    m_timer.setInterval(1000);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    m_timer.start();
}

MobRadarWidget::~MobRadarWidget()
{
    qDeleteAll(m_mobs);
    m_mobs.clear();

    delete ui;
}

void MobRadarWidget::onEntitySpawned(int entityId, int mobType, int x, int y, int z)
{
//    if(m_mobs.size() >= 5)
//        return;

    MobType trueMobType = static_cast<MobType>(mobType);
    m_mobs.insert(entityId, new MobInfo(entityId, trueMobType, x, z, m_graphicsScene));

    qDebug() << "Entity spawned! Id:" << entityId << "; coodrinates:"<< x << y << z;
}

void MobRadarWidget::onEntityPositionChanged(int entityId, int x, int y, int z, bool isRelative)
{
    if(!m_mobs.contains(entityId))
    {
        qDebug() << "Trying to change position of entity" << entityId << "that is not in the list.";
        return;
    }

    MobInfo * currentMob = m_mobs[entityId];
    QPointF pos = currentMob->getPos();

    if(!isRelative)
    {
        pos.setX(x);
        pos.setY(z);
//        qDebug() << "Changing position of entity" << entityId << "absolutely to" << x << y << z;
    }
    else
    {
        pos.rx() += x;
        pos.ry() += z;

//        qDebug() << "Changing position of entity" << entityId << "relatively to" << x << y << z;
    }

    currentMob->setPos(pos);
}

void MobRadarWidget::onEntityDestroyed(int entityId)
{
    if(m_mobs.contains(entityId))
    {
        delete m_mobs[entityId];
        m_mobs.remove(entityId);
        qDebug() << "Entity destroyed! Id:" << entityId;
    }
    else
    {
        qDebug() << "Trying to destroy entity" << entityId << "that is not in the list.";
    }
}

void MobRadarWidget::onTimer()
{
}

void MobRadarWidget::on_btnZoomIn_clicked()
{
    ui->graphicsView->scale(1.2, 1.2);
}

void MobRadarWidget::on_btnZoomOut_clicked()
{
    ui->graphicsView->scale(1 / 1.2, 1 / 1.2);
}
