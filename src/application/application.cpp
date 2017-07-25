/****************************************************************************
**
** QuiteRSS is a open-source cross-platform news feed reader
** Copyright (C) 2011-2017 QuiteRSS Team <quiterssteam@gmail.com>
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "application.h"
#include "webengine.h"

#include <QDebug>
#include <QQmlFileSelector>
#include <QQmlContext>
#include <QTranslator>

Application::Application(int &argc, char **argv)
  : QtSingleApplication(argc, argv)
{
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QTranslator translator_;
    translator_.load(QString("translations/quiterss_%1").arg("ru"));
    installTranslator(&translator_);

    WebEngine::initialize();

    QQmlFileSelector *qfs = new QQmlFileSelector(&engine, &engine);
    QStringList selectors = WebEngine::getQmlSelectors();
    qfs->setExtraSelectors(selectors);

    QQmlContext *context = engine.rootContext();
    context->setContextProperty("app", this);

    engine.load(QUrl(QStringLiteral("qrc:/qml/mainwindow.qml")));
}

Application::~Application()
{

}
