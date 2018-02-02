#include "dataserver.hpp"

#include <QSslConfiguration>

DataServer::DataServer(QObject *parent) : QObject(parent)
{
    //
}

DataServer::~DataServer()
{
    //
}

/**
 * init - called from starting the APP
 */
void DataServer::init(DataManager *dataManager)
{
    mDataManager = dataManager;

    // workaround bug iOS - cannot reuse QNetworkAccessManager QTBUG-49751
    // otherwise accessibility not detected if switch off and on again
    // mNetworkAccessManager = new QNetworkAccessManager(this);

    // ONLINE, OFFLINE, HUNGRY
    // get this from settings:
    mUseOnlineStableTimer = true;
    mOnlineStateCollectorInterval = 500;
    mOnlineStableTimerInterval = 2*60*1000; // 2 minutes

    bool connectResult = false;

    // ATTENTION problem on iOS to get correct isOnline from QNetworkConfigurationManager:
    // https://bugreports.qt.io/browse/QTBUG-56151
    // https://bugreports.qt.io/browse/QTBUG-58946
    // so even in airplane mode  isOnline reports true
    // so we' added're using iOS Reachability classes instead
#if defined (Q_OS_IOS)
    mCurrentIsOnline = status() != utility::NotReachable;
    qDebug() << "I O S  REACHABILITY: INIT - IS   ONLINE ?" << mCurrentIsOnline;
#else
    mNetworkConfigManager = new QNetworkConfigurationManager(this);
    mCurrentIsOnline = mNetworkConfigManager->isOnline();
    qDebug() << "INIT - IS   ONLINE ?" << mCurrentIsOnline;
    connectResult = connect(mNetworkConfigManager, SIGNAL(onlineStateChanged(bool)), this, SLOT(onOnlineStateChanged(bool)));
    Q_ASSERT(connectResult);
#endif

    // to avoid too short on-off-cycles
    // coming back from suspended all collected signals
    // fired by QNetworkConfigurationManager
    // this timer waits short time before emitting signal onlineChanged
    mOnlineStateCollector = new QTimer(this);
    mOnlineStateCollector->setSingleShot(true);
    mOnlineStateCollector->setInterval(mOnlineStateCollectorInterval);

    mIsHungry = false;
    if(mUseOnlineStableTimer) {
        mOnlineStableTimer = new QTimer(this);
        mOnlineStableTimer->setSingleShot(true);
        // 2 minutes
        mOnlineStableTimer->setInterval(mOnlineStableTimerInterval);
        if(mCurrentIsOnline) {
            mOnlineStableTimer->start();
        }
    }

    connectResult = connect(mOnlineStateCollector, SIGNAL(timeout()), this, SLOT(onOnlineStateCollected()));
    Q_ASSERT(connectResult);

    if(mUseOnlineStableTimer) {
        connectResult = connect(mOnlineStableTimer, SIGNAL(timeout()), this, SLOT(onOnlineStableConnection()));
        Q_ASSERT(connectResult);
    }

    Q_UNUSED(connectResult);


    qDebug() << "Data Server INIT done";
}

// get the current state
bool DataServer::isOnline()
{
    return mCurrentIsOnline;
}

bool DataServer::serverIsHungry()
{
    return mIsHungry;
}

// Network Info INVOKABLE used in TitleBar onlineButton
// see https://bugreports.qt.io/browse/QTBUG-56151
QString DataServer::networkInfo()
{
    QString networkInfo;
    networkInfo.append("Online: ");
    if(mCurrentIsOnline) {
        networkInfo.append(tr("YES\n"));
    } else {
        networkInfo.append(tr("NO\n"));
    }
#if defined (Q_OS_IOS)
    // active network configurations are confusing
    // under iOS "utun0" and "en2" are reported active even in airplane mode
    // so we don't use QNetworkConfigurationManager for iOS
    switch (status()) {
    case utility::NotReachable:
        networkInfo.append(tr("Internet not reachable"));
        break;
    case utility::ReachableViaWiFi:
        networkInfo.append(tr("WiFi internet connection"));
        break;
    case utility::ReachableViaWWAN:
        networkInfo.append(tr("mobile data internet connection"));
        break;
    default:
        break;
    }
#else
    QString activeNetworkConfigNames;
    QList<QNetworkConfiguration> activeConfigs = mNetworkConfigManager->allConfigurations(QNetworkConfiguration::Active);
    for (int i = 0; i < activeConfigs.size(); ++i) {
        QNetworkConfiguration config = activeConfigs.at(i);
        if(!activeNetworkConfigNames.isEmpty()) {
            activeNetworkConfigNames.append(" | ");
        }
        if(config.bearerTypeName() == "Unknown") {
            activeNetworkConfigNames.append("?? ");
        } else {
            activeNetworkConfigNames.append(config.bearerTypeName());
        }
        activeNetworkConfigNames.append(":").append(config.name());
    } // all active configurations
    networkInfo.append("Active: ");
    networkInfo.append(activeNetworkConfigNames).append("\n");
    networkInfo.append("Default: ").append(mNetworkConfigManager->defaultConfiguration().name());

    if(activeNetworkConfigNames.isEmpty()) {
        networkInfo.append("\n").append(tr("no network connection - WIFI On ?"));
    }
#endif
    // ad more infos from configurations
    // or add infos about running requests, last action done, ...
    return networkInfo;
}

