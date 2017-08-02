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
#include "ganalytics.h"

#if defined(GANALYTICS_QTQUICK)
#include <QGuiApplication>
#include <QScreen>
#else
#ifdef QT_GUI_LIB
#include <QApplication>
#include <QDesktopWidget>
#endif
#endif
#ifdef Q_OS_MAC
#include <CoreServices/CoreServices.h>
#endif
#include <QDebug>
#include <QNetworkRequest>
#include <QProcess>
#include <QSettings>
#include <QUrl>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QUrlQuery>
#define URL_QUERY QUrlQuery
#else
#define URL_QUERY QUrl
#endif
#if defined Q_OS_WIN
#include <windows.h>
#endif

GAnalytics::GAnalytics(QCoreApplication *parent, const QString &trackingID,
                       const QString &clientID, bool useGET) :
    QObject(parent),
    m_networkManager(this),
    m_trackingID(trackingID),
    m_clientID(clientID),
    m_useGET(useGET),
    m_isFail(false),
    m_waitLoop()
{
    if (parent) {
        setAppName(parent->applicationName());
        setAppVersion(parent->applicationVersion());
#if GANALYTICS_DEBUG
        parent->dumpObjectTree();
#endif
    }
    if (!m_clientID.size()) {
        // load client id from settings
        QSettings settings;
        if (!settings.contains("GAnalytics-cid"))
            settings.setValue("GAnalytics-cid", QUuid::createUuid().toString());
        m_clientID = settings.value("GAnalytics-cid").toString();
    }
    connect(&m_networkManager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(replyFinished(QNetworkReply *)));
#if QT_VERSION >= 0x040800
#if GANALYTICS_DEBUG
    if (!m_networkManager.networkAccessible())
        qDebug() << "error: network inaccessible\n";
#endif
#endif
}

GAnalytics::~GAnalytics()
{
#if GANALYTICS_DEBUG
    // this generally happens after the event-loop is done, so no more network processing
    QList<QNetworkReply *> replies = m_networkManager.findChildren<QNetworkReply *>();
    for (QList<QNetworkReply *>::iterator it = replies.begin(); it != replies.end(); ++it) {
        if ((*it)->isRunning()) {
            qDebug() << "~GAnalytics, request still running: "
                     << (*it)->url().toString() << ", aborting.";
        }
    }
    qWarning() << "~GAnalytics";
#endif
}

void GAnalytics::sendPageview(const QString &docHostname, const QString &page,
                              const QString &title)
{
    URL_QUERY params = buildMetric("pageview");
    params.addQueryItem("dh", docHostname);
    params.addQueryItem("dp", page);
    params.addQueryItem("dt", title);
    sendMetric(params);
}

void GAnalytics::sendEvent(const QString &eventCategory, const QString &eventAction,
                           const QString &eventLabel, int eventValue)
{
    URL_QUERY params = buildMetric("event");
    if (m_appName.size())
        params.addQueryItem("an", m_appName);
    if (m_appVersion.size())
        params.addQueryItem("av", m_appVersion);
    if (eventCategory.size())
        params.addQueryItem("ec", eventCategory);
    if (eventAction.size())
        params.addQueryItem("ea", eventAction);
    if (eventLabel.size())
        params.addQueryItem("el", eventLabel);
    if (eventValue)
        params.addQueryItem("ev", QString::number(eventValue));
    sendMetric(params);
}

void GAnalytics::sendTransaction(const QString &transactionID,
                                 const QString &transactionAffiliation)
{
    URL_QUERY params = buildMetric("transaction");
    params.addQueryItem("ti", transactionID);
    if (transactionAffiliation.size())
        params.addQueryItem("ta", transactionAffiliation);
    sendMetric(params);
}

void GAnalytics::sendItem(const QString &itemName)
{
    URL_QUERY params = buildMetric("item");
    params.addQueryItem("in", itemName);
    sendMetric(params);
}

void GAnalytics::sendSocial(const QString &socialNetwork, const QString &socialAction,
                            const QString &socialActionTarget)
{
    URL_QUERY params = buildMetric("social");
    params.addQueryItem("sn", socialNetwork);
    params.addQueryItem("sa", socialAction);
    params.addQueryItem("st", socialActionTarget);
    sendMetric(params);
}

void GAnalytics::sendException(const QString &exceptionDescription,
                               bool exceptionFatal)
{
    URL_QUERY params = buildMetric("exception");
    if (exceptionDescription.size())
        params.addQueryItem("exd", exceptionDescription);
    if (!exceptionFatal)
        params.addQueryItem("exf", "0");
    sendMetric(params);
}

