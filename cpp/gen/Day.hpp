#ifndef DAY_HPP_
#define DAY_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>
#include <QDate>


// forward declaration (target references to this)
class Key;


class Day: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged FINAL)
	Q_PROPERTY(int weekDay READ weekDay WRITE setWeekDay NOTIFY weekDayChanged FINAL)
	Q_PROPERTY(QDate repositoryDay READ repositoryDay WRITE setRepositoryDay NOTIFY repositoryDayChanged FINAL)

	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Key> keysPropertyList READ keysPropertyList NOTIFY keysPropertyListChanged)

public:
	Day(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& dayMap);
	void fillFromForeignMap(const QVariantMap& dayMap);
	void fillFromCacheMap(const QVariantMap& dayMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int id() const;
	void setId(int id);
	int weekDay() const;
	void setWeekDay(int weekDay);
	QDate repositoryDay() const;

	Q_INVOKABLE
	bool hasRepositoryDay();
	void setRepositoryDay(QDate repositoryDay);

	
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

	virtual ~Day();

	Q_SIGNALS:

	void idChanged(int id);
	void weekDayChanged(int weekDay);
	void repositoryDayChanged(QDate repositoryDay);
	void keysChanged(QList<Key*> keys);
	void addedToKeys(Key* key);
	void keysPropertyListChanged();
	

private:

	int mId;
	int mWeekDay;
	QDate mRepositoryDay;
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
	

	Q_DISABLE_COPY (Day)
};
Q_DECLARE_METATYPE(Day*)

#endif /* DAY_HPP_ */

