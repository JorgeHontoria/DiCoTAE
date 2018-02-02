#include "KeyAPI.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString keyIdKey = "keyId";
static const QString keyIdRefViewKey = "keyIdRefView";

static const QString titleKey = "title";
static const QString subtitleKey = "subtitle";
static const QString descriptionKey = "description";
static const QString keyTypeKey = "keyType";
static const QString contentStandardNameKey = "contentStandardName";
static const QString contentArticulo1Key = "contentArticulo1";
static const QString contentSituacion1Key = "contentSituacion1";
static const QString contentLinkKey = "contentLink";
static const QString startTimeKey = "startTime";
static const QString start_timeKey = "start_time";
static const QString sectionKey = "section";
static const QString keyTagsKey = "keyTags";
static const QString keyContentBlocksKey = "keyContentBlocks";
static const QString keyContentStandardTypesKey = "keyContentStandardTypes";
static const QString creatorKey = "creator";
static const QString keyLinksKey = "keyLinks";

// keys used from Server API etc
static const QString keyIdForeignKey = "id";
static const QString keyIdRefViewForeignKey = "idRefView";

static const QString titleForeignKey = "title";
static const QString subtitleForeignKey = "subtitle";
static const QString descriptionForeignKey = "description";
static const QString keyTypeForeignKey = "type";
static const QString contentStandardNameForeignKey = "contentStandardName";
static const QString contentArticulo1ForeignKey = "contentArticulo1";
static const QString contentSituacion1ForeignKey = "contentSituacion1";
static const QString contentLinkForeignKey = "contentLink";
static const QString startForeignKey = "start";
static const QString startTimeForeignKey = "start_time";
static const QString sectionForeignKey = "section";

static const QString keyTagsForeignKey = "tags";
static const QString keyContentBlocksForeignKey = "contentBlocks";
static const QString keyContentStandardTypesForeignKey = "contentStandardTypes";

static const QString creatorForeignKey = "creators";
static const QString keyLinksForeignKey = "links";

/*
 * Default Constructor if KeyAPI not initialized from QVariantMap
 */
KeyAPI::KeyAPI(QObject *parent) :
        QObject(parent), mKeyId(-1), mKeyIdRefView(-1), mTitle(""), mSubtitle(""), mDescription(""), mKeyType(""), mContentStandardName(""), mContentArticulo1(""), mContentSituacion1(""), mDuration(""), mContentLink(""), mContentSection("")
{
	// Date, Time or Timestamp ? construct null value
	mStartTime = QTime();
    // lazy Arrays where only keys are persisted
    mCreatorKeysResolved = false;
    mKeyLinksKeysResolved = false;
}

