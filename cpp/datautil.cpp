#include "datautil.hpp"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QImage>

const QString YYYY_MM_DD = "yyyy-MM-dd";
const QString HH_MM = "HH:mm";
const QString LOCAL_HH_MM = "hh:mm";
const QString YYYY_MM_DD_HH_MM = "yyyy-MM-ddHH:mm";
const QString DAYNAME = "dddd";
const QString DAYNAME_HH_MM = "dddd, HH:mm";
const QString DEFAULT_CREATOR_IMAGE_URL = "http://conf.qtcon.org/person_original.png";
const QString EMPTY_TAG = "*****";
const QString EMPTY_CONTENTBLOCK = "*****";
const QString EMPTY_CONTENTSTANDARDTYPE = "*****";

DataUtil::DataUtil(QObject *parent) : QObject(parent)
{

}

void DataUtil::init(DataManager* dataManager, DataServer* dataServer)
{
    mDataManager = dataManager;
    mDataServer = dataServer;
    mRepositoryDataPath = mDataManager->mDataPath + "repository/";
    mDataServer->setRepositoryDataPath(mRepositoryDataPath);
    // used for temp dynamic lists as QQmlPropertyLists
    mKeyLists = mDataManager->createKeyLists();

    // connections
    bool res = connect(mDataServer, SIGNAL(serverSuccess()), this,
                       SLOT(onServerSuccess()));
    if (!res) {
        Q_ASSERT(res);
    }
    res = connect(mDataServer, SIGNAL(serverFailed(QString)), this,
                  SLOT(onServerFailed(QString)));
    if (!res) {
        Q_ASSERT(res);
    }

    res = connect(mDataServer, SIGNAL(versionSuccess(QByteArray)), this,
                  SLOT(onVersionSuccess(QByteArray)));
    if (!res) {
        Q_ASSERT(res);
    }
    res = connect(mDataServer, SIGNAL(versionFailed(QString)), this,
                  SLOT(onVersionFailed(QString)));
    if (!res) {
        Q_ASSERT(res);
    }
}

QString DataUtil::repositoryDataPath4QML() {
    return "file://"+mRepositoryDataPath;
}

bool DataUtil::isDateTooLate()
{
    QString todayDate = QDate::currentDate().toString(YYYY_MM_DD);
    QString lastRepositoryDay = ((Day*) mDataManager->mAllDay.last())->repositoryDay().toString(YYYY_MM_DD);
    qDebug() << "todayDate" << todayDate << "lastRepositoryDay" << lastRepositoryDay;
    return todayDate > lastRepositoryDay;
}

/**
 * @brief DataUtil::keyInfoForCreator
 * @param creator
 * @return all Key Title line by line
 */
QString DataUtil::keyInfoForCreator(Creator *creator)
{
    QString info;
    if(!creator) {
        return info;
    }
    for (int i = 0; i < creator->keys().size(); ++i) {
        if(i > 0) {
            info.append("\n");
        }
        info.append(creator->keys().at(i)->title());
    }
    return info;
}

QString DataUtil::creatorNamesForKey(Key *key)
{
    QString info;
    if(!key) {
        return info;
    }
    for (int i = 0; i < key->creator().size(); ++i) {
        QString name = key->creator().at(i)->name();
        if(name.length()) {
            if(info.length()) {
                info.append(", ");
            }
            info.append(name);
        }
    }
    return info;
}

QString DataUtil::dictionaryItemImageForKey(Key *key)
{
    if(!key || !key->hasDictionaryItem()) {
        return "";
    }
    if(key->dictionaryItemAsDataObject()->isRegistration()) {
        return "key.png";
    }
    if(key->dictionaryItemAsDataObject()->isLunch()) {
        return "lunch.png";
    }
    if(key->dictionaryItemAsDataObject()->isEvent()) {
        return "party_event.png";
    }
    return "break.png";
}

QString DataUtil::sectionForButton(Key *key)
{
    if(!key) {
        return "E";
    }
    if(key->isRefView()) {
        return "V";
    }
    if(key->isBoolean2()) {
        return "2";
    }
    if(key->isBoolean3()) {
        return "3";
    }
    if(key->isBoolean4()) {
        return "4";
    }
    if(key->isBoolean5()) {
        return "5";
    }
    if(key->isBoolean6()) {
        return "6";
    }
    if(key->isBoolean7()) {
        return "7";
    }
    if(key->isBoolean8()) {
        return "8";
    }
    return "T";
}


QString DataUtil::textForKeyType(Key *key)
{
    if(!key) {
        return "";
    }
    QString info = "";
    if(key->hasDictionaryItem()) {
        if(key->dictionaryItemAsDataObject()->isRegistration()) {
            return tr("Registration")+info;
        }
        if(key->dictionaryItemAsDataObject()->isEvent()) {
            return tr("Event")+info;
        }
        if(key->dictionaryItemAsDataObject()->isLunch()) {
            return tr("Lunch")+info;
        }
        return tr("Break")+info;
    }
    if(key->isRefView()) {
        return tr("isRefView")+info;
    }
    if(key->isBoolean2()) {
        return tr("isBoolean2")+info;
    }
    if(key->isBoolean3()) {
        return tr("isBoolean3")+info;
    }
    if(key->isBoolean4()) {
        return tr("isBoolean4")+info;
    }
    if(key->isBoolean5()) {
        return tr("isBoolean5")+info;
    }
    if(key->isBoolean6()) {
        return tr("isBoolean6")+info;
    }
    if(key->isBoolean7()) {
        return tr("isBoolean7")+info;
    }
    if(key->isBoolean8()) {
        return tr("isBoolean8")+info;
    }
    return tr("Key")+info;
}

QString DataUtil::textForKeyTag(KeyTag *keyTag)
{
    if(keyTag->name() == "*****") {
        return "";
    }
    return keyTag->name();//.right(keyTag->name().length()-3);
}

QString DataUtil::textForKeyContentBlock(KeyContentBlock *keyContentBlock)
{
    if(keyContentBlock->name() == "*****") {
        return "";
    }
    return keyContentBlock->name().right(keyContentBlock->name().length()-2);
}

QString DataUtil::textForKeyContentStandardType(KeyContentStandardType *keyContentStandardType)
{
    if(keyContentStandardType->name() == "*****") {
        return "";
    }
    return keyContentStandardType->name().right(keyContentStandardType->name().length()-2);
}


QString DataUtil::tagColor(const int tagId)
{
    switch (tagId) {
    case 0:
        // *****
        return "transparent";
    case 3:
        // 3D
        return "#FFCDD2";
    case 18:
        // Automotive, Mobile and Embedded
        return "#F06292";
    case 21:
        // Best practices
        return "#BA68C8";
    case 14:
        // Beyond Code
        return "#D1C4E9";
    case 2:
        // Break "#FFFFFF"
        return "grey";
    case 23:
        // Codecs
        return "#E8EAF6";
    case 4:
        // isRefView
        return "#B3E5FC";
    case 12:
        // Distributing Software and Resources
        return "#64B5F6";
    case 6:
        // Free Software policies and politics
        return "#4DD0E1";
    case 16:
        // Geolocation
        return "#CDDC39";
    case 13:
        // In depth
        return "#26A69A";
    case 10:
        // KDE‘s Latest and Greatest
        return "#2196F3";
    case 5:
        // Let‘s talk business
        return "#FFEB3B";
    case 24:
        // Let‘s talk software
        return "#FFC107";
    case 1:
        // Misc
        return "#FF9890";
    case 17:
        // Multithreading
        return "#FF7043";
    case 22:
        // OpenGL and 3D
        return "#E53935";
    case 11:
        // Platforms and Integration
        return "#BCAAA4";
    case 9:
        // Qt in Use
        return "#4CAF50";
    case 15:
        // QtQuick
        return "#8BC34A";
    case 20:
        // Technical Discussions
        return "#8D6E63";
    case 8:
        // Testing and Continuous Integration
        return "#E0E0E0";
    case 7:
        // Tooling
        return "#90A4AE";
    case 19:
        // Web
        return "#607D8B";
    default:
        // transparent
        return "transparent";
    }
}

QString DataUtil::contentBlockColor(const int contentBlockId)
{
    switch (contentBlockId) {
    case 0:
        // *****
        return "transparent";
    case 3:
        // 3D
        return "#FFCDD2";
    case 18:
        // Automotive, Mobile and Embedded
        return "#F06292";
    case 21:
        // Best practices
        return "#BA68C8";
    case 14:
        // Beyond Code
        return "#D1C4E9";
    case 2:
        // Break "#FFFFFF"
        return "grey";
    case 23:
        // Codecs
        return "#E8EAF6";
    case 4:
        // isRefView
        return "#B3E5FC";
    case 12:
        // Distributing Software and Resources
        return "#64B5F6";
    case 6:
        // Free Software policies and politics
        return "#4DD0E1";
    case 16:
        // Geolocation
        return "#CDDC39";
    case 13:
        // In depth
        return "#26A69A";
    case 10:
        // KDE‘s Latest and Greatest
        return "#2196F3";
    case 5:
        // Let‘s talk business
        return "#FFEB3B";
    case 24:
        // Let‘s talk software
        return "#FFC107";
    case 1:
        // Misc
        return "#FF9890";
    case 17:
        // Multithreading
        return "#FF7043";
    case 22:
        // OpenGL and 3D
        return "#E53935";
    case 11:
        // Platforms and Integration
        return "#BCAAA4";
    case 9:
        // Qt in Use
        return "#4CAF50";
    case 15:
        // QtQuick
        return "#8BC34A";
    case 20:
        // Technical Discussions
        return "#8D6E63";
    case 8:
        // Testing and Continuous Integration
        return "#E0E0E0";
    case 7:
        // Tooling
        return "#90A4AE";
    case 19:
        // Web
        return "#607D8B";
    default:
        // transparent
        return "transparent";
    }
}

QString DataUtil::contentStandardTypeColor(const int contentStandardTypeId)
{
    switch (contentStandardTypeId) {
    case 0:
        // *****
        return "transparent";
    case 3:
        // 3D
        return "#FFCDD2";
    case 18:
        // Automotive, Mobile and Embedded
        return "#F06292";
    case 21:
        // Best practices
        return "#BA68C8";
    case 14:
        // Beyond Code
        return "#D1C4E9";
    case 2:
        // Break "#FFFFFF"
        return "grey";
    case 23:
        // Codecs
        return "#E8EAF6";
    case 4:
        // isRefView
        return "#B3E5FC";
    case 12:
        // Distributing Software and Resources
        return "#64B5F6";
    case 6:
        // Free Software policies and politics
        return "#4DD0E1";
    case 16:
        // Geolocation
        return "#CDDC39";
    case 13:
        // In depth
        return "#26A69A";
    case 10:
        // KDE‘s Latest and Greatest
        return "#2196F3";
    case 5:
        // Let‘s talk business
        return "#FFEB3B";
    case 24:
        // Let‘s talk software
        return "#FFC107";
    case 1:
        // Misc
        return "#FF9890";
    case 17:
        // Multithreading
        return "#FF7043";
    case 22:
        // OpenGL and 3D
        return "#E53935";
    case 11:
        // Platforms and Integration
        return "#BCAAA4";
    case 9:
        // Qt in Use
        return "#4CAF50";
    case 15:
        // QtQuick
        return "#8BC34A";
    case 20:
        // Technical Discussions
        return "#8D6E63";
    case 8:
        // Testing and Continuous Integration
        return "#E0E0E0";
    case 7:
        // Tooling
        return "#90A4AE";
    case 19:
        // Web
        return "#607D8B";
    default:
        // transparent
        return "transparent";
    }
}


