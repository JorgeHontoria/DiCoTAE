#include <QObject>

#include "DataManager.hpp"

#include <QtQml>
#include <QJsonObject>
#include <QFile>


static const QString PRODUCTION_ENVIRONMENT = "prod/";
static const QString TEST_ENVIRONMENT = "test/";
static bool isProductionEnvironment = true;


static const QString cacheSettingsData = "cacheSettingsData.json";
static const QString cacheRepository = "cacheRepository.json";
static const QString cacheDictionary = "cacheDictionary.json";
static const QString cacheCategory = "cacheCategory.json";
static const QString cacheSection = "cacheSection.json";
static const QString cacheKey = "cacheKey.json";
static const QString cacheDictionaryItem = "cacheDictionaryItem.json";
static const QString cacheFavorite = "cacheFavorite.json";
static const QString cacheBookmark = "cacheBookmark.json";
static const QString cacheKeyLists = "cacheKeyLists.json";
static const QString cacheKeyLink = "cacheKeyLink.json";
static const QString cacheCreator = "cacheCreator.json";
static const QString cacheCreatorImage = "cacheCreatorImage.json";
static const QString cacheKeyTag = "cacheKeyTag.json";
static const QString cacheKeyContentBlock = "cacheKeyContentBlock.json";
static const QString cacheKeyContentStandardType = "cacheKeyContentStandardType.json";
static const QString cacheDay = "cacheDay.json";
static const QString cacheKeyAPI = "cacheKeyAPI.json";
static const QString cachePersonsAPI = "cachePersonsAPI.json";
static const QString cacheKeyLinkAPI = "cacheKeyLinkAPI.json";
static const QString cacheCreatorAPI = "cacheCreatorAPI.json";


DataManager::DataManager(QObject *parent) :
        QObject(parent)
{
   // Android: HomeLocation works, iOS: not writable
    // Android: AppDataLocation works out of the box, iOS you must create the DIR first !!
    mDataRoot = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).value(0);
    mDataPath = mDataRoot+"/data/";
    mDataAssetsPath = ":/data-assets/";
    qDebug() << "Data Path: " << mDataPath << " data-assets: " << mDataAssetsPath;
    // guarantee that dirs exist
    bool ok = checkDirs();
    if(!ok) {
        qFatal("App won't work - cannot create data directory");
    }

    // at first read settingsData (always from Sandbox)
    mSettingsPath = mDataRoot+"/"+cacheSettingsData;
    qDebug() << "Settings Path: " << mSettingsPath;
    readSettings();

#ifdef QT_DEBUG
    qDebug() << "Running a DEBUG BUILD";
    // DEBUG MODE ?
    // now check if public cache is used
    if (mSettingsData->hasPublicCache()) {
        // great while testing: access files from file explorer
        // only works on Android - on iOS it helps to use a 2nd cache for dev
        mDataRoot = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).value(0);
        // per ex. /data/msolanogadea/theAppName
        mDataRoot += mSettingsData->publicRoot4Dev();
        mDataPath = mDataRoot+"/data/";
        ok = checkDirs();
        if(!ok) {
            qFatal("App won't work - cannot create data directory");
        }
        qDebug() << "Data Path redirected to PUBLIC CACHE: " << mDataPath;
        // tip: copy settingsData to public cache to see the content
        // but settings will always be used from AppDataLocation
    }
#else
    qDebug() << "Running a RELEASE BUILD";
    // always use compact JSON in release builds
    mSettingsData->setUseCompactJsonFormat(true);
    // never use public data path in releae build
    mSettingsData->setHasPublicCache(false);
#endif
    // now set the compact or indent mode for JSON Documents
    mCompactJson = mSettingsData->useCompactJsonFormat();
	isProductionEnvironment = mSettingsData->isProductionEnvironment();

    // ApplicationUI is parent of DataManager
    // DataManager is parent of all root DataObjects
    // ROOT DataObjects are parent of contained DataObjects
    // ROOT:
    // SettingsData
    // Repository
    // Dictionary
    // Category
    // Section
    // Key
    // DictionaryItem
    // Favorite
    // Bookmark
    // KeyLists
    // KeyLink
    // Creator
    // CreatorImage
    // KeyTag
    // KeyContentBlock
    // KeyContentStandardType
    // Day
    // KeyAPI
    // PersonsAPI
    // KeyLinkAPI
    // CreatorAPI

    // register all DataObjects to get access to properties from QML:
    qmlRegisterType<SettingsData>("org.msolanogadea.data", 1, 0, "SettingsData");
    qmlRegisterType<Repository>("org.msolanogadea.data", 1, 0, "Repository");
    qmlRegisterType<Dictionary>("org.msolanogadea.data", 1, 0, "Dictionary");
    qmlRegisterType<Category>("org.msolanogadea.data", 1, 0, "Category");
    qmlRegisterType<Section>("org.msolanogadea.data", 1, 0, "Section");
    qmlRegisterType<Key>("org.msolanogadea.data", 1, 0, "Key");
    qmlRegisterType<DictionaryItem>("org.msolanogadea.data", 1, 0, "DictionaryItem");
    qmlRegisterType<Favorite>("org.msolanogadea.data", 1, 0, "Favorite");
    qmlRegisterType<Bookmark>("org.msolanogadea.data", 1, 0, "Bookmark");
    qmlRegisterType<KeyLists>("org.msolanogadea.data", 1, 0, "KeyLists");
    qmlRegisterType<KeyLink>("org.msolanogadea.data", 1, 0, "KeyLink");
    qmlRegisterType<Creator>("org.msolanogadea.data", 1, 0, "Creator");
    qmlRegisterType<CreatorImage>("org.msolanogadea.data", 1, 0, "CreatorImage");
    qmlRegisterType<KeyTag>("org.msolanogadea.data", 1, 0, "KeyTag");
    qmlRegisterType<KeyContentBlock>("org.msolanogadea.data", 1, 0, "KeyContentBlock");
    qmlRegisterType<KeyContentStandardType>("org.msolanogadea.data", 1, 0, "KeyContentStandardType");
    qmlRegisterType<Day>("org.msolanogadea.data", 1, 0, "Day");
    qmlRegisterType<KeyAPI>("org.msolanogadea.data", 1, 0, "KeyAPI");
    qmlRegisterType<PersonsAPI>("org.msolanogadea.data", 1, 0, "PersonsAPI");
    qmlRegisterType<KeyLinkAPI>("org.msolanogadea.data", 1, 0, "KeyLinkAPI");
    qmlRegisterType<CreatorAPI>("org.msolanogadea.data", 1, 0, "CreatorAPI");
	// register all ENUMs to get access from QML
	
	
	
}

QString DataManager::dataAssetsPath(const QString& fileName)
{
    return mDataAssetsPath + (isProductionEnvironment?PRODUCTION_ENVIRONMENT:TEST_ENVIRONMENT) + fileName;
}
QString DataManager::dataPath(const QString& fileName)
{
    return mDataPath + (isProductionEnvironment?PRODUCTION_ENVIRONMENT:TEST_ENVIRONMENT) + fileName;
}

bool DataManager::checkDirs()
{
    QDir myDir;
    bool exists;
    exists = myDir.exists(mDataRoot);
    if (!exists) {
        bool ok = myDir.mkpath(mDataRoot);
        if(!ok) {
            qWarning() << "Couldn't create mDataRoot " << mDataRoot;
            return false;
        }
        qDebug() << "created directory mDataRoot " << mDataRoot;
    }

    exists = myDir.exists(mDataPath);
    if (!exists) {
        bool ok = myDir.mkpath(mDataPath);
        if(!ok) {
            qWarning() << "Couldn't create mDataPath " << mDataPath;
            return false;
        }
        qDebug() << "created directory mDataPath"  << mDataRoot;
    }

    exists = myDir.exists(mDataPath+PRODUCTION_ENVIRONMENT);
    if (!exists) {
        bool ok = myDir.mkpath(mDataPath+PRODUCTION_ENVIRONMENT);
        if(!ok) {
            qWarning() << "Couldn't create /data/prod " << mDataPath+PRODUCTION_ENVIRONMENT;
            return false;
        }
        qDebug() << "created directory /data/prod " << mDataPath+PRODUCTION_ENVIRONMENT;
    }
    exists = myDir.exists(mDataPath+TEST_ENVIRONMENT);
    if (!exists) {
        bool ok = myDir.mkpath(mDataPath+TEST_ENVIRONMENT);
        if(!ok) {
            qWarning() << "Couldn't create /data/test " << mDataPath+TEST_ENVIRONMENT;
            return false;
        }
        qDebug() << "created directory /data/test " << mDataPath+TEST_ENVIRONMENT;
    }
    return true;
}

/*
 * loads all data from cache.
 * called from main.qml with delay using QTimer
 * Data with 2PhaseInit Caching Policy will only
 * load priority records needed to resolve from others
 */
void DataManager::init()
{
    // get all from cache

    initRepositoryFromCache();
    initDictionaryFromCache();
    initCategoryFromCache();
    initSectionFromCache();
    initKeyFromCache();
    initDictionaryItemFromCache();
    initFavoriteFromCache();
    initBookmarkFromCache();
    // KeyLists is transient - not automatically read from cache
    initKeyLinkFromCache();
    initCreatorFromCache();
    initCreatorImageFromCache();
    initKeyTagFromCache();
    initKeyContentBlockFromCache();
    initKeyContentStandardTypeFromCache();
    initDayFromCache();
    // KeyAPI is transient - not automatically read from cache
    // PersonsAPI is transient - not automatically read from cache
    // KeyLinkAPI is transient - not automatically read from cache
    // CreatorAPI is transient - not automatically read from cache
}



void DataManager::finish()
{
    saveSettings();
    // Repository is read-only - not saved to cache
    // Dictionary is read-only - not saved to cache
    // Category is read-only - not saved to cache
    // Section is read-only - not saved to cache
    // Key is read-only - not saved to cache
    // DictionaryItem is read-only - not saved to cache
    saveFavoriteToCache();
    saveBookmarkToCache();
    // KeyLists is read-only - not saved to cache
    // KeyLink is read-only - not saved to cache
    // Creator is read-only - not saved to cache
    // CreatorImage is read-only - not saved to cache
    // KeyTag is read-only - not saved to cache
    // KeyContentBlock is read-only - not saved to cache
    // KeyContentStandardType is read-only - not saved to cache
    // Day is read-only - not saved to cache
    // KeyAPI is read-only - not saved to cache
    // PersonsAPI is read-only - not saved to cache
    // KeyLinkAPI is read-only - not saved to cache
    // CreatorAPI is read-only - not saved to cache
}







/**
 * creates a new SettingsData
 * parent is DataManager
 * if data is successfully entered you must insertSettingsData
 * if edit was canceled you must undoCreateSettingsData to free up memory
 */
SettingsData* DataManager::createSettingsData()
{
    SettingsData* settingsData;
    settingsData = new SettingsData();
    settingsData->setParent(this);
    settingsData->prepareNew();
    return settingsData;
}

/**
 * deletes SettingsData
 * if createSettingsData was canceled from UI
 * to delete a previous successfully inserted SettingsData
 * use deleteSettingsData
 */
void DataManager::undoCreateSettingsData(SettingsData* settingsData)
{
    if (settingsData) {
        qDebug() << "undoCreateSettingsData " << settingsData->id();
        settingsData->deleteLater();
        settingsData = 0;
    }
}




/*
 * reads Maps of Repository in from JSON cache
 * creates List of Repository*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initRepositoryFromCache()
{
	qDebug() << "start initRepositoryFromCache";
    mAllRepository.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheRepository);
    qDebug() << "read Repository from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Repository* repository = new Repository();
        // Important: DataManager must be parent of all root DTOs
        repository->setParent(this);
        repository->fillFromCacheMap(cacheMap);
        mAllRepository.append(repository);
    }
    qDebug() << "created Repository* #" << mAllRepository.size();
}


/*
 * save List of Repository* to JSON cache
 * convert list of Repository* to QVariantList
 * toCacheMap stores all properties without transient values
 * Repository is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveRepositoryToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Repository* #" << mAllRepository.size();
    for (int i = 0; i < mAllRepository.size(); ++i) {
        Repository* repository;
        repository = (Repository*)mAllRepository.at(i);
        QVariantMap cacheMap;
        cacheMap = repository->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Repository* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheRepository, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Repository*> DataManager::listOfRepositoryForKeys(
        QStringList keyList)
{
    QList<Repository*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllRepository.size(); ++i) {
        Repository* repository;
        repository = (Repository*) mAllRepository.at(i);
        if (keyList.contains(QString::number(repository->id()))) {
            listOfData.append(repository);
            keyList.removeOne(QString::number(repository->id()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Repository: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::repositoryAsQVariantList()
{
    QVariantList repositoryList;
    for (int i = 0; i < mAllRepository.size(); ++i) {
        repositoryList.append(((Repository*) (mAllRepository.at(i)))->toMap());
    }
    return repositoryList;
}

QList<QObject*> DataManager::allRepository()
{
    return mAllRepository;
}

QQmlListProperty<Repository> DataManager::repositoryPropertyList()
{
    return QQmlListProperty<Repository>(this, 0,
            &DataManager::appendToRepositoryProperty, &DataManager::repositoryPropertyCount,
            &DataManager::atRepositoryProperty, &DataManager::clearRepositoryProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Repository*
void DataManager::appendToRepositoryProperty(
        QQmlListProperty<Repository> *repositoryList,
        Repository* repository)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(repositoryList->object);
    if (dataManagerObject) {
        repository->setParent(dataManagerObject);
        dataManagerObject->mAllRepository.append(repository);
        emit dataManagerObject->addedToAllRepository(repository);
    } else {
        qWarning() << "cannot append Repository* to mAllRepository "
                << "Object is not of type DataManager*";
    }
}
int DataManager::repositoryPropertyCount(
        QQmlListProperty<Repository> *repositoryList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(repositoryList->object);
    if (dataManager) {
        return dataManager->mAllRepository.size();
    } else {
        qWarning() << "cannot get size mAllRepository " << "Object is not of type DataManager*";
    }
    return 0;
}
Repository* DataManager::atRepositoryProperty(
        QQmlListProperty<Repository> *repositoryList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(repositoryList->object);
    if (dataManager) {
        if (dataManager->mAllRepository.size() > pos) {
            return (Repository*) dataManager->mAllRepository.at(pos);
        }
        qWarning() << "cannot get Repository* at pos " << pos << " size is "
                << dataManager->mAllRepository.size();
    } else {
        qWarning() << "cannot get Repository* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearRepositoryProperty(
        QQmlListProperty<Repository> *repositoryList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(repositoryList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllRepository.size(); ++i) {
            Repository* repository;
            repository = (Repository*) dataManager->mAllRepository.at(i);
			emit dataManager->deletedFromAllRepositoryById(repository->id());
			emit dataManager->deletedFromAllRepository(repository);
            repository->deleteLater();
            repository = 0;
        }
        dataManager->mAllRepository.clear();
    } else {
        qWarning() << "cannot clear mAllRepository " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Repository
 * and clears the list
 */
void DataManager::deleteRepository()
{
    for (int i = 0; i < mAllRepository.size(); ++i) {
        Repository* repository;
        repository = (Repository*) mAllRepository.at(i);
        emit deletedFromAllRepositoryById(repository->id());
		emit deletedFromAllRepository(repository);
		emit repositoryPropertyListChanged();
        repository->deleteLater();
        repository = 0;
     }
     mAllRepository.clear();
}

/**
 * creates a new Repository
 * parent is DataManager
 * if data is successfully entered you must insertRepository
 * if edit was canceled you must undoCreateRepository to free up memory
 */
Repository* DataManager::createRepository()
{
    Repository* repository;
    repository = new Repository();
    repository->setParent(this);
    repository->prepareNew();
    return repository;
}

/**
 * deletes Repository
 * if createRepository was canceled from UI
 * to delete a previous successfully inserted Repository
 * use deleteRepository
 */
void DataManager::undoCreateRepository(Repository* repository)
{
    if (repository) {
        qDebug() << "undoCreateRepository " << repository->id();
        repository->deleteLater();
        repository = 0;
    }
}

void DataManager::insertRepository(Repository* repository)
{
    // Important: DataManager must be parent of all root DTOs
    repository->setParent(this);
    mAllRepository.append(repository);
    emit addedToAllRepository(repository);
    emit repositoryPropertyListChanged();
}

void DataManager::insertRepositoryFromMap(const QVariantMap& repositoryMap,
        const bool& useForeignProperties)
{
    Repository* repository = new Repository();
    repository->setParent(this);
    if (useForeignProperties) {
        repository->fillFromForeignMap(repositoryMap);
    } else {
        repository->fillFromMap(repositoryMap);
    }
    mAllRepository.append(repository);
    emit addedToAllRepository(repository);
    repositoryPropertyListChanged();
}

bool DataManager::deleteRepository(Repository* repository)
{
    bool ok = false;
    ok = mAllRepository.removeOne(repository);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllRepositoryById(repository->id());
    emit deletedFromAllRepository(repository);
    emit repositoryPropertyListChanged();
    repository->deleteLater();
    repository = 0;
    return ok;
}