void GAnalytics::sendTiming()
{
    URL_QUERY params = buildMetric("timing");
    sendMetric(params);
}

void GAnalytics::sendScreenview(const QString &screenName, const QString &appName,
                                const QString &appVersion)
{
    URL_QUERY params = buildMetric("screenview");
    if (!appName.isEmpty())
        params.addQueryItem("an", appName);
    else if (!m_appName.isEmpty())
        params.addQueryItem("an", m_appName);
    if (!appVersion.isEmpty())
        params.addQueryItem("av", appVersion);
    else if (!m_appVersion.isEmpty())
        params.addQueryItem("av", m_appVersion);
    if (screenName.size())
        params.addQueryItem("cd", screenName);
    sendMetric(params);
}

void GAnalytics::startSession()
{
    URL_QUERY params = buildMetric("event");
    if (m_appName.size())
        params.addQueryItem("an", m_appName);
    if (m_appVersion.size())
        params.addQueryItem("av", m_appVersion);
    params.addQueryItem("sc", "start");
    params.addQueryItem("ec", "General");
    params.addQueryItem("ea", "Start");
    params.addQueryItem("ev", "0");
    sendMetric(params);
}

void GAnalytics::endSession()
{
    URL_QUERY params = buildMetric("event");
    if (m_appName.size())
        params.addQueryItem("an", m_appName);
    if (m_appVersion.size())
        params.addQueryItem("av", m_appVersion);
    params.addQueryItem("sc", "end");
    params.addQueryItem("ec", "General");
    params.addQueryItem("ea", "End");
    params.addQueryItem("ev", "0");
    sendMetric(params);
}

void GAnalytics::waitForIdle()
{
    if (m_waitLoop) {
        qCritical() << "Recursive call GAnalytics::waitForIdle";
        return;
    }

    QEventLoop loop;
    m_waitLoop = &loop;
    loop.exec(QEventLoop::ExcludeUserInputEvents);
    m_waitLoop = 0;
}

void GAnalytics::generateUserAgentEtc()
{
    QString locale = QLocale::system().name().toLower().replace("_", "-");
    QString os = operatingSystemLong();
    m_userAgent = "Mozilla/5.0 (" + os + "; " + locale
            + ") GAnalytics/1.0 (Qt/" QT_VERSION_STR ")";
    m_userLanguage = locale;
#if defined(GANALYTICS_QTQUICK)
    QScreen *screen = qApp->primaryScreen();
    QString geom = QString::number(screen->geometry().width())
            + "x" + QString::number(screen->geometry().height());
    m_screenResolution = geom;
#else
#ifdef QT_GUI_LIB
    QString geom = QString::number(QApplication::desktop()->screenGeometry().width())
            + "x" + QString::number(QApplication::desktop()->screenGeometry().height());
    _screenResolution = geom;
#endif
#endif
#if GANALYTICS_DEBUG > 1
    qDebug() << "User-Agent:" << m_userAgent;
    qDebug() << "Language:" << m_userLanguage;
    qDebug() << "Screen Resolution:" << m_screenResolution;
#endif
}

void GAnalytics::replyFinished(QNetworkReply *reply)
{
    if (QNetworkReply::NoError != reply->error()) {
        qCritical() << "replyFinished error: " << reply->errorString() << "\n";
    } else {
        int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (httpStatus < 200 || httpStatus > 299) {
#if GANALYTICS_DEBUG
            qDebug() << "response code: " << httpStatus;
#endif
            m_isFail = true;
        }
    }

    m_workingQueries.removeAll(reply);
    if (m_workingQueries.isEmpty()) {
        emit busyChanged();
        if (m_waitLoop)
            m_waitLoop->exit();
    }

    reply->deleteLater();
}

void GAnalytics::replyError(QNetworkReply::NetworkError code)
{
    qDebug() << "network error signal: " << code << "\n";
}

URL_QUERY GAnalytics::buildMetric(const QString &hitType) const
{
    URL_QUERY params;
    // required in v1
    params.addQueryItem("v", "1" ); // version
    params.addQueryItem("tid", m_trackingID);
    params.addQueryItem("cid", m_clientID);
    params.addQueryItem("t", hitType);
    // optional
    if (m_userID.size())
        params.addQueryItem("uid", m_userID);
    if (m_userIPAddr.size())
        params.addQueryItem("uip", m_userIPAddr);
    if (m_screenResolution.size())
        params.addQueryItem("sr", m_screenResolution);
    if (m_viewportSize.size())
        params.addQueryItem("vp", m_viewportSize);
    if (m_userLanguage.size())
        params.addQueryItem("ul", m_userLanguage);
    return params;
}