QString DataUtil::tagColorFirstTag(Key *key)
{
    if(!key->keyTags().size()) {
        return "transparent";
    }
    return tagColor(key->keyTagsKeys().first().toInt());
}

QString DataUtil::contentBlockColorFirstContentBlock(Key *key)
{
    if(!key->keyContentBlocks().size()) {
        return "transparent";
    }
    return contentBlockColor(key->keyContentBlocksKeys().first().toInt());
}

QString DataUtil::contentStandardTypeColorFirstContentStandardType(Key *key)
{
    if(!key->keyContentStandardTypes().size()) {
        return "transparent";
    }
    return contentStandardTypeColor(key->keyContentStandardTypesKeys().first().toInt());
}



// if update failed Data in memory is inconsistent
// delete all, then do init again
void DataUtil::reloadData() {
    // delete all to avoid orphans
    mProgressInfotext.append(tr(" Rollback"));
    emit progressInfo(mProgressInfotext);
    // DO IT
    // reload
    mProgressInfotext.append(tr(" Reload"));
    emit progressInfo(mProgressInfotext);
    // DO IT
    mDataManager->init();
}

// creates missing dirs if preparing repository (pre-conf-stuff)
// or checking for dictionary updates
bool DataUtil::checkDirs()
{
    // data/repository
    QString directory = mDataManager->mDataPath + "repository/";
    QDir myDir;
    bool exists;
    exists = myDir.exists(directory);
    if (!exists) {
        bool ok = myDir.mkpath(directory);
        if(!ok) {
            qWarning() << "Couldn't create repository dir " << directory;
            return false;
        }
        qDebug() << "created directory repository " << directory;
    }
    // data/repository/creatorImages
    directory.append("creatorImages/");
    exists = myDir.exists(directory);
    if (!exists) {
        bool ok = myDir.mkpath(directory);
        if(!ok) {
            qWarning() << "Couldn't create creatorImages dir " << directory;
            return false;
        }
        qDebug() << "created directory creatorImages " << directory;
    }
    return true;
}

// P R E   R E P O S I T O R Y
/**
 * Prepare will be done before submitting the App to AppStore
 * Use public Cache
 * Best practice:
 * 1. remove app from dev device
 * 2. start app, switch to public cache
 * 3. restart app
 * If all done send files from Phone to PC/Mac
 * Copy them into data-assets /prod or /test
 * Test again after deleting app from dev device
 * Now Users will get this directly by installing the app
 * and updates are fast
 */
void DataUtil::prepareRepository() {
    // must use public cache
    //    if(!mDataManager->settingsData()->hasPublicCache()) {
    //        qWarning() << "Preparation must be done from public cache";
    //        return;
    //    }
    qDebug() << "PREPARE REPOSITORY ";
    // check dirs for pre-repository stuff
    bool dirsOk = checkDirs();
    if(!dirsOk) {
        qWarning() << "cannot create directories";
        return;
    }
    // create some data for this specific repository
    prepareEventData();
    // prepare creator
    prepareCreator();
    // prepare keys
    // creator must exist because keys will be added to
    prepareKeys();
    // download creator images
    prepareCreatorImages();
    // now cache all the data
    // if ok - copied to qrc: data-assets
    qDebug() << "cache DATA";
    mDataManager->saveRepositoryToCache();
    mDataManager->saveDayToCache();
    mDataManager->saveDictionaryToCache();
    mDataManager->saveCategoryToCache();
    mDataManager->saveSectionToCache();
    mDataManager->saveKeyToCache();

    // sort Tags
    QMultiMap<QString, KeyTag*> keyTagSortMap;
    for (int i = 0; i < mDataManager->allKeyTag().size(); ++i) {
        KeyTag* keyTag = (KeyTag*) mDataManager->allKeyTag().at(i);
        keyTagSortMap.insert(keyTag->name(), keyTag);
    }
    mDataManager->mAllKeyTag.clear();
    QMapIterator<QString, KeyTag*> keyTagIterator(keyTagSortMap);
    while (keyTagIterator.hasNext()) {
        keyTagIterator.next();
        KeyTag* keyTag = keyTagIterator.value();
        mDataManager->insertKeyTag(keyTag);
    }
    mDataManager->saveKeyTagToCache();

    // sort ContentBlocks
    QMultiMap<QString, KeyContentBlock*> keyContentBlockSortMap;
    for (int i = 0; i < mDataManager->allKeyContentBlock().size(); ++i) {
        KeyContentBlock* keyContentBlock = (KeyContentBlock*) mDataManager->allKeyContentBlock().at(i);
        keyContentBlockSortMap.insert(keyContentBlock->name(), keyContentBlock);
    }
    mDataManager->mAllKeyContentBlock.clear();
    QMapIterator<QString, KeyContentBlock*> keyContentBlockIterator(keyContentBlockSortMap);
    while (keyContentBlockIterator.hasNext()) {
        keyContentBlockIterator.next();
        KeyContentBlock* keyContentBlock = keyContentBlockIterator.value();
        mDataManager->insertKeyContentBlock(keyContentBlock);
    }
    mDataManager->saveKeyContentBlockToCache();

    // sort ContentStandardTypes
    QMultiMap<QString, KeyContentStandardType*> keyContentStandardTypeSortMap;
    for (int i = 0; i < mDataManager->allKeyContentStandardType().size(); ++i) {
        KeyContentStandardType* keyContentStandardType = (KeyContentStandardType*) mDataManager->allKeyContentStandardType().at(i);
        keyContentStandardTypeSortMap.insert(keyContentStandardType->name(), keyContentStandardType);
    }
    mDataManager->mAllKeyContentStandardType.clear();
    QMapIterator<QString, KeyContentStandardType*> keyContentStandardTypeIterator(keyContentStandardTypeSortMap);
    while (keyContentStandardTypeIterator.hasNext()) {
        keyContentStandardTypeIterator.next();
        KeyContentStandardType* keyContentStandardType = keyContentStandardTypeIterator.value();
        mDataManager->insertKeyContentStandardType(keyContentStandardType);
    }
    mDataManager->saveKeyContentStandardTypeToCache();

    mDataManager->saveDictionaryItemToCache();
    mDataManager->saveKeyLinkToCache();

    mDataManager->saveCreatorToCache();
    // set API Version
    mDataManager->mSettingsData->setApiVersion(mNewApi);
    qDebug() << "PREPARE   D O N E - WAIT FOR IMAGES";
}

/**
 * repository, days, dictionary-category-sections
 **/
void DataUtil::prepareEventData() {
    qDebug() << "PREPARE EVENT ";
    // REPOSITORY
    mDataManager->deleteRepository();
    mDataManager->deleteDay();
    mDataManager->deleteKey();
    mDataManager->deleteDictionaryItem();
    mDataManager->deleteKeyLink();
    mDataManager->deleteCreator();
    mDataManager->deleteCreatorImage();
    Repository* repository = mDataManager->createRepository();

    int lastNr = 0;
    for (int i = 0; i < mDataManager->allDictionary().size(); ++i) {
        Dictionary* dictionary = (Dictionary*) mDataManager->allDictionary().at(i);
        if(dictionary->dictionaryId() > lastNr) {
            lastNr = dictionary->dictionaryId();
        }
    }
    repository->setLastDictionaryId(lastNr);

    lastNr = 0;
    for (int i = 0; i < mDataManager->allCategory().size(); ++i) {
        Category* category = (Category*) mDataManager->allCategory().at(i);
        if(category->categoryId() > lastNr) {
            lastNr = category->categoryId();
        }
    }
    repository->setLastCategoryId(lastNr);

    lastNr = 0;
    for (int i = 0; i < mDataManager->allSection().size(); ++i) {
        Section* section = (Section*) mDataManager->allSection().at(i);
        if(section->sectionId() > lastNr) {
            lastNr = section->sectionId();
        }
        section->clearKeys();
    }
    repository->setLastSectionId(lastNr);

    lastNr = 0;
    for (int i = 0; i < mDataManager->allKeyTag().size(); ++i) {
        KeyTag* tag = (KeyTag*) mDataManager->allKeyTag().at(i);
        if(tag->tagId() > lastNr) {
            lastNr = tag->tagId();
        }
        tag->clearKeys();
    }
    repository->setLastKeyTagId(lastNr);

    lastNr = 0;
    for (int i = 0; i < mDataManager->allKeyContentBlock().size(); ++i) {
        KeyContentBlock* contentBlock = (KeyContentBlock*) mDataManager->allKeyContentBlock().at(i);
        if(contentBlock->contentBlockId() > lastNr) {
            lastNr = contentBlock->contentBlockId();
        }
        contentBlock->clearKeys();
    }
    repository->setLastKeyContentBlockId(lastNr);

    lastNr = 0;
    for (int i = 0; i < mDataManager->allKeyContentStandardType().size(); ++i) {
        KeyContentStandardType* contentStandardType = (KeyContentStandardType*) mDataManager->allKeyContentStandardType().at(i);
        if(contentStandardType->contentStandardTypeId() > lastNr) {
            lastNr = contentStandardType->contentStandardTypeId();
        }
        contentStandardType->clearKeys();
    }
    repository->setLastKeyContentStandardTypeId(lastNr);

    repository->setRepositoryName("DiCoTAE");
    repository->setRepositoryCity("Madrid, ES");
    QString venueAddress;
    venueAddress = "Spain, ES XXXXX";
    venueAddress.append("\n");
    venueAddress.append("Spain");
    repository->setAddress(venueAddress);
    repository->setTimeZoneName("PST – Pacific Standard Time");
    repository->setTimeZoneOffsetSeconds(-8 * 60 * 60); // -08:00 GMT
    repository->setRepositoryFrom(QDate::fromString("2018-01-26", YYYY_MM_DD));
    repository->setRepositoryTo(QDate::fromString("2040-10-20", YYYY_MM_DD));
    repository->setHashTag("#DiCoTAE");
    repository->setHashContentBlock("#DiCoTAE");
    repository->setHashContentStandardType("#DiCoTAE");
    repository->setHomePage("http://www.solanogadea.es/");
    QString coordinate;
    coordinate = QString::number(37.799263)+";"+QString::number(-122.397673);
    repository->setCoordinate(coordinate);
    mDataManager->insertRepository(repository);
    // DAYS
    // Days dayOfWeek 1=monday, 7 = sunday
    // wednesday
    Day* day = mDataManager->createDay();
    day->setId(1);
    day->setWeekDay(3);
    day->setRepositoryDay(QDate::fromString("2018-01-26", YYYY_MM_DD));
    repository->addToDays(day);
    mDataManager->insertDay(day);
    // thursday
    day = mDataManager->createDay();
    day->setId(2);
    day->setWeekDay(4);
    day->setRepositoryDay(QDate::fromString("2040-10-20", YYYY_MM_DD));
    repository->addToDays(day);
    mDataManager->insertDay(day);
    // DICTIONARY
    // Dictionary and CATEGORIES already created
    // SECTIONS created, but delete current keys yet
    qDebug() << "SECTIONS: #" << mDataManager->allSection().size();
    for (int r = 0; r < mDataManager->allSection().size(); ++r) {
        Section* section = (Section*) mDataManager->allSection().at(r);
        section->clearKeys();
    }
    //
}

