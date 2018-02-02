#ifndef KEY_HPP_
#define KEY_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>
#include <QTime>


// forward declaration (target references to this)
class Creator;
#include "KeyLink.hpp"
// forward declaration (target references to this)
class KeyTag;
// forward declaration (target references to this)
class KeyContentBlock;
// forward declaration (target references to this)
class KeyContentStandardType;
// forward declaration (target references to this)
class Day;
// forward declaration (target references to this)
class Section;
// forward declaration (target references to this)
class DictionaryItem;


class Key: public QObject
{
	Q_OBJECT

    Q_PROPERTY(int keyId READ keyId WRITE setKeyId NOTIFY keyIdChanged FINAL)
    Q_PROPERTY(int keyIdRefView READ keyIdRefView WRITE setKeyIdRefView NOTIFY keyIdRefViewChanged FINAL)
    Q_PROPERTY(bool isBoolean2 READ isBoolean2 WRITE setIsBoolean2 NOTIFY isBoolean2Changed FINAL)
	Q_PROPERTY(QString sortKey READ sortKey WRITE setSortKey NOTIFY sortKeyChanged FINAL)
    Q_PROPERTY(bool isBoolean7 READ isBoolean7 WRITE setIsBoolean7 NOTIFY isBoolean7Changed FINAL)
    Q_PROPERTY(bool isBoolean4 READ isBoolean4 WRITE setIsBoolean4 NOTIFY isBoolean4Changed FINAL)
    Q_PROPERTY(bool isBoolean3 READ isBoolean3 WRITE setIsBoolean3 NOTIFY isBoolean3Changed FINAL)
    Q_PROPERTY(bool isBoolean6 READ isBoolean6 WRITE setIsBoolean6 NOTIFY isBoolean6Changed FINAL)
    Q_PROPERTY(bool isRefView READ isRefView WRITE setIsRefView NOTIFY isRefViewChanged FINAL)
    Q_PROPERTY(bool isBoolean8 READ isBoolean8 WRITE setIsBoolean8 NOTIFY isBoolean8Changed FINAL)
    Q_PROPERTY(bool isBoolean5 READ isBoolean5 WRITE setIsBoolean5 NOTIFY isBoolean5Changed FINAL)
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
	Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY subtitleChanged FINAL)
	Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged FINAL)
    Q_PROPERTY(QString keyType READ keyType WRITE setKeyType NOTIFY keyTypeChanged FINAL)
    Q_PROPERTY(QString contentStandardName READ contentStandardName WRITE setContentStandardName NOTIFY contentStandardNameChanged FINAL)
    Q_PROPERTY(QString contentArticulo1 READ contentArticulo1 WRITE setContentArticulo1 NOTIFY contentArticulo1Changed FINAL)
    Q_PROPERTY(QString contentSituacion1 READ contentSituacion1 WRITE setContentSituacion1 NOTIFY contentSituacion1Changed FINAL)
    Q_PROPERTY(QString contentLink READ contentLink WRITE setContentLink NOTIFY contentLinkChanged FINAL)
    Q_PROPERTY(QTime startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged FINAL)
	Q_PROPERTY(QTime endTime READ endTime WRITE setEndTime NOTIFY endTimeChanged FINAL)
	Q_PROPERTY(int minutes READ minutes WRITE setMinutes NOTIFY minutesChanged FINAL)
	Q_PROPERTY(bool isFavorite READ isFavorite WRITE setIsFavorite NOTIFY isFavoriteChanged FINAL)
	Q_PROPERTY(bool isBookmarked READ isBookmarked WRITE setIsBookmarked NOTIFY isBookmarkedChanged FINAL)
	// keyDay lazy pointing to Day* (domainKey: id)
    Q_PROPERTY(int keyDay READ keyDay WRITE setKeyDay NOTIFY keyDayChanged FINAL)
    Q_PROPERTY(Day* keyDayAsDataObject READ keyDayAsDataObject WRITE resolveKeyDayAsDataObject NOTIFY keyDayAsDataObjectChanged FINAL)
    // section lazy pointing to Section* (domainKey: sectionId)
    Q_PROPERTY(int section READ section WRITE setSection NOTIFY sectionChanged FINAL)
    Q_PROPERTY(Section* sectionAsDataObject READ sectionAsDataObject WRITE resolveSectionAsDataObject NOTIFY sectionAsDataObjectChanged FINAL)
	// dictionaryItem lazy pointing to DictionaryItem* (domainKey: keyId)
	Q_PROPERTY(int dictionaryItem READ dictionaryItem WRITE setDictionaryItem NOTIFY dictionaryItemChanged FINAL)
	Q_PROPERTY(DictionaryItem* dictionaryItemAsDataObject READ dictionaryItemAsDataObject WRITE resolveDictionaryItemAsDataObject NOTIFY dictionaryItemAsDataObjectChanged FINAL)

	// QQmlListProperty to get easy access from QML
    Q_PROPERTY(QQmlListProperty<Creator> creatorPropertyList READ creatorPropertyList NOTIFY creatorPropertyListChanged)
	// QQmlListProperty to get easy access from QML
    Q_PROPERTY(QQmlListProperty<KeyLink> keyLinksPropertyList READ keyLinksPropertyList NOTIFY keyLinksPropertyListChanged)
	// QQmlListProperty to get easy access from QML
    Q_PROPERTY(QQmlListProperty<KeyTag> keyTagsPropertyList READ keyTagsPropertyList NOTIFY keyTagsPropertyListChanged)
    // QQmlListProperty to get easy access from QML
    Q_PROPERTY(QQmlListProperty<KeyContentBlock> keyContentBlocksPropertyList READ keyContentBlocksPropertyList NOTIFY keyContentBlocksPropertyListChanged)
    // QQmlListProperty to get easy access from QML
    Q_PROPERTY(QQmlListProperty<KeyContentStandardType> keyContentStandardTypesPropertyList READ keyContentStandardTypesPropertyList NOTIFY keyContentStandardTypesPropertyListChanged)

