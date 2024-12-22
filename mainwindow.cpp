#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QMessageBox>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    loadSettings();
    ui->setupUi(this);
    connect(ui->listWidget, &QListWidget::itemChanged, this, &MainWindow::on_ChangeCheckBox);
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::on_pbAddNewTask_clicked()
{
    QString task = ui->lineEdit->text();
    if(task.isEmpty())
    {
        QMessageBox msgBoxWithError;
        msgBoxWithError.setWindowTitle("Ошибка");
        msgBoxWithError.setText("Поле ввода новой задачи пусто");
        msgBoxWithError.exec();
        return;
    }

    QListWidgetItem* item = new QListWidgetItem(task, ui->listWidget);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Unchecked);
    ui->lineEdit->clear();
}

void MainWindow::on_pbDeleteTask_clicked()
{
    QListWidgetItem *selectedItem = ui->listWidget->currentItem();
    if (selectedItem)
        delete ui->listWidget->takeItem(ui->listWidget->row(selectedItem));
}

void MainWindow::on_ChangeCheckBox(QListWidgetItem *item)
{
    QFont font = item->font();
    font.setStrikeOut(item->checkState() == Qt::Checked);
    item->setFont(font);
}

void MainWindow::on_pbSave_clicked()
{
    QFile file("saveList.xml");
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для записи.");
        return;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("Items");

    for (int i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        xmlWriter.writeStartElement("task");
        xmlWriter.writeAttribute("text", item->text());
        xmlWriter.writeAttribute("checked", item->checkState() == Qt::Checked ? "true" : "false");
        xmlWriter.writeEndElement(); // Item
    }

    xmlWriter.writeEndElement(); // Items
    xmlWriter.writeEndDocument();
    file.close();
}


void MainWindow::on_pbLoad_clicked()
{
    QFile file("saveList.xml");
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для чтения.");
        return;
    }

    ui->listWidget->clear();

    QXmlStreamReader xmlReader(&file);
    while (!xmlReader.atEnd() && !xmlReader.hasError())
    {
        xmlReader.readNext();
        if (xmlReader.isStartElement() && xmlReader.name() == QString("task"))
        {
            QString text = xmlReader.attributes().value("text").toString();
            bool checked = xmlReader.attributes().value("checked") == QString("true");

            QListWidgetItem *item = new QListWidgetItem(text);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);

            on_ChangeCheckBox(item);
            ui->listWidget->addItem(item);
        }
    }

    if (xmlReader.hasError())
        QMessageBox::warning(this, "Ошибка", "Ошибка при чтении XML файла.");

    file.close();
}

void MainWindow::on_pbClear_clicked()
{
    ui->listWidget->clear();
}

void MainWindow::loadSettings()
{
    QSettings settings("IK", "TaskList");
    move(settings.value("pos", QPoint(200, 200)).toPoint());
}

void MainWindow::saveSettings()
{
    QSettings settings("IK", "TaskList");
    settings.setValue("pos", pos());
}

