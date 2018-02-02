#include "Dictionary.hpp"
#include <QDebug>
#include <quuid.h>
// target also references to this
#include "Category.hpp"

// keys of QVariantMap used in this APP
static const QString dictionaryIdKey = "dictionaryId";
static const QString dictionaryNameKey = "dictionaryName";
static const QString inAssetsKey = "inAssets";
static const QString categorysKey = "relatedCategories";

// keys used from Server API etc
static const QString dictionaryIdForeignKey = "dictionaryId";
static const QString dictionaryNameForeignKey = "dictionaryName";
static const QString inAssetsForeignKey = "inAssets";
static const QString categorysForeignKey = "relatedCategories";

/*
 * Default Constructor if Dictionary not initialized from QVariantMap
 */
Dictionary::Dictionary(QObject *parent) :
        QObject(parent), mDictionaryId(-1), mDictionaryName(""), mInAssets(false)
{
		// lazy Arrays where only keys are persisted
        mCategoriesKeysResolved = false;
}

bool Dictionary::isAllResolved()
{
    if(!areCategoriesKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize Dictionary from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Dictionary::fillFromMap(const QVariantMap& dictionaryMap)
{
    mDictionaryId = dictionaryMap.value(dictionaryIdKey).toInt();
    mDictionaryName = dictionaryMap.value(dictionaryNameKey).toString();
    mInAssets = dictionaryMap.value(inAssetsKey).toBool();
    // mCategories is (lazy loaded) Array of Category*
    mCategoriesKeys = dictionaryMap.value(categorysKey).toStringList();
	// mCategories must be resolved later if there are keys
    mCategoriesKeysResolved = (mCategoriesKeys.size() == 0);
    mCategories.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void Dictionary::fillFromForeignMap(const QVariantMap& dictionaryMap)
{
    mDictionaryId = dictionaryMap.value(dictionaryIdForeignKey).toInt();
    mDictionaryName = dictionaryMap.value(dictionaryNameForeignKey).toString();
    mInAssets = dictionaryMap.value(inAssetsForeignKey).toBool();
    // mCategories is (lazy loaded) Array of Category*
    mCategoriesKeys = dictionaryMap.value(categorysForeignKey).toStringList();
	// mCategories must be resolved later if there are keys
    mCategoriesKeysResolved = (mCategoriesKeys.size() == 0);
    mCategories.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void Dictionary::fillFromCacheMap(const QVariantMap& dictionaryMap)
{
    mDictionaryId = dictionaryMap.value(dictionaryIdKey).toInt();
    mDictionaryName = dictionaryMap.value(dictionaryNameKey).toString();
    mInAssets = dictionaryMap.value(inAssetsKey).toBool();
    // mCategories is (lazy loaded) Array of Category*
    mCategoriesKeys = dictionaryMap.value(categorysKey).toStringList();
	// mCategories must be resolved later if there are keys
    mCategoriesKeysResolved = (mCategoriesKeys.size() == 0);
    mCategories.clear();
}

void Dictionary::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Dictionary::isValid()
{
    if (mDictionaryId == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Dictionary as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Dictionary::toMap()
{
    QVariantMap dictionaryMap;
    // mCategories points to Category*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mCategoriesKeysResolved) {
        mCategoriesKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mCategories.size(); ++i) {
        Category* category;
		category = mCategories.at(i);
        mCategoriesKeys << QString::number(category->categoryId());
	}
    dictionaryMap.insert(categorysKey, mCategoriesKeys);
    dictionaryMap.insert(dictionaryIdKey, mDictionaryId);
    dictionaryMap.insert(dictionaryNameKey, mDictionaryName);
    dictionaryMap.insert(inAssetsKey, mInAssets);
    return dictionaryMap;
}

/*
 * Exports Properties from Dictionary as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Dictionary::toForeignMap()
{
    QVariantMap dictionaryMap;
    // mCategories points to Category*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mCategoriesKeysResolved) {
        mCategoriesKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mCategories.size(); ++i) {
        Category* category;
		category = mCategories.at(i);
        mCategoriesKeys << QString::number(category->categoryId());
	}
    dictionaryMap.insert(categorysKey, mCategoriesKeys);
    dictionaryMap.insert(dictionaryIdForeignKey, mDictionaryId);
    dictionaryMap.insert(dictionaryNameForeignKey, mDictionaryName);
    dictionaryMap.insert(inAssetsForeignKey, mInAssets);
    return dictionaryMap;
}


/*
 * Exports Properties from Dictionary as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Dictionary::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: dictionaryId
// Domain KEY: dictionaryId
int Dictionary::dictionaryId() const
{
    return mDictionaryId;
}

void Dictionary::setDictionaryId(int dictionaryId)
{
    if (dictionaryId != mDictionaryId) {
        mDictionaryId = dictionaryId;
        emit dictionaryIdChanged(dictionaryId);
	}
}
// ATT 
// Optional: dictionaryName
QString Dictionary::dictionaryName() const
{
    return mDictionaryName;
}

void Dictionary::setDictionaryName(QString dictionaryName)
{
    if (dictionaryName != mDictionaryName) {
        mDictionaryName = dictionaryName;
        emit dictionaryNameChanged(dictionaryName);
	}
}
// ATT 
// Optional: inAssets
bool Dictionary::inAssets() const
{
	return mInAssets;
}

void Dictionary::setInAssets(bool inAssets)
{
	if (inAssets != mInAssets) {
		mInAssets = inAssets;
		emit inAssetsChanged(inAssets);
	}
}
// ATT 
// Optional: relatedCategories
QVariantList Dictionary::categorysAsQVariantList()
{
	QVariantList categorysList;
	for (int i = 0; i < mCategories.size(); ++i) {
        categorysList.append((mCategories.at(i))->toMap());
    }
	return categorysList;
}
QVariantList Dictionary::categorysAsForeignQVariantList()
{
	QVariantList categorysList;
	for (int i = 0; i < mCategories.size(); ++i) {
        categorysList.append((mCategories.at(i))->toForeignMap());
    }
	return categorysList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add relatedCategories without resolving existing keys before
 * attention: before looping through the objects
 * you must resolveCategoriesKeys
 */
void Dictionary::addToCategories(Category* category)
{
    mCategories.append(category);
    emit addedToCategories(category);
    emit categorysPropertyListChanged();
}

bool Dictionary::removeFromCategories(Category* category)
{
    bool ok = false;
    ok = mCategories.removeOne(category);
    if (!ok) {
        qDebug() << "Category* not found in relatedCategories";
    	return false;
    }
    emit categorysPropertyListChanged();
    // relatedCategories are independent - DON'T delete them
    return true;
}
void Dictionary::clearCategories()
{
    for (int i = mCategories.size(); i > 0; --i) {
        removeFromCategories(mCategories.last());
    }
    mCategoriesKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: categorysKeys()
 * - resolve them from DataManager
 * - then resolveCategoriesKeys()
 */
bool Dictionary::areCategoriesKeysResolved()
{
    return mCategoriesKeysResolved;
}

QStringList Dictionary::categorysKeys()
{
    return mCategoriesKeys;
}

/**
 * Objects from categorysKeys will be added to existing relatedCategories
 * This enables to use addToCategories() without resolving before
 * Hint: it's your responsibility to resolve before looping thru relatedCategories
 */
void Dictionary::resolveCategoriesKeys(QList<Category*> relatedCategories)
{
    if(mCategoriesKeysResolved){
        return;
    }
    // don't clear mCategories (see above)
    for (int i = 0; i < relatedCategories.size(); ++i) {
        addToCategories(relatedCategories.at(i));
    }
    mCategoriesKeysResolved = true;
}

int Dictionary::categorysCount()
{
    return mCategories.size();
}
QList<Category*> Dictionary::relatedCategories()
{
	return mCategories;
}
void Dictionary::setCategories(QList<Category*> relatedCategories)
{
    if (relatedCategories != mCategories) {
        mCategories = relatedCategories;
        emit categorysChanged(relatedCategories);
		emit categorysPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * dictionary.categorysPropertyList.length to get the size
 * dictionary.categorysPropertyList[2] to get Category* at position 2
 * dictionary.categorysPropertyList = [] to clear the list
 * or get easy access to properties like
 * dictionary.categorysPropertyList[2].myPropertyName
 */
QQmlListProperty<Category> Dictionary::categorysPropertyList()
{
    return QQmlListProperty<Category>(this, 0, &Dictionary::appendToCategoriesProperty,
            &Dictionary::categorysPropertyCount, &Dictionary::atCategoriesProperty,
            &Dictionary::clearCategoriesProperty);
}
void Dictionary::appendToCategoriesProperty(QQmlListProperty<Category> *categorysList,
        Category* category)
{
    Dictionary *dictionaryObject = qobject_cast<Dictionary *>(categorysList->object);
    if (dictionaryObject) {
        dictionaryObject->mCategories.append(category);
        emit dictionaryObject->addedToCategories(category);
    } else {
        qWarning() << "cannot append Category* to relatedCategories " << "Object is not of type Dictionary*";
    }
}
int Dictionary::categorysPropertyCount(QQmlListProperty<Category> *categorysList)
{
    Dictionary *dictionary = qobject_cast<Dictionary *>(categorysList->object);
    if (dictionary) {
        return dictionary->mCategories.size();
    } else {
        qWarning() << "cannot get size relatedCategories " << "Object is not of type Dictionary*";
    }
    return 0;
}
Category* Dictionary::atCategoriesProperty(QQmlListProperty<Category> *categorysList, int pos)
{
    Dictionary *dictionary = qobject_cast<Dictionary *>(categorysList->object);
    if (dictionary) {
        if (dictionary->mCategories.size() > pos) {
            return dictionary->mCategories.at(pos);
        }
        qWarning() << "cannot get Category* at pos " << pos << " size is "
                << dictionary->mCategories.size();
    } else {
        qWarning() << "cannot get Category* at pos " << pos << "Object is not of type Dictionary*";
    }
    return 0;
}
void Dictionary::clearCategoriesProperty(QQmlListProperty<Category> *categorysList)
{
    Dictionary *dictionary = qobject_cast<Dictionary *>(categorysList->object);
    if (dictionary) {
        // relatedCategories are independent - DON'T delete them
        dictionary->mCategories.clear();
    } else {
        qWarning() << "cannot clear relatedCategories " << "Object is not of type Dictionary*";
    }
}



Dictionary::~Dictionary()
{
	// place cleanUp code here
}
	
