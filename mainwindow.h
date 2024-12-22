#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QListWidgetItem;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pbAddNewTask_clicked();

    void on_pbDeleteTask_clicked();

    void on_ChangeCheckBox(QListWidgetItem *item);

    void on_pbSave_clicked();

    void on_pbLoad_clicked();

    void on_pbClear_clicked();

    void loadSettings();

    void saveSettings();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
