/*

  Copyright (c) 2014, Michael Tesch
  Copyright (C) 2017 QuiteRSS Team <quiterssteam@gmail.com>
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  * Neither the name of Michael Tesch nor the names of other
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  https://github.com/tesch1/qt-google-analytics-collector

  to enable debugging messages, '#define GANALYTICS_DEBUG 1' before including this file
  to get super verbose debugging, '#define GANALYTICS_DEBUG 2'

  To build GAnalytics with QtQuick application (QGuiApplication) instead of Desktop,
  define GANALYTICS_QTQUICK in your .pro file like this: 'DEFINES += GANALYTICS_QTQUICK',
  or in cmake project: 'add_definitions(-DGANALYTICS_QTQUICK)'
*/
#ifndef GANALYTICS_H
#define GANALYTICS_H

#include <QCoreApplication>
#include <QEventLoop>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUuid>

/*!
 * send google tracking data according to
 * https://developers.google.com/analytics/devguides/collection/protocol/v1/reference
 */

#ifndef GANALYTICS_DEBUG
#define GANALYTICS_DEBUG 0
#endif

class GAnalytics : public QObject {
    Q_OBJECT
    Q_PROPERTY (bool isBusy READ isBusy NOTIFY busyChanged)
public:
    GAnalytics(QCoreApplication *parent, const QString &trackingID,
               const QString &clientID = QString(), bool useGET = false);
    ~GAnalytics();

    // manual config of static fields
    void setClientID(const QString &clientID) { m_clientID = clientID; }
    void setUserID(const QString &userID) { m_userID = userID; }
    void setUserIPAddr(const QString &userIPAddr) { m_userIPAddr = userIPAddr; }
    void setUserAgent(const QString &userAgent) { m_userAgent = userAgent; }
    void setAppName(const QString &appName) { m_appName = appName; }
    void setAppVersion(const QString &appVersion) { m_appVersion = appVersion; }
    void setScreenResolution(const QString &resolution) { m_screenResolution = resolution; }
    void setViewportSize(const QString &viewportSize) { m_viewportSize = viewportSize; }
    void setUserLanguage(const QString &userLanguage) { m_userLanguage = userLanguage; }
    QString getClientID() const { return m_clientID; }
    QString getUserAgent() const { return m_userAgent; }

    // query processing in progress
    bool isBusy() const { return !m_workingQueries.isEmpty(); }

public slots:
    // pageview
    void sendPageview(const QString &docHostname, const QString &page, const QString &title);
    // event
    void sendEvent(const QString &eventCategory, const QString& eventAction,
                   const QString &eventLabel = QString(), int eventValue = 0);
    // transaction
    void sendTransaction(const QString &transactionID,
                         const QString &transactionAffiliation = QString());
    // item
    void sendItem(const QString &itemName);
    // social
    void sendSocial(const QString &socialNetwork, const QString &socialAction,
                    const QString &socialActionTarget);
    // exception
    void sendException(const QString &exceptionDescription, bool exceptionFatal = true);
    // timing
    void sendTiming();
    // screenview
    void sendScreenview(const QString &screenName, const QString &appName = QString(),
                        const QString &appVersion = QString());
    void startSession();
    // To ensure that query was sent before application quit, call waitForIdle()
    void endSession();
    // Waiting for any network operations complete. This method can be used with endSession
    // to ensure that query was completed before application was closed.
    void waitForIdle();

signals:
    void busyChanged();

public:
    void generateUserAgentEtc();

private slots:
    void replyFinished(QNetworkReply *reply);
    void replyError(QNetworkReply::NetworkError code);

private:
    // disable copy const constructor
    GAnalytics(const GAnalytics &);
    QUrlQuery buildMetric(const QString &hitType) const;
    void sendMetric(const QUrlQuery &params);
    QString operatingSystem();
    QString cpuArchitecture();
    QString operatingSystemLong();

    mutable QNetworkAccessManager m_networkManager;
    QString m_trackingID;
    QString m_clientID;
    bool m_useGET; // true=GET, false=POST
    QString m_userID;

    // various parameters:
    bool m_anonymizeIP;
    bool m_cacheBust;

    QString m_userIPAddr;
    QString m_userAgent;
    QString m_appName;
    QString m_appVersion;
#if 0 // todo
    // traffic sources
    QString m_documentReferrer;
    QString m_campaignName;
    QString m_campaignSource;
    QString m_campaignMedium;
    QString m_campaignKeyword;
    QString m_campaignContent;
    QString m_campaignID;
    QString m_adwordsID;
    QString m_displayAdsID;
#endif
    // system info
    QString m_screenResolution;
    QString m_viewportSize;
    QString m_userLanguage;

    // internal
    bool m_isFail;

    QList<QNetworkReply *> m_workingQueries;
    QEventLoop *m_waitLoop;

};

#endif // GANALYTICS_H
