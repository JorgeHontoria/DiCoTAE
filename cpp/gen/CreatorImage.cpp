#include "CreatorImage.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString creatorIdKey = "creatorId";
static const QString originImageUrlKey = "originImageUrl";
static const QString downloadSuccessKey = "downloadSuccess";
static const QString downloadFailedKey = "downloadFailed";
static const QString inAssetsKey = "inAssets";
static const QString inDataKey = "inData";
static const QString suffixKey = "suffix";
static const QString maxScaleFactorKey = "maxScaleFactor";

// keys used from Server API etc
static const QString creatorIdForeignKey = "creatorId";
static const QString originImageUrlForeignKey = "originImageUrl";
static const QString downloadSuccessForeignKey = "downloadSuccess";
static const QString downloadFailedForeignKey = "downloadFailed";
static const QString inAssetsForeignKey = "inAssets";
static const QString inDataForeignKey = "inData";
static const QString suffixForeignKey = "suffix";
static const QString maxScaleFactorForeignKey = "maxScaleFactor";

/*
 * Default Constructor if CreatorImage not initialized from QVariantMap
 */
CreatorImage::CreatorImage(QObject *parent) :
        QObject(parent), mCreatorId(-1), mOriginImageUrl(""), mDownloadSuccess(false), mDownloadFailed(false), mInAssets(false), mInData(false), mSuffix(""), mMaxScaleFactor(0)
{
}

