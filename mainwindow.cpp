#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionNew_Room, SIGNAL(triggered()), this, SLOT(NewRoom()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(SaveRoom()));
    connect(ui->btnLoadObjectFile, SIGNAL(clicked()), this, SLOT(LoadObjects()));
    connect(ui->btnRoomSizeApply, SIGNAL(clicked()), this, SLOT(ApplyRoomSize()));
    connect(ui->checkBoxShowGrid, SIGNAL(toggled(bool)), this, SLOT(ShowGrid(bool)));
    connect(ui->btnSetBackground, SIGNAL(clicked()), this, SLOT(ApplyBackground()));
    connect(ui->btnClearBackground, SIGNAL(clicked()), this, SLOT(ClearBackground()));
    connect(ui->listWidgetBackgrounds, SIGNAL(currentTextChanged(const QString&)), this, SLOT(ShowBackgroundPreview(const QString&)));
    connect(ui->listWidgetObjects, SIGNAL(currentTextChanged(const QString&)), this, SLOT(SetupParamTable(const QString&)));

    backgroundPreview = new QGraphicsScene();
    ui->graphicsViewBackground->setScene(backgroundPreview);

    QValidator *intValidator = new QIntValidator(this);
    ui->lineEditDepth->setValidator(intValidator);
    ui->lineEditGrid->setValidator(intValidator);
    ui->lineEditRoomX->setValidator(intValidator);
    ui->lineEditRoomY->setValidator(intValidator);
}


//Set the room dimensions based on values in input boxes
void MainWindow::ApplyRoomSize()
{
    roomScene->setSceneRect(0, 0, ui->lineEditRoomX->text().toInt(), ui->lineEditRoomY->text().toInt());
    roomBorderRect->setRect(0, 0, roomScene->width(), roomScene->height());
    GenerateGrid();
}


//Set the value of the tiled background
void MainWindow::ApplyBackground()
{
    if (ui->listWidgetBackgrounds->currentItem() != NULL)
    {
        roomScene->tiledBackground = ui->listWidgetBackgrounds->currentItem()->text();
        roomScene->setBackgroundBrush(QPixmap(ui->listWidgetBackgrounds->currentItem()->text()));
    }
    else
    {
        qDebug() << "WARNING: Selected background is NULL";
    }
}


//Clear the value of the tiled background
void MainWindow::ClearBackground()
{
    roomScene->tiledBackground = "";
}


//Deletes all gridlines, then recreates them based on the room size
void MainWindow::GenerateGrid()
{
    //Delete all grid lines from the room scene
    QList<QGraphicsLineItem*>::iterator i;
    for (i = gridLines.begin(); i != gridLines.end(); ++i)
    {
        roomScene->removeItem(*i);
    }
    gridLines.clear();

    //Generate grid lines
    for (int row = 0; row < roomScene->height(); row += ui->lineEditGrid->text().toInt())
    {
        gridLines.append(roomScene->addLine(0, row, roomScene->width(), row));
        gridLines.last()->setVisible(ui->checkBoxShowGrid->checkState());
        for (int col = 0; col < roomScene->width(); col += ui->lineEditGrid->text().toInt())
        {
            gridLines.append(roomScene->addLine(col, 0, col, roomScene->height()));
            gridLines.last()->setVisible(ui->checkBoxShowGrid->checkState());
        }
    }
}


//Load game objects (Python classes)
void MainWindow::LoadObjects()
{
    objectFile = QFileDialog::getOpenFileName(this, QString("Select Background File"), QString("/home"), QString(""));
    QFile* xmlFile = new QFile(objectFile);
    xmlFile->open(QIODevice::ReadOnly);

    ui->listWidgetObjects->clear();
    ui->listWidgetBackgrounds->clear();

    RoomObject* roomObj; //Temp pointer for new RoomObject objects

    QXmlStreamReader reader(xmlFile);
    while (!reader.atEnd())
    {
        QXmlStreamReader::TokenType node = reader.readNext();
        if (node == QXmlStreamReader::StartDocument)
        {
            continue;
        }
        if (node == QXmlStreamReader::StartElement)
        {
            if (reader.name().toString() == "object")
            {
                ui->listWidgetObjects->addItem(new QListWidgetItem(QIcon(reader.attributes().value("sprite").toString()), reader.attributes().value("class").toString()));

                roomObj = new RoomObject(reader.attributes().value("sprite").toString());
                roomObj->className = reader.attributes().value("class").toString();
                roomObj->sprite = reader.attributes().value("sprite").toString();

                //objectPool.insert(reader.attributes().value("class").toString(), roomObj);
            }
            else if (reader.name().toString() == "param")
            {
                QString paramName = reader.readElementText();
                if (paramName == "")
                {
                    QMessageBox errorMessageBox;
                    errorMessageBox.setIcon(QMessageBox::Critical);
                    errorMessageBox.setText("ERROR: Empty param name.");
                    errorMessageBox.exec();
                }
                roomObj->paramList.append(paramName);
            }
            else if (reader.name().toString() == "background")
            {
                ui->listWidgetBackgrounds->addItem(new QListWidgetItem(QIcon(reader.attributes().value("path").toString()), reader.attributes().value("path").toString()));
                backgroundPool.append(reader.attributes().value("path").toString());
            }
        }
        if (node == QXmlStreamReader::EndElement)
        {
            if (reader.name().toString() == "object")
            {
                objectPool.insert(roomObj->className, roomObj);
            }
        }
    }
    ui->labelObjectFilePath->setText(objectFile);
    ui->labelObjectFilePath->setToolTip(objectFile);

    if (ui->listWidgetBackgrounds->count() > 0)
    {
        ui->btnSetBackground->setEnabled(true);
    }
}


