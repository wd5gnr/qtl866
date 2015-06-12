#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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


#include <QMainWindow>
#include <QProcess>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private slots:
    void on_browse_clicked();

    void on_exec_clicked();

    void on_process_error(QProcess::ProcessError);
    void on_finished(int code);
    void on_print();

    void on_action_About_triggered();

    void on_action_Options_triggered();

    void on_editbtn_clicked();

protected:
    virtual void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QProcess *slave;
};

#endif // MAINWINDOW_H
