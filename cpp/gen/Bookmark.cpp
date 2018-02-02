#include "Bookmark.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString keyIdKey = "keyId";
static const QString keyKey = "key";

// keys used from Server API etc
static const QString keyIdForeignKey = "keyId";
static const QString keyForeignKey = "key";

/*
 * Default Constructor if Bookmark not initialized from QVariantMap
 */
Bookmark::Bookmark(QObject *parent) :
        QObject(parent), mKeyId(-1)
{
	// lazy references:
	mKey = -1;
	mKeyAsDataObject = 0;
	mKeyInvalid = false;
}

bool Bookmark::isAllResolved()
{
	if (hasKey() && !isKeyResolvedAsDataObject()) {
		return false;
	}
    return true;
}

/*
 * initialize Bookmark from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Bookmark::fillFromMap(const QVariantMap& bookmarkMap)
{
	mKeyId = bookmarkMap.value(keyIdKey).toInt();
	// key lazy pointing to Key* (domainKey: keyId)
	if (bookmarkMap.contains(keyKey)) {
		mKey = bookmarkMap.value(keyKey).toInt();
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
void Bookmark::fillFromForeignMap(const QVariantMap& bookmarkMap)
{
	mKeyId = bookmarkMap.value(keyIdForeignKey).toInt();
	// key lazy pointing to Key* (domainKey: keyId)
	if (bookmarkMap.contains(keyForeignKey)) {
		mKey = bookmarkMap.value(keyForeignKey).toInt();
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
void Bookmark::fillFromCacheMap(const QVariantMap& bookmarkMap)
{
	mKeyId = bookmarkMap.value(keyIdKey).toInt();
	// key lazy pointing to Key* (domainKey: keyId)
	if (bookmarkMap.contains(keyKey)) {
		mKey = bookmarkMap.value(keyKey).toInt();
		if (mKey != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
}

void Bookmark::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Bookmark::isValid()
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
 * Exports Properties from Bookmark as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Bookmark::toMap()
{
	QVariantMap bookmarkMap;
	// key lazy pointing to Key* (domainKey: keyId)
	if (mKey != -1) {
		bookmarkMap.insert(keyKey, mKey);
	}
	bookmarkMap.insert(keyIdKey, mKeyId);
	return bookmarkMap;
}

/*
 * Exports Properties from Bookmark as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Bookmark::toForeignMap()
{
	QVariantMap bookmarkMap;
	// key lazy pointing to Key* (domainKey: keyId)
	if (mKey != -1) {
		bookmarkMap.insert(keyForeignKey, mKey);
	}
	bookmarkMap.insert(keyIdForeignKey, mKeyId);
	return bookmarkMap;
}


/*
 * Exports Properties from Bookmark as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Bookmark::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// REF
// Lazy: key
// Mandatory: key
// key lazy pointing to Key* (domainKey: keyId)
int Bookmark::key() const
{
	return mKey;
}
Key* Bookmark::keyAsDataObject() const
{
	return mKeyAsDataObject;
}
void Bookmark::setKey(int key)
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
void Bookmark::removeKey()
{
	if (mKey != -1) {
		setKey(-1);
	}
}
bool Bookmark::hasKey()
{
    if (!mKeyInvalid && mKey != -1) {
        return true;
    } else {
        return false;
    }
}
bool Bookmark::isKeyResolvedAsDataObject()
{
    if (!mKeyInvalid && mKeyAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite keyId if different
void Bookmark::resolveKeyAsDataObject(Key* key)
{
    if (key) {
        if (key->keyId() != mKey) {
            setKey(key->keyId());
        }
        mKeyAsDataObject = key;
        mKeyInvalid = false;
    }
}
void Bookmark::markKeyAsInvalid()
{
    mKeyInvalid = true;
}
// ATT 
// Mandatory: keyId
// Domain KEY: keyId
int Bookmark::keyId() const
{
	return mKeyId;
}

void Bookmark::setKeyId(int keyId)
{
	if (keyId != mKeyId) {
		mKeyId = keyId;
		emit keyIdChanged(keyId);
	}
}


Bookmark::~Bookmark()
{
	// place cleanUp code here
}
	
