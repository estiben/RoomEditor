#ifndef ROOMOBJECT_H
#define ROOMOBJECT_H

#include <QPixmap>

class RoomObject : public QPixmap
{

public:
    explicit RoomObject(const QString &spritePath);
    QString className;
    qint32 xPos;
    qint32 yPos;
    qint32 depth;
    QString params;//parameters not including x, y, and depth
    QString sprite;

};

#endif // ROOMOBJECT_H
