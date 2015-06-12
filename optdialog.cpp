#include "optdialog.h"
#include "ui_optdialog.h"
#include "mainwindow.h"

static const char *defpgm = "minipro";
static const char *defed = "binhexedit -r";

OptDialog::OptDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptDialog)
{
    QSettings settings;
    ui->setupUi(this);
    ui->exec->setText(settings.value("options/command",defpgm).toString());
    ui->editor->setText(settings.value("options/editcmd",defed).toString());
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
    QSettings settings;
    settings.setValue("options/command",ui->exec->text());
    settings.setValue("options/editcmd",ui->editor->text());
}

void OptDialog::on_resetbtn_clicked()
{
    ui->exec->setText(defpgm);
    ui->editor->setText(defed);
}