QVariantMap DataUtil::readDictionaryFile(const QString dictionaryPath) {
    QVariantMap map;
    QFile readFile(dictionaryPath);
    if(!readFile.exists()) {
        qWarning() << "Dictionary Path not found " << dictionaryPath;
        return map;
    }
    if (!readFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file: " << dictionaryPath;
        return map;
    }
    QJsonDocument jda = QJsonDocument::fromJson(readFile.readAll());

    readFile.close();
    if(!jda.isObject()) {
        qWarning() << "Couldn't create JSON from file: " << dictionaryPath;
        return map;
    }
    qDebug() << "QJsonDocument for dictionary with Object :)";
    map = jda.toVariant().toMap();
    return map;
}

Day* DataUtil::findDayForServerDate(const QString& dayDate) {
    Day* day = nullptr;
    bool found = false;
    for (int dl = 0; dl < mDataManager->mAllDay.size(); ++dl) {
        day = (Day*) mDataManager->mAllDay.at(dl);
        if(day->repositoryDay().toString(YYYY_MM_DD) == dayDate) {
            found = true;
            break;
        }
    }
    if(found) {
        qDebug() << "Day found";
        return day;
    }
    qDebug() << "Day not found";
    return 0;
}

void DataUtil::adjustTags(QVariantMap& keyMap, Repository* repository, const bool isUpdate) {
    QStringList tagKeys;
    QStringList tagNames;
    tagNames = keyMap.value("tags").toStringList();
    for (int tnl = 0; tnl < tagNames.size(); ++tnl) {
        QString tagName;
        tagName = tagNames.at(tnl);
        bool found = false;
        for (int i = 0; i < mDataManager->allKeyTag().size(); ++i) {
            KeyTag* keyTag = (KeyTag*) mDataManager->allKeyTag().at(i);
            if(keyTag->name() == tagName) {
                found = true;
                tagKeys.append(QString::number(keyTag->tagId()));
                break;
            }
        } // for all tags
        if(!found) {
            KeyTag* keyTag = mDataManager->createKeyTag();
            repository->setLastKeyTagId(repository->lastKeyTagId()+1);
            keyTag->setTagId(repository->lastKeyTagId());
            keyTag->setName(tagName);
            keyTag->setInAssets(isUpdate?false:true);
            mDataManager->insertKeyTag(keyTag);
            tagKeys.append(QString::number(keyTag->tagId()));
        }
    }
    keyMap.insert("tags", tagKeys);
    qDebug() << "TAG KEYS: " << tagKeys;
}

void DataUtil::adjustContentBlocks(QVariantMap& keyMap, Repository* repository, const bool isUpdate) {
    QStringList contentBlockKeys;
    QStringList contentBlockNames;
    contentBlockNames = keyMap.value("contentBlocks").toStringList();
    for (int tnl = 0; tnl < contentBlockNames.size(); ++tnl) {
        QString contentBlockName;
        contentBlockName = contentBlockNames.at(tnl);
        bool found = false;
        for (int i = 0; i < mDataManager->allKeyContentBlock().size(); ++i) {
            KeyContentBlock* keyContentBlock = (KeyContentBlock*) mDataManager->allKeyContentBlock().at(i);
            if(keyContentBlock->name() == contentBlockName) {
                found = true;
                contentBlockKeys.append(QString::number(keyContentBlock->contentBlockId()));
                break;
            }
        } // for all contentBlocks
        if(!found) {
            KeyContentBlock* keyContentBlock = mDataManager->createKeyContentBlock();
            repository->setLastKeyContentBlockId(repository->lastKeyContentBlockId()+1);
            keyContentBlock->setContentBlockId(repository->lastKeyContentBlockId());
            keyContentBlock->setName(contentBlockName);
            keyContentBlock->setInAssets(isUpdate?false:true);
            mDataManager->insertKeyContentBlock(keyContentBlock);
            contentBlockKeys.append(QString::number(keyContentBlock->contentBlockId()));
        }
    }
    keyMap.insert("contentBlocks", contentBlockKeys);
    qDebug() << "CONTENTBLOCK KEYS: " << contentBlockKeys;
}

void DataUtil::adjustContentStandardTypes(QVariantMap& keyMap, Repository* repository, const bool isUpdate) {
    QStringList contentStandardTypeKeys;
    QStringList contentStandardTypeNames;
    contentStandardTypeNames = keyMap.value("contentStandardTypes").toStringList();
    for (int tnl = 0; tnl < contentStandardTypeNames.size(); ++tnl) {
        QString contentStandardTypeName;
        contentStandardTypeName = contentStandardTypeNames.at(tnl);
        bool found = false;
        for (int i = 0; i < mDataManager->allKeyContentStandardType().size(); ++i) {
            KeyContentStandardType* keyContentStandardType = (KeyContentStandardType*) mDataManager->allKeyContentStandardType().at(i);
            if(keyContentStandardType->name() == contentStandardTypeName) {
                found = true;
                contentStandardTypeKeys.append(QString::number(keyContentStandardType->contentStandardTypeId()));
                break;
            }
        } // for all contentStandardTypes
        if(!found) {
            KeyContentStandardType* keyContentStandardType = mDataManager->createKeyContentStandardType();
            repository->setLastKeyContentStandardTypeId(repository->lastKeyContentStandardTypeId()+1);
            keyContentStandardType->setContentStandardTypeId(repository->lastKeyContentStandardTypeId());
            keyContentStandardType->setName(contentStandardTypeName);
            keyContentStandardType->setInAssets(isUpdate?false:true);
            mDataManager->insertKeyContentStandardType(keyContentStandardType);
            contentStandardTypeKeys.append(QString::number(keyContentStandardType->contentStandardTypeId()));
        }
    }
    keyMap.insert("contentStandardTypes", contentStandardTypeKeys);
    qDebug() << "CONTENTSTANDARDTYPE KEYS: " << contentStandardTypeKeys;
}

void DataUtil::adjustPersons(QVariantMap& keyMap) {
    QStringList personKeys;
    QVariantList creatorsList;
    creatorsList = keyMap.value("creators").toList();
    if (creatorsList.size() > 0) {
        for (int pvl = 0; pvl < creatorsList.size(); ++pvl) {
            QVariantMap map = creatorsList.at(pvl).toMap();
            if(map.contains("id")) {
                personKeys.append(map.value("id").toString());
            }
        }
        keyMap.insert("creators", personKeys);
    }
}

void DataUtil::createAndAdjustLinks(QVariantMap& keyMap) {
    QStringList linkKeys;
    QVariantList linksList;
    linksList = keyMap.value("links").toList();
    if (linksList.size() > 0) {
        for (int lvl = 0; lvl < linksList.size(); ++lvl) {
            QVariantMap map = linksList.at(lvl).toMap();
            if(map.contains("url")) {
                KeyLink* keyLink = mDataManager->createKeyLink();
                keyLink->setUrl(map.value("url").toString());
                keyLink->setTitle(map.value("title").toString());
                mDataManager->insertKeyLink(keyLink);
                linkKeys.append(keyLink->uuid());
            }
        }
        keyMap.insert("links", linkKeys);
    }
}

bool DataUtil::checkIfIgnored(KeyAPI* keyAPI) {
    return false;
    if(keyAPI->title() == "Registration and Coffee" && keyAPI->section() != "B02") {
        qDebug() << "unwanted key: " << keyAPI->keyId() << " " << keyAPI->title() << " " << keyAPI->section();
        return true;
    }
    if(keyAPI->title() == "Lunch" && keyAPI->section() != "B02") {
        qDebug() << "unwanted key: " << keyAPI->keyId() << " " << keyAPI->title() << " " << keyAPI->section();
        return true;
    }
    if(keyAPI->title() == "Coffee break" && keyAPI->section() != "B02") {
        qDebug() << "unwanted key: " << keyAPI->keyId() << " " << keyAPI->title() << " " << keyAPI->section();
        return true;
    }
    if(keyAPI->title() == "Evening event" && keyAPI->section() != "B02") {
        qDebug() << "unwanted key: " << keyAPI->keyId() << " " << keyAPI->title() << " " << keyAPI->section();
        return true;
    }
    if(keyAPI->title() == "Welcome" && keyAPI->section() != "C01") {
        qDebug() << "unwanted key: " << keyAPI->keyId() << " " << keyAPI->title() << " " << keyAPI->section();
        return true;
    }
    return false;
}

void DataUtil::setDuration(KeyAPI* keyAPI, Key* key) {
    QStringList start_time;
    start_time = keyAPI->start_time().split(":");
    int minutes = 0;
    if(start_time.length() == 2) {
        minutes = start_time.last().toInt();
        minutes += start_time.first().toInt()*60;
    } else {
        qWarning() << "Duration wrong: " << keyAPI->start_time() << " key ID: " << keyAPI->keyId();
    }
    key->setMinutes(minutes);
    key->setEndTime(key->startTime().addSecs(minutes * 60));
}

void DataUtil::setType(Key* key) {
    // TODO: Fijar tipos de entradas en el diccionario (de momento solo dos T y V)
    if (key->keyType() == "V") {
        key->setIsRefView(true);
    }
    // for DiCoTAE here we create DictionaryItems which are not part of DiCoTAE data
}