public:
    Key(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& keyMap);
	void fillFromForeignMap(const QVariantMap& keyMap);
	void fillFromCacheMap(const QVariantMap& keyMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int keyId() const;
    void setKeyId(int keyId);
    int keyIdRefView() const;
    void setKeyIdRefView(int keyIdRefView);
	QString sortKey() const;
	void setSortKey(QString sortKey);

    bool isRefView() const;
    void setIsRefView(bool isRefView);
    bool isBoolean2() const;
    void setIsBoolean2(bool isBoolean2);
    bool isBoolean3() const;
    void setIsBoolean3(bool isBoolean3);
    bool isBoolean4() const;
    void setIsBoolean4(bool isBoolean4);
    bool isBoolean5() const;
    void setIsBoolean5(bool isBoolean5);
	bool isBoolean6() const;
    void setIsBoolean6(bool isBoolean6);
    bool isBoolean7() const;
    void setIsBoolean7(bool isBoolean7);
    bool isBoolean8() const;
    void setIsBoolean8(bool isBoolean8);

	QString title() const;
	void setTitle(QString title);
	QString subtitle() const;
	void setSubtitle(QString subtitle);
	QString description() const;
	void setDescription(QString description);
	QString keyType() const;
    QString contentStandardName() const;
    void setContentStandardName(QString contentStandardName);
    QString contentArticulo1() const;
    void setContentArticulo1(QString contentArticulo1);
    QString contentSituacion1() const;
    void setContentSituacion1(QString contentSituacion1);

    QString contentLink() const;
    void setContentLink(QString contentLink);

    void setKeyType(QString keyType);
	QTime startTime() const;

	Q_INVOKABLE
	bool hasStartTime();

	Q_INVOKABLE
	void setStartTimeFromPickerValue(QString startTimeValue);
	void setStartTime(QTime startTime);
	QTime endTime() const;

	Q_INVOKABLE
	bool hasEndTime();

	Q_INVOKABLE
	void setEndTimeFromPickerValue(QString endTimeValue);
	void setEndTime(QTime endTime);
	int minutes() const;
	void setMinutes(int minutes);
	bool isFavorite() const;
	void setIsFavorite(bool isFavorite);
	bool isBookmarked() const;
	void setIsBookmarked(bool isBookmarked);
	// keyDay lazy pointing to Day* (domainKey: id)
	int keyDay() const;
    void setKeyDay(int keyDay);
	Day* keyDayAsDataObject() const;
	
	Q_INVOKABLE
    void resolveKeyDayAsDataObject(Day* day);
	
	Q_INVOKABLE
    void removeKeyDay();
	
	Q_INVOKABLE
    bool hasKeyDay();
	
	Q_INVOKABLE
    bool isKeyDayResolvedAsDataObject();
	
	Q_INVOKABLE
    void markKeyDayAsInvalid();
	
    // section lazy pointing to Section* (domainKey: sectionId)
    int section() const;
    void setSection(int section);
    Section* sectionAsDataObject() const;
	
	Q_INVOKABLE
    void resolveSectionAsDataObject(Section* section);
	
	Q_INVOKABLE
	void removeSection();
	
	Q_INVOKABLE
	bool hasSection();
	
	Q_INVOKABLE
	bool isSectionResolvedAsDataObject();
	
	Q_INVOKABLE
	void markSectionAsInvalid();
	
	// dictionaryItem lazy pointing to DictionaryItem* (domainKey: keyId)
	int dictionaryItem() const;
	void setDictionaryItem(int dictionaryItem);
    DictionaryItem* dictionaryItemAsDataObject() const;
	
	Q_INVOKABLE
	void resolveDictionaryItemAsDataObject(DictionaryItem* dictionaryItem);
	
	Q_INVOKABLE
	void removeDictionaryItem();
	
	Q_INVOKABLE
	bool hasDictionaryItem();
	
	Q_INVOKABLE
	bool isDictionaryItemResolvedAsDataObject();
	
	Q_INVOKABLE
	void markDictionaryItemAsInvalid();
	

	
	Q_INVOKABLE
	QVariantList creatorAsQVariantList();
	
	Q_INVOKABLE
	QVariantList creatorAsForeignQVariantList();

	
	Q_INVOKABLE
    void addToCreator(Creator* creator);
	
	Q_INVOKABLE
    bool removeFromCreator(Creator* creator);

	Q_INVOKABLE
	void clearCreator();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areCreatorKeysResolved();

	Q_INVOKABLE
	QStringList creatorKeys();

	Q_INVOKABLE
    void resolveCreatorKeys(QList<Creator*> creator);
	
	Q_INVOKABLE
	int creatorCount();
	
	 // access from C++ to creator
    QList<Creator*> creator();
    void setCreator(QList<Creator*> creator);
	// access from QML to creator
    QQmlListProperty<Creator> creatorPropertyList();
	
	Q_INVOKABLE
	QVariantList keyLinksAsQVariantList();
	
	Q_INVOKABLE
	QVariantList keyLinksAsForeignQVariantList();

	
	Q_INVOKABLE
    void addToKeyLinks(KeyLink* keyLink);
	
	Q_INVOKABLE
    bool removeFromKeyLinks(KeyLink* keyLink);

	Q_INVOKABLE
    void clearKeyLinks();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
    bool areKeyLinksKeysResolved();

	Q_INVOKABLE
	QStringList keyLinksKeys();

	Q_INVOKABLE
    void resolveKeyLinksKeys(QList<KeyLink*> keyLinks);
	
	Q_INVOKABLE
	int keyLinksCount();
	
	 // access from C++ to keyLinks
    QList<KeyLink*> keyLinks();
    void setKeyLinks(QList<KeyLink*> keyLinks);
	// access from QML to keyLinks
    QQmlListProperty<KeyLink> keyLinksPropertyList();
	
	Q_INVOKABLE
	QVariantList keyTagsAsQVariantList();
	
	Q_INVOKABLE
	QVariantList keyTagsAsForeignQVariantList();

	
	Q_INVOKABLE
    void addToKeyTags(KeyTag* keyTag);
	
	Q_INVOKABLE
    bool removeFromKeyTags(KeyTag* keyTag);

	Q_INVOKABLE
    void clearKeyTags();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
    bool areKeyTagsKeysResolved();

	Q_INVOKABLE
	QStringList keyTagsKeys();

	Q_INVOKABLE
    void resolveKeyTagsKeys(QList<KeyTag*> keyTags);
	
	Q_INVOKABLE
	int keyTagsCount();
	
	 // access from C++ to keyTags
    QList<KeyTag*> keyTags();
    void setKeyTags(QList<KeyTag*> keyTags);
	// access from QML to keyTags
    QQmlListProperty<KeyTag> keyTagsPropertyList();


    Q_INVOKABLE
    QVariantList keyContentBlocksAsQVariantList();

    Q_INVOKABLE
    QVariantList keyContentBlocksAsForeignQVariantList();


    Q_INVOKABLE
    void addToKeyContentBlocks(KeyContentBlock* keyContentBlock);

    Q_INVOKABLE
    bool removeFromKeyContentBlocks(KeyContentBlock* keyContentBlock);

    Q_INVOKABLE
    void clearKeyContentBlocks();

    // lazy Array of independent Data Objects: only keys are persisted
    Q_INVOKABLE
    bool areKeyContentBlocksKeysResolved();

    Q_INVOKABLE
    QStringList keyContentBlocksKeys();

    Q_INVOKABLE
    void resolveKeyContentBlocksKeys(QList<KeyContentBlock*> keyContentBlocks);

    Q_INVOKABLE
    int keyContentBlocksCount();

     // access from C++ to keyContentBlocks
    QList<KeyContentBlock*> keyContentBlocks();
    void setKeyContentBlocks(QList<KeyContentBlock*> keyContentBlocks);
    // access from QML to keyContentBlocks
    QQmlListProperty<KeyContentBlock> keyContentBlocksPropertyList();



    Q_INVOKABLE
    QVariantList keyContentStandardTypesAsQVariantList();

    Q_INVOKABLE
    QVariantList keyContentStandardTypesAsForeignQVariantList();


    Q_INVOKABLE
    void addToKeyContentStandardTypes(KeyContentStandardType* keyContentStandardType);

    Q_INVOKABLE
    bool removeFromKeyContentStandardTypes(KeyContentStandardType* keyContentStandardType);

    Q_INVOKABLE
    void clearKeyContentStandardTypes();

    // lazy Array of independent Data Objects: only keys are persisted
    Q_INVOKABLE
    bool areKeyContentStandardTypesKeysResolved();

    Q_INVOKABLE
    QStringList keyContentStandardTypesKeys();

    Q_INVOKABLE
    void resolveKeyContentStandardTypesKeys(QList<KeyContentStandardType*> keyContentStandardTypes);

    Q_INVOKABLE
    int keyContentStandardTypesCount();

     // access from C++ to keyContentStandardTypes
    QList<KeyContentStandardType*> keyContentStandardTypes();
    void setKeyContentStandardTypes(QList<KeyContentStandardType*> keyContentStandardTypes);
    // access from QML to keyContentStandardTypes
    QQmlListProperty<KeyContentStandardType> keyContentStandardTypesPropertyList();


    virtual ~Key();

	Q_SIGNALS:

	void keyIdChanged(int keyId);
    void keyIdRefViewChanged(int keyId);
	void sortKeyChanged(QString sortKey);

    void isBoolean2Changed(bool isBoolean2);
    void isBoolean7Changed(bool isBoolean7);
    void isBoolean4Changed(bool isBoolean4);
    void isBoolean3Changed(bool isBoolean3);
    void isBoolean6Changed(bool isBoolean6);
    void isRefViewChanged(bool isRefView);
    void isBoolean8Changed(bool isBoolean8);
    void isBoolean5Changed(bool isBoolean5);

	void titleChanged(QString title);
	void subtitleChanged(QString subtitle);
	void descriptionChanged(QString description);
	void keyTypeChanged(QString keyType);
    void contentStandardNameChanged(QString contentStandardName);
    void contentArticulo1Changed(QString contentArticulo1);
    void contentSituacion1Changed(QString contentSituacion1);
    void contentLinkChanged(QString contentLink);
    void startTimeChanged(QTime startTime);
	void endTimeChanged(QTime endTime);
	void minutesChanged(int minutes);
	void isFavoriteChanged(bool isFavorite);
	void isBookmarkedChanged(bool isBookmarked);
	// keyDay lazy pointing to Day* (domainKey: id)
	void keyDayChanged(int keyDay);
	void keyDayAsDataObjectChanged(Day* day);
    // section lazy pointing to Section* (domainKey: sectionId)
    void sectionChanged(int section);
    void sectionAsDataObjectChanged(Section* section);
	// dictionaryItem lazy pointing to DictionaryItem* (domainKey: keyId)
	void dictionaryItemChanged(int dictionaryItem);
	void dictionaryItemAsDataObjectChanged(DictionaryItem* dictionaryItem);
    void creatorChanged(QList<Creator*> creator);
    void addedToCreator(Creator* creator);
	void creatorPropertyListChanged();
	
    void keyLinksChanged(QList<KeyLink*> keyLinks);
    void addedToKeyLinks(KeyLink* keyLink);
	void keyLinksPropertyListChanged();
	
    void keyTagsChanged(QList<KeyTag*> keyTags);
    void addedToKeyTags(KeyTag* keyTag);
	void keyTagsPropertyListChanged();
	
    void keyContentBlocksChanged(QList<KeyContentBlock*> keyContentBlocks);
    void addedToKeyContentBlocks(KeyContentBlock* keyContentBlock);
    void keyContentBlocksPropertyListChanged();

    void keyContentStandardTypesChanged(QList<KeyContentStandardType*> keyContentStandardTypes);
    void addedToKeyContentStandardTypes(KeyContentStandardType* keyContentStandardType);
    void keyContentStandardTypesPropertyListChanged();


private:

    int mKeyId;
    int mKeyIdRefView;
    bool mIsBoolean2;
	QString mSortKey;
    bool mIsBoolean7;
    bool mIsBoolean4;
    bool mIsBoolean3;
    bool mIsBoolean6;
    bool mIsRefView;
    bool mIsBoolean8;
    bool mIsBoolean5;
	QString mTitle;
	QString mSubtitle;
	QString mDescription;
    QString mKeyType;
    QString mContentStandardName;
    QString mContentArticulo1;
    QString mContentSituacion1;
    QString mContentLink;
    QTime mStartTime;
	QTime mEndTime;
	int mMinutes;
	bool mIsFavorite;
	bool mIsBookmarked;
    int mKeyDay;
    bool mKeyDayInvalid;
    Day* mKeyDayAsDataObject;
	int mContentSection;
	bool mContentSectionInvalid;
	Section* mContentSectionAsDataObject;
	int mDictionaryItem;
	bool mDictionaryItemInvalid;
    DictionaryItem* mDictionaryItemAsDataObject;
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mCreatorKeys;
	bool mCreatorKeysResolved;
    QList<Creator*> mCreator;
	// implementation for QQmlListProperty to use
    // QML functions for List of Creator*
    static void appendToCreatorProperty(QQmlListProperty<Creator> *creatorList,
        Creator* creator);
    static int creatorPropertyCount(QQmlListProperty<Creator> *creatorList);
    static Creator* atCreatorProperty(QQmlListProperty<Creator> *creatorList, int pos);
    static void clearCreatorProperty(QQmlListProperty<Creator> *creatorList);
	
	// lazy Array of independent Data Objects: only keys are persisted
    QStringList mKeyLinksKeys;
    bool mKeyLinksKeysResolved;
    QList<KeyLink*> mKeyLinks;
	// implementation for QQmlListProperty to use
    // QML functions for List of KeyLink*
    static void appendToKeyLinksProperty(QQmlListProperty<KeyLink> *keyLinksList,
        KeyLink* keyLink);
    static int keyLinksPropertyCount(QQmlListProperty<KeyLink> *keyLinksList);
    static KeyLink* atKeyLinksProperty(QQmlListProperty<KeyLink> *keyLinksList, int pos);
    static void clearKeyLinksProperty(QQmlListProperty<KeyLink> *keyLinksList);
	
	// lazy Array of independent Data Objects: only keys are persisted
    QStringList mKeyTagsKeys;
    bool mKeyTagsKeysResolved;
    QList<KeyTag*> mKeyTags;
	// implementation for QQmlListProperty to use
    // QML functions for List of KeyTag*
    static void appendToKeyTagsProperty(QQmlListProperty<KeyTag> *keyTagsList,
        KeyTag* keyTag);
    static int keyTagsPropertyCount(QQmlListProperty<KeyTag> *keyTagsList);
    static KeyTag* atKeyTagsProperty(QQmlListProperty<KeyTag> *keyTagsList, int pos);
    static void clearKeyTagsProperty(QQmlListProperty<KeyTag> *keyTagsList);
	
    // lazy Array of independent Data Objects: only keys are persisted
    QStringList mKeyContentBlocksKeys;
    bool mKeyContentBlocksKeysResolved;
    QList<KeyContentBlock*> mKeyContentBlocks;
    // implementation for QQmlListProperty to use
    // QML functions for List of KeyContentBlock*
    static void appendToKeyContentBlocksProperty(QQmlListProperty<KeyContentBlock> *keyContentBlocksList,
        KeyContentBlock* keyContentBlock);
    static int keyContentBlocksPropertyCount(QQmlListProperty<KeyContentBlock> *keyContentBlocksList);
    static KeyContentBlock* atKeyContentBlocksProperty(QQmlListProperty<KeyContentBlock> *keyContentBlocksList, int pos);
    static void clearKeyContentBlocksProperty(QQmlListProperty<KeyContentBlock> *keyContentBlocksList);


    // lazy Array of independent Data Objects: only keys are persisted
    QStringList mKeyContentStandardTypesKeys;
    bool mKeyContentStandardTypesKeysResolved;
    QList<KeyContentStandardType*> mKeyContentStandardTypes;
    // implementation for QQmlListProperty to use
    // QML functions for List of KeyContentStandardType*
    static void appendToKeyContentStandardTypesProperty(QQmlListProperty<KeyContentStandardType> *keyContentStandardTypesList,
        KeyContentStandardType* keyContentStandardType);
    static int keyContentStandardTypesPropertyCount(QQmlListProperty<KeyContentStandardType> *keyContentStandardTypesList);
    static KeyContentStandardType* atKeyContentStandardTypesProperty(QQmlListProperty<KeyContentStandardType> *keyContentStandardTypesList, int pos);
    static void clearKeyContentStandardTypesProperty(QQmlListProperty<KeyContentStandardType> *keyContentStandardTypesList);


    Q_DISABLE_COPY (Key)
};
Q_DECLARE_METATYPE(Key*)

#endif /* KEY_HPP_ */

