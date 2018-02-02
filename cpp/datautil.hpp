#ifndef DATAUTIL_H
#define DATAUTIL_H

#include <QObject>
#include "gen/DataManager.hpp"
#include "imageloader.hpp"
#include "dataserver.hpp"

class DataUtil : public QObject
{
    Q_OBJECT
public:
    explicit DataUtil(QObject *parent = 0);

    void init(DataManager *dataManager, DataServer* dataServer);

    Q_INVOKABLE
    void prepareRepository();
    void prepareKeys();
    void prepareCreator();
    void prepareCreatorImages();

    Q_INVOKABLE
    void checkVersion();

    Q_INVOKABLE
    void startUpdate();

    Q_INVOKABLE
    void setKeyFavorites();
    void saveKeyFavorites();

    Q_INVOKABLE
    void resolveKeysForDictionary();

    Q_INVOKABLE
    QString unaccent(const QString s);

    Q_INVOKABLE
    void resolveKeysForDictionaryByFilter(QString filter, bool startWith, bool fullTextSearch);

    Q_INVOKABLE
    void resolveKeysForTags();

    Q_INVOKABLE
    void resolveKeysForContentBlocks();

    Q_INVOKABLE
    void resolveKeysForContentStandardTypes();

    Q_INVOKABLE
    void resolveKeysForSections();

    Q_INVOKABLE
    void resolveKeysForCreator(Creator *creator);

    Q_INVOKABLE
    KeyLists* myDictionary();

    Q_INVOKABLE
    void refreshMyDictionary();

    Q_INVOKABLE
    int findFirstKeyItem(int repositoryDayIndex, QString pickedTime);

    Q_INVOKABLE
    int findFirstKeyItemByLetter(int repositoryDayIndex, QString pickedLetter);

    Q_INVOKABLE
    int findFirstCreatorItem(QString section);

    Q_INVOKABLE
    QString localWeekdayAndTime(QString keySortkey);

    Q_INVOKABLE
    QString apiInfo();

    Q_INVOKABLE
    void reloadData();

    // some helpers for QML UI
    Q_INVOKABLE
    QString repositoryDataPath4QML();

    Q_INVOKABLE
    bool isDateTooLate();

    Q_INVOKABLE
    QString keyInfoForCreator(Creator* creator);

    Q_INVOKABLE
    QString creatorNamesForKey(Key* key);

    Q_INVOKABLE
    QString dictionaryItemImageForKey(Key* key);

    Q_INVOKABLE
    QString sectionForButton(Key* key);

    Q_INVOKABLE
    QString textForKeyTag(KeyTag *keyTag);

    Q_INVOKABLE
    QString textForKeyContentBlock(KeyContentBlock *keyContentBlock);

    Q_INVOKABLE
    QString textForKeyContentStandardType(KeyContentStandardType *keyContentStandardType);

    Q_INVOKABLE
    QString textForKeyType(Key* key);

    Q_INVOKABLE
    QString tagColor(const int tagId);

    Q_INVOKABLE
    QString contentBlockColor(const int contentBlockId);

    Q_INVOKABLE
    QString contentStandardTypeColor(const int contentStandardTypeId);

    Q_INVOKABLE
    QString tagColorFirstTag(Key* key);

    Q_INVOKABLE
    QString contentBlockColorFirstContentBlock(Key* key);

    Q_INVOKABLE
    QString contentStandardTypeColorFirstContentStandardType(Key* key);

signals:
    void myDictionaryRefreshed();

    void updateAvailable(const QString apiVersion);
    void noUpdateRequired();
    void checkForUpdateFailed(const QString message);

    void updateDone();
    void updateFailed(const QString message);

    void progressInfo(const QString progressInfo);

public slots:
    void onCreatorImageLoaded(QObject* dataObject, int width, int height);

    void onCreatorImageUpdateLoaded(QObject *dataObject, int width, int height);
    void onCreatorImageFailed(QObject *dataObject, QString message);
    void onCreatorImageUpdateFailed(QObject *dataObject, QString message);
private slots:
    void onServerSuccess();
    void onServerFailed(const QString message);

    void onVersionSuccess(QByteArray currentVersionBytes);
    void onVersionFailed(const QString message);

private:

    DataManager* mDataManager;
    DataServer* mDataServer;

    ImageLoader* mImageLoader;

    KeyLists* mKeyLists;

    void prepareHighDpiImages(CreatorImage *creatorImage, int width, int height);

    void prepareEventData();

    bool checkDirs();
    QString mRepositoryDataPath;

    // UPDATE
    void continueUpdate();
    QString mNewApi;
    QString mProgressInfotext;
    QMultiMap<QString, Key*> mMultiKey;
    QMultiMap<QString, Creator*> mMultiCreator;
    QMultiMap<bool, CreatorImage*> mMultiCreatorImages;
    QVariantMap readDictionaryFile(const QString dictionaryPath);
    QVariantList readCreatorFile(const QString creatorPath);
    void calcCreatorName(Creator *creator, CreatorAPI *creatorAPI);
    void updateCreatorImages();
    void updateKeys();
    Day *findDayForServerDate(const QString &dayDate);

    void adjustPersons(QVariantMap &keyMap);
    void adjustTags(QVariantMap &keyMap, Repository *repository, const bool isUpdate);
    void adjustContentBlocks(QVariantMap &keyMap, Repository *repository, const bool isUpdate);
    void adjustContentStandardTypes(QVariantMap &keyMap, Repository *repository, const bool isUpdate);

    bool checkIfIgnored(KeyAPI *keyAPI);
    void setDuration(KeyAPI *keyAPI, Key* key);
    void setType(Key *key);
    void createAndAdjustLinks(QVariantMap &keyMap);
    void updateAndAdjustLinks(QVariantMap &keyMap);
    void finishUpdate();
    void sortedKeysIntoSectionDayTagContentBlockContentStandardTypeCreator();

    QString diacriticLetters_;
    QStringList noDiacriticLetters_;
};

#endif // DATAUTIL_H