// SLOT
// for some ms no new SIGNAL collected
// now we can inform UI if different from last state
// if stableTimer is used, also watch if offline / online
void DataServer::onOnlineStateCollected() {
    if(mCurrentIsOnline != mNewestIsOnline) {
        mCurrentIsOnline = mNewestIsOnline;
        qDebug() << "NEW ONLINE STATE: " << mCurrentIsOnline;
        emit onlineChanged(mCurrentIsOnline);
        if(mUseOnlineStableTimer) {
            if(!mCurrentIsOnline) {
                // stop mOnlineStableTimer if offline
                mOnlineStableTimer->stop();
                mIsHungry = false;
            } else {
                // if online start mOnlineStableTimer if not already running
                if(!mOnlineStableTimer->isActive()) {
                    mOnlineStableTimer->start();
                }
            }
        }
    }
}

// SLOT
// waits 2 minutes stable online connection
// before emitting SIGNAL that server is now hungry
// to do heavy work (downloads, uploads)
void DataServer::onOnlineStableConnection() {
    qDebug() << "SERVER H U N G R Y";
    mIsHungry = true;
    emit serverIsHungryForHeavyWork();
    // do something - per ex process server queue
}

// Signal coming from QNetworkConfigurationManager
void DataServer::onOnlineStateChanged(bool isOnline)
{
    mNewestIsOnline = isOnline;
    // always restart Collector (Timer)
    mOnlineStateCollector->start();
    if(isOnline) {
        qDebug() << "collect: O N";
    } else {
        qDebug() << "collect: O F F";
    }
}

#if defined (Q_OS_IOS)
void DataServer::statusChanged(utility::NetworkStatus newStatus)
{
    if (newStatus == utility::NotReachable) {
        qDebug("I O S   REACHABILITY: OFFLINE");
        onOnlineStateChanged(false);
    } else {
        onOnlineStateChanged(true);
        qDebug("I O S   REACHABILITY: ONLINE");
    }
}
#endif

// ONLINE end


void DataServer::setRepositoryDataPath(const QString &repositoryDataPath)
{
    mRepositoryDataPath = repositoryDataPath;
    qDebug() << "Repository Data path: " << mRepositoryDataPath;
}

void DataServer::requestDictionary()
{
    // workaround bug iOS - cannot reuse QNetworkAccessManager QTBUG-49751
    // otherwise accessibility not detected if switch off and on again
    QNetworkAccessManager* networkAccessManager = new QNetworkAccessManager(this);
    if(networkAccessManager->networkAccessible() != QNetworkAccessManager::Accessible) {
        if(networkAccessManager->networkAccessible() == QNetworkAccessManager::NotAccessible) {
            qDebug() << "requestDictionary NO ACCESS TO NETWORK";
            emit serverFailed(tr("No Network Access"));
            return;
        }
        qDebug() << "requestDictionary NO ACCESS: The network accessibility cannot be determined.";
        emit serverFailed(tr("No Network Access"));
        return;
    }

    QString uri;
    uri = "https://tipesoft.com/dl/api/dicotae/dictionary.json";
    qDebug() << "requestDictionary uri:" << uri;

    QNetworkRequest request(uri);

    // to avoid ssl errors:
//    QSslConfiguration conf = request.sslConfiguration();
//    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
//    request.setSslConfiguration(conf);

    QNetworkReply* reply = networkAccessManager->get(request);
    bool connectResult = connect(reply, SIGNAL(finished()), this, SLOT(onFinishedDictionary()));
    Q_ASSERT(connectResult);
    Q_UNUSED(connectResult);
}

void DataServer::requestVersion()
{
    // workaround bug iOS - cannot reuse QNetworkAccessManager QTBUG-49751
    // otherwise accessibility not detected if switch off and on again
    QNetworkAccessManager* networkAccessManager = new QNetworkAccessManager(this);
    if(networkAccessManager->networkAccessible() != QNetworkAccessManager::Accessible) {
        if(networkAccessManager->networkAccessible() == QNetworkAccessManager::NotAccessible) {
            qDebug() << "requestVersion NO ACCESS TO NETWORK";
            emit versionFailed(tr("No Network Access"));
            return;
        }
        qDebug() << "requestVersion NO ACCESS: The network accessibility cannot be determined.";
        emit versionFailed(tr("No Network Access"));
        return;
    }

    QString uri;
    uri = "https://tipesoft.com/dl/api/dicotae/version.json";
    qDebug() << "requestVersion uri:" << uri;

    QNetworkRequest request(uri);

    // to avoid ssl errors:
//    QSslConfiguration conf = request.sslConfiguration();
//    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
//    request.setSslConfiguration(conf);

    QNetworkReply* reply = networkAccessManager->get(request);
    bool connectResult = connect(reply, SIGNAL(finished()), this, SLOT(onFinishedVersion()));
    Q_ASSERT(connectResult);
    Q_UNUSED(connectResult);
}