void DataUtil::prepareKeys()
{
    const QString dictionaryPath = mDataManager->mDataAssetsPath + "repository/dictionary.json";
    qDebug() << "PREPARE KEYS ";
    QVariantMap map;
    map = readDictionaryFile(dictionaryPath);

    if(map.isEmpty()) {
        qWarning() << "Dictionary is no Map";
        return;
    }

    mNewApi = map.value("version").toString();
    qDebug() << "VERSION: " + mNewApi;

    map = map.value("repository").toMap();
    if(map.isEmpty()) {
        qWarning() << "No 'repository' found";
        return;
    }

    qDebug() << "TITLE: " << map.value("title").toString();
    Repository* repository;
    repository = (Repository*) mDataManager->allRepository().first();

    QVariantList levelList;
    // DiCoTAE gives us a list of updates
    // DiCoTAE a days object, so we have to construct the list
    QVariantMap allLevelsMap;
    allLevelsMap = map.value("updates").toMap();
    levelList.append(allLevelsMap.value("2018-01-26").toMap());
    levelList.append(allLevelsMap.value("2018-01-27").toMap());

    if(levelList.isEmpty()) {
        qWarning() << "No 'days' found";
        return;
    }
    mMultiKey.clear();
    qDebug() << "DAYS: " << levelList.size();
    for (int i = 0; i < levelList.size(); ++i) {
        QVariantMap dayMap;
        dayMap = levelList.at(i).toMap();
        if(dayMap.isEmpty()) {
            qWarning() << "No 'DAY' found #" << i;
            continue;
        }
        QString dayDate;
        dayDate = dayMap.value("date").toString();
        qDebug() << "processing DATE: " << dayDate;

        Day* day = findDayForServerDate(dayDate);
        if(!day) {
            qWarning() << "No Day* found for " << dayDate;
            continue;
        }

        bool found = false;
        QVariantMap sectionMap;
        sectionMap = dayMap.value("sections").toMap();
        QStringList sectionKeys = sectionMap.keys();
        if(sectionKeys.isEmpty()) {
            qWarning() << "No 'SECTIONS' found for DAY # i";
            continue;
        }
        for (int r = 0; r < sectionKeys.size(); ++r) {
            QVariantList keyList;
            keyList = sectionMap.value(sectionKeys.at(r)).toList();
            if(keyList.isEmpty()) {
                qWarning() << "DAY: " << dayDate << " SECTION: " << sectionKeys.at(r) << " ignored - No Keys available";
                continue;
            }
            Section* section = nullptr;
            found = false;
            for (int rl = 0; rl < mDataManager->mAllSection.size(); ++rl) {
                section = (Section*) mDataManager->mAllSection.at(rl);
                if(section->sectionName() == sectionKeys.at(r)) {
                    found = true;
                    break;
                }
            }
            if(!found) {
                qWarning() << "Section* not found for " << dayDate << " Section: " << sectionKeys.at(r);
                if(sectionKeys.at(r).isEmpty()) {
                    // use dummi section
                    section = (Section*) mDataManager->mAllSection.first();
                    qDebug() << "Section Name empty - using Section " << section->sectionName();
                } else {
                    section = mDataManager->createSection();
                    repository->setLastSectionId(repository->lastSectionId()+1);
                    section->setSectionId(repository->lastSectionId());
                    section->setInAssets(true);
                    section->setSectionName(sectionKeys.at(r));
                    mDataManager->insertSection(section);
                }
            }
            for (int sl = 0; sl < keyList.size(); ++sl) {
                QVariantMap keyMap;
                keyMap = keyList.at(sl).toMap();
                if(keyMap.isEmpty()) {
                    qWarning() << "No 'KEY' Map DAY: " << dayDate << " SECTION: " << sectionKeys.at(r);
                    continue;
                }

                // adjust creators
                adjustPersons(keyMap);
                // adjust tags
                adjustTags(keyMap, repository, false);
                // adjust contentBlocks
                adjustContentBlocks(keyMap, repository, false);
                // adjust contentStandardTypes
                adjustContentStandardTypes(keyMap, repository, false);

                // create and adjust links
                createAndAdjustLinks(keyMap);

                KeyAPI* keyAPI = mDataManager->createKeyAPI();
                keyAPI->fillFromForeignMap(keyMap);
                // ignore unwanted Keys
                if (checkIfIgnored(keyAPI)) {
                    continue;
                }

                Key* key = mDataManager->createKey();
                key->fillFromMap(keyAPI->toMap());
                setDuration(keyAPI, key);
                // refs
                // DAY
                key->setKeyDay(day->id());
                // SECTION
                key->setSection(section->sectionId());
                // TYPE DICTIONARY
                setType(key);
                // SORT
                key->setSortKey(section->sectionName()); //day->repositoryDay().toString(YYYY_MM_DD)+key->startTime().toString(HH_MM)
                mMultiKey.insert(key->sortKey(), key);
            } // end for key of a section
        } // section keys
    } // for days list
    // insert sorted Keys
    sortedKeysIntoSectionDayTagContentBlockContentStandardTypeCreator();
}

// keys must be cleared before for Section, Day, Tag, ContentBlock, ContentStandardType, Creator
// always if prepare Repository, explicitely if Update Dictionary
void DataUtil::sortedKeysIntoSectionDayTagContentBlockContentStandardTypeCreator() {
    QMapIterator<QString, Key*> keyIterator(mMultiKey);
    while (keyIterator.hasNext()) {
        keyIterator.next();
        Key* key = keyIterator.value();
        mDataManager->insertKey(key);

        // Sections
        Section* section = mDataManager->findSectionBySectionId(key->section());
        if(section != NULL) {
            section->addToKeys(key);
        } else {
            qWarning() << "SECTION is NULL for Key " << key->keyId() << " #:" << key->section();
        }

        // Days
        Day* day = mDataManager->findDayById(key->keyDay());
        if(day != NULL) {
            day->addToKeys(key);
        } else {
            qWarning() << "DAY is NULL for Key " << key->keyId() << " #:" << key->keyDay();
        }

        // Tags
        for (int i = 0; i < key->keyTagsKeys().size(); ++i) {
            int tKey = key->keyTagsKeys().at(i).toInt();
            KeyTag* keyTag = (KeyTag*) mDataManager->findKeyTagByTagId(tKey);
            if(keyTag != NULL) {
                keyTag->addToKeys(key);
            } else {
                qWarning() << "TAG is NULL for Key " << key->keyId() << " #:" << tKey;
            }
        }

        // ContentBlocks
        for (int i = 0; i < key->keyContentBlocksKeys().size(); ++i) {
            int tKey = key->keyContentBlocksKeys().at(i).toInt();
            KeyContentBlock* keyContentBlock = (KeyContentBlock*) mDataManager->findKeyContentBlockByContentBlockId(tKey);
            if(keyContentBlock != NULL) {
                keyContentBlock->addToKeys(key);
            } else {
                qWarning() << "CONTENTBLOCK is NULL for Key " << key->keyId() << " #:" << tKey;
            }
        }

        // ContentStandardTypes
        for (int i = 0; i < key->keyContentStandardTypesKeys().size(); ++i) {
            int tKey = key->keyContentStandardTypesKeys().at(i).toInt();
            KeyContentStandardType* keyContentStandardType = (KeyContentStandardType*) mDataManager->findKeyContentStandardTypeByContentStandardTypeId(tKey);
            if(keyContentStandardType != NULL) {
                keyContentStandardType->addToKeys(key);
            } else {
                qWarning() << "CONTENTSTANDARDTYPE is NULL for Key " << key->keyId() << " #:" << tKey;
            }
        }

        // Creators
        for (int i = 0; i < key->creatorKeys().size(); ++i) {
            int pKey = key->creatorKeys().at(i).toInt();
            Creator* creator = (Creator*) mDataManager->findCreatorByCreatorId(pKey);
            if(creator != NULL) {
                creator->addToKeys(key);
            } else {
                qWarning() << "CREATOR is NULL for Key " << key->keyId() << " #:" << pKey;
            }
        }
    } // while all keys
}

QVariantList DataUtil::readCreatorFile(const QString creatorPath) {
    QVariantList dataList;
    QFile readFile(creatorPath);
    if(!readFile.exists()) {
        qWarning() << "Creator Path not found " << creatorPath;
        return dataList;
    }
    if (!readFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file: " << creatorPath;
        return dataList;
    }
    QJsonDocument jda = QJsonDocument::fromJson(readFile.readAll());

    readFile.close();
    if(!jda.isArray()) {
        qWarning() << "Couldn't create JSON from file: " << creatorPath;
        return dataList;
    }
    qDebug() << "QJsonDocument for creator with Array :)";
    dataList = jda.toVariant().toList();
    return dataList;
}

void DataUtil::calcCreatorName(Creator* creator, CreatorAPI* creatorAPI) {
    creator->setName(creatorAPI->firstName());
    if(creator->name().length() > 0) {
        creator->setName(creator->name()+" ");
    }
    creator->setName(creator->name()+creatorAPI->lastName());
    if(creator->name().length() > 0) {
        if(creatorAPI->lastName().length() > 0) {
            creator->setSortKey(creatorAPI->lastName().left(5).toUpper());
        } else {
            creator->setSortKey(creatorAPI->firstName().left(5).toUpper());
        }
        creator->setSortGroup(creator->sortKey().left(1));
    } else {
        creator->setSortKey("*");
        creator->setSortGroup("*");
    }
}

void DataUtil::prepareCreator()
{
    const QString creatorsPath = mDataManager->mDataAssetsPath + "repository/creator.json";
    qDebug() << "PREPARE CREATOR ";
    QVariantList dataList;
    dataList = readCreatorFile(creatorsPath);
    if(dataList.size() == 0) {
        qWarning() << "Creator List empty";
        return;
    }

    mMultiCreator.clear();
    for (int i = 0; i < dataList.size(); ++i) {
        CreatorAPI* creatorAPI = mDataManager->createCreatorAPI();
        creatorAPI->fillFromForeignMap(dataList.at(i).toMap());

        Creator* creator = mDataManager->createCreator();
        creator->setCreatorId(creatorAPI->id());

        calcCreatorName(creator, creatorAPI);
        creator->setBio(creatorAPI->bio());

        if(creatorAPI->avatar().length() > 0 && creatorAPI->avatar() != DEFAULT_CREATOR_IMAGE_URL) {
            QString avatar = creatorAPI->avatar();
            QStringList sl = avatar.split("?");
            if(sl.size() > 1) {
                sl.removeLast();
                avatar = sl.join("?");
            }
            sl = avatar.split(".");
            if(sl.size() < 2) {
                qWarning() << "AVATAR wrong "+creatorAPI->avatar();
            } else {
                // only for DiCoTAE16 avatar = avatar.replace("http://","https://");
                CreatorImage* creatorImage = mDataManager->createCreatorImage();
                creatorImage->setCreatorId(creator->creatorId());
                creatorImage->setOriginImageUrl(avatar);
                creatorImage->setSuffix(sl.last());
                mDataManager->insertCreatorImage(creatorImage);
                creator->resolveCreatorImageAsDataObject(creatorImage);
            }
        }
        // using MultiMap to get Creators sorted
        mMultiCreator.insert(creator->sortKey(), creator);
    } // end for all CreatorsAPI
    // insert sorted Creators
    mDataManager->mAllCreator.clear();
    QMapIterator<QString, Creator*> creatorIterator(mMultiCreator);
    while (creatorIterator.hasNext()) {
        creatorIterator.next();
        mDataManager->insertCreator(creatorIterator.value());
    }
}

