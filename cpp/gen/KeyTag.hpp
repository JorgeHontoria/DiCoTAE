#ifndef KEYTAG_HPP_
#define KEYTAG_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>


// forward declaration (target references to this)
class Key;


class KeyTag: public QObject
{
	Q_OBJECT

    Q_PROPERTY(int tagId READ tagId WRITE setTagId NOTIFY tagIdChanged FINAL)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
	Q_PROPERTY(bool inAssets READ inAssets WRITE setInAssets NOTIFY inAssetsChanged FINAL)

	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Key> keysPropertyList READ keysPropertyList NOTIFY keysPropertyListChanged)

public:
	KeyTag(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& keyTagMap);
	void fillFromForeignMap(const QVariantMap& keyTagMap);
	void fillFromCacheMap(const QVariantMap& keyTagMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

    int tagId() const;
    void setTagId(int tagId);
	QString name() const;
	void setName(QString name);
	bool inAssets() const;
	void setInAssets(bool inAssets);

	
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


	virtual ~KeyTag();

	Q_SIGNALS:

    void tagIdChanged(int tagId);
	void nameChanged(QString name);
	void inAssetsChanged(bool inAssets);
	void keysChanged(QList<Key*> keys);
	void addedToKeys(Key* key);
	void keysPropertyListChanged();
	
	

private:

	int mTagId;
	QString mName;
	bool mInAssets;
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
	

	Q_DISABLE_COPY (KeyTag)
};
Q_DECLARE_METATYPE(KeyTag*)

#endif /* KEYTAG_HPP_ */