//Creates a new room
void MainWindow::NewRoom()
{
    ui->lineEditRoomX->setText("640");
    ui->lineEditRoomY->setText("480");
    roomScene = new Room(this, this);
    ui->graphicsView->setScene(roomScene);

    roomBorderRect = roomScene->addRect(0, 0, roomScene->width(), roomScene->height(), QPen(QColor(255,0,0)));
    ApplyRoomSize();

    //some options are initially disabled when no room is open
    ui->checkBoxShowGrid->setEnabled(true);
    ui->checkBoxSnapToGrid->setEnabled(true);
    ui->lineEditGrid->setEnabled(true);
    ui->lineEditRoomX->setEnabled(true);
    ui->lineEditRoomY->setEnabled(true);
    ui->labelGrid->setEnabled(true);
    ui->labelWidth->setEnabled(true);
    ui->labelHeight->setEnabled(true);
}


//Save the room to a file
void MainWindow::SaveRoom()
{
    roomFileName = QFileDialog::getSaveFileName(this, QString("Save Room"), QString("/home"), QString(""));
    QFile roomFile(roomFileName);
    roomFile.open(QIODevice::WriteOnly);

    QXmlStreamWriter writer(&roomFile);
    writer.writeStartElement(QString("room"));
    writer.writeAttribute(QString("width"), QString::number(roomScene->width()));
    writer.writeAttribute(QString("height"), QString::number(roomScene->height()));
    writer.writeAttribute(QString("background"), roomScene->tiledBackground);

    //Write tile information
    writer.writeStartElement(QString("tiles"));
    QList<RoomObject*>::iterator i;
    for (i = roomScene->tiles->begin(); i != roomScene->tiles->end(); ++i)
    {
        //out << (*i)->className;
        //out << (*i)->x();

    }
    writer.writeEndElement();//tiles

    //Write object information
    writer.writeStartElement(QString("objects"));
    for (i = roomScene->objects->begin(); i != roomScene->objects->end(); ++i)
    {
        writer.writeStartElement(QString("object"));
        writer.writeAttribute(QString("class"), QString((*i)->className));

        writer.writeTextElement(QString("param"), QString::number((*i)->x()));
        writer.writeTextElement(QString("param"), QString::number((*i)->y()));
        writer.writeTextElement(QString("param"), QString::number((*i)->zValue()));

        //Iterate over additional RoomObject parameters
        QList<QString>::iterator param;
        for (param = (*i)->paramList.begin(); param != (*i)->paramList.end(); ++param)
        {
            writer.writeTextElement(QString("param"), QString(*param));
        }

        writer.writeEndElement();//object

    }
    writer.writeEndElement();//objects
    writer.writeEndElement();//room
}


//Set rows in the object param table
void MainWindow::SetupParamTable(const QString& className)
{
    RoomObject *selectedObject = objectPool.value(className);
    ui->tableWidgetProperties->clear();
    ui->tableWidgetProperties->setRowCount(selectedObject->paramList.length());
    ui->tableWidgetProperties->setHorizontalHeaderLabels(QStringList("Value"));
    ui->tableWidgetProperties->setVerticalHeaderLabels(QStringList(selectedObject->paramList));
    ui->tableWidgetProperties->verticalHeader()->setVisible(true);
}


//Show a preview of the background in the background preview view
void MainWindow::ShowBackgroundPreview(const QString& file)
{
    backgroundPreview->clear();
    backgroundPreview->addPixmap(QPixmap(file));
}


//Set the visibility of grid lines
void MainWindow::ShowGrid(bool show)
{
    QList<QGraphicsLineItem*>::iterator i;
    for (i = gridLines.begin(); i != gridLines.end(); ++i)
    {
        (*i)->setVisible(show);
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
