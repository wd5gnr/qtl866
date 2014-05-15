#include "optdialog.h"
#include "ui_optdialog.h"
#include "mainwindow.h"

static char defpgm[]="minipro";
static char defed[]="binhexedit -r";

OptDialog::OptDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptDialog)
{
    MainWindow *main=(MainWindow *)parent;
    ui->setupUi(this);
    ui->exec->setText(main->settings->value("options/command",defpgm).toString());
    ui->editor->setText(main->settings->value("options/editcmd",defed).toString());
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
    main->settings->setValue("options/editcmd",ui->editor->text());
}

void OptDialog::on_resetbtn_clicked()
{
    ui->exec->setText(defpgm);
    ui->editor->setText(defed);
}
