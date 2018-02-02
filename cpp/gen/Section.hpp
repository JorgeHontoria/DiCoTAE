#ifndef SECTION_HPP_
#define SECTION_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>


// forward declaration (target references to this)
class Category;
// forward declaration (target references to this)
class Key;


class Section: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int sectionId READ sectionId WRITE setSectionId NOTIFY sectionIdChanged FINAL)
	Q_PROPERTY(QString sectionName READ sectionName WRITE setSectionName NOTIFY sectionNameChanged FINAL)
	Q_PROPERTY(bool inAssets READ inAssets WRITE setInAssets NOTIFY inAssetsChanged FINAL)
	// category lazy pointing to Category* (domainKey: categoryId)
	Q_PROPERTY(int category READ category WRITE setCategory NOTIFY categoryChanged FINAL)
	Q_PROPERTY(Category* categoryAsDataObject READ categoryAsDataObject WRITE resolveCategoryAsDataObject NOTIFY categoryAsDataObjectChanged FINAL)

	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Key> keysPropertyList READ keysPropertyList NOTIFY keysPropertyListChanged)

public:
	Section(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& sectionMap);
	void fillFromForeignMap(const QVariantMap& sectionMap);
	void fillFromCacheMap(const QVariantMap& sectionMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int sectionId() const;
	void setSectionId(int sectionId);
	QString sectionName() const;
	void setSectionName(QString sectionName);
	bool inAssets() const;
	void setInAssets(bool inAssets);
	// category lazy pointing to Category* (domainKey: categoryId)
	int category() const;
	void setCategory(int category);
	Category* categoryAsDataObject() const;
	
	Q_INVOKABLE
	void resolveCategoryAsDataObject(Category* category);
	
	Q_INVOKABLE
	void removeCategory();
	
	Q_INVOKABLE
	bool hasCategory();
	
	Q_INVOKABLE
	bool isCategoryResolvedAsDataObject();
	
	Q_INVOKABLE
	void markCategoryAsInvalid();
	

	
	Q_INVOKABLE
	QVariantList keysAsQVariantList();
	
	Q_INVOKABLE
	QVariantList keysAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToKeys(Key* key);
	
	Q_INVOKABLE
	bool removeFromKeys(Key* key);

	Q_INVOKABLE
	void clearKeys();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areKeysKeysResolved();

	Q_INVOKABLE
	QStringList keysKeys();

	Q_INVOKABLE
	void resolveKeysKeys(QList<Key*> keys);
	
	Q_INVOKABLE
	int keysCount();
	
	 // access from C++ to keys
	QList<Key*> keys();
	void setKeys(QList<Key*> keys);
	// access from QML to keys
	QQmlListProperty<Key> keysPropertyList();


	virtual ~Section();

	Q_SIGNALS:

	void sectionIdChanged(int sectionId);
	void sectionNameChanged(QString sectionName);
	void inAssetsChanged(bool inAssets);
	// category lazy pointing to Category* (domainKey: categoryId)
	void categoryChanged(int category);
	void categoryAsDataObjectChanged(Category* category);
	void keysChanged(QList<Key*> keys);
	void addedToKeys(Key* key);
	void keysPropertyListChanged();
	
	

private:

	int mContentSectionId;
	QString mContentSectionName;
	bool mInAssets;
	int mCategory;
	bool mCategoryInvalid;
	Category* mCategoryAsDataObject;
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mKeysKeys;
	bool mKeysKeysResolved;
	QList<Key*> mKeys;
	// implementation for QQmlListProperty to use
	// QML functions for List of Key*
	static void appendToKeysProperty(QQmlListProperty<Key> *keysList,
		Key* key);
	static int keysPropertyCount(QQmlListProperty<Key> *keysList);
	static Key* atKeysProperty(QQmlListProperty<Key> *keysList, int pos);
	static void clearKeysProperty(QQmlListProperty<Key> *keysList);
	

	Q_DISABLE_COPY (Section)
};
Q_DECLARE_METATYPE(Section*)

#endif /* SECTION_HPP_ */