bool KeyAPI::isAllResolved()
{
    if(!areCreatorKeysResolved()) {
        return false;
    }
    if(!areKeyLinksKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize KeyAPI from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void KeyAPI::fillFromMap(const QVariantMap& keyAPIMap)
{
	mKeyId = keyAPIMap.value(keyIdKey).toInt();
    mKeyIdRefView = keyAPIMap.value(keyIdRefViewKey).toInt();

	mTitle = keyAPIMap.value(titleKey).toString();
	mSubtitle = keyAPIMap.value(subtitleKey).toString();
	mDescription = keyAPIMap.value(descriptionKey).toString();
	mKeyType = keyAPIMap.value(keyTypeKey).toString();
    mContentStandardName = keyAPIMap.value(contentStandardNameKey).toString();
    mContentArticulo1 = keyAPIMap.value(contentArticulo1Key).toString();
    mContentSituacion1 = keyAPIMap.value(contentSituacion1Key).toString();
    mContentLink = keyAPIMap.value(contentLinkKey).toString();
	if (keyAPIMap.contains(startTimeKey)) {
		// always getting the Date as a String (from server or JSON)
		QString startTimeAsString = keyAPIMap.value(startTimeKey).toString();
		mStartTime = QTime::fromString(startTimeAsString, "HH:mm");
		if (!mStartTime.isValid()) {
			mStartTime = QTime();
			qDebug() << "mStartTime is not valid for String: " << startTimeAsString;
		}
	}
	mDuration = keyAPIMap.value(start_timeKey).toString();
	mContentSection = keyAPIMap.value(sectionKey).toString();
	// mCreator is (lazy loaded) Array of PersonsAPI*
	mCreatorKeys = keyAPIMap.value(creatorKey).toStringList();
	// mCreator must be resolved later if there are keys
	mCreatorKeysResolved = (mCreatorKeys.size() == 0);
	mCreator.clear();
	// mKeyLinks is (lazy loaded) Array of KeyLinkAPI*
	mKeyLinksKeys = keyAPIMap.value(keyLinksKey).toStringList();
	// mKeyLinks must be resolved later if there are keys
	mKeyLinksKeysResolved = (mKeyLinksKeys.size() == 0);
	mKeyLinks.clear();
	mKeyTagsStringList = keyAPIMap.value(keyTagsKey).toStringList();
    mKeyContentBlocksStringList = keyAPIMap.value(keyContentBlocksKey).toStringList();
    mKeyContentStandardTypesStringList = keyAPIMap.value(keyContentStandardTypesKey).toStringList();

}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void KeyAPI::fillFromForeignMap(const QVariantMap& keyAPIMap)
{
	mKeyId = keyAPIMap.value(keyIdForeignKey).toInt();
    mKeyIdRefView = keyAPIMap.value(keyIdRefViewForeignKey).toInt();

	mTitle = keyAPIMap.value(titleForeignKey).toString();
	mSubtitle = keyAPIMap.value(subtitleForeignKey).toString();
	mDescription = keyAPIMap.value(descriptionForeignKey).toString();
	mKeyType = keyAPIMap.value(keyTypeForeignKey).toString();
    mContentStandardName = keyAPIMap.value(contentStandardNameForeignKey).toString();
    mContentArticulo1 = keyAPIMap.value(contentArticulo1ForeignKey).toString();
    mContentSituacion1 = keyAPIMap.value(contentSituacion1ForeignKey).toString();
    mContentLink = keyAPIMap.value(contentLinkForeignKey).toString();
    if (keyAPIMap.contains(startForeignKey)) {
		// always getting the Date as a String (from server or JSON)
        QString startTimeAsString = keyAPIMap.value(startForeignKey).toString();
		mStartTime = QTime::fromString(startTimeAsString, "HH:mm");
		if (!mStartTime.isValid()) {
			mStartTime = QTime();
			qDebug() << "mStartTime is not valid for String: " << startTimeAsString;
		}
	}
    mDuration = keyAPIMap.value(startTimeForeignKey).toString();
	mContentSection = keyAPIMap.value(sectionForeignKey).toString();
	// mCreator is (lazy loaded) Array of PersonsAPI*
	mCreatorKeys = keyAPIMap.value(creatorForeignKey).toStringList();
	// mCreator must be resolved later if there are keys
	mCreatorKeysResolved = (mCreatorKeys.size() == 0);
	mCreator.clear();
	// mKeyLinks is (lazy loaded) Array of KeyLinkAPI*
	mKeyLinksKeys = keyAPIMap.value(keyLinksForeignKey).toStringList();
	// mKeyLinks must be resolved later if there are keys
	mKeyLinksKeysResolved = (mKeyLinksKeys.size() == 0);
	mKeyLinks.clear();

	mKeyTagsStringList = keyAPIMap.value(keyTagsForeignKey).toStringList();
    mKeyContentBlocksStringList = keyAPIMap.value(keyContentBlocksForeignKey).toStringList();
    mKeyContentStandardTypesStringList = keyAPIMap.value(keyContentStandardTypesForeignKey).toStringList();

}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void KeyAPI::fillFromCacheMap(const QVariantMap& keyAPIMap)
{
	mKeyId = keyAPIMap.value(keyIdKey).toInt();
    mKeyIdRefView = keyAPIMap.value(keyIdRefViewKey).toInt();

	mTitle = keyAPIMap.value(titleKey).toString();
	mSubtitle = keyAPIMap.value(subtitleKey).toString();
	mDescription = keyAPIMap.value(descriptionKey).toString();
	mKeyType = keyAPIMap.value(keyTypeKey).toString();
    mContentStandardName = keyAPIMap.value(contentStandardNameKey).toString();
    mContentArticulo1 = keyAPIMap.value(contentArticulo1Key).toString();
    mContentSituacion1 = keyAPIMap.value(contentSituacion1Key).toString();
    mContentLink = keyAPIMap.value(contentLinkKey).toString();
	if (keyAPIMap.contains(startTimeKey)) {
		// always getting the Date as a String (from server or JSON)
		QString startTimeAsString = keyAPIMap.value(startTimeKey).toString();
		mStartTime = QTime::fromString(startTimeAsString, "HH:mm");
		if (!mStartTime.isValid()) {
			mStartTime = QTime();
			qDebug() << "mStartTime is not valid for String: " << startTimeAsString;
		}
	}
	mDuration = keyAPIMap.value(start_timeKey).toString();
	mContentSection = keyAPIMap.value(sectionKey).toString();
	// mCreator is (lazy loaded) Array of PersonsAPI*
	mCreatorKeys = keyAPIMap.value(creatorKey).toStringList();
	// mCreator must be resolved later if there are keys
	mCreatorKeysResolved = (mCreatorKeys.size() == 0);
	mCreator.clear();
	// mKeyLinks is (lazy loaded) Array of KeyLinkAPI*
	mKeyLinksKeys = keyAPIMap.value(keyLinksKey).toStringList();
	// mKeyLinks must be resolved later if there are keys
	mKeyLinksKeysResolved = (mKeyLinksKeys.size() == 0);
	mKeyLinks.clear();

	mKeyTagsStringList = keyAPIMap.value(keyTagsKey).toStringList();
    mKeyContentBlocksStringList = keyAPIMap.value(keyContentBlocksKey).toStringList();
    mKeyContentStandardTypesStringList = keyAPIMap.value(keyContentStandardTypesKey).toStringList();
}

void KeyAPI::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool KeyAPI::isValid()
{
	if (mKeyId == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from KeyAPI as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap KeyAPI::toMap()
{
	QVariantMap keyAPIMap;
	// mCreator points to PersonsAPI*
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
		PersonsAPI* creatorsAPI;
		creatorsAPI = mCreator.at(i);
		mCreatorKeys << QString::number(creatorsAPI->creatorId());
	}
	keyAPIMap.insert(creatorKey, mCreatorKeys);
	// mKeyLinks points to KeyLinkAPI*
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
		KeyLinkAPI* keyLinkAPI;
		keyLinkAPI = mKeyLinks.at(i);
		mKeyLinksKeys << keyLinkAPI->uuid();
	}
	keyAPIMap.insert(keyLinksKey, mKeyLinksKeys);

	keyAPIMap.insert(keyIdKey, mKeyId);
    keyAPIMap.insert(keyIdRefViewKey, mKeyIdRefView);

	keyAPIMap.insert(titleKey, mTitle);
	keyAPIMap.insert(subtitleKey, mSubtitle);
	keyAPIMap.insert(descriptionKey, mDescription);
	keyAPIMap.insert(keyTypeKey, mKeyType);
    keyAPIMap.insert(contentStandardNameKey, mContentStandardName);
    keyAPIMap.insert(contentArticulo1Key, mContentArticulo1);
    keyAPIMap.insert(contentSituacion1Key, mContentSituacion1);
    keyAPIMap.insert(contentLinkKey, mContentLink);
    if (hasStartTime()) {
		keyAPIMap.insert(startTimeKey, mStartTime.toString("HH:mm"));
	}
	keyAPIMap.insert(start_timeKey, mDuration);
	keyAPIMap.insert(sectionKey, mContentSection);
	// Array of QString
	keyAPIMap.insert(keyTagsKey, mKeyTagsStringList);
    keyAPIMap.insert(keyContentBlocksKey, mKeyContentBlocksStringList);
    keyAPIMap.insert(keyContentStandardTypesKey, mKeyContentStandardTypesStringList);

	return keyAPIMap;
}

/*
 * Exports Properties from KeyAPI as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap KeyAPI::toForeignMap()
{
	QVariantMap keyAPIMap;
	// mCreator points to PersonsAPI*
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
		PersonsAPI* creatorsAPI;
		creatorsAPI = mCreator.at(i);
		mCreatorKeys << QString::number(creatorsAPI->creatorId());
	}
	keyAPIMap.insert(creatorKey, mCreatorKeys);
	// mKeyLinks points to KeyLinkAPI*
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
		KeyLinkAPI* keyLinkAPI;
		keyLinkAPI = mKeyLinks.at(i);
		mKeyLinksKeys << keyLinkAPI->uuid();
	}
	keyAPIMap.insert(keyLinksKey, mKeyLinksKeys);

	keyAPIMap.insert(keyIdForeignKey, mKeyId);
    keyAPIMap.insert(keyIdRefViewForeignKey, mKeyIdRefView);

	keyAPIMap.insert(titleForeignKey, mTitle);
	keyAPIMap.insert(subtitleForeignKey, mSubtitle);
	keyAPIMap.insert(descriptionForeignKey, mDescription);
	keyAPIMap.insert(keyTypeForeignKey, mKeyType);
    keyAPIMap.insert(contentStandardNameForeignKey, mContentStandardName);
    keyAPIMap.insert(contentArticulo1ForeignKey, mContentArticulo1);
    keyAPIMap.insert(contentSituacion1ForeignKey, mContentSituacion1);
    keyAPIMap.insert(contentLinkForeignKey, mContentLink);
    if (hasStartTime()) {
        keyAPIMap.insert(startForeignKey, mStartTime.toString("HH:mm"));
	}
    keyAPIMap.insert(startTimeForeignKey, mDuration);
	keyAPIMap.insert(sectionForeignKey, mContentSection);
	// Array of QString
	keyAPIMap.insert(keyTagsForeignKey, mKeyTagsStringList);
    keyAPIMap.insert(keyContentBlocksForeignKey, mKeyContentBlocksStringList);
    keyAPIMap.insert(keyContentStandardTypesForeignKey, mKeyContentStandardTypesStringList);

	return keyAPIMap;
}


/*
 * Exports Properties from KeyAPI as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap KeyAPI::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: keyId
// Domain KEY: keyId
int KeyAPI::keyId() const
{
	return mKeyId;
}

void KeyAPI::setKeyId(int keyId)
{
	if (keyId != mKeyId) {
		mKeyId = keyId;
		emit keyIdChanged(keyId);
	}
}
// ATT
// Mandatory: keyIdRefView
// Domain KEY: keyIdRefView
int KeyAPI::keyIdRefView() const
{
    return mKeyIdRefView;
}

void KeyAPI::setKeyIdRefView(int keyIdRefView)
{
    if (keyIdRefView != mKeyIdRefView) {
        mKeyIdRefView = keyIdRefView;
        emit keyIdRefViewChanged(keyIdRefView);
    }
}
// ATT 
// Optional: title
QString KeyAPI::title() const
{
	return mTitle;
}

void KeyAPI::setTitle(QString title)
{
	if (title != mTitle) {
		mTitle = title;
		emit titleChanged(title);
	}
}
// ATT 
// Optional: subtitle
QString KeyAPI::subtitle() const
{
	return mSubtitle;
}

void KeyAPI::setSubtitle(QString subtitle)
{
	if (subtitle != mSubtitle) {
		mSubtitle = subtitle;
		emit subtitleChanged(subtitle);
	}
}
// ATT 
// Optional: description
QString KeyAPI::description() const
{
	return mDescription;
}

void KeyAPI::setDescription(QString description)
{
	if (description != mDescription) {
		mDescription = description;
		emit descriptionChanged(description);
	}
}
// ATT 
// Optional: keyType
QString KeyAPI::keyType() const
{
	return mKeyType;
}

void KeyAPI::setKeyType(QString keyType)
{
	if (keyType != mKeyType) {
		mKeyType = keyType;
		emit keyTypeChanged(keyType);
	}
}
// ATT
// Optional: contentStandardName
QString KeyAPI::contentStandardName() const
{
    return mContentStandardName;
}

void KeyAPI::setContentStandardName(QString contentStandardName)
{
    if (contentStandardName != mContentStandardName) {
        mContentStandardName = contentStandardName;
        emit contentStandardNameChanged(contentStandardName);
    }
}
// ATT
// Optional: contentArticulo1
QString KeyAPI::contentArticulo1() const
{
    return mContentArticulo1;
}

void KeyAPI::setContentArticulo1(QString contentArticulo1)
{
    if (contentArticulo1 != mContentArticulo1) {
        mContentArticulo1 = contentArticulo1;
        emit contentArticulo1Changed(contentArticulo1);
    }
}
// ATT
// Optional: contentSituacion1
QString KeyAPI::contentSituacion1() const
{
    return mContentSituacion1;
}

void KeyAPI::setContentSituacion1(QString contentSituacion1)
{
    if (contentSituacion1 != mContentSituacion1) {
        mContentSituacion1 = contentSituacion1;
        emit contentSituacion1Changed(contentSituacion1);
    }
}
// ATT
// Optional: contentLink
QString KeyAPI::contentLink() const
{
    return mContentLink;
}

void KeyAPI::setContentLink(QString contentLink)
{
    if (contentLink != mContentLink) {
        mContentLink = contentLink;
        emit contentLinkChanged(contentLink);
    }
}
// ATT 
// Optional: startTime
/**
 * in QML set DateTimePicker value this way:
 * myPicker.value = myPicker.dateFromTime(startTime)
 */
QTime KeyAPI::startTime() const
{
	return mStartTime;
}

void KeyAPI::setStartTime(QTime startTime)
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
void KeyAPI::setStartTimeFromPickerValue(QString startTimeValue)
{
    QTime startTime = QTime::fromString(startTimeValue.left(8), "HH:mm:ss");
    if (startTime != mStartTime) {
        mStartTime = startTime;
        emit startTimeChanged(startTime);
    }
}
bool KeyAPI::hasStartTime()
{
	return !mStartTime.isNull() && mStartTime.isValid();
}
// ATT 
// Optional: start_time
QString KeyAPI::start_time() const
{
	return mDuration;
}

void KeyAPI::setDuration(QString start_time)
{
	if (start_time != mDuration) {
		mDuration = start_time;
		emit start_timeChanged(start_time);
	}
}
// ATT 
// Optional: section
QString KeyAPI::section() const
{
	return mContentSection;
}

void KeyAPI::setSection(QString section)
{
	if (section != mContentSection) {
		mContentSection = section;
		emit sectionChanged(section);
	}
}
// ATT 
// Optional: keyTags
void KeyAPI::addToKeyTagsStringList(const QString& stringValue)
{
    mKeyTagsStringList.append(stringValue);
    emit addedToKeyTagsStringList(stringValue);
}
bool KeyAPI::removeFromKeyTagsStringList(const QString& stringValue)
{
    bool ok = false;
    ok = mKeyTagsStringList.removeOne(stringValue);
    if (!ok) {
    	qDebug() << "QString& not found in mKeyTagsStringList: " << stringValue;
    	return false;
    }
    emit removedFromKeyTagsStringList(stringValue);
    return true;
}
int KeyAPI::keyTagsCount()
{
    return mKeyTagsStringList.size();
}
QStringList KeyAPI::keyTagsStringList()
{
	return mKeyTagsStringList;
}
void KeyAPI::setKeyTagsStringList(const QStringList& keyTags) 
{
	if (keyTags != mKeyTagsStringList) {
		mKeyTagsStringList = keyTags;
		emit keyTagsStringListChanged(keyTags);
	}
}
// ATT
// Optional: keyContentBlocks
void KeyAPI::addToKeyContentBlocksStringList(const QString& stringValue)
{
    mKeyContentBlocksStringList.append(stringValue);
    emit addedToKeyContentBlocksStringList(stringValue);
}
bool KeyAPI::removeFromKeyContentBlocksStringList(const QString& stringValue)
{
    bool ok = false;
    ok = mKeyContentBlocksStringList.removeOne(stringValue);
    if (!ok) {
        qDebug() << "QString& not found in mKeyContentBlocksStringList: " << stringValue;
        return false;
    }
    emit removedFromKeyContentBlocksStringList(stringValue);
    return true;
}
int KeyAPI::keyContentBlocksCount()
{
    return mKeyContentBlocksStringList.size();
}
QStringList KeyAPI::keyContentBlocksStringList()
{
    return mKeyContentBlocksStringList;
}
void KeyAPI::setKeyContentBlocksStringList(const QStringList& keyContentBlocks)
{
    if (keyContentBlocks != mKeyContentBlocksStringList) {
        mKeyContentBlocksStringList = keyContentBlocks;
        emit keyContentBlocksStringListChanged(keyContentBlocks);
    }
}
// ATT
// Optional: keyContentStandardTypes
void KeyAPI::addToKeyContentStandardTypesStringList(const QString& stringValue)
{
    mKeyContentStandardTypesStringList.append(stringValue);
    emit addedToKeyContentStandardTypesStringList(stringValue);
}
bool KeyAPI::removeFromKeyContentStandardTypesStringList(const QString& stringValue)
{
    bool ok = false;
    ok = mKeyContentStandardTypesStringList.removeOne(stringValue);
    if (!ok) {
        qDebug() << "QString& not found in mKeyContentStandardTypesStringList: " << stringValue;
        return false;
    }
    emit removedFromKeyContentStandardTypesStringList(stringValue);
    return true;
}
int KeyAPI::keyContentStandardTypesCount()
{
    return mKeyContentStandardTypesStringList.size();
}
QStringList KeyAPI::keyContentStandardTypesStringList()
{
    return mKeyContentStandardTypesStringList;
}
void KeyAPI::setKeyContentStandardTypesStringList(const QStringList& keyContentStandardTypes)
{
    if (keyContentStandardTypes != mKeyContentStandardTypesStringList) {
        mKeyContentStandardTypesStringList = keyContentStandardTypes;
        emit keyContentStandardTypesStringListChanged(keyContentStandardTypes);
    }
}

// ATT 
// Optional: creator
QVariantList KeyAPI::creatorAsQVariantList()
{
	QVariantList creatorList;
	for (int i = 0; i < mCreator.size(); ++i) {
        creatorList.append((mCreator.at(i))->toMap());
    }
	return creatorList;
}
QVariantList KeyAPI::creatorAsForeignQVariantList()
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
void KeyAPI::addToCreator(PersonsAPI* creatorsAPI)
{
    mCreator.append(creatorsAPI);
    emit addedToCreator(creatorsAPI);
    emit creatorPropertyListChanged();
}

bool KeyAPI::removeFromCreator(PersonsAPI* creatorsAPI)
{
    bool ok = false;
    ok = mCreator.removeOne(creatorsAPI);
    if (!ok) {
    	qDebug() << "PersonsAPI* not found in creator";
    	return false;
    }
    emit creatorPropertyListChanged();
    // creator are independent - DON'T delete them
    return true;
}
void KeyAPI::clearCreator()
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
bool KeyAPI::areCreatorKeysResolved()
{
    return mCreatorKeysResolved;
}

QStringList KeyAPI::creatorKeys()
{
    return mCreatorKeys;
}

/**
 * Objects from creatorKeys will be added to existing creator
 * This enables to use addToCreator() without resolving before
 * Hint: it's your responsibility to resolve before looping thru creator
 */
void KeyAPI::resolveCreatorKeys(QList<PersonsAPI*> creator)
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

int KeyAPI::creatorCount()
{
    return mCreator.size();
}
QList<PersonsAPI*> KeyAPI::creator()
{
	return mCreator;
}
void KeyAPI::setCreator(QList<PersonsAPI*> creator) 
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
 * keyAPI.creatorPropertyList.length to get the size
 * keyAPI.creatorPropertyList[2] to get PersonsAPI* at position 2
 * keyAPI.creatorPropertyList = [] to clear the list
 * or get easy access to properties like
 * keyAPI.creatorPropertyList[2].myPropertyName
 */
QQmlListProperty<PersonsAPI> KeyAPI::creatorPropertyList()
{
    return QQmlListProperty<PersonsAPI>(this, 0, &KeyAPI::appendToCreatorProperty,
            &KeyAPI::creatorPropertyCount, &KeyAPI::atCreatorProperty,
            &KeyAPI::clearCreatorProperty);
}
void KeyAPI::appendToCreatorProperty(QQmlListProperty<PersonsAPI> *creatorList,
        PersonsAPI* creatorsAPI)
{
    KeyAPI *keyAPIObject = qobject_cast<KeyAPI *>(creatorList->object);
    if (keyAPIObject) {
        keyAPIObject->mCreator.append(creatorsAPI);
        emit keyAPIObject->addedToCreator(creatorsAPI);
    } else {
        qWarning() << "cannot append PersonsAPI* to creator " << "Object is not of type KeyAPI*";
    }
}
int KeyAPI::creatorPropertyCount(QQmlListProperty<PersonsAPI> *creatorList)
{
    KeyAPI *keyAPI = qobject_cast<KeyAPI *>(creatorList->object);
    if (keyAPI) {
        return keyAPI->mCreator.size();
    } else {
        qWarning() << "cannot get size creator " << "Object is not of type KeyAPI*";
    }
    return 0;
}
PersonsAPI* KeyAPI::atCreatorProperty(QQmlListProperty<PersonsAPI> *creatorList, int pos)
{
    KeyAPI *keyAPI = qobject_cast<KeyAPI *>(creatorList->object);
    if (keyAPI) {
        if (keyAPI->mCreator.size() > pos) {
            return keyAPI->mCreator.at(pos);
        }
        qWarning() << "cannot get PersonsAPI* at pos " << pos << " size is "
                << keyAPI->mCreator.size();
    } else {
        qWarning() << "cannot get PersonsAPI* at pos " << pos << "Object is not of type KeyAPI*";
    }
    return 0;
}
void KeyAPI::clearCreatorProperty(QQmlListProperty<PersonsAPI> *creatorList)
{
    KeyAPI *keyAPI = qobject_cast<KeyAPI *>(creatorList->object);
    if (keyAPI) {
        // creator are independent - DON'T delete them
        keyAPI->mCreator.clear();
    } else {
        qWarning() << "cannot clear creator " << "Object is not of type KeyAPI*";
    }
}

// ATT 
// Optional: keyLinks
QVariantList KeyAPI::keyLinksAsQVariantList()
{
	QVariantList keyLinksList;
	for (int i = 0; i < mKeyLinks.size(); ++i) {
        keyLinksList.append((mKeyLinks.at(i))->toMap());
    }
	return keyLinksList;
}
QVariantList KeyAPI::keyLinksAsForeignQVariantList()
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
void KeyAPI::addToKeyLinks(KeyLinkAPI* keyLinkAPI)
{
    mKeyLinks.append(keyLinkAPI);
    emit addedToKeyLinks(keyLinkAPI);
    emit keyLinksPropertyListChanged();
}

bool KeyAPI::removeFromKeyLinks(KeyLinkAPI* keyLinkAPI)
{
    bool ok = false;
    ok = mKeyLinks.removeOne(keyLinkAPI);
    if (!ok) {
    	qDebug() << "KeyLinkAPI* not found in keyLinks";
    	return false;
    }
    emit keyLinksPropertyListChanged();
    // keyLinks are independent - DON'T delete them
    return true;
}
void KeyAPI::clearKeyLinks()
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
bool KeyAPI::areKeyLinksKeysResolved()
{
    return mKeyLinksKeysResolved;
}

QStringList KeyAPI::keyLinksKeys()
{
    return mKeyLinksKeys;
}

/**
 * Objects from keyLinksKeys will be added to existing keyLinks
 * This enables to use addToKeyLinks() without resolving before
 * Hint: it's your responsibility to resolve before looping thru keyLinks
 */
void KeyAPI::resolveKeyLinksKeys(QList<KeyLinkAPI*> keyLinks)
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

int KeyAPI::keyLinksCount()
{
    return mKeyLinks.size();
}
QList<KeyLinkAPI*> KeyAPI::keyLinks()
{
	return mKeyLinks;
}
void KeyAPI::setKeyLinks(QList<KeyLinkAPI*> keyLinks) 
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
 * keyAPI.keyLinksPropertyList.length to get the size
 * keyAPI.keyLinksPropertyList[2] to get KeyLinkAPI* at position 2
 * keyAPI.keyLinksPropertyList = [] to clear the list
 * or get easy access to properties like
 * keyAPI.keyLinksPropertyList[2].myPropertyName
 */
QQmlListProperty<KeyLinkAPI> KeyAPI::keyLinksPropertyList()
{
    return QQmlListProperty<KeyLinkAPI>(this, 0, &KeyAPI::appendToKeyLinksProperty,
            &KeyAPI::keyLinksPropertyCount, &KeyAPI::atKeyLinksProperty,
            &KeyAPI::clearKeyLinksProperty);
}
void KeyAPI::appendToKeyLinksProperty(QQmlListProperty<KeyLinkAPI> *keyLinksList,
        KeyLinkAPI* keyLinkAPI)
{
    KeyAPI *keyAPIObject = qobject_cast<KeyAPI *>(keyLinksList->object);
    if (keyAPIObject) {
        keyAPIObject->mKeyLinks.append(keyLinkAPI);
        emit keyAPIObject->addedToKeyLinks(keyLinkAPI);
    } else {
        qWarning() << "cannot append KeyLinkAPI* to keyLinks " << "Object is not of type KeyAPI*";
    }
}
int KeyAPI::keyLinksPropertyCount(QQmlListProperty<KeyLinkAPI> *keyLinksList)
{
    KeyAPI *keyAPI = qobject_cast<KeyAPI *>(keyLinksList->object);
    if (keyAPI) {
        return keyAPI->mKeyLinks.size();
    } else {
        qWarning() << "cannot get size keyLinks " << "Object is not of type KeyAPI*";
    }
    return 0;
}
KeyLinkAPI* KeyAPI::atKeyLinksProperty(QQmlListProperty<KeyLinkAPI> *keyLinksList, int pos)
{
    KeyAPI *keyAPI = qobject_cast<KeyAPI *>(keyLinksList->object);
    if (keyAPI) {
        if (keyAPI->mKeyLinks.size() > pos) {
            return keyAPI->mKeyLinks.at(pos);
        }
        qWarning() << "cannot get KeyLinkAPI* at pos " << pos << " size is "
                << keyAPI->mKeyLinks.size();
    } else {
        qWarning() << "cannot get KeyLinkAPI* at pos " << pos << "Object is not of type KeyAPI*";
    }
    return 0;
}
void KeyAPI::clearKeyLinksProperty(QQmlListProperty<KeyLinkAPI> *keyLinksList)
{
    KeyAPI *keyAPI = qobject_cast<KeyAPI *>(keyLinksList->object);
    if (keyAPI) {
        // keyLinks are independent - DON'T delete them
        keyAPI->mKeyLinks.clear();
    } else {
        qWarning() << "cannot clear keyLinks " << "Object is not of type KeyAPI*";
    }
}



KeyAPI::~KeyAPI()
{
	// place cleanUp code here
}
	
