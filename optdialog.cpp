#include "optdialog.h"
#include "ui_optdialog.h"
#include "mainwindow.h"

OptDialog::OptDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptDialog)
{
    MainWindow *main=(MainWindow *)parent;
    ui->setupUi(this);
    ui->exec->setText(main->settings->value("options/command","minipro").toString());
}

OptDialog::~OptDialog()
{
    delete ui;
}

void OptDialog::on_OptDialog_accepted()
{
}

void OptDialog::on_buttonBox_accepted()
{
    MainWindow *main=(MainWindow *)parent();
    main->settings->setValue("options/command",ui->exec->text());
}