bool DataManager::deleteRepositoryById(const int& id)
{
    for (int i = 0; i < mAllRepository.size(); ++i) {
        Repository* repository;
        repository = (Repository*) mAllRepository.at(i);
        if (repository->id() == id) {
            mAllRepository.removeAt(i);
            emit deletedFromAllRepositoryById(id);
            emit deletedFromAllRepository(repository);
            emit repositoryPropertyListChanged();
            repository->deleteLater();
            repository = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Repository* DataManager::findRepositoryById(const int& id){
    for (int i = 0; i < mAllRepository.size(); ++i) {
        Repository* repository;
        repository = (Repository*)mAllRepository.at(i);
        if(repository->id() == id){
            return repository;
        }
    }
    qDebug() << "no Repository found for id " << id;
    return 0;
}

/*
 * reads Maps of Dictionary in from JSON cache
 * creates List of Dictionary*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initDictionaryFromCache()
{
	qDebug() << "start initDictionaryFromCache";
    mAllDictionary.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheDictionary);
    qDebug() << "read Dictionary from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Dictionary* dictionary = new Dictionary();
        // Important: DataManager must be parent of all root DTOs
        dictionary->setParent(this);
        dictionary->fillFromCacheMap(cacheMap);
        mAllDictionary.append(dictionary);
    }
    qDebug() << "created Dictionary* #" << mAllDictionary.size();
}


/*
 * save List of Dictionary* to JSON cache
 * convert list of Dictionary* to QVariantList
 * toCacheMap stores all properties without transient values
 * Dictionary is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveDictionaryToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Dictionary* #" << mAllDictionary.size();
    for (int i = 0; i < mAllDictionary.size(); ++i) {
        Dictionary* dictionary;
        dictionary = (Dictionary*)mAllDictionary.at(i);
        QVariantMap cacheMap;
        cacheMap = dictionary->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Dictionary* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheDictionary, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Dictionary*> DataManager::listOfDictionaryForKeys(
        QStringList keyList)
{
    QList<Dictionary*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllDictionary.size(); ++i) {
        Dictionary* dictionary;
        dictionary = (Dictionary*) mAllDictionary.at(i);
        if (keyList.contains(QString::number(dictionary->dictionaryId()))) {
            listOfData.append(dictionary);
            keyList.removeOne(QString::number(dictionary->dictionaryId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Dictionary: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::dictionaryAsQVariantList()
{
    QVariantList dictionaryList;
    for (int i = 0; i < mAllDictionary.size(); ++i) {
        dictionaryList.append(((Dictionary*) (mAllDictionary.at(i)))->toMap());
    }
    return dictionaryList;
}

QList<QObject*> DataManager::allDictionary()
{
    return mAllDictionary;
}

QQmlListProperty<Dictionary> DataManager::dictionaryPropertyList()
{
    return QQmlListProperty<Dictionary>(this, 0,
            &DataManager::appendToDictionaryProperty, &DataManager::dictionaryPropertyCount,
            &DataManager::atDictionaryProperty, &DataManager::clearDictionaryProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Dictionary*
void DataManager::appendToDictionaryProperty(
        QQmlListProperty<Dictionary> *dictionaryList,
        Dictionary* dictionary)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(dictionaryList->object);
    if (dataManagerObject) {
        dictionary->setParent(dataManagerObject);
        dataManagerObject->mAllDictionary.append(dictionary);
        emit dataManagerObject->addedToAllDictionary(dictionary);
    } else {
        qWarning() << "cannot append Dictionary* to mAllDictionary "
                << "Object is not of type DataManager*";
    }
}
int DataManager::dictionaryPropertyCount(
        QQmlListProperty<Dictionary> *dictionaryList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(dictionaryList->object);
    if (dataManager) {
        return dataManager->mAllDictionary.size();
    } else {
        qWarning() << "cannot get size mAllDictionary " << "Object is not of type DataManager*";
    }
    return 0;
}
Dictionary* DataManager::atDictionaryProperty(
        QQmlListProperty<Dictionary> *dictionaryList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(dictionaryList->object);
    if (dataManager) {
        if (dataManager->mAllDictionary.size() > pos) {
            return (Dictionary*) dataManager->mAllDictionary.at(pos);
        }
        qWarning() << "cannot get Dictionary* at pos " << pos << " size is "
                << dataManager->mAllDictionary.size();
    } else {
        qWarning() << "cannot get Dictionary* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearDictionaryProperty(
        QQmlListProperty<Dictionary> *dictionaryList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(dictionaryList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllDictionary.size(); ++i) {
            Dictionary* dictionary;
            dictionary = (Dictionary*) dataManager->mAllDictionary.at(i);
			emit dataManager->deletedFromAllDictionaryByDictionaryId(dictionary->dictionaryId());
			emit dataManager->deletedFromAllDictionary(dictionary);
            dictionary->deleteLater();
            dictionary = 0;
        }
        dataManager->mAllDictionary.clear();
    } else {
        qWarning() << "cannot clear mAllDictionary " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Dictionary
 * and clears the list
 */
void DataManager::deleteDictionary()
{
    for (int i = 0; i < mAllDictionary.size(); ++i) {
        Dictionary* dictionary;
        dictionary = (Dictionary*) mAllDictionary.at(i);
        emit deletedFromAllDictionaryByDictionaryId(dictionary->dictionaryId());
		emit deletedFromAllDictionary(dictionary);
		emit dictionaryPropertyListChanged();
        dictionary->deleteLater();
        dictionary = 0;
     }
     mAllDictionary.clear();
}

/**
 * creates a new Dictionary
 * parent is DataManager
 * if data is successfully entered you must insertDictionary
 * if edit was canceled you must undoCreateDictionary to free up memory
 */
Dictionary* DataManager::createDictionary()
{
    Dictionary* dictionary;
    dictionary = new Dictionary();
    dictionary->setParent(this);
    dictionary->prepareNew();
    return dictionary;
}

/**
 * deletes Dictionary
 * if createDictionary was canceled from UI
 * to delete a previous successfully inserted Dictionary
 * use deleteDictionary
 */
void DataManager::undoCreateDictionary(Dictionary* dictionary)
{
    if (dictionary) {
        qDebug() << "undoCreateDictionary " << dictionary->dictionaryId();
        dictionary->deleteLater();
        dictionary = 0;
    }
}

void DataManager::insertDictionary(Dictionary* dictionary)
{
    // Important: DataManager must be parent of all root DTOs
    dictionary->setParent(this);
    mAllDictionary.append(dictionary);
    emit addedToAllDictionary(dictionary);
    emit dictionaryPropertyListChanged();
}

void DataManager::insertDictionaryFromMap(const QVariantMap& dictionaryMap,
        const bool& useForeignProperties)
{
    Dictionary* dictionary = new Dictionary();
    dictionary->setParent(this);
    if (useForeignProperties) {
        dictionary->fillFromForeignMap(dictionaryMap);
    } else {
        dictionary->fillFromMap(dictionaryMap);
    }
    mAllDictionary.append(dictionary);
    emit addedToAllDictionary(dictionary);
    dictionaryPropertyListChanged();
}

bool DataManager::deleteDictionary(Dictionary* dictionary)
{
    bool ok = false;
    ok = mAllDictionary.removeOne(dictionary);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllDictionaryByDictionaryId(dictionary->dictionaryId());
    emit deletedFromAllDictionary(dictionary);
    emit dictionaryPropertyListChanged();
    dictionary->deleteLater();
    dictionary = 0;
    return ok;
}


bool DataManager::deleteDictionaryByDictionaryId(const int& dictionaryId)
{
    for (int i = 0; i < mAllDictionary.size(); ++i) {
        Dictionary* dictionary;
        dictionary = (Dictionary*) mAllDictionary.at(i);
        if (dictionary->dictionaryId() == dictionaryId) {
            mAllDictionary.removeAt(i);
            emit deletedFromAllDictionaryByDictionaryId(dictionaryId);
            emit deletedFromAllDictionary(dictionary);
            emit dictionaryPropertyListChanged();
            dictionary->deleteLater();
            dictionary = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Dictionary* DataManager::findDictionaryByDictionaryId(const int& dictionaryId){
    for (int i = 0; i < mAllDictionary.size(); ++i) {
        Dictionary* dictionary;
        dictionary = (Dictionary*)mAllDictionary.at(i);
        if(dictionary->dictionaryId() == dictionaryId){
            return dictionary;
        }
    }
    qDebug() << "no Dictionary found for dictionaryId " << dictionaryId;
    return 0;
}

/*
 * reads Maps of Category in from JSON cache
 * creates List of Category*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initCategoryFromCache()
{
	qDebug() << "start initCategoryFromCache";
    mAllCategory.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheCategory);
    qDebug() << "read Category from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Category* category = new Category();
        // Important: DataManager must be parent of all root DTOs
        category->setParent(this);
        category->fillFromCacheMap(cacheMap);
        mAllCategory.append(category);
    }
    qDebug() << "created Category* #" << mAllCategory.size();
}


/*
 * save List of Category* to JSON cache
 * convert list of Category* to QVariantList
 * toCacheMap stores all properties without transient values
 * Category is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveCategoryToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Category* #" << mAllCategory.size();
    for (int i = 0; i < mAllCategory.size(); ++i) {
        Category* category;
        category = (Category*)mAllCategory.at(i);
        QVariantMap cacheMap;
        cacheMap = category->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Category* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheCategory, cacheList);
}


void DataManager::resolveCategoryReferences(Category* category)
{
	if (!category) {
        qDebug() << "cannot resolveCategoryReferences with category NULL";
        return;
    }
    if(category->isAllResolved()) {
	    qDebug() << "nothing to do: all is resolved";
	    return;
	}
    if (category->hasDictionary() && !category->isDictionaryResolvedAsDataObject()) {
    	Dictionary* dictionary;
   		dictionary = findDictionaryByDictionaryId(category->dictionary());
    	if (dictionary) {
    		category->resolveDictionaryAsDataObject(dictionary);
    	} else {
    		qDebug() << "markDictionaryAsInvalid: " << category->dictionary();
    		category->markDictionaryAsInvalid();
    	}
    }
    if (!category->areSectionsKeysResolved()) {
        category->resolveSectionsKeys(
                listOfSectionForKeys(category->sectionsKeys()));
    }
}

void DataManager::resolveReferencesForAllCategory()
{
    for (int i = 0; i < mAllCategory.size(); ++i) {
        Category* category;
        category = (Category*)mAllCategory.at(i);
    	resolveCategoryReferences(category);
    }
}


/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Category*> DataManager::listOfCategoryForKeys(
        QStringList keyList)
{
    QList<Category*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllCategory.size(); ++i) {
        Category* category;
        category = (Category*) mAllCategory.at(i);
        if (keyList.contains(QString::number(category->categoryId()))) {
            listOfData.append(category);
            keyList.removeOne(QString::number(category->categoryId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Category: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::categoryAsQVariantList()
{
    QVariantList categoryList;
    for (int i = 0; i < mAllCategory.size(); ++i) {
        categoryList.append(((Category*) (mAllCategory.at(i)))->toMap());
    }
    return categoryList;
}

QList<QObject*> DataManager::allCategory()
{
    return mAllCategory;
}

QQmlListProperty<Category> DataManager::categoryPropertyList()
{
    return QQmlListProperty<Category>(this, 0,
            &DataManager::appendToCategoryProperty, &DataManager::categoryPropertyCount,
            &DataManager::atCategoryProperty, &DataManager::clearCategoryProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Category*
void DataManager::appendToCategoryProperty(
        QQmlListProperty<Category> *categoryList,
        Category* category)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(categoryList->object);
    if (dataManagerObject) {
        category->setParent(dataManagerObject);
        dataManagerObject->mAllCategory.append(category);
        emit dataManagerObject->addedToAllCategory(category);
    } else {
        qWarning() << "cannot append Category* to mAllCategory "
                << "Object is not of type DataManager*";
    }
}
int DataManager::categoryPropertyCount(
        QQmlListProperty<Category> *categoryList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(categoryList->object);
    if (dataManager) {
        return dataManager->mAllCategory.size();
    } else {
        qWarning() << "cannot get size mAllCategory " << "Object is not of type DataManager*";
    }
    return 0;
}
Category* DataManager::atCategoryProperty(
        QQmlListProperty<Category> *categoryList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(categoryList->object);
    if (dataManager) {
        if (dataManager->mAllCategory.size() > pos) {
            return (Category*) dataManager->mAllCategory.at(pos);
        }
        qWarning() << "cannot get Category* at pos " << pos << " size is "
                << dataManager->mAllCategory.size();
    } else {
        qWarning() << "cannot get Category* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearCategoryProperty(
        QQmlListProperty<Category> *categoryList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(categoryList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllCategory.size(); ++i) {
            Category* category;
            category = (Category*) dataManager->mAllCategory.at(i);
			emit dataManager->deletedFromAllCategoryByCategoryId(category->categoryId());
			emit dataManager->deletedFromAllCategory(category);
            category->deleteLater();
            category = 0;
        }
        dataManager->mAllCategory.clear();
    } else {
        qWarning() << "cannot clear mAllCategory " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Category
 * and clears the list
 */
void DataManager::deleteCategory()
{
    for (int i = 0; i < mAllCategory.size(); ++i) {
        Category* category;
        category = (Category*) mAllCategory.at(i);
        emit deletedFromAllCategoryByCategoryId(category->categoryId());
		emit deletedFromAllCategory(category);
		emit categoryPropertyListChanged();
        category->deleteLater();
        category = 0;
     }
     mAllCategory.clear();
}

/**
 * creates a new Category
 * parent is DataManager
 * if data is successfully entered you must insertCategory
 * if edit was canceled you must undoCreateCategory to free up memory
 */
Category* DataManager::createCategory()
{
    Category* category;
    category = new Category();
    category->setParent(this);
    category->prepareNew();
    return category;
}

/**
 * deletes Category
 * if createCategory was canceled from UI
 * to delete a previous successfully inserted Category
 * use deleteCategory
 */
void DataManager::undoCreateCategory(Category* category)
{
    if (category) {
        qDebug() << "undoCreateCategory " << category->categoryId();
        category->deleteLater();
        category = 0;
    }
}

void DataManager::insertCategory(Category* category)
{
    // Important: DataManager must be parent of all root DTOs
    category->setParent(this);
    mAllCategory.append(category);
    emit addedToAllCategory(category);
    emit categoryPropertyListChanged();
}

void DataManager::insertCategoryFromMap(const QVariantMap& categoryMap,
        const bool& useForeignProperties)
{
    Category* category = new Category();
    category->setParent(this);
    if (useForeignProperties) {
        category->fillFromForeignMap(categoryMap);
    } else {
        category->fillFromMap(categoryMap);
    }
    mAllCategory.append(category);
    emit addedToAllCategory(category);
    categoryPropertyListChanged();
}

bool DataManager::deleteCategory(Category* category)
{
    bool ok = false;
    ok = mAllCategory.removeOne(category);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllCategoryByCategoryId(category->categoryId());
    emit deletedFromAllCategory(category);
    emit categoryPropertyListChanged();
    category->deleteLater();
    category = 0;
    return ok;
}


bool DataManager::deleteCategoryByCategoryId(const int& categoryId)
{
    for (int i = 0; i < mAllCategory.size(); ++i) {
        Category* category;
        category = (Category*) mAllCategory.at(i);
        if (category->categoryId() == categoryId) {
            mAllCategory.removeAt(i);
            emit deletedFromAllCategoryByCategoryId(categoryId);
            emit deletedFromAllCategory(category);
            emit categoryPropertyListChanged();
            category->deleteLater();
            category = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Category* DataManager::findCategoryByCategoryId(const int& categoryId){
    for (int i = 0; i < mAllCategory.size(); ++i) {
        Category* category;
        category = (Category*)mAllCategory.at(i);
        if(category->categoryId() == categoryId){
            return category;
        }
    }
    qDebug() << "no Category found for categoryId " << categoryId;
    return 0;
}

/*
 * reads Maps of Section in from JSON cache
 * creates List of Section*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initSectionFromCache()
{
	qDebug() << "start initSectionFromCache";
    mAllSection.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheSection);
    qDebug() << "read Section from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Section* section = new Section();
        // Important: DataManager must be parent of all root DTOs
        section->setParent(this);
        section->fillFromCacheMap(cacheMap);
        mAllSection.append(section);
    }
    qDebug() << "created Section* #" << mAllSection.size();
}


/*
 * save List of Section* to JSON cache
 * convert list of Section* to QVariantList
 * toCacheMap stores all properties without transient values
 * Section is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveSectionToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Section* #" << mAllSection.size();
    for (int i = 0; i < mAllSection.size(); ++i) {
        Section* section;
        section = (Section*)mAllSection.at(i);
        QVariantMap cacheMap;
        cacheMap = section->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Section* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheSection, cacheList);
}

void DataManager::resolveSectionReferences(Section* section)
{
	if (!section) {
        qDebug() << "cannot resolveSectionReferences with section NULL";
        return;
    }
    if(section->isAllResolved()) {
	    qDebug() << "nothing to do: all is resolved";
	    return;
	}
    if (section->hasCategory() && !section->isCategoryResolvedAsDataObject()) {
    	Category* category;
   		category = findCategoryByCategoryId(section->category());
    	if (category) {
    		section->resolveCategoryAsDataObject(category);
    	} else {
    		qDebug() << "markCategoryAsInvalid: " << section->category();
    		section->markCategoryAsInvalid();
    	}
    }
    if (!section->areKeysKeysResolved()) {
        section->resolveKeysKeys(listOfKeyForKeys(section->keysKeys()));
    }
}

void DataManager::resolveReferencesForAllSection()
{
    for (int i = 0; i < mAllSection.size(); ++i) {
        Section* section;
        section = (Section*)mAllSection.at(i);
    	resolveSectionReferences(section);
    }
}


/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Section*> DataManager::listOfSectionForKeys(
        QStringList keyList)
{
    QList<Section*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllSection.size(); ++i) {
        Section* section;
        section = (Section*) mAllSection.at(i);
        if (keyList.contains(QString::number(section->sectionId()))) {
            listOfData.append(section);
            keyList.removeOne(QString::number(section->sectionId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Section: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::sectionAsQVariantList()
{
    QVariantList sectionList;
    for (int i = 0; i < mAllSection.size(); ++i) {
        sectionList.append(((Section*) (mAllSection.at(i)))->toMap());
    }
    return sectionList;
}

QList<QObject*> DataManager::allSection()
{
    return mAllSection;
}

QQmlListProperty<Section> DataManager::sectionPropertyList()
{
    return QQmlListProperty<Section>(this, 0,
            &DataManager::appendToSectionProperty, &DataManager::sectionPropertyCount,
            &DataManager::atSectionProperty, &DataManager::clearSectionProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Section*
void DataManager::appendToSectionProperty(
        QQmlListProperty<Section> *sectionList,
        Section* section)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(sectionList->object);
    if (dataManagerObject) {
        section->setParent(dataManagerObject);
        dataManagerObject->mAllSection.append(section);
        emit dataManagerObject->addedToAllSection(section);
    } else {
        qWarning() << "cannot append Section* to mAllSection "
                << "Object is not of type DataManager*";
    }
}
int DataManager::sectionPropertyCount(
        QQmlListProperty<Section> *sectionList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sectionList->object);
    if (dataManager) {
        return dataManager->mAllSection.size();
    } else {
        qWarning() << "cannot get size mAllSection " << "Object is not of type DataManager*";
    }
    return 0;
}
Section* DataManager::atSectionProperty(
        QQmlListProperty<Section> *sectionList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sectionList->object);
    if (dataManager) {
        if (dataManager->mAllSection.size() > pos) {
            return (Section*) dataManager->mAllSection.at(pos);
        }
        qWarning() << "cannot get Section* at pos " << pos << " size is "
                << dataManager->mAllSection.size();
    } else {
        qWarning() << "cannot get Section* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearSectionProperty(
        QQmlListProperty<Section> *sectionList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sectionList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllSection.size(); ++i) {
            Section* section;
            section = (Section*) dataManager->mAllSection.at(i);
			emit dataManager->deletedFromAllSectionBySectionId(section->sectionId());
			emit dataManager->deletedFromAllSection(section);
            section->deleteLater();
            section = 0;
        }
        dataManager->mAllSection.clear();
    } else {
        qWarning() << "cannot clear mAllSection " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Section
 * and clears the list
 */
void DataManager::deleteSection()
{
    for (int i = 0; i < mAllSection.size(); ++i) {
        Section* section;
        section = (Section*) mAllSection.at(i);
        emit deletedFromAllSectionBySectionId(section->sectionId());
		emit deletedFromAllSection(section);
		emit sectionPropertyListChanged();
        section->deleteLater();
        section = 0;
     }
     mAllSection.clear();
}

/**
 * creates a new Section
 * parent is DataManager
 * if data is successfully entered you must insertSection
 * if edit was canceled you must undoCreateSection to free up memory
 */
Section* DataManager::createSection()
{
    Section* section;
    section = new Section();
    section->setParent(this);
    section->prepareNew();
    return section;
}

/**
 * deletes Section
 * if createSection was canceled from UI
 * to delete a previous successfully inserted Section
 * use deleteSection
 */
void DataManager::undoCreateSection(Section* section)
{
    if (section) {
        qDebug() << "undoCreateSection " << section->sectionId();
        section->deleteLater();
        section = 0;
    }
}

void DataManager::insertSection(Section* section)
{
    // Important: DataManager must be parent of all root DTOs
    section->setParent(this);
    mAllSection.append(section);
    emit addedToAllSection(section);
    emit sectionPropertyListChanged();
}

void DataManager::insertSectionFromMap(const QVariantMap& sectionMap,
        const bool& useForeignProperties)
{
    Section* section = new Section();
    section->setParent(this);
    if (useForeignProperties) {
        section->fillFromForeignMap(sectionMap);
    } else {
        section->fillFromMap(sectionMap);
    }
    mAllSection.append(section);
    emit addedToAllSection(section);
    sectionPropertyListChanged();
}

bool DataManager::deleteSection(Section* section)
{
    bool ok = false;
    ok = mAllSection.removeOne(section);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllSectionBySectionId(section->sectionId());
    emit deletedFromAllSection(section);
    emit sectionPropertyListChanged();
    section->deleteLater();
    section = 0;
    return ok;
}


bool DataManager::deleteSectionBySectionId(const int& sectionId)
{
    for (int i = 0; i < mAllSection.size(); ++i) {
        Section* section;
        section = (Section*) mAllSection.at(i);
        if (section->sectionId() == sectionId) {
            mAllSection.removeAt(i);
            emit deletedFromAllSectionBySectionId(sectionId);
            emit deletedFromAllSection(section);
            emit sectionPropertyListChanged();
            section->deleteLater();
            section = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Section* DataManager::findSectionBySectionId(const int& sectionId){
    for (int i = 0; i < mAllSection.size(); ++i) {
        Section* section;
        section = (Section*)mAllSection.at(i);
        if(section->sectionId() == sectionId){
            return section;
        }
    }
    qDebug() << "no Section found for sectionId " << sectionId;
    return 0;
}

/*
 * reads Maps of Key in from JSON cache
 * creates List of Key*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initKeyFromCache()
{
    qDebug() << "start initKeyFromCache";
    mAllKey.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheKey);
    qDebug() << "read Key from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Key* key = new Key();
        // Important: DataManager must be parent of all root DTOs
        key->setParent(this);
        key->fillFromCacheMap(cacheMap);
        mAllKey.append(key);
    }
    qDebug() << "created Key* #" << mAllKey.size();
}


/*
 * save List of Key* to JSON cache
 * convert list of Key* to QVariantList
 * toCacheMap stores all properties without transient values
 * Key is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveKeyToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Key* #" << mAllKey.size();
    for (int i = 0; i < mAllKey.size(); ++i) {
        Key* key;
        key = (Key*)mAllKey.at(i);
        QVariantMap cacheMap;
        cacheMap = key->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Key* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheKey, cacheList);
}


void DataManager::resolveKeyReferences(Key* key)
{
	if (!key) {
        qDebug() << "cannot resolveKeyReferences with key NULL";
        return;
    }
    if(key->isAllResolved()) {
	    qDebug() << "nothing to do: all is resolved";
	    return;
	}
    if (key->hasKeyDay() && !key->isKeyDayResolvedAsDataObject()) {
    	Day* keyDay;
   		keyDay = findDayById(key->keyDay());
    	if (keyDay) {
            key->resolveKeyDayAsDataObject(keyDay);
    	} else {
            qDebug() << "markKeyDayAsInvalid: " << key->keyDay();
            key->markKeyDayAsInvalid();
    	}
    }
    if (key->hasSection() && !key->isSectionResolvedAsDataObject()) {
    	Section* section;
   		section = findSectionBySectionId(key->section());
    	if (section) {
    		key->resolveSectionAsDataObject(section);
    	} else {
    		qDebug() << "markSectionAsInvalid: " << key->section();
    		key->markSectionAsInvalid();
    	}
    }
    if (key->hasDictionaryItem() && !key->isDictionaryItemResolvedAsDataObject()) {
    	DictionaryItem* dictionaryItem;
        dictionaryItem = findDictionaryItemByKeyId(key->dictionaryItem());
    	if (dictionaryItem) {
    		key->resolveDictionaryItemAsDataObject(dictionaryItem);
    	} else {
    		qDebug() << "markDictionaryItemAsInvalid: " << key->dictionaryItem();
    		key->markDictionaryItemAsInvalid();
    	}
    }
    if (!key->areCreatorKeysResolved()) {
        key->resolveCreatorKeys(
                listOfCreatorForKeys(key->creatorKeys()));
    }
    if (!key->areKeyLinksKeysResolved()) {
        key->resolveKeyLinksKeys(
                listOfKeyLinkForKeys(key->keyLinksKeys()));
    }
    if (!key->areKeyTagsKeysResolved()) {
        key->resolveKeyTagsKeys(
                listOfKeyTagForKeys(key->keyTagsKeys()));
    }
    if (!key->areKeyContentBlocksKeysResolved()) {
        key->resolveKeyContentBlocksKeys(
                listOfKeyContentBlockForKeys(key->keyContentBlocksKeys()));
    }
    if (!key->areKeyContentStandardTypesKeysResolved()) {
        key->resolveKeyContentStandardTypesKeys(
                listOfKeyContentStandardTypeForKeys(key->keyContentStandardTypesKeys()));
    }
}

void DataManager::resolveReferencesForAllKey()
{
    for (int i = 0; i < mAllKey.size(); ++i) {
        Key* key;
        key = (Key*)mAllKey.at(i);
        resolveKeyReferences(key);
    }
}


/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Key*> DataManager::listOfKeyForKeys(
        QStringList keyList)
{
    QList<Key*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllKey.size(); ++i) {
        Key* key;
        key = (Key*) mAllKey.at(i);
        if (keyList.contains(QString::number(key->keyId()))) {
            listOfData.append(key);
            keyList.removeOne(QString::number(key->keyId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Key: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::keyAsQVariantList()
{
    QVariantList keyList;
    for (int i = 0; i < mAllKey.size(); ++i) {
        keyList.append(((Key*) (mAllKey.at(i)))->toMap());
    }
    return keyList;
}

QList<QObject*> DataManager::allKey()
{
    return mAllKey;
}

QQmlListProperty<Key> DataManager::keyPropertyList()
{
    return QQmlListProperty<Key>(this, 0,
            &DataManager::appendToKeyProperty, &DataManager::keyPropertyCount,
            &DataManager::atKeyProperty, &DataManager::clearKeyProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Key*
void DataManager::appendToKeyProperty(
        QQmlListProperty<Key> *keyList,
        Key* key)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(keyList->object);
    if (dataManagerObject) {
        key->setParent(dataManagerObject);
        dataManagerObject->mAllKey.append(key);
        emit dataManagerObject->addedToAllKey(key);
    } else {
        qWarning() << "cannot append Key* to mAllKey "
                << "Object is not of type DataManager*";
    }
}
int DataManager::keyPropertyCount(
        QQmlListProperty<Key> *keyList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyList->object);
    if (dataManager) {
        return dataManager->mAllKey.size();
    } else {
        qWarning() << "cannot get size mAllKey " << "Object is not of type DataManager*";
    }
    return 0;
}
Key* DataManager::atKeyProperty(
        QQmlListProperty<Key> *keyList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyList->object);
    if (dataManager) {
        if (dataManager->mAllKey.size() > pos) {
            return (Key*) dataManager->mAllKey.at(pos);
        }
        qWarning() << "cannot get Key* at pos " << pos << " size is "
                << dataManager->mAllKey.size();
    } else {
        qWarning() << "cannot get Key* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearKeyProperty(
        QQmlListProperty<Key> *keyList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllKey.size(); ++i) {
            Key* key;
            key = (Key*) dataManager->mAllKey.at(i);
            emit dataManager->deletedFromAllKeyByKeyId(key->keyId());
            emit dataManager->deletedFromAllKey(key);
            key->deleteLater();
            key = 0;
        }
        dataManager->mAllKey.clear();
    } else {
        qWarning() << "cannot clear mAllKey " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Key
 * and clears the list
 */
void DataManager::deleteKey()
{
    for (int i = 0; i < mAllKey.size(); ++i) {
        Key* key;
        key = (Key*) mAllKey.at(i);
        emit deletedFromAllKeyByKeyId(key->keyId());
        emit deletedFromAllKey(key);
		emit keyPropertyListChanged();
        key->deleteLater();
        key = 0;
     }
     mAllKey.clear();
}

/**
 * creates a new Key
 * parent is DataManager
 * if data is successfully entered you must insertKey
 * if edit was canceled you must undoCreateKey to free up memory
 */
Key* DataManager::createKey()
{
    Key* key;
    key = new Key();
    key->setParent(this);
    key->prepareNew();
    return key;
}

/**
 * deletes Key
 * if createKey was canceled from UI
 * to delete a previous successfully inserted Key
 * use deleteKey
 */
void DataManager::undoCreateKey(Key* key)
{
    if (key) {
        qDebug() << "undoCreateKey " << key->keyId();
        key->deleteLater();
        key = 0;
    }
}

void DataManager::insertKey(Key* key)
{
    // Important: DataManager must be parent of all root DTOs
    key->setParent(this);
    mAllKey.append(key);
    emit addedToAllKey(key);
    emit keyPropertyListChanged();
}

void DataManager::insertKeyFromMap(const QVariantMap& keyMap,
        const bool& useForeignProperties)
{
    Key* key = new Key();
    key->setParent(this);
    if (useForeignProperties) {
        key->fillFromForeignMap(keyMap);
    } else {
        key->fillFromMap(keyMap);
    }
    mAllKey.append(key);
    emit addedToAllKey(key);
    keyPropertyListChanged();
}

bool DataManager::deleteKey(Key* key)
{
    bool ok = false;
    ok = mAllKey.removeOne(key);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllKeyByKeyId(key->keyId());
    emit deletedFromAllKey(key);
    emit keyPropertyListChanged();
    key->deleteLater();
    key = 0;
    return ok;
}


bool DataManager::deleteKeyByKeyId(const int& keyId)
{
    for (int i = 0; i < mAllKey.size(); ++i) {
        Key* key;
        key = (Key*) mAllKey.at(i);
        if (key->keyId() == keyId) {
            mAllKey.removeAt(i);
            emit deletedFromAllKeyByKeyId(keyId);
            emit deletedFromAllKey(key);
            emit keyPropertyListChanged();
            key->deleteLater();
            key = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Key* DataManager::findKeyByKeyId(const int& keyId){
    for (int i = 0; i < mAllKey.size(); ++i) {
        Key* key;
        key = (Key*)mAllKey.at(i);
        if(key->keyId() == keyId){
            return key;
        }
    }
    qDebug() << "no Key found for keyId " << keyId;
    return 0;
}

/*
 * reads Maps of DictionaryItem in from JSON cache
 * creates List of DictionaryItem*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initDictionaryItemFromCache()
{
	qDebug() << "start initDictionaryItemFromCache";
    mAllDictionaryItem.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheDictionaryItem);
    qDebug() << "read DictionaryItem from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        DictionaryItem* dictionaryItem = new DictionaryItem();
        // Important: DataManager must be parent of all root DTOs
        dictionaryItem->setParent(this);
        dictionaryItem->fillFromCacheMap(cacheMap);
        mAllDictionaryItem.append(dictionaryItem);
    }
    qDebug() << "created DictionaryItem* #" << mAllDictionaryItem.size();
}


/*
 * save List of DictionaryItem* to JSON cache
 * convert list of DictionaryItem* to QVariantList
 * toCacheMap stores all properties without transient values
 * DictionaryItem is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveDictionaryItemToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching DictionaryItem* #" << mAllDictionaryItem.size();
    for (int i = 0; i < mAllDictionaryItem.size(); ++i) {
        DictionaryItem* dictionaryItem;
        dictionaryItem = (DictionaryItem*)mAllDictionaryItem.at(i);
        QVariantMap cacheMap;
        cacheMap = dictionaryItem->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "DictionaryItem* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheDictionaryItem, cacheList);
}


void DataManager::resolveDictionaryItemReferences(DictionaryItem* dictionaryItem)
{
	if (!dictionaryItem) {
        qDebug() << "cannot resolveDictionaryItemReferences with dictionaryItem NULL";
        return;
    }
    if(dictionaryItem->isAllResolved()) {
	    qDebug() << "nothing to do: all is resolved";
	    return;
	}
    if (dictionaryItem->hasKey() && !dictionaryItem->isKeyResolvedAsDataObject()) {
        Key* key;
        key = findKeyByKeyId(dictionaryItem->key());
    	if (key) {
            dictionaryItem->resolveKeyAsDataObject(key);
    	} else {
            qDebug() << "markKeyAsInvalid: " << dictionaryItem->key();
            dictionaryItem->markKeyAsInvalid();
    	}
    }
}

void DataManager::resolveReferencesForAllDictionaryItem()
{
    for (int i = 0; i < mAllDictionaryItem.size(); ++i) {
        DictionaryItem* dictionaryItem;
        dictionaryItem = (DictionaryItem*)mAllDictionaryItem.at(i);
    	resolveDictionaryItemReferences(dictionaryItem);
    }
}


/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<DictionaryItem*> DataManager::listOfDictionaryItemForKeys(
        QStringList keyList)
{
    QList<DictionaryItem*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllDictionaryItem.size(); ++i) {
        DictionaryItem* dictionaryItem;
        dictionaryItem = (DictionaryItem*) mAllDictionaryItem.at(i);
        if (keyList.contains(QString::number(dictionaryItem->keyId()))) {
            listOfData.append(dictionaryItem);
            keyList.removeOne(QString::number(dictionaryItem->keyId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for DictionaryItem: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::dictionaryItemAsQVariantList()
{
    QVariantList dictionaryItemList;
    for (int i = 0; i < mAllDictionaryItem.size(); ++i) {
        dictionaryItemList.append(((DictionaryItem*) (mAllDictionaryItem.at(i)))->toMap());
    }
    return dictionaryItemList;
}

QList<QObject*> DataManager::allDictionaryItem()
{
    return mAllDictionaryItem;
}

QQmlListProperty<DictionaryItem> DataManager::dictionaryItemPropertyList()
{
    return QQmlListProperty<DictionaryItem>(this, 0,
            &DataManager::appendToDictionaryItemProperty, &DataManager::dictionaryItemPropertyCount,
            &DataManager::atDictionaryItemProperty, &DataManager::clearDictionaryItemProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of DictionaryItem*
void DataManager::appendToDictionaryItemProperty(
        QQmlListProperty<DictionaryItem> *dictionaryItemList,
        DictionaryItem* dictionaryItem)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(dictionaryItemList->object);
    if (dataManagerObject) {
        dictionaryItem->setParent(dataManagerObject);
        dataManagerObject->mAllDictionaryItem.append(dictionaryItem);
        emit dataManagerObject->addedToAllDictionaryItem(dictionaryItem);
    } else {
        qWarning() << "cannot append DictionaryItem* to mAllDictionaryItem "
                << "Object is not of type DataManager*";
    }
}
int DataManager::dictionaryItemPropertyCount(
        QQmlListProperty<DictionaryItem> *dictionaryItemList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(dictionaryItemList->object);
    if (dataManager) {
        return dataManager->mAllDictionaryItem.size();
    } else {
        qWarning() << "cannot get size mAllDictionaryItem " << "Object is not of type DataManager*";
    }
    return 0;
}
DictionaryItem* DataManager::atDictionaryItemProperty(
        QQmlListProperty<DictionaryItem> *dictionaryItemList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(dictionaryItemList->object);
    if (dataManager) {
        if (dataManager->mAllDictionaryItem.size() > pos) {
            return (DictionaryItem*) dataManager->mAllDictionaryItem.at(pos);
        }
        qWarning() << "cannot get DictionaryItem* at pos " << pos << " size is "
                << dataManager->mAllDictionaryItem.size();
    } else {
        qWarning() << "cannot get DictionaryItem* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearDictionaryItemProperty(
        QQmlListProperty<DictionaryItem> *dictionaryItemList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(dictionaryItemList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllDictionaryItem.size(); ++i) {
            DictionaryItem* dictionaryItem;
            dictionaryItem = (DictionaryItem*) dataManager->mAllDictionaryItem.at(i);
            emit dataManager->deletedFromAllDictionaryItemByKeyId(dictionaryItem->keyId());
			emit dataManager->deletedFromAllDictionaryItem(dictionaryItem);
            dictionaryItem->deleteLater();
            dictionaryItem = 0;
        }
        dataManager->mAllDictionaryItem.clear();
    } else {
        qWarning() << "cannot clear mAllDictionaryItem " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all DictionaryItem
 * and clears the list
 */
void DataManager::deleteDictionaryItem()
{
    for (int i = 0; i < mAllDictionaryItem.size(); ++i) {
        DictionaryItem* dictionaryItem;
        dictionaryItem = (DictionaryItem*) mAllDictionaryItem.at(i);
        emit deletedFromAllDictionaryItemByKeyId(dictionaryItem->keyId());
		emit deletedFromAllDictionaryItem(dictionaryItem);
		emit dictionaryItemPropertyListChanged();
        dictionaryItem->deleteLater();
        dictionaryItem = 0;
     }
     mAllDictionaryItem.clear();
}

/**
 * creates a new DictionaryItem
 * parent is DataManager
 * if data is successfully entered you must insertDictionaryItem
 * if edit was canceled you must undoCreateDictionaryItem to free up memory
 */
DictionaryItem* DataManager::createDictionaryItem()
{
    DictionaryItem* dictionaryItem;
    dictionaryItem = new DictionaryItem();
    dictionaryItem->setParent(this);
    dictionaryItem->prepareNew();
    return dictionaryItem;
}

/**
 * deletes DictionaryItem
 * if createDictionaryItem was canceled from UI
 * to delete a previous successfully inserted DictionaryItem
 * use deleteDictionaryItem
 */
void DataManager::undoCreateDictionaryItem(DictionaryItem* dictionaryItem)
{
    if (dictionaryItem) {
        qDebug() << "undoCreateDictionaryItem " << dictionaryItem->keyId();
        dictionaryItem->deleteLater();
        dictionaryItem = 0;
    }
}

void DataManager::insertDictionaryItem(DictionaryItem* dictionaryItem)
{
    // Important: DataManager must be parent of all root DTOs
    dictionaryItem->setParent(this);
    mAllDictionaryItem.append(dictionaryItem);
    emit addedToAllDictionaryItem(dictionaryItem);
    emit dictionaryItemPropertyListChanged();
}

void DataManager::insertDictionaryItemFromMap(const QVariantMap& dictionaryItemMap,
        const bool& useForeignProperties)
{
    DictionaryItem* dictionaryItem = new DictionaryItem();
    dictionaryItem->setParent(this);
    if (useForeignProperties) {
        dictionaryItem->fillFromForeignMap(dictionaryItemMap);
    } else {
        dictionaryItem->fillFromMap(dictionaryItemMap);
    }
    mAllDictionaryItem.append(dictionaryItem);
    emit addedToAllDictionaryItem(dictionaryItem);
    dictionaryItemPropertyListChanged();
}

bool DataManager::deleteDictionaryItem(DictionaryItem* dictionaryItem)
{
    bool ok = false;
    ok = mAllDictionaryItem.removeOne(dictionaryItem);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllDictionaryItemByKeyId(dictionaryItem->keyId());
    emit deletedFromAllDictionaryItem(dictionaryItem);
    emit dictionaryItemPropertyListChanged();
    dictionaryItem->deleteLater();
    dictionaryItem = 0;
    return ok;
}


bool DataManager::deleteDictionaryItemByKeyId(const int& keyId)
{
    for (int i = 0; i < mAllDictionaryItem.size(); ++i) {
        DictionaryItem* dictionaryItem;
        dictionaryItem = (DictionaryItem*) mAllDictionaryItem.at(i);
        if (dictionaryItem->keyId() == keyId) {
            mAllDictionaryItem.removeAt(i);
            emit deletedFromAllDictionaryItemByKeyId(keyId);
            emit deletedFromAllDictionaryItem(dictionaryItem);
            emit dictionaryItemPropertyListChanged();
            dictionaryItem->deleteLater();
            dictionaryItem = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
DictionaryItem* DataManager::findDictionaryItemByKeyId(const int& keyId){
    for (int i = 0; i < mAllDictionaryItem.size(); ++i) {
        DictionaryItem* dictionaryItem;
        dictionaryItem = (DictionaryItem*)mAllDictionaryItem.at(i);
        if(dictionaryItem->keyId() == keyId){
            return dictionaryItem;
        }
    }
    qDebug() << "no DictionaryItem found for keyId " << keyId;
    return 0;
}

/*
 * reads Maps of Favorite in from JSON cache
 * creates List of Favorite*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initFavoriteFromCache()
{
	qDebug() << "start initFavoriteFromCache";
    mAllFavorite.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheFavorite);
    qDebug() << "read Favorite from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Favorite* favorite = new Favorite();
        // Important: DataManager must be parent of all root DTOs
        favorite->setParent(this);
        favorite->fillFromCacheMap(cacheMap);
        mAllFavorite.append(favorite);
    }
    qDebug() << "created Favorite* #" << mAllFavorite.size();
}


/*
 * save List of Favorite* to JSON cache
 * convert list of Favorite* to QVariantList
 * toCacheMap stores all properties without transient values
 */
void DataManager::saveFavoriteToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Favorite* #" << mAllFavorite.size();
    for (int i = 0; i < mAllFavorite.size(); ++i) {
        Favorite* favorite;
        favorite = (Favorite*)mAllFavorite.at(i);
        QVariantMap cacheMap;
        cacheMap = favorite->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Favorite* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheFavorite, cacheList);
}


void DataManager::resolveFavoriteReferences(Favorite* favorite)
{
	if (!favorite) {
        qDebug() << "cannot resolveFavoriteReferences with favorite NULL";
        return;
    }
    if(favorite->isAllResolved()) {
	    qDebug() << "nothing to do: all is resolved";
	    return;
	}
    if (favorite->hasKey() && !favorite->isKeyResolvedAsDataObject()) {
        Key* key;
        key = findKeyByKeyId(favorite->key());
    	if (key) {
            favorite->resolveKeyAsDataObject(key);
    	} else {
            qDebug() << "markKeyAsInvalid: " << favorite->key();
            favorite->markKeyAsInvalid();
    	}
    }
}

void DataManager::resolveReferencesForAllFavorite()
{
    for (int i = 0; i < mAllFavorite.size(); ++i) {
        Favorite* favorite;
        favorite = (Favorite*)mAllFavorite.at(i);
    	resolveFavoriteReferences(favorite);
    }
}


/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Favorite*> DataManager::listOfFavoriteForKeys(
        QStringList keyList)
{
    QList<Favorite*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllFavorite.size(); ++i) {
        Favorite* favorite;
        favorite = (Favorite*) mAllFavorite.at(i);
        if (keyList.contains(QString::number(favorite->keyId()))) {
            listOfData.append(favorite);
            keyList.removeOne(QString::number(favorite->keyId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Favorite: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::favoriteAsQVariantList()
{
    QVariantList favoriteList;
    for (int i = 0; i < mAllFavorite.size(); ++i) {
        favoriteList.append(((Favorite*) (mAllFavorite.at(i)))->toMap());
    }
    return favoriteList;
}

QList<QObject*> DataManager::allFavorite()
{
    return mAllFavorite;
}

QQmlListProperty<Favorite> DataManager::favoritePropertyList()
{
    return QQmlListProperty<Favorite>(this, 0,
            &DataManager::appendToFavoriteProperty, &DataManager::favoritePropertyCount,
            &DataManager::atFavoriteProperty, &DataManager::clearFavoriteProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Favorite*
void DataManager::appendToFavoriteProperty(
        QQmlListProperty<Favorite> *favoriteList,
        Favorite* favorite)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(favoriteList->object);
    if (dataManagerObject) {
        favorite->setParent(dataManagerObject);
        dataManagerObject->mAllFavorite.append(favorite);
        emit dataManagerObject->addedToAllFavorite(favorite);
    } else {
        qWarning() << "cannot append Favorite* to mAllFavorite "
                << "Object is not of type DataManager*";
    }
}
int DataManager::favoritePropertyCount(
        QQmlListProperty<Favorite> *favoriteList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(favoriteList->object);
    if (dataManager) {
        return dataManager->mAllFavorite.size();
    } else {
        qWarning() << "cannot get size mAllFavorite " << "Object is not of type DataManager*";
    }
    return 0;
}
Favorite* DataManager::atFavoriteProperty(
        QQmlListProperty<Favorite> *favoriteList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(favoriteList->object);
    if (dataManager) {
        if (dataManager->mAllFavorite.size() > pos) {
            return (Favorite*) dataManager->mAllFavorite.at(pos);
        }
        qWarning() << "cannot get Favorite* at pos " << pos << " size is "
                << dataManager->mAllFavorite.size();
    } else {
        qWarning() << "cannot get Favorite* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearFavoriteProperty(
        QQmlListProperty<Favorite> *favoriteList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(favoriteList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllFavorite.size(); ++i) {
            Favorite* favorite;
            favorite = (Favorite*) dataManager->mAllFavorite.at(i);
            emit dataManager->deletedFromAllFavoriteByKeyId(favorite->keyId());
			emit dataManager->deletedFromAllFavorite(favorite);
            favorite->deleteLater();
            favorite = 0;
        }
        dataManager->mAllFavorite.clear();
    } else {
        qWarning() << "cannot clear mAllFavorite " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Favorite
 * and clears the list
 */
void DataManager::deleteFavorite()
{
    for (int i = 0; i < mAllFavorite.size(); ++i) {
        Favorite* favorite;
        favorite = (Favorite*) mAllFavorite.at(i);
        emit deletedFromAllFavoriteByKeyId(favorite->keyId());
		emit deletedFromAllFavorite(favorite);
		emit favoritePropertyListChanged();
        favorite->deleteLater();
        favorite = 0;
     }
     mAllFavorite.clear();
}

/**
 * creates a new Favorite
 * parent is DataManager
 * if data is successfully entered you must insertFavorite
 * if edit was canceled you must undoCreateFavorite to free up memory
 */
Favorite* DataManager::createFavorite()
{
    Favorite* favorite;
    favorite = new Favorite();
    favorite->setParent(this);
    favorite->prepareNew();
    return favorite;
}

/**
 * deletes Favorite
 * if createFavorite was canceled from UI
 * to delete a previous successfully inserted Favorite
 * use deleteFavorite
 */
void DataManager::undoCreateFavorite(Favorite* favorite)
{
    if (favorite) {
        qDebug() << "undoCreateFavorite " << favorite->keyId();
        favorite->deleteLater();
        favorite = 0;
    }
}

void DataManager::insertFavorite(Favorite* favorite)
{
    // Important: DataManager must be parent of all root DTOs
    favorite->setParent(this);
    mAllFavorite.append(favorite);
    emit addedToAllFavorite(favorite);
    emit favoritePropertyListChanged();
}

void DataManager::insertFavoriteFromMap(const QVariantMap& favoriteMap,
        const bool& useForeignProperties)
{
    Favorite* favorite = new Favorite();
    favorite->setParent(this);
    if (useForeignProperties) {
        favorite->fillFromForeignMap(favoriteMap);
    } else {
        favorite->fillFromMap(favoriteMap);
    }
    mAllFavorite.append(favorite);
    emit addedToAllFavorite(favorite);
    favoritePropertyListChanged();
}

bool DataManager::deleteFavorite(Favorite* favorite)
{
    bool ok = false;
    ok = mAllFavorite.removeOne(favorite);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllFavoriteByKeyId(favorite->keyId());
    emit deletedFromAllFavorite(favorite);
    emit favoritePropertyListChanged();
    favorite->deleteLater();
    favorite = 0;
    return ok;
}


bool DataManager::deleteFavoriteByKeyId(const int& keyId)
{
    for (int i = 0; i < mAllFavorite.size(); ++i) {
        Favorite* favorite;
        favorite = (Favorite*) mAllFavorite.at(i);
        if (favorite->keyId() == keyId) {
            mAllFavorite.removeAt(i);
            emit deletedFromAllFavoriteByKeyId(keyId);
            emit deletedFromAllFavorite(favorite);
            emit favoritePropertyListChanged();
            favorite->deleteLater();
            favorite = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Favorite* DataManager::findFavoriteByKeyId(const int& keyId){
    for (int i = 0; i < mAllFavorite.size(); ++i) {
        Favorite* favorite;
        favorite = (Favorite*)mAllFavorite.at(i);
        if(favorite->keyId() == keyId){
            return favorite;
        }
    }
    qDebug() << "no Favorite found for keyId " << keyId;
    return 0;
}

/*
 * reads Maps of Bookmark in from JSON cache
 * creates List of Bookmark*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initBookmarkFromCache()
{
	qDebug() << "start initBookmarkFromCache";
    mAllBookmark.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheBookmark);
    qDebug() << "read Bookmark from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Bookmark* bookmark = new Bookmark();
        // Important: DataManager must be parent of all root DTOs
        bookmark->setParent(this);
        bookmark->fillFromCacheMap(cacheMap);
        mAllBookmark.append(bookmark);
    }
    qDebug() << "created Bookmark* #" << mAllBookmark.size();
}


/*
 * save List of Bookmark* to JSON cache
 * convert list of Bookmark* to QVariantList
 * toCacheMap stores all properties without transient values
 */
void DataManager::saveBookmarkToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Bookmark* #" << mAllBookmark.size();
    for (int i = 0; i < mAllBookmark.size(); ++i) {
        Bookmark* bookmark;
        bookmark = (Bookmark*)mAllBookmark.at(i);
        QVariantMap cacheMap;
        cacheMap = bookmark->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Bookmark* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheBookmark, cacheList);
}


void DataManager::resolveBookmarkReferences(Bookmark* bookmark)
{
	if (!bookmark) {
        qDebug() << "cannot resolveBookmarkReferences with bookmark NULL";
        return;
    }
    if(bookmark->isAllResolved()) {
	    qDebug() << "nothing to do: all is resolved";
	    return;
	}
    if (bookmark->hasKey() && !bookmark->isKeyResolvedAsDataObject()) {
        Key* key;
        key = findKeyByKeyId(bookmark->key());
    	if (key) {
            bookmark->resolveKeyAsDataObject(key);
    	} else {
            qDebug() << "markKeyAsInvalid: " << bookmark->key();
            bookmark->markKeyAsInvalid();
    	}
    }
}

void DataManager::resolveReferencesForAllBookmark()
{
    for (int i = 0; i < mAllBookmark.size(); ++i) {
        Bookmark* bookmark;
        bookmark = (Bookmark*)mAllBookmark.at(i);
    	resolveBookmarkReferences(bookmark);
    }
}


/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Bookmark*> DataManager::listOfBookmarkForKeys(
        QStringList keyList)
{
    QList<Bookmark*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllBookmark.size(); ++i) {
        Bookmark* bookmark;
        bookmark = (Bookmark*) mAllBookmark.at(i);
        if (keyList.contains(QString::number(bookmark->keyId()))) {
            listOfData.append(bookmark);
            keyList.removeOne(QString::number(bookmark->keyId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Bookmark: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::bookmarkAsQVariantList()
{
    QVariantList bookmarkList;
    for (int i = 0; i < mAllBookmark.size(); ++i) {
        bookmarkList.append(((Bookmark*) (mAllBookmark.at(i)))->toMap());
    }
    return bookmarkList;
}

QList<QObject*> DataManager::allBookmark()
{
    return mAllBookmark;
}

QQmlListProperty<Bookmark> DataManager::bookmarkPropertyList()
{
    return QQmlListProperty<Bookmark>(this, 0,
            &DataManager::appendToBookmarkProperty, &DataManager::bookmarkPropertyCount,
            &DataManager::atBookmarkProperty, &DataManager::clearBookmarkProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Bookmark*
void DataManager::appendToBookmarkProperty(
        QQmlListProperty<Bookmark> *bookmarkList,
        Bookmark* bookmark)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(bookmarkList->object);
    if (dataManagerObject) {
        bookmark->setParent(dataManagerObject);
        dataManagerObject->mAllBookmark.append(bookmark);
        emit dataManagerObject->addedToAllBookmark(bookmark);
    } else {
        qWarning() << "cannot append Bookmark* to mAllBookmark "
                << "Object is not of type DataManager*";
    }
}
int DataManager::bookmarkPropertyCount(
        QQmlListProperty<Bookmark> *bookmarkList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(bookmarkList->object);
    if (dataManager) {
        return dataManager->mAllBookmark.size();
    } else {
        qWarning() << "cannot get size mAllBookmark " << "Object is not of type DataManager*";
    }
    return 0;
}
Bookmark* DataManager::atBookmarkProperty(
        QQmlListProperty<Bookmark> *bookmarkList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(bookmarkList->object);
    if (dataManager) {
        if (dataManager->mAllBookmark.size() > pos) {
            return (Bookmark*) dataManager->mAllBookmark.at(pos);
        }
        qWarning() << "cannot get Bookmark* at pos " << pos << " size is "
                << dataManager->mAllBookmark.size();
    } else {
        qWarning() << "cannot get Bookmark* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearBookmarkProperty(
        QQmlListProperty<Bookmark> *bookmarkList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(bookmarkList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllBookmark.size(); ++i) {
            Bookmark* bookmark;
            bookmark = (Bookmark*) dataManager->mAllBookmark.at(i);
            emit dataManager->deletedFromAllBookmarkByKeyId(bookmark->keyId());
			emit dataManager->deletedFromAllBookmark(bookmark);
            bookmark->deleteLater();
            bookmark = 0;
        }
        dataManager->mAllBookmark.clear();
    } else {
        qWarning() << "cannot clear mAllBookmark " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Bookmark
 * and clears the list
 */
void DataManager::deleteBookmark()
{
    for (int i = 0; i < mAllBookmark.size(); ++i) {
        Bookmark* bookmark;
        bookmark = (Bookmark*) mAllBookmark.at(i);
        emit deletedFromAllBookmarkByKeyId(bookmark->keyId());
		emit deletedFromAllBookmark(bookmark);
		emit bookmarkPropertyListChanged();
        bookmark->deleteLater();
        bookmark = 0;
     }
     mAllBookmark.clear();
}

/**
 * creates a new Bookmark
 * parent is DataManager
 * if data is successfully entered you must insertBookmark
 * if edit was canceled you must undoCreateBookmark to free up memory
 */
Bookmark* DataManager::createBookmark()
{
    Bookmark* bookmark;
    bookmark = new Bookmark();
    bookmark->setParent(this);
    bookmark->prepareNew();
    return bookmark;
}

/**
 * deletes Bookmark
 * if createBookmark was canceled from UI
 * to delete a previous successfully inserted Bookmark
 * use deleteBookmark
 */
void DataManager::undoCreateBookmark(Bookmark* bookmark)
{
    if (bookmark) {
        qDebug() << "undoCreateBookmark " << bookmark->keyId();
        bookmark->deleteLater();
        bookmark = 0;
    }
}

void DataManager::insertBookmark(Bookmark* bookmark)
{
    // Important: DataManager must be parent of all root DTOs
    bookmark->setParent(this);
    mAllBookmark.append(bookmark);
    emit addedToAllBookmark(bookmark);
    emit bookmarkPropertyListChanged();
}

void DataManager::insertBookmarkFromMap(const QVariantMap& bookmarkMap,
        const bool& useForeignProperties)
{
    Bookmark* bookmark = new Bookmark();
    bookmark->setParent(this);
    if (useForeignProperties) {
        bookmark->fillFromForeignMap(bookmarkMap);
    } else {
        bookmark->fillFromMap(bookmarkMap);
    }
    mAllBookmark.append(bookmark);
    emit addedToAllBookmark(bookmark);
    bookmarkPropertyListChanged();
}

bool DataManager::deleteBookmark(Bookmark* bookmark)
{
    bool ok = false;
    ok = mAllBookmark.removeOne(bookmark);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllBookmarkByKeyId(bookmark->keyId());
    emit deletedFromAllBookmark(bookmark);
    emit bookmarkPropertyListChanged();
    bookmark->deleteLater();
    bookmark = 0;
    return ok;
}


bool DataManager::deleteBookmarkByKeyId(const int& keyId)
{
    for (int i = 0; i < mAllBookmark.size(); ++i) {
        Bookmark* bookmark;
        bookmark = (Bookmark*) mAllBookmark.at(i);
        if (bookmark->keyId() == keyId) {
            mAllBookmark.removeAt(i);
            emit deletedFromAllBookmarkByKeyId(keyId);
            emit deletedFromAllBookmark(bookmark);
            emit bookmarkPropertyListChanged();
            bookmark->deleteLater();
            bookmark = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Bookmark* DataManager::findBookmarkByKeyId(const int& keyId){
    for (int i = 0; i < mAllBookmark.size(); ++i) {
        Bookmark* bookmark;
        bookmark = (Bookmark*)mAllBookmark.at(i);
        if(bookmark->keyId() == keyId){
            return bookmark;
        }
    }
    qDebug() << "no Bookmark found for keyId " << keyId;
    return 0;
}

/*
 * reads Maps of KeyLists in from JSON cache
 * creates List of KeyLists*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initKeyListsFromCache()
{
    qDebug() << "start initKeyListsFromCache";
    mAllKeyLists.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheKeyLists);
    qDebug() << "read KeyLists from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        KeyLists* keyLists = new KeyLists();
        // Important: DataManager must be parent of all root DTOs
        keyLists->setParent(this);
        keyLists->fillFromCacheMap(cacheMap);
        mAllKeyLists.append(keyLists);
    }
    qDebug() << "created KeyLists* #" << mAllKeyLists.size();
}


/*
 * save List of KeyLists* to JSON cache
 * convert list of KeyLists* to QVariantList
 * toCacheMap stores all properties without transient values
 * KeyLists is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveKeyListsToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching KeyLists* #" << mAllKeyLists.size();
    for (int i = 0; i < mAllKeyLists.size(); ++i) {
        KeyLists* keyLists;
        keyLists = (KeyLists*)mAllKeyLists.at(i);
        QVariantMap cacheMap;
        cacheMap = keyLists->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "KeyLists* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheKeyLists, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<KeyLists*> DataManager::listOfKeyListsForKeys(
        QStringList keyList)
{
    QList<KeyLists*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllKeyLists.size(); ++i) {
        KeyLists* keyLists;
        keyLists = (KeyLists*) mAllKeyLists.at(i);
        if (keyList.contains(keyLists->uuid())) {
            listOfData.append(keyLists);
            keyList.removeOne(keyLists->uuid());
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for KeyLists: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::keyListsAsQVariantList()
{
    QVariantList keyListsList;
    for (int i = 0; i < mAllKeyLists.size(); ++i) {
        keyListsList.append(((KeyLists*) (mAllKeyLists.at(i)))->toMap());
    }
    return keyListsList;
}

QList<QObject*> DataManager::allKeyLists()
{
    return mAllKeyLists;
}

QQmlListProperty<KeyLists> DataManager::keyListsPropertyList()
{
    return QQmlListProperty<KeyLists>(this, 0,
            &DataManager::appendToKeyListsProperty, &DataManager::keyListsPropertyCount,
            &DataManager::atKeyListsProperty, &DataManager::clearKeyListsProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of KeyLists*
void DataManager::appendToKeyListsProperty(
        QQmlListProperty<KeyLists> *keyListsList,
        KeyLists* keyLists)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(keyListsList->object);
    if (dataManagerObject) {
        keyLists->setParent(dataManagerObject);
        dataManagerObject->mAllKeyLists.append(keyLists);
        emit dataManagerObject->addedToAllKeyLists(keyLists);
    } else {
        qWarning() << "cannot append KeyLists* to mAllKeyLists "
                << "Object is not of type DataManager*";
    }
}
int DataManager::keyListsPropertyCount(
        QQmlListProperty<KeyLists> *keyListsList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyListsList->object);
    if (dataManager) {
        return dataManager->mAllKeyLists.size();
    } else {
        qWarning() << "cannot get size mAllKeyLists " << "Object is not of type DataManager*";
    }
    return 0;
}
KeyLists* DataManager::atKeyListsProperty(
        QQmlListProperty<KeyLists> *keyListsList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyListsList->object);
    if (dataManager) {
        if (dataManager->mAllKeyLists.size() > pos) {
            return (KeyLists*) dataManager->mAllKeyLists.at(pos);
        }
        qWarning() << "cannot get KeyLists* at pos " << pos << " size is "
                << dataManager->mAllKeyLists.size();
    } else {
        qWarning() << "cannot get KeyLists* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearKeyListsProperty(
        QQmlListProperty<KeyLists> *keyListsList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyListsList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllKeyLists.size(); ++i) {
            KeyLists* keyLists;
            keyLists = (KeyLists*) dataManager->mAllKeyLists.at(i);
            emit dataManager->deletedFromAllKeyListsByUuid(keyLists->uuid());
            emit dataManager->deletedFromAllKeyLists(keyLists);
            keyLists->deleteLater();
            keyLists = 0;
        }
        dataManager->mAllKeyLists.clear();
    } else {
        qWarning() << "cannot clear mAllKeyLists " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all KeyLists
 * and clears the list
 */
void DataManager::deleteKeyLists()
{
    for (int i = 0; i < mAllKeyLists.size(); ++i) {
        KeyLists* keyLists;
        keyLists = (KeyLists*) mAllKeyLists.at(i);
        emit deletedFromAllKeyListsByUuid(keyLists->uuid());
        emit deletedFromAllKeyLists(keyLists);
		emit keyListsPropertyListChanged();
        keyLists->deleteLater();
        keyLists = 0;
     }
     mAllKeyLists.clear();
}

/**
 * creates a new KeyLists
 * parent is DataManager
 * if data is successfully entered you must insertKeyLists
 * if edit was canceled you must undoCreateKeyLists to free up memory
 */
KeyLists* DataManager::createKeyLists()
{
    KeyLists* keyLists;
    keyLists = new KeyLists();
    keyLists->setParent(this);
    keyLists->prepareNew();
    return keyLists;
}

/**
 * deletes KeyLists
 * if createKeyLists was canceled from UI
 * to delete a previous successfully inserted KeyLists
 * use deleteKeyLists
 */
void DataManager::undoCreateKeyLists(KeyLists* keyLists)
{
    if (keyLists) {
        qDebug() << "undoCreateKeyLists " << keyLists->uuid();
        keyLists->deleteLater();
        keyLists = 0;
    }
}

void DataManager::insertKeyLists(KeyLists* keyLists)
{
    // Important: DataManager must be parent of all root DTOs
    keyLists->setParent(this);
    mAllKeyLists.append(keyLists);
    emit addedToAllKeyLists(keyLists);
    emit keyListsPropertyListChanged();
}

void DataManager::insertKeyListsFromMap(const QVariantMap& keyListsMap,
        const bool& useForeignProperties)
{
    KeyLists* keyLists = new KeyLists();
    keyLists->setParent(this);
    if (useForeignProperties) {
        keyLists->fillFromForeignMap(keyListsMap);
    } else {
        keyLists->fillFromMap(keyListsMap);
    }
    mAllKeyLists.append(keyLists);
    emit addedToAllKeyLists(keyLists);
    keyListsPropertyListChanged();
}

bool DataManager::deleteKeyLists(KeyLists* keyLists)
{
    bool ok = false;
    ok = mAllKeyLists.removeOne(keyLists);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllKeyListsByUuid(keyLists->uuid());
    emit deletedFromAllKeyLists(keyLists);
    emit keyListsPropertyListChanged();
    keyLists->deleteLater();
    keyLists = 0;
    return ok;
}

bool DataManager::deleteKeyListsByUuid(const QString& uuid)
{
    if (uuid.isNull() || uuid.isEmpty()) {
        qDebug() << "cannot delete KeyLists from empty uuid";
        return false;
    }
    for (int i = 0; i < mAllKeyLists.size(); ++i) {
        KeyLists* keyLists;
        keyLists = (KeyLists*) mAllKeyLists.at(i);
        if (keyLists->uuid() == uuid) {
            mAllKeyLists.removeAt(i);
            emit deletedFromAllKeyListsByUuid(uuid);
            emit deletedFromAllKeyLists(keyLists);
            emit keyListsPropertyListChanged();
            keyLists->deleteLater();
            keyLists = 0;
            return true;
        }
    }
    return false;
}


KeyLists* DataManager::findKeyListsByUuid(const QString& uuid){
    if (uuid.isNull() || uuid.isEmpty()) {
        qDebug() << "cannot find KeyLists from empty uuid";
        return 0;
    }
    for (int i = 0; i < mAllKeyLists.size(); ++i) {
        KeyLists* keyLists;
        keyLists = (KeyLists*)mAllKeyLists.at(i);
        if(keyLists->uuid() == uuid){
            return keyLists;
        }
    }
    qDebug() << "no KeyLists found for uuid " << uuid;
    return 0;
}


/*
 * reads Maps of KeyLink in from JSON cache
 * creates List of KeyLink*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initKeyLinkFromCache()
{
    qDebug() << "start initKeyLinkFromCache";
    mAllKeyLink.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheKeyLink);
    qDebug() << "read KeyLink from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        KeyLink* keyLink = new KeyLink();
        // Important: DataManager must be parent of all root DTOs
        keyLink->setParent(this);
        keyLink->fillFromCacheMap(cacheMap);
        mAllKeyLink.append(keyLink);
    }
    qDebug() << "created KeyLink* #" << mAllKeyLink.size();
}


/*
 * save List of KeyLink* to JSON cache
 * convert list of KeyLink* to QVariantList
 * toCacheMap stores all properties without transient values
 * KeyLink is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveKeyLinkToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching KeyLink* #" << mAllKeyLink.size();
    for (int i = 0; i < mAllKeyLink.size(); ++i) {
        KeyLink* keyLink;
        keyLink = (KeyLink*)mAllKeyLink.at(i);
        QVariantMap cacheMap;
        cacheMap = keyLink->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "KeyLink* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheKeyLink, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<KeyLink*> DataManager::listOfKeyLinkForKeys(
        QStringList keyList)
{
    QList<KeyLink*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllKeyLink.size(); ++i) {
        KeyLink* keyLink;
        keyLink = (KeyLink*) mAllKeyLink.at(i);
        if (keyList.contains(keyLink->uuid())) {
            listOfData.append(keyLink);
            keyList.removeOne(keyLink->uuid());
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for KeyLink: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::keyLinkAsQVariantList()
{
    QVariantList keyLinkList;
    for (int i = 0; i < mAllKeyLink.size(); ++i) {
        keyLinkList.append(((KeyLink*) (mAllKeyLink.at(i)))->toMap());
    }
    return keyLinkList;
}

QList<QObject*> DataManager::allKeyLink()
{
    return mAllKeyLink;
}

QQmlListProperty<KeyLink> DataManager::keyLinkPropertyList()
{
    return QQmlListProperty<KeyLink>(this, 0,
            &DataManager::appendToKeyLinkProperty, &DataManager::keyLinkPropertyCount,
            &DataManager::atKeyLinkProperty, &DataManager::clearKeyLinkProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of KeyLink*
void DataManager::appendToKeyLinkProperty(
        QQmlListProperty<KeyLink> *keyLinkList,
        KeyLink* keyLink)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(keyLinkList->object);
    if (dataManagerObject) {
        keyLink->setParent(dataManagerObject);
        dataManagerObject->mAllKeyLink.append(keyLink);
        emit dataManagerObject->addedToAllKeyLink(keyLink);
    } else {
        qWarning() << "cannot append KeyLink* to mAllKeyLink "
                << "Object is not of type DataManager*";
    }
}
int DataManager::keyLinkPropertyCount(
        QQmlListProperty<KeyLink> *keyLinkList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyLinkList->object);
    if (dataManager) {
        return dataManager->mAllKeyLink.size();
    } else {
        qWarning() << "cannot get size mAllKeyLink " << "Object is not of type DataManager*";
    }
    return 0;
}
KeyLink* DataManager::atKeyLinkProperty(
        QQmlListProperty<KeyLink> *keyLinkList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyLinkList->object);
    if (dataManager) {
        if (dataManager->mAllKeyLink.size() > pos) {
            return (KeyLink*) dataManager->mAllKeyLink.at(pos);
        }
        qWarning() << "cannot get KeyLink* at pos " << pos << " size is "
                << dataManager->mAllKeyLink.size();
    } else {
        qWarning() << "cannot get KeyLink* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearKeyLinkProperty(
        QQmlListProperty<KeyLink> *keyLinkList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyLinkList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllKeyLink.size(); ++i) {
            KeyLink* keyLink;
            keyLink = (KeyLink*) dataManager->mAllKeyLink.at(i);
            emit dataManager->deletedFromAllKeyLinkByUuid(keyLink->uuid());
            emit dataManager->deletedFromAllKeyLink(keyLink);
            keyLink->deleteLater();
            keyLink = 0;
        }
        dataManager->mAllKeyLink.clear();
    } else {
        qWarning() << "cannot clear mAllKeyLink " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all KeyLink
 * and clears the list
 */
void DataManager::deleteKeyLink()
{
    for (int i = 0; i < mAllKeyLink.size(); ++i) {
        KeyLink* keyLink;
        keyLink = (KeyLink*) mAllKeyLink.at(i);
        emit deletedFromAllKeyLinkByUuid(keyLink->uuid());
        emit deletedFromAllKeyLink(keyLink);
		emit keyLinkPropertyListChanged();
        keyLink->deleteLater();
        keyLink = 0;
     }
     mAllKeyLink.clear();
}

/**
 * creates a new KeyLink
 * parent is DataManager
 * if data is successfully entered you must insertKeyLink
 * if edit was canceled you must undoCreateKeyLink to free up memory
 */
KeyLink* DataManager::createKeyLink()
{
    KeyLink* keyLink;
    keyLink = new KeyLink();
    keyLink->setParent(this);
    keyLink->prepareNew();
    return keyLink;
}

/**
 * deletes KeyLink
 * if createKeyLink was canceled from UI
 * to delete a previous successfully inserted KeyLink
 * use deleteKeyLink
 */
void DataManager::undoCreateKeyLink(KeyLink* keyLink)
{
    if (keyLink) {
        qDebug() << "undoCreateKeyLink " << keyLink->uuid();
        keyLink->deleteLater();
        keyLink = 0;
    }
}

void DataManager::insertKeyLink(KeyLink* keyLink)
{
    // Important: DataManager must be parent of all root DTOs
    keyLink->setParent(this);
    mAllKeyLink.append(keyLink);
    emit addedToAllKeyLink(keyLink);
    emit keyLinkPropertyListChanged();
}

void DataManager::insertKeyLinkFromMap(const QVariantMap& keyLinkMap,
        const bool& useForeignProperties)
{
    KeyLink* keyLink = new KeyLink();
    keyLink->setParent(this);
    if (useForeignProperties) {
        keyLink->fillFromForeignMap(keyLinkMap);
    } else {
        keyLink->fillFromMap(keyLinkMap);
    }
    mAllKeyLink.append(keyLink);
    emit addedToAllKeyLink(keyLink);
    keyLinkPropertyListChanged();
}

bool DataManager::deleteKeyLink(KeyLink* keyLink)
{
    bool ok = false;
    ok = mAllKeyLink.removeOne(keyLink);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllKeyLinkByUuid(keyLink->uuid());
    emit deletedFromAllKeyLink(keyLink);
    emit keyLinkPropertyListChanged();
    keyLink->deleteLater();
    keyLink = 0;
    return ok;
}

bool DataManager::deleteKeyLinkByUuid(const QString& uuid)
{
    if (uuid.isNull() || uuid.isEmpty()) {
        qDebug() << "cannot delete KeyLink from empty uuid";
        return false;
    }
    for (int i = 0; i < mAllKeyLink.size(); ++i) {
        KeyLink* keyLink;
        keyLink = (KeyLink*) mAllKeyLink.at(i);
        if (keyLink->uuid() == uuid) {
            mAllKeyLink.removeAt(i);
            emit deletedFromAllKeyLinkByUuid(uuid);
            emit deletedFromAllKeyLink(keyLink);
            emit keyLinkPropertyListChanged();
            keyLink->deleteLater();
            keyLink = 0;
            return true;
        }
    }
    return false;
}


KeyLink* DataManager::findKeyLinkByUuid(const QString& uuid){
    if (uuid.isNull() || uuid.isEmpty()) {
        qDebug() << "cannot find KeyLink from empty uuid";
        return 0;
    }
    for (int i = 0; i < mAllKeyLink.size(); ++i) {
        KeyLink* keyLink;
        keyLink = (KeyLink*)mAllKeyLink.at(i);
        if(keyLink->uuid() == uuid){
            return keyLink;
        }
    }
    qDebug() << "no KeyLink found for uuid " << uuid;
    return 0;
}


/*
 * reads Maps of Creator in from JSON cache
 * creates List of Creator*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initCreatorFromCache()
{
	qDebug() << "start initCreatorFromCache";
    mAllCreator.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheCreator);
    qDebug() << "read Creator from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Creator* creator = new Creator();
        // Important: DataManager must be parent of all root DTOs
        creator->setParent(this);
        creator->fillFromCacheMap(cacheMap);
        mAllCreator.append(creator);
    }
    qDebug() << "created Creator* #" << mAllCreator.size();
}


/*
 * save List of Creator* to JSON cache
 * convert list of Creator* to QVariantList
 * toCacheMap stores all properties without transient values
 * Creator is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveCreatorToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Creator* #" << mAllCreator.size();
    for (int i = 0; i < mAllCreator.size(); ++i) {
        Creator* creator;
        creator = (Creator*)mAllCreator.at(i);
        QVariantMap cacheMap;
        cacheMap = creator->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Creator* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheCreator, cacheList);
}


void DataManager::resolveCreatorReferences(Creator* creator)
{
	if (!creator) {
        qDebug() << "cannot resolveCreatorReferences with creator NULL";
        return;
    }
    if(creator->isAllResolved()) {
	    qDebug() << "nothing to do: all is resolved";
	    return;
	}
    if (creator->hasCreatorImage() && !creator->isCreatorImageResolvedAsDataObject()) {
    	CreatorImage* creatorImage;
   		creatorImage = findCreatorImageByCreatorId(creator->creatorImage());
    	if (creatorImage) {
    		creator->resolveCreatorImageAsDataObject(creatorImage);
    	} else {
    		qDebug() << "markCreatorImageAsInvalid: " << creator->creatorImage();
    		creator->markCreatorImageAsInvalid();
    	}
    }
    if (!creator->areKeysKeysResolved()) {
        creator->resolveKeysKeys(
                listOfKeyForKeys(creator->keysKeys()));
    }
}

void DataManager::resolveReferencesForAllCreator()
{
    for (int i = 0; i < mAllCreator.size(); ++i) {
        Creator* creator;
        creator = (Creator*)mAllCreator.at(i);
    	resolveCreatorReferences(creator);
    }
}


/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Creator*> DataManager::listOfCreatorForKeys(
        QStringList keyList)
{
    QList<Creator*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllCreator.size(); ++i) {
        Creator* creator;
        creator = (Creator*) mAllCreator.at(i);
        if (keyList.contains(QString::number(creator->creatorId()))) {
            listOfData.append(creator);
            keyList.removeOne(QString::number(creator->creatorId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Creator: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::creatorAsQVariantList()
{
    QVariantList creatorList;
    for (int i = 0; i < mAllCreator.size(); ++i) {
        creatorList.append(((Creator*) (mAllCreator.at(i)))->toMap());
    }
    return creatorList;
}

QList<QObject*> DataManager::allCreator()
{
    return mAllCreator;
}

QQmlListProperty<Creator> DataManager::creatorPropertyList()
{
    return QQmlListProperty<Creator>(this, 0,
            &DataManager::appendToCreatorProperty, &DataManager::creatorPropertyCount,
            &DataManager::atCreatorProperty, &DataManager::clearCreatorProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Creator*
void DataManager::appendToCreatorProperty(
        QQmlListProperty<Creator> *creatorList,
        Creator* creator)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(creatorList->object);
    if (dataManagerObject) {
        creator->setParent(dataManagerObject);
        dataManagerObject->mAllCreator.append(creator);
        emit dataManagerObject->addedToAllCreator(creator);
    } else {
        qWarning() << "cannot append Creator* to mAllCreator "
                << "Object is not of type DataManager*";
    }
}
int DataManager::creatorPropertyCount(
        QQmlListProperty<Creator> *creatorList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(creatorList->object);
    if (dataManager) {
        return dataManager->mAllCreator.size();
    } else {
        qWarning() << "cannot get size mAllCreator " << "Object is not of type DataManager*";
    }
    return 0;
}
Creator* DataManager::atCreatorProperty(
        QQmlListProperty<Creator> *creatorList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(creatorList->object);
    if (dataManager) {
        if (dataManager->mAllCreator.size() > pos) {
            return (Creator*) dataManager->mAllCreator.at(pos);
        }
        qWarning() << "cannot get Creator* at pos " << pos << " size is "
                << dataManager->mAllCreator.size();
    } else {
        qWarning() << "cannot get Creator* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearCreatorProperty(
        QQmlListProperty<Creator> *creatorList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(creatorList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllCreator.size(); ++i) {
            Creator* creator;
            creator = (Creator*) dataManager->mAllCreator.at(i);
			emit dataManager->deletedFromAllCreatorByCreatorId(creator->creatorId());
			emit dataManager->deletedFromAllCreator(creator);
            creator->deleteLater();
            creator = 0;
        }
        dataManager->mAllCreator.clear();
    } else {
        qWarning() << "cannot clear mAllCreator " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Creator
 * and clears the list
 */
void DataManager::deleteCreator()
{
    for (int i = 0; i < mAllCreator.size(); ++i) {
        Creator* creator;
        creator = (Creator*) mAllCreator.at(i);
        emit deletedFromAllCreatorByCreatorId(creator->creatorId());
		emit deletedFromAllCreator(creator);
		emit creatorPropertyListChanged();
        creator->deleteLater();
        creator = 0;
     }
     mAllCreator.clear();
}

/**
 * creates a new Creator
 * parent is DataManager
 * if data is successfully entered you must insertCreator
 * if edit was canceled you must undoCreateCreator to free up memory
 */
Creator* DataManager::createCreator()
{
    Creator* creator;
    creator = new Creator();
    creator->setParent(this);
    creator->prepareNew();
    return creator;
}

/**
 * deletes Creator
 * if createCreator was canceled from UI
 * to delete a previous successfully inserted Creator
 * use deleteCreator
 */
void DataManager::undoCreateCreator(Creator* creator)
{
    if (creator) {
        qDebug() << "undoCreateCreator " << creator->creatorId();
        creator->deleteLater();
        creator = 0;
    }
}

void DataManager::insertCreator(Creator* creator)
{
    // Important: DataManager must be parent of all root DTOs
    creator->setParent(this);
    mAllCreator.append(creator);
    emit addedToAllCreator(creator);
    emit creatorPropertyListChanged();
}

void DataManager::insertCreatorFromMap(const QVariantMap& creatorMap,
        const bool& useForeignProperties)
{
    Creator* creator = new Creator();
    creator->setParent(this);
    if (useForeignProperties) {
        creator->fillFromForeignMap(creatorMap);
    } else {
        creator->fillFromMap(creatorMap);
    }
    mAllCreator.append(creator);
    emit addedToAllCreator(creator);
    creatorPropertyListChanged();
}

bool DataManager::deleteCreator(Creator* creator)
{
    bool ok = false;
    ok = mAllCreator.removeOne(creator);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllCreatorByCreatorId(creator->creatorId());
    emit deletedFromAllCreator(creator);
    emit creatorPropertyListChanged();
    creator->deleteLater();
    creator = 0;
    return ok;
}


bool DataManager::deleteCreatorByCreatorId(const int& creatorId)
{
    for (int i = 0; i < mAllCreator.size(); ++i) {
        Creator* creator;
        creator = (Creator*) mAllCreator.at(i);
        if (creator->creatorId() == creatorId) {
            mAllCreator.removeAt(i);
            emit deletedFromAllCreatorByCreatorId(creatorId);
            emit deletedFromAllCreator(creator);
            emit creatorPropertyListChanged();
            creator->deleteLater();
            creator = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Creator* DataManager::findCreatorByCreatorId(const int& creatorId){
    for (int i = 0; i < mAllCreator.size(); ++i) {
        Creator* creator;
        creator = (Creator*)mAllCreator.at(i);
        if(creator->creatorId() == creatorId){
            return creator;
        }
    }
    qDebug() << "no Creator found for creatorId " << creatorId;
    return 0;
}

/*
 * reads Maps of CreatorImage in from JSON cache
 * creates List of CreatorImage*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initCreatorImageFromCache()
{
	qDebug() << "start initCreatorImageFromCache";
    mAllCreatorImage.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheCreatorImage);
    qDebug() << "read CreatorImage from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        CreatorImage* creatorImage = new CreatorImage();
        // Important: DataManager must be parent of all root DTOs
        creatorImage->setParent(this);
        creatorImage->fillFromCacheMap(cacheMap);
        mAllCreatorImage.append(creatorImage);
    }
    qDebug() << "created CreatorImage* #" << mAllCreatorImage.size();
}


/*
 * save List of CreatorImage* to JSON cache
 * convert list of CreatorImage* to QVariantList
 * toCacheMap stores all properties without transient values
 * CreatorImage is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveCreatorImageToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching CreatorImage* #" << mAllCreatorImage.size();
    for (int i = 0; i < mAllCreatorImage.size(); ++i) {
        CreatorImage* creatorImage;
        creatorImage = (CreatorImage*)mAllCreatorImage.at(i);
        QVariantMap cacheMap;
        cacheMap = creatorImage->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "CreatorImage* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheCreatorImage, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<CreatorImage*> DataManager::listOfCreatorImageForKeys(
        QStringList keyList)
{
    QList<CreatorImage*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllCreatorImage.size(); ++i) {
        CreatorImage* creatorImage;
        creatorImage = (CreatorImage*) mAllCreatorImage.at(i);
        if (keyList.contains(QString::number(creatorImage->creatorId()))) {
            listOfData.append(creatorImage);
            keyList.removeOne(QString::number(creatorImage->creatorId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for CreatorImage: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::creatorImageAsQVariantList()
{
    QVariantList creatorImageList;
    for (int i = 0; i < mAllCreatorImage.size(); ++i) {
        creatorImageList.append(((CreatorImage*) (mAllCreatorImage.at(i)))->toMap());
    }
    return creatorImageList;
}

QList<QObject*> DataManager::allCreatorImage()
{
    return mAllCreatorImage;
}

QQmlListProperty<CreatorImage> DataManager::creatorImagePropertyList()
{
    return QQmlListProperty<CreatorImage>(this, 0,
            &DataManager::appendToCreatorImageProperty, &DataManager::creatorImagePropertyCount,
            &DataManager::atCreatorImageProperty, &DataManager::clearCreatorImageProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of CreatorImage*
void DataManager::appendToCreatorImageProperty(
        QQmlListProperty<CreatorImage> *creatorImageList,
        CreatorImage* creatorImage)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(creatorImageList->object);
    if (dataManagerObject) {
        creatorImage->setParent(dataManagerObject);
        dataManagerObject->mAllCreatorImage.append(creatorImage);
        emit dataManagerObject->addedToAllCreatorImage(creatorImage);
    } else {
        qWarning() << "cannot append CreatorImage* to mAllCreatorImage "
                << "Object is not of type DataManager*";
    }
}
int DataManager::creatorImagePropertyCount(
        QQmlListProperty<CreatorImage> *creatorImageList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(creatorImageList->object);
    if (dataManager) {
        return dataManager->mAllCreatorImage.size();
    } else {
        qWarning() << "cannot get size mAllCreatorImage " << "Object is not of type DataManager*";
    }
    return 0;
}
CreatorImage* DataManager::atCreatorImageProperty(
        QQmlListProperty<CreatorImage> *creatorImageList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(creatorImageList->object);
    if (dataManager) {
        if (dataManager->mAllCreatorImage.size() > pos) {
            return (CreatorImage*) dataManager->mAllCreatorImage.at(pos);
        }
        qWarning() << "cannot get CreatorImage* at pos " << pos << " size is "
                << dataManager->mAllCreatorImage.size();
    } else {
        qWarning() << "cannot get CreatorImage* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearCreatorImageProperty(
        QQmlListProperty<CreatorImage> *creatorImageList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(creatorImageList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllCreatorImage.size(); ++i) {
            CreatorImage* creatorImage;
            creatorImage = (CreatorImage*) dataManager->mAllCreatorImage.at(i);
			emit dataManager->deletedFromAllCreatorImageByCreatorId(creatorImage->creatorId());
			emit dataManager->deletedFromAllCreatorImage(creatorImage);
            creatorImage->deleteLater();
            creatorImage = 0;
        }
        dataManager->mAllCreatorImage.clear();
    } else {
        qWarning() << "cannot clear mAllCreatorImage " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all CreatorImage
 * and clears the list
 */
void DataManager::deleteCreatorImage()
{
    for (int i = 0; i < mAllCreatorImage.size(); ++i) {
        CreatorImage* creatorImage;
        creatorImage = (CreatorImage*) mAllCreatorImage.at(i);
        emit deletedFromAllCreatorImageByCreatorId(creatorImage->creatorId());
		emit deletedFromAllCreatorImage(creatorImage);
		emit creatorImagePropertyListChanged();
        creatorImage->deleteLater();
        creatorImage = 0;
     }
     mAllCreatorImage.clear();
}

/**
 * creates a new CreatorImage
 * parent is DataManager
 * if data is successfully entered you must insertCreatorImage
 * if edit was canceled you must undoCreateCreatorImage to free up memory
 */
CreatorImage* DataManager::createCreatorImage()
{
    CreatorImage* creatorImage;
    creatorImage = new CreatorImage();
    creatorImage->setParent(this);
    creatorImage->prepareNew();
    return creatorImage;
}

/**
 * deletes CreatorImage
 * if createCreatorImage was canceled from UI
 * to delete a previous successfully inserted CreatorImage
 * use deleteCreatorImage
 */
void DataManager::undoCreateCreatorImage(CreatorImage* creatorImage)
{
    if (creatorImage) {
        qDebug() << "undoCreateCreatorImage " << creatorImage->creatorId();
        creatorImage->deleteLater();
        creatorImage = 0;
    }
}

void DataManager::insertCreatorImage(CreatorImage* creatorImage)
{
    // Important: DataManager must be parent of all root DTOs
    creatorImage->setParent(this);
    mAllCreatorImage.append(creatorImage);
    emit addedToAllCreatorImage(creatorImage);
    emit creatorImagePropertyListChanged();
}

void DataManager::insertCreatorImageFromMap(const QVariantMap& creatorImageMap,
        const bool& useForeignProperties)
{
    CreatorImage* creatorImage = new CreatorImage();
    creatorImage->setParent(this);
    if (useForeignProperties) {
        creatorImage->fillFromForeignMap(creatorImageMap);
    } else {
        creatorImage->fillFromMap(creatorImageMap);
    }
    mAllCreatorImage.append(creatorImage);
    emit addedToAllCreatorImage(creatorImage);
    creatorImagePropertyListChanged();
}

bool DataManager::deleteCreatorImage(CreatorImage* creatorImage)
{
    bool ok = false;
    ok = mAllCreatorImage.removeOne(creatorImage);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllCreatorImageByCreatorId(creatorImage->creatorId());
    emit deletedFromAllCreatorImage(creatorImage);
    emit creatorImagePropertyListChanged();
    creatorImage->deleteLater();
    creatorImage = 0;
    return ok;
}


bool DataManager::deleteCreatorImageByCreatorId(const int& creatorId)
{
    for (int i = 0; i < mAllCreatorImage.size(); ++i) {
        CreatorImage* creatorImage;
        creatorImage = (CreatorImage*) mAllCreatorImage.at(i);
        if (creatorImage->creatorId() == creatorId) {
            mAllCreatorImage.removeAt(i);
            emit deletedFromAllCreatorImageByCreatorId(creatorId);
            emit deletedFromAllCreatorImage(creatorImage);
            emit creatorImagePropertyListChanged();
            creatorImage->deleteLater();
            creatorImage = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
CreatorImage* DataManager::findCreatorImageByCreatorId(const int& creatorId){
    for (int i = 0; i < mAllCreatorImage.size(); ++i) {
        CreatorImage* creatorImage;
        creatorImage = (CreatorImage*)mAllCreatorImage.at(i);
        if(creatorImage->creatorId() == creatorId){
            return creatorImage;
        }
    }
    qDebug() << "no CreatorImage found for creatorId " << creatorId;
    return 0;
}

/*
 * reads Maps of KeyTag in from JSON cache
 * creates List of KeyTag*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initKeyTagFromCache()
{
    qDebug() << "start initKeyTagFromCache";
    mAllKeyTag.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheKeyTag);
    qDebug() << "read KeyTag from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        KeyTag* keyTag = new KeyTag();
        // Important: DataManager must be parent of all root DTOs
        keyTag->setParent(this);
        keyTag->fillFromCacheMap(cacheMap);
        mAllKeyTag.append(keyTag);
    }
    qDebug() << "created KeyTag* #" << mAllKeyTag.size();
}


/*
 * save List of KeyTag* to JSON cache
 * convert list of KeyTag* to QVariantList
 * toCacheMap stores all properties without transient values
 * KeyTag is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveKeyTagToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching KeyTag* #" << mAllKeyTag.size();
    for (int i = 0; i < mAllKeyTag.size(); ++i) {
        KeyTag* keyTag;
        keyTag = (KeyTag*)mAllKeyTag.at(i);
        QVariantMap cacheMap;
        cacheMap = keyTag->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "KeyTag* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheKeyTag, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<KeyTag*> DataManager::listOfKeyTagForKeys(
        QStringList keyList)
{
    QList<KeyTag*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllKeyTag.size(); ++i) {
        KeyTag* keyTag;
        keyTag = (KeyTag*) mAllKeyTag.at(i);
        if (keyList.contains(QString::number(keyTag->tagId()))) {
            listOfData.append(keyTag);
            keyList.removeOne(QString::number(keyTag->tagId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for KeyTag: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::keyTagAsQVariantList()
{
    QVariantList keyTagList;
    for (int i = 0; i < mAllKeyTag.size(); ++i) {
        keyTagList.append(((KeyTag*) (mAllKeyTag.at(i)))->toMap());
    }
    return keyTagList;
}

QList<QObject*> DataManager::allKeyTag()
{
    return mAllKeyTag;
}

QQmlListProperty<KeyTag> DataManager::keyTagPropertyList()
{
    return QQmlListProperty<KeyTag>(this, 0,
            &DataManager::appendToKeyTagProperty, &DataManager::keyTagPropertyCount,
            &DataManager::atKeyTagProperty, &DataManager::clearKeyTagProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of KeyTag*
void DataManager::appendToKeyTagProperty(
        QQmlListProperty<KeyTag> *keyTagList,
        KeyTag* keyTag)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(keyTagList->object);
    if (dataManagerObject) {
        keyTag->setParent(dataManagerObject);
        dataManagerObject->mAllKeyTag.append(keyTag);
        emit dataManagerObject->addedToAllKeyTag(keyTag);
    } else {
        qWarning() << "cannot append KeyTag* to mAllKeyTag "
                << "Object is not of type DataManager*";
    }
}
int DataManager::keyTagPropertyCount(
        QQmlListProperty<KeyTag> *keyTagList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyTagList->object);
    if (dataManager) {
        return dataManager->mAllKeyTag.size();
    } else {
        qWarning() << "cannot get size mAllKeyTag " << "Object is not of type DataManager*";
    }
    return 0;
}
KeyTag* DataManager::atKeyTagProperty(
        QQmlListProperty<KeyTag> *keyTagList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyTagList->object);
    if (dataManager) {
        if (dataManager->mAllKeyTag.size() > pos) {
            return (KeyTag*) dataManager->mAllKeyTag.at(pos);
        }
        qWarning() << "cannot get KeyTag* at pos " << pos << " size is "
                << dataManager->mAllKeyTag.size();
    } else {
        qWarning() << "cannot get KeyTag* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearKeyTagProperty(
        QQmlListProperty<KeyTag> *keyTagList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyTagList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllKeyTag.size(); ++i) {
            KeyTag* keyTag;
            keyTag = (KeyTag*) dataManager->mAllKeyTag.at(i);
            emit dataManager->deletedFromAllKeyTagByTagId(keyTag->tagId());
            emit dataManager->deletedFromAllKeyTag(keyTag);
            keyTag->deleteLater();
            keyTag = 0;
        }
        dataManager->mAllKeyTag.clear();
    } else {
        qWarning() << "cannot clear mAllKeyTag " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all KeyTag
 * and clears the list
 */
void DataManager::deleteKeyTag()
{
    for (int i = 0; i < mAllKeyTag.size(); ++i) {
        KeyTag* keyTag;
        keyTag = (KeyTag*) mAllKeyTag.at(i);
        emit deletedFromAllKeyTagByTagId(keyTag->tagId());
        emit deletedFromAllKeyTag(keyTag);
		emit keyTagPropertyListChanged();
        keyTag->deleteLater();
        keyTag = 0;
     }
     mAllKeyTag.clear();
}

/**
 * creates a new KeyTag
 * parent is DataManager
 * if data is successfully entered you must insertKeyTag
 * if edit was canceled you must undoCreateKeyTag to free up memory
 */
KeyTag* DataManager::createKeyTag()
{
    KeyTag* keyTag;
    keyTag = new KeyTag();
    keyTag->setParent(this);
    keyTag->prepareNew();
    return keyTag;
}

/**
 * deletes KeyTag
 * if createKeyTag was canceled from UI
 * to delete a previous successfully inserted KeyTag
 * use deleteKeyTag
 */
void DataManager::undoCreateKeyTag(KeyTag* keyTag)
{
    if (keyTag) {
        qDebug() << "undoCreateKeyTag " << keyTag->tagId();
        keyTag->deleteLater();
        keyTag = 0;
    }
}

void DataManager::insertKeyTag(KeyTag* keyTag)
{
    // Important: DataManager must be parent of all root DTOs
    keyTag->setParent(this);
    mAllKeyTag.append(keyTag);
    emit addedToAllKeyTag(keyTag);
    emit keyTagPropertyListChanged();
}

void DataManager::insertKeyTagFromMap(const QVariantMap& keyTagMap,
        const bool& useForeignProperties)
{
    KeyTag* keyTag = new KeyTag();
    keyTag->setParent(this);
    if (useForeignProperties) {
        keyTag->fillFromForeignMap(keyTagMap);
    } else {
        keyTag->fillFromMap(keyTagMap);
    }
    mAllKeyTag.append(keyTag);
    emit addedToAllKeyTag(keyTag);
    keyTagPropertyListChanged();
}

bool DataManager::deleteKeyTag(KeyTag* keyTag)
{
    bool ok = false;
    ok = mAllKeyTag.removeOne(keyTag);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllKeyTagByTagId(keyTag->tagId());
    emit deletedFromAllKeyTag(keyTag);
    emit keyTagPropertyListChanged();
    keyTag->deleteLater();
    keyTag = 0;
    return ok;
}


bool DataManager::deleteKeyTagByTagId(const int& tagId)
{
    for (int i = 0; i < mAllKeyTag.size(); ++i) {
        KeyTag* keyTag;
        keyTag = (KeyTag*) mAllKeyTag.at(i);
        if (keyTag->tagId() == tagId) {
            mAllKeyTag.removeAt(i);
            emit deletedFromAllKeyTagByTagId(tagId);
            emit deletedFromAllKeyTag(keyTag);
            emit keyTagPropertyListChanged();
            keyTag->deleteLater();
            keyTag = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
KeyTag* DataManager::findKeyTagByTagId(const int& tagId){
    for (int i = 0; i < mAllKeyTag.size(); ++i) {
        KeyTag* keyTag;
        keyTag = (KeyTag*)mAllKeyTag.at(i);
        if(keyTag->tagId() == tagId){
            return keyTag;
        }
    }
    qDebug() << "no KeyTag found for tagId " << tagId;
    return 0;
}




/*
 * reads Maps of KeyContentBlock in from JSON cache
 * creates List of KeyContentBlock*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initKeyContentBlockFromCache()
{
    qDebug() << "start initKeyContentBlockFromCache";
    mAllKeyContentBlock.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheKeyContentBlock);
    qDebug() << "read KeyContentBlock from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        KeyContentBlock* keyContentBlock = new KeyContentBlock();
        // Important: DataManager must be parent of all root DTOs
        keyContentBlock->setParent(this);
        keyContentBlock->fillFromCacheMap(cacheMap);
        mAllKeyContentBlock.append(keyContentBlock);
    }
    qDebug() << "created KeyContentBlock* #" << mAllKeyContentBlock.size();
}


/*
 * save List of KeyContentBlock* to JSON cache
 * convert list of KeyContentBlock* to QVariantList
 * toCacheMap stores all properties without transient values
 * KeyContentBlock is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveKeyContentBlockToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching KeyContentBlock* #" << mAllKeyContentBlock.size();
    for (int i = 0; i < mAllKeyContentBlock.size(); ++i) {
        KeyContentBlock* keyContentBlock;
        keyContentBlock = (KeyContentBlock*)mAllKeyContentBlock.at(i);
        QVariantMap cacheMap;
        cacheMap = keyContentBlock->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "KeyContentBlock* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheKeyContentBlock, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<KeyContentBlock*> DataManager::listOfKeyContentBlockForKeys(
        QStringList keyList)
{
    QList<KeyContentBlock*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllKeyContentBlock.size(); ++i) {
        KeyContentBlock* keyContentBlock;
        keyContentBlock = (KeyContentBlock*) mAllKeyContentBlock.at(i);
        if (keyList.contains(QString::number(keyContentBlock->contentBlockId()))) {
            listOfData.append(keyContentBlock);
            keyList.removeOne(QString::number(keyContentBlock->contentBlockId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for KeyContentBlock: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::keyContentBlockAsQVariantList()
{
    QVariantList keyContentBlockList;
    for (int i = 0; i < mAllKeyContentBlock.size(); ++i) {
        keyContentBlockList.append(((KeyContentBlock*) (mAllKeyContentBlock.at(i)))->toMap());
    }
    return keyContentBlockList;
}

QList<QObject*> DataManager::allKeyContentBlock()
{
    return mAllKeyContentBlock;
}

QQmlListProperty<KeyContentBlock> DataManager::keyContentBlockPropertyList()
{
    return QQmlListProperty<KeyContentBlock>(this, 0,
            &DataManager::appendToKeyContentBlockProperty, &DataManager::keyContentBlockPropertyCount,
            &DataManager::atKeyContentBlockProperty, &DataManager::clearKeyContentBlockProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of KeyContentBlock*
void DataManager::appendToKeyContentBlockProperty(
        QQmlListProperty<KeyContentBlock> *keyContentBlockList,
        KeyContentBlock* keyContentBlock)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(keyContentBlockList->object);
    if (dataManagerObject) {
        keyContentBlock->setParent(dataManagerObject);
        dataManagerObject->mAllKeyContentBlock.append(keyContentBlock);
        emit dataManagerObject->addedToAllKeyContentBlock(keyContentBlock);
    } else {
        qWarning() << "cannot append KeyContentBlock* to mAllKeyContentBlock "
                << "Object is not of type DataManager*";
    }
}
int DataManager::keyContentBlockPropertyCount(
        QQmlListProperty<KeyContentBlock> *keyContentBlockList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyContentBlockList->object);
    if (dataManager) {
        return dataManager->mAllKeyContentBlock.size();
    } else {
        qWarning() << "cannot get size mAllKeyContentBlock " << "Object is not of type DataManager*";
    }
    return 0;
}
KeyContentBlock* DataManager::atKeyContentBlockProperty(
        QQmlListProperty<KeyContentBlock> *keyContentBlockList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyContentBlockList->object);
    if (dataManager) {
        if (dataManager->mAllKeyContentBlock.size() > pos) {
            return (KeyContentBlock*) dataManager->mAllKeyContentBlock.at(pos);
        }
        qWarning() << "cannot get KeyContentBlock* at pos " << pos << " size is "
                << dataManager->mAllKeyContentBlock.size();
    } else {
        qWarning() << "cannot get KeyContentBlock* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearKeyContentBlockProperty(
        QQmlListProperty<KeyContentBlock> *keyContentBlockList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyContentBlockList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllKeyContentBlock.size(); ++i) {
            KeyContentBlock* keyContentBlock;
            keyContentBlock = (KeyContentBlock*) dataManager->mAllKeyContentBlock.at(i);
            emit dataManager->deletedFromAllKeyContentBlockByContentBlockId(keyContentBlock->contentBlockId());
            emit dataManager->deletedFromAllKeyContentBlock(keyContentBlock);
            keyContentBlock->deleteLater();
            keyContentBlock = 0;
        }
        dataManager->mAllKeyContentBlock.clear();
    } else {
        qWarning() << "cannot clear mAllKeyContentBlock " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all KeyContentBlock
 * and clears the list
 */
void DataManager::deleteKeyContentBlock()
{
    for (int i = 0; i < mAllKeyContentBlock.size(); ++i) {
        KeyContentBlock* keyContentBlock;
        keyContentBlock = (KeyContentBlock*) mAllKeyContentBlock.at(i);
        emit deletedFromAllKeyContentBlockByContentBlockId(keyContentBlock->contentBlockId());
        emit deletedFromAllKeyContentBlock(keyContentBlock);
        emit keyContentBlockPropertyListChanged();
        keyContentBlock->deleteLater();
        keyContentBlock = 0;
     }
     mAllKeyContentBlock.clear();
}

/**
 * creates a new KeyContentBlock
 * parent is DataManager
 * if data is successfully entered you must insertKeyContentBlock
 * if edit was canceled you must undoCreateKeyContentBlock to free up memory
 */
KeyContentBlock* DataManager::createKeyContentBlock()
{
    KeyContentBlock* keyContentBlock;
    keyContentBlock = new KeyContentBlock();
    keyContentBlock->setParent(this);
    keyContentBlock->prepareNew();
    return keyContentBlock;
}

/**
 * deletes KeyContentBlock
 * if createKeyContentBlock was canceled from UI
 * to delete a previous successfully inserted KeyContentBlock
 * use deleteKeyContentBlock
 */
void DataManager::undoCreateKeyContentBlock(KeyContentBlock* keyContentBlock)
{
    if (keyContentBlock) {
        qDebug() << "undoCreateKeyContentBlock " << keyContentBlock->contentBlockId();
        keyContentBlock->deleteLater();
        keyContentBlock = 0;
    }
}

void DataManager::insertKeyContentBlock(KeyContentBlock* keyContentBlock)
{
    // Important: DataManager must be parent of all root DTOs
    keyContentBlock->setParent(this);
    mAllKeyContentBlock.append(keyContentBlock);
    emit addedToAllKeyContentBlock(keyContentBlock);
    emit keyContentBlockPropertyListChanged();
}

void DataManager::insertKeyContentBlockFromMap(const QVariantMap& keyContentBlockMap,
        const bool& useForeignProperties)
{
    KeyContentBlock* keyContentBlock = new KeyContentBlock();
    keyContentBlock->setParent(this);
    if (useForeignProperties) {
        keyContentBlock->fillFromForeignMap(keyContentBlockMap);
    } else {
        keyContentBlock->fillFromMap(keyContentBlockMap);
    }
    mAllKeyContentBlock.append(keyContentBlock);
    emit addedToAllKeyContentBlock(keyContentBlock);
    keyContentBlockPropertyListChanged();
}

bool DataManager::deleteKeyContentBlock(KeyContentBlock* keyContentBlock)
{
    bool ok = false;
    ok = mAllKeyContentBlock.removeOne(keyContentBlock);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllKeyContentBlockByContentBlockId(keyContentBlock->contentBlockId());
    emit deletedFromAllKeyContentBlock(keyContentBlock);
    emit keyContentBlockPropertyListChanged();
    keyContentBlock->deleteLater();
    keyContentBlock = 0;
    return ok;
}


bool DataManager::deleteKeyContentBlockByContentBlockId(const int& contentBlockId)
{
    for (int i = 0; i < mAllKeyContentBlock.size(); ++i) {
        KeyContentBlock* keyContentBlock;
        keyContentBlock = (KeyContentBlock*) mAllKeyContentBlock.at(i);
        if (keyContentBlock->contentBlockId() == contentBlockId) {
            mAllKeyContentBlock.removeAt(i);
            emit deletedFromAllKeyContentBlockByContentBlockId(contentBlockId);
            emit deletedFromAllKeyContentBlock(keyContentBlock);
            emit keyContentBlockPropertyListChanged();
            keyContentBlock->deleteLater();
            keyContentBlock = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
KeyContentBlock* DataManager::findKeyContentBlockByContentBlockId(const int& contentBlockId){
    for (int i = 0; i < mAllKeyContentBlock.size(); ++i) {
        KeyContentBlock* keyContentBlock;
        keyContentBlock = (KeyContentBlock*)mAllKeyContentBlock.at(i);
        if(keyContentBlock->contentBlockId() == contentBlockId){
            return keyContentBlock;
        }
    }
    qDebug() << "no KeyContentBlock found for contentBlockId " << contentBlockId;
    return 0;
}





/*
 * reads Maps of KeyContentStandardType in from JSON cache
 * creates List of KeyContentStandardType*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initKeyContentStandardTypeFromCache()
{
    qDebug() << "start initKeyContentStandardTypeFromCache";
    mAllKeyContentStandardType.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheKeyContentStandardType);
    qDebug() << "read KeyContentStandardType from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        KeyContentStandardType* keyContentStandardType = new KeyContentStandardType();
        // Important: DataManager must be parent of all root DTOs
        keyContentStandardType->setParent(this);
        keyContentStandardType->fillFromCacheMap(cacheMap);
        mAllKeyContentStandardType.append(keyContentStandardType);
    }
    qDebug() << "created KeyContentStandardType* #" << mAllKeyContentStandardType.size();
}


/*
 * save List of KeyContentStandardType* to JSON cache
 * convert list of KeyContentStandardType* to QVariantList
 * toCacheMap stores all properties without transient values
 * KeyContentStandardType is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveKeyContentStandardTypeToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching KeyContentStandardType* #" << mAllKeyContentStandardType.size();
    for (int i = 0; i < mAllKeyContentStandardType.size(); ++i) {
        KeyContentStandardType* keyContentStandardType;
        keyContentStandardType = (KeyContentStandardType*)mAllKeyContentStandardType.at(i);
        QVariantMap cacheMap;
        cacheMap = keyContentStandardType->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "KeyContentStandardType* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheKeyContentStandardType, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<KeyContentStandardType*> DataManager::listOfKeyContentStandardTypeForKeys(
        QStringList keyList)
{
    QList<KeyContentStandardType*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllKeyContentStandardType.size(); ++i) {
        KeyContentStandardType* keyContentStandardType;
        keyContentStandardType = (KeyContentStandardType*) mAllKeyContentStandardType.at(i);
        if (keyList.contains(QString::number(keyContentStandardType->contentStandardTypeId()))) {
            listOfData.append(keyContentStandardType);
            keyList.removeOne(QString::number(keyContentStandardType->contentStandardTypeId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for KeyContentStandardType: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::keyContentStandardTypeAsQVariantList()
{
    QVariantList keyContentStandardTypeList;
    for (int i = 0; i < mAllKeyContentStandardType.size(); ++i) {
        keyContentStandardTypeList.append(((KeyContentStandardType*) (mAllKeyContentStandardType.at(i)))->toMap());
    }
    return keyContentStandardTypeList;
}

QList<QObject*> DataManager::allKeyContentStandardType()
{
    return mAllKeyContentStandardType;
}

QQmlListProperty<KeyContentStandardType> DataManager::keyContentStandardTypePropertyList()
{
    return QQmlListProperty<KeyContentStandardType>(this, 0,
            &DataManager::appendToKeyContentStandardTypeProperty, &DataManager::keyContentStandardTypePropertyCount,
            &DataManager::atKeyContentStandardTypeProperty, &DataManager::clearKeyContentStandardTypeProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of KeyContentStandardType*
void DataManager::appendToKeyContentStandardTypeProperty(
        QQmlListProperty<KeyContentStandardType> *keyContentStandardTypeList,
        KeyContentStandardType* keyContentStandardType)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(keyContentStandardTypeList->object);
    if (dataManagerObject) {
        keyContentStandardType->setParent(dataManagerObject);
        dataManagerObject->mAllKeyContentStandardType.append(keyContentStandardType);
        emit dataManagerObject->addedToAllKeyContentStandardType(keyContentStandardType);
    } else {
        qWarning() << "cannot append KeyContentStandardType* to mAllKeyContentStandardType "
                << "Object is not of type DataManager*";
    }
}
int DataManager::keyContentStandardTypePropertyCount(
        QQmlListProperty<KeyContentStandardType> *keyContentStandardTypeList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyContentStandardTypeList->object);
    if (dataManager) {
        return dataManager->mAllKeyContentStandardType.size();
    } else {
        qWarning() << "cannot get size mAllKeyContentStandardType " << "Object is not of type DataManager*";
    }
    return 0;
}
KeyContentStandardType* DataManager::atKeyContentStandardTypeProperty(
        QQmlListProperty<KeyContentStandardType> *keyContentStandardTypeList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyContentStandardTypeList->object);
    if (dataManager) {
        if (dataManager->mAllKeyContentStandardType.size() > pos) {
            return (KeyContentStandardType*) dataManager->mAllKeyContentStandardType.at(pos);
        }
        qWarning() << "cannot get KeyContentStandardType* at pos " << pos << " size is "
                << dataManager->mAllKeyContentStandardType.size();
    } else {
        qWarning() << "cannot get KeyContentStandardType* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearKeyContentStandardTypeProperty(
        QQmlListProperty<KeyContentStandardType> *keyContentStandardTypeList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyContentStandardTypeList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllKeyContentStandardType.size(); ++i) {
            KeyContentStandardType* keyContentStandardType;
            keyContentStandardType = (KeyContentStandardType*) dataManager->mAllKeyContentStandardType.at(i);
            emit dataManager->deletedFromAllKeyContentStandardTypeByContentStandardTypeId(keyContentStandardType->contentStandardTypeId());
            emit dataManager->deletedFromAllKeyContentStandardType(keyContentStandardType);
            keyContentStandardType->deleteLater();
            keyContentStandardType = 0;
        }
        dataManager->mAllKeyContentStandardType.clear();
    } else {
        qWarning() << "cannot clear mAllKeyContentStandardType " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all KeyContentStandardType
 * and clears the list
 */
void DataManager::deleteKeyContentStandardType()
{
    for (int i = 0; i < mAllKeyContentStandardType.size(); ++i) {
        KeyContentStandardType* keyContentStandardType;
        keyContentStandardType = (KeyContentStandardType*) mAllKeyContentStandardType.at(i);
        emit deletedFromAllKeyContentStandardTypeByContentStandardTypeId(keyContentStandardType->contentStandardTypeId());
        emit deletedFromAllKeyContentStandardType(keyContentStandardType);
        emit keyContentStandardTypePropertyListChanged();
        keyContentStandardType->deleteLater();
        keyContentStandardType = 0;
     }
     mAllKeyContentStandardType.clear();
}

/**
 * creates a new KeyContentStandardType
 * parent is DataManager
 * if data is successfully entered you must insertKeyContentStandardType
 * if edit was canceled you must undoCreateKeyContentStandardType to free up memory
 */
KeyContentStandardType* DataManager::createKeyContentStandardType()
{
    KeyContentStandardType* keyContentStandardType;
    keyContentStandardType = new KeyContentStandardType();
    keyContentStandardType->setParent(this);
    keyContentStandardType->prepareNew();
    return keyContentStandardType;
}

/**
 * deletes KeyContentStandardType
 * if createKeyContentStandardType was canceled from UI
 * to delete a previous successfully inserted KeyContentStandardType
 * use deleteKeyContentStandardType
 */
void DataManager::undoCreateKeyContentStandardType(KeyContentStandardType* keyContentStandardType)
{
    if (keyContentStandardType) {
        qDebug() << "undoCreateKeyContentStandardType " << keyContentStandardType->contentStandardTypeId();
        keyContentStandardType->deleteLater();
        keyContentStandardType = 0;
    }
}

void DataManager::insertKeyContentStandardType(KeyContentStandardType* keyContentStandardType)
{
    // Important: DataManager must be parent of all root DTOs
    keyContentStandardType->setParent(this);
    mAllKeyContentStandardType.append(keyContentStandardType);
    emit addedToAllKeyContentStandardType(keyContentStandardType);
    emit keyContentStandardTypePropertyListChanged();
}

void DataManager::insertKeyContentStandardTypeFromMap(const QVariantMap& keyContentStandardTypeMap,
        const bool& useForeignProperties)
{
    KeyContentStandardType* keyContentStandardType = new KeyContentStandardType();
    keyContentStandardType->setParent(this);
    if (useForeignProperties) {
        keyContentStandardType->fillFromForeignMap(keyContentStandardTypeMap);
    } else {
        keyContentStandardType->fillFromMap(keyContentStandardTypeMap);
    }
    mAllKeyContentStandardType.append(keyContentStandardType);
    emit addedToAllKeyContentStandardType(keyContentStandardType);
    keyContentStandardTypePropertyListChanged();
}

bool DataManager::deleteKeyContentStandardType(KeyContentStandardType* keyContentStandardType)
{
    bool ok = false;
    ok = mAllKeyContentStandardType.removeOne(keyContentStandardType);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllKeyContentStandardTypeByContentStandardTypeId(keyContentStandardType->contentStandardTypeId());
    emit deletedFromAllKeyContentStandardType(keyContentStandardType);
    emit keyContentStandardTypePropertyListChanged();
    keyContentStandardType->deleteLater();
    keyContentStandardType = 0;
    return ok;
}


bool DataManager::deleteKeyContentStandardTypeByContentStandardTypeId(const int& contentStandardTypeId)
{
    for (int i = 0; i < mAllKeyContentStandardType.size(); ++i) {
        KeyContentStandardType* keyContentStandardType;
        keyContentStandardType = (KeyContentStandardType*) mAllKeyContentStandardType.at(i);
        if (keyContentStandardType->contentStandardTypeId() == contentStandardTypeId) {
            mAllKeyContentStandardType.removeAt(i);
            emit deletedFromAllKeyContentStandardTypeByContentStandardTypeId(contentStandardTypeId);
            emit deletedFromAllKeyContentStandardType(keyContentStandardType);
            emit keyContentStandardTypePropertyListChanged();
            keyContentStandardType->deleteLater();
            keyContentStandardType = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
KeyContentStandardType* DataManager::findKeyContentStandardTypeByContentStandardTypeId(const int& contentStandardTypeId){
    for (int i = 0; i < mAllKeyContentStandardType.size(); ++i) {
        KeyContentStandardType* keyContentStandardType;
        keyContentStandardType = (KeyContentStandardType*)mAllKeyContentStandardType.at(i);
        if(keyContentStandardType->contentStandardTypeId() == contentStandardTypeId){
            return keyContentStandardType;
        }
    }
    qDebug() << "no KeyContentStandardType found for contentStandardTypeId " << contentStandardTypeId;
    return 0;
}




/*
 * reads Maps of Day in from JSON cache
 * creates List of Day*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initDayFromCache()
{
	qDebug() << "start initDayFromCache";
    mAllDay.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheDay);
    qDebug() << "read Day from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Day* day = new Day();
        // Important: DataManager must be parent of all root DTOs
        day->setParent(this);
        day->fillFromCacheMap(cacheMap);
        mAllDay.append(day);
    }
    qDebug() << "created Day* #" << mAllDay.size();
}


/*
 * save List of Day* to JSON cache
 * convert list of Day* to QVariantList
 * toCacheMap stores all properties without transient values
 * Day is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveDayToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Day* #" << mAllDay.size();
    for (int i = 0; i < mAllDay.size(); ++i) {
        Day* day;
        day = (Day*)mAllDay.at(i);
        QVariantMap cacheMap;
        cacheMap = day->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Day* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheDay, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Day*> DataManager::listOfDayForKeys(
        QStringList keyList)
{
    QList<Day*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllDay.size(); ++i) {
        Day* day;
        day = (Day*) mAllDay.at(i);
        if (keyList.contains(QString::number(day->id()))) {
            listOfData.append(day);
            keyList.removeOne(QString::number(day->id()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Day: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::dayAsQVariantList()
{
    QVariantList levelList;
    for (int i = 0; i < mAllDay.size(); ++i) {
        levelList.append(((Day*) (mAllDay.at(i)))->toMap());
    }
    return levelList;
}

QList<QObject*> DataManager::allDay()
{
    return mAllDay;
}

QQmlListProperty<Day> DataManager::dayPropertyList()
{
    return QQmlListProperty<Day>(this, 0,
            &DataManager::appendToDayProperty, &DataManager::dayPropertyCount,
            &DataManager::atDayProperty, &DataManager::clearDayProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Day*
void DataManager::appendToDayProperty(
        QQmlListProperty<Day> *levelList,
        Day* day)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(levelList->object);
    if (dataManagerObject) {
        day->setParent(dataManagerObject);
        dataManagerObject->mAllDay.append(day);
        emit dataManagerObject->addedToAllDay(day);
    } else {
        qWarning() << "cannot append Day* to mAllDay "
                << "Object is not of type DataManager*";
    }
}
int DataManager::dayPropertyCount(
        QQmlListProperty<Day> *levelList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(levelList->object);
    if (dataManager) {
        return dataManager->mAllDay.size();
    } else {
        qWarning() << "cannot get size mAllDay " << "Object is not of type DataManager*";
    }
    return 0;
}
Day* DataManager::atDayProperty(
        QQmlListProperty<Day> *levelList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(levelList->object);
    if (dataManager) {
        if (dataManager->mAllDay.size() > pos) {
            return (Day*) dataManager->mAllDay.at(pos);
        }
        qWarning() << "cannot get Day* at pos " << pos << " size is "
                << dataManager->mAllDay.size();
    } else {
        qWarning() << "cannot get Day* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearDayProperty(
        QQmlListProperty<Day> *levelList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(levelList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllDay.size(); ++i) {
            Day* day;
            day = (Day*) dataManager->mAllDay.at(i);
			emit dataManager->deletedFromAllDayById(day->id());
			emit dataManager->deletedFromAllDay(day);
            day->deleteLater();
            day = 0;
        }
        dataManager->mAllDay.clear();
    } else {
        qWarning() << "cannot clear mAllDay " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Day
 * and clears the list
 */
void DataManager::deleteDay()
{
    for (int i = 0; i < mAllDay.size(); ++i) {
        Day* day;
        day = (Day*) mAllDay.at(i);
        emit deletedFromAllDayById(day->id());
		emit deletedFromAllDay(day);
		emit dayPropertyListChanged();
        day->deleteLater();
        day = 0;
     }
     mAllDay.clear();
}

/**
 * creates a new Day
 * parent is DataManager
 * if data is successfully entered you must insertDay
 * if edit was canceled you must undoCreateDay to free up memory
 */
Day* DataManager::createDay()
{
    Day* day;
    day = new Day();
    day->setParent(this);
    day->prepareNew();
    return day;
}

/**
 * deletes Day
 * if createDay was canceled from UI
 * to delete a previous successfully inserted Day
 * use deleteDay
 */
void DataManager::undoCreateDay(Day* day)
{
    if (day) {
        qDebug() << "undoCreateDay " << day->id();
        day->deleteLater();
        day = 0;
    }
}

void DataManager::insertDay(Day* day)
{
    // Important: DataManager must be parent of all root DTOs
    day->setParent(this);
    mAllDay.append(day);
    emit addedToAllDay(day);
    emit dayPropertyListChanged();
}

void DataManager::insertDayFromMap(const QVariantMap& dayMap,
        const bool& useForeignProperties)
{
    Day* day = new Day();
    day->setParent(this);
    if (useForeignProperties) {
        day->fillFromForeignMap(dayMap);
    } else {
        day->fillFromMap(dayMap);
    }
    mAllDay.append(day);
    emit addedToAllDay(day);
    dayPropertyListChanged();
}

bool DataManager::deleteDay(Day* day)
{
    bool ok = false;
    ok = mAllDay.removeOne(day);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllDayById(day->id());
    emit deletedFromAllDay(day);
    emit dayPropertyListChanged();
    day->deleteLater();
    day = 0;
    return ok;
}


bool DataManager::deleteDayById(const int& id)
{
    for (int i = 0; i < mAllDay.size(); ++i) {
        Day* day;
        day = (Day*) mAllDay.at(i);
        if (day->id() == id) {
            mAllDay.removeAt(i);
            emit deletedFromAllDayById(id);
            emit deletedFromAllDay(day);
            emit dayPropertyListChanged();
            day->deleteLater();
            day = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Day* DataManager::findDayById(const int& id){
    for (int i = 0; i < mAllDay.size(); ++i) {
        Day* day;
        day = (Day*)mAllDay.at(i);
        if(day->id() == id){
            return day;
        }
    }
    qDebug() << "no Day found for id " << id;
    return 0;
}

/*
 * reads Maps of KeyAPI in from JSON cache
 * creates List of KeyAPI*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initKeyAPIFromCache()
{
    qDebug() << "start initKeyAPIFromCache";
    mAllKeyAPI.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheKeyAPI);
    qDebug() << "read KeyAPI from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        KeyAPI* keyAPI = new KeyAPI();
        // Important: DataManager must be parent of all root DTOs
        keyAPI->setParent(this);
        keyAPI->fillFromCacheMap(cacheMap);
        mAllKeyAPI.append(keyAPI);
    }
    qDebug() << "created KeyAPI* #" << mAllKeyAPI.size();
}


/*
 * save List of KeyAPI* to JSON cache
 * convert list of KeyAPI* to QVariantList
 * toCacheMap stores all properties without transient values
 * KeyAPI is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveKeyAPIToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching KeyAPI* #" << mAllKeyAPI.size();
    for (int i = 0; i < mAllKeyAPI.size(); ++i) {
        KeyAPI* keyAPI;
        keyAPI = (KeyAPI*)mAllKeyAPI.at(i);
        QVariantMap cacheMap;
        cacheMap = keyAPI->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "KeyAPI* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheKeyAPI, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<KeyAPI*> DataManager::listOfKeyAPIForKeys(
        QStringList keyList)
{
    QList<KeyAPI*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllKeyAPI.size(); ++i) {
        KeyAPI* keyAPI;
        keyAPI = (KeyAPI*) mAllKeyAPI.at(i);
        if (keyList.contains(QString::number(keyAPI->keyId()))) {
            listOfData.append(keyAPI);
            keyList.removeOne(QString::number(keyAPI->keyId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for KeyAPI: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::keyAPIAsQVariantList()
{
    QVariantList keyAPIList;
    for (int i = 0; i < mAllKeyAPI.size(); ++i) {
        keyAPIList.append(((KeyAPI*) (mAllKeyAPI.at(i)))->toMap());
    }
    return keyAPIList;
}

QList<QObject*> DataManager::allKeyAPI()
{
    return mAllKeyAPI;
}

QQmlListProperty<KeyAPI> DataManager::keyAPIPropertyList()
{
    return QQmlListProperty<KeyAPI>(this, 0,
            &DataManager::appendToKeyAPIProperty, &DataManager::keyAPIPropertyCount,
            &DataManager::atKeyAPIProperty, &DataManager::clearKeyAPIProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of KeyAPI*
void DataManager::appendToKeyAPIProperty(
        QQmlListProperty<KeyAPI> *keyAPIList,
        KeyAPI* keyAPI)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(keyAPIList->object);
    if (dataManagerObject) {
        keyAPI->setParent(dataManagerObject);
        dataManagerObject->mAllKeyAPI.append(keyAPI);
        emit dataManagerObject->addedToAllKeyAPI(keyAPI);
    } else {
        qWarning() << "cannot append KeyAPI* to mAllKeyAPI "
                << "Object is not of type DataManager*";
    }
}
int DataManager::keyAPIPropertyCount(
        QQmlListProperty<KeyAPI> *keyAPIList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyAPIList->object);
    if (dataManager) {
        return dataManager->mAllKeyAPI.size();
    } else {
        qWarning() << "cannot get size mAllKeyAPI " << "Object is not of type DataManager*";
    }
    return 0;
}
KeyAPI* DataManager::atKeyAPIProperty(
        QQmlListProperty<KeyAPI> *keyAPIList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyAPIList->object);
    if (dataManager) {
        if (dataManager->mAllKeyAPI.size() > pos) {
            return (KeyAPI*) dataManager->mAllKeyAPI.at(pos);
        }
        qWarning() << "cannot get KeyAPI* at pos " << pos << " size is "
                << dataManager->mAllKeyAPI.size();
    } else {
        qWarning() << "cannot get KeyAPI* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearKeyAPIProperty(
        QQmlListProperty<KeyAPI> *keyAPIList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyAPIList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllKeyAPI.size(); ++i) {
            KeyAPI* keyAPI;
            keyAPI = (KeyAPI*) dataManager->mAllKeyAPI.at(i);
            emit dataManager->deletedFromAllKeyAPIByKeyId(keyAPI->keyId());
            emit dataManager->deletedFromAllKeyAPI(keyAPI);
            keyAPI->deleteLater();
            keyAPI = 0;
        }
        dataManager->mAllKeyAPI.clear();
    } else {
        qWarning() << "cannot clear mAllKeyAPI " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all KeyAPI
 * and clears the list
 */
void DataManager::deleteKeyAPI()
{
    for (int i = 0; i < mAllKeyAPI.size(); ++i) {
        KeyAPI* keyAPI;
        keyAPI = (KeyAPI*) mAllKeyAPI.at(i);
        emit deletedFromAllKeyAPIByKeyId(keyAPI->keyId());
        emit deletedFromAllKeyAPI(keyAPI);
		emit keyAPIPropertyListChanged();
        keyAPI->deleteLater();
        keyAPI = 0;
     }
     mAllKeyAPI.clear();
}

/**
 * creates a new KeyAPI
 * parent is DataManager
 * if data is successfully entered you must insertKeyAPI
 * if edit was canceled you must undoCreateKeyAPI to free up memory
 */
KeyAPI* DataManager::createKeyAPI()
{
    KeyAPI* keyAPI;
    keyAPI = new KeyAPI();
    keyAPI->setParent(this);
    keyAPI->prepareNew();
    return keyAPI;
}

/**
 * deletes KeyAPI
 * if createKeyAPI was canceled from UI
 * to delete a previous successfully inserted KeyAPI
 * use deleteKeyAPI
 */
void DataManager::undoCreateKeyAPI(KeyAPI* keyAPI)
{
    if (keyAPI) {
        qDebug() << "undoCreateKeyAPI " << keyAPI->keyId();
        keyAPI->deleteLater();
        keyAPI = 0;
    }
}

void DataManager::insertKeyAPI(KeyAPI* keyAPI)
{
    // Important: DataManager must be parent of all root DTOs
    keyAPI->setParent(this);
    mAllKeyAPI.append(keyAPI);
    emit addedToAllKeyAPI(keyAPI);
    emit keyAPIPropertyListChanged();
}

void DataManager::insertKeyAPIFromMap(const QVariantMap& keyAPIMap,
        const bool& useForeignProperties)
{
    KeyAPI* keyAPI = new KeyAPI();
    keyAPI->setParent(this);
    if (useForeignProperties) {
        keyAPI->fillFromForeignMap(keyAPIMap);
    } else {
        keyAPI->fillFromMap(keyAPIMap);
    }
    mAllKeyAPI.append(keyAPI);
    emit addedToAllKeyAPI(keyAPI);
    keyAPIPropertyListChanged();
}

bool DataManager::deleteKeyAPI(KeyAPI* keyAPI)
{
    bool ok = false;
    ok = mAllKeyAPI.removeOne(keyAPI);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllKeyAPIByKeyId(keyAPI->keyId());
    emit deletedFromAllKeyAPI(keyAPI);
    emit keyAPIPropertyListChanged();
    keyAPI->deleteLater();
    keyAPI = 0;
    return ok;
}


bool DataManager::deleteKeyAPIByKeyId(const int& keyId)
{
    for (int i = 0; i < mAllKeyAPI.size(); ++i) {
        KeyAPI* keyAPI;
        keyAPI = (KeyAPI*) mAllKeyAPI.at(i);
        if (keyAPI->keyId() == keyId) {
            mAllKeyAPI.removeAt(i);
            emit deletedFromAllKeyAPIByKeyId(keyId);
            emit deletedFromAllKeyAPI(keyAPI);
            emit keyAPIPropertyListChanged();
            keyAPI->deleteLater();
            keyAPI = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
KeyAPI* DataManager::findKeyAPIByKeyId(const int& keyId){
    for (int i = 0; i < mAllKeyAPI.size(); ++i) {
        KeyAPI* keyAPI;
        keyAPI = (KeyAPI*)mAllKeyAPI.at(i);
        if(keyAPI->keyId() == keyId){
            return keyAPI;
        }
    }
    qDebug() << "no KeyAPI found for keyId " << keyId;
    return 0;
}

/*
 * reads Maps of PersonsAPI in from JSON cache
 * creates List of PersonsAPI*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initPersonsAPIFromCache()
{
	qDebug() << "start initPersonsAPIFromCache";
    mAllPersonsAPI.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cachePersonsAPI);
    qDebug() << "read PersonsAPI from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        PersonsAPI* creatorsAPI = new PersonsAPI();
        // Important: DataManager must be parent of all root DTOs
        creatorsAPI->setParent(this);
        creatorsAPI->fillFromCacheMap(cacheMap);
        mAllPersonsAPI.append(creatorsAPI);
    }
    qDebug() << "created PersonsAPI* #" << mAllPersonsAPI.size();
}


/*
 * save List of PersonsAPI* to JSON cache
 * convert list of PersonsAPI* to QVariantList
 * toCacheMap stores all properties without transient values
 * PersonsAPI is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::savePersonsAPIToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching PersonsAPI* #" << mAllPersonsAPI.size();
    for (int i = 0; i < mAllPersonsAPI.size(); ++i) {
        PersonsAPI* creatorsAPI;
        creatorsAPI = (PersonsAPI*)mAllPersonsAPI.at(i);
        QVariantMap cacheMap;
        cacheMap = creatorsAPI->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "PersonsAPI* converted to JSON cache #" << cacheList.size();
    writeToCache(cachePersonsAPI, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<PersonsAPI*> DataManager::listOfPersonsAPIForKeys(
        QStringList keyList)
{
    QList<PersonsAPI*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllPersonsAPI.size(); ++i) {
        PersonsAPI* creatorsAPI;
        creatorsAPI = (PersonsAPI*) mAllPersonsAPI.at(i);
        if (keyList.contains(QString::number(creatorsAPI->creatorId()))) {
            listOfData.append(creatorsAPI);
            keyList.removeOne(QString::number(creatorsAPI->creatorId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for PersonsAPI: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::creatorsAPIAsQVariantList()
{
    QVariantList creatorsAPIList;
    for (int i = 0; i < mAllPersonsAPI.size(); ++i) {
        creatorsAPIList.append(((PersonsAPI*) (mAllPersonsAPI.at(i)))->toMap());
    }
    return creatorsAPIList;
}

QList<QObject*> DataManager::allPersonsAPI()
{
    return mAllPersonsAPI;
}

QQmlListProperty<PersonsAPI> DataManager::creatorsAPIPropertyList()
{
    return QQmlListProperty<PersonsAPI>(this, 0,
            &DataManager::appendToPersonsAPIProperty, &DataManager::creatorsAPIPropertyCount,
            &DataManager::atPersonsAPIProperty, &DataManager::clearPersonsAPIProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of PersonsAPI*
void DataManager::appendToPersonsAPIProperty(
        QQmlListProperty<PersonsAPI> *creatorsAPIList,
        PersonsAPI* creatorsAPI)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(creatorsAPIList->object);
    if (dataManagerObject) {
        creatorsAPI->setParent(dataManagerObject);
        dataManagerObject->mAllPersonsAPI.append(creatorsAPI);
        emit dataManagerObject->addedToAllPersonsAPI(creatorsAPI);
    } else {
        qWarning() << "cannot append PersonsAPI* to mAllPersonsAPI "
                << "Object is not of type DataManager*";
    }
}
int DataManager::creatorsAPIPropertyCount(
        QQmlListProperty<PersonsAPI> *creatorsAPIList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(creatorsAPIList->object);
    if (dataManager) {
        return dataManager->mAllPersonsAPI.size();
    } else {
        qWarning() << "cannot get size mAllPersonsAPI " << "Object is not of type DataManager*";
    }
    return 0;
}
PersonsAPI* DataManager::atPersonsAPIProperty(
        QQmlListProperty<PersonsAPI> *creatorsAPIList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(creatorsAPIList->object);
    if (dataManager) {
        if (dataManager->mAllPersonsAPI.size() > pos) {
            return (PersonsAPI*) dataManager->mAllPersonsAPI.at(pos);
        }
        qWarning() << "cannot get PersonsAPI* at pos " << pos << " size is "
                << dataManager->mAllPersonsAPI.size();
    } else {
        qWarning() << "cannot get PersonsAPI* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearPersonsAPIProperty(
        QQmlListProperty<PersonsAPI> *creatorsAPIList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(creatorsAPIList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllPersonsAPI.size(); ++i) {
            PersonsAPI* creatorsAPI;
            creatorsAPI = (PersonsAPI*) dataManager->mAllPersonsAPI.at(i);
			emit dataManager->deletedFromAllPersonsAPIByCreatorId(creatorsAPI->creatorId());
			emit dataManager->deletedFromAllPersonsAPI(creatorsAPI);
            creatorsAPI->deleteLater();
            creatorsAPI = 0;
        }
        dataManager->mAllPersonsAPI.clear();
    } else {
        qWarning() << "cannot clear mAllPersonsAPI " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all PersonsAPI
 * and clears the list
 */
void DataManager::deletePersonsAPI()
{
    for (int i = 0; i < mAllPersonsAPI.size(); ++i) {
        PersonsAPI* creatorsAPI;
        creatorsAPI = (PersonsAPI*) mAllPersonsAPI.at(i);
        emit deletedFromAllPersonsAPIByCreatorId(creatorsAPI->creatorId());
		emit deletedFromAllPersonsAPI(creatorsAPI);
		emit creatorsAPIPropertyListChanged();
        creatorsAPI->deleteLater();
        creatorsAPI = 0;
     }
     mAllPersonsAPI.clear();
}

/**
 * creates a new PersonsAPI
 * parent is DataManager
 * if data is successfully entered you must insertPersonsAPI
 * if edit was canceled you must undoCreatePersonsAPI to free up memory
 */
PersonsAPI* DataManager::createPersonsAPI()
{
    PersonsAPI* creatorsAPI;
    creatorsAPI = new PersonsAPI();
    creatorsAPI->setParent(this);
    creatorsAPI->prepareNew();
    return creatorsAPI;
}

/**
 * deletes PersonsAPI
 * if createPersonsAPI was canceled from UI
 * to delete a previous successfully inserted PersonsAPI
 * use deletePersonsAPI
 */
void DataManager::undoCreatePersonsAPI(PersonsAPI* creatorsAPI)
{
    if (creatorsAPI) {
        qDebug() << "undoCreatePersonsAPI " << creatorsAPI->creatorId();
        creatorsAPI->deleteLater();
        creatorsAPI = 0;
    }
}

void DataManager::insertPersonsAPI(PersonsAPI* creatorsAPI)
{
    // Important: DataManager must be parent of all root DTOs
    creatorsAPI->setParent(this);
    mAllPersonsAPI.append(creatorsAPI);
    emit addedToAllPersonsAPI(creatorsAPI);
    emit creatorsAPIPropertyListChanged();
}

void DataManager::insertPersonsAPIFromMap(const QVariantMap& creatorsAPIMap,
        const bool& useForeignProperties)
{
    PersonsAPI* creatorsAPI = new PersonsAPI();
    creatorsAPI->setParent(this);
    if (useForeignProperties) {
        creatorsAPI->fillFromForeignMap(creatorsAPIMap);
    } else {
        creatorsAPI->fillFromMap(creatorsAPIMap);
    }
    mAllPersonsAPI.append(creatorsAPI);
    emit addedToAllPersonsAPI(creatorsAPI);
    creatorsAPIPropertyListChanged();
}

bool DataManager::deletePersonsAPI(PersonsAPI* creatorsAPI)
{
    bool ok = false;
    ok = mAllPersonsAPI.removeOne(creatorsAPI);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllPersonsAPIByCreatorId(creatorsAPI->creatorId());
    emit deletedFromAllPersonsAPI(creatorsAPI);
    emit creatorsAPIPropertyListChanged();
    creatorsAPI->deleteLater();
    creatorsAPI = 0;
    return ok;
}


bool DataManager::deletePersonsAPIByCreatorId(const int& creatorId)
{
    for (int i = 0; i < mAllPersonsAPI.size(); ++i) {
        PersonsAPI* creatorsAPI;
        creatorsAPI = (PersonsAPI*) mAllPersonsAPI.at(i);
        if (creatorsAPI->creatorId() == creatorId) {
            mAllPersonsAPI.removeAt(i);
            emit deletedFromAllPersonsAPIByCreatorId(creatorId);
            emit deletedFromAllPersonsAPI(creatorsAPI);
            emit creatorsAPIPropertyListChanged();
            creatorsAPI->deleteLater();
            creatorsAPI = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
PersonsAPI* DataManager::findPersonsAPIByCreatorId(const int& creatorId){
    for (int i = 0; i < mAllPersonsAPI.size(); ++i) {
        PersonsAPI* creatorsAPI;
        creatorsAPI = (PersonsAPI*)mAllPersonsAPI.at(i);
        if(creatorsAPI->creatorId() == creatorId){
            return creatorsAPI;
        }
    }
    qDebug() << "no PersonsAPI found for creatorId " << creatorId;
    return 0;
}

/*
 * reads Maps of KeyLinkAPI in from JSON cache
 * creates List of KeyLinkAPI*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initKeyLinkAPIFromCache()
{
    qDebug() << "start initKeyLinkAPIFromCache";
    mAllKeyLinkAPI.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheKeyLinkAPI);
    qDebug() << "read KeyLinkAPI from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        KeyLinkAPI* keyLinkAPI = new KeyLinkAPI();
        // Important: DataManager must be parent of all root DTOs
        keyLinkAPI->setParent(this);
        keyLinkAPI->fillFromCacheMap(cacheMap);
        mAllKeyLinkAPI.append(keyLinkAPI);
    }
    qDebug() << "created KeyLinkAPI* #" << mAllKeyLinkAPI.size();
}


/*
 * save List of KeyLinkAPI* to JSON cache
 * convert list of KeyLinkAPI* to QVariantList
 * toCacheMap stores all properties without transient values
 * KeyLinkAPI is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveKeyLinkAPIToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching KeyLinkAPI* #" << mAllKeyLinkAPI.size();
    for (int i = 0; i < mAllKeyLinkAPI.size(); ++i) {
        KeyLinkAPI* keyLinkAPI;
        keyLinkAPI = (KeyLinkAPI*)mAllKeyLinkAPI.at(i);
        QVariantMap cacheMap;
        cacheMap = keyLinkAPI->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "KeyLinkAPI* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheKeyLinkAPI, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<KeyLinkAPI*> DataManager::listOfKeyLinkAPIForKeys(
        QStringList keyList)
{
    QList<KeyLinkAPI*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllKeyLinkAPI.size(); ++i) {
        KeyLinkAPI* keyLinkAPI;
        keyLinkAPI = (KeyLinkAPI*) mAllKeyLinkAPI.at(i);
        if (keyList.contains(keyLinkAPI->uuid())) {
            listOfData.append(keyLinkAPI);
            keyList.removeOne(keyLinkAPI->uuid());
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for KeyLinkAPI: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::keyLinkAPIAsQVariantList()
{
    QVariantList keyLinkAPIList;
    for (int i = 0; i < mAllKeyLinkAPI.size(); ++i) {
        keyLinkAPIList.append(((KeyLinkAPI*) (mAllKeyLinkAPI.at(i)))->toMap());
    }
    return keyLinkAPIList;
}

QList<QObject*> DataManager::allKeyLinkAPI()
{
    return mAllKeyLinkAPI;
}

QQmlListProperty<KeyLinkAPI> DataManager::keyLinkAPIPropertyList()
{
    return QQmlListProperty<KeyLinkAPI>(this, 0,
            &DataManager::appendToKeyLinkAPIProperty, &DataManager::keyLinkAPIPropertyCount,
            &DataManager::atKeyLinkAPIProperty, &DataManager::clearKeyLinkAPIProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of KeyLinkAPI*
void DataManager::appendToKeyLinkAPIProperty(
        QQmlListProperty<KeyLinkAPI> *keyLinkAPIList,
        KeyLinkAPI* keyLinkAPI)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(keyLinkAPIList->object);
    if (dataManagerObject) {
        keyLinkAPI->setParent(dataManagerObject);
        dataManagerObject->mAllKeyLinkAPI.append(keyLinkAPI);
        emit dataManagerObject->addedToAllKeyLinkAPI(keyLinkAPI);
    } else {
        qWarning() << "cannot append KeyLinkAPI* to mAllKeyLinkAPI "
                << "Object is not of type DataManager*";
    }
}
int DataManager::keyLinkAPIPropertyCount(
        QQmlListProperty<KeyLinkAPI> *keyLinkAPIList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyLinkAPIList->object);
    if (dataManager) {
        return dataManager->mAllKeyLinkAPI.size();
    } else {
        qWarning() << "cannot get size mAllKeyLinkAPI " << "Object is not of type DataManager*";
    }
    return 0;
}
KeyLinkAPI* DataManager::atKeyLinkAPIProperty(
        QQmlListProperty<KeyLinkAPI> *keyLinkAPIList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyLinkAPIList->object);
    if (dataManager) {
        if (dataManager->mAllKeyLinkAPI.size() > pos) {
            return (KeyLinkAPI*) dataManager->mAllKeyLinkAPI.at(pos);
        }
        qWarning() << "cannot get KeyLinkAPI* at pos " << pos << " size is "
                << dataManager->mAllKeyLinkAPI.size();
    } else {
        qWarning() << "cannot get KeyLinkAPI* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearKeyLinkAPIProperty(
        QQmlListProperty<KeyLinkAPI> *keyLinkAPIList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(keyLinkAPIList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllKeyLinkAPI.size(); ++i) {
            KeyLinkAPI* keyLinkAPI;
            keyLinkAPI = (KeyLinkAPI*) dataManager->mAllKeyLinkAPI.at(i);
            emit dataManager->deletedFromAllKeyLinkAPIByUuid(keyLinkAPI->uuid());
            emit dataManager->deletedFromAllKeyLinkAPI(keyLinkAPI);
            keyLinkAPI->deleteLater();
            keyLinkAPI = 0;
        }
        dataManager->mAllKeyLinkAPI.clear();
    } else {
        qWarning() << "cannot clear mAllKeyLinkAPI " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all KeyLinkAPI
 * and clears the list
 */
void DataManager::deleteKeyLinkAPI()
{
    for (int i = 0; i < mAllKeyLinkAPI.size(); ++i) {
        KeyLinkAPI* keyLinkAPI;
        keyLinkAPI = (KeyLinkAPI*) mAllKeyLinkAPI.at(i);
        emit deletedFromAllKeyLinkAPIByUuid(keyLinkAPI->uuid());
        emit deletedFromAllKeyLinkAPI(keyLinkAPI);
		emit keyLinkAPIPropertyListChanged();
        keyLinkAPI->deleteLater();
        keyLinkAPI = 0;
     }
     mAllKeyLinkAPI.clear();
}

/**
 * creates a new KeyLinkAPI
 * parent is DataManager
 * if data is successfully entered you must insertKeyLinkAPI
 * if edit was canceled you must undoCreateKeyLinkAPI to free up memory
 */
KeyLinkAPI* DataManager::createKeyLinkAPI()
{
    KeyLinkAPI* keyLinkAPI;
    keyLinkAPI = new KeyLinkAPI();
    keyLinkAPI->setParent(this);
    keyLinkAPI->prepareNew();
    return keyLinkAPI;
}

/**
 * deletes KeyLinkAPI
 * if createKeyLinkAPI was canceled from UI
 * to delete a previous successfully inserted KeyLinkAPI
 * use deleteKeyLinkAPI
 */
void DataManager::undoCreateKeyLinkAPI(KeyLinkAPI* keyLinkAPI)
{
    if (keyLinkAPI) {
        qDebug() << "undoCreateKeyLinkAPI " << keyLinkAPI->uuid();
        keyLinkAPI->deleteLater();
        keyLinkAPI = 0;
    }
}

void DataManager::insertKeyLinkAPI(KeyLinkAPI* keyLinkAPI)
{
    // Important: DataManager must be parent of all root DTOs
    keyLinkAPI->setParent(this);
    mAllKeyLinkAPI.append(keyLinkAPI);
    emit addedToAllKeyLinkAPI(keyLinkAPI);
    emit keyLinkAPIPropertyListChanged();
}

void DataManager::insertKeyLinkAPIFromMap(const QVariantMap& keyLinkAPIMap,
        const bool& useForeignProperties)
{
    KeyLinkAPI* keyLinkAPI = new KeyLinkAPI();
    keyLinkAPI->setParent(this);
    if (useForeignProperties) {
        keyLinkAPI->fillFromForeignMap(keyLinkAPIMap);
    } else {
        keyLinkAPI->fillFromMap(keyLinkAPIMap);
    }
    mAllKeyLinkAPI.append(keyLinkAPI);
    emit addedToAllKeyLinkAPI(keyLinkAPI);
    keyLinkAPIPropertyListChanged();
}

bool DataManager::deleteKeyLinkAPI(KeyLinkAPI* keyLinkAPI)
{
    bool ok = false;
    ok = mAllKeyLinkAPI.removeOne(keyLinkAPI);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllKeyLinkAPIByUuid(keyLinkAPI->uuid());
    emit deletedFromAllKeyLinkAPI(keyLinkAPI);
    emit keyLinkAPIPropertyListChanged();
    keyLinkAPI->deleteLater();
    keyLinkAPI = 0;
    return ok;
}

bool DataManager::deleteKeyLinkAPIByUuid(const QString& uuid)
{
    if (uuid.isNull() || uuid.isEmpty()) {
        qDebug() << "cannot delete KeyLinkAPI from empty uuid";
        return false;
    }
    for (int i = 0; i < mAllKeyLinkAPI.size(); ++i) {
        KeyLinkAPI* keyLinkAPI;
        keyLinkAPI = (KeyLinkAPI*) mAllKeyLinkAPI.at(i);
        if (keyLinkAPI->uuid() == uuid) {
            mAllKeyLinkAPI.removeAt(i);
            emit deletedFromAllKeyLinkAPIByUuid(uuid);
            emit deletedFromAllKeyLinkAPI(keyLinkAPI);
            emit keyLinkAPIPropertyListChanged();
            keyLinkAPI->deleteLater();
            keyLinkAPI = 0;
            return true;
        }
    }
    return false;
}


KeyLinkAPI* DataManager::findKeyLinkAPIByUuid(const QString& uuid){
    if (uuid.isNull() || uuid.isEmpty()) {
        qDebug() << "cannot find KeyLinkAPI from empty uuid";
        return 0;
    }
    for (int i = 0; i < mAllKeyLinkAPI.size(); ++i) {
        KeyLinkAPI* keyLinkAPI;
        keyLinkAPI = (KeyLinkAPI*)mAllKeyLinkAPI.at(i);
        if(keyLinkAPI->uuid() == uuid){
            return keyLinkAPI;
        }
    }
    qDebug() << "no KeyLinkAPI found for uuid " << uuid;
    return 0;
}


/*
 * reads Maps of CreatorAPI in from JSON cache
 * creates List of CreatorAPI*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initCreatorAPIFromCache()
{
	qDebug() << "start initCreatorAPIFromCache";
    mAllCreatorAPI.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheCreatorAPI);
    qDebug() << "read CreatorAPI from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        CreatorAPI* creatorAPI = new CreatorAPI();
        // Important: DataManager must be parent of all root DTOs
        creatorAPI->setParent(this);
        creatorAPI->fillFromCacheMap(cacheMap);
        mAllCreatorAPI.append(creatorAPI);
    }
    qDebug() << "created CreatorAPI* #" << mAllCreatorAPI.size();
}


/*
 * save List of CreatorAPI* to JSON cache
 * convert list of CreatorAPI* to QVariantList
 * toCacheMap stores all properties without transient values
 * CreatorAPI is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveCreatorAPIToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching CreatorAPI* #" << mAllCreatorAPI.size();
    for (int i = 0; i < mAllCreatorAPI.size(); ++i) {
        CreatorAPI* creatorAPI;
        creatorAPI = (CreatorAPI*)mAllCreatorAPI.at(i);
        QVariantMap cacheMap;
        cacheMap = creatorAPI->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "CreatorAPI* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheCreatorAPI, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<CreatorAPI*> DataManager::listOfCreatorAPIForKeys(
        QStringList keyList)
{
    QList<CreatorAPI*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllCreatorAPI.size(); ++i) {
        CreatorAPI* creatorAPI;
        creatorAPI = (CreatorAPI*) mAllCreatorAPI.at(i);
        if (keyList.contains(QString::number(creatorAPI->id()))) {
            listOfData.append(creatorAPI);
            keyList.removeOne(QString::number(creatorAPI->id()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for CreatorAPI: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::creatorAPIAsQVariantList()
{
    QVariantList creatorAPIList;
    for (int i = 0; i < mAllCreatorAPI.size(); ++i) {
        creatorAPIList.append(((CreatorAPI*) (mAllCreatorAPI.at(i)))->toMap());
    }
    return creatorAPIList;
}

QList<QObject*> DataManager::allCreatorAPI()
{
    return mAllCreatorAPI;
}

QQmlListProperty<CreatorAPI> DataManager::creatorAPIPropertyList()
{
    return QQmlListProperty<CreatorAPI>(this, 0,
            &DataManager::appendToCreatorAPIProperty, &DataManager::creatorAPIPropertyCount,
            &DataManager::atCreatorAPIProperty, &DataManager::clearCreatorAPIProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of CreatorAPI*
void DataManager::appendToCreatorAPIProperty(
        QQmlListProperty<CreatorAPI> *creatorAPIList,
        CreatorAPI* creatorAPI)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(creatorAPIList->object);
    if (dataManagerObject) {
        creatorAPI->setParent(dataManagerObject);
        dataManagerObject->mAllCreatorAPI.append(creatorAPI);
        emit dataManagerObject->addedToAllCreatorAPI(creatorAPI);
    } else {
        qWarning() << "cannot append CreatorAPI* to mAllCreatorAPI "
                << "Object is not of type DataManager*";
    }
}
int DataManager::creatorAPIPropertyCount(
        QQmlListProperty<CreatorAPI> *creatorAPIList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(creatorAPIList->object);
    if (dataManager) {
        return dataManager->mAllCreatorAPI.size();
    } else {
        qWarning() << "cannot get size mAllCreatorAPI " << "Object is not of type DataManager*";
    }
    return 0;
}
CreatorAPI* DataManager::atCreatorAPIProperty(
        QQmlListProperty<CreatorAPI> *creatorAPIList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(creatorAPIList->object);
    if (dataManager) {
        if (dataManager->mAllCreatorAPI.size() > pos) {
            return (CreatorAPI*) dataManager->mAllCreatorAPI.at(pos);
        }
        qWarning() << "cannot get CreatorAPI* at pos " << pos << " size is "
                << dataManager->mAllCreatorAPI.size();
    } else {
        qWarning() << "cannot get CreatorAPI* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearCreatorAPIProperty(
        QQmlListProperty<CreatorAPI> *creatorAPIList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(creatorAPIList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllCreatorAPI.size(); ++i) {
            CreatorAPI* creatorAPI;
            creatorAPI = (CreatorAPI*) dataManager->mAllCreatorAPI.at(i);
			emit dataManager->deletedFromAllCreatorAPIById(creatorAPI->id());
			emit dataManager->deletedFromAllCreatorAPI(creatorAPI);
            creatorAPI->deleteLater();
            creatorAPI = 0;
        }
        dataManager->mAllCreatorAPI.clear();
    } else {
        qWarning() << "cannot clear mAllCreatorAPI " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all CreatorAPI
 * and clears the list
 */
void DataManager::deleteCreatorAPI()
{
    for (int i = 0; i < mAllCreatorAPI.size(); ++i) {
        CreatorAPI* creatorAPI;
        creatorAPI = (CreatorAPI*) mAllCreatorAPI.at(i);
        emit deletedFromAllCreatorAPIById(creatorAPI->id());
		emit deletedFromAllCreatorAPI(creatorAPI);
		emit creatorAPIPropertyListChanged();
        creatorAPI->deleteLater();
        creatorAPI = 0;
     }
     mAllCreatorAPI.clear();
}

/**
 * creates a new CreatorAPI
 * parent is DataManager
 * if data is successfully entered you must insertCreatorAPI
 * if edit was canceled you must undoCreateCreatorAPI to free up memory
 */
CreatorAPI* DataManager::createCreatorAPI()
{
    CreatorAPI* creatorAPI;
    creatorAPI = new CreatorAPI();
    creatorAPI->setParent(this);
    creatorAPI->prepareNew();
    return creatorAPI;
}

/**
 * deletes CreatorAPI
 * if createCreatorAPI was canceled from UI
 * to delete a previous successfully inserted CreatorAPI
 * use deleteCreatorAPI
 */
void DataManager::undoCreateCreatorAPI(CreatorAPI* creatorAPI)
{
    if (creatorAPI) {
        qDebug() << "undoCreateCreatorAPI " << creatorAPI->id();
        creatorAPI->deleteLater();
        creatorAPI = 0;
    }
}

void DataManager::insertCreatorAPI(CreatorAPI* creatorAPI)
{
    // Important: DataManager must be parent of all root DTOs
    creatorAPI->setParent(this);
    mAllCreatorAPI.append(creatorAPI);
    emit addedToAllCreatorAPI(creatorAPI);
    emit creatorAPIPropertyListChanged();
}

void DataManager::insertCreatorAPIFromMap(const QVariantMap& creatorAPIMap,
        const bool& useForeignProperties)
{
    CreatorAPI* creatorAPI = new CreatorAPI();
    creatorAPI->setParent(this);
    if (useForeignProperties) {
        creatorAPI->fillFromForeignMap(creatorAPIMap);
    } else {
        creatorAPI->fillFromMap(creatorAPIMap);
    }
    mAllCreatorAPI.append(creatorAPI);
    emit addedToAllCreatorAPI(creatorAPI);
    creatorAPIPropertyListChanged();
}

bool DataManager::deleteCreatorAPI(CreatorAPI* creatorAPI)
{
    bool ok = false;
    ok = mAllCreatorAPI.removeOne(creatorAPI);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllCreatorAPIById(creatorAPI->id());
    emit deletedFromAllCreatorAPI(creatorAPI);
    emit creatorAPIPropertyListChanged();
    creatorAPI->deleteLater();
    creatorAPI = 0;
    return ok;
}


bool DataManager::deleteCreatorAPIById(const int& id)
{
    for (int i = 0; i < mAllCreatorAPI.size(); ++i) {
        CreatorAPI* creatorAPI;
        creatorAPI = (CreatorAPI*) mAllCreatorAPI.at(i);
        if (creatorAPI->id() == id) {
            mAllCreatorAPI.removeAt(i);
            emit deletedFromAllCreatorAPIById(id);
            emit deletedFromAllCreatorAPI(creatorAPI);
            emit creatorAPIPropertyListChanged();
            creatorAPI->deleteLater();
            creatorAPI = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
CreatorAPI* DataManager::findCreatorAPIById(const int& id){
    for (int i = 0; i < mAllCreatorAPI.size(); ++i) {
        CreatorAPI* creatorAPI;
        creatorAPI = (CreatorAPI*)mAllCreatorAPI.at(i);
        if(creatorAPI->id() == id){
            return creatorAPI;
        }
    }
    qDebug() << "no CreatorAPI found for id " << id;
    return 0;
}

	

SettingsData* DataManager::settingsData()
{
    return mSettingsData;
}

void DataManager::readSettings()
{
    qDebug() << "Read the Settings File";
    mSettingsData = new SettingsData();
    mSettingsData->setParent(this);
    //
    QString assetsFilePath;

    QFile readFile(mSettingsPath);
    if(!readFile.exists()) {
        qDebug() << "settings cache doesn't exist: " << mSettingsPath;
        assetsFilePath = mDataAssetsPath+cacheSettingsData;
        QFile assetDataFile(assetsFilePath);
        if(assetDataFile.exists()) {
            // copy file from assets to data
            bool copyOk = assetDataFile.copy(mSettingsPath);
            if (!copyOk) {
                qDebug() << "cannot copy settings from data-assets to cache";
                return;
            }
            // IMPORTANT !!! copying from RESOURCES ":/data-assets/" to AppDataLocation
            // makes the target file READ ONLY - you must set PERMISSIONS
            // copying from RESOURCES ":/data-assets/" to GenericDataLocation the target is READ-WRITE
            copyOk = readFile.setPermissions(QFileDevice::ReadUser | QFileDevice::WriteUser);
            if (!copyOk) {
                qDebug() << "cannot set Permissions to read / write settings";
                return;
            }
        } else {
            qDebug() << "no settings from data-assets: " << assetsFilePath;
            return;
        }
    }
    if (!readFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file: " << mSettingsPath;
        return;
    }
    // create JSON Document from settings file
    QJsonDocument jda = QJsonDocument::fromJson(readFile.readAll());
    readFile.close();
    if(!jda.isObject()) {
        qWarning() << "Couldn't create JSON from file: " << mSettingsPath;
        return;
    }
    // create SettingsData* from JSON
    mSettingsData->fillFromMap(jda.toVariant().toMap());
    isProductionEnvironment = mSettingsData->isProductionEnvironment();
    qDebug() << "Settings* created";
}

void DataManager::saveSettings()
{
    qDebug() << "Save the Settings";
    // convert Settings* into JSONDocument and store to app data
    QJsonDocument jda = QJsonDocument::fromVariant(mSettingsData->toMap());
    // save JSON to data directory
    QFile saveFile(mSettingsPath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open file to write " << mSettingsPath;
        return;
    }
    qint64 bytesWritten = saveFile.write(jda.toJson());
    saveFile.close();
    qDebug() << "SettingsData Bytes written: " << bytesWritten;
}

/*
 * reads data in from stored cache
 * if no cache found tries to get data from assets/datamodel
 */
QVariantList DataManager::readFromCache(const QString& fileName)
{
    QJsonDocument jda;
    QVariantList cacheList;
    QString cacheFilePath = dataPath(fileName);
    QFile dataFile(cacheFilePath);
    // check if already something cached
    if (!dataFile.exists()) {
        // check if there are some pre-defined data in data-assets
        QString dataAssetsFilePath = dataAssetsPath(fileName);
        qDebug() << fileName << "not found in cache" << cacheFilePath;
        qDebug() << "try copy from: " << dataAssetsFilePath;
        QFile dataAssetsFile(dataAssetsFilePath);
        if (dataAssetsFile.exists()) {
            // copy file from data-assets to cached data
            bool copyOk = dataAssetsFile.copy(cacheFilePath);
            if (!copyOk) {
                qDebug() << "cannot copy " << dataAssetsFilePath << " to " << cacheFilePath;
                return cacheList;
            }
            if(!mSettingsData->hasPublicCache()) {
                // IMPORTANT !!! copying from RESOURCES ":/data-assets/" to AppDataLocation
                // makes the target file READ ONLY - you must set PERMISSIONS
                // copying from RESOURCES ":/data-assets/" to GenericDataLocation the target is READ-WRITE
                copyOk = dataFile.setPermissions(QFileDevice::ReadUser | QFileDevice::WriteUser);
                if (!copyOk) {
                    qDebug() << "cannot set Permissions to read / write settings";
                    return cacheList;
                }
            }
        } else {
            // no cache, no prefilled data-assets - empty list
            return cacheList;
        }
    }
    if (!dataFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file: " << cacheFilePath;
        return cacheList;
    }
    jda = QJsonDocument::fromJson(dataFile.readAll());
    dataFile.close();
    if(!jda.isArray()) {
        qWarning() << "Couldn't create JSON Array from file: " << cacheFilePath;
        return cacheList;
    }
    cacheList = jda.toVariant().toList();
    return cacheList;
}

void DataManager::writeToCache(const QString& fileName, QVariantList& data)
{
    QString cacheFilePath = dataPath(fileName);
    QJsonDocument jda = QJsonDocument::fromVariant(data);

    QFile saveFile(cacheFilePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open file to write " << cacheFilePath;
        return;
    }
    qint64 bytesWritten = saveFile.write(jda.toJson(mCompactJson?QJsonDocument::Compact:QJsonDocument::Indented));
    saveFile.close();
    qDebug() << "Data Bytes written: " << bytesWritten << " to: " << cacheFilePath;
}

DataManager::~DataManager()
{
    // clean up
}
