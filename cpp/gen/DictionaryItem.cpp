#include "DictionaryItem.hpp"
#include <QDebug>
#include <quuid.h>
// target also references to this
#include "Key.hpp"

// keys of QVariantMap used in this APP
static const QString keyIdKey = "keyId";
static const QString isBreakKey = "isBreak";
static const QString isLunchKey = "isLunch";
static const QString isEventKey = "isEvent";
static const QString isRegistrationKey = "isRegistration";
static const QString keyKey = "key";

// keys used from Server API etc
static const QString keyIdForeignKey = "keyId";
static const QString isBreakForeignKey = "isBreak";
static const QString isLunchForeignKey = "isLunch";
static const QString isEventForeignKey = "isEvent";
static const QString isRegistrationForeignKey = "isRegistration";
static const QString keyForeignKey = "key";

/*
 * Default Constructor if DictionaryItem not initialized from QVariantMap
 */
DictionaryItem::DictionaryItem(QObject *parent) :
        QObject(parent), mKeyId(-1), mIsBreak(false), mIsLunch(false), mIsEvent(false), mIsRegistration(false)
{
	// lazy references:
	mKey = -1;
	mKeyAsDataObject = 0;
	mKeyInvalid = false;
}

bool DictionaryItem::isAllResolved()
{
	if (hasKey() && !isKeyResolvedAsDataObject()) {
		return false;
	}
    return true;
}

