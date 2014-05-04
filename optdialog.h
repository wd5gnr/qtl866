#ifndef OPTDIALOG_H
#define OPTDIALOG_H

#include <QDialog>

namespace Ui {
class OptDialog;
}

class OptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptDialog(QWidget *parent = 0);
    ~OptDialog();

private slots:
    void on_OptDialog_accepted();

    void on_buttonBox_accepted();

private:
    Ui::OptDialog *ui;
};

#endif // OPTDIALOG_H