/*
 * initialize CreatorImage from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void CreatorImage::fillFromMap(const QVariantMap& creatorImageMap)
{
	mCreatorId = creatorImageMap.value(creatorIdKey).toInt();
	mOriginImageUrl = creatorImageMap.value(originImageUrlKey).toString();
	mDownloadSuccess = creatorImageMap.value(downloadSuccessKey).toBool();
	mDownloadFailed = creatorImageMap.value(downloadFailedKey).toBool();
	mInAssets = creatorImageMap.value(inAssetsKey).toBool();
	mInData = creatorImageMap.value(inDataKey).toBool();
	mSuffix = creatorImageMap.value(suffixKey).toString();
	mMaxScaleFactor = creatorImageMap.value(maxScaleFactorKey).toInt();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void CreatorImage::fillFromForeignMap(const QVariantMap& creatorImageMap)
{
	mCreatorId = creatorImageMap.value(creatorIdForeignKey).toInt();
	mOriginImageUrl = creatorImageMap.value(originImageUrlForeignKey).toString();
	mDownloadSuccess = creatorImageMap.value(downloadSuccessForeignKey).toBool();
	mDownloadFailed = creatorImageMap.value(downloadFailedForeignKey).toBool();
	mInAssets = creatorImageMap.value(inAssetsForeignKey).toBool();
	mInData = creatorImageMap.value(inDataForeignKey).toBool();
	mSuffix = creatorImageMap.value(suffixForeignKey).toString();
	mMaxScaleFactor = creatorImageMap.value(maxScaleFactorForeignKey).toInt();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void CreatorImage::fillFromCacheMap(const QVariantMap& creatorImageMap)
{
	mCreatorId = creatorImageMap.value(creatorIdKey).toInt();
	mOriginImageUrl = creatorImageMap.value(originImageUrlKey).toString();
	mDownloadSuccess = creatorImageMap.value(downloadSuccessKey).toBool();
	mDownloadFailed = creatorImageMap.value(downloadFailedKey).toBool();
	mInAssets = creatorImageMap.value(inAssetsKey).toBool();
	mInData = creatorImageMap.value(inDataKey).toBool();
	mSuffix = creatorImageMap.value(suffixKey).toString();
	mMaxScaleFactor = creatorImageMap.value(maxScaleFactorKey).toInt();
}

void CreatorImage::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool CreatorImage::isValid()
{
	if (mCreatorId == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from CreatorImage as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap CreatorImage::toMap()
{
	QVariantMap creatorImageMap;
	creatorImageMap.insert(creatorIdKey, mCreatorId);
	creatorImageMap.insert(originImageUrlKey, mOriginImageUrl);
	creatorImageMap.insert(downloadSuccessKey, mDownloadSuccess);
	creatorImageMap.insert(downloadFailedKey, mDownloadFailed);
	creatorImageMap.insert(inAssetsKey, mInAssets);
	creatorImageMap.insert(inDataKey, mInData);
	creatorImageMap.insert(suffixKey, mSuffix);
	creatorImageMap.insert(maxScaleFactorKey, mMaxScaleFactor);
	return creatorImageMap;
}

/*
 * Exports Properties from CreatorImage as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap CreatorImage::toForeignMap()
{
	QVariantMap creatorImageMap;
	creatorImageMap.insert(creatorIdForeignKey, mCreatorId);
	creatorImageMap.insert(originImageUrlForeignKey, mOriginImageUrl);
	creatorImageMap.insert(downloadSuccessForeignKey, mDownloadSuccess);
	creatorImageMap.insert(downloadFailedForeignKey, mDownloadFailed);
	creatorImageMap.insert(inAssetsForeignKey, mInAssets);
	creatorImageMap.insert(inDataForeignKey, mInData);
	creatorImageMap.insert(suffixForeignKey, mSuffix);
	creatorImageMap.insert(maxScaleFactorForeignKey, mMaxScaleFactor);
	return creatorImageMap;
}


/*
 * Exports Properties from CreatorImage as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap CreatorImage::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: creatorId
// Domain KEY: creatorId
int CreatorImage::creatorId() const
{
	return mCreatorId;
}

void CreatorImage::setCreatorId(int creatorId)
{
	if (creatorId != mCreatorId) {
		mCreatorId = creatorId;
		emit creatorIdChanged(creatorId);
	}
}
// ATT 
// Optional: originImageUrl
QString CreatorImage::originImageUrl() const
{
	return mOriginImageUrl;
}

void CreatorImage::setOriginImageUrl(QString originImageUrl)
{
	if (originImageUrl != mOriginImageUrl) {
		mOriginImageUrl = originImageUrl;
		emit originImageUrlChanged(originImageUrl);
	}
}
// ATT 
// Optional: downloadSuccess
bool CreatorImage::downloadSuccess() const
{
	return mDownloadSuccess;
}

void CreatorImage::setDownloadSuccess(bool downloadSuccess)
{
	if (downloadSuccess != mDownloadSuccess) {
		mDownloadSuccess = downloadSuccess;
		emit downloadSuccessChanged(downloadSuccess);
	}
}
// ATT 
// Optional: downloadFailed
bool CreatorImage::downloadFailed() const
{
	return mDownloadFailed;
}

void CreatorImage::setDownloadFailed(bool downloadFailed)
{
	if (downloadFailed != mDownloadFailed) {
		mDownloadFailed = downloadFailed;
		emit downloadFailedChanged(downloadFailed);
	}
}
// ATT 
// Optional: inAssets
bool CreatorImage::inAssets() const
{
	return mInAssets;
}

void CreatorImage::setInAssets(bool inAssets)
{
	if (inAssets != mInAssets) {
		mInAssets = inAssets;
		emit inAssetsChanged(inAssets);
	}
}
// ATT 
// Optional: inData
bool CreatorImage::inData() const
{
	return mInData;
}

void CreatorImage::setInData(bool inData)
{
	if (inData != mInData) {
		mInData = inData;
		emit inDataChanged(inData);
	}
}
// ATT 
// Optional: suffix
QString CreatorImage::suffix() const
{
	return mSuffix;
}

void CreatorImage::setSuffix(QString suffix)
{
	if (suffix != mSuffix) {
		mSuffix = suffix;
		emit suffixChanged(suffix);
	}
}
// ATT 
// Optional: maxScaleFactor
int CreatorImage::maxScaleFactor() const
{
	return mMaxScaleFactor;
}

void CreatorImage::setMaxScaleFactor(int maxScaleFactor)
{
	if (maxScaleFactor != mMaxScaleFactor) {
		mMaxScaleFactor = maxScaleFactor;
		emit maxScaleFactorChanged(maxScaleFactor);
	}
}


CreatorImage::~CreatorImage()
{
	// place cleanUp code here
}
	
