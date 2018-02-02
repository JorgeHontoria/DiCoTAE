#include "Day.hpp"
#include <QDebug>
#include <quuid.h>
// target also references to this
#include "Key.hpp"

// keys of QVariantMap used in this APP
static const QString idKey = "id";
static const QString weekDayKey = "weekDay";
static const QString repositoryDayKey = "repositoryDay";
static const QString keysKey = "keys";

// keys used from Server API etc
static const QString idForeignKey = "id";
static const QString weekDayForeignKey = "weekDay";
static const QString repositoryDayForeignKey = "repositoryDay";
static const QString keysForeignKey = "keys";

/*
 * Default Constructor if Day not initialized from QVariantMap
 */
Day::Day(QObject *parent) :
        QObject(parent), mId(-1), mWeekDay(0)
{
	// Date, Time or Timestamp ? construct null value
	mRepositoryDay = QDate();
		// lazy Arrays where only keys are persisted
		mKeysKeysResolved = false;
}

bool Day::isAllResolved()
{
    if(!areKeysKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize Day from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Day::fillFromMap(const QVariantMap& dayMap)
{
	mId = dayMap.value(idKey).toInt();
	mWeekDay = dayMap.value(weekDayKey).toInt();
	if (dayMap.contains(repositoryDayKey)) {
		// always getting the Date as a String (from server or JSON)
		QString repositoryDayAsString = dayMap.value(repositoryDayKey).toString();
		mRepositoryDay = QDate::fromString(repositoryDayAsString, "yyyy-MM-dd");
		if (!mRepositoryDay.isValid()) {
			mRepositoryDay = QDate();
			qDebug() << "mRepositoryDay is not valid for String: " << repositoryDayAsString;
		}
	}
	// mKeys is (lazy loaded) Array of Key*
	mKeysKeys = dayMap.value(keysKey).toStringList();
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
void Day::fillFromForeignMap(const QVariantMap& dayMap)
{
	mId = dayMap.value(idForeignKey).toInt();
	mWeekDay = dayMap.value(weekDayForeignKey).toInt();
	if (dayMap.contains(repositoryDayForeignKey)) {
		// always getting the Date as a String (from server or JSON)
		QString repositoryDayAsString = dayMap.value(repositoryDayForeignKey).toString();
		mRepositoryDay = QDate::fromString(repositoryDayAsString, "yyyy-MM-dd");
		if (!mRepositoryDay.isValid()) {
			mRepositoryDay = QDate();
			qDebug() << "mRepositoryDay is not valid for String: " << repositoryDayAsString;
		}
	}
	// mKeys is (lazy loaded) Array of Key*
	mKeysKeys = dayMap.value(keysForeignKey).toStringList();
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
void Day::fillFromCacheMap(const QVariantMap& dayMap)
{
	mId = dayMap.value(idKey).toInt();
	mWeekDay = dayMap.value(weekDayKey).toInt();
	if (dayMap.contains(repositoryDayKey)) {
		// always getting the Date as a String (from server or JSON)
		QString repositoryDayAsString = dayMap.value(repositoryDayKey).toString();
		mRepositoryDay = QDate::fromString(repositoryDayAsString, "yyyy-MM-dd");
		if (!mRepositoryDay.isValid()) {
			mRepositoryDay = QDate();
			qDebug() << "mRepositoryDay is not valid for String: " << repositoryDayAsString;
		}
	}
	// mKeys is (lazy loaded) Array of Key*
	mKeysKeys = dayMap.value(keysKey).toStringList();
	// mKeys must be resolved later if there are keys
	mKeysKeysResolved = (mKeysKeys.size() == 0);
	mKeys.clear();
}

void Day::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Day::isValid()
{
	if (mId == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Day as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Day::toMap()
{
	QVariantMap dayMap;
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
	dayMap.insert(keysKey, mKeysKeys);
	dayMap.insert(idKey, mId);
	dayMap.insert(weekDayKey, mWeekDay);
	if (hasRepositoryDay()) {
		dayMap.insert(repositoryDayKey, mRepositoryDay.toString("yyyy-MM-dd"));
	}
	return dayMap;
}

/*
 * Exports Properties from Day as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Day::toForeignMap()
{
	QVariantMap dayMap;
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
	dayMap.insert(keysKey, mKeysKeys);
	dayMap.insert(idForeignKey, mId);
	dayMap.insert(weekDayForeignKey, mWeekDay);
	if (hasRepositoryDay()) {
		dayMap.insert(repositoryDayForeignKey, mRepositoryDay.toString("yyyy-MM-dd"));
	}
	return dayMap;
}


/*
 * Exports Properties from Day as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Day::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: id
// Domain KEY: id
int Day::id() const
{
	return mId;
}

void Day::setId(int id)
{
	if (id != mId) {
		mId = id;
		emit idChanged(id);
	}
}
// ATT 
// Optional: weekDay
int Day::weekDay() const
{
	return mWeekDay;
}

void Day::setWeekDay(int weekDay)
{
	if (weekDay != mWeekDay) {
		mWeekDay = weekDay;
		emit weekDayChanged(weekDay);
	}
}
// ATT 
// Optional: repositoryDay
/**
 * in QML set DateTimePicker value this way:
 * myPicker.value = new Date(repositoryDay)
 */
QDate Day::repositoryDay() const
{
	return mRepositoryDay;
}

/**
 * from QML DateTime Picker use as parameter:
 * repositoryDay = new Date(myPicker.value)
 */
void Day::setRepositoryDay(QDate repositoryDay)
{
	if (repositoryDay != mRepositoryDay) {
		mRepositoryDay = repositoryDay;
		emit repositoryDayChanged(repositoryDay);
	}
}
bool Day::hasRepositoryDay()
{
	return !mRepositoryDay.isNull() && mRepositoryDay.isValid();
}
// ATT 
// Optional: keys
QVariantList Day::keysAsQVariantList()
{
	QVariantList keysList;
	for (int i = 0; i < mKeys.size(); ++i) {
        keysList.append((mKeys.at(i))->toMap());
    }
	return keysList;
}
QVariantList Day::keysAsForeignQVariantList()
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
void Day::addToKeys(Key* key)
{
    mKeys.append(key);
    emit addedToKeys(key);
    emit keysPropertyListChanged();
}

bool Day::removeFromKeys(Key* key)
{
    bool ok = false;
    ok = mKeys.removeOne(key);
    if (!ok) {
        //qDebug() << "Key* not found in keys";
    	return false;
    }
    emit keysPropertyListChanged();
    // keys are independent - DON'T delete them
    return true;
}
void Day::clearKeys()
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
bool Day::areKeysKeysResolved()
{
    return mKeysKeysResolved;
}

QStringList Day::keysKeys()
{
    return mKeysKeys;
}

/**
 * Objects from keysKeys will be added to existing keys
 * This enables to use addToKeys() without resolving before
 * Hint: it's your responsibility to resolve before looping thru keys
 */
void Day::resolveKeysKeys(QList<Key*> keys)
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

int Day::keysCount()
{
    return mKeys.size();
}
QList<Key*> Day::keys()
{
	return mKeys;
}
void Day::setKeys(QList<Key*> keys) 
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
 * day.keysPropertyList.length to get the size
 * day.keysPropertyList[2] to get Key* at position 2
 * day.keysPropertyList = [] to clear the list
 * or get easy access to properties like
 * day.keysPropertyList[2].myPropertyName
 */
QQmlListProperty<Key> Day::keysPropertyList()
{
    return QQmlListProperty<Key>(this, 0, &Day::appendToKeysProperty,
            &Day::keysPropertyCount, &Day::atKeysProperty,
            &Day::clearKeysProperty);
}
void Day::appendToKeysProperty(QQmlListProperty<Key> *keysList,
        Key* key)
{
    Day *dayObject = qobject_cast<Day *>(keysList->object);
    if (dayObject) {
        dayObject->mKeys.append(key);
        emit dayObject->addedToKeys(key);
    } else {
        qWarning() << "cannot append Key* to keys " << "Object is not of type Day*";
    }
}
int Day::keysPropertyCount(QQmlListProperty<Key> *keysList)
{
    Day *day = qobject_cast<Day *>(keysList->object);
    if (day) {
        return day->mKeys.size();
    } else {
        qWarning() << "cannot get size keys " << "Object is not of type Day*";
    }
    return 0;
}
Key* Day::atKeysProperty(QQmlListProperty<Key> *keysList, int pos)
{
    Day *day = qobject_cast<Day *>(keysList->object);
    if (day) {
        if (day->mKeys.size() > pos) {
            return day->mKeys.at(pos);
        }
        qWarning() << "cannot get Key* at pos " << pos << " size is "
                << day->mKeys.size();
    } else {
        qWarning() << "cannot get Key* at pos " << pos << "Object is not of type Day*";
    }
    return 0;
}
void Day::clearKeysProperty(QQmlListProperty<Key> *keysList)
{
    Day *day = qobject_cast<Day *>(keysList->object);
    if (day) {
        // keys are independent - DON'T delete them
        day->mKeys.clear();
    } else {
        qWarning() << "cannot clear keys " << "Object is not of type Day*";
    }
}



Day::~Day()
{
	// place cleanUp code here
}
	
