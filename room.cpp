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
        if (parentWindow->ui->tabMain->currentIndex() > 0)
        {
            int mousex = event->scenePos().x();
            int mousey = event->scenePos().y();
            if (parentWindow->ui->checkBoxSnapToGrid->checkState())
            {
                mousex -= (mousex % parentWindow->ui->lineEditGrid->text().toInt());
                mousey -= (mousey % parentWindow->ui->lineEditGrid->text().toInt());
            }

            RoomObject* ro;

            if (parentWindow->ui->tabMain->currentIndex() == 1)
            {
                //get pointer to the selected RoomObject in the list widget
                RoomObject* selectedObj = this->parentWindow->objectPool.value(this->parentWindow->ui->listWidgetObjects->currentItem()->text());

                //make a copy of the selected RoomObject
                ro = new RoomObject(*selectedObj);

                for (int row = 0; row < parentWindow->ui->tableWidgetProperties->rowCount(); row++)
                {
                    ro->paramList[row] = parentWindow->ui->tableWidgetProperties->model()->data(parentWindow->ui->tableWidgetProperties->model()->index(row, 0)).toString();
                }
                objects->append(ro);
            }
            else if (parentWindow->ui->tabMain->currentIndex() == 2)
            {
                //get pointer to the selected RoomObject in the list widget
                ro = new RoomObject(this->parentWindow->ui->listWidgetBackgrounds->currentItem()->text());
                tiles->append(ro);
            }

            //set RoomObject position
            ro->setPos(mousex, mousey);
            ro->setZValue(parentWindow->ui->lineEditDepth->text().toInt());

            this->addItem(ro);
        }
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