void DataServer::requestCreator()
{
    // workaround bug iOS - cannot reuse QNetworkAccessManager QTBUG-49751
    // otherwise accessibility not detected if switch off and on again
    QNetworkAccessManager* networkAccessManager = new QNetworkAccessManager(this);
    if(networkAccessManager->networkAccessible() != QNetworkAccessManager::Accessible) {
        if(networkAccessManager->networkAccessible() == QNetworkAccessManager::NotAccessible) {
            qDebug() << "requestCreator NO ACCESS TO NETWORK";
            emit serverFailed(tr("No Network Access"));
            return;
        }
        qDebug() << "requestCreator NO ACCESS: The network accessibility cannot be determined.";
        emit serverFailed(tr("No Network Access"));
        return;
    }

    QString uri;
    uri = "https://tipesoft.com/dl/api/dicotae/creators.json";
    qDebug() << "requestCreator uri:" << uri;

    QNetworkRequest request(uri);

    // to avoid ssl errors:
//    QSslConfiguration conf = request.sslConfiguration();
//    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
//    request.setSslConfiguration(conf);

    QNetworkReply* reply = networkAccessManager->get(request);
    bool connectResult = connect(reply, SIGNAL(finished()), this, SLOT(onFinishedCreator()));
    Q_ASSERT(connectResult);
    Q_UNUSED(connectResult);
}

// SLOTS
void DataServer::onFinishedDictionary()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if(!reply) {
        qWarning() << "Dictionary REPLY is NULL";
        emit serverFailed(tr("No Network Reply"));
        return;
    }
    const qint64 available = reply->bytesAvailable();
    if(available == 0) {
        qWarning() << "Dictionary: No Bytes received";
        emit serverFailed(tr("No Dictionary Data received"));
        return;
    }
    int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "dictionary HTTP STATUS: " << httpStatusCode << " Bytes: " << available;
    if(httpStatusCode != 200) {
        qDebug() << "Dictionary Status Code not 200";
        emit serverFailed(tr("No sucess getting Dictionary from Server. Got HTTP Status ")+QString::number(httpStatusCode));
        return;
    }
    QString dictionaryFilePath = mRepositoryDataPath+"dictionary.json";
    QFile saveFile(dictionaryFilePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open file to write " << dictionaryFilePath;
        emit serverFailed(tr("Dictionary Data cannot be written"));
        return;
    }
    qint64 bytesWritten = saveFile.write(reply->readAll());
    saveFile.close();
    qDebug() << "Dictionary Data Bytes written: " << bytesWritten << " to: " << dictionaryFilePath;
    // now getting the creator data
    requestCreator();
}

void DataServer::onFinishedCreator()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if(!reply) {
        qWarning() << "Creator REPLY is NULL";
        emit serverFailed(tr("No Network Reply"));
        return;
    }
    const qint64 available = reply->bytesAvailable();
    if(available == 0) {
        qWarning() << "Creator No Bytes received";
        emit serverFailed(tr("No Creator Data received"));
        return;
    }
    int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "Creator HTTP STATUS: " << httpStatusCode << " Bytes: " << available;
    if(httpStatusCode != 200) {
        qDebug() << "Creator Status Code not 200";
        emit serverFailed(tr("No sucess getting Creator from Server. Got HTTP Status ")+QString::number(httpStatusCode));
        return;
    }
    QString creatorFilePath = mRepositoryDataPath+"creator.json";
    QFile saveFile(creatorFilePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open file to write " << creatorFilePath;
        emit serverFailed(tr("Creator Data cannot be written"));
        return;
    }
    qint64 bytesWritten = saveFile.write(reply->readAll());
    saveFile.close();
    qDebug() << "Data Bytes written: " << bytesWritten << " to: " << creatorFilePath;
    emit serverSuccess();
}

void DataServer::onFinishedVersion()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if(!reply) {
        qWarning() << "Version REPLY is NULL";
        emit versionFailed(tr("No Network Reply"));
        return;
    }
    const qint64 available = reply->bytesAvailable();
    if(available == 0) {
        qWarning() << "Version No Bytes received";
        emit versionFailed(tr("No Version Data received"));
        return;
    }
    int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "Version HTTP STATUS: " << httpStatusCode << " Bytes: " << available;
    if(httpStatusCode != 200) {
        qDebug() << "Version Status Code not 200";
        emit versionFailed(tr("No sucess getting Version from Server. Got HTTP Status ")+QString::number(httpStatusCode));
        return;
    }
    emit versionSuccess(reply->readAll());
}


