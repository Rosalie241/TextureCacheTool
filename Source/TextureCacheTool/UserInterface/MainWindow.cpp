/*
 * Rosalie's Texture Cache Tool - https://github.com/Rosalie241/TextureCacheTool
 *  Copyright (C) 2021 Rosalie Wanders <rosalie@mailbox.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3.
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.
 */
#include "MainWindow.hpp"
#include "../../GLideNHQ/TextureCache.hpp"

using namespace UserInterface;

#include <QListWidgetItem>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(void)
{
    this->setupUi(this);
}

MainWindow::~MainWindow(void)
{
    if (cache != nullptr)
    {
        delete cache;
    }
}

 
void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *)
{
    if (current == nullptr)
        return;

    QPixmap pixmap = QPixmap::fromImage(cache->GetImage(getCurrentTexture()));
    label->setAlignment(Qt::AlignCenter);
    label->setPixmap(pixmap.scaled(500, 500, Qt::KeepAspectRatio));
}

void MainWindow::on_replaceButton_clicked(void)
{
    int row = listWidget->currentRow();

    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Open Texture"), "", tr("Texture (*.texture)"));

    if (fileName.isEmpty())
        return;

    if (!cache->ReplaceTexture(getCurrentTexture(), fileName))
    {
        displayMessage("Error", "cache->ReplaceTexture() Failed!");
        return;
    }

    reloadList();
    listWidget->setCurrentRow(row);
    on_lineEdit_textEdited(lineEdit->text());
}

void MainWindow::on_openButton_clicked(void)
{
    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Open Texture Pack"), "", tr("Texture Pack (*.HTS)"));

    if (fileName.isEmpty())
        return;

    if (cache != nullptr)
        delete cache;

    cache = new TextureCache(fileName);

    label->clear();
    lineEdit->clear();
    reloadList();
}

void MainWindow::on_exportButton_clicked(void)
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Texture"), "", tr("Texture (*.texture)"));

    if (fileName.isEmpty())
        return;

    if (!cache->SaveTexture(getCurrentTexture(), fileName))
    {
        displayMessage("Error", "cache->SaveTexture() Failed!");
    }
}

void MainWindow::on_lineEdit_textEdited(const QString& text)
{
    QString searchTerm = text;

    QList<QString> splitText = text.split("#");
    QString cksum, palchksum;
    if (splitText.count() > 1)
        cksum = splitText.at(1);
    if (splitText.count() > 4)
        palchksum = text.split("#").at(4).split("_").at(0);
    if (!cksum.isEmpty())
    {
        if (cksum != "$")
            searchTerm = cksum;
        if (!palchksum.isEmpty() &&
            palchksum != "$")
        {
            searchTerm += " " + palchksum;
        }
    }

    for (int row = 0; row < this->listWidget->count(); row++)
    {
        QListWidgetItem *item = this->listWidget->item(row);
        item->setHidden(!item->text().contains(searchTerm));
    }
}

void MainWindow::displayMessage(QString title, QString text)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.addButton(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::reloadList(void)
{
    if (cache == nullptr)
        return;

    listWidget->clear();

    for (Texture& t : cache->GetTextures())
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(t.ChecksumString);
        item->setData(Qt::UserRole, QVariant::fromValue(t));
        listWidget->addItem(item);
    }
}

Texture MainWindow::getCurrentTexture(void)
{
    QListWidgetItem *item = listWidget->currentItem();
    return item->data(Qt::UserRole).value<Texture>();
}
