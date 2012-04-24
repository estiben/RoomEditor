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
    QString sprite;
    QList<QString> paramList;//Not including x, y, or depth

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


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //The possible objects you can place in the room. The key is the class name
    QMap<QString, RoomObject*> objectPool;
    //The possible backgrounds you can place in the room
    QList<QString> backgroundPool;

    QString roomFileName;
    qint32 roomX;
    qint32 roomY;

    Ui::MainWindow *ui;

private:
    void GenerateGrid();
    Room *roomScene;
    QGraphicsScene *backgroundPreview;
    QString objectFile;
    QGraphicsRectItem *roomBorderRect;
    QList<QGraphicsLineItem*> gridLines;

private slots:
    void ApplyBackground();
    void ApplyRoomSize();
    void ClearBackground();
    void LoadObjects();
    void NewRoom();
    void SaveRoom();
    void SetupParamTable(const QString&);
    void ShowBackgroundPreview(const QString&);
    void ShowGrid(bool show);
};

#endif // MAINWINDOW_H
