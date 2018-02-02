#include "KeyLinkAPI.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString uuidKey = "uuid";
static const QString urlKey = "url";
static const QString titleKey = "title";

// keys used from Server API etc
static const QString uuidForeignKey = "uuid";
static const QString urlForeignKey = "url";
static const QString titleForeignKey = "title";

/*
 * Default Constructor if KeyLinkAPI not initialized from QVariantMap
 */
KeyLinkAPI::KeyLinkAPI(QObject *parent) :
        QObject(parent), mUuid(""), mUrl(""), mTitle("")
{
}

/*
 * initialize KeyLinkAPI from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void KeyLinkAPI::fillFromMap(const QVariantMap& keyLinkAPIMap)
{
	mUuid = keyLinkAPIMap.value(uuidKey).toString();
	if (mUuid.isEmpty()) {
		mUuid = QUuid::createUuid().toString();
		mUuid = mUuid.right(mUuid.length() - 1);
		mUuid = mUuid.left(mUuid.length() - 1);
	}	
	mUrl = keyLinkAPIMap.value(urlKey).toString();
	mTitle = keyLinkAPIMap.value(titleKey).toString();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void KeyLinkAPI::fillFromForeignMap(const QVariantMap& keyLinkAPIMap)
{
	mUuid = keyLinkAPIMap.value(uuidForeignKey).toString();
	if (mUuid.isEmpty()) {
		mUuid = QUuid::createUuid().toString();
		mUuid = mUuid.right(mUuid.length() - 1);
		mUuid = mUuid.left(mUuid.length() - 1);
	}	
	mUrl = keyLinkAPIMap.value(urlForeignKey).toString();
	mTitle = keyLinkAPIMap.value(titleForeignKey).toString();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void KeyLinkAPI::fillFromCacheMap(const QVariantMap& keyLinkAPIMap)
{
	mUuid = keyLinkAPIMap.value(uuidKey).toString();
	if (mUuid.isEmpty()) {
		mUuid = QUuid::createUuid().toString();
		mUuid = mUuid.right(mUuid.length() - 1);
		mUuid = mUuid.left(mUuid.length() - 1);
	}	
	mUrl = keyLinkAPIMap.value(urlKey).toString();
	mTitle = keyLinkAPIMap.value(titleKey).toString();
}

void KeyLinkAPI::prepareNew()
{
	mUuid = QUuid::createUuid().toString();
	mUuid = mUuid.right(mUuid.length() - 1);
	mUuid = mUuid.left(mUuid.length() - 1);
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool KeyLinkAPI::isValid()
{
	if (mUuid.isNull() || mUuid.isEmpty()) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from KeyLinkAPI as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap KeyLinkAPI::toMap()
{
	QVariantMap keyLinkAPIMap;
	keyLinkAPIMap.insert(uuidKey, mUuid);
	keyLinkAPIMap.insert(urlKey, mUrl);
	keyLinkAPIMap.insert(titleKey, mTitle);
	return keyLinkAPIMap;
}

/*
 * Exports Properties from KeyLinkAPI as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap KeyLinkAPI::toForeignMap()
{
	QVariantMap keyLinkAPIMap;
	keyLinkAPIMap.insert(uuidForeignKey, mUuid);
	keyLinkAPIMap.insert(urlForeignKey, mUrl);
	keyLinkAPIMap.insert(titleForeignKey, mTitle);
	return keyLinkAPIMap;
}


/*
 * Exports Properties from KeyLinkAPI as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap KeyLinkAPI::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: uuid
// Domain KEY: uuid
QString KeyLinkAPI::uuid() const
{
	return mUuid;
}

void KeyLinkAPI::setUuid(QString uuid)
{
	if (uuid != mUuid) {
		mUuid = uuid;
		emit uuidChanged(uuid);
	}
}
// ATT 
// Optional: url
QString KeyLinkAPI::url() const
{
	return mUrl;
}

void KeyLinkAPI::setUrl(QString url)
{
	if (url != mUrl) {
		mUrl = url;
		emit urlChanged(url);
	}
}
// ATT 
// Optional: title
QString KeyLinkAPI::title() const
{
	return mTitle;
}

void KeyLinkAPI::setTitle(QString title)
{
	if (title != mTitle) {
		mTitle = title;
		emit titleChanged(title);
	}
}


KeyLinkAPI::~KeyLinkAPI()
{
	// place cleanUp code here
}
	
