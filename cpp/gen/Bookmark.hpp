#ifndef BOOKMARK_HPP_
#define BOOKMARK_HPP_

#include <QObject>
#include <qvariant.h>


#include "Key.hpp"


class Bookmark: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int keyId READ keyId WRITE setKeyId NOTIFY keyIdChanged FINAL)
	// key lazy pointing to Key* (domainKey: keyId)
	Q_PROPERTY(int key READ key WRITE setKey NOTIFY keyChanged FINAL)
	Q_PROPERTY(Key* keyAsDataObject READ keyAsDataObject WRITE resolveKeyAsDataObject NOTIFY keyAsDataObjectChanged FINAL)


public:
	Bookmark(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& bookmarkMap);
	void fillFromForeignMap(const QVariantMap& bookmarkMap);
	void fillFromCacheMap(const QVariantMap& bookmarkMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int keyId() const;
	void setKeyId(int keyId);
	// key lazy pointing to Key* (domainKey: keyId)
	int key() const;
	void setKey(int key);
	Key* keyAsDataObject() const;
	
	Q_INVOKABLE
	void resolveKeyAsDataObject(Key* key);
	
	Q_INVOKABLE
	void removeKey();
	
	Q_INVOKABLE
	bool hasKey();
	
	Q_INVOKABLE
	bool isKeyResolvedAsDataObject();
	
	Q_INVOKABLE
	void markKeyAsInvalid();
	



	virtual ~Bookmark();

	Q_SIGNALS:

	void keyIdChanged(int keyId);
	// key lazy pointing to Key* (domainKey: keyId)
	void keyChanged(int key);
	void keyAsDataObjectChanged(Key* key);
	

private:

	int mKeyId;
	int mKey;
	bool mKeyInvalid;
	Key* mKeyAsDataObject;

	Q_DISABLE_COPY (Bookmark)
};
Q_DECLARE_METATYPE(Bookmark*)

#endif /* BOOKMARK_HPP_ */

