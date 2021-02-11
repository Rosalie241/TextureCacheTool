/*
 * Rosalie's Texture Cache Tool - https://github.com/Rosalie241/TextureCacheTool
 *  Copyright (C) 2021 Rosalie Wanders <rosalie@mailbox.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3.
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.
 */
#include <UserInterface/MainWindow.hpp>

#include <QApplication>
#include <QDir>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    UserInterface::MainWindow window;

    window.show();

    return app.exec();
}