void GAnalytics::sendMetric(const URL_QUERY &params)
{
    // when google has err'd us, then stop sending events!
    if (m_isFail)
        return;
    QUrl collect_url("http://www.google-analytics.com/collect");
    QNetworkRequest request;
    if (m_userAgent.size())
        request.setRawHeader("User-Agent", m_userAgent.toUtf8());
    QNetworkReply *reply;
    if (m_useGET) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        collect_url.setQuery(params);
#else
        collect_url.setQueryItems(params.queryItems());
#endif
        request.setUrl(collect_url);
        reply = m_networkManager.get(request);
    } else {
        request.setUrl(collect_url);
        request.setHeader(QNetworkRequest::ContentTypeHeader,
                          "application/x-www-form-urlencoded");

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        QByteArray postData = params.query(QUrl::FullyEncoded).toUtf8();
#else
        QByteArray postData = params.encodedQuery();
#endif
        reply = m_networkManager.post(request, postData);
    }
    m_workingQueries << reply;
    if (m_workingQueries.size() == 1)
        emit busyChanged();

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(replyError(QNetworkReply::NetworkError)));
#if GANALYTICS_DEBUG > 1
    qDebug() << "GAnalytics sent: " << params.toString();
#endif
    reply->setParent(&m_networkManager);
}

QString GAnalytics::operatingSystem()
{
#ifdef Q_OS_MAC
    QString str = "Mac OS X";
    return str;
#endif
#ifdef Q_OS_LINUX
    QString str = "Linux";
#ifdef Q_OS_ANDROID
    str.append("; U; Android " + QSysInfo::productVersion());
#endif
    return str;
#endif
#ifdef Q_OS_BSD4
    return "BSD 4.4";
#endif
#ifdef Q_OS_BSDI
    return "BSD/OS";
#endif
#ifdef Q_OS_FREEBSD
    return "FreeBSD";
#endif
#ifdef Q_OS_HPUX
    return "HP-UX";
#endif
#ifdef Q_OS_HURD
    return "GNU Hurd";
#endif
#ifdef Q_OS_LYNX
    return "LynxOS";
#endif
#ifdef Q_OS_NETBSD
    return "NetBSD";
#endif
#ifdef Q_OS_OS2
    return "OS/2";
#endif
#ifdef Q_OS_OPENBSD
    return "OpenBSD";
#endif
#ifdef Q_OS_OSF
    return "HP Tru64 UNIX";
#endif
#ifdef Q_OS_SOLARIS
    return "Sun Solaris";
#endif
#ifdef Q_OS_UNIXWARE
    return "UnixWare 7 / Open UNIX 8";
#endif
#ifdef Q_OS_UNIX
    return "Unix";
#endif
#ifdef Q_OS_HAIKU
    return "Haiku";
#endif
#ifdef Q_OS_WIN32
    QString str = "Windows NT";

    switch (QSysInfo::windowsVersion()) {
    case QSysInfo::WV_NT:
        str.append(" 4.0");
        break;

    case QSysInfo::WV_2000:
        str.append(" 5.0");
        break;

    case QSysInfo::WV_XP:
        str.append(" 5.1");
        break;
    case QSysInfo::WV_2003:
        str.append(" 5.2");
        break;

    case QSysInfo::WV_VISTA:
        str.append(" 6.0");
        break;

    case QSysInfo::WV_WINDOWS7:
        str.append(" 6.1");
        break;

    case QSysInfo::WV_WINDOWS8:
        str.append(" 6.2");
        break;
#if QT_VERSION >= 0x050400
    case QSysInfo::WV_WINDOWS8_1:
        str.append(" 6.3");
        break;
#endif
#if QT_VERSION >= 0x050600
    case QSysInfo::WV_WINDOWS10:
        str.append(" 10.0");
        break;
#endif
    default:
        break;
    }

    return str;
#endif
}

QString GAnalytics::cpuArchitecture()
{
#if QT_VERSION >= 0x050400
#ifdef Q_OS_ANDROID
    return "";
#else
    return QSysInfo::currentCpuArchitecture();
#endif
#else
    return "";
#endif
}

QString GAnalytics::operatingSystemLong()
{
    QString os = operatingSystem();
#if QT_VERSION >= 0x050400
#ifdef Q_OS_UNIX
    if (QGuiApplication::platformName() == QLatin1String("xcb"))
        os.prepend(QLatin1String("X11; "));
    else if (QGuiApplication::platformName().startsWith(QLatin1String("wayland")))
        os.prepend(QLatin1String("Wayland; "));
#endif
#endif

    const QString arch = cpuArchitecture();
    if (arch.isEmpty())
        return os;
    return os + QStringLiteral(" ") + arch;
}
