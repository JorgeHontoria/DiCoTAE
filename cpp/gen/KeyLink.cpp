#include "KeyLink.hpp"
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
 * Default Constructor if KeyLink not initialized from QVariantMap
 */
KeyLink::KeyLink(QObject *parent) :
        QObject(parent), mUuid(""), mUrl(""), mTitle("")
{
}

/*
 * initialize KeyLink from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void KeyLink::fillFromMap(const QVariantMap& keyLinkMap)
{
	mUuid = keyLinkMap.value(uuidKey).toString();
	if (mUuid.isEmpty()) {
		mUuid = QUuid::createUuid().toString();
		mUuid = mUuid.right(mUuid.length() - 1);
		mUuid = mUuid.left(mUuid.length() - 1);
	}	
	mUrl = keyLinkMap.value(urlKey).toString();
	mTitle = keyLinkMap.value(titleKey).toString();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void KeyLink::fillFromForeignMap(const QVariantMap& keyLinkMap)
{
	mUuid = keyLinkMap.value(uuidForeignKey).toString();
	if (mUuid.isEmpty()) {
		mUuid = QUuid::createUuid().toString();
		mUuid = mUuid.right(mUuid.length() - 1);
		mUuid = mUuid.left(mUuid.length() - 1);
	}	
	mUrl = keyLinkMap.value(urlForeignKey).toString();
	mTitle = keyLinkMap.value(titleForeignKey).toString();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void KeyLink::fillFromCacheMap(const QVariantMap& keyLinkMap)
{
	mUuid = keyLinkMap.value(uuidKey).toString();
	if (mUuid.isEmpty()) {
		mUuid = QUuid::createUuid().toString();
		mUuid = mUuid.right(mUuid.length() - 1);
		mUuid = mUuid.left(mUuid.length() - 1);
	}	
	mUrl = keyLinkMap.value(urlKey).toString();
	mTitle = keyLinkMap.value(titleKey).toString();
}

void KeyLink::prepareNew()
{
	mUuid = QUuid::createUuid().toString();
	mUuid = mUuid.right(mUuid.length() - 1);
	mUuid = mUuid.left(mUuid.length() - 1);
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool KeyLink::isValid()
{
	if (mUuid.isNull() || mUuid.isEmpty()) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from KeyLink as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap KeyLink::toMap()
{
	QVariantMap keyLinkMap;
	keyLinkMap.insert(uuidKey, mUuid);
	keyLinkMap.insert(urlKey, mUrl);
	keyLinkMap.insert(titleKey, mTitle);
	return keyLinkMap;
}

/*
 * Exports Properties from KeyLink as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap KeyLink::toForeignMap()
{
	QVariantMap keyLinkMap;
	keyLinkMap.insert(uuidForeignKey, mUuid);
	keyLinkMap.insert(urlForeignKey, mUrl);
	keyLinkMap.insert(titleForeignKey, mTitle);
	return keyLinkMap;
}


/*
 * Exports Properties from KeyLink as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap KeyLink::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: uuid
// Domain KEY: uuid
QString KeyLink::uuid() const
{
	return mUuid;
}

void KeyLink::setUuid(QString uuid)
{
	if (uuid != mUuid) {
		mUuid = uuid;
		emit uuidChanged(uuid);
	}
}
// ATT 
// Optional: url
QString KeyLink::url() const
{
	return mUrl;
}

void KeyLink::setUrl(QString url)
{
	if (url != mUrl) {
		mUrl = url;
		emit urlChanged(url);
	}
}
// ATT 
// Optional: title
QString KeyLink::title() const
{
	return mTitle;
}

void KeyLink::setTitle(QString title)
{
	if (title != mTitle) {
		mTitle = title;
		emit titleChanged(title);
	}
}


KeyLink::~KeyLink()
{
	// place cleanUp code here
}
	
