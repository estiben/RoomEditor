#ifndef ROOMGRAPHICSSCENE_H
#define ROOMGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <roomobject.h>

class RoomGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit RoomGraphicsScene(QObject *parent = 0);

private:
    QWidget *parent;

signals:

public slots:

};

#endif // ROOMGRAPHICSSCENE_H
