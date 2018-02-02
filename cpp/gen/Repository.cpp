#include "Repository.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString idKey = "id";
static const QString repositoryNameKey = "repositoryName";
static const QString repositoryCityKey = "repositoryCity";
static const QString addressKey = "address";
static const QString timeZoneNameKey = "timeZoneName";
static const QString timeZoneOffsetSecondsKey = "timeZoneOffsetSeconds";
static const QString repositoryFromKey = "repositoryFrom";
static const QString repositoryToKey = "repositoryTo";
static const QString hashTagKey = "hashTag";
static const QString hashContentBlockKey = "hashContentBlock";
static const QString hashContentStandardTypeKey = "hashContentStandardType";
static const QString homePageKey = "homePage";
static const QString coordinateKey = "coordinate";
static const QString lastDictionaryIdKey = "lastDictionaryId";
static const QString lastCategoryIdKey = "lastCategoryId";
static const QString lastSectionIdKey = "lastSectionId";
static const QString lastKeyTagIdKey = "lastKeyTagId";
static const QString lastKeyContentBlockIdKey = "lastKeyContentBlockId";
static const QString lastKeyContentStandardTypeIdKey = "lastKeyContentStandardTypeId";
static const QString daysKey = "updates";

// keys used from Server API etc
static const QString idForeignKey = "id";
static const QString repositoryNameForeignKey = "repositoryName";
static const QString repositoryCityForeignKey = "repositoryCity";
static const QString addressForeignKey = "address";
static const QString timeZoneNameForeignKey = "timeZoneName";
static const QString timeZoneOffsetSecondsForeignKey = "timeZoneOffsetSeconds";
static const QString repositoryFromForeignKey = "repositoryFrom";
static const QString repositoryToForeignKey = "repositoryTo";
static const QString hashTagForeignKey = "hashTag";
static const QString hashContentBlockForeignKey = "hashContentBlock";
static const QString hashContentStandardTypeForeignKey = "hashContentStandardType";
static const QString homePageForeignKey = "homePage";
static const QString coordinateForeignKey = "coordinate";
static const QString lastDictionaryIdForeignKey = "lastDictionaryId";
static const QString lastCategoryIdForeignKey = "lastCategoryId";
static const QString lastSectionIdForeignKey = "lastSectionId";
static const QString lastKeyTagIdForeignKey = "lastKeyTagId";
static const QString lastKeyContentBlockIdForeignKey = "lastKeyContentBlockId";
static const QString lastKeyContentStandardTypeIdForeignKey = "lastKeyContentStandardTypeId";
static const QString daysForeignKey = "updates";

/*
 * Default Constructor if Repository not initialized from QVariantMap
 */
Repository::Repository(QObject *parent) :
        QObject(parent), mId(-1), mRepositoryName(""), mRepositoryCity(""), mAddress(""), mTimeZoneName(""), mTimeZoneOffsetSeconds(0), mHashTag(""), mHashContentBlock(""), mHashContentStandardType(""), mHomePage(""), mCoordinate(""), mLastDictionaryId(0), mLastCategoryId(0), mLastSectionId(0), mLastKeyTagId(0), mLastKeyContentBlockId(0), mLastKeyContentStandardTypeId(0)
{
	// Date, Time or Timestamp ? construct null value
	mRepositoryFrom = QDate();
	mRepositoryTo = QDate();

    // lazy Arrays where only keys are persisted
    mDaysKeysResolved = false;
}

