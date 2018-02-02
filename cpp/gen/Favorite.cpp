#include "Favorite.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString keyIdKey = "keyId";
static const QString keyKey = "key";

// keys used from Server API etc
static const QString keyIdForeignKey = "keyId";
static const QString keyForeignKey = "key";

/*
 * Default Constructor if Favorite not initialized from QVariantMap
 */
Favorite::Favorite(QObject *parent) :
        QObject(parent), mKeyId(-1)
{
	// lazy references:
	mKey = -1;
	mKeyAsDataObject = 0;
	mKeyInvalid = false;
}

bool Favorite::isAllResolved()
{
	if (hasKey() && !isKeyResolvedAsDataObject()) {
		return false;
	}
    return true;
}

/*
 * initialize Favorite from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Favorite::fillFromMap(const QVariantMap& favoriteMap)
{
	mKeyId = favoriteMap.value(keyIdKey).toInt();
	// key lazy pointing to Key* (domainKey: keyId)
	if (favoriteMap.contains(keyKey)) {
		mKey = favoriteMap.value(keyKey).toInt();
		if (mKey != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void Favorite::fillFromForeignMap(const QVariantMap& favoriteMap)
{
	mKeyId = favoriteMap.value(keyIdForeignKey).toInt();
	// key lazy pointing to Key* (domainKey: keyId)
	if (favoriteMap.contains(keyForeignKey)) {
		mKey = favoriteMap.value(keyForeignKey).toInt();
		if (mKey != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void Favorite::fillFromCacheMap(const QVariantMap& favoriteMap)
{
	mKeyId = favoriteMap.value(keyIdKey).toInt();
	// key lazy pointing to Key* (domainKey: keyId)
	if (favoriteMap.contains(keyKey)) {
		mKey = favoriteMap.value(keyKey).toInt();
		if (mKey != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
}

void Favorite::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Favorite::isValid()
{
	if (mKeyId == -1) {
		return false;
	}
	// key lazy pointing to Key* (domainKey: keyId)
	if (mKey == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Favorite as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Favorite::toMap()
{
	QVariantMap favoriteMap;
	// key lazy pointing to Key* (domainKey: keyId)
	if (mKey != -1) {
		favoriteMap.insert(keyKey, mKey);
	}
	favoriteMap.insert(keyIdKey, mKeyId);
	return favoriteMap;
}

/*
 * Exports Properties from Favorite as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Favorite::toForeignMap()
{
	QVariantMap favoriteMap;
	// key lazy pointing to Key* (domainKey: keyId)
	if (mKey != -1) {
		favoriteMap.insert(keyForeignKey, mKey);
	}
	favoriteMap.insert(keyIdForeignKey, mKeyId);
	return favoriteMap;
}


/*
 * Exports Properties from Favorite as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Favorite::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// REF
// Lazy: key
// Mandatory: key
// key lazy pointing to Key* (domainKey: keyId)
int Favorite::key() const
{
	return mKey;
}
Key* Favorite::keyAsDataObject() const
{
	return mKeyAsDataObject;
}
void Favorite::setKey(int key)
{
	if (key != mKey) {
        // remove old Data Object if one was resolved
        if (mKeyAsDataObject) {
            // reset pointer, don't delete the independent object !
            mKeyAsDataObject = 0;
        }
        // set the new lazy reference
        mKey = key;
        mKeyInvalid = false;
        emit keyChanged(key);
        if (key != -1) {
            // resolve the corresponding Data Object on demand from DataManager
        }
    }
}
void Favorite::removeKey()
{
	if (mKey != -1) {
		setKey(-1);
	}
}
bool Favorite::hasKey()
{
    if (!mKeyInvalid && mKey != -1) {
        return true;
    } else {
        return false;
    }
}
bool Favorite::isKeyResolvedAsDataObject()
{
    if (!mKeyInvalid && mKeyAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite keyId if different
void Favorite::resolveKeyAsDataObject(Key* key)
{
    if (key) {
        if (key->keyId() != mKey) {
            setKey(key->keyId());
        }
        mKeyAsDataObject = key;
        mKeyInvalid = false;
    }
}
void Favorite::markKeyAsInvalid()
{
    mKeyInvalid = true;
}
// ATT 
// Mandatory: keyId
// Domain KEY: keyId
int Favorite::keyId() const
{
	return mKeyId;
}

void Favorite::setKeyId(int keyId)
{
	if (keyId != mKeyId) {
		mKeyId = keyId;
		emit keyIdChanged(keyId);
	}
}


Favorite::~Favorite()
{
	// place cleanUp code here
}
	
