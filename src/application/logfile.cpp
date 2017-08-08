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
#include "logfile.h"
#include "application.h"

const qint64 maxLogFileSize = 1 * 1024 * 1024; // 1 MB

LogFile::LogFile()
{

}

void LogFile::initialize()
{
#ifndef NO_LOG_FILE
    qInstallMessageHandler(LogFile::msgHandler);
#endif
}

void LogFile::msgHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    if (!mainApp || mainApp->dataDirPath().isEmpty())
        return;

    if (type == QtDebugMsg) {
        if (!mainApp->canWriteDebugMsgLog())
            return;
    }

    QFile file;
    file.setFileName(mainApp->dataDirPath() + "/debug.log");
    QIODevice::OpenMode openMode = QIODevice::WriteOnly | QIODevice::Text;

    if (file.exists() && (file.size() < maxLogFileSize))
        openMode |= QIODevice::Append;

    file.open(openMode);

    QTextStream stream;
    stream.setDevice(&file);
    stream.setCodec("UTF-8");

    if (file.isOpen()) {
        const QDateTime currentDateTime = QDateTime::currentDateTime();
        QString dateTimeStr = currentDateTime.toString("dd.MM.yyyy hh:mm:ss.zzz");
        switch (type) {
        case QtDebugMsg:
            stream << dateTimeStr << " Debug:    " << msg << "\n";
            break;
        case QtInfoMsg:
            stream << dateTimeStr << " Info:     " << msg << "\n";
            break;
        case QtWarningMsg:
            stream << dateTimeStr << " Warning:  " << msg << "\n";
            break;
        case QtCriticalMsg:
            stream << dateTimeStr << " Critical: " << msg << "\n";
            break;
        case QtFatalMsg:
            stream << dateTimeStr << " Fatal:    " << msg << "\n";
        default:
            break;
        }

        stream.flush();
        file.flush();
        file.close();

        if (type == QtFatalMsg)
            abort();
    }
}
