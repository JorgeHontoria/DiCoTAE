#include "Section.hpp"
#include <QDebug>
#include <quuid.h>
// target also references to this
#include "Category.hpp"
// target also references to this
#include "Key.hpp"

// keys of QVariantMap used in this APP
static const QString sectionIdKey = "sectionId";
static const QString sectionNameKey = "sectionName";
static const QString inAssetsKey = "inAssets";
static const QString categoryKey = "category";
static const QString keysKey = "keys";

// keys used from Server API etc
static const QString sectionIdForeignKey = "sectionId";
static const QString sectionNameForeignKey = "sectionName";
static const QString inAssetsForeignKey = "inAssets";
static const QString categoryForeignKey = "category";
static const QString keysForeignKey = "keys";

/*
 * Default Constructor if Section not initialized from QVariantMap
 */
Section::Section(QObject *parent) :
        QObject(parent), mContentSectionId(-1), mContentSectionName(""), mInAssets(false)
{
	// lazy references:
	mCategory = -1;
	mCategoryAsDataObject = 0;
	mCategoryInvalid = false;
		// lazy Arrays where only keys are persisted
		mKeysKeysResolved = false;
}

bool Section::isAllResolved()
{
	if (hasCategory() && !isCategoryResolvedAsDataObject()) {
		return false;
	}
    if(!areKeysKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize Section from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Section::fillFromMap(const QVariantMap& sectionMap)
{
    mContentSectionId = sectionMap.value(sectionIdKey).toInt();
    mContentSectionName = sectionMap.value(sectionNameKey).toString();
    mInAssets = sectionMap.value(inAssetsKey).toBool();
	// category lazy pointing to Category* (domainKey: categoryId)
    if (sectionMap.contains(categoryKey)) {
        mCategory = sectionMap.value(categoryKey).toInt();
		if (mCategory != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mKeys is (lazy loaded) Array of Key*
    mKeysKeys = sectionMap.value(keysKey).toStringList();
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
void Section::fillFromForeignMap(const QVariantMap& sectionMap)
{
    mContentSectionId = sectionMap.value(sectionIdForeignKey).toInt();
    mContentSectionName = sectionMap.value(sectionNameForeignKey).toString();
    mInAssets = sectionMap.value(inAssetsForeignKey).toBool();
	// category lazy pointing to Category* (domainKey: categoryId)
    if (sectionMap.contains(categoryForeignKey)) {
        mCategory = sectionMap.value(categoryForeignKey).toInt();
		if (mCategory != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mKeys is (lazy loaded) Array of Key*
    mKeysKeys = sectionMap.value(keysForeignKey).toStringList();
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
void Section::fillFromCacheMap(const QVariantMap& sectionMap)
{
    mContentSectionId = sectionMap.value(sectionIdKey).toInt();
    mContentSectionName = sectionMap.value(sectionNameKey).toString();
    mInAssets = sectionMap.value(inAssetsKey).toBool();
	// category lazy pointing to Category* (domainKey: categoryId)
    if (sectionMap.contains(categoryKey)) {
        mCategory = sectionMap.value(categoryKey).toInt();
		if (mCategory != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mKeys is (lazy loaded) Array of Key*
    mKeysKeys = sectionMap.value(keysKey).toStringList();
	// mKeys must be resolved later if there are keys
	mKeysKeysResolved = (mKeysKeys.size() == 0);
	mKeys.clear();
}

void Section::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Section::isValid()
{
	if (mContentSectionId == -1) {
		return false;
	}
	// category lazy pointing to Category* (domainKey: categoryId)
	if (mCategory == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Section as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Section::toMap()
{
    QVariantMap sectionMap;
	// category lazy pointing to Category* (domainKey: categoryId)
	if (mCategory != -1) {
        sectionMap.insert(categoryKey, mCategory);
	}
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
    sectionMap.insert(keysKey, mKeysKeys);
    sectionMap.insert(sectionIdKey, mContentSectionId);
    sectionMap.insert(sectionNameKey, mContentSectionName);
    sectionMap.insert(inAssetsKey, mInAssets);
    return sectionMap;
}

/*
 * Exports Properties from Section as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Section::toForeignMap()
{
    QVariantMap sectionMap;
	// category lazy pointing to Category* (domainKey: categoryId)
	if (mCategory != -1) {
        sectionMap.insert(categoryForeignKey, mCategory);
	}
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
    sectionMap.insert(keysKey, mKeysKeys);
    sectionMap.insert(sectionIdForeignKey, mContentSectionId);
    sectionMap.insert(sectionNameForeignKey, mContentSectionName);
    sectionMap.insert(inAssetsForeignKey, mInAssets);
    return sectionMap;
}


/*
 * Exports Properties from Section as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Section::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// REF
// Lazy: category
// Mandatory: category
// category lazy pointing to Category* (domainKey: categoryId)
int Section::category() const
{
	return mCategory;
}
Category* Section::categoryAsDataObject() const
{
	return mCategoryAsDataObject;
}
void Section::setCategory(int category)
{
	if (category != mCategory) {
        // remove old Data Object if one was resolved
        if (mCategoryAsDataObject) {
            // reset pointer, don't delete the independent object !
            mCategoryAsDataObject = 0;
        }
        // set the new lazy reference
        mCategory = category;
        mCategoryInvalid = false;
        emit categoryChanged(category);
        if (category != -1) {
            // resolve the corresponding Data Object on demand from DataManager
        }
    }
}
void Section::removeCategory()
{
	if (mCategory != -1) {
		setCategory(-1);
	}
}
bool Section::hasCategory()
{
    if (!mCategoryInvalid && mCategory != -1) {
        return true;
    } else {
        return false;
    }
}
bool Section::isCategoryResolvedAsDataObject()
{
    if (!mCategoryInvalid && mCategoryAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite categoryId if different
void Section::resolveCategoryAsDataObject(Category* category)
{
    if (category) {
        if (category->categoryId() != mCategory) {
            setCategory(category->categoryId());
        }
        mCategoryAsDataObject = category;
        mCategoryInvalid = false;
    }
}
void Section::markCategoryAsInvalid()
{
    mCategoryInvalid = true;
}
// ATT 
// Mandatory: sectionId
// Domain KEY: sectionId
int Section::sectionId() const
{
	return mContentSectionId;
}

void Section::setSectionId(int sectionId)
{
    if (sectionId != mContentSectionId) {
        mContentSectionId = sectionId;
        emit sectionIdChanged(sectionId);
	}
}
// ATT 
// Optional: sectionName
QString Section::sectionName() const
{
	return mContentSectionName;
}

void Section::setSectionName(QString sectionName)
{
    if (sectionName != mContentSectionName) {
        mContentSectionName = sectionName;
        emit sectionNameChanged(sectionName);
	}
}
// ATT 
// Optional: inAssets
bool Section::inAssets() const
{
	return mInAssets;
}

void Section::setInAssets(bool inAssets)
{
	if (inAssets != mInAssets) {
		mInAssets = inAssets;
		emit inAssetsChanged(inAssets);
	}
}
// ATT 
// Optional: keys
QVariantList Section::keysAsQVariantList()
{
	QVariantList keysList;
	for (int i = 0; i < mKeys.size(); ++i) {
        keysList.append((mKeys.at(i))->toMap());
    }
	return keysList;
}
QVariantList Section::keysAsForeignQVariantList()
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
void Section::addToKeys(Key* key)
{
    mKeys.append(key);
    emit addedToKeys(key);
    emit keysPropertyListChanged();
}

bool Section::removeFromKeys(Key* key)
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
void Section::clearKeys()
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
bool Section::areKeysKeysResolved()
{
    return mKeysKeysResolved;
}

QStringList Section::keysKeys()
{
    return mKeysKeys;
}

/**
 * Objects from keysKeys will be added to existing keys
 * This enables to use addToKeys() without resolving before
 * Hint: it's your responsibility to resolve before looping thru keys
 */
void Section::resolveKeysKeys(QList<Key*> keys)
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

int Section::keysCount()
{
    return mKeys.size();
}
QList<Key*> Section::keys()
{
	return mKeys;
}
void Section::setKeys(QList<Key*> keys)
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
 * section.keysPropertyList.length to get the size
 * section.keysPropertyList[2] to get Key* at position 2
 * section.keysPropertyList = [] to clear the list
 * or get easy access to properties like
 * section.keysPropertyList[2].myPropertyName
 */
QQmlListProperty<Key> Section::keysPropertyList()
{
    return QQmlListProperty<Key>(this, 0, &Section::appendToKeysProperty,
            &Section::keysPropertyCount, &Section::atKeysProperty,
            &Section::clearKeysProperty);
}
void Section::appendToKeysProperty(QQmlListProperty<Key> *keysList,
        Key* key)
{
    Section *sectionObject = qobject_cast<Section *>(keysList->object);
    if (sectionObject) {
        sectionObject->mKeys.append(key);
        emit sectionObject->addedToKeys(key);
    } else {
        qWarning() << "cannot append Key* to keys " << "Object is not of type Section*";
    }
}
int Section::keysPropertyCount(QQmlListProperty<Key> *keysList)
{
    Section *section = qobject_cast<Section *>(keysList->object);
    if (section) {
        return section->mKeys.size();
    } else {
        qWarning() << "cannot get size keys " << "Object is not of type Section*";
    }
    return 0;
}
Key* Section::atKeysProperty(QQmlListProperty<Key> *keysList, int pos)
{
    Section *section = qobject_cast<Section *>(keysList->object);
    if (section) {
        if (section->mKeys.size() > pos) {
            return section->mKeys.at(pos);
        }
        qWarning() << "cannot get Key* at pos " << pos << " size is "
                << section->mKeys.size();
    } else {
        qWarning() << "cannot get Key* at pos " << pos << "Object is not of type Section*";
    }
    return 0;
}
void Section::clearKeysProperty(QQmlListProperty<Key> *keysList)
{
    Section *section = qobject_cast<Section *>(keysList->object);
    if (section) {
        // keys are independent - DON'T delete them
        section->mKeys.clear();
    } else {
        qWarning() << "cannot clear keys " << "Object is not of type Section*";
    }
}



Section::~Section()
{
	// place cleanUp code here
}
	
