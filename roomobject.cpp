#include "mainwindow.h"

RoomObject::RoomObject(const QString& spritePath) :
    QGraphicsPixmapItem()
{
    const QPixmap pm = QPixmap(spritePath);
    this->setPixmap(pm);
    this->paramList = QList<QString>();
    this->sprite = spritePath;
}

RoomObject::RoomObject(const RoomObject& ro) : QGraphicsPixmapItem()
{
    this->className = ro.className;
    this->sprite = ro.sprite;
    this->depth = ro.depth;
    this->paramList = ro.paramList;
    const QPixmap pm = QPixmap(this->sprite);
    this->setPixmap(pm);
}
