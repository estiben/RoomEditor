#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>

Room::Room(QWidget *parent, MainWindow *parentWindow) : QGraphicsScene(parent)
{
    roomX = (qint32)640;
    roomY = (qint32)480;
    this->setSceneRect(0, 0, 640, 480);
    this->parent = parent;
    this->parentWindow = parentWindow;
    this->objects = new QList<RoomObject*>();
    this->tiles = new QList<RoomObject*>();
}

void Room::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        int mousex = event->scenePos().x();
        int mousey = event->scenePos().y();
        if (parentWindow->ui->checkBoxSnapToGrid->checkState())
        {
            mousex -= (mousex % parentWindow->ui->lineEditGrid->text().toInt());
            mousey -= (mousey % parentWindow->ui->lineEditGrid->text().toInt());
        }

        //get pointer to the selected RoomObject in the list widget
        RoomObject* selectedObj = this->parentWindow->objectPool.value(this->parentWindow->ui->listWidgetObjects->currentItem()->text());

        //make a copy of the selected RoomObject
        RoomObject* ro = new RoomObject(*selectedObj);

        objects->append(ro);
        this->addItem(ro);

        ro->setPos(mousex, mousey);
        ro->setZValue(0);
    }
}

void Room::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        //Remove the top-most object, if it is a RoomObject
        if (this->itemAt(event->scenePos())->type() == RoomObject::Type)
        {
            this->removeItem(this->itemAt(event->scenePos()));
        }
    }
}