void DataUtil::prepareCreatorImages()
{
    const QString creatorImagesPath = mRepositoryDataPath + "creatorImages/";
    for (int i = 0; i < mDataManager->allCreatorImage().size(); ++i) {
        CreatorImage* creatorImage = (CreatorImage*) mDataManager->allCreatorImage().at(i);
        if (!creatorImage->downloadSuccess() && !creatorImage->downloadFailed()) {
            qDebug() << "loading..." << creatorImage->creatorId();
            QString fileName;
            fileName = "creator_";
            fileName.append(QString::number(creatorImage->creatorId()));
            fileName.append('.');
            fileName.append(creatorImage->suffix());
            mImageLoader = new ImageLoader(creatorImage->originImageUrl(), creatorImagesPath+fileName, this);
            bool res = connect(mImageLoader, SIGNAL(loaded(QObject*, int, int)), this,
                               SLOT(onCreatorImageLoaded(QObject*, int, int)));
            if (!res) {
                Q_ASSERT(res);
            }
            res = connect(mImageLoader, SIGNAL(loadingFailed(QObject*, QString)), this,
                          SLOT(onCreatorImageFailed(QObject*, QString)));
            if (!res) {
                Q_ASSERT(res);
            }
            mImageLoader->loadCreator(creatorImage);
            return;
        }
    } // for all creator images
    // N OW cache creator images
    mDataManager->saveCreatorImageToCache();
    qDebug() << "CREATOR IMAGES   D O W N L O A D E D";
}

void DataUtil::checkVersion()
{
    mDataServer->requestVersion();
}

void DataUtil::startUpdate()
{
    bool dirOk = checkDirs();
    if(!dirOk) {
        qWarning() << "Cannot create Directories";
        emit updateFailed(tr("startUpdate - Cannot create Directories"));
        return;
    }
    mProgressInfotext = tr("Request Dictionary and Creators from Server");
    emit progressInfo(mProgressInfotext);
    mDataServer->requestDictionary();
}

void DataUtil::continueUpdate()
{
    mProgressInfotext = tr("Save Favorites");
    emit progressInfo(mProgressInfotext);
    // save F A V O R I T E S and bookmarks
    saveKeyFavorites();
    // C R E A T O R
    mProgressInfotext = tr("Sync Creator");
    emit progressInfo(mProgressInfotext);
    const QString creatorsPath = mRepositoryDataPath + "creator.json";
    qDebug() << "PREPARE CREATOR ";
    QVariantList dataList;
    dataList = readCreatorFile(creatorsPath);
    if(dataList.size() == 0) {
        qWarning() << "Creator List empty";
        emit updateFailed(tr("Update failed. No Creator received.\nReloading current Data"));
        return;
    }
    mMultiKey.clear();
    mMultiCreator.clear();
    mMultiCreatorImages.clear();
    for (int i = 0; i < dataList.size(); ++i) {
        CreatorAPI* creatorAPI = mDataManager->createCreatorAPI();
        creatorAPI->fillFromForeignMap(dataList.at(i).toMap());

        Creator* creator = mDataManager->findCreatorByCreatorId(creatorAPI->id());
        if(!creator) {
            // NEW creator
            qDebug() << "NEW CREATOR";
            mProgressInfotext.append("+");
            creator = mDataManager->createCreator();
            creator->setCreatorId(creatorAPI->id());
        } else {
            // update Creator
            mProgressInfotext.append(".");
        }
        emit progressInfo(mProgressInfotext);
        calcCreatorName(creator, creatorAPI);
        creator->setBio(creatorAPI->bio());
        if(creatorAPI->avatar().length() > 0 && creatorAPI->avatar() != DEFAULT_CREATOR_IMAGE_URL) {
            QString avatar = creatorAPI->avatar();
            QStringList sl = avatar.split("?");
            if(sl.size() > 1) {
                sl.removeLast();
                avatar = sl.join("?");
            }
            sl = avatar.split(".");
            if(sl.size() < 2) {
                qWarning() << "AVATAR wrong "+creatorAPI->avatar();
            } else {
                // check if modified
                // only DiCoTAE16 avatar = avatar.replace("http://","https://");
                if(creator->hasCreatorImage()) {
                    if(creator->creatorImageAsDataObject()->originImageUrl() != avatar) {
                        qDebug() << "IMAGE Changed";
                        CreatorImage* creatorImage = creator->creatorImageAsDataObject();
                        creatorImage->setOriginImageUrl(avatar);
                        creatorImage->setSuffix(sl.last());
                        mMultiCreatorImages.insert(false, creatorImage);
                    }
                } else {
                    qDebug() << "IMAGE NEW";
                    CreatorImage* creatorImage = mDataManager->createCreatorImage();
                    creatorImage->setCreatorId(creator->creatorId());
                    creatorImage->setOriginImageUrl(avatar);
                    creatorImage->setSuffix(sl.last());
                    creatorImage->setInAssets(false);
                    creator->resolveCreatorImageAsDataObject(creatorImage);
                    mMultiCreatorImages.insert(false, creatorImage);
                }
            } // end if valid Avatar URL
        } // end check avatar if URL && not default
        // using MultiMap to get Creators sorted
        mMultiCreator.insert(creator->sortKey(), creator);
    } // for creator from server
    //
    qDebug() << "CREATORS: " << mDataManager->mAllCreator.size() << " --> " << mMultiCreator.size() << " IMG: " << mMultiCreatorImages.size();
    mProgressInfotext.append("\n").append(tr("Sync Creator Images"));
    emit progressInfo(mProgressInfotext);
    updateCreatorImages();
}

void DataUtil::updateCreatorImages() {
    if(mMultiCreatorImages.size() > 0) {
        QList<CreatorImage*> waitingForDownload = mMultiCreatorImages.values(false);
        if(waitingForDownload.size() > 0) {
            mProgressInfotext.append(".");
            emit progressInfo(mProgressInfotext);
            // DO IT
            const QString creatorImagesPath = mRepositoryDataPath + "creatorImages/";
            CreatorImage* creatorImage = waitingForDownload.first();
            QString fileName;
            fileName = "creator_";
            fileName.append(QString::number(creatorImage->creatorId()));
            fileName.append('.');
            fileName.append(creatorImage->suffix());
            mImageLoader = new ImageLoader(creatorImage->originImageUrl(), creatorImagesPath+fileName, this);
            bool res = connect(mImageLoader, SIGNAL(loaded(QObject*, int, int)), this,
                               SLOT(onCreatorImageUpdateLoaded(QObject*, int, int)));
            if (!res) {
                Q_ASSERT(res);
            }
            res = connect(mImageLoader, SIGNAL(loadingFailed(QObject*, QString)), this,
                          SLOT(onCreatorImageUpdateFailed(QObject*, QString)));
            if (!res) {
                Q_ASSERT(res);
            }
            mImageLoader->loadCreator(creatorImage);
            return;
        } // waiting for download
    } // new images map
    // all done
    updateKeys();
}

void DataUtil::updateAndAdjustLinks(QVariantMap &keyMap) {
    QStringList linkKeys;
    QVariantList linksList;
    linksList = keyMap.value("links").toList();
    if (linksList.size() > 0) {
        for (int lvl = 0; lvl < linksList.size(); ++lvl) {
            QVariantMap map = linksList.at(lvl).toMap();
            if(map.contains("url")) {
                QString linkUrl = map.value("url").toString();
                KeyLink* keyLink = nullptr;
                bool linkFound = false;
                for (int xsl = 0; xsl < mDataManager->allKeyLink().size(); ++xsl) {
                    keyLink = (KeyLink*) mDataManager->allKeyLink().at(xsl);
                    if(keyLink->url() == linkUrl) {
                        linkFound = true;
                        break;
                    }
                } // end for all existing links
                if(!linkFound) {
                    keyLink = mDataManager->createKeyLink();
                }
                keyLink->setUrl(linkUrl);
                keyLink->setTitle(map.value("title").toString());
                if(!linkFound) {
                    mDataManager->insertKeyLink(keyLink);
                }
                linkKeys.append(keyLink->uuid());
            } // map contains url
        } // end for all links
        keyMap.insert("links", linkKeys);
    } // end if linklist size
}

