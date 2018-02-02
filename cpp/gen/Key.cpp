#include "Key.hpp"
#include <QDebug>
#include <quuid.h>
// target also references to this
#include "Creator.hpp"
// target also references to this
#include "KeyTag.hpp"
// target also references to this
#include "KeyContentBlock.hpp"
// target also references to this
#include "KeyContentStandardType.hpp"
// target also references to this
#include "Day.hpp"
// target also references to this
#include "Section.hpp"
// target also references to this
#include "DictionaryItem.hpp"

// keys of QVariantMap used in this APP
static const QString keyIdKey = "keyId";
static const QString keyIdRefViewKey = "keyIdRefView";

static const QString sortKeyKey = "sortKey";

static const QString isRefViewKey = "isRefView";
static const QString isBoolean2Key = "isBoolean2";
static const QString isBoolean3Key = "isBoolean3";
static const QString isBoolean4Key = "isBoolean4";
static const QString isBoolean5Key = "isBoolean5";
static const QString isBoolean6Key = "isBoolean6";
static const QString isBoolean7Key = "isBoolean7";
static const QString isBoolean8Key = "isBoolean8";

static const QString titleKey = "title";
static const QString subtitleKey = "subtitle";
static const QString descriptionKey = "description";

static const QString keyTypeKey = "keyType";

static const QString startTimeKey = "startTime";
static const QString endTimeKey = "endTime";
static const QString minutesKey = "minutes";

static const QString contentStandardNameKey = "contentStandardName";
static const QString contentArticulo1Key = "contentArticulo1";
static const QString contentSituacion1Key = "contentSituacion1";

static const QString contentLinkKey = "contentLink";

static const QString isFavoriteKey = "isFavorite";
static const QString isBookmarkedKey = "isBookmarked";

static const QString creatorKey = "creator";

static const QString keyLinksKey = "keyLinks";
static const QString keyTagsKey = "keyTags";
static const QString keyContentBlocksKey = "keyContentBlocks";
static const QString keyContentStandardTypesKey = "keyContentStandardTypes";
static const QString keyDayKey = "keyDay";

static const QString sectionKey = "section";
static const QString dictionaryItemKey = "dictionaryItem";

// keys used from Server API etc
static const QString keyIdForeignKey = "keyId";
static const QString keyIdRefViewForeignKey = "keyIdRefView";

static const QString sortKeyForeignKey = "sortKey";

static const QString isRefViewForeignKey = "isRefView";
static const QString isBoolean2ForeignKey = "isBoolean2";
static const QString isBoolean3ForeignKey = "isBoolean3";
static const QString isBoolean4ForeignKey = "isBoolean4";
static const QString isBoolean5ForeignKey = "isBoolean5";
static const QString isBoolean6ForeignKey = "isBoolean6";
static const QString isBoolean7ForeignKey = "isBoolean7";
static const QString isBoolean8ForeignKey = "isBoolean8";

static const QString titleForeignKey = "title";
static const QString subtitleForeignKey = "subtitle";
static const QString descriptionForeignKey = "description";

static const QString keyTypeForeignKey = "keyType";

static const QString contentStandardNameForeignKey = "contentStandardName";
static const QString contentArticulo1ForeignKey = "contentArticulo1";
static const QString contentSituacion1ForeignKey = "contentSituacion1";

static const QString contentLinkForeignKey = "contentLink";

static const QString startForeignKey = "startTime";
static const QString endTimeForeignKey = "endTime";
static const QString minutesForeignKey = "minutes";

static const QString isFavoriteForeignKey = "isFavorite";
static const QString isBookmarkedForeignKey = "isBookmarked";

static const QString creatorForeignKey = "creator";

static const QString keyLinksForeignKey = "keyLinks";
static const QString keyTagsForeignKey = "keyTags";
static const QString keyContentBlocksForeignKey = "keyContentBlocks";
static const QString keyContentStandardTypesForeignKey = "keyContentStandardTypes";
static const QString keyDayForeignKey = "keyDay";

static const QString sectionForeignKey = "section";

static const QString dictionaryItemForeignKey = "dictionaryItem";

/*
 * Default Constructor if Key not initialized from QVariantMap
 */
Key::Key(QObject *parent) :
        QObject(parent), mKeyId(-1), mIsBoolean2(false), mSortKey(""), mIsBoolean7(false), mIsBoolean4(false), mIsBoolean3(false), mIsBoolean6(false), mIsRefView(false), mIsBoolean8(false), mIsBoolean5(false), mTitle(""), mSubtitle(""), mDescription(""), mKeyType(""), mContentStandardName(""), mContentArticulo1(""), mContentSituacion1(""), mContentLink(""), mMinutes(0), mIsFavorite(false), mIsBookmarked(false)
{
    // lazy references:
    mKeyDay = -1;
    mKeyDayAsDataObject = 0;
    mKeyDayInvalid = false;
    mContentSection = -1;
    mContentSectionAsDataObject = 0;
    mContentSectionInvalid = false;
    mDictionaryItem = -1;
    mDictionaryItemAsDataObject = 0;
    mDictionaryItemInvalid = false;

    // Date, Time or Timestamp ? construct null value
    mStartTime = QTime();
    mEndTime = QTime();

    // transient values (not cached)
    // bool mIsFavorite
    // bool mIsBookmarked
    // lazy Arrays where only keys are persisted
    mCreatorKeysResolved = false;
    mKeyLinksKeysResolved = false;
    mKeyTagsKeysResolved = false;
    mKeyContentBlocksKeysResolved = false;
    mKeyContentStandardTypesKeysResolved = false;
}

