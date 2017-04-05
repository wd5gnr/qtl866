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
    virtual ~OptDialog();

protected:
    virtual void accept();

private slots:
    void restoreDefaults();

private:
    Ui::OptDialog *ui;
};

#endif // OPTDIALOG_H
