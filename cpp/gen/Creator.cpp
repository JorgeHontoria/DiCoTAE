#include "Creator.hpp"
#include <QDebug>
#include <quuid.h>
// target also references to this
#include "Key.hpp"

// keys of QVariantMap used in this APP
static const QString creatorIdKey = "creatorId";
static const QString isBoolean2Key = "isBoolean2";
static const QString sortKeyKey = "sortKey";
static const QString sortGroupKey = "sortGroup";
static const QString nameKey = "name";
static const QString publicNameKey = "publicName";
static const QString bioKey = "bio";
static const QString creatorImageKey = "creatorImage";
static const QString keysKey = "keys";

// keys used from Server API etc
static const QString creatorIdForeignKey = "creatorId";
static const QString isBoolean2ForeignKey = "isBoolean2";
static const QString sortKeyForeignKey = "sortKey";
static const QString sortGroupForeignKey = "sortGroup";
static const QString nameForeignKey = "name";
static const QString publicNameForeignKey = "publicName";
static const QString bioForeignKey = "bio";
static const QString creatorImageForeignKey = "creatorImage";
static const QString keysForeignKey = "keys";

/*
 * Default Constructor if Creator not initialized from QVariantMap
 */
Creator::Creator(QObject *parent) :
        QObject(parent), mCreatorId(-1), mIsBoolean2(false), mSortKey(""), mSortGroup(""), mName(""), mPublicName(""), mBio("")
{
	// lazy references:
    mCreatorImage = -1;
    mCreatorImageAsDataObject = 0;
    mCreatorImageInvalid = false;
		// lazy Arrays where only keys are persisted
		mKeysKeysResolved = false;
}

