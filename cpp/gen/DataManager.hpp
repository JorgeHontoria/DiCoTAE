#ifndef DATAMANAGER_HPP_
#define DATAMANAGER_HPP_

#include <qobject.h>
#include <QQmlListProperty>
#include <QStringList>

#include "SettingsData.hpp"
#include "Repository.hpp"
#include "Dictionary.hpp"
#include "Category.hpp"
#include "Section.hpp"
#include "Key.hpp"
#include "DictionaryItem.hpp"
#include "Favorite.hpp"
#include "Bookmark.hpp"
#include "KeyLists.hpp"
#include "KeyLink.hpp"
#include "Creator.hpp"
#include "CreatorImage.hpp"
#include "KeyTag.hpp"
#include "KeyContentBlock.hpp"
#include "KeyContentStandardType.hpp"
#include "Day.hpp"
#include "KeyAPI.hpp"
#include "PersonsAPI.hpp"
#include "KeyLinkAPI.hpp"
#include "CreatorAPI.hpp"

class DataManager: public QObject
{
    Q_OBJECT

    // QQmlListProperty to get easy access from QML
    Q_PROPERTY(QQmlListProperty<Repository> repositoryPropertyList READ repositoryPropertyList NOTIFY repositoryPropertyListChanged)
    Q_PROPERTY(QQmlListProperty<Dictionary> dictionaryPropertyList READ dictionaryPropertyList NOTIFY dictionaryPropertyListChanged)
    Q_PROPERTY(QQmlListProperty<Category> categoryPropertyList READ categoryPropertyList NOTIFY categoryPropertyListChanged)
    Q_PROPERTY(QQmlListProperty<Section> sectionPropertyList READ sectionPropertyList NOTIFY sectionPropertyListChanged)
    Q_PROPERTY(QQmlListProperty<Key> keyPropertyList READ keyPropertyList NOTIFY keyPropertyListChanged)
    Q_PROPERTY(QQmlListProperty<DictionaryItem> dictionaryItemPropertyList READ dictionaryItemPropertyList NOTIFY dictionaryItemPropertyListChanged)
    Q_PROPERTY(QQmlListProperty<Favorite> favoritePropertyList READ favoritePropertyList NOTIFY favoritePropertyListChanged)
    Q_PROPERTY(QQmlListProperty<Bookmark> bookmarkPropertyList READ bookmarkPropertyList NOTIFY bookmarkPropertyListChanged)
    Q_PROPERTY(QQmlListProperty<KeyLists> keyListsPropertyList READ keyListsPropertyList NOTIFY keyListsPropertyListChanged)
    Q_PROPERTY(QQmlListProperty<KeyLink> keyLinkPropertyList READ keyLinkPropertyList NOTIFY keyLinkPropertyListChanged)
    Q_PROPERTY(QQmlListProperty<Creator> creatorPropertyList READ creatorPropertyList NOTIFY creatorPropertyListChanged)
    Q_PROPERTY(QQmlListProperty<CreatorImage> creatorImagePropertyList READ creatorImagePropertyList NOTIFY creatorImagePropertyListChanged)
    Q_PROPERTY(QQmlListProperty<KeyTag> keyTagPropertyList READ keyTagPropertyList NOTIFY keyTagPropertyListChanged)
    Q_PROPERTY(QQmlListProperty<KeyContentBlock> keyContentBlockPropertyList READ keyContentBlockPropertyList NOTIFY keyContentBlockPropertyListChanged)
    Q_PROPERTY(QQmlListProperty<KeyContentStandardType> keyContentStandardTypePropertyList READ keyContentStandardTypePropertyList NOTIFY keyContentStandardTypePropertyListChanged)
    Q_PROPERTY(QQmlListProperty<Day> dayPropertyList READ dayPropertyList NOTIFY dayPropertyListChanged)
    Q_PROPERTY(QQmlListProperty<KeyAPI> keyAPIPropertyList READ keyAPIPropertyList NOTIFY keyAPIPropertyListChanged)
    Q_PROPERTY(QQmlListProperty<PersonsAPI> creatorsAPIPropertyList READ creatorsAPIPropertyList NOTIFY creatorsAPIPropertyListChanged)
    Q_PROPERTY(QQmlListProperty<KeyLinkAPI> keyLinkAPIPropertyList READ keyLinkAPIPropertyList NOTIFY keyLinkAPIPropertyListChanged)
    Q_PROPERTY(QQmlListProperty<CreatorAPI> creatorAPIPropertyList READ creatorAPIPropertyList NOTIFY creatorAPIPropertyListChanged)

public:
    DataManager(QObject *parent = 0);
	
	friend class DataUtil;
	friend class DataServer;
	
    virtual ~DataManager();
    Q_INVOKABLE
    void init();
	bool checkDirs();


	Q_INVOKABLE
	SettingsData* createSettingsData();

	Q_INVOKABLE
	void undoCreateSettingsData(SettingsData* settingsData);

	
	Q_INVOKABLE
	QList<Repository*> listOfRepositoryForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList repositoryAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allRepository();

	Q_INVOKABLE
	void deleteRepository();

	// access from QML to list of all Repository
	QQmlListProperty<Repository> repositoryPropertyList();

	Q_INVOKABLE
	Repository* createRepository();

	Q_INVOKABLE
	void undoCreateRepository(Repository* repository);

	Q_INVOKABLE
	void insertRepository(Repository* repository);

