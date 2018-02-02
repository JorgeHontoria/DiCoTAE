#include "KeyContentStandardType.hpp"
#include <QDebug>
#include <quuid.h>
// target also references to this
#include "Key.hpp"

// keys of QVariantMap used in this APP
static const QString contentStandardTypeIdKey = "contentStandardTypeId";
static const QString nameKey = "name";
static const QString inAssetsKey = "inAssets";
static const QString keysKey = "keys";

// keys used from Server API etc
static const QString contentStandardTypeIdForeignKey = "contentStandardTypeId";
static const QString nameForeignKey = "name";
static const QString inAssetsForeignKey = "inAssets";
static const QString keysForeignKey = "keys";

/*
 * Default Constructor if KeyContentStandardType not initialized from QVariantMap
 */
KeyContentStandardType::KeyContentStandardType(QObject *parent) :
        QObject(parent), mContentStandardTypeId(-1), mName(""), mInAssets(false)
{
        // lazy Arrays where only keys are persisted
        mKeysKeysResolved = false;
}

bool KeyContentStandardType::isAllResolved()
{
    if(!areKeysKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize KeyContentStandardType from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void KeyContentStandardType::fillFromMap(const QVariantMap& keyContentStandardTypeMap)
{
    mContentStandardTypeId = keyContentStandardTypeMap.value(contentStandardTypeIdKey).toInt();
    mName = keyContentStandardTypeMap.value(nameKey).toString();
    mInAssets = keyContentStandardTypeMap.value(inAssetsKey).toBool();
    // mKeys is (lazy loaded) Array of Key*
    mKeysKeys = keyContentStandardTypeMap.value(keysKey).toStringList();
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
void KeyContentStandardType::fillFromForeignMap(const QVariantMap& keyContentStandardTypeMap)
{
    mContentStandardTypeId = keyContentStandardTypeMap.value(contentStandardTypeIdForeignKey).toInt();
    mName = keyContentStandardTypeMap.value(nameForeignKey).toString();
    mInAssets = keyContentStandardTypeMap.value(inAssetsForeignKey).toBool();
    // mKeys is (lazy loaded) Array of Key*
    mKeysKeys = keyContentStandardTypeMap.value(keysForeignKey).toStringList();
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
void KeyContentStandardType::fillFromCacheMap(const QVariantMap& keyContentStandardTypeMap)
{
    mContentStandardTypeId = keyContentStandardTypeMap.value(contentStandardTypeIdKey).toInt();
    mName = keyContentStandardTypeMap.value(nameKey).toString();
    mInAssets = keyContentStandardTypeMap.value(inAssetsKey).toBool();
    // mKeys is (lazy loaded) Array of Key*
    mKeysKeys = keyContentStandardTypeMap.value(keysKey).toStringList();
    // mKeys must be resolved later if there are keys
    mKeysKeysResolved = (mKeysKeys.size() == 0);
    mKeys.clear();
}

void KeyContentStandardType::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool KeyContentStandardType::isValid()
{
    if (mContentStandardTypeId == -1) {
        return false;
    }
    return true;
}

/*
 * Exports Properties from KeyContentStandardType as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap KeyContentStandardType::toMap()
{
    QVariantMap keyContentStandardTypeMap;
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
    keyContentStandardTypeMap.insert(keysKey, mKeysKeys);
    keyContentStandardTypeMap.insert(contentStandardTypeIdKey, mContentStandardTypeId);
    keyContentStandardTypeMap.insert(nameKey, mName);
    keyContentStandardTypeMap.insert(inAssetsKey, mInAssets);
    return keyContentStandardTypeMap;
}

/*
 * Exports Properties from KeyContentStandardType as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap KeyContentStandardType::toForeignMap()
{
    QVariantMap keyContentStandardTypeMap;
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
    keyContentStandardTypeMap.insert(keysKey, mKeysKeys);
    keyContentStandardTypeMap.insert(contentStandardTypeIdForeignKey, mContentStandardTypeId);
    keyContentStandardTypeMap.insert(nameForeignKey, mName);
    keyContentStandardTypeMap.insert(inAssetsForeignKey, mInAssets);
    return keyContentStandardTypeMap;
}


/*
 * Exports Properties from KeyContentStandardType as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap KeyContentStandardType::toCacheMap()
{
    // no transient properties found from data model
    // use default toMao()
    return toMap();
}
// ATT
// Mandatory: contentStandardTypeId
// Domain KEY: contentStandardTypeId
int KeyContentStandardType::contentStandardTypeId() const
{
    return mContentStandardTypeId;
}

void KeyContentStandardType::setContentStandardTypeId(int contentStandardTypeId)
{
    if (contentStandardTypeId != mContentStandardTypeId) {
        mContentStandardTypeId = contentStandardTypeId;
        emit contentStandardTypeIdChanged(contentStandardTypeId);
    }
}
// ATT
// Optional: name
QString KeyContentStandardType::name() const
{
    return mName;
}

void KeyContentStandardType::setName(QString name)
{
    if (name != mName) {
        mName = name;
        emit nameChanged(name);
    }
}
// ATT
// Optional: inAssets
bool KeyContentStandardType::inAssets() const
{
    return mInAssets;
}

void KeyContentStandardType::setInAssets(bool inAssets)
{
    if (inAssets != mInAssets) {
        mInAssets = inAssets;
        emit inAssetsChanged(inAssets);
    }
}
// ATT
// Optional: keys
QVariantList KeyContentStandardType::keysAsQVariantList()
{
    QVariantList keysList;
    for (int i = 0; i < mKeys.size(); ++i) {
        keysList.append((mKeys.at(i))->toMap());
    }
    return keysList;
}
QVariantList KeyContentStandardType::keysAsForeignQVariantList()
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
void KeyContentStandardType::addToKeys(Key* key)
{
    mKeys.append(key);
    emit addedToKeys(key);
    emit keysPropertyListChanged();
}

bool KeyContentStandardType::removeFromKeys(Key* key)
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
void KeyContentStandardType::clearKeys()
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
bool KeyContentStandardType::areKeysKeysResolved()
{
    return mKeysKeysResolved;
}

QStringList KeyContentStandardType::keysKeys()
{
    return mKeysKeys;
}

/**
 * Objects from keysKeys will be added to existing keys
 * This enables to use addToKeys() without resolving before
 * Hint: it's your responsibility to resolve before looping thru keys
 */
void KeyContentStandardType::resolveKeysKeys(QList<Key*> keys)
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

int KeyContentStandardType::keysCount()
{
    return mKeys.size();
}
QList<Key*> KeyContentStandardType::keys()
{
    return mKeys;
}
void KeyContentStandardType::setKeys(QList<Key*> keys)
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
 * keyContentStandardType.keysPropertyList.length to get the size
 * keyContentStandardType.keysPropertyList[2] to get Key* at position 2
 * keyContentStandardType.keysPropertyList = [] to clear the list
 * or get easy access to properties like
 * keyContentStandardType.keysPropertyList[2].myPropertyName
 */
QQmlListProperty<Key> KeyContentStandardType::keysPropertyList()
{
    return QQmlListProperty<Key>(this, 0, &KeyContentStandardType::appendToKeysProperty,
            &KeyContentStandardType::keysPropertyCount, &KeyContentStandardType::atKeysProperty,
            &KeyContentStandardType::clearKeysProperty);
}
void KeyContentStandardType::appendToKeysProperty(QQmlListProperty<Key> *keysList,
        Key* key)
{
    KeyContentStandardType *keyContentStandardTypeObject = qobject_cast<KeyContentStandardType *>(keysList->object);
    if (keyContentStandardTypeObject) {
        keyContentStandardTypeObject->mKeys.append(key);
        emit keyContentStandardTypeObject->addedToKeys(key);
    } else {
        qWarning() << "cannot append Key* to keys " << "Object is not of type KeyContentStandardType*";
    }
}
int KeyContentStandardType::keysPropertyCount(QQmlListProperty<Key> *keysList)
{
    KeyContentStandardType *keyContentStandardType = qobject_cast<KeyContentStandardType *>(keysList->object);
    if (keyContentStandardType) {
        return keyContentStandardType->mKeys.size();
    } else {
        qWarning() << "cannot get size keys " << "Object is not of type KeyContentStandardType*";
    }
    return 0;
}
Key* KeyContentStandardType::atKeysProperty(QQmlListProperty<Key> *keysList, int pos)
{
    KeyContentStandardType *keyContentStandardType = qobject_cast<KeyContentStandardType *>(keysList->object);
    if (keyContentStandardType) {
        if (keyContentStandardType->mKeys.size() > pos) {
            return keyContentStandardType->mKeys.at(pos);
        }
        qWarning() << "cannot get Key* at pos " << pos << " size is "
                << keyContentStandardType->mKeys.size();
    } else {
        qWarning() << "cannot get Key* at pos " << pos << "Object is not of type KeyContentStandardType*";
    }
    return 0;
}
void KeyContentStandardType::clearKeysProperty(QQmlListProperty<Key> *keysList)
{
    KeyContentStandardType *keyContentStandardType = qobject_cast<KeyContentStandardType *>(keysList->object);
    if (keyContentStandardType) {
        // keys are independent - DON'T delete them
        keyContentStandardType->mKeys.clear();
    } else {
        qWarning() << "cannot clear keys " << "Object is not of type KeyContentStandardType*";
    }
}



KeyContentStandardType::~KeyContentStandardType()
{
    // place cleanUp code here
}

