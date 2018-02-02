#include "KeyContentBlock.hpp"
#include <QDebug>
#include <quuid.h>
// target also references to this
#include "Key.hpp"

// keys of QVariantMap used in this APP
static const QString contentBlockIdKey = "contentBlockId";
static const QString nameKey = "name";
static const QString inAssetsKey = "inAssets";
static const QString keysKey = "keys";

// keys used from Server API etc
static const QString contentBlockIdForeignKey = "contentBlockId";
static const QString nameForeignKey = "name";
static const QString inAssetsForeignKey = "inAssets";
static const QString keysForeignKey = "keys";

/*
 * Default Constructor if KeyContentBlock not initialized from QVariantMap
 */
KeyContentBlock::KeyContentBlock(QObject *parent) :
        QObject(parent), mContentBlockId(-1), mName(""), mInAssets(false)
{
        // lazy Arrays where only keys are persisted
        mKeysKeysResolved = false;
}

bool KeyContentBlock::isAllResolved()
{
    if(!areKeysKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize KeyContentBlock from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void KeyContentBlock::fillFromMap(const QVariantMap& keyContentBlockMap)
{
    mContentBlockId = keyContentBlockMap.value(contentBlockIdKey).toInt();
    mName = keyContentBlockMap.value(nameKey).toString();
    mInAssets = keyContentBlockMap.value(inAssetsKey).toBool();
    // mKeys is (lazy loaded) Array of Key*
    mKeysKeys = keyContentBlockMap.value(keysKey).toStringList();
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
void KeyContentBlock::fillFromForeignMap(const QVariantMap& keyContentBlockMap)
{
    mContentBlockId = keyContentBlockMap.value(contentBlockIdForeignKey).toInt();
    mName = keyContentBlockMap.value(nameForeignKey).toString();
    mInAssets = keyContentBlockMap.value(inAssetsForeignKey).toBool();
    // mKeys is (lazy loaded) Array of Key*
    mKeysKeys = keyContentBlockMap.value(keysForeignKey).toStringList();
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
void KeyContentBlock::fillFromCacheMap(const QVariantMap& keyContentBlockMap)
{
    mContentBlockId = keyContentBlockMap.value(contentBlockIdKey).toInt();
    mName = keyContentBlockMap.value(nameKey).toString();
    mInAssets = keyContentBlockMap.value(inAssetsKey).toBool();
    // mKeys is (lazy loaded) Array of Key*
    mKeysKeys = keyContentBlockMap.value(keysKey).toStringList();
    // mKeys must be resolved later if there are keys
    mKeysKeysResolved = (mKeysKeys.size() == 0);
    mKeys.clear();
}

void KeyContentBlock::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool KeyContentBlock::isValid()
{
    if (mContentBlockId == -1) {
        return false;
    }
    return true;
}

/*
 * Exports Properties from KeyContentBlock as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap KeyContentBlock::toMap()
{
    QVariantMap keyContentBlockMap;
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
    keyContentBlockMap.insert(keysKey, mKeysKeys);
    keyContentBlockMap.insert(contentBlockIdKey, mContentBlockId);
    keyContentBlockMap.insert(nameKey, mName);
    keyContentBlockMap.insert(inAssetsKey, mInAssets);
    return keyContentBlockMap;
}

/*
 * Exports Properties from KeyContentBlock as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap KeyContentBlock::toForeignMap()
{
    QVariantMap keyContentBlockMap;
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
    keyContentBlockMap.insert(keysKey, mKeysKeys);
    keyContentBlockMap.insert(contentBlockIdForeignKey, mContentBlockId);
    keyContentBlockMap.insert(nameForeignKey, mName);
    keyContentBlockMap.insert(inAssetsForeignKey, mInAssets);
    return keyContentBlockMap;
}


/*
 * Exports Properties from KeyContentBlock as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap KeyContentBlock::toCacheMap()
{
    // no transient properties found from data model
    // use default toMao()
    return toMap();
}
// ATT
// Mandatory: contentBlockId
// Domain KEY: contentBlockId
int KeyContentBlock::contentBlockId() const
{
    return mContentBlockId;
}

void KeyContentBlock::setContentBlockId(int contentBlockId)
{
    if (contentBlockId != mContentBlockId) {
        mContentBlockId = contentBlockId;
        emit contentBlockIdChanged(contentBlockId);
    }
}
// ATT
// Optional: name
QString KeyContentBlock::name() const
{
    return mName;
}

void KeyContentBlock::setName(QString name)
{
    if (name != mName) {
        mName = name;
        emit nameChanged(name);
    }
}
// ATT
// Optional: inAssets
bool KeyContentBlock::inAssets() const
{
    return mInAssets;
}

void KeyContentBlock::setInAssets(bool inAssets)
{
    if (inAssets != mInAssets) {
        mInAssets = inAssets;
        emit inAssetsChanged(inAssets);
    }
}
// ATT
// Optional: keys
QVariantList KeyContentBlock::keysAsQVariantList()
{
    QVariantList keysList;
    for (int i = 0; i < mKeys.size(); ++i) {
        keysList.append((mKeys.at(i))->toMap());
    }
    return keysList;
}
QVariantList KeyContentBlock::keysAsForeignQVariantList()
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
void KeyContentBlock::addToKeys(Key* key)
{
    mKeys.append(key);
    emit addedToKeys(key);
    emit keysPropertyListChanged();
}

bool KeyContentBlock::removeFromKeys(Key* key)
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
void KeyContentBlock::clearKeys()
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
bool KeyContentBlock::areKeysKeysResolved()
{
    return mKeysKeysResolved;
}

QStringList KeyContentBlock::keysKeys()
{
    return mKeysKeys;
}

/**
 * Objects from keysKeys will be added to existing keys
 * This enables to use addToKeys() without resolving before
 * Hint: it's your responsibility to resolve before looping thru keys
 */
void KeyContentBlock::resolveKeysKeys(QList<Key*> keys)
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

int KeyContentBlock::keysCount()
{
    return mKeys.size();
}
QList<Key*> KeyContentBlock::keys()
{
    return mKeys;
}
void KeyContentBlock::setKeys(QList<Key*> keys)
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
 * keyContentBlock.keysPropertyList.length to get the size
 * keyContentBlock.keysPropertyList[2] to get Key* at position 2
 * keyContentBlock.keysPropertyList = [] to clear the list
 * or get easy access to properties like
 * keyContentBlock.keysPropertyList[2].myPropertyName
 */
QQmlListProperty<Key> KeyContentBlock::keysPropertyList()
{
    return QQmlListProperty<Key>(this, 0, &KeyContentBlock::appendToKeysProperty,
            &KeyContentBlock::keysPropertyCount, &KeyContentBlock::atKeysProperty,
            &KeyContentBlock::clearKeysProperty);
}
void KeyContentBlock::appendToKeysProperty(QQmlListProperty<Key> *keysList,
        Key* key)
{
    KeyContentBlock *keyContentBlockObject = qobject_cast<KeyContentBlock *>(keysList->object);
    if (keyContentBlockObject) {
        keyContentBlockObject->mKeys.append(key);
        emit keyContentBlockObject->addedToKeys(key);
    } else {
        qWarning() << "cannot append Key* to keys " << "Object is not of type KeyContentBlock*";
    }
}
int KeyContentBlock::keysPropertyCount(QQmlListProperty<Key> *keysList)
{
    KeyContentBlock *keyContentBlock = qobject_cast<KeyContentBlock *>(keysList->object);
    if (keyContentBlock) {
        return keyContentBlock->mKeys.size();
    } else {
        qWarning() << "cannot get size keys " << "Object is not of type KeyContentBlock*";
    }
    return 0;
}
Key* KeyContentBlock::atKeysProperty(QQmlListProperty<Key> *keysList, int pos)
{
    KeyContentBlock *keyContentBlock = qobject_cast<KeyContentBlock *>(keysList->object);
    if (keyContentBlock) {
        if (keyContentBlock->mKeys.size() > pos) {
            return keyContentBlock->mKeys.at(pos);
        }
        qWarning() << "cannot get Key* at pos " << pos << " size is "
                << keyContentBlock->mKeys.size();
    } else {
        qWarning() << "cannot get Key* at pos " << pos << "Object is not of type KeyContentBlock*";
    }
    return 0;
}
void KeyContentBlock::clearKeysProperty(QQmlListProperty<Key> *keysList)
{
    KeyContentBlock *keyContentBlock = qobject_cast<KeyContentBlock *>(keysList->object);
    if (keyContentBlock) {
        // keys are independent - DON'T delete them
        keyContentBlock->mKeys.clear();
    } else {
        qWarning() << "cannot clear keys " << "Object is not of type KeyContentBlock*";
    }
}



KeyContentBlock::~KeyContentBlock()
{
    // place cleanUp code here
}