void DataUtil::updateKeys() {
    mProgressInfotext.append("\n").append(tr("Sync Keys"));
    emit progressInfo(mProgressInfotext);
    mMultiKey.clear();
    const QString dictionaryPath = mRepositoryDataPath + "dictionary.json";
    QVariantMap map;
    map = readDictionaryFile(dictionaryPath);
    if(map.isEmpty()) {
        qWarning() << "Dictionary is no Map";
        emit updateFailed(tr("Error: Received Map is empty."));
        return;
    }

    map = map.value("repository").toMap();
    if(map.isEmpty()) {
        qWarning() << "No 'repository' found";
        emit updateFailed(tr("Error: Received Map missed 'repository'."));
        return;
    }
    Repository* repository;
    repository = (Repository*) mDataManager->allRepository().first();

    QVariantList levelList;
    // DiCoTAE gives us a list of updates
    // DiCoTAE a days object, so we have to construct the list
    QVariantMap allLevelsMap;
    allLevelsMap = map.value("updates").toMap();
    levelList.append(allLevelsMap.value("2018-01-26").toMap());
    levelList.append(allLevelsMap.value("2040-10-20").toMap());

    if(levelList.isEmpty()) {
        qWarning() << "No 'days' found";
        emit updateFailed(tr("Error: Received Map missed 'days'."));
        return;
    }
    /*if(levelList.size() != 2) {
        qWarning() << "Wrong number of 'days' found";
        emit updateFailed(tr("Error: # of 'days' expected: 2 - but got ")+QString::number(levelList.size()));
        return;
    }*/
    for (int i = 0; i < levelList.size(); ++i) {
        QVariantMap dayMap;
        dayMap = levelList.at(i).toMap();
        if(dayMap.isEmpty()) {
            qWarning() << "No 'DAY' found #" << i;
            emit updateFailed(tr("Finished"));
            continue;
        }
        QString dayDate;
        dayDate = dayMap.value("date").toString();
        qDebug() << "processing DATE: " << dayDate;
        Day* day = findDayForServerDate(dayDate);
        if(!day) {
            qWarning() << "No Day* found for " << dayDate;
            emit updateFailed(tr("No Day* found for ")+dayDate);
            return;
        }
        bool found = false;
        QVariantMap sectionMap;
        sectionMap = dayMap.value("sections").toMap();
        QStringList sectionKeys = sectionMap.keys();
        if(sectionKeys.isEmpty()) {
            qWarning() << "No 'SECTIONS' found for DAY # i";
            emit updateFailed(tr("No 'SECTIONS' found for DAY ") + dayDate);
            return;
        }
        for (int r = 0; r < sectionKeys.size(); ++r) {
            QVariantList keyList;
            keyList = sectionMap.value(sectionKeys.at(r)).toList();
            if(keyList.isEmpty()) {
                qWarning() << "DAY: " << dayDate << " SECTION: " << sectionKeys.at(r) << " ignored - No Keys available";
                continue;
            }
            Section* section = nullptr;
            found = false;
            for (int rl = 0; rl < mDataManager->mAllSection.size(); ++rl) {
                section = (Section*) mDataManager->mAllSection.at(rl);
                if(section->sectionName() == sectionKeys.at(r)) {
                    found = true;
                    break;
                }
            }
            if(!found) {
                qDebug() << "Section* not found for " << dayDate << " Section: " << sectionKeys.at(r);
                if(sectionKeys.at(r).isEmpty()) {
                    // use dummi section
                    section = (Section*) mDataManager->mAllSection.first();
                    qDebug() << "Section Name empty - using Section " << section->sectionName();
                } else {
                    section = mDataManager->createSection();
                    repository->setLastSectionId(repository->lastSectionId()+1);
                    section->setSectionId(repository->lastSectionId());
                    section->setInAssets(false);
                    section->setSectionName(sectionKeys.at(r));
                    mDataManager->insertSection(section);
                    mProgressInfotext.append("R");
                    progressInfo(mProgressInfotext);
                }
            }
            for (int sl = 0; sl < keyList.size(); ++sl) {
                QVariantMap keyMap;
                keyMap = keyList.at(sl).toMap();
                if(keyMap.isEmpty()) {
                    qWarning() << "No 'KEY' Map DAY: " << dayDate << " SECTION: " << sectionKeys.at(r);
                    continue;
                }

                // adjust creators
                adjustPersons(keyMap);
                // adjust tags
                adjustTags(keyMap, repository, true);
                // adjust contentBlocks
                adjustContentBlocks(keyMap, repository, true);
                // adjust contentStandardTypes
                adjustContentStandardTypes(keyMap, repository, true);

                // update and adjust KeyLink
                updateAndAdjustLinks(keyMap);

                KeyAPI* keyAPI = mDataManager->createKeyAPI();
                keyAPI->fillFromForeignMap(keyMap);
                // ignore unwanted Keys
                if (checkIfIgnored(keyAPI)) {
                    continue;
                }

                Key* key = mDataManager->findKeyByKeyId(keyAPI->keyId());
                if(!key) {
                    // NEW
                    qDebug() << "NEW Key " << QString::number(keyAPI->keyId());
                    mProgressInfotext.append("+");
                    key = mDataManager->createKey();
                    key->setKeyId(keyAPI->keyId());
                } else {
                    // UPDATE
                    qDebug() << "UPDATE Key " << QString::number(keyAPI->keyId());
                    mProgressInfotext.append(".");
                }
                emit progressInfo(mProgressInfotext);
                key->fillFromMap(keyAPI->toMap());
                setDuration(keyAPI, key);
                // refs
                // DAY
                key->setKeyDay(day->id());
                key->resolveKeyDayAsDataObject(day);
                // SECTION
                key->setSection(section->sectionId());
                key->resolveSectionAsDataObject(section);
                // TYPE DICTIONARY
                setType(key);
                // KeyLinks
                // SORT
                //key->setSortKey(day->repositoryDay().toString(YYYY_MM_DD)+key->startTime().toString(HH_MM));
                key->setSortKey(section->sectionName()); //day->repositoryDay().toString(YYYY_MM_DD)+key->startTime().toString(HH_MM)
                mMultiKey.insert(key->sortKey(), key);
            } // end for keys of a section of a level
        } // end for sections of a day
    } // end for list of updates from server

    qDebug() << "KEYS: " << mDataManager->mAllKey.size() << " --> " << mMultiKey.size();
    // creator, images, keys, days, sections, tags --> cache, contentBlocks --> cache, contentStandardTypes --> cache
    // delete orphans
    mProgressInfotext.append("\n").append(tr("Dictionary and Creator successfully synchronized :)"));
    emit progressInfo(mProgressInfotext);
    finishUpdate();
}

void DataUtil::finishUpdate() {
    mProgressInfotext.append("\n").append(tr("Now saving Repository Data to Cache"));
    emit progressInfo(mProgressInfotext);
    // Repository save last xxx Id's
    mDataManager->saveRepositoryToCache();
    qDebug() << "FINISH: Repository saved";

    // Dictionary not changed - always initialized from Prepare Repository

    // Category not changed - always initialized from Prepare Repository

    // Section: clear keys for update
    for (int r = 0; r < mDataManager->allSection().size(); ++r) {
        Section* section = (Section*) mDataManager->allSection().at(r);
        section->clearKeys();
    }
    // to reset all correct
    resolveKeysForSections();
    qDebug() << "FINISH: Sections keys cleared";

    // KeyTag: clear keys for update
    for (int st = 0; st < mDataManager->allKeyTag().size(); ++st) {
        KeyTag* tag = (KeyTag*) mDataManager->allKeyTag().at(st);
        tag->clearKeys();
        qDebug() << "clear keys for Tag " << tag->name();
    }
    // to reset all correct
    resolveKeysForTags();
    qDebug() << "FINISH: Tags keys cleared";

    // KeyContentBlock: clear keys for update
    for (int st = 0; st < mDataManager->allKeyContentBlock().size(); ++st) {
        KeyContentBlock* contentBlock = (KeyContentBlock*) mDataManager->allKeyContentBlock().at(st);
        contentBlock->clearKeys();
        qDebug() << "clear keys for ContentBlock " << contentBlock->name();
    }
    // to reset all correct
    resolveKeysForContentBlocks();
    qDebug() << "FINISH: ContentBlocks keys cleared";

    // KeyContentStandardType: clear keys for update
    for (int st = 0; st < mDataManager->allKeyContentStandardType().size(); ++st) {
        KeyContentStandardType* contentStandardType = (KeyContentStandardType*) mDataManager->allKeyContentStandardType().at(st);
        contentStandardType->clearKeys();
        qDebug() << "clear keys for ContentStandardType " << contentStandardType->name();
    }
    // to reset all correct
    resolveKeysForContentStandardTypes();
    qDebug() << "FINISH: ContentStandardTypes keys cleared";

    // Day: clear keys for update
    for (int d = 0; d < mDataManager->allDay().size(); ++d) {
        Day* day = (Day*) mDataManager->allDay().at(d);
        day->clearKeys();
    }
    // to reset all correct
    resolveKeysForDictionary();
    qDebug() << "FINISH: Days keys cleared";

    // Creator: insert sorted Creators, clear Keys
    mDataManager->mAllCreator.clear();
    QMapIterator<QString, Creator*> creatorIterator(mMultiCreator);
    while (creatorIterator.hasNext()) {
        creatorIterator.next();
        Creator* creator = creatorIterator.value();
        creator->clearKeys();
        resolveKeysForCreator(creator);
        mDataManager->insertCreator(creator);
    }
    qDebug() << "FINISH: Sorted Creators inserted";

    // Check orphans
    for (int i = 0; i < mDataManager->mAllKey.size(); ++i) {
        Key* key = (Key*) mDataManager->mAllKey.at(i);
        bool keyFound = false;
        QMapIterator<QString, Key*> keyIterator(mMultiKey);
        while (keyIterator.hasNext()) {
            keyIterator.next();
            Key* updatedKey = keyIterator.value();
            if(updatedKey->keyId() == key->keyId()) {
                keyFound = true;
                break;
            }
        }
        if(!keyFound) {
            key->deleteLater();
            qDebug() << "Key removed: " << key->keyId();
        }
    }
    qDebug() << "FINISH: Key orphans deleted";

    // Key: insert sorted Keys
    // creator, keyLinks, day, section, tag, contentBlock, contentStandardType dictionaryItem are updated
    mDataManager->mAllKey.clear();

    // now insert sorted Keys, update keys for Day, Section, Tags, ContentBlocks, ContentStandardTypes Creator
    sortedKeysIntoSectionDayTagContentBlockContentStandardTypeCreator();
    qDebug() << "FINISH: Sections, Days, Tags, ContentBlocks, ContentStandardTypes, Creator, Keys sorted";

    mDataManager->saveKeyToCache();
    qDebug() << "FINISH: Keys saved";

    // KeyLink
    mDataManager->saveKeyLinkToCache();
    qDebug() << "FINISH: KeyLinks saved";

    // DictionaryItem
    mDataManager->saveDictionaryItemToCache();
    qDebug() << "FINISH: DictionaryItems saved";

    // CREATOR
    mDataManager->saveCreatorToCache();
    qDebug() << "FINISH: Creator saved";

    // insert Creator Images
    // do NOT mDataManager->mAllCreatorImage.clear();
    // mMultiCreatorImages contains NEW or UPDATED Creator Images !
    QMapIterator<bool, CreatorImage*> creatorImagesIterator(mMultiCreatorImages);
    while (creatorImagesIterator.hasNext()) {
        creatorImagesIterator.next();
        CreatorImage* creatorImage = creatorImagesIterator.value();
        if(!mDataManager->findCreatorImageByCreatorId(creatorImage->creatorId())) {
            // a NEW Creator Image
            qDebug() << "DETECTED NEW Creator Image: " << creatorImage->creatorId();
            mDataManager->insertCreatorImage(creatorImage);
        } else {
            qDebug() << "DETECTED UPDATED Creator Image: " << creatorImage->creatorId();
        }
    }
    mDataManager->saveCreatorImageToCache();
    qDebug() << "FINISH: Creator Images saved";

    // Tag sort by Name
    QMultiMap<QString, KeyTag*> keyTagSortMap;
    for (int i = 0; i < mDataManager->allKeyTag().size(); ++i) {
        KeyTag* keyTag = (KeyTag*) mDataManager->allKeyTag().at(i);
        keyTagSortMap.insert(keyTag->name(), keyTag);
    }
    qDebug() << "FINISH: Tags sorted by Name";
    mDataManager->mAllKeyTag.clear();
    QMapIterator<QString, KeyTag*> keyTagIterator(keyTagSortMap);
    while (keyTagIterator.hasNext()) {
        keyTagIterator.next();
        KeyTag* keyTag = keyTagIterator.value();
        mDataManager->insertKeyTag(keyTag);
    }
    // save all Tags with sorted Keys
    mDataManager->saveKeyTagToCache();
    qDebug() << "FINISH: Tags saved";

    // ContentBlock sort by Name
    QMultiMap<QString, KeyContentBlock*> keyContentBlockSortMap;
    for (int i = 0; i < mDataManager->allKeyContentBlock().size(); ++i) {
        KeyContentBlock* keyContentBlock = (KeyContentBlock*) mDataManager->allKeyContentBlock().at(i);
        keyContentBlockSortMap.insert(keyContentBlock->name(), keyContentBlock);
    }
    qDebug() << "FINISH: ContentBlocks sorted by Name";
    mDataManager->mAllKeyContentBlock.clear();
    QMapIterator<QString, KeyContentBlock*> keyContentBlockIterator(keyContentBlockSortMap);
    while (keyContentBlockIterator.hasNext()) {
        keyContentBlockIterator.next();
        KeyContentBlock* keyContentBlock = keyContentBlockIterator.value();
        mDataManager->insertKeyContentBlock(keyContentBlock);
    }
    // save all ContentBlocks with sorted Keys
    mDataManager->saveKeyContentBlockToCache();
    qDebug() << "FINISH: ContentBlocks saved";

    // ContentStandardType sort by Name
    QMultiMap<QString, KeyContentStandardType*> keyContentStandardTypeSortMap;
    for (int i = 0; i < mDataManager->allKeyContentStandardType().size(); ++i) {
        KeyContentStandardType* keyContentStandardType = (KeyContentStandardType*) mDataManager->allKeyContentStandardType().at(i);
        keyContentStandardTypeSortMap.insert(keyContentStandardType->name(), keyContentStandardType);
    }
    qDebug() << "FINISH: ContentStandardTypes sorted by Name";
    mDataManager->mAllKeyContentStandardType.clear();
    QMapIterator<QString, KeyContentStandardType*> keyContentStandardTypeIterator(keyContentStandardTypeSortMap);
    while (keyContentStandardTypeIterator.hasNext()) {
        keyContentStandardTypeIterator.next();
        KeyContentStandardType* keyContentStandardType = keyContentStandardTypeIterator.value();
        mDataManager->insertKeyContentStandardType(keyContentStandardType);
    }
    // save all ContentStandardTypes with sorted Keys
    mDataManager->saveKeyContentStandardTypeToCache();
    qDebug() << "FINISH: ContentStandardTypes saved";

    // Day: save all days with sorted Keys
    mDataManager->saveDayToCache();
    qDebug() << "FINISH: Days saved";

    // Sections: save all Sections with sorted Keys
    mDataManager->saveSectionToCache();
    qDebug() << "FINISH: Sections saved";

    // SETTINGS update API
    mDataManager->mSettingsData->setApiVersion(mNewApi);
    mDataManager->mSettingsData->setLastUpdate(QDateTime::currentDateTime());
    //
    mProgressInfotext.append("\n").append(tr("All done"));
    emit progressInfo(mProgressInfotext);
    qDebug() << "FINISH: All Done";
    emit updateDone();
}