bool Creator::isAllResolved()
{
    if (hasCreatorImage() && !isCreatorImageResolvedAsDataObject()) {
		return false;
	}
    if(!areKeysKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize Creator from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Creator::fillFromMap(const QVariantMap& creatorMap)
{
    mCreatorId = creatorMap.value(creatorIdKey).toInt();
	mIsBoolean2 = creatorMap.value(isBoolean2Key).toBool();
	mSortKey = creatorMap.value(sortKeyKey).toString();
	mSortGroup = creatorMap.value(sortGroupKey).toString();
	mName = creatorMap.value(nameKey).toString();
	mPublicName = creatorMap.value(publicNameKey).toString();
	mBio = creatorMap.value(bioKey).toString();
    // creatorImage lazy pointing to CreatorImage* (domainKey: creatorId)
	if (creatorMap.contains(creatorImageKey)) {
        mCreatorImage = creatorMap.value(creatorImageKey).toInt();
        if (mCreatorImage != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mKeys is (lazy loaded) Array of Key*
	mKeysKeys = creatorMap.value(keysKey).toStringList();
	// mKeys must be resolved later if there are keys
	mKeysKeysResolved = (mKeysKeys.size() == 0);
	mKeys.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void Creator::fillFromForeignMap(const QVariantMap& creatorMap)
{
    mCreatorId = creatorMap.value(creatorIdForeignKey).toInt();
	mIsBoolean2 = creatorMap.value(isBoolean2ForeignKey).toBool();
	mSortKey = creatorMap.value(sortKeyForeignKey).toString();
	mSortGroup = creatorMap.value(sortGroupForeignKey).toString();
	mName = creatorMap.value(nameForeignKey).toString();
	mPublicName = creatorMap.value(publicNameForeignKey).toString();
	mBio = creatorMap.value(bioForeignKey).toString();
    // creatorImage lazy pointing to CreatorImage* (domainKey: creatorId)
	if (creatorMap.contains(creatorImageForeignKey)) {
        mCreatorImage = creatorMap.value(creatorImageForeignKey).toInt();
        if (mCreatorImage != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mKeys is (lazy loaded) Array of Key*
	mKeysKeys = creatorMap.value(keysForeignKey).toStringList();
	// mKeys must be resolved later if there are keys
	mKeysKeysResolved = (mKeysKeys.size() == 0);
	mKeys.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void Creator::fillFromCacheMap(const QVariantMap& creatorMap)
{
    mCreatorId = creatorMap.value(creatorIdKey).toInt();
	mIsBoolean2 = creatorMap.value(isBoolean2Key).toBool();
	mSortKey = creatorMap.value(sortKeyKey).toString();
	mSortGroup = creatorMap.value(sortGroupKey).toString();
	mName = creatorMap.value(nameKey).toString();
	mPublicName = creatorMap.value(publicNameKey).toString();
	mBio = creatorMap.value(bioKey).toString();
    // creatorImage lazy pointing to CreatorImage* (domainKey: creatorId)
	if (creatorMap.contains(creatorImageKey)) {
        mCreatorImage = creatorMap.value(creatorImageKey).toInt();
        if (mCreatorImage != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mKeys is (lazy loaded) Array of Key*
	mKeysKeys = creatorMap.value(keysKey).toStringList();
	// mKeys must be resolved later if there are keys
	mKeysKeysResolved = (mKeysKeys.size() == 0);
	mKeys.clear();
}

void Creator::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Creator::isValid()
{
    if (mCreatorId == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Creator as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Creator::toMap()
{
	QVariantMap creatorMap;
    // creatorImage lazy pointing to CreatorImage* (domainKey: creatorId)
    if (mCreatorImage != -1) {
        creatorMap.insert(creatorImageKey, mCreatorImage);
	}
	// mKeys points to Key*
	// lazy array: persist only keys
	//
    // if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mKeysKeysResolved) {
		mKeysKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mKeys.size(); ++i) {
		Key* key;
		key = mKeys.at(i);
		mKeysKeys << QString::number(key->keyId());
	}
	creatorMap.insert(keysKey, mKeysKeys);
    creatorMap.insert(creatorIdKey, mCreatorId);
	creatorMap.insert(isBoolean2Key, mIsBoolean2);
	creatorMap.insert(sortKeyKey, mSortKey);
	creatorMap.insert(sortGroupKey, mSortGroup);
	creatorMap.insert(nameKey, mName);
	creatorMap.insert(publicNameKey, mPublicName);
	creatorMap.insert(bioKey, mBio);
	return creatorMap;
}

/*
 * Exports Properties from Creator as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Creator::toForeignMap()
{
	QVariantMap creatorMap;
    // creatorImage lazy pointing to CreatorImage* (domainKey: creatorId)
    if (mCreatorImage != -1) {
        creatorMap.insert(creatorImageForeignKey, mCreatorImage);
	}
	// mKeys points to Key*
	// lazy array: persist only keys
	//
    // if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mKeysKeysResolved) {
		mKeysKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mKeys.size(); ++i) {
		Key* key;
		key = mKeys.at(i);
		mKeysKeys << QString::number(key->keyId());
	}
	creatorMap.insert(keysKey, mKeysKeys);
    creatorMap.insert(creatorIdForeignKey, mCreatorId);
	creatorMap.insert(isBoolean2ForeignKey, mIsBoolean2);
	creatorMap.insert(sortKeyForeignKey, mSortKey);
	creatorMap.insert(sortGroupForeignKey, mSortGroup);
	creatorMap.insert(nameForeignKey, mName);
	creatorMap.insert(publicNameForeignKey, mPublicName);
	creatorMap.insert(bioForeignKey, mBio);
	return creatorMap;
}


/*
 * Exports Properties from Creator as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Creator::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// REF
// Lazy: creatorImage
// Optional: creatorImage
// creatorImage lazy pointing to CreatorImage* (domainKey: creatorId)
int Creator::creatorImage() const
{
    return mCreatorImage;
}
CreatorImage* Creator::creatorImageAsDataObject() const
{
    return mCreatorImageAsDataObject;
}
void Creator::setCreatorImage(int creatorImage)
{
    if (creatorImage != mCreatorImage) {
        // remove old Data Object if one was resolved
        if (mCreatorImageAsDataObject) {
            // reset pointer, don't delete the independent object !
            mCreatorImageAsDataObject = 0;
        }
        // set the new lazy reference
        mCreatorImage = creatorImage;
        mCreatorImageInvalid = false;
        emit creatorImageChanged(creatorImage);
        if (creatorImage != -1) {
            // resolve the corresponding Data Object on demand from DataManager
        }
    }
}
void Creator::removeCreatorImage()
{
    if (mCreatorImage != -1) {
        setCreatorImage(-1);
	}
}
bool Creator::hasCreatorImage()
{
    if (!mCreatorImageInvalid && mCreatorImage != -1) {
        return true;
    } else {
        return false;
    }
}
bool Creator::isCreatorImageResolvedAsDataObject()
{
    if (!mCreatorImageInvalid && mCreatorImageAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite creatorId if different
void Creator::resolveCreatorImageAsDataObject(CreatorImage* creatorImage)
{
    if (creatorImage) {
        if (creatorImage->creatorId() != mCreatorImage) {
            setCreatorImage(creatorImage->creatorId());
        }
        mCreatorImageAsDataObject = creatorImage;
        mCreatorImageInvalid = false;
    }
}
void Creator::markCreatorImageAsInvalid()
{
    mCreatorImageInvalid = true;
}
// ATT 
// Mandatory: creatorId
// Domain KEY: creatorId
int Creator::creatorId() const
{
    return mCreatorId;
}

void Creator::setCreatorId(int creatorId)
{
    if (creatorId != mCreatorId) {
        mCreatorId = creatorId;
		emit creatorIdChanged(creatorId);
	}
}
// ATT 
// Optional: isBoolean2
bool Creator::isBoolean2() const
{
	return mIsBoolean2;
}

void Creator::setIsDeprecated(bool isBoolean2)
{
	if (isBoolean2 != mIsBoolean2) {
		mIsBoolean2 = isBoolean2;
		emit isDeprecatedChanged(isBoolean2);
	}
}
// ATT 
// Optional: sortKey
QString Creator::sortKey() const
{
	return mSortKey;
}

void Creator::setSortKey(QString sortKey)
{
	if (sortKey != mSortKey) {
		mSortKey = sortKey;
		emit sortKeyChanged(sortKey);
	}
}
// ATT 
// Optional: sortGroup
QString Creator::sortGroup() const
{
	return mSortGroup;
}

void Creator::setSortGroup(QString sortGroup)
{
	if (sortGroup != mSortGroup) {
		mSortGroup = sortGroup;
		emit sortGroupChanged(sortGroup);
	}
}
// ATT 
// Optional: name
QString Creator::name() const
{
	return mName;
}

void Creator::setName(QString name)
{
	if (name != mName) {
		mName = name;
		emit nameChanged(name);
	}
}
// ATT 
// Optional: publicName
QString Creator::publicName() const
{
	return mPublicName;
}

void Creator::setPublicName(QString publicName)
{
	if (publicName != mPublicName) {
		mPublicName = publicName;
		emit publicNameChanged(publicName);
	}
}
// ATT 
// Optional: bio
QString Creator::bio() const
{
	return mBio;
}

void Creator::setBio(QString bio)
{
	if (bio != mBio) {
		mBio = bio;
		emit bioChanged(bio);
	}
}
// ATT 
// Optional: keys
QVariantList Creator::keysAsQVariantList()
{
	QVariantList keysList;
	for (int i = 0; i < mKeys.size(); ++i) {
        keysList.append((mKeys.at(i))->toMap());
    }
	return keysList;
}
QVariantList Creator::keysAsForeignQVariantList()
{
	QVariantList keysList;
	for (int i = 0; i < mKeys.size(); ++i) {
        keysList.append((mKeys.at(i))->toForeignMap());
    }
	return keysList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add keys without resolving existing keys before
 * attention: before looping through the objects
 * you must resolveKeysKeys
 */
void Creator::addToKeys(Key* key)
{
    mKeys.append(key);
    emit addedToKeys(key);
    emit keysPropertyListChanged();
}

bool Creator::removeFromKeys(Key* key)
{
    bool ok = false;
    ok = mKeys.removeOne(key);
    if (!ok) {
    	qDebug() << "Key* not found in keys";
    	return false;
    }
    emit keysPropertyListChanged();
    // keys are independent - DON'T delete them
    return true;
}
void Creator::clearKeys()
{
    for (int i = mKeys.size(); i > 0; --i) {
        removeFromKeys(mKeys.last());
    }
    mKeysKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: keysKeys()
 * - resolve them from DataManager
 * - then resolveKeysKeys()
 */
bool Creator::areKeysKeysResolved()
{
    return mKeysKeysResolved;
}

QStringList Creator::keysKeys()
{
    return mKeysKeys;
}

/**
 * Objects from keysKeys will be added to existing keys
 * This enables to use addToKeys() without resolving before
 * Hint: it's your responsibility to resolve before looping thru keys
 */
void Creator::resolveKeysKeys(QList<Key*> keys)
{
    if(mKeysKeysResolved){
        return;
    }
    // don't clear mKeys (see above)
    for (int i = 0; i < keys.size(); ++i) {
        addToKeys(keys.at(i));
    }
    mKeysKeysResolved = true;
}

int Creator::keysCount()
{
    return mKeys.size();
}
QList<Key*> Creator::keys()
{
	return mKeys;
}
void Creator::setKeys(QList<Key*> keys)
{
	if (keys != mKeys) {
		mKeys = keys;
		emit keysChanged(keys);
		emit keysPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * creator.keysPropertyList.length to get the size
 * creator.keysPropertyList[2] to get Key* at position 2
 * creator.keysPropertyList = [] to clear the list
 * or get easy access to properties like
 * creator.keysPropertyList[2].myPropertyName
 */
QQmlListProperty<Key> Creator::keysPropertyList()
{
    return QQmlListProperty<Key>(this, 0, &Creator::appendToKeysProperty,
            &Creator::keysPropertyCount, &Creator::atKeysProperty,
            &Creator::clearKeysProperty);
}
void Creator::appendToKeysProperty(QQmlListProperty<Key> *keysList,
        Key* key)
{
    Creator *creatorObject = qobject_cast<Creator *>(keysList->object);
    if (creatorObject) {
        creatorObject->mKeys.append(key);
        emit creatorObject->addedToKeys(key);
    } else {
        qWarning() << "cannot append Key* to keys " << "Object is not of type Creator*";
    }
}
int Creator::keysPropertyCount(QQmlListProperty<Key> *keysList)
{
    Creator *creator = qobject_cast<Creator *>(keysList->object);
    if (creator) {
        return creator->mKeys.size();
    } else {
        qWarning() << "cannot get size keys " << "Object is not of type Creator*";
    }
    return 0;
}
Key* Creator::atKeysProperty(QQmlListProperty<Key> *keysList, int pos)
{
    Creator *creator = qobject_cast<Creator *>(keysList->object);
    if (creator) {
        if (creator->mKeys.size() > pos) {
            return creator->mKeys.at(pos);
        }
        qWarning() << "cannot get Key* at pos " << pos << " size is "
                << creator->mKeys.size();
    } else {
        qWarning() << "cannot get Key* at pos " << pos << "Object is not of type Creator*";
    }
    return 0;
}
void Creator::clearKeysProperty(QQmlListProperty<Key> *keysList)
{
    Creator *creator = qobject_cast<Creator *>(keysList->object);
    if (creator) {
        // keys are independent - DON'T delete them
        creator->mKeys.clear();
    } else {
        qWarning() << "cannot clear keys " << "Object is not of type Creator*";
    }
}



Creator::~Creator()
{
	// place cleanUp code here
}
	
