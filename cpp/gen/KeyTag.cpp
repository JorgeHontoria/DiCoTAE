#include "KeyTag.hpp"
#include <QDebug>
#include <quuid.h>
// target also references to this
#include "Key.hpp"

// keys of QVariantMap used in this APP
static const QString tagIdKey = "tagId";
static const QString nameKey = "name";
static const QString inAssetsKey = "inAssets";
static const QString keysKey = "keys";

// keys used from Server API etc
static const QString tagIdForeignKey = "tagId";
static const QString nameForeignKey = "name";
static const QString inAssetsForeignKey = "inAssets";
static const QString keysForeignKey = "keys";

/*
 * Default Constructor if KeyTag not initialized from QVariantMap
 */
KeyTag::KeyTag(QObject *parent) :
        QObject(parent), mTagId(-1), mName(""), mInAssets(false)
{
		// lazy Arrays where only keys are persisted
		mKeysKeysResolved = false;
}

bool KeyTag::isAllResolved()
{
    if(!areKeysKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize KeyTag from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void KeyTag::fillFromMap(const QVariantMap& keyTagMap)
{
    mTagId = keyTagMap.value(tagIdKey).toInt();
	mName = keyTagMap.value(nameKey).toString();
	mInAssets = keyTagMap.value(inAssetsKey).toBool();
	// mKeys is (lazy loaded) Array of Key*
	mKeysKeys = keyTagMap.value(keysKey).toStringList();
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
void KeyTag::fillFromForeignMap(const QVariantMap& keyTagMap)
{
    mTagId = keyTagMap.value(tagIdForeignKey).toInt();
	mName = keyTagMap.value(nameForeignKey).toString();
	mInAssets = keyTagMap.value(inAssetsForeignKey).toBool();
	// mKeys is (lazy loaded) Array of Key*
	mKeysKeys = keyTagMap.value(keysForeignKey).toStringList();
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
void KeyTag::fillFromCacheMap(const QVariantMap& keyTagMap)
{
    mTagId = keyTagMap.value(tagIdKey).toInt();
	mName = keyTagMap.value(nameKey).toString();
	mInAssets = keyTagMap.value(inAssetsKey).toBool();
	// mKeys is (lazy loaded) Array of Key*
	mKeysKeys = keyTagMap.value(keysKey).toStringList();
	// mKeys must be resolved later if there are keys
	mKeysKeysResolved = (mKeysKeys.size() == 0);
	mKeys.clear();
}

void KeyTag::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool KeyTag::isValid()
{
	if (mTagId == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from KeyTag as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap KeyTag::toMap()
{
	QVariantMap keyTagMap;
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
	keyTagMap.insert(keysKey, mKeysKeys);
    keyTagMap.insert(tagIdKey, mTagId);
	keyTagMap.insert(nameKey, mName);
	keyTagMap.insert(inAssetsKey, mInAssets);
	return keyTagMap;
}

/*
 * Exports Properties from KeyTag as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap KeyTag::toForeignMap()
{
	QVariantMap keyTagMap;
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
	keyTagMap.insert(keysKey, mKeysKeys);
    keyTagMap.insert(tagIdForeignKey, mTagId);
	keyTagMap.insert(nameForeignKey, mName);
	keyTagMap.insert(inAssetsForeignKey, mInAssets);
	return keyTagMap;
}


/*
 * Exports Properties from KeyTag as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap KeyTag::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: tagId
// Domain KEY: tagId
int KeyTag::tagId() const
{
	return mTagId;
}

void KeyTag::setTagId(int tagId)
{
    if (tagId != mTagId) {
        mTagId = tagId;
        emit tagIdChanged(tagId);
	}
}
// ATT 
// Optional: name
QString KeyTag::name() const
{
	return mName;
}

void KeyTag::setName(QString name)
{
	if (name != mName) {
		mName = name;
		emit nameChanged(name);
	}
}
// ATT 
// Optional: inAssets
bool KeyTag::inAssets() const
{
	return mInAssets;
}

void KeyTag::setInAssets(bool inAssets)
{
	if (inAssets != mInAssets) {
		mInAssets = inAssets;
		emit inAssetsChanged(inAssets);
	}
}
// ATT 
// Optional: keys
QVariantList KeyTag::keysAsQVariantList()
{
	QVariantList keysList;
	for (int i = 0; i < mKeys.size(); ++i) {
        keysList.append((mKeys.at(i))->toMap());
    }
	return keysList;
}
QVariantList KeyTag::keysAsForeignQVariantList()
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
void KeyTag::addToKeys(Key* key)
{
    mKeys.append(key);
    emit addedToKeys(key);
    emit keysPropertyListChanged();
}

bool KeyTag::removeFromKeys(Key* key)
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
void KeyTag::clearKeys()
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
bool KeyTag::areKeysKeysResolved()
{
    return mKeysKeysResolved;
}

QStringList KeyTag::keysKeys()
{
    return mKeysKeys;
}

/**
 * Objects from keysKeys will be added to existing keys
 * This enables to use addToKeys() without resolving before
 * Hint: it's your responsibility to resolve before looping thru keys
 */
void KeyTag::resolveKeysKeys(QList<Key*> keys)
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

int KeyTag::keysCount()
{
    return mKeys.size();
}
QList<Key*> KeyTag::keys()
{
	return mKeys;
}
void KeyTag::setKeys(QList<Key*> keys) 
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
 * keyTag.keysPropertyList.length to get the size
 * keyTag.keysPropertyList[2] to get Key* at position 2
 * keyTag.keysPropertyList = [] to clear the list
 * or get easy access to properties like
 * keyTag.keysPropertyList[2].myPropertyName
 */
QQmlListProperty<Key> KeyTag::keysPropertyList()
{
    return QQmlListProperty<Key>(this, 0, &KeyTag::appendToKeysProperty,
            &KeyTag::keysPropertyCount, &KeyTag::atKeysProperty,
            &KeyTag::clearKeysProperty);
}
void KeyTag::appendToKeysProperty(QQmlListProperty<Key> *keysList,
        Key* key)
{
    KeyTag *keyTagObject = qobject_cast<KeyTag *>(keysList->object);
    if (keyTagObject) {
        keyTagObject->mKeys.append(key);
        emit keyTagObject->addedToKeys(key);
    } else {
        qWarning() << "cannot append Key* to keys " << "Object is not of type KeyTag*";
    }
}
int KeyTag::keysPropertyCount(QQmlListProperty<Key> *keysList)
{
    KeyTag *keyTag = qobject_cast<KeyTag *>(keysList->object);
    if (keyTag) {
        return keyTag->mKeys.size();
    } else {
        qWarning() << "cannot get size keys " << "Object is not of type KeyTag*";
    }
    return 0;
}
Key* KeyTag::atKeysProperty(QQmlListProperty<Key> *keysList, int pos)
{
    KeyTag *keyTag = qobject_cast<KeyTag *>(keysList->object);
    if (keyTag) {
        if (keyTag->mKeys.size() > pos) {
            return keyTag->mKeys.at(pos);
        }
        qWarning() << "cannot get Key* at pos " << pos << " size is "
                << keyTag->mKeys.size();
    } else {
        qWarning() << "cannot get Key* at pos " << pos << "Object is not of type KeyTag*";
    }
    return 0;
}
void KeyTag::clearKeysProperty(QQmlListProperty<Key> *keysList)
{
    KeyTag *keyTag = qobject_cast<KeyTag *>(keysList->object);
    if (keyTag) {
        // keys are independent - DON'T delete them
        keyTag->mKeys.clear();
    } else {
        qWarning() << "cannot clear keys " << "Object is not of type KeyTag*";
    }
}



KeyTag::~KeyTag()
{
	// place cleanUp code here
}
	
