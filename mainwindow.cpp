/*
qtl866 - GUI driver for minipro EPROM/Device programmer software
    See also https://github.com/vdudouyt/minipro
    Copyright (C) 2014  Al Williams al.williams@awce.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QRegExp>
#include <QFileInfo>
#include <QDebug>
#include "optdialog.h"
#include "devices.h"

#include <QTextCursor>
#include <QTextDocument>
#include <QTextFrame>
#include <QCompleter>

static
QString getColoredText(QString color, QString text)
{
    return QStringLiteral("<span style='color: %1;'>%2</span>").arg(
                color,
                text
                    .toHtmlEscaped()
                    .replace("\n", "<br/>"));
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    unsigned int i;
    QString selected;
    QSettings settings;
    selected=settings.value("session/seldevice","PIC16F886").toString();
    ui->setupUi(this);
    for (i=0;i<sizeof(devnames)/sizeof(devnames[0]);i++)
        ui->device->addItem(devnames[i],devnames[i]);
    ui->device->setCurrentText(selected);
    ui->filename->setText(settings.value("session/filename","").toString());
    ui->useisp->setChecked(settings.value("session/isp",false).toBool());
    ui->ignoreid->setChecked(settings.value("session/ignoreid", false).toBool());

    QAbstractButton *mode = ui->mode->findChild<QAbstractButton*>(settings.value("session/mode", "readAll").toString());
    if(mode) {
        mode->setChecked(true);
    }

    QCompleter *completer = ui->device->completer();
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    completer->setFilterMode(Qt::MatchContains);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("session/seldevice", ui->device->currentText());
    settings.setValue("session/filename", ui->filename->text());
    settings.setValue("session/isp", ui->useisp->isChecked());
    settings.setValue("session/ignoreid", ui->ignoreid->isChecked());


    QStringList modeWidgets;
    modeWidgets << "readcode" << "readdata" << "readconfig"
                << "writecode" << "writedata" << "writeconfig"
                << "readAll";

    for(QStringList::const_iterator it = modeWidgets.begin(); it != modeWidgets.end(); ++it) {
        QAbstractButton *widget = ui->mode->findChild<QAbstractButton*>(*it);
        if(widget->isChecked()) {
            settings.setValue("session/mode", *it);
            break;
        }
    }

    QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browse_clicked()
{
    ui->filename->setText(QFileDialog::getSaveFileName(this,
       tr("Image File"), ui->filename->text(), tr("Binary Image Files (*.bin);;Text Files (*.txt);;Intel Hex File (*.hex);;Motorola S Record Files (*.srec);;All Files (*)"),0,QFileDialog::DontConfirmOverwrite));
}



// Reenable interface after slave finishes
void MainWindow::on_finished(int code)
{
    QString color = code ? "red" : "black";
    shellAppend(color, tr("\n\nminipro exited with code %1").arg(code));
    qDebug() << "minipro exited with code" << code;

    ui->controls->setEnabled(true);
}

void MainWindow::shellAppend(QString color, QString text)
{
    text = text.replace(QRegExp("\\x001b\\[[^A-Z]*[A-Z]"), "\n");
    QTextCursor cursor = ui->shell->document()->rootFrame()->lastCursorPosition();
    cursor.insertHtml(getColoredText(color, text));
    ui->shell->setTextCursor(cursor);
    statusBar()->showMessage(text.split('\n').last());
}

void MainWindow::on_process_stderr()
{
    shellAppend("red", slave->readAllStandardError());
}

void MainWindow::on_process_stdout()
{
    shellAppend("black", slave->readAllStandardOutput());
}

void MainWindow::on_process_error(QProcess::ProcessError)
{
    QProcess *process = qobject_cast<QProcess*>(sender());
    QMessageBox error(this);
    error.setIcon(QMessageBox::Critical);
    error.setWindowTitle("Could not start minipro");
    error.setText(QStringLiteral("Could not start minipro: %1").arg(process->errorString()));
    error.exec();
    on_finished(-1);
}

void MainWindow::on_exec_clicked()
{
    unsigned int i;
    bool found=false;
    bool reading=true;
    QSettings settings;
    QString cmd=settings.value("options/command","minipro").toString();
    QString devname;
    QStringList args;
    QFileInfo *testfile;
    ui->shell->setText("");  // clear shell window
    if (ui->filename->text().isEmpty()||ui->filename->text().isNull())
    {
        on_browse_clicked();
        emit ui->exec->clicked(); // avoid stack overflow with patient users
        return;
    }
    // TODO: detect .hex .srec etc and convert to temporary file on write (or could do this in script)
    devname=ui->device->currentText();
    for (i=0;i<sizeof(devnames)/sizeof(devnames[0]);i++)
    {
        if (!strcmp(devname.toStdString().c_str(),devnames[i]))
        {
            found=true;
            break;
        }
    }
    if (!found)
    {
        QMessageBox::critical(this,tr("Error"),tr("Unknown device (device names are case sensitive)."));
        return;
    }
    // Build argument string
    args << "-p";
    args << devname;
    if (ui->ignoreid->isChecked()) args << "-y";
    if (!ui->erasechip->isChecked()) args << "-e";
    if (ui->useisp->isChecked()) args << "-i";

    if (!ui->readAll->isChecked()) {
        args << "-c";
        if (ui->readcode->isChecked() || ui->writecode->isChecked()) args << "code";
        if (ui->readdata->isChecked() || ui->writedata->isChecked()) args << "data";
        if (ui->readconfig->isChecked() || ui->writeconfig->isChecked()) args << "config";
    }
    if (ui->writecode->isChecked() || ui->writedata->isChecked() || ui->writeconfig->isChecked())
        reading=false;

    if (reading)
        args << "-r";
    else
        args << "-w";
    args << ui->filename->text();
    testfile=new QFileInfo(ui->filename->text());
    if (reading)
    {
        if (testfile->exists())
        {
            // prompt for overwrite
            if (QMessageBox::No==QMessageBox::question(this,tr("Confirm"),tr("Overwrite File?"),QMessageBox::Yes,QMessageBox::No))
            {
                return;
            }
        }
        if (ui->readAll->isChecked())
        {
            if (QMessageBox::No==QMessageBox::question(this,tr("Warning"),tr("Read All also creates files eeprom.bin and fuses.txt and will overwrite any existing files. Continue?"),QMessageBox::Yes,QMessageBox::No))
            {
                return;
            }

        }

    }
    else
    {
        if (!testfile->exists())
        {
            QMessageBox::critical(this,tr("Error"),tr("File not found or not readable"));
        }
    }
    // TODO: Could add write all and do in 3 operations


    ui->controls->setEnabled(false);
    slave=new QProcess(this);
    connect(slave,SIGNAL(finished(int)),this,SLOT(on_finished(int)));
    connect(slave, SIGNAL(error(QProcess::ProcessError)), this, SLOT(on_process_error(QProcess::ProcessError)));
    connect(slave, SIGNAL(readyReadStandardOutput()), this, SLOT(on_process_stdout()));
    connect(slave, SIGNAL(readyReadStandardError()), this, SLOT(on_process_stderr()));

    qDebug() << "Executing" << cmd << args;
    slave->start(cmd, args, QProcess::ReadWrite|QProcess::Unbuffered);
}

void MainWindow::on_action_About_triggered()
{
    QMessageBox::information(this,tr("About"),tr("qtl866 V0.3 Copyright (c) 2014 by Al Williams http://www.awce.com.\n"
                                                 "This program comes with ABSOLUTELY NO WARRANTY. "
                                                 "This is free software, and you are welcome to redistribute it under certain conditions.\n"
                                                 "See the file COPYING for more information."
                                                 ));
}

void MainWindow::on_action_Options_triggered()
{
    OptDialog *dlg=new OptDialog(this);
    dlg->exec();
}

void MainWindow::on_editbtn_clicked()
{
    QString cmdline=settings->value("options/editcmd","binhexedit -r").toString() +" " + ui->filename->text();
    system(cmdline.toLatin1());
}
