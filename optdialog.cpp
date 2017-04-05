#include <QPushButton>

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

    QPushButton *restoreButton = ui->buttonBox->button(QDialogButtonBox::RestoreDefaults);
    connect(restoreButton, SIGNAL(clicked()), this, SLOT(restoreDefaults()));
}

OptDialog::~OptDialog()
{
    delete ui;
}

void OptDialog::accept()
{
    QSettings settings;
    settings.setValue("options/command",ui->exec->text());
    settings.setValue("options/editcmd",ui->editor->text());

    QDialog::accept();
}

void OptDialog::restoreDefaults()
{
    ui->exec->setText(defpgm);
    ui->editor->setText(defed);
}