bool Key::isAllResolved()
{
	if (hasKeyDay() && !isKeyDayResolvedAsDataObject()) {
		return false;
	}
	if (hasSection() && !isSectionResolvedAsDataObject()) {
		return false;
	}
	if (hasDictionaryItem() && !isDictionaryItemResolvedAsDataObject()) {
		return false;
	}
    if(!areCreatorKeysResolved()) {
        return false;
    }
    if(!areKeyLinksKeysResolved()) {
        return false;
    }
    if(!areKeyTagsKeysResolved()) {
        return false;
    }
    if(!areKeyContentBlocksKeysResolved()) {
        return false;
    }
    if(!areKeyContentStandardTypesKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize Key from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Key::fillFromMap(const QVariantMap& keyMap)
{
    mKeyId = keyMap.value(keyIdKey).toInt();
    mKeyIdRefView = keyMap.value(keyIdRefViewKey).toInt();

    mSortKey = keyMap.value(sortKeyKey).toString();

    mIsRefView = keyMap.value(isRefViewKey).toBool();
    mIsBoolean2 = keyMap.value(isBoolean2Key).toBool();
    mIsBoolean3 = keyMap.value(isBoolean3Key).toBool();
    mIsBoolean4 = keyMap.value(isBoolean4Key).toBool();
    mIsBoolean5 = keyMap.value(isBoolean5Key).toBool();
    mIsBoolean6 = keyMap.value(isBoolean6Key).toBool();
    mIsBoolean7 = keyMap.value(isBoolean7Key).toBool();
    mIsBoolean8 = keyMap.value(isBoolean8Key).toBool();

	mTitle = keyMap.value(titleKey).toString();
	mSubtitle = keyMap.value(subtitleKey).toString();
	mDescription = keyMap.value(descriptionKey).toString();
	mKeyType = keyMap.value(keyTypeKey).toString();
    mContentStandardName = keyMap.value(contentStandardNameKey).toString();
    mContentArticulo1 = keyMap.value(contentArticulo1Key).toString();
    mContentSituacion1 = keyMap.value(contentSituacion1Key).toString();
    mContentLink = keyMap.value(contentLinkKey).toString();
    if (keyMap.contains(startTimeKey)) {
		// always getting the Date as a String (from server or JSON)
		QString startTimeAsString = keyMap.value(startTimeKey).toString();
		mStartTime = QTime::fromString(startTimeAsString, "HH:mm");
		if (!mStartTime.isValid()) {
			mStartTime = QTime();
			qDebug() << "mStartTime is not valid for String: " << startTimeAsString;
		}
	}
	if (keyMap.contains(endTimeKey)) {
		// always getting the Date as a String (from server or JSON)
		QString endTimeAsString = keyMap.value(endTimeKey).toString();
		mEndTime = QTime::fromString(endTimeAsString, "HH:mm");
		if (!mEndTime.isValid()) {
			mEndTime = QTime();
			qDebug() << "mEndTime is not valid for String: " << endTimeAsString;
		}
	}
	mMinutes = keyMap.value(minutesKey).toInt();
	// mIsFavorite is transient
	if (keyMap.contains(isFavoriteKey)) {
		mIsFavorite = keyMap.value(isFavoriteKey).toBool();
	}
	// mIsBookmarked is transient
	if (keyMap.contains(isBookmarkedKey)) {
		mIsBookmarked = keyMap.value(isBookmarkedKey).toBool();
	}
	// keyDay lazy pointing to Day* (domainKey: id)
	if (keyMap.contains(keyDayKey)) {
		mKeyDay = keyMap.value(keyDayKey).toInt();
		if (mKeyDay != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// section lazy pointing to Section* (domainKey: sectionId)
    if (keyMap.contains(sectionKey)) {
        mContentSection = keyMap.value(sectionKey).toInt();
        if (mContentSection != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// dictionaryItem lazy pointing to DictionaryItem* (domainKey: keyId)
	if (keyMap.contains(dictionaryItemKey)) {
		mDictionaryItem = keyMap.value(dictionaryItemKey).toInt();
		if (mDictionaryItem != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mCreator is (lazy loaded) Array of Creator*
	mCreatorKeys = keyMap.value(creatorKey).toStringList();
	// mCreator must be resolved later if there are keys
	mCreatorKeysResolved = (mCreatorKeys.size() == 0);
	mCreator.clear();
	// mKeyLinks is (lazy loaded) Array of KeyLink*
	mKeyLinksKeys = keyMap.value(keyLinksKey).toStringList();
	// mKeyLinks must be resolved later if there are keys
	mKeyLinksKeysResolved = (mKeyLinksKeys.size() == 0);
	mKeyLinks.clear();
	// mKeyTags is (lazy loaded) Array of KeyTag*
	mKeyTagsKeys = keyMap.value(keyTagsKey).toStringList();
	// mKeyTags must be resolved later if there are keys
	mKeyTagsKeysResolved = (mKeyTagsKeys.size() == 0);
	mKeyTags.clear();
    // mKeyContentBlocks is (lazy loaded) Array of KeyContentBlock*
    mKeyContentBlocksKeys = keyMap.value(keyContentBlocksKey).toStringList();
    // mKeyContentBlocks must be resolved later if there are keys
    mKeyContentBlocksKeysResolved = (mKeyContentBlocksKeys.size() == 0);
    mKeyContentBlocks.clear();
    // mKeyContentStandardTypes is (lazy loaded) Array of KeyContentStandardType*
    mKeyContentStandardTypesKeys = keyMap.value(keyContentStandardTypesKey).toStringList();
    // mKeyContentStandardTypes must be resolved later if there are keys
    mKeyContentStandardTypesKeysResolved = (mKeyContentStandardTypesKeys.size() == 0);
    mKeyContentStandardTypes.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void Key::fillFromForeignMap(const QVariantMap& keyMap)
{
    mKeyId = keyMap.value(keyIdForeignKey).toInt();
    mKeyIdRefView = keyMap.value(keyIdRefViewKey).toInt();
	mSortKey = keyMap.value(sortKeyForeignKey).toString();

    mIsRefView = keyMap.value(isRefViewForeignKey).toBool();
    mIsBoolean2 = keyMap.value(isBoolean2ForeignKey).toBool();
    mIsBoolean3 = keyMap.value(isBoolean3ForeignKey).toBool();
    mIsBoolean4 = keyMap.value(isBoolean4ForeignKey).toBool();
    mIsBoolean5 = keyMap.value(isBoolean5ForeignKey).toBool();
    mIsBoolean6 = keyMap.value(isBoolean6ForeignKey).toBool();
    mIsBoolean7 = keyMap.value(isBoolean7ForeignKey).toBool();
    mIsBoolean8 = keyMap.value(isBoolean8ForeignKey).toBool();

	mTitle = keyMap.value(titleForeignKey).toString();
	mSubtitle = keyMap.value(subtitleForeignKey).toString();
	mDescription = keyMap.value(descriptionForeignKey).toString();
	mKeyType = keyMap.value(keyTypeForeignKey).toString();
    mContentStandardName = keyMap.value(contentStandardNameForeignKey).toString();
    mContentArticulo1 = keyMap.value(contentArticulo1ForeignKey).toString();
    mContentSituacion1 = keyMap.value(contentSituacion1ForeignKey).toString();
    mContentLink = keyMap.value(contentLinkForeignKey).toString();
    if (keyMap.contains(startForeignKey)) {
		// always getting the Date as a String (from server or JSON)
        QString startTimeAsString = keyMap.value(startForeignKey).toString();
		mStartTime = QTime::fromString(startTimeAsString, "HH:mm");
		if (!mStartTime.isValid()) {
			mStartTime = QTime();
			qDebug() << "mStartTime is not valid for String: " << startTimeAsString;
		}
	}
	if (keyMap.contains(endTimeForeignKey)) {
		// always getting the Date as a String (from server or JSON)
		QString endTimeAsString = keyMap.value(endTimeForeignKey).toString();
		mEndTime = QTime::fromString(endTimeAsString, "HH:mm");
		if (!mEndTime.isValid()) {
			mEndTime = QTime();
			qDebug() << "mEndTime is not valid for String: " << endTimeAsString;
		}
	}
	mMinutes = keyMap.value(minutesForeignKey).toInt();
	// mIsFavorite is transient
	if (keyMap.contains(isFavoriteForeignKey)) {
		mIsFavorite = keyMap.value(isFavoriteForeignKey).toBool();
	}
	// mIsBookmarked is transient
	if (keyMap.contains(isBookmarkedForeignKey)) {
		mIsBookmarked = keyMap.value(isBookmarkedForeignKey).toBool();
	}
	// keyDay lazy pointing to Day* (domainKey: id)
	if (keyMap.contains(keyDayForeignKey)) {
		mKeyDay = keyMap.value(keyDayForeignKey).toInt();
		if (mKeyDay != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// section lazy pointing to Section* (domainKey: sectionId)
    if (keyMap.contains(sectionForeignKey)) {
        mContentSection = keyMap.value(sectionForeignKey).toInt();
        if (mContentSection != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// dictionaryItem lazy pointing to DictionaryItem* (domainKey: keyId)
	if (keyMap.contains(dictionaryItemForeignKey)) {
		mDictionaryItem = keyMap.value(dictionaryItemForeignKey).toInt();
		if (mDictionaryItem != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mCreator is (lazy loaded) Array of Creator*
	mCreatorKeys = keyMap.value(creatorForeignKey).toStringList();
	// mCreator must be resolved later if there are keys
	mCreatorKeysResolved = (mCreatorKeys.size() == 0);
	mCreator.clear();
	// mKeyLinks is (lazy loaded) Array of KeyLink*
	mKeyLinksKeys = keyMap.value(keyLinksForeignKey).toStringList();
	// mKeyLinks must be resolved later if there are keys
	mKeyLinksKeysResolved = (mKeyLinksKeys.size() == 0);
	mKeyLinks.clear();
	// mKeyTags is (lazy loaded) Array of KeyTag*
	mKeyTagsKeys = keyMap.value(keyTagsForeignKey).toStringList();
	// mKeyTags must be resolved later if there are keys
	mKeyTagsKeysResolved = (mKeyTagsKeys.size() == 0);
	mKeyTags.clear();
    // mKeyContentBlocks is (lazy loaded) Array of KeyContentBlock*
    mKeyContentBlocksKeys = keyMap.value(keyContentBlocksForeignKey).toStringList();
    // mKeyContentBlocks must be resolved later if there are keys
    mKeyContentBlocksKeysResolved = (mKeyContentBlocksKeys.size() == 0);
    mKeyContentBlocks.clear();
    // mKeyContentStandardTypes is (lazy loaded) Array of KeyContentStandardType*
    mKeyContentStandardTypesKeys = keyMap.value(keyContentStandardTypesForeignKey).toStringList();
    // mKeyContentStandardTypes must be resolved later if there are keys
    mKeyContentStandardTypesKeysResolved = (mKeyContentStandardTypesKeys.size() == 0);
    mKeyContentStandardTypes.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void Key::fillFromCacheMap(const QVariantMap& keyMap)
{
    mKeyId = keyMap.value(keyIdKey).toInt();
    mKeyIdRefView = keyMap.value(keyIdRefViewKey).toInt();

	mSortKey = keyMap.value(sortKeyKey).toString();

    mIsRefView = keyMap.value(isRefViewKey).toBool();
    mIsBoolean2 = keyMap.value(isBoolean2Key).toBool();
    mIsBoolean3 = keyMap.value(isBoolean3Key).toBool();
    mIsBoolean4 = keyMap.value(isBoolean4Key).toBool();
    mIsBoolean5 = keyMap.value(isBoolean5Key).toBool();
    mIsBoolean6 = keyMap.value(isBoolean6Key).toBool();
    mIsBoolean7 = keyMap.value(isBoolean7Key).toBool();
    mIsBoolean8 = keyMap.value(isBoolean8Key).toBool();

	mTitle = keyMap.value(titleKey).toString();
	mSubtitle = keyMap.value(subtitleKey).toString();
	mDescription = keyMap.value(descriptionKey).toString();
	mKeyType = keyMap.value(keyTypeKey).toString();

    mContentStandardName = keyMap.value(contentStandardNameKey).toString();
    mContentArticulo1 = keyMap.value(contentArticulo1Key).toString();
    mContentSituacion1 = keyMap.value(contentSituacion1Key).toString();
    mContentLink = keyMap.value(contentLinkKey).toString();
	if (keyMap.contains(startTimeKey)) {
		// always getting the Date as a String (from server or JSON)
		QString startTimeAsString = keyMap.value(startTimeKey).toString();
		mStartTime = QTime::fromString(startTimeAsString, "HH:mm");
		if (!mStartTime.isValid()) {
			mStartTime = QTime();
			qDebug() << "mStartTime is not valid for String: " << startTimeAsString;
		}
	}
	if (keyMap.contains(endTimeKey)) {
		// always getting the Date as a String (from server or JSON)
		QString endTimeAsString = keyMap.value(endTimeKey).toString();
		mEndTime = QTime::fromString(endTimeAsString, "HH:mm");
		if (!mEndTime.isValid()) {
			mEndTime = QTime();
			qDebug() << "mEndTime is not valid for String: " << endTimeAsString;
		}
	}
	mMinutes = keyMap.value(minutesKey).toInt();
	// mIsFavorite is transient - don't forget to initialize
	// mIsBookmarked is transient - don't forget to initialize
	// keyDay lazy pointing to Day* (domainKey: id)
	if (keyMap.contains(keyDayKey)) {
		mKeyDay = keyMap.value(keyDayKey).toInt();
		if (mKeyDay != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// section lazy pointing to Section* (domainKey: sectionId)
    if (keyMap.contains(sectionKey)) {
        mContentSection = keyMap.value(sectionKey).toInt();
        if (mContentSection != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// dictionaryItem lazy pointing to DictionaryItem* (domainKey: keyId)
	if (keyMap.contains(dictionaryItemKey)) {
		mDictionaryItem = keyMap.value(dictionaryItemKey).toInt();
		if (mDictionaryItem != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mCreator is (lazy loaded) Array of Creator*
	mCreatorKeys = keyMap.value(creatorKey).toStringList();
	// mCreator must be resolved later if there are keys
	mCreatorKeysResolved = (mCreatorKeys.size() == 0);
	mCreator.clear();
	// mKeyLinks is (lazy loaded) Array of KeyLink*
	mKeyLinksKeys = keyMap.value(keyLinksKey).toStringList();
	// mKeyLinks must be resolved later if there are keys
	mKeyLinksKeysResolved = (mKeyLinksKeys.size() == 0);
	mKeyLinks.clear();
    // mKeyTags is (lazy loaded) Array of KeyTag*
    mKeyTagsKeys = keyMap.value(keyTagsKey).toStringList();
    // mKeyTags must be resolved later if there are keys
    mKeyTagsKeysResolved = (mKeyTagsKeys.size() == 0);
    mKeyTags.clear();
    // mKeyContentBlocks is (lazy loaded) Array of KeyContentBlock*
    mKeyContentBlocksKeys = keyMap.value(keyContentBlocksKey).toStringList();
    // mKeyContentBlocks must be resolved later if there are keys
    mKeyContentBlocksKeysResolved = (mKeyContentBlocksKeys.size() == 0);
    mKeyContentBlocks.clear();
    // mKeyContentStandardTypes is (lazy loaded) Array of KeyContentStandardType*
    mKeyContentStandardTypesKeys = keyMap.value(keyContentStandardTypesKey).toStringList();
    // mKeyContentStandardTypes must be resolved later if there are keys
    mKeyContentStandardTypesKeysResolved = (mKeyContentStandardTypesKeys.size() == 0);
    mKeyContentStandardTypes.clear();
}

void Key::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Key::isValid()
{
	if (mKeyId == -1) {
		return false;
	}
	// keyDay lazy pointing to Day* (domainKey: id)
	if (mKeyDay == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Key as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Key::toMap()
{
	QVariantMap keyMap;
	// keyDay lazy pointing to Day* (domainKey: id)
	if (mKeyDay != -1) {
		keyMap.insert(keyDayKey, mKeyDay);
	}
	// section lazy pointing to Section* (domainKey: sectionId)
    if (mContentSection != -1) {
        keyMap.insert(sectionKey, mContentSection);
	}
	// dictionaryItem lazy pointing to DictionaryItem* (domainKey: keyId)
	if (mDictionaryItem != -1) {
		keyMap.insert(dictionaryItemKey, mDictionaryItem);
	}
	// mCreator points to Creator*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mCreatorKeysResolved) {
		mCreatorKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mCreator.size(); ++i) {
		Creator* creator;
		creator = mCreator.at(i);
		mCreatorKeys << QString::number(creator->creatorId());
	}
	keyMap.insert(creatorKey, mCreatorKeys);
	// mKeyLinks points to KeyLink*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mKeyLinksKeysResolved) {
		mKeyLinksKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mKeyLinks.size(); ++i) {
		KeyLink* keyLink;
		keyLink = mKeyLinks.at(i);
		mKeyLinksKeys << keyLink->uuid();
	}
	keyMap.insert(keyLinksKey, mKeyLinksKeys);
	// mKeyTags points to KeyTag*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mKeyTagsKeysResolved) {
		mKeyTagsKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mKeyTags.size(); ++i) {
		KeyTag* keyTag;
		keyTag = mKeyTags.at(i);
		mKeyTagsKeys << QString::number(keyTag->tagId());
	}
    keyMap.insert(keyTagsKey, mKeyTagsKeys);

    // mKeyContentBlocks points to KeyContentBlock*
    // lazy array: persist only keys
    //
    // if keys already resolved: clear them
    // otherwise reuse the keys and add objects from mPositions
    // this can happen if added to objects without resolving keys before
    if(mKeyContentBlocksKeysResolved) {
        mKeyContentBlocksKeys.clear();
    }
    // add objects from mPositions
    for (int i = 0; i < mKeyContentBlocks.size(); ++i) {
        KeyContentBlock* keyContentBlock;
        keyContentBlock = mKeyContentBlocks.at(i);
        mKeyContentBlocksKeys << QString::number(keyContentBlock->contentBlockId());
    }
    keyMap.insert(keyContentBlocksKey, mKeyContentBlocksKeys);

    // mKeyContentStandardTypes points to KeyContentStandardType*
    // lazy array: persist only keys
    //
    // if keys already resolved: clear them
    // otherwise reuse the keys and add objects from mPositions
    // this can happen if added to objects without resolving keys before
    if(mKeyContentStandardTypesKeysResolved) {
        mKeyContentStandardTypesKeys.clear();
    }
    // add objects from mPositions
    for (int i = 0; i < mKeyContentStandardTypes.size(); ++i) {
        KeyContentStandardType* keyContentStandardType;
        keyContentStandardType = mKeyContentStandardTypes.at(i);
        mKeyContentStandardTypesKeys << QString::number(keyContentStandardType->contentStandardTypeId());
    }
    keyMap.insert(keyContentStandardTypesKey, mKeyContentStandardTypesKeys);

    keyMap.insert(keyIdKey, mKeyId);
    keyMap.insert(keyIdRefViewKey, mKeyIdRefView);
    keyMap.insert(sortKeyKey, mSortKey);

    keyMap.insert(isRefViewKey, mIsRefView);
    keyMap.insert(isBoolean2Key, mIsBoolean2);
    keyMap.insert(isBoolean3Key, mIsBoolean3);
    keyMap.insert(isBoolean4Key, mIsBoolean4);
    keyMap.insert(isBoolean5Key, mIsBoolean5);
    keyMap.insert(isBoolean6Key, mIsBoolean6);
    keyMap.insert(isBoolean7Key, mIsBoolean7);
    keyMap.insert(isBoolean8Key, mIsBoolean8);

	keyMap.insert(titleKey, mTitle);
	keyMap.insert(subtitleKey, mSubtitle);
	keyMap.insert(descriptionKey, mDescription);
	keyMap.insert(keyTypeKey, mKeyType);
    keyMap.insert(contentStandardNameKey, mContentStandardName);
    keyMap.insert(contentArticulo1Key, mContentArticulo1);
    keyMap.insert(contentSituacion1Key, mContentSituacion1);
    keyMap.insert(contentLinkKey, mContentLink);

    if (hasStartTime()) {
		keyMap.insert(startTimeKey, mStartTime.toString("HH:mm"));
	}
	if (hasEndTime()) {
		keyMap.insert(endTimeKey, mEndTime.toString("HH:mm"));
	}
	keyMap.insert(minutesKey, mMinutes);
	keyMap.insert(isFavoriteKey, mIsFavorite);
	keyMap.insert(isBookmarkedKey, mIsBookmarked);
	return keyMap;
}

/*
 * Exports Properties from Key as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Key::toForeignMap()
{
	QVariantMap keyMap;
	// keyDay lazy pointing to Day* (domainKey: id)
	if (mKeyDay != -1) {
		keyMap.insert(keyDayForeignKey, mKeyDay);
	}
	// section lazy pointing to Section* (domainKey: sectionId)
    if (mContentSection != -1) {
        keyMap.insert(sectionForeignKey, mContentSection);
	}
	// dictionaryItem lazy pointing to DictionaryItem* (domainKey: keyId)
	if (mDictionaryItem != -1) {
		keyMap.insert(dictionaryItemForeignKey, mDictionaryItem);
	}
	// mCreator points to Creator*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mCreatorKeysResolved) {
		mCreatorKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mCreator.size(); ++i) {
		Creator* creator;
		creator = mCreator.at(i);
		mCreatorKeys << QString::number(creator->creatorId());
	}
	keyMap.insert(creatorKey, mCreatorKeys);
	// mKeyLinks points to KeyLink*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mKeyLinksKeysResolved) {
		mKeyLinksKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mKeyLinks.size(); ++i) {
		KeyLink* keyLink;
		keyLink = mKeyLinks.at(i);
		mKeyLinksKeys << keyLink->uuid();
	}
	keyMap.insert(keyLinksKey, mKeyLinksKeys);
	// mKeyTags points to KeyTag*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mKeyTagsKeysResolved) {
		mKeyTagsKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mKeyTags.size(); ++i) {
		KeyTag* keyTag;
		keyTag = mKeyTags.at(i);
		mKeyTagsKeys << QString::number(keyTag->tagId());
	}
    keyMap.insert(keyTagsKey, mKeyTagsKeys);

    // mKeyContentBlocks points to KeyContentBlock*
    // lazy array: persist only keys
    //
    // if keys already resolved: clear them
    // otherwise reuse the keys and add objects from mPositions
    // this can happen if added to objects without resolving keys before
    if(mKeyContentBlocksKeysResolved) {
        mKeyContentBlocksKeys.clear();
    }
    // add objects from mPositions
    for (int i = 0; i < mKeyContentBlocks.size(); ++i) {
        KeyContentBlock* keyContentBlock;
        keyContentBlock = mKeyContentBlocks.at(i);
        mKeyContentBlocksKeys << QString::number(keyContentBlock->contentBlockId());
    }
    keyMap.insert(keyContentBlocksKey, mKeyContentBlocksKeys);

    // mKeyContentStandardTypes points to KeyContentStandardType*
    // lazy array: persist only keys
    //
    // if keys already resolved: clear them
    // otherwise reuse the keys and add objects from mPositions
    // this can happen if added to objects without resolving keys before
    if(mKeyContentStandardTypesKeysResolved) {
        mKeyContentStandardTypesKeys.clear();
    }
    // add objects from mPositions
    for (int i = 0; i < mKeyContentStandardTypes.size(); ++i) {
        KeyContentStandardType* keyContentStandardType;
        keyContentStandardType = mKeyContentStandardTypes.at(i);
        mKeyContentStandardTypesKeys << QString::number(keyContentStandardType->contentStandardTypeId());
    }
    keyMap.insert(keyContentStandardTypesKey, mKeyContentStandardTypesKeys);

    keyMap.insert(keyIdForeignKey, mKeyId);
    keyMap.insert(keyIdRefViewForeignKey, mKeyIdRefView);
    keyMap.insert(sortKeyForeignKey, mSortKey);

    keyMap.insert(isRefViewForeignKey, mIsRefView);
    keyMap.insert(isBoolean2ForeignKey, mIsBoolean2);
    keyMap.insert(isBoolean3ForeignKey, mIsBoolean3);
    keyMap.insert(isBoolean4ForeignKey, mIsBoolean4);
    keyMap.insert(isBoolean5ForeignKey, mIsBoolean5);
    keyMap.insert(isBoolean6ForeignKey, mIsBoolean6);
    keyMap.insert(isBoolean7ForeignKey, mIsBoolean7);
    keyMap.insert(isBoolean8ForeignKey, mIsBoolean8);

	keyMap.insert(titleForeignKey, mTitle);
	keyMap.insert(subtitleForeignKey, mSubtitle);
	keyMap.insert(descriptionForeignKey, mDescription);
	keyMap.insert(keyTypeForeignKey, mKeyType);
    keyMap.insert(contentStandardNameForeignKey, mContentStandardName);
    keyMap.insert(contentArticulo1ForeignKey, mContentArticulo1);
    keyMap.insert(contentSituacion1ForeignKey, mContentSituacion1);
    keyMap.insert(contentLinkForeignKey, mContentLink);

    if (hasStartTime()) {
        keyMap.insert(startForeignKey, mStartTime.toString("HH:mm"));
	}
	if (hasEndTime()) {
		keyMap.insert(endTimeForeignKey, mEndTime.toString("HH:mm"));
	}
	keyMap.insert(minutesForeignKey, mMinutes);
	return keyMap;
}


/*
 * Exports Properties from Key as QVariantMap
 * transient properties are excluded:
 * mIsFavorite, mIsBookmarked
 * To export ALL data use toMap()
 */
QVariantMap Key::toCacheMap()
{
	QVariantMap keyMap;
	// keyDay lazy pointing to Day* (domainKey: id)
	if (mKeyDay != -1) {
		keyMap.insert(keyDayKey, mKeyDay);
	}
	// section lazy pointing to Section* (domainKey: sectionId)
    if (mContentSection != -1) {
        keyMap.insert(sectionKey, mContentSection);
	}
	// dictionaryItem lazy pointing to DictionaryItem* (domainKey: keyId)
	if (mDictionaryItem != -1) {
		keyMap.insert(dictionaryItemKey, mDictionaryItem);
	}
	// mCreator points to Creator*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mCreatorKeysResolved) {
		mCreatorKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mCreator.size(); ++i) {
		Creator* creator;
		creator = mCreator.at(i);
		mCreatorKeys << QString::number(creator->creatorId());
	}
	keyMap.insert(creatorKey, mCreatorKeys);
	// mKeyLinks points to KeyLink*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mKeyLinksKeysResolved) {
		mKeyLinksKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mKeyLinks.size(); ++i) {
		KeyLink* keyLink;
		keyLink = mKeyLinks.at(i);
		mKeyLinksKeys << keyLink->uuid();
	}
	keyMap.insert(keyLinksKey, mKeyLinksKeys);
	// mKeyTags points to KeyTag*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mKeyTagsKeysResolved) {
		mKeyTagsKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mKeyTags.size(); ++i) {
		KeyTag* keyTag;
		keyTag = mKeyTags.at(i);
		mKeyTagsKeys << QString::number(keyTag->tagId());
	}
    keyMap.insert(keyTagsKey, mKeyTagsKeys);

    // mKeyContentBlocks points to KeyContentBlock*
    // lazy array: persist only keys
    //
    // if keys already resolved: clear them
    // otherwise reuse the keys and add objects from mPositions
    // this can happen if added to objects without resolving keys before
    if(mKeyContentBlocksKeysResolved) {
        mKeyContentBlocksKeys.clear();
    }
    // add objects from mPositions
    for (int i = 0; i < mKeyContentBlocks.size(); ++i) {
        KeyContentBlock* keyContentBlock;
        keyContentBlock = mKeyContentBlocks.at(i);
        mKeyContentBlocksKeys << QString::number(keyContentBlock->contentBlockId());
    }
    keyMap.insert(keyContentBlocksKey, mKeyContentBlocksKeys);

    // mKeyContentStandardTypes points to KeyContentStandardType*
    // lazy array: persist only keys
    //
    // if keys already resolved: clear them
    // otherwise reuse the keys and add objects from mPositions
    // this can happen if added to objects without resolving keys before
    if(mKeyContentStandardTypesKeysResolved) {
        mKeyContentStandardTypesKeys.clear();
    }
    // add objects from mPositions
    for (int i = 0; i < mKeyContentStandardTypes.size(); ++i) {
        KeyContentStandardType* keyContentStandardType;
        keyContentStandardType = mKeyContentStandardTypes.at(i);
        mKeyContentStandardTypesKeys << QString::number(keyContentStandardType->contentStandardTypeId());
    }
    keyMap.insert(keyContentStandardTypesKey, mKeyContentStandardTypesKeys);

    keyMap.insert(keyIdKey, mKeyId);
    keyMap.insert(keyIdRefViewKey, mKeyIdRefView);
    keyMap.insert(sortKeyKey, mSortKey);

    keyMap.insert(isRefViewKey, mIsRefView);
    keyMap.insert(isBoolean2Key, mIsBoolean2);
    keyMap.insert(isBoolean3Key, mIsBoolean3);
    keyMap.insert(isBoolean4Key, mIsBoolean4);
    keyMap.insert(isBoolean5Key, mIsBoolean5);
    keyMap.insert(isBoolean6Key, mIsBoolean6);
    keyMap.insert(isBoolean7Key, mIsBoolean7);
    keyMap.insert(isBoolean8Key, mIsBoolean8);

	keyMap.insert(titleKey, mTitle);
	keyMap.insert(subtitleKey, mSubtitle);
	keyMap.insert(descriptionKey, mDescription);
	keyMap.insert(keyTypeKey, mKeyType);
    keyMap.insert(contentStandardNameKey, mContentStandardName);
    keyMap.insert(contentArticulo1Key, mContentArticulo1);
    keyMap.insert(contentSituacion1Key, mContentSituacion1);
    keyMap.insert(contentLinkKey, mContentLink);
	if (hasStartTime()) {
		keyMap.insert(startTimeKey, mStartTime.toString("HH:mm"));
	}
	if (hasEndTime()) {
		keyMap.insert(endTimeKey, mEndTime.toString("HH:mm"));
	}
	keyMap.insert(minutesKey, mMinutes);
	// excluded: mIsFavorite
	// excluded: mIsBookmarked
	return keyMap;
}
// REF
// Lazy: keyDay
// Mandatory: keyDay
// keyDay lazy pointing to Day* (domainKey: id)
int Key::keyDay() const
{
	return mKeyDay;
}
Day* Key::keyDayAsDataObject() const
{
	return mKeyDayAsDataObject;
}
void Key::setKeyDay(int keyDay)
{
	if (keyDay != mKeyDay) {
        // remove old Data Object if one was resolved
        if (mKeyDayAsDataObject) {
            // reset pointer, don't delete the independent object !
            mKeyDayAsDataObject = 0;
        }
        // set the new lazy reference
        mKeyDay = keyDay;
        mKeyDayInvalid = false;
        emit keyDayChanged(keyDay);
        if (keyDay != -1) {
            // resolve the corresponding Data Object on demand from DataManager
        }
    }
}
void Key::removeKeyDay()
{
	if (mKeyDay != -1) {
		setKeyDay(-1);
	}
}
bool Key::hasKeyDay()
{
    if (!mKeyDayInvalid && mKeyDay != -1) {
        return true;
    } else {
        return false;
    }
}
bool Key::isKeyDayResolvedAsDataObject()
{
    if (!mKeyDayInvalid && mKeyDayAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite id if different
void Key::resolveKeyDayAsDataObject(Day* day)
{
    if (day) {
        if (day->id() != mKeyDay) {
            setKeyDay(day->id());
        }
        mKeyDayAsDataObject = day;
        mKeyDayInvalid = false;
    }
}
void Key::markKeyDayAsInvalid()
{
    mKeyDayInvalid = true;
}
// REF
// Lazy: section
// Optional: section
// section lazy pointing to Section* (domainKey: sectionId)
int Key::section() const
{
    return mContentSection;
}
Section* Key::sectionAsDataObject() const
{
    return mContentSectionAsDataObject;
}
void Key::setSection(int section)
{
    if (section != mContentSection) {
        // remove old Data Object if one was resolved
        if (mContentSectionAsDataObject) {
            // reset pointer, don't delete the independent object !
            mContentSectionAsDataObject = 0;
        }
        // set the new lazy reference
        mContentSection = section;
        mContentSectionInvalid = false;
        emit sectionChanged(section);
        if (section != -1) {
            // resolve the corresponding Data Object on demand from DataManager
        }
    }
}
void Key::removeSection()
{
    if (mContentSection != -1) {
		setSection(-1);
	}
}
bool Key::hasSection()
{
    if (!mContentSectionInvalid && mContentSection != -1) {
        return true;
    } else {
        return false;
    }
}
bool Key::isSectionResolvedAsDataObject()
{
    if (!mContentSectionInvalid && mContentSectionAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite sectionId if different
void Key::resolveSectionAsDataObject(Section* section)
{
    if (section) {
        if (section->sectionId() != mContentSection) {
            setSection(section->sectionId());
        }
        mContentSectionAsDataObject = section;
        mContentSectionInvalid = false;
    }
}
void Key::markSectionAsInvalid()
{
    mContentSectionInvalid = true;
}
// REF
// Lazy: dictionaryItem
// Optional: dictionaryItem
// dictionaryItem lazy pointing to DictionaryItem* (domainKey: keyId)
int Key::dictionaryItem() const
{
	return mDictionaryItem;
}
DictionaryItem* Key::dictionaryItemAsDataObject() const
{
	return mDictionaryItemAsDataObject;
}
void Key::setDictionaryItem(int dictionaryItem)
{
	if (dictionaryItem != mDictionaryItem) {
        // remove old Data Object if one was resolved
        if (mDictionaryItemAsDataObject) {
            // reset pointer, don't delete the independent object !
            mDictionaryItemAsDataObject = 0;
        }
        // set the new lazy reference
        mDictionaryItem = dictionaryItem;
        mDictionaryItemInvalid = false;
        emit dictionaryItemChanged(dictionaryItem);
        if (dictionaryItem != -1) {
            // resolve the corresponding Data Object on demand from DataManager
        }
    }
}
void Key::removeDictionaryItem()
{
	if (mDictionaryItem != -1) {
		setDictionaryItem(-1);
	}
}
bool Key::hasDictionaryItem()
{
    if (!mDictionaryItemInvalid && mDictionaryItem != -1) {
        return true;
    } else {
        return false;
    }
}
bool Key::isDictionaryItemResolvedAsDataObject()
{
    if (!mDictionaryItemInvalid && mDictionaryItemAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite keyId if different
void Key::resolveDictionaryItemAsDataObject(DictionaryItem* dictionaryItem)
{
    if (dictionaryItem) {
        if (dictionaryItem->keyId() != mDictionaryItem) {
            setDictionaryItem(dictionaryItem->keyId());
        }
        mDictionaryItemAsDataObject = dictionaryItem;
        mDictionaryItemInvalid = false;
    }
}
void Key::markDictionaryItemAsInvalid()
{
    mDictionaryItemInvalid = true;
}
// ATT 
// Mandatory: keyId
// Domain KEY: keyId
int Key::keyId() const
{
	return mKeyId;
}

void Key::setKeyId(int keyId)
{
	if (keyId != mKeyId) {
		mKeyId = keyId;
		emit keyIdChanged(keyId);
	}
}
// ATT
// Mandatory: keyIdRefView
// Domain KEY: keyIdRefView
int Key::keyIdRefView() const
{
    return mKeyIdRefView;
}

void Key::setKeyIdRefView(int keyIdRefView)
{
    if (keyIdRefView != mKeyIdRefView) {
        mKeyIdRefView = keyIdRefView;
        emit keyIdRefViewChanged(keyIdRefView);
    }
}
// ATT 
// Optional: isBoolean2
bool Key::isBoolean2() const
{
    return mIsBoolean2;
}

void Key::setIsBoolean2(bool isBoolean2)
{
    if (isBoolean2 != mIsBoolean2) {
        mIsBoolean2 = isBoolean2;
        emit isBoolean2Changed(isBoolean2);
	}
}
// ATT 
// Optional: sortKey
QString Key::sortKey() const
{
	return mSortKey;
}

void Key::setSortKey(QString sortKey)
{
	if (sortKey != mSortKey) {
		mSortKey = sortKey;
		emit sortKeyChanged(sortKey);
	}
}
// ATT 
// Optional: isBoolean7
bool Key::isBoolean7() const
{
    return mIsBoolean7;
}

void Key::setIsBoolean7(bool isBoolean7)
{
    if (isBoolean7 != mIsBoolean7) {
        mIsBoolean7 = isBoolean7;
        emit isBoolean7Changed(isBoolean7);
	}
}
// ATT 
// Optional: isBoolean4
bool Key::isBoolean4() const
{
    return mIsBoolean4;
}

void Key::setIsBoolean4(bool isBoolean4)
{
    if (isBoolean4 != mIsBoolean4) {
        mIsBoolean4 = isBoolean4;
        emit isBoolean4Changed(isBoolean4);
	}
}
// ATT 
// Optional: isBoolean3
bool Key::isBoolean3() const
{
    return mIsBoolean3;
}

void Key::setIsBoolean3(bool isBoolean3)
{
    if (isBoolean3 != mIsBoolean3) {
        mIsBoolean3 = isBoolean3;
        emit isBoolean3Changed(isBoolean3);
	}
}
// ATT 
// Optional: isBoolean6
bool Key::isBoolean6() const
{
    return mIsBoolean6;
}

void Key::setIsBoolean6(bool isBoolean6)
{
    if (isBoolean6 != mIsBoolean6) {
        mIsBoolean6 = isBoolean6;
        emit isBoolean6Changed(isBoolean6);
	}
}
// ATT 
// Optional: isRefView
bool Key::isRefView() const
{
    return mIsRefView;
}

void Key::setIsRefView(bool isRefView)
{
    if (isRefView != mIsRefView) {
        mIsRefView = isRefView;
        emit isRefViewChanged(isRefView);
	}
}
// ATT 
// Optional: isBoolean8
bool Key::isBoolean8() const
{
    return mIsBoolean8;
}

void Key::setIsBoolean8(bool isBoolean8)
{
    if (isBoolean8 != mIsBoolean8) {
        mIsBoolean8 = isBoolean8;
        emit isBoolean8Changed(isBoolean8);
	}
}
// ATT 
// Optional: isBoolean5
bool Key::isBoolean5() const
{
    return mIsBoolean5;
}

void Key::setIsBoolean5(bool isBoolean5)
{
    if (isBoolean5 != mIsBoolean5) {
        mIsBoolean5 = isBoolean5;
        emit isBoolean5Changed(isBoolean5);
	}
}
// ATT 
// Optional: title
QString Key::title() const
{
	return mTitle;
}

void Key::setTitle(QString title)
{
	if (title != mTitle) {
		mTitle = title;
		emit titleChanged(title);
	}
}
// ATT 
// Optional: subtitle
QString Key::subtitle() const
{
	return mSubtitle;
}

void Key::setSubtitle(QString subtitle)
{
	if (subtitle != mSubtitle) {
		mSubtitle = subtitle;
		emit subtitleChanged(subtitle);
	}
}
// ATT 
// Optional: description
QString Key::description() const
{
	return mDescription;
}

void Key::setDescription(QString description)
{
	if (description != mDescription) {
		mDescription = description;
		emit descriptionChanged(description);
	}
}
// ATT 
// Optional: keyType
QString Key::keyType() const
{
	return mKeyType;
}

void Key::setKeyType(QString keyType)
{
	if (keyType != mKeyType) {
		mKeyType = keyType;
		emit keyTypeChanged(keyType);
	}
}
// ATT
// Optional: contentStandardName
QString Key::contentStandardName() const
{
    return mContentStandardName;
}

void Key::setContentStandardName(QString contentStandardName)
{
    if (contentStandardName != mContentStandardName) {
        mContentStandardName = contentStandardName;
        emit contentStandardNameChanged(contentStandardName);
    }
}
// ATT
// Optional: contentArticulo1
QString Key::contentArticulo1() const
{
    return mContentArticulo1;
}

void Key::setContentArticulo1(QString contentArticulo1)
{
    if (contentArticulo1 != mContentArticulo1) {
        mContentArticulo1 = contentArticulo1;
        emit contentArticulo1Changed(contentArticulo1);
    }
}
// ATT
// Optional: contentSituacion1
QString Key::contentSituacion1() const
{
    return mContentSituacion1;
}

void Key::setContentSituacion1(QString contentSituacion1)
{
    if (contentSituacion1 != mContentSituacion1) {
        mContentSituacion1 = contentSituacion1;
        emit contentSituacion1Changed(contentSituacion1);
    }
}
// ATT 
// Optional: startTime
/**
 * in QML set DateTimePicker value this way:
 * myPicker.value = myPicker.dateFromTime(startTime)
 */
QTime Key::startTime() const
{
	return mStartTime;
}

void Key::setStartTime(QTime startTime)
{
	if (startTime != mStartTime) {
		mStartTime = startTime;
		emit startTimeChanged(startTime);
	}
}
/**
 * INVOKABLE
 * Convenience method to make it easy to set the value from QML
 * use myPicker.value.toTimeString() as Parameter
 */
void Key::setStartTimeFromPickerValue(QString startTimeValue)
{
    QTime startTime = QTime::fromString(startTimeValue.left(8), "HH:mm:ss");
    if (startTime != mStartTime) {
        mStartTime = startTime;
        emit startTimeChanged(startTime);
    }
}
bool Key::hasStartTime()
{
	return !mStartTime.isNull() && mStartTime.isValid();
}
// ATT 
// Optional: endTime
/**
 * in QML set DateTimePicker value this way:
 * myPicker.value = myPicker.dateFromTime(endTime)
 */
QTime Key::endTime() const
{
	return mEndTime;
}

void Key::setEndTime(QTime endTime)
{
	if (endTime != mEndTime) {
		mEndTime = endTime;
		emit endTimeChanged(endTime);
	}
}
/**
 * INVOKABLE
 * Convenience method to make it easy to set the value from QML
 * use myPicker.value.toTimeString() as Parameter
 */
void Key::setEndTimeFromPickerValue(QString endTimeValue)
{
    QTime endTime = QTime::fromString(endTimeValue.left(8), "HH:mm:ss");
    if (endTime != mEndTime) {
        mEndTime = endTime;
        emit endTimeChanged(endTime);
    }
}
bool Key::hasEndTime()
{
	return !mEndTime.isNull() && mEndTime.isValid();
}
// ATT 
// Optional: minutes
int Key::minutes() const
{
	return mMinutes;
}

void Key::setMinutes(int minutes)
{
	if (minutes != mMinutes) {
		mMinutes = minutes;
		emit minutesChanged(minutes);
	}
}
// ATT 
// Optional: contentLink
QString Key::contentLink() const
{
    return mContentLink;
}

void Key::setContentLink(QString contentLink)
{
    if (contentLink != mContentLink) {
        mContentLink = contentLink;
        emit contentLinkChanged(contentLink);
	}
}
// ATT 
// Optional: isFavorite
bool Key::isFavorite() const
{
	return mIsFavorite;
}

void Key::setIsFavorite(bool isFavorite)
{
	if (isFavorite != mIsFavorite) {
		mIsFavorite = isFavorite;
		emit isFavoriteChanged(isFavorite);
	}
}
// ATT 
// Optional: isBookmarked
bool Key::isBookmarked() const
{
	return mIsBookmarked;
}

void Key::setIsBookmarked(bool isBookmarked)
{
	if (isBookmarked != mIsBookmarked) {
		mIsBookmarked = isBookmarked;
		emit isBookmarkedChanged(isBookmarked);
	}
}
// ATT 
// Optional: creator
QVariantList Key::creatorAsQVariantList()
{
	QVariantList creatorList;
	for (int i = 0; i < mCreator.size(); ++i) {
        creatorList.append((mCreator.at(i))->toMap());
    }
	return creatorList;
}
QVariantList Key::creatorAsForeignQVariantList()
{
	QVariantList creatorList;
	for (int i = 0; i < mCreator.size(); ++i) {
        creatorList.append((mCreator.at(i))->toForeignMap());
    }
	return creatorList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add creator without resolving existing keys before
 * attention: before looping through the objects
 * you must resolveCreatorKeys
 */
void Key::addToCreator(Creator* creator)
{
    mCreator.append(creator);
    emit addedToCreator(creator);
    emit creatorPropertyListChanged();
}

bool Key::removeFromCreator(Creator* creator)
{
    bool ok = false;
    ok = mCreator.removeOne(creator);
    if (!ok) {
    	qDebug() << "Creator* not found in creator";
    	return false;
    }
    emit creatorPropertyListChanged();
    // creator are independent - DON'T delete them
    return true;
}
void Key::clearCreator()
{
    for (int i = mCreator.size(); i > 0; --i) {
        removeFromCreator(mCreator.last());
    }
    mCreatorKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: creatorKeys()
 * - resolve them from DataManager
 * - then resolveCreatorKeys()
 */
bool Key::areCreatorKeysResolved()
{
    return mCreatorKeysResolved;
}

QStringList Key::creatorKeys()
{
    return mCreatorKeys;
}

/**
 * Objects from creatorKeys will be added to existing creator
 * This enables to use addToCreator() without resolving before
 * Hint: it's your responsibility to resolve before looping thru creator
 */
void Key::resolveCreatorKeys(QList<Creator*> creator)
{
    if(mCreatorKeysResolved){
        return;
    }
    // don't clear mCreator (see above)
    for (int i = 0; i < creator.size(); ++i) {
        addToCreator(creator.at(i));
    }
    mCreatorKeysResolved = true;
}

int Key::creatorCount()
{
    return mCreator.size();
}
QList<Creator*> Key::creator()
{
	return mCreator;
}
void Key::setCreator(QList<Creator*> creator) 
{
	if (creator != mCreator) {
		mCreator = creator;
		emit creatorChanged(creator);
		emit creatorPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * key.creatorPropertyList.length to get the size
 * key.creatorPropertyList[2] to get Creator* at position 2
 * key.creatorPropertyList = [] to clear the list
 * or get easy access to properties like
 * key.creatorPropertyList[2].myPropertyName
 */
QQmlListProperty<Creator> Key::creatorPropertyList()
{
    return QQmlListProperty<Creator>(this, 0, &Key::appendToCreatorProperty,
            &Key::creatorPropertyCount, &Key::atCreatorProperty,
            &Key::clearCreatorProperty);
}
void Key::appendToCreatorProperty(QQmlListProperty<Creator> *creatorList,
        Creator* creator)
{
    Key *keyObject = qobject_cast<Key *>(creatorList->object);
    if (keyObject) {
        keyObject->mCreator.append(creator);
        emit keyObject->addedToCreator(creator);
    } else {
        qWarning() << "cannot append Creator* to creator " << "Object is not of type Key*";
    }
}
int Key::creatorPropertyCount(QQmlListProperty<Creator> *creatorList)
{
    Key *key = qobject_cast<Key *>(creatorList->object);
    if (key) {
        return key->mCreator.size();
    } else {
        qWarning() << "cannot get size creator " << "Object is not of type Key*";
    }
    return 0;
}
Creator* Key::atCreatorProperty(QQmlListProperty<Creator> *creatorList, int pos)
{
    Key *key = qobject_cast<Key *>(creatorList->object);
    if (key) {
        if (key->mCreator.size() > pos) {
            return key->mCreator.at(pos);
        }
        qWarning() << "cannot get Creator* at pos " << pos << " size is "
                << key->mCreator.size();
    } else {
        qWarning() << "cannot get Creator* at pos " << pos << "Object is not of type Key*";
    }
    return 0;
}
void Key::clearCreatorProperty(QQmlListProperty<Creator> *creatorList)
{
    Key *key = qobject_cast<Key *>(creatorList->object);
    if (key) {
        // creator are independent - DON'T delete them
        key->mCreator.clear();
    } else {
        qWarning() << "cannot clear creator " << "Object is not of type Key*";
    }
}

// ATT 
// Optional: keyLinks
QVariantList Key::keyLinksAsQVariantList()
{
	QVariantList keyLinksList;
	for (int i = 0; i < mKeyLinks.size(); ++i) {
        keyLinksList.append((mKeyLinks.at(i))->toMap());
    }
	return keyLinksList;
}
QVariantList Key::keyLinksAsForeignQVariantList()
{
	QVariantList keyLinksList;
	for (int i = 0; i < mKeyLinks.size(); ++i) {
        keyLinksList.append((mKeyLinks.at(i))->toForeignMap());
    }
	return keyLinksList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add keyLinks without resolving existing keys before
 * attention: before looping through the objects
 * you must resolveKeyLinksKeys
 */
void Key::addToKeyLinks(KeyLink* keyLink)
{
    mKeyLinks.append(keyLink);
    emit addedToKeyLinks(keyLink);
    emit keyLinksPropertyListChanged();
}

bool Key::removeFromKeyLinks(KeyLink* keyLink)
{
    bool ok = false;
    ok = mKeyLinks.removeOne(keyLink);
    if (!ok) {
    	qDebug() << "KeyLink* not found in keyLinks";
    	return false;
    }
    emit keyLinksPropertyListChanged();
    // keyLinks are independent - DON'T delete them
    return true;
}
void Key::clearKeyLinks()
{
    for (int i = mKeyLinks.size(); i > 0; --i) {
        removeFromKeyLinks(mKeyLinks.last());
    }
    mKeyLinksKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: keyLinksKeys()
 * - resolve them from DataManager
 * - then resolveKeyLinksKeys()
 */
bool Key::areKeyLinksKeysResolved()
{
    return mKeyLinksKeysResolved;
}

QStringList Key::keyLinksKeys()
{
    return mKeyLinksKeys;
}

/**
 * Objects from keyLinksKeys will be added to existing keyLinks
 * This enables to use addToKeyLinks() without resolving before
 * Hint: it's your responsibility to resolve before looping thru keyLinks
 */
void Key::resolveKeyLinksKeys(QList<KeyLink*> keyLinks)
{
    if(mKeyLinksKeysResolved){
        return;
    }
    // don't clear mKeyLinks (see above)
    for (int i = 0; i < keyLinks.size(); ++i) {
        addToKeyLinks(keyLinks.at(i));
    }
    mKeyLinksKeysResolved = true;
}

int Key::keyLinksCount()
{
    return mKeyLinks.size();
}
QList<KeyLink*> Key::keyLinks()
{
	return mKeyLinks;
}
void Key::setKeyLinks(QList<KeyLink*> keyLinks) 
{
	if (keyLinks != mKeyLinks) {
		mKeyLinks = keyLinks;
		emit keyLinksChanged(keyLinks);
		emit keyLinksPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * key.keyLinksPropertyList.length to get the size
 * key.keyLinksPropertyList[2] to get KeyLink* at position 2
 * key.keyLinksPropertyList = [] to clear the list
 * or get easy access to properties like
 * key.keyLinksPropertyList[2].myPropertyName
 */
QQmlListProperty<KeyLink> Key::keyLinksPropertyList()
{
    return QQmlListProperty<KeyLink>(this, 0, &Key::appendToKeyLinksProperty,
            &Key::keyLinksPropertyCount, &Key::atKeyLinksProperty,
            &Key::clearKeyLinksProperty);
}
void Key::appendToKeyLinksProperty(QQmlListProperty<KeyLink> *keyLinksList,
        KeyLink* keyLink)
{
    Key *keyObject = qobject_cast<Key *>(keyLinksList->object);
    if (keyObject) {
        keyObject->mKeyLinks.append(keyLink);
        emit keyObject->addedToKeyLinks(keyLink);
    } else {
        qWarning() << "cannot append KeyLink* to keyLinks " << "Object is not of type Key*";
    }
}
int Key::keyLinksPropertyCount(QQmlListProperty<KeyLink> *keyLinksList)
{
    Key *key = qobject_cast<Key *>(keyLinksList->object);
    if (key) {
        return key->mKeyLinks.size();
    } else {
        qWarning() << "cannot get size keyLinks " << "Object is not of type Key*";
    }
    return 0;
}
KeyLink* Key::atKeyLinksProperty(QQmlListProperty<KeyLink> *keyLinksList, int pos)
{
    Key *key = qobject_cast<Key *>(keyLinksList->object);
    if (key) {
        if (key->mKeyLinks.size() > pos) {
            return key->mKeyLinks.at(pos);
        }
        qWarning() << "cannot get KeyLink* at pos " << pos << " size is "
                << key->mKeyLinks.size();
    } else {
        qWarning() << "cannot get KeyLink* at pos " << pos << "Object is not of type Key*";
    }
    return 0;
}
void Key::clearKeyLinksProperty(QQmlListProperty<KeyLink> *keyLinksList)
{
    Key *key = qobject_cast<Key *>(keyLinksList->object);
    if (key) {
        // keyLinks are independent - DON'T delete them
        key->mKeyLinks.clear();
    } else {
        qWarning() << "cannot clear keyLinks " << "Object is not of type Key*";
    }
}

// ATT 
// Optional: keyTags
QVariantList Key::keyTagsAsQVariantList()
{
	QVariantList keyTagsList;
	for (int i = 0; i < mKeyTags.size(); ++i) {
        keyTagsList.append((mKeyTags.at(i))->toMap());
    }
	return keyTagsList;
}
QVariantList Key::keyTagsAsForeignQVariantList()
{
	QVariantList keyTagsList;
	for (int i = 0; i < mKeyTags.size(); ++i) {
        keyTagsList.append((mKeyTags.at(i))->toForeignMap());
    }
	return keyTagsList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add keyTags without resolving existing keys before
 * attention: before looping through the objects
 * you must resolveKeyTagsKeys
 */
void Key::addToKeyTags(KeyTag* keyTag)
{
    mKeyTags.append(keyTag);
    emit addedToKeyTags(keyTag);
    emit keyTagsPropertyListChanged();
}
bool Key::removeFromKeyTags(KeyTag* keyTag)
{
    bool ok = false;
    ok = mKeyTags.removeOne(keyTag);
    if (!ok) {
    	qDebug() << "KeyTag* not found in keyTags";
    	return false;
    }
    emit keyTagsPropertyListChanged();
    // keyTags are independent - DON'T delete them
    return true;
}
void Key::clearKeyTags()
{
    for (int i = mKeyTags.size(); i > 0; --i) {
        removeFromKeyTags(mKeyTags.last());
    }
    mKeyTagsKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: keyTagsKeys()
 * - resolve them from DataManager
 * - then resolveKeyTagsKeys()
 */
bool Key::areKeyTagsKeysResolved()
{
    return mKeyTagsKeysResolved;
}
QStringList Key::keyTagsKeys()
{
    return mKeyTagsKeys;
}

/**
 * Objects from keyTagsKeys will be added to existing keyTags
 * This enables to use addToKeyTags() without resolving before
 * Hint: it's your responsibility to resolve before looping thru keyTags
 */
void Key::resolveKeyTagsKeys(QList<KeyTag*> keyTags)
{
    if(mKeyTagsKeysResolved){
        return;
    }
    // don't clear mKeyTags (see above)
    for (int i = 0; i < keyTags.size(); ++i) {
        addToKeyTags(keyTags.at(i));
    }
    mKeyTagsKeysResolved = true;
}
int Key::keyTagsCount()
{
    return mKeyTags.size();
}
QList<KeyTag*> Key::keyTags()
{
	return mKeyTags;
}
void Key::setKeyTags(QList<KeyTag*> keyTags) 
{
	if (keyTags != mKeyTags) {
		mKeyTags = keyTags;
		emit keyTagsChanged(keyTags);
		emit keyTagsPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * key.keyTagsPropertyList.length to get the size
 * key.keyTagsPropertyList[2] to get KeyTag* at position 2
 * key.keyTagsPropertyList = [] to clear the list
 * or get easy access to properties like
 * key.keyTagsPropertyList[2].myPropertyName
 */
QQmlListProperty<KeyTag> Key::keyTagsPropertyList()
{
    return QQmlListProperty<KeyTag>(this, 0, &Key::appendToKeyTagsProperty,
            &Key::keyTagsPropertyCount, &Key::atKeyTagsProperty,
            &Key::clearKeyTagsProperty);
}
void Key::appendToKeyTagsProperty(QQmlListProperty<KeyTag> *keyTagsList, KeyTag* keyTag)
{
    Key *keyObject = qobject_cast<Key *>(keyTagsList->object);
    if (keyObject) {
        keyObject->mKeyTags.append(keyTag);
        emit keyObject->addedToKeyTags(keyTag);
    } else {
        qWarning() << "cannot append KeyTag* to keyTags " << "Object is not of type Key*";
    }
}
int Key::keyTagsPropertyCount(QQmlListProperty<KeyTag> *keyTagsList)
{
    Key *key = qobject_cast<Key *>(keyTagsList->object);
    if (key) {
        return key->mKeyTags.size();
    } else {
        qWarning() << "cannot get size keyTags " << "Object is not of type Key*";
    }
    return 0;
}
KeyTag* Key::atKeyTagsProperty(QQmlListProperty<KeyTag> *keyTagsList, int pos)
{
    Key *key = qobject_cast<Key *>(keyTagsList->object);
    if (key) {
        if (key->mKeyTags.size() > pos) {
            return key->mKeyTags.at(pos);
        }
        qWarning() << "cannot get KeyTag* at pos " << pos << " size is "
                << key->mKeyTags.size();
    } else {
        qWarning() << "cannot get KeyTag* at pos " << pos << "Object is not of type Key*";
    }
    return 0;
}
void Key::clearKeyTagsProperty(QQmlListProperty<KeyTag> *keyTagsList)
{
    Key *key = qobject_cast<Key *>(keyTagsList->object);
    if (key) {
        // keyTags are independent - DON'T delete them
        key->mKeyTags.clear();
    } else {
        qWarning() << "cannot clear keyTags " << "Object is not of type Key*";
    }
}


// ATT
// Optional: keyContentBlocks
QVariantList Key::keyContentBlocksAsQVariantList()
{
    QVariantList keyContentBlocksList;
    for (int i = 0; i < mKeyContentBlocks.size(); ++i) {
        keyContentBlocksList.append((mKeyContentBlocks.at(i))->toMap());
    }
    return keyContentBlocksList;
}
QVariantList Key::keyContentBlocksAsForeignQVariantList()
{
    QVariantList keyContentBlocksList;
    for (int i = 0; i < mKeyContentBlocks.size(); ++i) {
        keyContentBlocksList.append((mKeyContentBlocks.at(i))->toForeignMap());
    }
    return keyContentBlocksList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add keyContentBlocks without resolving existing keys before
 * attention: before looping through the objects
 * you must resolveKeyContentBlocksKeys
 */
void Key::addToKeyContentBlocks(KeyContentBlock* keyContentBlock)
{
    mKeyContentBlocks.append(keyContentBlock);
    emit addedToKeyContentBlocks(keyContentBlock);
    emit keyContentBlocksPropertyListChanged();
}
bool Key::removeFromKeyContentBlocks(KeyContentBlock* keyContentBlock)
{
    bool ok = false;
    ok = mKeyContentBlocks.removeOne(keyContentBlock);
    if (!ok) {
        qDebug() << "KeyContentBlock* not found in keyContentBlocks";
        return false;
    }
    emit keyContentBlocksPropertyListChanged();
    // keyContentBlocks are independent - DON'T delete them
    return true;
}
void Key::clearKeyContentBlocks()
{
    for (int i = mKeyContentBlocks.size(); i > 0; --i) {
        removeFromKeyContentBlocks(mKeyContentBlocks.last());
    }
    mKeyContentBlocksKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: keyContentBlocksKeys()
 * - resolve them from DataManager
 * - then resolveKeyContentBlocksKeys()
 */
bool Key::areKeyContentBlocksKeysResolved()
{
    return mKeyContentBlocksKeysResolved;
}
QStringList Key::keyContentBlocksKeys()
{
    return mKeyContentBlocksKeys;
}

/**
 * Objects from keyContentBlocksKeys will be added to existing keyContentBlocks
 * This enables to use addToKeyContentBlocks() without resolving before
 * Hint: it's your responsibility to resolve before looping thru keyContentBlocks
 */
void Key::resolveKeyContentBlocksKeys(QList<KeyContentBlock*> keyContentBlocks)
{
    if(mKeyContentBlocksKeysResolved){
        return;
    }
    // don't clear mKeyContentBlocks (see above)
    for (int i = 0; i < keyContentBlocks.size(); ++i) {
        addToKeyContentBlocks(keyContentBlocks.at(i));
    }
    mKeyContentBlocksKeysResolved = true;
}
int Key::keyContentBlocksCount()
{
    return mKeyContentBlocks.size();
}
QList<KeyContentBlock*> Key::keyContentBlocks()
{
    return mKeyContentBlocks;
}
void Key::setKeyContentBlocks(QList<KeyContentBlock*> keyContentBlocks)
{
    if (keyContentBlocks != mKeyContentBlocks) {
        mKeyContentBlocks = keyContentBlocks;
        emit keyContentBlocksChanged(keyContentBlocks);
        emit keyContentBlocksPropertyListChanged();
    }
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * key.keyContentBlocksPropertyList.length to get the size
 * key.keyContentBlocksPropertyList[2] to get KeyContentBlock* at position 2
 * key.keyContentBlocksPropertyList = [] to clear the list
 * or get easy access to properties like
 * key.keyContentBlocksPropertyList[2].myPropertyName
 */
QQmlListProperty<KeyContentBlock> Key::keyContentBlocksPropertyList()
{
    return QQmlListProperty<KeyContentBlock>(this, 0, &Key::appendToKeyContentBlocksProperty,
            &Key::keyContentBlocksPropertyCount, &Key::atKeyContentBlocksProperty,
            &Key::clearKeyContentBlocksProperty);
}
void Key::appendToKeyContentBlocksProperty(QQmlListProperty<KeyContentBlock> *keyContentBlocksList, KeyContentBlock* keyContentBlock)
{
    Key *keyObject = qobject_cast<Key *>(keyContentBlocksList->object);
    if (keyObject) {
        keyObject->mKeyContentBlocks.append(keyContentBlock);
        emit keyObject->addedToKeyContentBlocks(keyContentBlock);
    } else {
        qWarning() << "cannot append KeyContentBlock* to keyContentBlocks " << "Object is not of type Key*";
    }
}
int Key::keyContentBlocksPropertyCount(QQmlListProperty<KeyContentBlock> *keyContentBlocksList)
{
    Key *key = qobject_cast<Key *>(keyContentBlocksList->object);
    if (key) {
        return key->mKeyContentBlocks.size();
    } else {
        qWarning() << "cannot get size keyContentBlocks " << "Object is not of type Key*";
    }
    return 0;
}
KeyContentBlock* Key::atKeyContentBlocksProperty(QQmlListProperty<KeyContentBlock> *keyContentBlocksList, int pos)
{
    Key *key = qobject_cast<Key *>(keyContentBlocksList->object);
    if (key) {
        if (key->mKeyContentBlocks.size() > pos) {
            return key->mKeyContentBlocks.at(pos);
        }
        qWarning() << "cannot get KeyContentBlock* at pos " << pos << " size is "
                << key->mKeyContentBlocks.size();
    } else {
        qWarning() << "cannot get KeyContentBlock* at pos " << pos << "Object is not of type Key*";
    }
    return 0;
}
void Key::clearKeyContentBlocksProperty(QQmlListProperty<KeyContentBlock> *keyContentBlocksList)
{
    Key *key = qobject_cast<Key *>(keyContentBlocksList->object);
    if (key) {
        // keyContentBlocks are independent - DON'T delete them
        key->mKeyContentBlocks.clear();
    } else {
        qWarning() << "cannot clear keyContentBlocks " << "Object is not of type Key*";
    }
}


// ATT
// Optional: keyContentStandardTypes
QVariantList Key::keyContentStandardTypesAsQVariantList()
{
    QVariantList keyContentStandardTypesList;
    for (int i = 0; i < mKeyContentStandardTypes.size(); ++i) {
        keyContentStandardTypesList.append((mKeyContentStandardTypes.at(i))->toMap());
    }
    return keyContentStandardTypesList;
}
QVariantList Key::keyContentStandardTypesAsForeignQVariantList()
{
    QVariantList keyContentStandardTypesList;
    for (int i = 0; i < mKeyContentStandardTypes.size(); ++i) {
        keyContentStandardTypesList.append((mKeyContentStandardTypes.at(i))->toForeignMap());
    }
    return keyContentStandardTypesList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add keyContentStandardTypes without resolving existing keys before
 * attention: before looping through the objects
 * you must resolveKeyContentStandardTypesKeys
 */
void Key::addToKeyContentStandardTypes(KeyContentStandardType* keyContentStandardType)
{
    mKeyContentStandardTypes.append(keyContentStandardType);
    emit addedToKeyContentStandardTypes(keyContentStandardType);
    emit keyContentStandardTypesPropertyListChanged();
}
bool Key::removeFromKeyContentStandardTypes(KeyContentStandardType* keyContentStandardType)
{
    bool ok = false;
    ok = mKeyContentStandardTypes.removeOne(keyContentStandardType);
    if (!ok) {
        qDebug() << "KeyContentStandardType* not found in keyContentStandardTypes";
        return false;
    }
    emit keyContentStandardTypesPropertyListChanged();
    // keyContentStandardTypes are independent - DON'T delete them
    return true;
}
void Key::clearKeyContentStandardTypes()
{
    for (int i = mKeyContentStandardTypes.size(); i > 0; --i) {
        removeFromKeyContentStandardTypes(mKeyContentStandardTypes.last());
    }
    mKeyContentStandardTypesKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: keyContentStandardTypesKeys()
 * - resolve them from DataManager
 * - then resolveKeyContentStandardTypesKeys()
 */
bool Key::areKeyContentStandardTypesKeysResolved()
{
    return mKeyContentStandardTypesKeysResolved;
}
QStringList Key::keyContentStandardTypesKeys()
{
    return mKeyContentStandardTypesKeys;
}

/**
 * Objects from keyContentStandardTypesKeys will be added to existing keyContentStandardTypes
 * This enables to use addToKeyContentStandardTypes() without resolving before
 * Hint: it's your responsibility to resolve before looping thru keyContentStandardTypes
 */
void Key::resolveKeyContentStandardTypesKeys(QList<KeyContentStandardType*> keyContentStandardTypes)
{
    if(mKeyContentStandardTypesKeysResolved){
        return;
    }
    // don't clear mKeyContentStandardTypes (see above)
    for (int i = 0; i < keyContentStandardTypes.size(); ++i) {
        addToKeyContentStandardTypes(keyContentStandardTypes.at(i));
    }
    mKeyContentStandardTypesKeysResolved = true;
}
int Key::keyContentStandardTypesCount()
{
    return mKeyContentStandardTypes.size();
}
QList<KeyContentStandardType*> Key::keyContentStandardTypes()
{
    return mKeyContentStandardTypes;
}
void Key::setKeyContentStandardTypes(QList<KeyContentStandardType*> keyContentStandardTypes)
{
    if (keyContentStandardTypes != mKeyContentStandardTypes) {
        mKeyContentStandardTypes = keyContentStandardTypes;
        emit keyContentStandardTypesChanged(keyContentStandardTypes);
        emit keyContentStandardTypesPropertyListChanged();
    }
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * key.keyContentStandardTypesPropertyList.length to get the size
 * key.keyContentStandardTypesPropertyList[2] to get KeyContentStandardType* at position 2
 * key.keyContentStandardTypesPropertyList = [] to clear the list
 * or get easy access to properties like
 * key.keyContentStandardTypesPropertyList[2].myPropertyName
 */
QQmlListProperty<KeyContentStandardType> Key::keyContentStandardTypesPropertyList()
{
    return QQmlListProperty<KeyContentStandardType>(this, 0, &Key::appendToKeyContentStandardTypesProperty,
            &Key::keyContentStandardTypesPropertyCount, &Key::atKeyContentStandardTypesProperty,
            &Key::clearKeyContentStandardTypesProperty);
}
void Key::appendToKeyContentStandardTypesProperty(QQmlListProperty<KeyContentStandardType> *keyContentStandardTypesList, KeyContentStandardType* keyContentStandardType)
{
    Key *keyObject = qobject_cast<Key *>(keyContentStandardTypesList->object);
    if (keyObject) {
        keyObject->mKeyContentStandardTypes.append(keyContentStandardType);
        emit keyObject->addedToKeyContentStandardTypes(keyContentStandardType);
    } else {
        qWarning() << "cannot append KeyContentStandardType* to keyContentStandardTypes " << "Object is not of type Key*";
    }
}
int Key::keyContentStandardTypesPropertyCount(QQmlListProperty<KeyContentStandardType> *keyContentStandardTypesList)
{
    Key *key = qobject_cast<Key *>(keyContentStandardTypesList->object);
    if (key) {
        return key->mKeyContentStandardTypes.size();
    } else {
        qWarning() << "cannot get size keyContentStandardTypes " << "Object is not of type Key*";
    }
    return 0;
}
KeyContentStandardType* Key::atKeyContentStandardTypesProperty(QQmlListProperty<KeyContentStandardType> *keyContentStandardTypesList, int pos)
{
    Key *key = qobject_cast<Key *>(keyContentStandardTypesList->object);
    if (key) {
        if (key->mKeyContentStandardTypes.size() > pos) {
            return key->mKeyContentStandardTypes.at(pos);
        }
        qWarning() << "cannot get KeyContentStandardType* at pos " << pos << " size is "
                << key->mKeyContentStandardTypes.size();
    } else {
        qWarning() << "cannot get KeyContentStandardType* at pos " << pos << "Object is not of type Key*";
    }
    return 0;
}
void Key::clearKeyContentStandardTypesProperty(QQmlListProperty<KeyContentStandardType> *keyContentStandardTypesList)
{
    Key *key = qobject_cast<Key *>(keyContentStandardTypesList->object);
    if (key) {
        // keyContentStandardTypes are independent - DON'T delete them
        key->mKeyContentStandardTypes.clear();
    } else {
        qWarning() << "cannot clear keyContentStandardTypes " << "Object is not of type Key*";
    }
}


Key::~Key()
{
	// place cleanUp code here
}
	
