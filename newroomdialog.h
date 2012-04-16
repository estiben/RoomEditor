#ifndef NEWROOMDIALOG_H
#define NEWROOMDIALOG_H

#include <QDialog>

namespace Ui {
    class NewRoomDialog;
}

class NewRoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewRoomDialog(QWidget *parent = 0);
    ~NewRoomDialog();

private:
    Ui::NewRoomDialog *ui;
};

#endif // NEWROOMDIALOG_H