//  U T I L I T I E S  to manage Repository data

/**
 * Favorites are transient on Keys
 * Keys are read-only
 * So at startup the propertie will be set
 */
void DataUtil::setKeyFavorites()
{
    for (int i = 0; i < mDataManager->mAllFavorite.size(); ++i) {
        Favorite* favorite = (Favorite*) mDataManager->mAllFavorite.at(i);
        Key* key = mDataManager->findKeyByKeyId(favorite->keyId());
        if(key != NULL) {
            key->setIsFavorite(true);
        }
    }
}

/**
 * Favorites are transient on Keys
 * Keys are read-only
 * So while caching data Favorites will be created
 */
void DataUtil::saveKeyFavorites()
{
    mDataManager->mAllFavorite.clear();
    for (int i = 0; i < mDataManager->mAllKey.size(); ++i) {
        Key* key = (Key*) mDataManager->mAllKey.at(i);
        if(key->isFavorite()) {
            Favorite* favorite = mDataManager->createFavorite();
            favorite->setKeyId(key->keyId());
            mDataManager->insertFavorite(favorite);
        }
    }
}

/**
 * list of keys for a Day are lazy
 * (only IDs stored in an Array)
 * for the Repository we always need all to create the dictionary
 * so at startup or update this will be called
 */
void DataUtil::resolveKeysForDictionary() {
    for (int i = 0; i < mDataManager->mAllDay.size(); ++i) {
        Day* day = (Day*) mDataManager->mAllDay.at(i);
        day->resolveKeysKeys(mDataManager->listOfKeyForKeys(day->keysKeys()));
    }
}

QString DataUtil::unaccent(QString s) {
    if (diacriticLetters_.isEmpty()) {
        diacriticLetters_ = QString::fromUtf8("ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ");
        noDiacriticLetters_ << "S"<<"OE"<<"Z"<<"s"<<"oe"<<"z"<<"Y"<<"Y"<<"u"<<"A"<<"A"<<"A"<<"A"<<"A"<<"A"<<"AE"<<"C"<<"E"<<"E"<<"E"<<"E"<<"I"<<"I"<<"I"<<"I"<<"D"<<"N"<<"O"<<"O"<<"O"<<"O"<<"O"<<"O"<<"U"<<"U"<<"U"<<"U"<<"Y"<<"s"<<"a"<<"a"<<"a"<<"a"<<"a"<<"a"<<"ae"<<"c"<<"e"<<"e"<<"e"<<"e"<<"i"<<"i"<<"i"<<"i"<<"o"<<"n"<<"o"<<"o"<<"o"<<"o"<<"o"<<"o"<<"u"<<"u"<<"u"<<"u"<<"y"<<"y";
    }

    QString output = "";
    for (int i = 0; i < s.length(); i++) {
        QChar c = s[i];
        int dIndex = diacriticLetters_.indexOf(c);
        if (dIndex < 0) {
            output.append(c);
        } else {
            QString replacement = noDiacriticLetters_[dIndex];
            output.append(replacement);
        }
    }

    return output;
}

/**
 * list of keys for a Day are lazy
 * (only IDs stored in an Array)
 * for the Repository we always need all to create the dictionary
 * so at startup or update this will be called
 */
void DataUtil::resolveKeysForDictionaryByFilter(QString filter, bool startWith = false, bool fullTextSearch = false) {
    // Remove *
    filter = filter.replace("*","");

    // Get Day1 and Day2
    Day* day1 = (Day*) mDataManager->mAllDay.at(0);
    Day* day2 = (Day*) mDataManager->mAllDay.at(1);

    // Get all Keys
    QList<Key*> keys1 = mDataManager->listOfKeyForKeys(day1->keysKeys());
    QList<Key*> keys2 = mDataManager->listOfKeyForKeys(day2->keysKeys());

    // Clear All Keys on Day 2
    for (int i = 0; i < keys1.size(); ++i) {
        day2->removeFromKeys(keys1.at(i));
    }

    // MODE
    for (int i = 0; i < keys1.size(); ++i) {
        // Add Keys - TEXT FREE
        if (startWith == false)  {
            if (unaccent(keys1.at(i)->title().toUpper()).indexOf(unaccent(filter.toUpper())) != -1){
                day2->addToKeys(keys1.at(i));
            }
        }
        // Add Keys - START WITH
        else if (startWith == true) {
            if (unaccent(keys1.at(i)->title().toUpper()).startsWith(unaccent(filter.toUpper())) == true){
                day2->addToKeys(keys1.at(i));
            }
        }
        // Add Keys - FULLTEXT
        if (fullTextSearch == true) {
            if (startWith == false)  {
                if (unaccent(keys1.at(i)->description()).toUpper().indexOf(unaccent(filter.toUpper())) != -1){
                    day2->addToKeys(keys1.at(i));
                }
            }
            // Add Keys - START WITH
            else if (startWith == true) {
                if (unaccent(keys1.at(i)->description()).toUpper().startsWith(unaccent(filter.toUpper())) == true){
                    day2->addToKeys(keys1.at(i));
                }
            }
        }
    }
}

/**
 * list of keys for a Tag are lazy
 * (only IDs stored in an Array)
 * this will be called as soon as Tags are first time displayed
 */
void DataUtil::resolveKeysForTags()
{
    for (int i = 0; i < mDataManager->mAllKeyTag.size(); ++i) {
        KeyTag* keyTag = (KeyTag*) mDataManager->mAllKeyTag.at(i);
        keyTag->resolveKeysKeys(mDataManager->listOfKeyForKeys(keyTag->keysKeys()));
    }
}

/**
 * list of keys for a ContentBlock are lazy
 * (only IDs stored in an Array)
 * this will be called as soon as ContentBlocks are first time displayed
 */
void DataUtil::resolveKeysForContentBlocks()
{
    for (int i = 0; i < mDataManager->mAllKeyContentBlock.size(); ++i) {
        KeyContentBlock* keyContentBlock = (KeyContentBlock*) mDataManager->mAllKeyContentBlock.at(i);
        keyContentBlock->resolveKeysKeys(mDataManager->listOfKeyForKeys(keyContentBlock->keysKeys()));
    }
}

/**
 * list of keys for a ContentStandardType are lazy
 * (only IDs stored in an Array)
 * this will be called as soon as ContentStandardTypes are first time displayed
 */
void DataUtil::resolveKeysForContentStandardTypes()
{
    for (int i = 0; i < mDataManager->mAllKeyContentStandardType.size(); ++i) {
        KeyContentStandardType* keyContentStandardType = (KeyContentStandardType*) mDataManager->mAllKeyContentStandardType.at(i);
        keyContentStandardType->resolveKeysKeys(mDataManager->listOfKeyForKeys(keyContentStandardType->keysKeys()));
    }
}

/**
 * list of keys for a Section are lazy
 * (only IDs stored in an Array)
 * this will be called as soon as Section are displayed
 */
void DataUtil::resolveKeysForSections()
{
    for (int i = 0; i < mDataManager->mAllSection.size(); ++i) {
        Section* section = (Section*) mDataManager->mAllSection.at(i);
        section->resolveKeysKeys(mDataManager->listOfKeyForKeys(section->keysKeys()));
    }
}

/**
 * list of keys for a Creator are lazy
 * (only IDs stored in an Array)
 * this will be called as soon as Creator are displayed
 */
void DataUtil::resolveKeysForCreator(Creator* creator)
{
    creator->resolveKeysKeys(mDataManager->listOfKeyForKeys(creator->keysKeys()));
}

KeyLists *DataUtil::myDictionary()
{
    return mKeyLists;
}

void DataUtil::refreshMyDictionary()
{
    mKeyLists->clearDictionarydKeys();
    for (int i = 0; i < mDataManager->allKey().size(); ++i) {
        Key* key = (Key*) mDataManager->allKey().at(i);
        if(!key->isBoolean2() && key->isFavorite()) {
            mKeyLists->addToDictionarydKeys(key);
        }
    }
    qDebug() << "MY DICTIONARY #:" << mKeyLists->dictionarydKeysCount();
    emit myDictionaryRefreshed();
}

int DataUtil::findFirstKeyItem(int repositoryDayIndex, QString pickedTime)
{
    if(repositoryDayIndex < 0 || repositoryDayIndex > (mDataManager->mAllDay.size()-1)) {
        qDebug() << "Day Index wrong: repositoryDayIndex";
        return -1;
    }
    Day* day = (Day*) mDataManager->mAllDay.at(repositoryDayIndex);
    for (int i = 0; i < day->keys().size(); ++i) {
        Key* key = day->keys().at(i);
        QString theTime = key->sortKey().right(5);
        if(theTime >= pickedTime) {
            return i;
        }
    }
    return day->keys().size();
}

