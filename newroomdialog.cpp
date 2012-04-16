#include "newroomdialog.h"
#include "ui_newroomdialog.h"

NewRoomDialog::NewRoomDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewRoomDialog)
{
    ui->setupUi(this);
}

NewRoomDialog::~NewRoomDialog()
{
    delete ui;
}
