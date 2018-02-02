#ifndef DICTIONARY_HPP_
#define DICTIONARY_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>


// forward declaration (target references to this)
class Category;


class Dictionary: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int dictionaryId READ dictionaryId WRITE setDictionaryId NOTIFY dictionaryIdChanged FINAL)
	Q_PROPERTY(QString dictionaryName READ dictionaryName WRITE setDictionaryName NOTIFY dictionaryNameChanged FINAL)
	Q_PROPERTY(bool inAssets READ inAssets WRITE setInAssets NOTIFY inAssetsChanged FINAL)

	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Category> categorysPropertyList READ categorysPropertyList NOTIFY categorysPropertyListChanged)

public:
	Dictionary(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& dictionaryMap);
	void fillFromForeignMap(const QVariantMap& dictionaryMap);
	void fillFromCacheMap(const QVariantMap& dictionaryMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int dictionaryId() const;
	void setDictionaryId(int dictionaryId);
	QString dictionaryName() const;
	void setDictionaryName(QString dictionaryName);
	bool inAssets() const;
	void setInAssets(bool inAssets);

	
	Q_INVOKABLE
	QVariantList categorysAsQVariantList();
	
	Q_INVOKABLE
	QVariantList categorysAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToCategories(Category* category);
	
	Q_INVOKABLE
	bool removeFromCategories(Category* category);

	Q_INVOKABLE
	void clearCategories();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areCategoriesKeysResolved();

	Q_INVOKABLE
	QStringList categorysKeys();

	Q_INVOKABLE
	void resolveCategoriesKeys(QList<Category*> relatedCategories);
	
	Q_INVOKABLE
	int categorysCount();
	
	 // access from C++ to relatedCategories
	QList<Category*> relatedCategories();
	void setCategories(QList<Category*> relatedCategories);
	// access from QML to relatedCategories
	QQmlListProperty<Category> categorysPropertyList();


	virtual ~Dictionary();

	Q_SIGNALS:

	void dictionaryIdChanged(int dictionaryId);
	void dictionaryNameChanged(QString dictionaryName);
	void inAssetsChanged(bool inAssets);
	void categorysChanged(QList<Category*> relatedCategories);
	void addedToCategories(Category* category);
	void categorysPropertyListChanged();
	
	

private:

	int mDictionaryId;
	QString mDictionaryName;
	bool mInAssets;
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mCategoriesKeys;
	bool mCategoriesKeysResolved;
	QList<Category*> mCategories;
	// implementation for QQmlListProperty to use
	// QML functions for List of Category*
	static void appendToCategoriesProperty(QQmlListProperty<Category> *categorysList,
		Category* category);
	static int categorysPropertyCount(QQmlListProperty<Category> *categorysList);
	static Category* atCategoriesProperty(QQmlListProperty<Category> *categorysList, int pos);
	static void clearCategoriesProperty(QQmlListProperty<Category> *categorysList);
	

	Q_DISABLE_COPY (Dictionary)
};
Q_DECLARE_METATYPE(Dictionary*)

#endif /* DICTIONARY_HPP_ */

