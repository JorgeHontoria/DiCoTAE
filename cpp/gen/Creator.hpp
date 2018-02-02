#ifndef CREATOR_HPP_
#define CREATOR_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>


#include "CreatorImage.hpp"
// forward declaration (target references to this)
class Key;


class Creator: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int creatorId READ creatorId WRITE setCreatorId NOTIFY creatorIdChanged FINAL)
	Q_PROPERTY(bool isBoolean2 READ isBoolean2 WRITE setIsDeprecated NOTIFY isDeprecatedChanged FINAL)
	Q_PROPERTY(QString sortKey READ sortKey WRITE setSortKey NOTIFY sortKeyChanged FINAL)
	Q_PROPERTY(QString sortGroup READ sortGroup WRITE setSortGroup NOTIFY sortGroupChanged FINAL)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
	Q_PROPERTY(QString publicName READ publicName WRITE setPublicName NOTIFY publicNameChanged FINAL)
	Q_PROPERTY(QString bio READ bio WRITE setBio NOTIFY bioChanged FINAL)
	// creatorImage lazy pointing to CreatorImage* (domainKey: creatorId)
	Q_PROPERTY(int creatorImage READ creatorImage WRITE setCreatorImage NOTIFY creatorImageChanged FINAL)
	Q_PROPERTY(CreatorImage* creatorImageAsDataObject READ creatorImageAsDataObject WRITE resolveCreatorImageAsDataObject NOTIFY creatorImageAsDataObjectChanged FINAL)

	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Key> keysPropertyList READ keysPropertyList NOTIFY keysPropertyListChanged)

public:
	Creator(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& creatorMap);
	void fillFromForeignMap(const QVariantMap& creatorMap);
	void fillFromCacheMap(const QVariantMap& creatorMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int creatorId() const;
	void setCreatorId(int creatorId);
	bool isBoolean2() const;
	void setIsDeprecated(bool isBoolean2);
	QString sortKey() const;
	void setSortKey(QString sortKey);
	QString sortGroup() const;
	void setSortGroup(QString sortGroup);
	QString name() const;
	void setName(QString name);
	QString publicName() const;
	void setPublicName(QString publicName);
	QString bio() const;
	void setBio(QString bio);
	// creatorImage lazy pointing to CreatorImage* (domainKey: creatorId)
	int creatorImage() const;
	void setCreatorImage(int creatorImage);
	CreatorImage* creatorImageAsDataObject() const;
	
	Q_INVOKABLE
	void resolveCreatorImageAsDataObject(CreatorImage* creatorImage);
	
	Q_INVOKABLE
	void removeCreatorImage();
	
	Q_INVOKABLE
	bool hasCreatorImage();
	
	Q_INVOKABLE
	bool isCreatorImageResolvedAsDataObject();
	
	Q_INVOKABLE
	void markCreatorImageAsInvalid();
	

	
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


	virtual ~Creator();

	Q_SIGNALS:

	void creatorIdChanged(int creatorId);
	void isDeprecatedChanged(bool isBoolean2);
	void sortKeyChanged(QString sortKey);
	void sortGroupChanged(QString sortGroup);
	void nameChanged(QString name);
	void publicNameChanged(QString publicName);
	void bioChanged(QString bio);
	// creatorImage lazy pointing to CreatorImage* (domainKey: creatorId)
	void creatorImageChanged(int creatorImage);
	void creatorImageAsDataObjectChanged(CreatorImage* creatorImage);
	void keysChanged(QList<Key*> keys);
	void addedToKeys(Key* key);
	void keysPropertyListChanged();
	
	

private:

	int mCreatorId;
	bool mIsBoolean2;
	QString mSortKey;
	QString mSortGroup;
	QString mName;
	QString mPublicName;
	QString mBio;
	int mCreatorImage;
	bool mCreatorImageInvalid;
	CreatorImage* mCreatorImageAsDataObject;
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
	

	Q_DISABLE_COPY (Creator)
};
Q_DECLARE_METATYPE(Creator*)

#endif /* CREATOR_HPP_ */

