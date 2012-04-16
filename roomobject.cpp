#include "mainwindow.h"

RoomObject::RoomObject(const QString& spritePath) :
    QGraphicsPixmapItem()
{
    const QPixmap pm = QPixmap(spritePath);
    this->setPixmap(pm);
    this->paramList = QList<QString>();
}

RoomObject::RoomObject(const RoomObject& ro) : QGraphicsPixmapItem()
{
    this->className = ro.className;
    this->params = ro.params;
    this->sprite = ro.sprite;
    this->depth = ro.depth;
    const QPixmap pm = QPixmap(this->sprite);
    this->setPixmap(pm);
}
