#ifndef ROOM_H
#define ROOM_H

#include <QString>
#include <QList>
#include <QFile>
#include <QFileDialog>
#include <roomobject.h>

//xPos, yPos, and depth are not included in params
/*struct RoomObject
{
    QString className;
    qint32 xPos;
    qint32 yPos;
    qint32 depth;
    QString params;//parameters not including x, y, and depth
    QString sprite;
};*/

class Room
{
public:
    Room();
    void Save(QWidget*);
    void AddObject(QString, qint32, qint32);

    QString roomFileName;
    qint32 roomX;
    qint32 roomY;
    QString *tiledBackground;
    QList<RoomObject> *objects;
    QList<RoomObject> *tiles;
};

#endif // ROOM_H
