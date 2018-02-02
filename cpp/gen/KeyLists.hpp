#ifndef KEYLISTS_HPP_
#define KEYLISTS_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>


#include "Key.hpp"
#include "Key.hpp"
#include "Key.hpp"


class KeyLists: public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged FINAL)

	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Key> dictionarydKeysPropertyList READ dictionarydKeysPropertyList NOTIFY dictionarydKeysPropertyListChanged)
	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Key> sameTimeKeysPropertyList READ sameTimeKeysPropertyList NOTIFY sameTimeKeysPropertyListChanged)
	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Key> specialTimeKeysPropertyList READ specialTimeKeysPropertyList NOTIFY specialTimeKeysPropertyListChanged)

public:
	KeyLists(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& keyListsMap);
	void fillFromForeignMap(const QVariantMap& keyListsMap);
	void fillFromCacheMap(const QVariantMap& keyListsMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	QString uuid() const;
	void setUuid(QString uuid);

	
	Q_INVOKABLE
	QVariantList dictionarydKeysAsQVariantList();
	
	Q_INVOKABLE
	QVariantList dictionarydKeysAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToDictionarydKeys(Key* key);
	
	Q_INVOKABLE
	bool removeFromDictionarydKeys(Key* key);

	Q_INVOKABLE
	void clearDictionarydKeys();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areDictionarydKeysKeysResolved();

	Q_INVOKABLE
	QStringList dictionarydKeysKeys();

	Q_INVOKABLE
	void resolveDictionarydKeysKeys(QList<Key*> dictionarydKeys);
	
	Q_INVOKABLE
	int dictionarydKeysCount();
	
	 // access from C++ to dictionarydKeys
	QList<Key*> dictionarydKeys();
	void setDictionarydKeys(QList<Key*> dictionarydKeys);
	// access from QML to dictionarydKeys
	QQmlListProperty<Key> dictionarydKeysPropertyList();
	
	Q_INVOKABLE
	QVariantList sameTimeKeysAsQVariantList();
	
	Q_INVOKABLE
	QVariantList sameTimeKeysAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToSameTimeKeys(Key* key);
	
	Q_INVOKABLE
	bool removeFromSameTimeKeys(Key* key);

	Q_INVOKABLE
	void clearSameTimeKeys();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areSameTimeKeysKeysResolved();

	Q_INVOKABLE
	QStringList sameTimeKeysKeys();

	Q_INVOKABLE
	void resolveSameTimeKeysKeys(QList<Key*> sameTimeKeys);
	
	Q_INVOKABLE
	int sameTimeKeysCount();
	
	 // access from C++ to sameTimeKeys
	QList<Key*> sameTimeKeys();
	void setSameTimeKeys(QList<Key*> sameTimeKeys);
	// access from QML to sameTimeKeys
	QQmlListProperty<Key> sameTimeKeysPropertyList();
	
	Q_INVOKABLE
	QVariantList specialTimeKeysAsQVariantList();
	
	Q_INVOKABLE
	QVariantList specialTimeKeysAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToSpecialTimeKeys(Key* key);
	
	Q_INVOKABLE
	bool removeFromSpecialTimeKeys(Key* key);

	Q_INVOKABLE
	void clearSpecialTimeKeys();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areSpecialTimeKeysKeysResolved();

	Q_INVOKABLE
	QStringList specialTimeKeysKeys();

	Q_INVOKABLE
	void resolveSpecialTimeKeysKeys(QList<Key*> specialTimeKeys);
	
	Q_INVOKABLE
	int specialTimeKeysCount();
	
	 // access from C++ to specialTimeKeys
	QList<Key*> specialTimeKeys();
	void setSpecialTimeKeys(QList<Key*> specialTimeKeys);
	// access from QML to specialTimeKeys
	QQmlListProperty<Key> specialTimeKeysPropertyList();


	virtual ~KeyLists();

	Q_SIGNALS:

	void uuidChanged(QString uuid);
	void dictionarydKeysChanged(QList<Key*> dictionarydKeys);
	void addedToDictionarydKeys(Key* key);
	void dictionarydKeysPropertyListChanged();
	
	void sameTimeKeysChanged(QList<Key*> sameTimeKeys);
	void addedToSameTimeKeys(Key* key);
	void sameTimeKeysPropertyListChanged();
	
	void specialTimeKeysChanged(QList<Key*> specialTimeKeys);
	void addedToSpecialTimeKeys(Key* key);
	void specialTimeKeysPropertyListChanged();
	
	

private:

	QString mUuid;
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mDictionarydKeysKeys;
	bool mDictionarydKeysKeysResolved;
	QList<Key*> mDictionarydKeys;
	// implementation for QQmlListProperty to use
	// QML functions for List of Key*
	static void appendToDictionarydKeysProperty(QQmlListProperty<Key> *dictionarydKeysList,
		Key* key);
	static int dictionarydKeysPropertyCount(QQmlListProperty<Key> *dictionarydKeysList);
	static Key* atDictionarydKeysProperty(QQmlListProperty<Key> *dictionarydKeysList, int pos);
	static void clearDictionarydKeysProperty(QQmlListProperty<Key> *dictionarydKeysList);
	
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mSameTimeKeysKeys;
	bool mSameTimeKeysKeysResolved;
	QList<Key*> mSameTimeKeys;
	// implementation for QQmlListProperty to use
	// QML functions for List of Key*
	static void appendToSameTimeKeysProperty(QQmlListProperty<Key> *sameTimeKeysList,
		Key* key);
	static int sameTimeKeysPropertyCount(QQmlListProperty<Key> *sameTimeKeysList);
	static Key* atSameTimeKeysProperty(QQmlListProperty<Key> *sameTimeKeysList, int pos);
	static void clearSameTimeKeysProperty(QQmlListProperty<Key> *sameTimeKeysList);
	
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mSpecialTimeKeysKeys;
	bool mSpecialTimeKeysKeysResolved;
	QList<Key*> mSpecialTimeKeys;
	// implementation for QQmlListProperty to use
	// QML functions for List of Key*
	static void appendToSpecialTimeKeysProperty(QQmlListProperty<Key> *specialTimeKeysList,
		Key* key);
	static int specialTimeKeysPropertyCount(QQmlListProperty<Key> *specialTimeKeysList);
	static Key* atSpecialTimeKeysProperty(QQmlListProperty<Key> *specialTimeKeysList, int pos);
	static void clearSpecialTimeKeysProperty(QQmlListProperty<Key> *specialTimeKeysList);
	

	Q_DISABLE_COPY (KeyLists)
};
Q_DECLARE_METATYPE(KeyLists*)

#endif /* KEYLISTS_HPP_ */