int DataUtil::findFirstKeyItemByLetter(int repositoryDayIndex, QString pickedLetter)
{
    if(repositoryDayIndex < 0 || repositoryDayIndex > (mDataManager->mAllDay.size()-1)) {
        qDebug() << "Day Index wrong: repositoryDayIndex";
        return -1;
    }
    Day* day = (Day*) mDataManager->mAllDay.at(repositoryDayIndex);
    for (int i = 0; i < day->keys().size(); ++i) {
        Key* key = day->keys().at(i);
        QString theLetter = key->title().left(1);
        if (theLetter == pickedLetter) {
            return i;
        }
    }
    return day->keys().size();
}


int DataUtil::findFirstCreatorItem(QString section)
{
    for (int i = 0; i < mDataManager->mAllCreator.size(); ++i) {
        Creator* creator = (Creator*) mDataManager->mAllCreator.at(i);
        if(creator->sortGroup() >= section) {
            return i;
        }
    }
    return mDataManager->mAllCreator.size();
}

// Sortkey: day->repositoryDay().toString(YYYY_MM_DD)+key->startTime().toString("HH:mm")
QString DataUtil::localWeekdayAndTime(QString keySortkey)
{
    QDateTime sortTime = QDateTime::fromString(keySortkey, YYYY_MM_DD_HH_MM);
    QString weekDayAndTime = sortTime.toString(DAYNAME);
    weekDayAndTime.append(", ");
    QTime myTime = QTime::fromString(keySortkey.right(5));
    weekDayAndTime.append(myTime.toString(Qt::SystemLocaleShortDate));
    return weekDayAndTime;
}

QString DataUtil::apiInfo()
{
    QString apiInfo = tr("API Version ");
    apiInfo.append(mDataManager->settingsData()->apiVersion());
    apiInfo.append(("\n")).append(tr("Last Update "));
    if(mDataManager->mSettingsData->hasLastUpdate()) {
        apiInfo.append(mDataManager->mSettingsData->lastUpdate().toString(Qt::SystemLocaleShortDate));
    } else {
        apiInfo.append("n/a");
    }
    return apiInfo;
}


// DOWNLOAD   C R E A T O R     I M A G E S
// K E Y   UPDATE REPOSITORY
void DataUtil::onCreatorImageUpdateLoaded(QObject *dataObject, int width, int height)
{
    mImageLoader->deleteLater();
    CreatorImage* creatorImage = (CreatorImage*) dataObject;
    qDebug() << "onCreatorImage  L O A D E D ";
    creatorImage->setDownloadSuccess(true);
    creatorImage->setDownloadFailed(false);
    creatorImage->setInAssets(false);
    creatorImage->setInData(true);
    prepareHighDpiImages(creatorImage, width, height);
    // set update flag
    int count = mMultiCreatorImages.remove(false, creatorImage);
    if(count != 1) {
        qWarning() << "something went wrong: the CreatorImage MUST exist in MultiMap";
    }
    mMultiCreatorImages.insert(true, creatorImage);
    // check for more
    updateCreatorImages();
}

void DataUtil::onCreatorImageUpdateFailed(QObject *dataObject, QString message) {
    mImageLoader->deleteLater();
    CreatorImage* creatorImage = (CreatorImage*) dataObject;
    qDebug() << "UPDATE: Cannot load Creator Image:  " << message << creatorImage->creatorId();
    creatorImage->setDownloadSuccess(false);
    creatorImage->setDownloadFailed(true);
    creatorImage->setInAssets(false);
    creatorImage->setInData(false);
    // set update flag
    int count = mMultiCreatorImages.remove(false, creatorImage);
    if(count != 1) {
        qWarning() << "something went wrong: the CreatorImage MUST exist in MultiMap";
    }
    mMultiCreatorImages.insert(true, creatorImage);
    // check for more
    updateCreatorImages();
}

// K E Y  PREPARE REPOSITORY
void DataUtil::onCreatorImageLoaded(QObject *dataObject, int width, int height)
{
    mImageLoader->deleteLater();
    CreatorImage* creatorImage = (CreatorImage*) dataObject;
    qDebug() << "loaded..." << creatorImage->creatorId() << " " << width << "x" << height;
    creatorImage->setDownloadSuccess(true);
    creatorImage->setDownloadFailed(false);
    creatorImage->setInAssets(true);
    creatorImage->setInData(false);
    prepareHighDpiImages(creatorImage, width, height);
    // more to load ?
    prepareCreatorImages();
}

void DataUtil::onCreatorImageFailed(QObject *dataObject, QString message) {
    mImageLoader->deleteLater();
    CreatorImage* creatorImage = (CreatorImage*) dataObject;
    qDebug() << "PREPARE: Cannot load Creator Image:  " << message << creatorImage->creatorId();
    creatorImage->setDownloadSuccess(false);
    creatorImage->setDownloadFailed(true);
    creatorImage->setInAssets(false);
    creatorImage->setInData(false);
    // more to load ?
    prepareCreatorImages();
}

void DataUtil::prepareHighDpiImages(CreatorImage* creatorImage, int width, int height) {
    const QString creatorImagesPath = mRepositoryDataPath + "creatorImages/";
    QString fileName;
    fileName = creatorImagesPath + "creator_";
    fileName.append(QString::number(creatorImage->creatorId()));
    QString originFileName;
    originFileName = fileName + "." + creatorImage->suffix();
    const int size1 = 64;
    const int size2 = 128;
    const int size3 = 192;
    const int size4 = 256;
    if(width >= height) {
        if(width < size1) {
            creatorImage->setMaxScaleFactor(0);
            return;
        }
        QFile originFile(originFileName);
        if(!originFile.exists()) {
            qWarning() << "CreatorImage Path not found " << originFileName;
            return;
        }
        if (!originFile.open(QIODevice::ReadOnly)) {
            qWarning() << "Couldn't open file: " << originFileName;
            return;
        }
        QImage originImage = QImage::fromData(originFile.readAll());
        if(originImage.isNull()) {
            qWarning() << "Cannot construct Image from file: " << originFileName;
            return;
        }
        QImage scaledImage;
        if(width >= size1) {
            scaledImage = originImage.scaledToWidth(size1);
            scaledImage.save(originFileName);
            creatorImage->setMaxScaleFactor(1);
        }
        if(width >= size2) {
            scaledImage = originImage.scaledToWidth(size2);
            scaledImage.save(fileName+"@2x."+creatorImage->suffix());
            creatorImage->setMaxScaleFactor(2);
        }
        if(width >= size3) {
            scaledImage = originImage.scaledToWidth(size3);
            scaledImage.save(fileName+"@3x."+creatorImage->suffix());
            creatorImage->setMaxScaleFactor(3);
        }
        if(width >= size4) {
            scaledImage = originImage.scaledToWidth(size4);
            scaledImage.save(fileName+"@4x."+creatorImage->suffix());
            creatorImage->setMaxScaleFactor(4);
        }
    } else {
        if(height < size1) {
            creatorImage->setMaxScaleFactor(0);
            return;
        }
        QFile originFile(originFileName);
        if(!originFile.exists()) {
            qWarning() << "CreatorImage Path not found " << originFileName;
            return;
        }
        if (!originFile.open(QIODevice::ReadOnly)) {
            qWarning() << "Couldn't open file: " << originFileName;
            return;
        }
        QImage originImage = QImage::fromData(originFile.readAll());
        if(originImage.isNull()) {
            qWarning() << "Cannot construct Image from file: " << originFileName;
            return;
        }
        QImage scaledImage;
        if(height >= size1) {
            scaledImage = originImage.scaledToHeight(size1);
            scaledImage.save(originFileName);
            creatorImage->setMaxScaleFactor(1);
        }
        if(height >= size2) {
            scaledImage = originImage.scaledToHeight(size2);
            scaledImage.save(fileName+"@2x."+creatorImage->suffix());
            creatorImage->setMaxScaleFactor(2);
        }
        if(height >= size3) {
            scaledImage = originImage.scaledToHeight(size3);
            scaledImage.save(fileName+"@3x."+creatorImage->suffix());
            creatorImage->setMaxScaleFactor(3);
        }
        if(height >= size4) {
            scaledImage = originImage.scaledToHeight(size4);
            scaledImage.save(fileName+"@4x."+creatorImage->suffix());
            creatorImage->setMaxScaleFactor(4);
        }
    }
}

// K E Y S
void DataUtil::onServerSuccess()
{
    qDebug() << "S U C C E S S";

    const QString dictionaryPath = mRepositoryDataPath + "dictionary.json";
    qDebug() << "CHECK FOR UPDATE KEYS ";
    QVariantMap map;
    map = readDictionaryFile(dictionaryPath);

    if(map.isEmpty()) {
        qWarning() << "Dictionary is no Map";
        emit checkForUpdateFailed(tr("Error: Received Map is empty."));
        return;
    }

    // now do the real work
    continueUpdate();
}

void DataUtil::onVersionSuccess(QByteArray currentVersionBytes)
{
    QJsonDocument jda;
    jda = QJsonDocument::fromJson(currentVersionBytes);
    if(!jda.isObject()) {
        onVersionFailed(tr("No JSON Object received for Version Check"));
        return;
    }
    QVariantMap versionMap = jda.toVariant().toMap();
    if(versionMap.isEmpty()) {
        onVersionFailed(tr("Version Check: received Version Map is empty"));
        return;
    }
    if(!versionMap.contains("version")) {
        onVersionFailed(tr("Version Check: received Version Map missed Version Property"));
        return;
    }
    mNewApi = versionMap.value("version").toString();
    if(mNewApi.isEmpty()) {
        onVersionFailed(tr("Error: Received Map missed 'version'."));
        return;
    }
    qDebug() << "VERSION: " + mNewApi;

    QStringList versionList;
    versionList = mNewApi.split(".");
    if(versionList.size() != 2) {
        emit onVersionFailed(tr("Error: 'Version' wrong: ")+mNewApi);
        return;
    }

    if(mDataManager->mSettingsData->apiVersion().length() == 0) {
        emit updateAvailable(mNewApi);
        return;
    }
    QStringList oldVersionList;
    oldVersionList = mDataManager->mSettingsData->apiVersion().split(".");
    if(oldVersionList.size() != 2) {
        emit updateAvailable(mNewApi);
        return;
    }

    int oldValue = oldVersionList.at(0).toInt();
    int newValue = versionList.at(0).toInt();
    if(oldValue > newValue) {
        emit noUpdateRequired();
        return;
    }
    oldValue = oldVersionList.at(0).toInt();
    newValue = versionList.at(0).toInt();
    if(oldValue < newValue) {
        emit updateAvailable(mNewApi);
        return;
    }
    oldValue = oldVersionList.at(1).toInt();
    newValue = versionList.at(1).toInt();
    if(oldValue <  newValue) {
        emit updateAvailable(mNewApi);
        return;
    }
    emit noUpdateRequired();
}

void DataUtil::onVersionFailed(const QString message)
{
    qDebug() << "Version failed" << message;
    emit checkForUpdateFailed(message);
}

void DataUtil::onServerFailed(QString message)
{
    qDebug() << "FAILED: " << message;
    emit updateFailed(message);
}

