#include "KeyLists.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString uuidKey = "uuid";
static const QString dictionarydKeysKey = "dictionarydKeys";
static const QString sameTimeKeysKey = "sameTimeKeys";
static const QString specialTimeKeysKey = "specialTimeKeys";

// keys used from Server API etc
static const QString uuidForeignKey = "uuid";
static const QString dictionarydKeysForeignKey = "dictionarydKeys";
static const QString sameTimeKeysForeignKey = "sameTimeKeys";
static const QString specialTimeKeysForeignKey = "specialTimeKeys";

/*
 * Default Constructor if KeyLists not initialized from QVariantMap
 */
KeyLists::KeyLists(QObject *parent) :
        QObject(parent), mUuid("")
{
		// lazy Arrays where only keys are persisted
		mDictionarydKeysKeysResolved = false;
		mSameTimeKeysKeysResolved = false;
		mSpecialTimeKeysKeysResolved = false;
}

bool KeyLists::isAllResolved()
{
    if(!areDictionarydKeysKeysResolved()) {
        return false;
    }
    if(!areSameTimeKeysKeysResolved()) {
        return false;
    }
    if(!areSpecialTimeKeysKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize KeyLists from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void KeyLists::fillFromMap(const QVariantMap& keyListsMap)
{
	mUuid = keyListsMap.value(uuidKey).toString();
	if (mUuid.isEmpty()) {
		mUuid = QUuid::createUuid().toString();
		mUuid = mUuid.right(mUuid.length() - 1);
		mUuid = mUuid.left(mUuid.length() - 1);
	}	
	// mDictionarydKeys is (lazy loaded) Array of Key*
	mDictionarydKeysKeys = keyListsMap.value(dictionarydKeysKey).toStringList();
	// mDictionarydKeys must be resolved later if there are keys
	mDictionarydKeysKeysResolved = (mDictionarydKeysKeys.size() == 0);
	mDictionarydKeys.clear();
	// mSameTimeKeys is (lazy loaded) Array of Key*
	mSameTimeKeysKeys = keyListsMap.value(sameTimeKeysKey).toStringList();
	// mSameTimeKeys must be resolved later if there are keys
	mSameTimeKeysKeysResolved = (mSameTimeKeysKeys.size() == 0);
	mSameTimeKeys.clear();
	// mSpecialTimeKeys is (lazy loaded) Array of Key*
	mSpecialTimeKeysKeys = keyListsMap.value(specialTimeKeysKey).toStringList();
	// mSpecialTimeKeys must be resolved later if there are keys
	mSpecialTimeKeysKeysResolved = (mSpecialTimeKeysKeys.size() == 0);
	mSpecialTimeKeys.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void KeyLists::fillFromForeignMap(const QVariantMap& keyListsMap)
{
	mUuid = keyListsMap.value(uuidForeignKey).toString();
	if (mUuid.isEmpty()) {
		mUuid = QUuid::createUuid().toString();
		mUuid = mUuid.right(mUuid.length() - 1);
		mUuid = mUuid.left(mUuid.length() - 1);
	}	
	// mDictionarydKeys is (lazy loaded) Array of Key*
	mDictionarydKeysKeys = keyListsMap.value(dictionarydKeysForeignKey).toStringList();
	// mDictionarydKeys must be resolved later if there are keys
	mDictionarydKeysKeysResolved = (mDictionarydKeysKeys.size() == 0);
	mDictionarydKeys.clear();
	// mSameTimeKeys is (lazy loaded) Array of Key*
	mSameTimeKeysKeys = keyListsMap.value(sameTimeKeysForeignKey).toStringList();
	// mSameTimeKeys must be resolved later if there are keys
	mSameTimeKeysKeysResolved = (mSameTimeKeysKeys.size() == 0);
	mSameTimeKeys.clear();
	// mSpecialTimeKeys is (lazy loaded) Array of Key*
	mSpecialTimeKeysKeys = keyListsMap.value(specialTimeKeysForeignKey).toStringList();
	// mSpecialTimeKeys must be resolved later if there are keys
	mSpecialTimeKeysKeysResolved = (mSpecialTimeKeysKeys.size() == 0);
	mSpecialTimeKeys.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void KeyLists::fillFromCacheMap(const QVariantMap& keyListsMap)
{
	mUuid = keyListsMap.value(uuidKey).toString();
	if (mUuid.isEmpty()) {
		mUuid = QUuid::createUuid().toString();
		mUuid = mUuid.right(mUuid.length() - 1);
		mUuid = mUuid.left(mUuid.length() - 1);
	}	
	// mDictionarydKeys is (lazy loaded) Array of Key*
	mDictionarydKeysKeys = keyListsMap.value(dictionarydKeysKey).toStringList();
	// mDictionarydKeys must be resolved later if there are keys
	mDictionarydKeysKeysResolved = (mDictionarydKeysKeys.size() == 0);
	mDictionarydKeys.clear();
	// mSameTimeKeys is (lazy loaded) Array of Key*
	mSameTimeKeysKeys = keyListsMap.value(sameTimeKeysKey).toStringList();
	// mSameTimeKeys must be resolved later if there are keys
	mSameTimeKeysKeysResolved = (mSameTimeKeysKeys.size() == 0);
	mSameTimeKeys.clear();
	// mSpecialTimeKeys is (lazy loaded) Array of Key*
	mSpecialTimeKeysKeys = keyListsMap.value(specialTimeKeysKey).toStringList();
	// mSpecialTimeKeys must be resolved later if there are keys
	mSpecialTimeKeysKeysResolved = (mSpecialTimeKeysKeys.size() == 0);
	mSpecialTimeKeys.clear();
}

void KeyLists::prepareNew()
{
	mUuid = QUuid::createUuid().toString();
	mUuid = mUuid.right(mUuid.length() - 1);
	mUuid = mUuid.left(mUuid.length() - 1);
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool KeyLists::isValid()
{
	if (mUuid.isNull() || mUuid.isEmpty()) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from KeyLists as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap KeyLists::toMap()
{
	QVariantMap keyListsMap;
	// mDictionarydKeys points to Key*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mDictionarydKeysKeysResolved) {
		mDictionarydKeysKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mDictionarydKeys.size(); ++i) {
		Key* key;
		key = mDictionarydKeys.at(i);
		mDictionarydKeysKeys << QString::number(key->keyId());
	}
	keyListsMap.insert(dictionarydKeysKey, mDictionarydKeysKeys);
	// mSameTimeKeys points to Key*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mSameTimeKeysKeysResolved) {
		mSameTimeKeysKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mSameTimeKeys.size(); ++i) {
		Key* key;
		key = mSameTimeKeys.at(i);
		mSameTimeKeysKeys << QString::number(key->keyId());
	}
	keyListsMap.insert(sameTimeKeysKey, mSameTimeKeysKeys);
	// mSpecialTimeKeys points to Key*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mSpecialTimeKeysKeysResolved) {
		mSpecialTimeKeysKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mSpecialTimeKeys.size(); ++i) {
		Key* key;
		key = mSpecialTimeKeys.at(i);
		mSpecialTimeKeysKeys << QString::number(key->keyId());
	}
	keyListsMap.insert(specialTimeKeysKey, mSpecialTimeKeysKeys);
	keyListsMap.insert(uuidKey, mUuid);
	return keyListsMap;
}

/*
 * Exports Properties from KeyLists as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap KeyLists::toForeignMap()
{
	QVariantMap keyListsMap;
	// mDictionarydKeys points to Key*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mDictionarydKeysKeysResolved) {
		mDictionarydKeysKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mDictionarydKeys.size(); ++i) {
		Key* key;
		key = mDictionarydKeys.at(i);
		mDictionarydKeysKeys << QString::number(key->keyId());
	}
	keyListsMap.insert(dictionarydKeysKey, mDictionarydKeysKeys);
	// mSameTimeKeys points to Key*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mSameTimeKeysKeysResolved) {
		mSameTimeKeysKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mSameTimeKeys.size(); ++i) {
		Key* key;
		key = mSameTimeKeys.at(i);
		mSameTimeKeysKeys << QString::number(key->keyId());
	}
	keyListsMap.insert(sameTimeKeysKey, mSameTimeKeysKeys);
	// mSpecialTimeKeys points to Key*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mSpecialTimeKeysKeysResolved) {
		mSpecialTimeKeysKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mSpecialTimeKeys.size(); ++i) {
		Key* key;
		key = mSpecialTimeKeys.at(i);
		mSpecialTimeKeysKeys << QString::number(key->keyId());
	}
	keyListsMap.insert(specialTimeKeysKey, mSpecialTimeKeysKeys);
	keyListsMap.insert(uuidForeignKey, mUuid);
	return keyListsMap;
}


/*
 * Exports Properties from KeyLists as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap KeyLists::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: uuid
// Domain KEY: uuid
QString KeyLists::uuid() const
{
	return mUuid;
}

void KeyLists::setUuid(QString uuid)
{
	if (uuid != mUuid) {
		mUuid = uuid;
		emit uuidChanged(uuid);
	}
}
// ATT 
// Optional: dictionarydKeys
QVariantList KeyLists::dictionarydKeysAsQVariantList()
{
	QVariantList dictionarydKeysList;
	for (int i = 0; i < mDictionarydKeys.size(); ++i) {
        dictionarydKeysList.append((mDictionarydKeys.at(i))->toMap());
    }
	return dictionarydKeysList;
}
QVariantList KeyLists::dictionarydKeysAsForeignQVariantList()
{
	QVariantList dictionarydKeysList;
	for (int i = 0; i < mDictionarydKeys.size(); ++i) {
        dictionarydKeysList.append((mDictionarydKeys.at(i))->toForeignMap());
    }
	return dictionarydKeysList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add dictionarydKeys without resolving existing keys before
 * attention: before looping through the objects
 * you must resolveDictionarydKeysKeys
 */
void KeyLists::addToDictionarydKeys(Key* key)
{
    mDictionarydKeys.append(key);
    emit addedToDictionarydKeys(key);
    emit dictionarydKeysPropertyListChanged();
}

bool KeyLists::removeFromDictionarydKeys(Key* key)
{
    bool ok = false;
    ok = mDictionarydKeys.removeOne(key);
    if (!ok) {
    	qDebug() << "Key* not found in dictionarydKeys";
    	return false;
    }
    emit dictionarydKeysPropertyListChanged();
    // dictionarydKeys are independent - DON'T delete them
    return true;
}
void KeyLists::clearDictionarydKeys()
{
    for (int i = mDictionarydKeys.size(); i > 0; --i) {
        removeFromDictionarydKeys(mDictionarydKeys.last());
    }
    mDictionarydKeysKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: dictionarydKeysKeys()
 * - resolve them from DataManager
 * - then resolveDictionarydKeysKeys()
 */
bool KeyLists::areDictionarydKeysKeysResolved()
{
    return mDictionarydKeysKeysResolved;
}

QStringList KeyLists::dictionarydKeysKeys()
{
    return mDictionarydKeysKeys;
}

/**
 * Objects from dictionarydKeysKeys will be added to existing dictionarydKeys
 * This enables to use addToDictionarydKeys() without resolving before
 * Hint: it's your responsibility to resolve before looping thru dictionarydKeys
 */
void KeyLists::resolveDictionarydKeysKeys(QList<Key*> dictionarydKeys)
{
    if(mDictionarydKeysKeysResolved){
        return;
    }
    // don't clear mDictionarydKeys (see above)
    for (int i = 0; i < dictionarydKeys.size(); ++i) {
        addToDictionarydKeys(dictionarydKeys.at(i));
    }
    mDictionarydKeysKeysResolved = true;
}

int KeyLists::dictionarydKeysCount()
{
    return mDictionarydKeys.size();
}
QList<Key*> KeyLists::dictionarydKeys()
{
	return mDictionarydKeys;
}
void KeyLists::setDictionarydKeys(QList<Key*> dictionarydKeys) 
{
	if (dictionarydKeys != mDictionarydKeys) {
		mDictionarydKeys = dictionarydKeys;
		emit dictionarydKeysChanged(dictionarydKeys);
		emit dictionarydKeysPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * keyLists.dictionarydKeysPropertyList.length to get the size
 * keyLists.dictionarydKeysPropertyList[2] to get Key* at position 2
 * keyLists.dictionarydKeysPropertyList = [] to clear the list
 * or get easy access to properties like
 * keyLists.dictionarydKeysPropertyList[2].myPropertyName
 */
QQmlListProperty<Key> KeyLists::dictionarydKeysPropertyList()
{
    return QQmlListProperty<Key>(this, 0, &KeyLists::appendToDictionarydKeysProperty,
            &KeyLists::dictionarydKeysPropertyCount, &KeyLists::atDictionarydKeysProperty,
            &KeyLists::clearDictionarydKeysProperty);
}
void KeyLists::appendToDictionarydKeysProperty(QQmlListProperty<Key> *dictionarydKeysList,
        Key* key)
{
    KeyLists *keyListsObject = qobject_cast<KeyLists *>(dictionarydKeysList->object);
    if (keyListsObject) {
        keyListsObject->mDictionarydKeys.append(key);
        emit keyListsObject->addedToDictionarydKeys(key);
    } else {
        qWarning() << "cannot append Key* to dictionarydKeys " << "Object is not of type KeyLists*";
    }
}
int KeyLists::dictionarydKeysPropertyCount(QQmlListProperty<Key> *dictionarydKeysList)
{
    KeyLists *keyLists = qobject_cast<KeyLists *>(dictionarydKeysList->object);
    if (keyLists) {
        return keyLists->mDictionarydKeys.size();
    } else {
        qWarning() << "cannot get size dictionarydKeys " << "Object is not of type KeyLists*";
    }
    return 0;
}
Key* KeyLists::atDictionarydKeysProperty(QQmlListProperty<Key> *dictionarydKeysList, int pos)
{
    KeyLists *keyLists = qobject_cast<KeyLists *>(dictionarydKeysList->object);
    if (keyLists) {
        if (keyLists->mDictionarydKeys.size() > pos) {
            return keyLists->mDictionarydKeys.at(pos);
        }
        qWarning() << "cannot get Key* at pos " << pos << " size is "
                << keyLists->mDictionarydKeys.size();
    } else {
        qWarning() << "cannot get Key* at pos " << pos << "Object is not of type KeyLists*";
    }
    return 0;
}
void KeyLists::clearDictionarydKeysProperty(QQmlListProperty<Key> *dictionarydKeysList)
{
    KeyLists *keyLists = qobject_cast<KeyLists *>(dictionarydKeysList->object);
    if (keyLists) {
        // dictionarydKeys are independent - DON'T delete them
        keyLists->mDictionarydKeys.clear();
    } else {
        qWarning() << "cannot clear dictionarydKeys " << "Object is not of type KeyLists*";
    }
}

// ATT 
// Optional: sameTimeKeys
QVariantList KeyLists::sameTimeKeysAsQVariantList()
{
	QVariantList sameTimeKeysList;
	for (int i = 0; i < mSameTimeKeys.size(); ++i) {
        sameTimeKeysList.append((mSameTimeKeys.at(i))->toMap());
    }
	return sameTimeKeysList;
}
QVariantList KeyLists::sameTimeKeysAsForeignQVariantList()
{
	QVariantList sameTimeKeysList;
	for (int i = 0; i < mSameTimeKeys.size(); ++i) {
        sameTimeKeysList.append((mSameTimeKeys.at(i))->toForeignMap());
    }
	return sameTimeKeysList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add sameTimeKeys without resolving existing keys before
 * attention: before looping through the objects
 * you must resolveSameTimeKeysKeys
 */
void KeyLists::addToSameTimeKeys(Key* key)
{
    mSameTimeKeys.append(key);
    emit addedToSameTimeKeys(key);
    emit sameTimeKeysPropertyListChanged();
}

bool KeyLists::removeFromSameTimeKeys(Key* key)
{
    bool ok = false;
    ok = mSameTimeKeys.removeOne(key);
    if (!ok) {
    	qDebug() << "Key* not found in sameTimeKeys";
    	return false;
    }
    emit sameTimeKeysPropertyListChanged();
    // sameTimeKeys are independent - DON'T delete them
    return true;
}
void KeyLists::clearSameTimeKeys()
{
    for (int i = mSameTimeKeys.size(); i > 0; --i) {
        removeFromSameTimeKeys(mSameTimeKeys.last());
    }
    mSameTimeKeysKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: sameTimeKeysKeys()
 * - resolve them from DataManager
 * - then resolveSameTimeKeysKeys()
 */
bool KeyLists::areSameTimeKeysKeysResolved()
{
    return mSameTimeKeysKeysResolved;
}

QStringList KeyLists::sameTimeKeysKeys()
{
    return mSameTimeKeysKeys;
}

/**
 * Objects from sameTimeKeysKeys will be added to existing sameTimeKeys
 * This enables to use addToSameTimeKeys() without resolving before
 * Hint: it's your responsibility to resolve before looping thru sameTimeKeys
 */
void KeyLists::resolveSameTimeKeysKeys(QList<Key*> sameTimeKeys)
{
    if(mSameTimeKeysKeysResolved){
        return;
    }
    // don't clear mSameTimeKeys (see above)
    for (int i = 0; i < sameTimeKeys.size(); ++i) {
        addToSameTimeKeys(sameTimeKeys.at(i));
    }
    mSameTimeKeysKeysResolved = true;
}

int KeyLists::sameTimeKeysCount()
{
    return mSameTimeKeys.size();
}
QList<Key*> KeyLists::sameTimeKeys()
{
	return mSameTimeKeys;
}
void KeyLists::setSameTimeKeys(QList<Key*> sameTimeKeys) 
{
	if (sameTimeKeys != mSameTimeKeys) {
		mSameTimeKeys = sameTimeKeys;
		emit sameTimeKeysChanged(sameTimeKeys);
		emit sameTimeKeysPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * keyLists.sameTimeKeysPropertyList.length to get the size
 * keyLists.sameTimeKeysPropertyList[2] to get Key* at position 2
 * keyLists.sameTimeKeysPropertyList = [] to clear the list
 * or get easy access to properties like
 * keyLists.sameTimeKeysPropertyList[2].myPropertyName
 */
QQmlListProperty<Key> KeyLists::sameTimeKeysPropertyList()
{
    return QQmlListProperty<Key>(this, 0, &KeyLists::appendToSameTimeKeysProperty,
            &KeyLists::sameTimeKeysPropertyCount, &KeyLists::atSameTimeKeysProperty,
            &KeyLists::clearSameTimeKeysProperty);
}
void KeyLists::appendToSameTimeKeysProperty(QQmlListProperty<Key> *sameTimeKeysList,
        Key* key)
{
    KeyLists *keyListsObject = qobject_cast<KeyLists *>(sameTimeKeysList->object);
    if (keyListsObject) {
        keyListsObject->mSameTimeKeys.append(key);
        emit keyListsObject->addedToSameTimeKeys(key);
    } else {
        qWarning() << "cannot append Key* to sameTimeKeys " << "Object is not of type KeyLists*";
    }
}
int KeyLists::sameTimeKeysPropertyCount(QQmlListProperty<Key> *sameTimeKeysList)
{
    KeyLists *keyLists = qobject_cast<KeyLists *>(sameTimeKeysList->object);
    if (keyLists) {
        return keyLists->mSameTimeKeys.size();
    } else {
        qWarning() << "cannot get size sameTimeKeys " << "Object is not of type KeyLists*";
    }
    return 0;
}
Key* KeyLists::atSameTimeKeysProperty(QQmlListProperty<Key> *sameTimeKeysList, int pos)
{
    KeyLists *keyLists = qobject_cast<KeyLists *>(sameTimeKeysList->object);
    if (keyLists) {
        if (keyLists->mSameTimeKeys.size() > pos) {
            return keyLists->mSameTimeKeys.at(pos);
        }
        qWarning() << "cannot get Key* at pos " << pos << " size is "
                << keyLists->mSameTimeKeys.size();
    } else {
        qWarning() << "cannot get Key* at pos " << pos << "Object is not of type KeyLists*";
    }
    return 0;
}
void KeyLists::clearSameTimeKeysProperty(QQmlListProperty<Key> *sameTimeKeysList)
{
    KeyLists *keyLists = qobject_cast<KeyLists *>(sameTimeKeysList->object);
    if (keyLists) {
        // sameTimeKeys are independent - DON'T delete them
        keyLists->mSameTimeKeys.clear();
    } else {
        qWarning() << "cannot clear sameTimeKeys " << "Object is not of type KeyLists*";
    }
}

// ATT 
// Optional: specialTimeKeys
QVariantList KeyLists::specialTimeKeysAsQVariantList()
{
	QVariantList specialTimeKeysList;
	for (int i = 0; i < mSpecialTimeKeys.size(); ++i) {
        specialTimeKeysList.append((mSpecialTimeKeys.at(i))->toMap());
    }
	return specialTimeKeysList;
}
QVariantList KeyLists::specialTimeKeysAsForeignQVariantList()
{
	QVariantList specialTimeKeysList;
	for (int i = 0; i < mSpecialTimeKeys.size(); ++i) {
        specialTimeKeysList.append((mSpecialTimeKeys.at(i))->toForeignMap());
    }
	return specialTimeKeysList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add specialTimeKeys without resolving existing keys before
 * attention: before looping through the objects
 * you must resolveSpecialTimeKeysKeys
 */
void KeyLists::addToSpecialTimeKeys(Key* key)
{
    mSpecialTimeKeys.append(key);
    emit addedToSpecialTimeKeys(key);
    emit specialTimeKeysPropertyListChanged();
}

bool KeyLists::removeFromSpecialTimeKeys(Key* key)
{
    bool ok = false;
    ok = mSpecialTimeKeys.removeOne(key);
    if (!ok) {
    	qDebug() << "Key* not found in specialTimeKeys";
    	return false;
    }
    emit specialTimeKeysPropertyListChanged();
    // specialTimeKeys are independent - DON'T delete them
    return true;
}
void KeyLists::clearSpecialTimeKeys()
{
    for (int i = mSpecialTimeKeys.size(); i > 0; --i) {
        removeFromSpecialTimeKeys(mSpecialTimeKeys.last());
    }
    mSpecialTimeKeysKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: specialTimeKeysKeys()
 * - resolve them from DataManager
 * - then resolveSpecialTimeKeysKeys()
 */
bool KeyLists::areSpecialTimeKeysKeysResolved()
{
    return mSpecialTimeKeysKeysResolved;
}

QStringList KeyLists::specialTimeKeysKeys()
{
    return mSpecialTimeKeysKeys;
}

/**
 * Objects from specialTimeKeysKeys will be added to existing specialTimeKeys
 * This enables to use addToSpecialTimeKeys() without resolving before
 * Hint: it's your responsibility to resolve before looping thru specialTimeKeys
 */
void KeyLists::resolveSpecialTimeKeysKeys(QList<Key*> specialTimeKeys)
{
    if(mSpecialTimeKeysKeysResolved){
        return;
    }
    // don't clear mSpecialTimeKeys (see above)
    for (int i = 0; i < specialTimeKeys.size(); ++i) {
        addToSpecialTimeKeys(specialTimeKeys.at(i));
    }
    mSpecialTimeKeysKeysResolved = true;
}

int KeyLists::specialTimeKeysCount()
{
    return mSpecialTimeKeys.size();
}
QList<Key*> KeyLists::specialTimeKeys()
{
	return mSpecialTimeKeys;
}
void KeyLists::setSpecialTimeKeys(QList<Key*> specialTimeKeys) 
{
	if (specialTimeKeys != mSpecialTimeKeys) {
		mSpecialTimeKeys = specialTimeKeys;
		emit specialTimeKeysChanged(specialTimeKeys);
		emit specialTimeKeysPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * keyLists.specialTimeKeysPropertyList.length to get the size
 * keyLists.specialTimeKeysPropertyList[2] to get Key* at position 2
 * keyLists.specialTimeKeysPropertyList = [] to clear the list
 * or get easy access to properties like
 * keyLists.specialTimeKeysPropertyList[2].myPropertyName
 */
QQmlListProperty<Key> KeyLists::specialTimeKeysPropertyList()
{
    return QQmlListProperty<Key>(this, 0, &KeyLists::appendToSpecialTimeKeysProperty,
            &KeyLists::specialTimeKeysPropertyCount, &KeyLists::atSpecialTimeKeysProperty,
            &KeyLists::clearSpecialTimeKeysProperty);
}
void KeyLists::appendToSpecialTimeKeysProperty(QQmlListProperty<Key> *specialTimeKeysList,
        Key* key)
{
    KeyLists *keyListsObject = qobject_cast<KeyLists *>(specialTimeKeysList->object);
    if (keyListsObject) {
        keyListsObject->mSpecialTimeKeys.append(key);
        emit keyListsObject->addedToSpecialTimeKeys(key);
    } else {
        qWarning() << "cannot append Key* to specialTimeKeys " << "Object is not of type KeyLists*";
    }
}
int KeyLists::specialTimeKeysPropertyCount(QQmlListProperty<Key> *specialTimeKeysList)
{
    KeyLists *keyLists = qobject_cast<KeyLists *>(specialTimeKeysList->object);
    if (keyLists) {
        return keyLists->mSpecialTimeKeys.size();
    } else {
        qWarning() << "cannot get size specialTimeKeys " << "Object is not of type KeyLists*";
    }
    return 0;
}
Key* KeyLists::atSpecialTimeKeysProperty(QQmlListProperty<Key> *specialTimeKeysList, int pos)
{
    KeyLists *keyLists = qobject_cast<KeyLists *>(specialTimeKeysList->object);
    if (keyLists) {
        if (keyLists->mSpecialTimeKeys.size() > pos) {
            return keyLists->mSpecialTimeKeys.at(pos);
        }
        qWarning() << "cannot get Key* at pos " << pos << " size is "
                << keyLists->mSpecialTimeKeys.size();
    } else {
        qWarning() << "cannot get Key* at pos " << pos << "Object is not of type KeyLists*";
    }
    return 0;
}
void KeyLists::clearSpecialTimeKeysProperty(QQmlListProperty<Key> *specialTimeKeysList)
{
    KeyLists *keyLists = qobject_cast<KeyLists *>(specialTimeKeysList->object);
    if (keyLists) {
        // specialTimeKeys are independent - DON'T delete them
        keyLists->mSpecialTimeKeys.clear();
    } else {
        qWarning() << "cannot clear specialTimeKeys " << "Object is not of type KeyLists*";
    }
}



KeyLists::~KeyLists()
{
	// place cleanUp code here
}
	
