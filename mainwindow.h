#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QGraphicsScene>
#include <QFile>
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>
#include <Qt>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QListWidget>

namespace Ui {
    class MainWindow;
}
class MainWindow;

class RoomObject : public QGraphicsPixmapItem
{
public:
    RoomObject(const QString &spritePath);
    RoomObject(const RoomObject& ro); //copy constructor
    QString className;
    qint32 depth;
    QString params;//A string containing parameters; Should always contain at least x, y, and depth.
    QString sprite;
    QList<QString> paramList;

};

class Room : public QGraphicsScene
{
    Q_OBJECT
public:
    Room(QWidget*, MainWindow*);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    QString roomFileName;
    qint32 roomX;
    qint32 roomY;
    MainWindow *parentWindow;
    QList<RoomObject*> *objects;
    QList<RoomObject*> *tiles;
    //Name of the background which is tiled across the room
    QString tiledBackground;

    QWidget *parent;
    QGraphicsView *graphicsView;
};

class RoomGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit RoomGraphicsScene(QWidget *parent = 0);
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QWidget *parent;

signals:

public slots:

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //The possible objects you can place in the room
    QMap<QString, RoomObject*> objectPool;
    //The possible backgrounds you can place in the room
    QList<QString> backgroundPool;

    QString roomFileName;
    qint32 roomX;
    qint32 roomY;
    //List of objects placed in the room
    //QList<RoomObject> *objects;
    //List of background tiles placed in the room
    QList<QString> *tiles;
    Ui::MainWindow *ui;

private:
    void GenerateGrid();
    Room *roomScene;
    QGraphicsScene *backgroundPreview;
    QString objectFile;
    QGraphicsRectItem *roomBorderRect;
    QList<QGraphicsLineItem*> gridLines;

private slots:
    void NewRoom();
    void SaveRoom();
    void LoadObjects();
    void ApplyRoomSize();
    void ShowGrid(bool show);
    void ApplyBackground();
    void ClearBackground();
    void ShowBackgroundPreview(const QString&);
};

#endif // MAINWINDOW_H