/*
 * initialize DictionaryItem from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void DictionaryItem::fillFromMap(const QVariantMap& dictionaryItemMap)
{
	mKeyId = dictionaryItemMap.value(keyIdKey).toInt();
	mIsBreak = dictionaryItemMap.value(isBreakKey).toBool();
	mIsLunch = dictionaryItemMap.value(isLunchKey).toBool();
	mIsEvent = dictionaryItemMap.value(isEventKey).toBool();
	mIsRegistration = dictionaryItemMap.value(isRegistrationKey).toBool();
	// key lazy pointing to Key* (domainKey: keyId)
	if (dictionaryItemMap.contains(keyKey)) {
		mKey = dictionaryItemMap.value(keyKey).toInt();
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
void DictionaryItem::fillFromForeignMap(const QVariantMap& dictionaryItemMap)
{
	mKeyId = dictionaryItemMap.value(keyIdForeignKey).toInt();
	mIsBreak = dictionaryItemMap.value(isBreakForeignKey).toBool();
	mIsLunch = dictionaryItemMap.value(isLunchForeignKey).toBool();
	mIsEvent = dictionaryItemMap.value(isEventForeignKey).toBool();
	mIsRegistration = dictionaryItemMap.value(isRegistrationForeignKey).toBool();
	// key lazy pointing to Key* (domainKey: keyId)
	if (dictionaryItemMap.contains(keyForeignKey)) {
		mKey = dictionaryItemMap.value(keyForeignKey).toInt();
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
void DictionaryItem::fillFromCacheMap(const QVariantMap& dictionaryItemMap)
{
	mKeyId = dictionaryItemMap.value(keyIdKey).toInt();
	mIsBreak = dictionaryItemMap.value(isBreakKey).toBool();
	mIsLunch = dictionaryItemMap.value(isLunchKey).toBool();
	mIsEvent = dictionaryItemMap.value(isEventKey).toBool();
	mIsRegistration = dictionaryItemMap.value(isRegistrationKey).toBool();
	// key lazy pointing to Key* (domainKey: keyId)
	if (dictionaryItemMap.contains(keyKey)) {
		mKey = dictionaryItemMap.value(keyKey).toInt();
		if (mKey != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
}

void DictionaryItem::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool DictionaryItem::isValid()
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
 * Exports Properties from DictionaryItem as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap DictionaryItem::toMap()
{
	QVariantMap dictionaryItemMap;
	// key lazy pointing to Key* (domainKey: keyId)
	if (mKey != -1) {
		dictionaryItemMap.insert(keyKey, mKey);
	}
	dictionaryItemMap.insert(keyIdKey, mKeyId);
	dictionaryItemMap.insert(isBreakKey, mIsBreak);
	dictionaryItemMap.insert(isLunchKey, mIsLunch);
	dictionaryItemMap.insert(isEventKey, mIsEvent);
	dictionaryItemMap.insert(isRegistrationKey, mIsRegistration);
	return dictionaryItemMap;
}

/*
 * Exports Properties from DictionaryItem as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap DictionaryItem::toForeignMap()
{
	QVariantMap dictionaryItemMap;
	// key lazy pointing to Key* (domainKey: keyId)
	if (mKey != -1) {
		dictionaryItemMap.insert(keyForeignKey, mKey);
	}
	dictionaryItemMap.insert(keyIdForeignKey, mKeyId);
	dictionaryItemMap.insert(isBreakForeignKey, mIsBreak);
	dictionaryItemMap.insert(isLunchForeignKey, mIsLunch);
	dictionaryItemMap.insert(isEventForeignKey, mIsEvent);
	dictionaryItemMap.insert(isRegistrationForeignKey, mIsRegistration);
	return dictionaryItemMap;
}


/*
 * Exports Properties from DictionaryItem as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap DictionaryItem::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// REF
// Lazy: key
// Mandatory: key
// key lazy pointing to Key* (domainKey: keyId)
int DictionaryItem::key() const
{
	return mKey;
}
Key* DictionaryItem::keyAsDataObject() const
{
	return mKeyAsDataObject;
}
void DictionaryItem::setKey(int key)
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
void DictionaryItem::removeKey()
{
	if (mKey != -1) {
		setKey(-1);
	}
}
bool DictionaryItem::hasKey()
{
    if (!mKeyInvalid && mKey != -1) {
        return true;
    } else {
        return false;
    }
}
bool DictionaryItem::isKeyResolvedAsDataObject()
{
    if (!mKeyInvalid && mKeyAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite keyId if different
void DictionaryItem::resolveKeyAsDataObject(Key* key)
{
    if (key) {
        if (key->keyId() != mKey) {
            setKey(key->keyId());
        }
        mKeyAsDataObject = key;
        mKeyInvalid = false;
    }
}
void DictionaryItem::markKeyAsInvalid()
{
    mKeyInvalid = true;
}
// ATT 
// Mandatory: keyId
// Domain KEY: keyId
int DictionaryItem::keyId() const
{
	return mKeyId;
}

void DictionaryItem::setKeyId(int keyId)
{
	if (keyId != mKeyId) {
		mKeyId = keyId;
		emit keyIdChanged(keyId);
	}
}
// ATT 
// Optional: isBreak
bool DictionaryItem::isBreak() const
{
	return mIsBreak;
}

void DictionaryItem::setIsBreak(bool isBreak)
{
	if (isBreak != mIsBreak) {
		mIsBreak = isBreak;
		emit isBreakChanged(isBreak);
	}
}
// ATT 
// Optional: isLunch
bool DictionaryItem::isLunch() const
{
	return mIsLunch;
}

void DictionaryItem::setIsLunch(bool isLunch)
{
	if (isLunch != mIsLunch) {
		mIsLunch = isLunch;
		emit isLunchChanged(isLunch);
	}
}
// ATT 
// Optional: isEvent
bool DictionaryItem::isEvent() const
{
	return mIsEvent;
}

void DictionaryItem::setIsEvent(bool isEvent)
{
	if (isEvent != mIsEvent) {
		mIsEvent = isEvent;
		emit isEventChanged(isEvent);
	}
}
// ATT 
// Optional: isRegistration
bool DictionaryItem::isRegistration() const
{
	return mIsRegistration;
}

void DictionaryItem::setIsRegistration(bool isRegistration)
{
	if (isRegistration != mIsRegistration) {
		mIsRegistration = isRegistration;
		emit isRegistrationChanged(isRegistration);
	}
}


DictionaryItem::~DictionaryItem()
{
	// place cleanUp code here
}
	