	Q_INVOKABLE
	void insertRepositoryFromMap(const QVariantMap& repositoryMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteRepository(Repository* repository);
	

	Q_INVOKABLE
	bool deleteRepositoryById(const int& id);

	Q_INVOKABLE
    Repository* findRepositoryById(const int& id);

	
	
	Q_INVOKABLE
	QList<Dictionary*> listOfDictionaryForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList dictionaryAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allDictionary();

	Q_INVOKABLE
	void deleteDictionary();

	// access from QML to list of all Dictionary
	QQmlListProperty<Dictionary> dictionaryPropertyList();

	Q_INVOKABLE
	Dictionary* createDictionary();

	Q_INVOKABLE
	void undoCreateDictionary(Dictionary* dictionary);

	Q_INVOKABLE
	void insertDictionary(Dictionary* dictionary);

	Q_INVOKABLE
	void insertDictionaryFromMap(const QVariantMap& dictionaryMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteDictionary(Dictionary* dictionary);
	

	Q_INVOKABLE
	bool deleteDictionaryByDictionaryId(const int& dictionaryId);

	Q_INVOKABLE
    Dictionary* findDictionaryByDictionaryId(const int& dictionaryId);

	

	Q_INVOKABLE
	void resolveCategoryReferences(Category* category);

	Q_INVOKABLE
	void resolveReferencesForAllCategory();
	
	Q_INVOKABLE
	QList<Category*> listOfCategoryForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList categoryAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allCategory();

	Q_INVOKABLE
	void deleteCategory();

	// access from QML to list of all Category
	QQmlListProperty<Category> categoryPropertyList();

	Q_INVOKABLE
	Category* createCategory();

	Q_INVOKABLE
	void undoCreateCategory(Category* category);

	Q_INVOKABLE
	void insertCategory(Category* category);

	Q_INVOKABLE
	void insertCategoryFromMap(const QVariantMap& categoryMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteCategory(Category* category);
	

	Q_INVOKABLE
	bool deleteCategoryByCategoryId(const int& categoryId);

	Q_INVOKABLE
    Category* findCategoryByCategoryId(const int& categoryId);

	

	Q_INVOKABLE
	void resolveSectionReferences(Section* section);

	Q_INVOKABLE
	void resolveReferencesForAllSection();
	
	Q_INVOKABLE
	QList<Section*> listOfSectionForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList sectionAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allSection();

	Q_INVOKABLE
	void deleteSection();

	// access from QML to list of all Section
	QQmlListProperty<Section> sectionPropertyList();

	Q_INVOKABLE
	Section* createSection();

	Q_INVOKABLE
	void undoCreateSection(Section* section);

	Q_INVOKABLE
	void insertSection(Section* section);

	Q_INVOKABLE
	void insertSectionFromMap(const QVariantMap& sectionMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteSection(Section* section);
	

	Q_INVOKABLE
	bool deleteSectionBySectionId(const int& sectionId);

	Q_INVOKABLE
    Section* findSectionBySectionId(const int& sectionId);

	

	Q_INVOKABLE
	void resolveKeyReferences(Key* key);

	Q_INVOKABLE
	void resolveReferencesForAllKey();
	
	Q_INVOKABLE
	QList<Key*> listOfKeyForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList keyAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allKey();

	Q_INVOKABLE
	void deleteKey();

	// access from QML to list of all Key
	QQmlListProperty<Key> keyPropertyList();

	Q_INVOKABLE
	Key* createKey();

	Q_INVOKABLE
	void undoCreateKey(Key* key);

	Q_INVOKABLE
	void insertKey(Key* key);

	Q_INVOKABLE
	void insertKeyFromMap(const QVariantMap& keyMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteKey(Key* key);
	

	Q_INVOKABLE
	bool deleteKeyByKeyId(const int& keyId);

	Q_INVOKABLE
    Key* findKeyByKeyId(const int& keyId);

	

	Q_INVOKABLE
	void resolveDictionaryItemReferences(DictionaryItem* dictionaryItem);

	Q_INVOKABLE
	void resolveReferencesForAllDictionaryItem();
	
	Q_INVOKABLE
	QList<DictionaryItem*> listOfDictionaryItemForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList dictionaryItemAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allDictionaryItem();

	Q_INVOKABLE
	void deleteDictionaryItem();

	// access from QML to list of all DictionaryItem
	QQmlListProperty<DictionaryItem> dictionaryItemPropertyList();

	Q_INVOKABLE
    DictionaryItem* createDictionaryItem();

	Q_INVOKABLE
	void undoCreateDictionaryItem(DictionaryItem* dictionaryItem);

	Q_INVOKABLE
	void insertDictionaryItem(DictionaryItem* dictionaryItem);

	Q_INVOKABLE
	void insertDictionaryItemFromMap(const QVariantMap& dictionaryItemMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteDictionaryItem(DictionaryItem* dictionaryItem);
	

	Q_INVOKABLE
	bool deleteDictionaryItemByKeyId(const int& keyId);

	Q_INVOKABLE
    DictionaryItem* findDictionaryItemByKeyId(const int& keyId);

	

	Q_INVOKABLE
	void resolveFavoriteReferences(Favorite* favorite);

	Q_INVOKABLE
	void resolveReferencesForAllFavorite();
	
	Q_INVOKABLE
	QList<Favorite*> listOfFavoriteForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList favoriteAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allFavorite();

	Q_INVOKABLE
	void deleteFavorite();

	// access from QML to list of all Favorite
	QQmlListProperty<Favorite> favoritePropertyList();

	Q_INVOKABLE
	Favorite* createFavorite();

	Q_INVOKABLE
	void undoCreateFavorite(Favorite* favorite);

	Q_INVOKABLE
	void insertFavorite(Favorite* favorite);

	Q_INVOKABLE
	void insertFavoriteFromMap(const QVariantMap& favoriteMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteFavorite(Favorite* favorite);
	

	Q_INVOKABLE
	bool deleteFavoriteByKeyId(const int& keyId);

	Q_INVOKABLE
    Favorite* findFavoriteByKeyId(const int& keyId);

	

	Q_INVOKABLE
	void resolveBookmarkReferences(Bookmark* bookmark);

	Q_INVOKABLE
	void resolveReferencesForAllBookmark();
	
	Q_INVOKABLE
	QList<Bookmark*> listOfBookmarkForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList bookmarkAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allBookmark();

	Q_INVOKABLE
	void deleteBookmark();

	// access from QML to list of all Bookmark
	QQmlListProperty<Bookmark> bookmarkPropertyList();

	Q_INVOKABLE
	Bookmark* createBookmark();

	Q_INVOKABLE
	void undoCreateBookmark(Bookmark* bookmark);

	Q_INVOKABLE
	void insertBookmark(Bookmark* bookmark);

	Q_INVOKABLE
	void insertBookmarkFromMap(const QVariantMap& bookmarkMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteBookmark(Bookmark* bookmark);
	

	Q_INVOKABLE
	bool deleteBookmarkByKeyId(const int& keyId);

	Q_INVOKABLE
    Bookmark* findBookmarkByKeyId(const int& keyId);

	
	
	Q_INVOKABLE
	QList<KeyLists*> listOfKeyListsForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList keyListsAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allKeyLists();

	Q_INVOKABLE
	void deleteKeyLists();

	// access from QML to list of all KeyLists
	QQmlListProperty<KeyLists> keyListsPropertyList();

	Q_INVOKABLE
	KeyLists* createKeyLists();

	Q_INVOKABLE
	void undoCreateKeyLists(KeyLists* keyLists);

	Q_INVOKABLE
	void insertKeyLists(KeyLists* keyLists);

	Q_INVOKABLE
	void insertKeyListsFromMap(const QVariantMap& keyListsMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteKeyLists(KeyLists* keyLists);
	

	Q_INVOKABLE
	bool deleteKeyListsByUuid(const QString& uuid);

	Q_INVOKABLE
	KeyLists* findKeyListsByUuid(const QString& uuid);
	
	
	Q_INVOKABLE
	QList<KeyLink*> listOfKeyLinkForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList keyLinkAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allKeyLink();

	Q_INVOKABLE
	void deleteKeyLink();

	// access from QML to list of all KeyLink
	QQmlListProperty<KeyLink> keyLinkPropertyList();

	Q_INVOKABLE
	KeyLink* createKeyLink();

	Q_INVOKABLE
	void undoCreateKeyLink(KeyLink* keyLink);

	Q_INVOKABLE
	void insertKeyLink(KeyLink* keyLink);

	Q_INVOKABLE
	void insertKeyLinkFromMap(const QVariantMap& keyLinkMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteKeyLink(KeyLink* keyLink);
	

	Q_INVOKABLE
	bool deleteKeyLinkByUuid(const QString& uuid);

	Q_INVOKABLE
	KeyLink* findKeyLinkByUuid(const QString& uuid);
	

	Q_INVOKABLE
    void resolveCreatorReferences(Creator* creator);

	Q_INVOKABLE
    void resolveReferencesForAllCreator();
	
	Q_INVOKABLE
    QList<Creator*> listOfCreatorForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList creatorAsQVariantList();

	Q_INVOKABLE
    QList<QObject*> allCreator();

	Q_INVOKABLE
    void deleteCreator();

    // access from QML to list of all Creator
    QQmlListProperty<Creator> creatorPropertyList();

	Q_INVOKABLE
    Creator* createCreator();

	Q_INVOKABLE
    void undoCreateCreator(Creator* creator);

	Q_INVOKABLE
    void insertCreator(Creator* creator);

	Q_INVOKABLE
    void insertCreatorFromMap(const QVariantMap& creatorMap, const bool& useForeignProperties);

	Q_INVOKABLE
    bool deleteCreator(Creator* creator);
	

	Q_INVOKABLE
    bool deleteCreatorByCreatorId(const int& creatorId);

	Q_INVOKABLE
    Creator* findCreatorByCreatorId(const int& creatorId);

	
	
	Q_INVOKABLE
    QList<CreatorImage*> listOfCreatorImageForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList creatorImageAsQVariantList();

	Q_INVOKABLE
    QList<QObject*> allCreatorImage();

	Q_INVOKABLE
    void deleteCreatorImage();

    // access from QML to list of all CreatorImage
    QQmlListProperty<CreatorImage> creatorImagePropertyList();

	Q_INVOKABLE
    CreatorImage* createCreatorImage();

	Q_INVOKABLE
    void undoCreateCreatorImage(CreatorImage* creatorImage);

	Q_INVOKABLE
    void insertCreatorImage(CreatorImage* creatorImage);

	Q_INVOKABLE
    void insertCreatorImageFromMap(const QVariantMap& creatorImageMap, const bool& useForeignProperties);

	Q_INVOKABLE
    bool deleteCreatorImage(CreatorImage* creatorImage);
	

	Q_INVOKABLE
    bool deleteCreatorImageByCreatorId(const int& creatorId);

	Q_INVOKABLE
    CreatorImage* findCreatorImageByCreatorId(const int& creatorId);

	
	
	Q_INVOKABLE
	QList<KeyTag*> listOfKeyTagForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList keyTagAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allKeyTag();

	Q_INVOKABLE
	void deleteKeyTag();

	// access from QML to list of all KeyTag
	QQmlListProperty<KeyTag> keyTagPropertyList();

	Q_INVOKABLE
	KeyTag* createKeyTag();

	Q_INVOKABLE
	void undoCreateKeyTag(KeyTag* keyTag);

	Q_INVOKABLE
	void insertKeyTag(KeyTag* keyTag);

	Q_INVOKABLE
	void insertKeyTagFromMap(const QVariantMap& keyTagMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteKeyTag(KeyTag* keyTag);
	

	Q_INVOKABLE
	bool deleteKeyTagByTagId(const int& tagId);

	Q_INVOKABLE
    KeyTag* findKeyTagByTagId(const int& tagId);

	
	
    Q_INVOKABLE
    QList<KeyContentBlock*> listOfKeyContentBlockForKeys(QStringList keyList);

    Q_INVOKABLE
    QVariantList keyContentBlockAsQVariantList();

    Q_INVOKABLE
    QList<QObject*> allKeyContentBlock();

    Q_INVOKABLE
    void deleteKeyContentBlock();

    // access from QML to list of all KeyContentBlock
    QQmlListProperty<KeyContentBlock> keyContentBlockPropertyList();

    Q_INVOKABLE
    KeyContentBlock* createKeyContentBlock();

    Q_INVOKABLE
    void undoCreateKeyContentBlock(KeyContentBlock* keyContentBlock);

    Q_INVOKABLE
    void insertKeyContentBlock(KeyContentBlock* keyContentBlock);

    Q_INVOKABLE
    void insertKeyContentBlockFromMap(const QVariantMap& keyContentBlockMap, const bool& useForeignProperties);

    Q_INVOKABLE
    bool deleteKeyContentBlock(KeyContentBlock* keyContentBlock);


    Q_INVOKABLE
    bool deleteKeyContentBlockByContentBlockId(const int& contentBlockId);

    Q_INVOKABLE
    KeyContentBlock* findKeyContentBlockByContentBlockId(const int& contentBlockId);




    Q_INVOKABLE
    QList<KeyContentStandardType*> listOfKeyContentStandardTypeForKeys(QStringList keyList);

    Q_INVOKABLE
    QVariantList keyContentStandardTypeAsQVariantList();

    Q_INVOKABLE
    QList<QObject*> allKeyContentStandardType();

    Q_INVOKABLE
    void deleteKeyContentStandardType();

    // access from QML to list of all KeyContentStandardType
    QQmlListProperty<KeyContentStandardType> keyContentStandardTypePropertyList();

    Q_INVOKABLE
    KeyContentStandardType* createKeyContentStandardType();

    Q_INVOKABLE
    void undoCreateKeyContentStandardType(KeyContentStandardType* keyContentStandardType);

    Q_INVOKABLE
    void insertKeyContentStandardType(KeyContentStandardType* keyContentStandardType);

    Q_INVOKABLE
    void insertKeyContentStandardTypeFromMap(const QVariantMap& keyContentStandardTypeMap, const bool& useForeignProperties);

    Q_INVOKABLE
    bool deleteKeyContentStandardType(KeyContentStandardType* keyContentStandardType);


    Q_INVOKABLE
    bool deleteKeyContentStandardTypeByContentStandardTypeId(const int& contentStandardTypeId);

    Q_INVOKABLE
    KeyContentStandardType* findKeyContentStandardTypeByContentStandardTypeId(const int& contentStandardTypeId);



	Q_INVOKABLE
	QList<Day*> listOfDayForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList dayAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allDay();

	Q_INVOKABLE
	void deleteDay();

	// access from QML to list of all Day
	QQmlListProperty<Day> dayPropertyList();

	Q_INVOKABLE
	Day* createDay();

	Q_INVOKABLE
	void undoCreateDay(Day* day);

	Q_INVOKABLE
	void insertDay(Day* day);

	Q_INVOKABLE
	void insertDayFromMap(const QVariantMap& dayMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteDay(Day* day);
	

	Q_INVOKABLE
	bool deleteDayById(const int& id);

	Q_INVOKABLE
    Day* findDayById(const int& id);

	
	
	Q_INVOKABLE
	QList<KeyAPI*> listOfKeyAPIForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList keyAPIAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allKeyAPI();

	Q_INVOKABLE
	void deleteKeyAPI();

	// access from QML to list of all KeyAPI
	QQmlListProperty<KeyAPI> keyAPIPropertyList();

	Q_INVOKABLE
	KeyAPI* createKeyAPI();

	Q_INVOKABLE
	void undoCreateKeyAPI(KeyAPI* keyAPI);

	Q_INVOKABLE
	void insertKeyAPI(KeyAPI* keyAPI);

	Q_INVOKABLE
	void insertKeyAPIFromMap(const QVariantMap& keyAPIMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteKeyAPI(KeyAPI* keyAPI);
	

	Q_INVOKABLE
	bool deleteKeyAPIByKeyId(const int& keyId);

	Q_INVOKABLE
    KeyAPI* findKeyAPIByKeyId(const int& keyId);

	
	
	Q_INVOKABLE
	QList<PersonsAPI*> listOfPersonsAPIForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList creatorsAPIAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allPersonsAPI();

	Q_INVOKABLE
	void deletePersonsAPI();

	// access from QML to list of all PersonsAPI
	QQmlListProperty<PersonsAPI> creatorsAPIPropertyList();

	Q_INVOKABLE
	PersonsAPI* createPersonsAPI();

	Q_INVOKABLE
	void undoCreatePersonsAPI(PersonsAPI* creatorsAPI);

	Q_INVOKABLE
	void insertPersonsAPI(PersonsAPI* creatorsAPI);

	Q_INVOKABLE
	void insertPersonsAPIFromMap(const QVariantMap& creatorsAPIMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deletePersonsAPI(PersonsAPI* creatorsAPI);
	

	Q_INVOKABLE
    bool deletePersonsAPIByCreatorId(const int& creatorId);

	Q_INVOKABLE
    PersonsAPI* findPersonsAPIByCreatorId(const int& creatorId);

	
	
	Q_INVOKABLE
	QList<KeyLinkAPI*> listOfKeyLinkAPIForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList keyLinkAPIAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allKeyLinkAPI();

	Q_INVOKABLE
	void deleteKeyLinkAPI();

	// access from QML to list of all KeyLinkAPI
	QQmlListProperty<KeyLinkAPI> keyLinkAPIPropertyList();

	Q_INVOKABLE
	KeyLinkAPI* createKeyLinkAPI();

	Q_INVOKABLE
	void undoCreateKeyLinkAPI(KeyLinkAPI* keyLinkAPI);

	Q_INVOKABLE
	void insertKeyLinkAPI(KeyLinkAPI* keyLinkAPI);

	Q_INVOKABLE
	void insertKeyLinkAPIFromMap(const QVariantMap& keyLinkAPIMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteKeyLinkAPI(KeyLinkAPI* keyLinkAPI);
	

	Q_INVOKABLE
	bool deleteKeyLinkAPIByUuid(const QString& uuid);

	Q_INVOKABLE
	KeyLinkAPI* findKeyLinkAPIByUuid(const QString& uuid);
	
	
	Q_INVOKABLE
    QList<CreatorAPI*> listOfCreatorAPIForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList creatorAPIAsQVariantList();

	Q_INVOKABLE
    QList<QObject*> allCreatorAPI();

	Q_INVOKABLE
    void deleteCreatorAPI();

    // access from QML to list of all CreatorAPI
    QQmlListProperty<CreatorAPI> creatorAPIPropertyList();

	Q_INVOKABLE
    CreatorAPI* createCreatorAPI();

	Q_INVOKABLE
    void undoCreateCreatorAPI(CreatorAPI* creatorAPI);

	Q_INVOKABLE
    void insertCreatorAPI(CreatorAPI* creatorAPI);

	Q_INVOKABLE
    void insertCreatorAPIFromMap(const QVariantMap& creatorAPIMap, const bool& useForeignProperties);

	Q_INVOKABLE
    bool deleteCreatorAPI(CreatorAPI* creatorAPI);
	

	Q_INVOKABLE
    bool deleteCreatorAPIById(const int& id);

	Q_INVOKABLE
    CreatorAPI* findCreatorAPIById(const int& id);



    void initRepositoryFromCache();
    void initDictionaryFromCache();
    void initCategoryFromCache();
    void initSectionFromCache();
    void initKeyFromCache();
    void initDictionaryItemFromCache();
    void initFavoriteFromCache();
    void initBookmarkFromCache();
    void initKeyListsFromCache();
    void initKeyLinkFromCache();
    void initCreatorFromCache();
    void initCreatorImageFromCache();
    void initKeyTagFromCache();
    void initKeyContentBlockFromCache();
    void initKeyContentStandardTypeFromCache();
    void initDayFromCache();
    void initKeyAPIFromCache();
    void initPersonsAPIFromCache();
    void initKeyLinkAPIFromCache();
    void initCreatorAPIFromCache();

	Q_INVOKABLE
	SettingsData* settingsData();
	
	void finish();

Q_SIGNALS:

	void shuttingDown();
	void addedToAllRepository(Repository* repository);
	void deletedFromAllRepositoryById(int id);
	void deletedFromAllRepository(Repository* repository);
	void repositoryPropertyListChanged();
	void addedToAllDictionary(Dictionary* dictionary);
	void deletedFromAllDictionaryByDictionaryId(int dictionaryId);
	void deletedFromAllDictionary(Dictionary* dictionary);
	void dictionaryPropertyListChanged();
	void addedToAllCategory(Category* category);
	void deletedFromAllCategoryByCategoryId(int categoryId);
	void deletedFromAllCategory(Category* category);
	void categoryPropertyListChanged();
	void addedToAllSection(Section* section);
	void deletedFromAllSectionBySectionId(int sectionId);
	void deletedFromAllSection(Section* section);
	void sectionPropertyListChanged();
	void addedToAllKey(Key* key);
	void deletedFromAllKeyByKeyId(int keyId);
	void deletedFromAllKey(Key* key);
	void keyPropertyListChanged();
	void addedToAllDictionaryItem(DictionaryItem* dictionaryItem);
	void deletedFromAllDictionaryItemByKeyId(int keyId);
	void deletedFromAllDictionaryItem(DictionaryItem* dictionaryItem);
	void dictionaryItemPropertyListChanged();
	void addedToAllFavorite(Favorite* favorite);
	void deletedFromAllFavoriteByKeyId(int keyId);
	void deletedFromAllFavorite(Favorite* favorite);
	void favoritePropertyListChanged();
	void addedToAllBookmark(Bookmark* bookmark);
	void deletedFromAllBookmarkByKeyId(int keyId);
	void deletedFromAllBookmark(Bookmark* bookmark);
	void bookmarkPropertyListChanged();
	void addedToAllKeyLists(KeyLists* keyLists);
	void deletedFromAllKeyListsByUuid(QString uuid);
	void deletedFromAllKeyLists(KeyLists* keyLists);
	void keyListsPropertyListChanged();
	void addedToAllKeyLink(KeyLink* keyLink);
	void deletedFromAllKeyLinkByUuid(QString uuid);
	void deletedFromAllKeyLink(KeyLink* keyLink);
	void keyLinkPropertyListChanged();
    void addedToAllCreator(Creator* creator);
    void deletedFromAllCreatorByCreatorId(int creatorId);
    void deletedFromAllCreator(Creator* creator);
	void creatorPropertyListChanged();
    void addedToAllCreatorImage(CreatorImage* creatorImage);
    void deletedFromAllCreatorImageByCreatorId(int creatorId);
    void deletedFromAllCreatorImage(CreatorImage* creatorImage);
	void creatorImagePropertyListChanged();
	void addedToAllKeyTag(KeyTag* keyTag);
	void deletedFromAllKeyTagByTagId(int tagId);
	void deletedFromAllKeyTag(KeyTag* keyTag);
	void keyTagPropertyListChanged();
    void addedToAllKeyContentBlock(KeyContentBlock* keyContentBlock);
    void deletedFromAllKeyContentBlockByContentBlockId(int contentBlockId);
    void deletedFromAllKeyContentBlock(KeyContentBlock* keyContentBlock);
    void keyContentBlockPropertyListChanged();
    void addedToAllKeyContentStandardType(KeyContentStandardType* keyContentStandardType);
    void deletedFromAllKeyContentStandardTypeByContentStandardTypeId(int contentStandardTypeId);
    void deletedFromAllKeyContentStandardType(KeyContentStandardType* keyContentStandardType);
    void keyContentStandardTypePropertyListChanged();
	void addedToAllDay(Day* day);
	void deletedFromAllDayById(int id);
	void deletedFromAllDay(Day* day);
	void dayPropertyListChanged();
	void addedToAllKeyAPI(KeyAPI* keyAPI);
	void deletedFromAllKeyAPIByKeyId(int keyId);
	void deletedFromAllKeyAPI(KeyAPI* keyAPI);
	void keyAPIPropertyListChanged();
	void addedToAllPersonsAPI(PersonsAPI* creatorsAPI);
    void deletedFromAllPersonsAPIByCreatorId(int creatorId);
	void deletedFromAllPersonsAPI(PersonsAPI* creatorsAPI);
	void creatorsAPIPropertyListChanged();
	void addedToAllKeyLinkAPI(KeyLinkAPI* keyLinkAPI);
	void deletedFromAllKeyLinkAPIByUuid(QString uuid);
	void deletedFromAllKeyLinkAPI(KeyLinkAPI* keyLinkAPI);
	void keyLinkAPIPropertyListChanged();
    void addedToAllCreatorAPI(CreatorAPI* creatorAPI);
    void deletedFromAllCreatorAPIById(int id);
    void deletedFromAllCreatorAPI(CreatorAPI* creatorAPI);
	void creatorAPIPropertyListChanged();


private:
	QString mDataRoot;
	QString mDataPath;
	QString mSettingsPath;
	QString mDataAssetsPath;
	QString dataAssetsPath(const QString& fileName);
	QString dataPath(const QString& fileName);

	SettingsData* mSettingsData;
	void readSettings();
	void saveSettings();
	bool mCompactJson;
	// DataObject stored in List of QObject*
	// GroupDataModel only supports QObject*
    	
    QList<QObject*> mAllRepository;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Repository*
    static void appendToRepositoryProperty(
    	QQmlListProperty<Repository> *repositoryList,
    	Repository* repository);
    static int repositoryPropertyCount(
    	QQmlListProperty<Repository> *repositoryList);
    static Repository* atRepositoryProperty(
    	QQmlListProperty<Repository> *repositoryList, int pos);
    static void clearRepositoryProperty(
    	QQmlListProperty<Repository> *repositoryList);
    	
    QList<QObject*> mAllDictionary;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Dictionary*
    static void appendToDictionaryProperty(
    	QQmlListProperty<Dictionary> *dictionaryList,
    	Dictionary* dictionary);
    static int dictionaryPropertyCount(
    	QQmlListProperty<Dictionary> *dictionaryList);
    static Dictionary* atDictionaryProperty(
    	QQmlListProperty<Dictionary> *dictionaryList, int pos);
    static void clearDictionaryProperty(
    	QQmlListProperty<Dictionary> *dictionaryList);
    	
    QList<QObject*> mAllCategory;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Category*
    static void appendToCategoryProperty(
    	QQmlListProperty<Category> *categoryList,
    	Category* category);
    static int categoryPropertyCount(
    	QQmlListProperty<Category> *categoryList);
    static Category* atCategoryProperty(
    	QQmlListProperty<Category> *categoryList, int pos);
    static void clearCategoryProperty(
    	QQmlListProperty<Category> *categoryList);
    	
    QList<QObject*> mAllSection;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Section*
    static void appendToSectionProperty(
    	QQmlListProperty<Section> *sectionList,
    	Section* section);
    static int sectionPropertyCount(
    	QQmlListProperty<Section> *sectionList);
    static Section* atSectionProperty(
    	QQmlListProperty<Section> *sectionList, int pos);
    static void clearSectionProperty(
    	QQmlListProperty<Section> *sectionList);
    	
    QList<QObject*> mAllKey;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Key*
    static void appendToKeyProperty(
    	QQmlListProperty<Key> *keyList,
    	Key* key);
    static int keyPropertyCount(
    	QQmlListProperty<Key> *keyList);
    static Key* atKeyProperty(
    	QQmlListProperty<Key> *keyList, int pos);
    static void clearKeyProperty(
    	QQmlListProperty<Key> *keyList);
    	
    QList<QObject*> mAllDictionaryItem;
    // implementation for QQmlListProperty to use
    // QML functions for List of All DictionaryItem*
    static void appendToDictionaryItemProperty(
    	QQmlListProperty<DictionaryItem> *dictionaryItemList,
    	DictionaryItem* dictionaryItem);
    static int dictionaryItemPropertyCount(
    	QQmlListProperty<DictionaryItem> *dictionaryItemList);
    static DictionaryItem* atDictionaryItemProperty(
    	QQmlListProperty<DictionaryItem> *dictionaryItemList, int pos);
    static void clearDictionaryItemProperty(
    	QQmlListProperty<DictionaryItem> *dictionaryItemList);
    	
    QList<QObject*> mAllFavorite;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Favorite*
    static void appendToFavoriteProperty(
    	QQmlListProperty<Favorite> *favoriteList,
    	Favorite* favorite);
    static int favoritePropertyCount(
    	QQmlListProperty<Favorite> *favoriteList);
    static Favorite* atFavoriteProperty(
    	QQmlListProperty<Favorite> *favoriteList, int pos);
    static void clearFavoriteProperty(
    	QQmlListProperty<Favorite> *favoriteList);
    	
    QList<QObject*> mAllBookmark;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Bookmark*
    static void appendToBookmarkProperty(
    	QQmlListProperty<Bookmark> *bookmarkList,
    	Bookmark* bookmark);
    static int bookmarkPropertyCount(
    	QQmlListProperty<Bookmark> *bookmarkList);
    static Bookmark* atBookmarkProperty(
    	QQmlListProperty<Bookmark> *bookmarkList, int pos);
    static void clearBookmarkProperty(
    	QQmlListProperty<Bookmark> *bookmarkList);
    	
    QList<QObject*> mAllKeyLists;
    // implementation for QQmlListProperty to use
    // QML functions for List of All KeyLists*
    static void appendToKeyListsProperty(
    	QQmlListProperty<KeyLists> *keyListsList,
    	KeyLists* keyLists);
    static int keyListsPropertyCount(
    	QQmlListProperty<KeyLists> *keyListsList);
    static KeyLists* atKeyListsProperty(
    	QQmlListProperty<KeyLists> *keyListsList, int pos);
    static void clearKeyListsProperty(
    	QQmlListProperty<KeyLists> *keyListsList);
    	
    QList<QObject*> mAllKeyLink;
    // implementation for QQmlListProperty to use
    // QML functions for List of All KeyLink*
    static void appendToKeyLinkProperty(
    	QQmlListProperty<KeyLink> *keyLinkList,
    	KeyLink* keyLink);
    static int keyLinkPropertyCount(
    	QQmlListProperty<KeyLink> *keyLinkList);
    static KeyLink* atKeyLinkProperty(
    	QQmlListProperty<KeyLink> *keyLinkList, int pos);
    static void clearKeyLinkProperty(
    	QQmlListProperty<KeyLink> *keyLinkList);
    	
    QList<QObject*> mAllCreator;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Creator*
    static void appendToCreatorProperty(
        QQmlListProperty<Creator> *creatorList,
        Creator* creator);
    static int creatorPropertyCount(
        QQmlListProperty<Creator> *creatorList);
    static Creator* atCreatorProperty(
        QQmlListProperty<Creator> *creatorList, int pos);
    static void clearCreatorProperty(
        QQmlListProperty<Creator> *creatorList);
    	
    QList<QObject*> mAllCreatorImage;
    // implementation for QQmlListProperty to use
    // QML functions for List of All CreatorImage*
    static void appendToCreatorImageProperty(
        QQmlListProperty<CreatorImage> *creatorImageList,
        CreatorImage* creatorImage);
    static int creatorImagePropertyCount(
        QQmlListProperty<CreatorImage> *creatorImageList);
    static CreatorImage* atCreatorImageProperty(
        QQmlListProperty<CreatorImage> *creatorImageList, int pos);
    static void clearCreatorImageProperty(
        QQmlListProperty<CreatorImage> *creatorImageList);
    	
    QList<QObject*> mAllKeyTag;
    // implementation for QQmlListProperty to use
    // QML functions for List of All KeyTag*
    static void appendToKeyTagProperty(
    	QQmlListProperty<KeyTag> *keyTagList,
    	KeyTag* keyTag);
    static int keyTagPropertyCount(
    	QQmlListProperty<KeyTag> *keyTagList);
    static KeyTag* atKeyTagProperty(
    	QQmlListProperty<KeyTag> *keyTagList, int pos);
    static void clearKeyTagProperty(
    	QQmlListProperty<KeyTag> *keyTagList);
    	

    QList<QObject*> mAllKeyContentBlock;
    // implementation for QQmlListProperty to use
    // QML functions for List of All KeyContentBlock*
    static void appendToKeyContentBlockProperty(
        QQmlListProperty<KeyContentBlock> *keyContentBlockList,
        KeyContentBlock* keyContentBlock);
    static int keyContentBlockPropertyCount(
        QQmlListProperty<KeyContentBlock> *keyContentBlockList);
    static KeyContentBlock* atKeyContentBlockProperty(
        QQmlListProperty<KeyContentBlock> *keyContentBlockList, int pos);
    static void clearKeyContentBlockProperty(
        QQmlListProperty<KeyContentBlock> *keyContentBlockList);


    QList<QObject*> mAllKeyContentStandardType;
    // implementation for QQmlListProperty to use
    // QML functions for List of All KeyContentStandardType*
    static void appendToKeyContentStandardTypeProperty(
        QQmlListProperty<KeyContentStandardType> *keyContentStandardTypeList,
        KeyContentStandardType* keyContentStandardType);
    static int keyContentStandardTypePropertyCount(
        QQmlListProperty<KeyContentStandardType> *keyContentStandardTypeList);
    static KeyContentStandardType* atKeyContentStandardTypeProperty(
        QQmlListProperty<KeyContentStandardType> *keyContentStandardTypeList, int pos);
    static void clearKeyContentStandardTypeProperty(
        QQmlListProperty<KeyContentStandardType> *keyContentStandardTypeList);


    QList<QObject*> mAllDay;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Day*
    static void appendToDayProperty(
    	QQmlListProperty<Day> *levelList,
    	Day* day);
    static int dayPropertyCount(
    	QQmlListProperty<Day> *levelList);
    static Day* atDayProperty(
    	QQmlListProperty<Day> *levelList, int pos);
    static void clearDayProperty(
    	QQmlListProperty<Day> *levelList);
    	
    QList<QObject*> mAllKeyAPI;
    // implementation for QQmlListProperty to use
    // QML functions for List of All KeyAPI*
    static void appendToKeyAPIProperty(
    	QQmlListProperty<KeyAPI> *keyAPIList,
    	KeyAPI* keyAPI);
    static int keyAPIPropertyCount(
    	QQmlListProperty<KeyAPI> *keyAPIList);
    static KeyAPI* atKeyAPIProperty(
    	QQmlListProperty<KeyAPI> *keyAPIList, int pos);
    static void clearKeyAPIProperty(
    	QQmlListProperty<KeyAPI> *keyAPIList);
    	
    QList<QObject*> mAllPersonsAPI;
    // implementation for QQmlListProperty to use
    // QML functions for List of All PersonsAPI*
    static void appendToPersonsAPIProperty(
    	QQmlListProperty<PersonsAPI> *creatorsAPIList,
    	PersonsAPI* creatorsAPI);
    static int creatorsAPIPropertyCount(
    	QQmlListProperty<PersonsAPI> *creatorsAPIList);
    static PersonsAPI* atPersonsAPIProperty(
    	QQmlListProperty<PersonsAPI> *creatorsAPIList, int pos);
    static void clearPersonsAPIProperty(
    	QQmlListProperty<PersonsAPI> *creatorsAPIList);
    	
    QList<QObject*> mAllKeyLinkAPI;
    // implementation for QQmlListProperty to use
    // QML functions for List of All KeyLinkAPI*
    static void appendToKeyLinkAPIProperty(
    	QQmlListProperty<KeyLinkAPI> *keyLinkAPIList,
    	KeyLinkAPI* keyLinkAPI);
    static int keyLinkAPIPropertyCount(
    	QQmlListProperty<KeyLinkAPI> *keyLinkAPIList);
    static KeyLinkAPI* atKeyLinkAPIProperty(
    	QQmlListProperty<KeyLinkAPI> *keyLinkAPIList, int pos);
    static void clearKeyLinkAPIProperty(
    	QQmlListProperty<KeyLinkAPI> *keyLinkAPIList);
    	
    QList<QObject*> mAllCreatorAPI;
    // implementation for QQmlListProperty to use
    // QML functions for List of All CreatorAPI*
    static void appendToCreatorAPIProperty(
        QQmlListProperty<CreatorAPI> *creatorAPIList,
        CreatorAPI* creatorAPI);
    static int creatorAPIPropertyCount(
        QQmlListProperty<CreatorAPI> *creatorAPIList);
    static CreatorAPI* atCreatorAPIProperty(
        QQmlListProperty<CreatorAPI> *creatorAPIList, int pos);
    static void clearCreatorAPIProperty(
        QQmlListProperty<CreatorAPI> *creatorAPIList);
    	

    void saveRepositoryToCache();
    void saveDictionaryToCache();
    void saveCategoryToCache();
    void saveSectionToCache();
    void saveKeyToCache();
    void saveDictionaryItemToCache();
    void saveFavoriteToCache();
    void saveBookmarkToCache();
    void saveKeyListsToCache();
    void saveKeyLinkToCache();
    void saveCreatorToCache();
    void saveCreatorImageToCache();
    void saveKeyTagToCache();
    void saveKeyContentBlockToCache();
    void saveKeyContentStandardTypeToCache();
    void saveDayToCache();
    void saveKeyAPIToCache();
    void savePersonsAPIToCache();
    void saveKeyLinkAPIToCache();
    void saveCreatorAPIToCache();


	QVariantList readFromCache(const QString& fileName);
	void writeToCache(const QString& fileName, QVariantList& data);
};

#endif /* DATAMANAGER_HPP_ */
