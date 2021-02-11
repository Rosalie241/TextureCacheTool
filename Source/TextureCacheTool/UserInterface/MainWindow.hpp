/*
 * Rosalie's Texture Cache Tool - https://github.com/Rosalie241/TextureCacheTool
 *  Copyright (C) 2021 Rosalie Wanders <rosalie@mailbox.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3.
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ui_MainWindow.h"
#include "../../GLideNHQ/TextureCache.hpp"

#include <QList>
#include <QImage>

namespace UserInterface
{
    class MainWindow : public QMainWindow, private Ui::MainWindow
    {
        Q_OBJECT
    public:
        MainWindow();
        ~MainWindow();

    private slots:
        void on_listWidget_currentItemChanged(QListWidgetItem *, QListWidgetItem *);

        void on_openButton_clicked(void);
        void on_replaceButton_clicked(void);
        void on_exportButton_clicked(void);

        void on_lineEdit_textEdited(const QString&);

    private:
        QList<Texture> textures;
        TextureCache *cache = nullptr;

        void displayMessage(QString, QString);
        void reloadList(void);

        Texture getCurrentTexture(void);
    };

} // namespace UserInterface
#endif // MAINWINDOW_HPP