bool Repository::isAllResolved()
{
    if(!areDaysKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize Repository from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Repository::fillFromMap(const QVariantMap& repositoryMap)
{
	mId = repositoryMap.value(idKey).toInt();
	mRepositoryName = repositoryMap.value(repositoryNameKey).toString();
	mRepositoryCity = repositoryMap.value(repositoryCityKey).toString();
	mAddress = repositoryMap.value(addressKey).toString();
	mTimeZoneName = repositoryMap.value(timeZoneNameKey).toString();
	mTimeZoneOffsetSeconds = repositoryMap.value(timeZoneOffsetSecondsKey).toInt();
	if (repositoryMap.contains(repositoryFromKey)) {
		// always getting the Date as a String (from server or JSON)
		QString repositoryFromAsString = repositoryMap.value(repositoryFromKey).toString();
		mRepositoryFrom = QDate::fromString(repositoryFromAsString, "yyyy-MM-dd");
		if (!mRepositoryFrom.isValid()) {
			mRepositoryFrom = QDate();
			qDebug() << "mRepositoryFrom is not valid for String: " << repositoryFromAsString;
		}
	}
	if (repositoryMap.contains(repositoryToKey)) {
		// always getting the Date as a String (from server or JSON)
		QString repositoryToAsString = repositoryMap.value(repositoryToKey).toString();
		mRepositoryTo = QDate::fromString(repositoryToAsString, "yyyy-MM-dd");
		if (!mRepositoryTo.isValid()) {
			mRepositoryTo = QDate();
			qDebug() << "mRepositoryTo is not valid for String: " << repositoryToAsString;
		}
	}
	mHashTag = repositoryMap.value(hashTagKey).toString();
    mHashContentBlock = repositoryMap.value(hashContentBlockKey).toString();
    mHashContentStandardType = repositoryMap.value(hashContentStandardTypeKey).toString();
    mHomePage = repositoryMap.value(homePageKey).toString();
	mCoordinate = repositoryMap.value(coordinateKey).toString();
	mLastDictionaryId = repositoryMap.value(lastDictionaryIdKey).toInt();
	mLastCategoryId = repositoryMap.value(lastCategoryIdKey).toInt();
	mLastSectionId = repositoryMap.value(lastSectionIdKey).toInt();
	mLastKeyTagId = repositoryMap.value(lastKeyTagIdKey).toInt();
    mLastKeyContentBlockId = repositoryMap.value(lastKeyContentBlockIdKey).toInt();
    mLastKeyContentStandardTypeId = repositoryMap.value(lastKeyContentStandardTypeIdKey).toInt();

	// mDays is (lazy loaded) Array of Day*
	mDaysKeys = repositoryMap.value(daysKey).toStringList();
	// mDays must be resolved later if there are keys
	mDaysKeysResolved = (mDaysKeys.size() == 0);
	mDays.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void Repository::fillFromForeignMap(const QVariantMap& repositoryMap)
{
	mId = repositoryMap.value(idForeignKey).toInt();
	mRepositoryName = repositoryMap.value(repositoryNameForeignKey).toString();
	mRepositoryCity = repositoryMap.value(repositoryCityForeignKey).toString();
	mAddress = repositoryMap.value(addressForeignKey).toString();
	mTimeZoneName = repositoryMap.value(timeZoneNameForeignKey).toString();
	mTimeZoneOffsetSeconds = repositoryMap.value(timeZoneOffsetSecondsForeignKey).toInt();
	if (repositoryMap.contains(repositoryFromForeignKey)) {
		// always getting the Date as a String (from server or JSON)
		QString repositoryFromAsString = repositoryMap.value(repositoryFromForeignKey).toString();
		mRepositoryFrom = QDate::fromString(repositoryFromAsString, "yyyy-MM-dd");
		if (!mRepositoryFrom.isValid()) {
			mRepositoryFrom = QDate();
			qDebug() << "mRepositoryFrom is not valid for String: " << repositoryFromAsString;
		}
	}
	if (repositoryMap.contains(repositoryToForeignKey)) {
		// always getting the Date as a String (from server or JSON)
		QString repositoryToAsString = repositoryMap.value(repositoryToForeignKey).toString();
		mRepositoryTo = QDate::fromString(repositoryToAsString, "yyyy-MM-dd");
		if (!mRepositoryTo.isValid()) {
			mRepositoryTo = QDate();
			qDebug() << "mRepositoryTo is not valid for String: " << repositoryToAsString;
		}
	}
	mHashTag = repositoryMap.value(hashTagForeignKey).toString();
    mHashContentBlock = repositoryMap.value(hashContentBlockForeignKey).toString();
    mHashContentStandardType = repositoryMap.value(hashContentStandardTypeForeignKey).toString();
    mHomePage = repositoryMap.value(homePageForeignKey).toString();
	mCoordinate = repositoryMap.value(coordinateForeignKey).toString();
	mLastDictionaryId = repositoryMap.value(lastDictionaryIdForeignKey).toInt();
	mLastCategoryId = repositoryMap.value(lastCategoryIdForeignKey).toInt();
	mLastSectionId = repositoryMap.value(lastSectionIdForeignKey).toInt();
	mLastKeyTagId = repositoryMap.value(lastKeyTagIdForeignKey).toInt();
    mLastKeyContentBlockId = repositoryMap.value(lastKeyContentBlockIdForeignKey).toInt();
    mLastKeyContentStandardTypeId = repositoryMap.value(lastKeyContentStandardTypeIdForeignKey).toInt();
    // mDays is (lazy loaded) Array of Day*
	mDaysKeys = repositoryMap.value(daysForeignKey).toStringList();
	// mDays must be resolved later if there are keys
	mDaysKeysResolved = (mDaysKeys.size() == 0);
	mDays.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void Repository::fillFromCacheMap(const QVariantMap& repositoryMap)
{
	mId = repositoryMap.value(idKey).toInt();
	mRepositoryName = repositoryMap.value(repositoryNameKey).toString();
	mRepositoryCity = repositoryMap.value(repositoryCityKey).toString();
	mAddress = repositoryMap.value(addressKey).toString();
	mTimeZoneName = repositoryMap.value(timeZoneNameKey).toString();
	mTimeZoneOffsetSeconds = repositoryMap.value(timeZoneOffsetSecondsKey).toInt();
	if (repositoryMap.contains(repositoryFromKey)) {
		// always getting the Date as a String (from server or JSON)
		QString repositoryFromAsString = repositoryMap.value(repositoryFromKey).toString();
		mRepositoryFrom = QDate::fromString(repositoryFromAsString, "yyyy-MM-dd");
		if (!mRepositoryFrom.isValid()) {
			mRepositoryFrom = QDate();
			qDebug() << "mRepositoryFrom is not valid for String: " << repositoryFromAsString;
		}
	}
	if (repositoryMap.contains(repositoryToKey)) {
		// always getting the Date as a String (from server or JSON)
		QString repositoryToAsString = repositoryMap.value(repositoryToKey).toString();
		mRepositoryTo = QDate::fromString(repositoryToAsString, "yyyy-MM-dd");
		if (!mRepositoryTo.isValid()) {
			mRepositoryTo = QDate();
			qDebug() << "mRepositoryTo is not valid for String: " << repositoryToAsString;
		}
	}
	mHashTag = repositoryMap.value(hashTagKey).toString();
    mHashContentBlock = repositoryMap.value(hashContentBlockKey).toString();
    mHashContentStandardType = repositoryMap.value(hashContentStandardTypeKey).toString();
    mHomePage = repositoryMap.value(homePageKey).toString();
	mCoordinate = repositoryMap.value(coordinateKey).toString();
	mLastDictionaryId = repositoryMap.value(lastDictionaryIdKey).toInt();
	mLastCategoryId = repositoryMap.value(lastCategoryIdKey).toInt();
	mLastSectionId = repositoryMap.value(lastSectionIdKey).toInt();
	mLastKeyTagId = repositoryMap.value(lastKeyTagIdKey).toInt();
    mLastKeyContentBlockId = repositoryMap.value(lastKeyContentBlockIdKey).toInt();
    mLastKeyContentStandardTypeId = repositoryMap.value(lastKeyContentStandardTypeIdKey).toInt();
    // mDays is (lazy loaded) Array of Day*
	mDaysKeys = repositoryMap.value(daysKey).toStringList();
	// mDays must be resolved later if there are keys
	mDaysKeysResolved = (mDaysKeys.size() == 0);
	mDays.clear();
}

void Repository::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Repository::isValid()
{
	if (mId == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Repository as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Repository::toMap()
{
	QVariantMap repositoryMap;
	// mDays points to Day*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mDaysKeysResolved) {
		mDaysKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mDays.size(); ++i) {
		Day* day;
		day = mDays.at(i);
		mDaysKeys << QString::number(day->id());
	}
	repositoryMap.insert(daysKey, mDaysKeys);
	repositoryMap.insert(idKey, mId);
	repositoryMap.insert(repositoryNameKey, mRepositoryName);
	repositoryMap.insert(repositoryCityKey, mRepositoryCity);
	repositoryMap.insert(addressKey, mAddress);
	repositoryMap.insert(timeZoneNameKey, mTimeZoneName);
	repositoryMap.insert(timeZoneOffsetSecondsKey, mTimeZoneOffsetSeconds);
	if (hasRepositoryFrom()) {
		repositoryMap.insert(repositoryFromKey, mRepositoryFrom.toString("yyyy-MM-dd"));
	}
	if (hasRepositoryTo()) {
		repositoryMap.insert(repositoryToKey, mRepositoryTo.toString("yyyy-MM-dd"));
	}
	repositoryMap.insert(hashTagKey, mHashTag);
    repositoryMap.insert(hashContentBlockKey, mHashContentBlock);
    repositoryMap.insert(hashContentStandardTypeKey, mHashContentStandardType);
    repositoryMap.insert(homePageKey, mHomePage);
	repositoryMap.insert(coordinateKey, mCoordinate);
	repositoryMap.insert(lastDictionaryIdKey, mLastDictionaryId);
	repositoryMap.insert(lastCategoryIdKey, mLastCategoryId);
	repositoryMap.insert(lastSectionIdKey, mLastSectionId);
	repositoryMap.insert(lastKeyTagIdKey, mLastKeyTagId);
    repositoryMap.insert(lastKeyContentBlockIdKey, mLastKeyContentBlockId);
    repositoryMap.insert(lastKeyContentStandardTypeIdKey, mLastKeyContentStandardTypeId);
    return repositoryMap;
}

/*
 * Exports Properties from Repository as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Repository::toForeignMap()
{
	QVariantMap repositoryMap;
	// mDays points to Day*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mDaysKeysResolved) {
		mDaysKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mDays.size(); ++i) {
		Day* day;
		day = mDays.at(i);
		mDaysKeys << QString::number(day->id());
	}
	repositoryMap.insert(daysKey, mDaysKeys);
	repositoryMap.insert(idForeignKey, mId);
	repositoryMap.insert(repositoryNameForeignKey, mRepositoryName);
	repositoryMap.insert(repositoryCityForeignKey, mRepositoryCity);
	repositoryMap.insert(addressForeignKey, mAddress);
	repositoryMap.insert(timeZoneNameForeignKey, mTimeZoneName);
	repositoryMap.insert(timeZoneOffsetSecondsForeignKey, mTimeZoneOffsetSeconds);
	if (hasRepositoryFrom()) {
		repositoryMap.insert(repositoryFromForeignKey, mRepositoryFrom.toString("yyyy-MM-dd"));
	}
	if (hasRepositoryTo()) {
		repositoryMap.insert(repositoryToForeignKey, mRepositoryTo.toString("yyyy-MM-dd"));
	}
	repositoryMap.insert(hashTagForeignKey, mHashTag);
    repositoryMap.insert(hashContentBlockForeignKey, mHashContentBlock);
    repositoryMap.insert(hashContentStandardTypeForeignKey, mHashContentStandardType);
    repositoryMap.insert(homePageForeignKey, mHomePage);
	repositoryMap.insert(coordinateForeignKey, mCoordinate);
	repositoryMap.insert(lastDictionaryIdForeignKey, mLastDictionaryId);
	repositoryMap.insert(lastCategoryIdForeignKey, mLastCategoryId);
	repositoryMap.insert(lastSectionIdForeignKey, mLastSectionId);
	repositoryMap.insert(lastKeyTagIdForeignKey, mLastKeyTagId);
    repositoryMap.insert(lastKeyContentBlockIdForeignKey, mLastKeyContentBlockId);
    repositoryMap.insert(lastKeyContentStandardTypeIdForeignKey, mLastKeyContentStandardTypeId);
	return repositoryMap;
}


/*
 * Exports Properties from Repository as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Repository::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: id
// Domain KEY: id
int Repository::id() const
{
	return mId;
}

void Repository::setId(int id)
{
	if (id != mId) {
		mId = id;
		emit idChanged(id);
	}
}
// ATT 
// Optional: repositoryName
QString Repository::repositoryName() const
{
	return mRepositoryName;
}

void Repository::setRepositoryName(QString repositoryName)
{
	if (repositoryName != mRepositoryName) {
		mRepositoryName = repositoryName;
		emit repositoryNameChanged(repositoryName);
	}
}
// ATT 
// Optional: repositoryCity
QString Repository::repositoryCity() const
{
	return mRepositoryCity;
}

void Repository::setRepositoryCity(QString repositoryCity)
{
	if (repositoryCity != mRepositoryCity) {
		mRepositoryCity = repositoryCity;
		emit repositoryCityChanged(repositoryCity);
	}
}
// ATT 
// Optional: address
QString Repository::address() const
{
	return mAddress;
}

void Repository::setAddress(QString address)
{
	if (address != mAddress) {
		mAddress = address;
		emit addressChanged(address);
	}
}
// ATT 
// Optional: timeZoneName
QString Repository::timeZoneName() const
{
	return mTimeZoneName;
}

void Repository::setTimeZoneName(QString timeZoneName)
{
	if (timeZoneName != mTimeZoneName) {
		mTimeZoneName = timeZoneName;
		emit timeZoneNameChanged(timeZoneName);
	}
}
// ATT 
// Optional: timeZoneOffsetSeconds
int Repository::timeZoneOffsetSeconds() const
{
	return mTimeZoneOffsetSeconds;
}

void Repository::setTimeZoneOffsetSeconds(int timeZoneOffsetSeconds)
{
	if (timeZoneOffsetSeconds != mTimeZoneOffsetSeconds) {
		mTimeZoneOffsetSeconds = timeZoneOffsetSeconds;
		emit timeZoneOffsetSecondsChanged(timeZoneOffsetSeconds);
	}
}
// ATT 
// Optional: repositoryFrom
/**
 * in QML set DateTimePicker value this way:
 * myPicker.value = new Date(repositoryFrom)
 */
QDate Repository::repositoryFrom() const
{
	return mRepositoryFrom;
}

/**
 * from QML DateTime Picker use as parameter:
 * repositoryFrom = new Date(myPicker.value)
 */
void Repository::setRepositoryFrom(QDate repositoryFrom)
{
	if (repositoryFrom != mRepositoryFrom) {
		mRepositoryFrom = repositoryFrom;
		emit repositoryFromChanged(repositoryFrom);
	}
}
bool Repository::hasRepositoryFrom()
{
	return !mRepositoryFrom.isNull() && mRepositoryFrom.isValid();
}
// ATT 
// Optional: repositoryTo
/**
 * in QML set DateTimePicker value this way:
 * myPicker.value = new Date(repositoryTo)
 */
QDate Repository::repositoryTo() const
{
	return mRepositoryTo;
}

/**
 * from QML DateTime Picker use as parameter:
 * repositoryTo = new Date(myPicker.value)
 */
void Repository::setRepositoryTo(QDate repositoryTo)
{
	if (repositoryTo != mRepositoryTo) {
		mRepositoryTo = repositoryTo;
		emit repositoryToChanged(repositoryTo);
	}
}
bool Repository::hasRepositoryTo()
{
	return !mRepositoryTo.isNull() && mRepositoryTo.isValid();
}
// ATT 
// Optional: hashTag
QString Repository::hashTag() const
{
	return mHashTag;
}

void Repository::setHashTag(QString hashTag)
{
	if (hashTag != mHashTag) {
		mHashTag = hashTag;
		emit hashTagChanged(hashTag);
	}
}
// ATT
// Optional: hashContentBlock
QString Repository::hashContentBlock() const
{
    return mHashContentBlock;
}

void Repository::setHashContentBlock(QString hashContentBlock)
{
    if (hashContentBlock != mHashContentBlock) {
        mHashContentBlock = hashContentBlock;
        emit hashContentBlockChanged(hashContentBlock);
    }
}
// ATT
// Optional: hashContentStandardType
QString Repository::hashContentStandardType() const
{
    return mHashContentStandardType;
}

void Repository::setHashContentStandardType(QString hashContentStandardType)
{
    if (hashContentStandardType != mHashContentStandardType) {
        mHashContentStandardType = hashContentStandardType;
        emit hashContentStandardTypeChanged(hashContentStandardType);
    }
}
// ATT 
// Optional: homePage
QString Repository::homePage() const
{
	return mHomePage;
}

void Repository::setHomePage(QString homePage)
{
	if (homePage != mHomePage) {
		mHomePage = homePage;
		emit homePageChanged(homePage);
	}
}
// ATT 
// Optional: coordinate
QString Repository::coordinate() const
{
	return mCoordinate;
}

void Repository::setCoordinate(QString coordinate)
{
	if (coordinate != mCoordinate) {
		mCoordinate = coordinate;
		emit coordinateChanged(coordinate);
	}
}
// ATT 
// Optional: lastDictionaryId
int Repository::lastDictionaryId() const
{
	return mLastDictionaryId;
}

void Repository::setLastDictionaryId(int lastDictionaryId)
{
	if (lastDictionaryId != mLastDictionaryId) {
		mLastDictionaryId = lastDictionaryId;
		emit lastDictionaryIdChanged(lastDictionaryId);
	}
}
// ATT 
// Optional: lastCategoryId
int Repository::lastCategoryId() const
{
	return mLastCategoryId;
}

void Repository::setLastCategoryId(int lastCategoryId)
{
	if (lastCategoryId != mLastCategoryId) {
		mLastCategoryId = lastCategoryId;
		emit lastCategoryIdChanged(lastCategoryId);
	}
}
// ATT 
// Optional: lastSectionId
int Repository::lastSectionId() const
{
	return mLastSectionId;
}

void Repository::setLastSectionId(int lastSectionId)
{
	if (lastSectionId != mLastSectionId) {
		mLastSectionId = lastSectionId;
		emit lastSectionIdChanged(lastSectionId);
	}
}
// ATT 
// Optional: lastKeyTagId
int Repository::lastKeyTagId() const
{
	return mLastKeyTagId;
}

void Repository::setLastKeyTagId(int lastKeyTagId)
{
	if (lastKeyTagId != mLastKeyTagId) {
		mLastKeyTagId = lastKeyTagId;
		emit lastKeyTagIdChanged(lastKeyTagId);
	}
}
// ATT
// Optional: lastKeyContentBlockId
int Repository::lastKeyContentBlockId() const
{
    return mLastKeyContentBlockId;
}

void Repository::setLastKeyContentBlockId(int lastKeyContentBlockId)
{
    if (lastKeyContentBlockId != mLastKeyContentBlockId) {
        mLastKeyContentBlockId = lastKeyContentBlockId;
        emit lastKeyContentBlockIdChanged(lastKeyContentBlockId);
    }
}
// ATT
// Optional: lastKeyContentStandardTypeId
int Repository::lastKeyContentStandardTypeId() const
{
    return mLastKeyContentStandardTypeId;
}

void Repository::setLastKeyContentStandardTypeId(int lastKeyContentStandardTypeId)
{
    if (lastKeyContentStandardTypeId != mLastKeyContentStandardTypeId) {
        mLastKeyContentStandardTypeId = lastKeyContentStandardTypeId;
        emit lastKeyContentStandardTypeIdChanged(lastKeyContentStandardTypeId);
    }
}
// ATT 
// Optional: days
QVariantList Repository::daysAsQVariantList()
{
	QVariantList daysList;
	for (int i = 0; i < mDays.size(); ++i) {
        daysList.append((mDays.at(i))->toMap());
    }
	return daysList;
}
QVariantList Repository::daysAsForeignQVariantList()
{
	QVariantList daysList;
	for (int i = 0; i < mDays.size(); ++i) {
        daysList.append((mDays.at(i))->toForeignMap());
    }
	return daysList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add days without resolving existing keys before
 * attention: before looping through the objects
 * you must resolveDaysKeys
 */
void Repository::addToDays(Day* day)
{
    mDays.append(day);
    emit addedToDays(day);
    emit daysPropertyListChanged();
}

bool Repository::removeFromDays(Day* day)
{
    bool ok = false;
    ok = mDays.removeOne(day);
    if (!ok) {
    	qDebug() << "Day* not found in days";
    	return false;
    }
    emit daysPropertyListChanged();
    // days are independent - DON'T delete them
    return true;
}
void Repository::clearDays()
{
    for (int i = mDays.size(); i > 0; --i) {
        removeFromDays(mDays.last());
    }
    mDaysKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: daysKeys()
 * - resolve them from DataManager
 * - then resolveDaysKeys()
 */
bool Repository::areDaysKeysResolved()
{
    return mDaysKeysResolved;
}

QStringList Repository::daysKeys()
{
    return mDaysKeys;
}

/**
 * Objects from daysKeys will be added to existing days
 * This enables to use addToDays() without resolving before
 * Hint: it's your responsibility to resolve before looping thru days
 */
void Repository::resolveDaysKeys(QList<Day*> days)
{
    if(mDaysKeysResolved){
        return;
    }
    // don't clear mDays (see above)
    for (int i = 0; i < days.size(); ++i) {
        addToDays(days.at(i));
    }
    mDaysKeysResolved = true;
}

int Repository::daysCount()
{
    return mDays.size();
}
QList<Day*> Repository::days()
{
	return mDays;
}
void Repository::setDays(QList<Day*> days) 
{
	if (days != mDays) {
		mDays = days;
		emit daysChanged(days);
		emit daysPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * repository.daysPropertyList.length to get the size
 * repository.daysPropertyList[2] to get Day* at position 2
 * repository.daysPropertyList = [] to clear the list
 * or get easy access to properties like
 * repository.daysPropertyList[2].myPropertyName
 */
QQmlListProperty<Day> Repository::daysPropertyList()
{
    return QQmlListProperty<Day>(this, 0, &Repository::appendToDaysProperty,
            &Repository::daysPropertyCount, &Repository::atDaysProperty,
            &Repository::clearDaysProperty);
}
void Repository::appendToDaysProperty(QQmlListProperty<Day> *daysList,
        Day* day)
{
    Repository *repositoryObject = qobject_cast<Repository *>(daysList->object);
    if (repositoryObject) {
        repositoryObject->mDays.append(day);
        emit repositoryObject->addedToDays(day);
    } else {
        qWarning() << "cannot append Day* to days " << "Object is not of type Repository*";
    }
}
int Repository::daysPropertyCount(QQmlListProperty<Day> *daysList)
{
    Repository *repository = qobject_cast<Repository *>(daysList->object);
    if (repository) {
        return repository->mDays.size();
    } else {
        qWarning() << "cannot get size days " << "Object is not of type Repository*";
    }
    return 0;
}
Day* Repository::atDaysProperty(QQmlListProperty<Day> *daysList, int pos)
{
    Repository *repository = qobject_cast<Repository *>(daysList->object);
    if (repository) {
        if (repository->mDays.size() > pos) {
            return repository->mDays.at(pos);
        }
        qWarning() << "cannot get Day* at pos " << pos << " size is "
                << repository->mDays.size();
    } else {
        qWarning() << "cannot get Day* at pos " << pos << "Object is not of type Repository*";
    }
    return 0;
}
void Repository::clearDaysProperty(QQmlListProperty<Day> *daysList)
{
    Repository *repository = qobject_cast<Repository *>(daysList->object);
    if (repository) {
        // days are independent - DON'T delete them
        repository->mDays.clear();
    } else {
        qWarning() << "cannot clear days " << "Object is not of type Repository*";
    }
}



Repository::~Repository()
{
	// place cleanUp code here
}
	
