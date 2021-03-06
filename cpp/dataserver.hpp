#ifndef DATASERVER_H
#define DATASERVER_H

#include <QObject>

#include "gen/DataManager.hpp"

#if defined (Q_OS_IOS)
#include "ios/src/ReachabilityListener.h"
#endif

#include <QUrl>
#include <QDebug>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkrequest.h>
#include <QtNetwork/qnetworkreply.h>
#include <QtNetwork/qnetworkconfigmanager.h>
#include <QtNetwork/qnetworkconfiguration.h>
#include <qfile.h>
#include <QTimer>

class DataServer : public QObject
#if defined (Q_OS_IOS)
    , private utility::ReachabilityDelegate
#endif

{
    Q_OBJECT
public:
    explicit DataServer(QObject *parent = 0);
    virtual ~DataServer();

    void init(DataManager* dataManager);
    void setRepositoryDataPath(const QString& repositoryDataPath);

    // ONLINE, OFFLINE, NETWORK
    Q_INVOKABLE
    bool isOnline();

    Q_INVOKABLE
    bool serverIsHungry();

    Q_INVOKABLE
    QString networkInfo();
    //

    void requestDictionary();
    void requestVersion();

signals:
    // triggered if online state changed
    void onlineChanged(const bool currentState);
    // optional: wait given delay before changing state
    void onlineWarmUp(const bool isOn);
    void serverIsHungryForHeavyWork();

    void versionSuccess(QByteArray currentVersion);
    void versionFailed(QString message);

    void serverSuccess();
    void serverFailed(QString message);

public slots:

private slots:
    // NETWORK
    void onOnlineStateChanged(bool isOnline);
    // timeouts from Timers
    void onOnlineStateCollected();
    void onOnlineStableConnection();

    // REST
    void onFinishedDictionary();
    void onFinishedCreator();

    void onFinishedVersion();

private:

    DataManager* mDataManager;
    QString mRepositoryDataPath;

    QNetworkConfigurationManager* mNetworkConfigManager;
    // to avoid too short on-off-cycles
    // coming back from suspended all collected signals
    // fired by QNetworkConfigurationManager
    // this timer waits some ms before emitting signal onlineChanged
    QTimer* mOnlineStateCollector;
    // current online state propagated to UI
    bool mCurrentIsOnline;
    // last online state got from QNetworkConfigurationManager
    bool mNewestIsOnline;
    // Timer waiting a given time with stable Online state
    QTimer* mOnlineStableTimer;
    // server is ready to do heavy work
    bool mIsHungry;
    // flag if stable timer is in use
    bool mUseOnlineStableTimer;
    // timer timeout intervals
    int mOnlineStateCollectorInterval;
    int mOnlineStableTimerInterval;

#if defined (Q_OS_IOS)
    void statusChanged(utility::NetworkStatus newStatus);
#endif

    // REST
    QNetworkAccessManager* mNetworkAccessManager;
    void requestCreator();

};

#endif // DATASERVER_H
