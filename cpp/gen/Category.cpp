#include "Category.hpp"
#include <QDebug>
#include <quuid.h>
// target also references to this
#include "Dictionary.hpp"
// target also references to this
#include "Section.hpp"

// keys of QVariantMap used in this APP
static const QString categoryIdKey = "categoryId";
static const QString categoryNameKey = "categoryName";
static const QString inAssetsKey = "inAssets";
static const QString dictionaryKey = "dictionary";
static const QString sectionsKey = "sections";

// keys used from Server API etc
static const QString categoryIdForeignKey = "categoryId";
static const QString categoryNameForeignKey = "categoryName";
static const QString inAssetsForeignKey = "inAssets";
static const QString dictionaryForeignKey = "dictionary";
static const QString sectionsForeignKey = "sections";

/*
 * Default Constructor if Category not initialized from QVariantMap
 */
Category::Category(QObject *parent) :
        QObject(parent), mCategoryId(-1), mCategoryName(""), mInAssets(false)
{
	// lazy references:
    mDictionary = -1;
    mDictionaryAsDataObject = 0;
    mDictionaryInvalid = false;
		// lazy Arrays where only keys are persisted
        mContentSectionsKeysResolved = false;
}

bool Category::isAllResolved()
{
    if (hasDictionary() && !isDictionaryResolvedAsDataObject()) {
		return false;
	}
    if(!areSectionsKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize Category from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Category::fillFromMap(const QVariantMap& categoryMap)
{
    mCategoryId = categoryMap.value(categoryIdKey).toInt();
    mCategoryName = categoryMap.value(categoryNameKey).toString();
    mInAssets = categoryMap.value(inAssetsKey).toBool();
    // dictionary lazy pointing to Dictionary* (domainKey: dictionaryId)
    if (categoryMap.contains(dictionaryKey)) {
        mDictionary = categoryMap.value(dictionaryKey).toInt();
        if (mDictionary != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
    // mContentSections is (lazy loaded) Array of Section*
    mContentSectionsKeys = categoryMap.value(sectionsKey).toStringList();
    // mContentSections must be resolved later if there are keys
    mContentSectionsKeysResolved = (mContentSectionsKeys.size() == 0);
    mContentSections.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void Category::fillFromForeignMap(const QVariantMap& categoryMap)
{
    mCategoryId = categoryMap.value(categoryIdForeignKey).toInt();
    mCategoryName = categoryMap.value(categoryNameForeignKey).toString();
    mInAssets = categoryMap.value(inAssetsForeignKey).toBool();
    // dictionary lazy pointing to Dictionary* (domainKey: dictionaryId)
    if (categoryMap.contains(dictionaryForeignKey)) {
        mDictionary = categoryMap.value(dictionaryForeignKey).toInt();
        if (mDictionary != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
    // mContentSections is (lazy loaded) Array of Section*
    mContentSectionsKeys = categoryMap.value(sectionsForeignKey).toStringList();
    // mContentSections must be resolved later if there are keys
    mContentSectionsKeysResolved = (mContentSectionsKeys.size() == 0);
    mContentSections.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void Category::fillFromCacheMap(const QVariantMap& categoryMap)
{
    mCategoryId = categoryMap.value(categoryIdKey).toInt();
    mCategoryName = categoryMap.value(categoryNameKey).toString();
    mInAssets = categoryMap.value(inAssetsKey).toBool();
    // dictionary lazy pointing to Dictionary* (domainKey: dictionaryId)
    if (categoryMap.contains(dictionaryKey)) {
        mDictionary = categoryMap.value(dictionaryKey).toInt();
        if (mDictionary != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
    // mContentSections is (lazy loaded) Array of Section*
    mContentSectionsKeys = categoryMap.value(sectionsKey).toStringList();
    // mContentSections must be resolved later if there are keys
    mContentSectionsKeysResolved = (mContentSectionsKeys.size() == 0);
    mContentSections.clear();
}

void Category::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Category::isValid()
{
    if (mCategoryId == -1) {
		return false;
	}
    // dictionary lazy pointing to Dictionary* (domainKey: dictionaryId)
    if (mDictionary == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Category as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Category::toMap()
{
    QVariantMap categoryMap;
    // dictionary lazy pointing to Dictionary* (domainKey: dictionaryId)
    if (mDictionary != -1) {
        categoryMap.insert(dictionaryKey, mDictionary);
	}
    // mContentSections points to Section*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
    if(mContentSectionsKeysResolved) {
        mContentSectionsKeys.clear();
	}
	// add objects from mPositions
    for (int i = 0; i < mContentSections.size(); ++i) {
		Section* section;
        section = mContentSections.at(i);
        mContentSectionsKeys << QString::number(section->sectionId());
	}
    categoryMap.insert(sectionsKey, mContentSectionsKeys);
    categoryMap.insert(categoryIdKey, mCategoryId);
    categoryMap.insert(categoryNameKey, mCategoryName);
    categoryMap.insert(inAssetsKey, mInAssets);
    return categoryMap;
}

/*
 * Exports Properties from Category as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Category::toForeignMap()
{
    QVariantMap categoryMap;
    // dictionary lazy pointing to Dictionary* (domainKey: dictionaryId)
    if (mDictionary != -1) {
        categoryMap.insert(dictionaryForeignKey, mDictionary);
	}
    // mContentSections points to Section*
	// lazy array: persist only keys
	//
	// if keys already resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
    if(mContentSectionsKeysResolved) {
        mContentSectionsKeys.clear();
	}
	// add objects from mPositions
    for (int i = 0; i < mContentSections.size(); ++i) {
		Section* section;
        section = mContentSections.at(i);
        mContentSectionsKeys << QString::number(section->sectionId());
	}
    categoryMap.insert(sectionsKey, mContentSectionsKeys);
    categoryMap.insert(categoryIdForeignKey, mCategoryId);
    categoryMap.insert(categoryNameForeignKey, mCategoryName);
    categoryMap.insert(inAssetsForeignKey, mInAssets);
    return categoryMap;
}


/*
 * Exports Properties from Category as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Category::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// REF
// Lazy: dictionary
// Mandatory: dictionary
// dictionary lazy pointing to Dictionary* (domainKey: dictionaryId)
int Category::dictionary() const
{
    return mDictionary;
}
Dictionary* Category::dictionaryAsDataObject() const
{
    return mDictionaryAsDataObject;
}
void Category::setDictionary(int dictionary)
{
    if (dictionary != mDictionary) {
        // remove old Data Object if one was resolved
        if (mDictionaryAsDataObject) {
            // reset pointer, don't delete the independent object !
            mDictionaryAsDataObject = 0;
        }
        // set the new lazy reference
        mDictionary = dictionary;
        mDictionaryInvalid = false;
        emit dictionaryChanged(dictionary);
        if (dictionary != -1) {
            // resolve the corresponding Data Object on demand from DataManager
        }
    }
}
void Category::removeDictionary()
{
    if (mDictionary != -1) {
        setDictionary(-1);
	}
}
bool Category::hasDictionary()
{
    if (!mDictionaryInvalid && mDictionary != -1) {
        return true;
    } else {
        return false;
    }
}
bool Category::isDictionaryResolvedAsDataObject()
{
    if (!mDictionaryInvalid && mDictionaryAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite dictionaryId if different
void Category::resolveDictionaryAsDataObject(Dictionary* dictionary)
{
    if (dictionary) {
        if (dictionary->dictionaryId() != mDictionary) {
            setDictionary(dictionary->dictionaryId());
        }
        mDictionaryAsDataObject = dictionary;
        mDictionaryInvalid = false;
    }
}
void Category::markDictionaryAsInvalid()
{
    mDictionaryInvalid = true;
}
// ATT 
// Mandatory: categoryId
// Domain KEY: categoryId
int Category::categoryId() const
{
    return mCategoryId;
}

void Category::setCategoryId(int categoryId)
{
    if (categoryId != mCategoryId) {
        mCategoryId = categoryId;
        emit categoryIdChanged(categoryId);
	}
}
// ATT 
// Optional: categoryName
QString Category::categoryName() const
{
    return mCategoryName;
}

void Category::setCategoryName(QString categoryName)
{
    if (categoryName != mCategoryName) {
        mCategoryName = categoryName;
        emit categoryNameChanged(categoryName);
	}
}
// ATT 
// Optional: inAssets
bool Category::inAssets() const
{
	return mInAssets;
}

void Category::setInAssets(bool inAssets)
{
	if (inAssets != mInAssets) {
		mInAssets = inAssets;
		emit inAssetsChanged(inAssets);
	}
}
// ATT 
// Optional: sections
QVariantList Category::sectionsAsQVariantList()
{
	QVariantList sectionsList;
    for (int i = 0; i < mContentSections.size(); ++i) {
        sectionsList.append((mContentSections.at(i))->toMap());
    }
	return sectionsList;
}
QVariantList Category::sectionsAsForeignQVariantList()
{
	QVariantList sectionsList;
    for (int i = 0; i < mContentSections.size(); ++i) {
        sectionsList.append((mContentSections.at(i))->toForeignMap());
    }
	return sectionsList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add sections without resolving existing keys before
 * attention: before looping through the objects
 * you must resolveSectionsKeys
 */
void Category::addToSections(Section* section)
{
    mContentSections.append(section);
    emit addedToSections(section);
    emit sectionsPropertyListChanged();
}

bool Category::removeFromContentSections(Section* section)
{
    bool ok = false;
    ok = mContentSections.removeOne(section);
    if (!ok) {
    	qDebug() << "Section* not found in sections";
    	return false;
    }
    emit sectionsPropertyListChanged();
    // sections are independent - DON'T delete them
    return true;
}
void Category::clearSections()
{
    for (int i = mContentSections.size(); i > 0; --i) {
        removeFromContentSections(mContentSections.last());
    }
    mContentSectionsKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: sectionsKeys()
 * - resolve them from DataManager
 * - then resolveSectionsKeys()
 */
bool Category::areSectionsKeysResolved()
{
    return mContentSectionsKeysResolved;
}

QStringList Category::sectionsKeys()
{
    return mContentSectionsKeys;
}

/**
 * Objects from sectionsKeys will be added to existing sections
 * This enables to use addToSections() without resolving before
 * Hint: it's your responsibility to resolve before looping thru sections
 */
void Category::resolveSectionsKeys(QList<Section*> sections)
{
    if(mContentSectionsKeysResolved){
        return;
    }
    // don't clear mContentSections (see above)
    for (int i = 0; i < sections.size(); ++i) {
        addToSections(sections.at(i));
    }
    mContentSectionsKeysResolved = true;
}

int Category::sectionsCount()
{
    return mContentSections.size();
}
QList<Section*> Category::sections()
{
    return mContentSections;
}
void Category::setSections(QList<Section*> sections) 
{
    if (sections != mContentSections) {
        mContentSections = sections;
		emit sectionsChanged(sections);
		emit sectionsPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * category.sectionsPropertyList.length to get the size
 * category.sectionsPropertyList[2] to get Section* at position 2
 * category.sectionsPropertyList = [] to clear the list
 * or get easy access to properties like
 * category.sectionsPropertyList[2].myPropertyName
 */
QQmlListProperty<Section> Category::sectionsPropertyList()
{
    return QQmlListProperty<Section>(this, 0, &Category::appendToSectionsProperty,
            &Category::sectionsPropertyCount, &Category::atSectionsProperty,
            &Category::clearSectionsProperty);
}
void Category::appendToSectionsProperty(QQmlListProperty<Section> *sectionsList,
        Section* section)
{
    Category *categoryObject = qobject_cast<Category *>(sectionsList->object);
    if (categoryObject) {
        categoryObject->mContentSections.append(section);
        emit categoryObject->addedToSections(section);
    } else {
        qWarning() << "cannot append Section* to sections " << "Object is not of type Category*";
    }
}
int Category::sectionsPropertyCount(QQmlListProperty<Section> *sectionsList)
{
    Category *category = qobject_cast<Category *>(sectionsList->object);
    if (category) {
        return category->mContentSections.size();
    } else {
        qWarning() << "cannot get size sections " << "Object is not of type Category*";
    }
    return 0;
}
Section* Category::atSectionsProperty(QQmlListProperty<Section> *sectionsList, int pos)
{
    Category *category = qobject_cast<Category *>(sectionsList->object);
    if (category) {
        if (category->mContentSections.size() > pos) {
            return category->mContentSections.at(pos);
        }
        qWarning() << "cannot get Section* at pos " << pos << " size is "
                << category->mContentSections.size();
    } else {
        qWarning() << "cannot get Section* at pos " << pos << "Object is not of type Category*";
    }
    return 0;
}
void Category::clearSectionsProperty(QQmlListProperty<Section> *sectionsList)
{
    Category *category = qobject_cast<Category *>(sectionsList->object);
    if (category) {
        // sections are independent - DON'T delete them
        category->mContentSections.clear();
    } else {
        qWarning() << "cannot clear sections " << "Object is not of type Category*";
    }
}



Category::~Category()
{
	// place cleanUp code here
}
	
