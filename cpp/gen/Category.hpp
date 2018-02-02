#ifndef CATEGORY_HPP_
#define CATEGORY_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>


// forward declaration (target references to this)
class Dictionary;
// forward declaration (target references to this)
class Section;


class Category: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int categoryId READ categoryId WRITE setCategoryId NOTIFY categoryIdChanged FINAL)
	Q_PROPERTY(QString categoryName READ categoryName WRITE setCategoryName NOTIFY categoryNameChanged FINAL)
	Q_PROPERTY(bool inAssets READ inAssets WRITE setInAssets NOTIFY inAssetsChanged FINAL)
	// dictionary lazy pointing to Dictionary* (domainKey: dictionaryId)
	Q_PROPERTY(int dictionary READ dictionary WRITE setDictionary NOTIFY dictionaryChanged FINAL)
	Q_PROPERTY(Dictionary* dictionaryAsDataObject READ dictionaryAsDataObject WRITE resolveDictionaryAsDataObject NOTIFY dictionaryAsDataObjectChanged FINAL)

	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Section> sectionsPropertyList READ sectionsPropertyList NOTIFY sectionsPropertyListChanged)

public:
	Category(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& categoryMap);
	void fillFromForeignMap(const QVariantMap& categoryMap);
	void fillFromCacheMap(const QVariantMap& categoryMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int categoryId() const;
	void setCategoryId(int categoryId);
	QString categoryName() const;
	void setCategoryName(QString categoryName);
	bool inAssets() const;
	void setInAssets(bool inAssets);
	// dictionary lazy pointing to Dictionary* (domainKey: dictionaryId)
	int dictionary() const;
	void setDictionary(int dictionary);
	Dictionary* dictionaryAsDataObject() const;
	
	Q_INVOKABLE
	void resolveDictionaryAsDataObject(Dictionary* dictionary);
	
	Q_INVOKABLE
	void removeDictionary();
	
	Q_INVOKABLE
	bool hasDictionary();
	
	Q_INVOKABLE
	bool isDictionaryResolvedAsDataObject();
	
	Q_INVOKABLE
	void markDictionaryAsInvalid();
	

	
	Q_INVOKABLE
	QVariantList sectionsAsQVariantList();
	
	Q_INVOKABLE
	QVariantList sectionsAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToSections(Section* section);
	
	Q_INVOKABLE
	bool removeFromContentSections(Section* section);

	Q_INVOKABLE
	void clearSections();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areSectionsKeysResolved();

	Q_INVOKABLE
	QStringList sectionsKeys();

	Q_INVOKABLE
	void resolveSectionsKeys(QList<Section*> sections);
	
	Q_INVOKABLE
	int sectionsCount();
	
	 // access from C++ to sections
	QList<Section*> sections();
	void setSections(QList<Section*> sections);
	// access from QML to sections
	QQmlListProperty<Section> sectionsPropertyList();


	virtual ~Category();

	Q_SIGNALS:

	void categoryIdChanged(int categoryId);
	void categoryNameChanged(QString categoryName);
	void inAssetsChanged(bool inAssets);
	// dictionary lazy pointing to Dictionary* (domainKey: dictionaryId)
	void dictionaryChanged(int dictionary);
	void dictionaryAsDataObjectChanged(Dictionary* dictionary);
	void sectionsChanged(QList<Section*> sections);
	void addedToSections(Section* section);
	void sectionsPropertyListChanged();
	
	

private:

	int mCategoryId;
	QString mCategoryName;
	bool mInAssets;
	int mDictionary;
	bool mDictionaryInvalid;
	Dictionary* mDictionaryAsDataObject;
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mContentSectionsKeys;
	bool mContentSectionsKeysResolved;
	QList<Section*> mContentSections;
	// implementation for QQmlListProperty to use
	// QML functions for List of Section*
	static void appendToSectionsProperty(QQmlListProperty<Section> *sectionsList,
		Section* section);
	static int sectionsPropertyCount(QQmlListProperty<Section> *sectionsList);
	static Section* atSectionsProperty(QQmlListProperty<Section> *sectionsList, int pos);
	static void clearSectionsProperty(QQmlListProperty<Section> *sectionsList);
	

	Q_DISABLE_COPY (Category)
};
Q_DECLARE_METATYPE(Category*)

#endif /* CATEGORY_HPP_ */

