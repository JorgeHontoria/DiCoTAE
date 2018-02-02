#ifndef DICTIONARYITEM_HPP_
#define DICTIONARYITEM_HPP_

#include <QObject>
#include <qvariant.h>


// forward declaration (target references to this)
class Key;


class DictionaryItem: public QObject
{
	Q_OBJECT

    Q_PROPERTY(int keyId READ keyId WRITE setKeyId NOTIFY keyIdChanged FINAL)
	Q_PROPERTY(bool isBreak READ isBreak WRITE setIsBreak NOTIFY isBreakChanged FINAL)
	Q_PROPERTY(bool isLunch READ isLunch WRITE setIsLunch NOTIFY isLunchChanged FINAL)
	Q_PROPERTY(bool isEvent READ isEvent WRITE setIsEvent NOTIFY isEventChanged FINAL)
	Q_PROPERTY(bool isRegistration READ isRegistration WRITE setIsRegistration NOTIFY isRegistrationChanged FINAL)
    // key lazy pointing to Key* (domainKey: keyId)
    Q_PROPERTY(int key READ key WRITE setKey NOTIFY keyChanged FINAL)
    Q_PROPERTY(Key* keyAsDataObject READ keyAsDataObject WRITE resolveKeyAsDataObject NOTIFY keyAsDataObjectChanged FINAL)


public:
    DictionaryItem(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

    void fillFromMap(const QVariantMap& dictionaryItemMap);
    void fillFromForeignMap(const QVariantMap& dictionaryItemMap);
    void fillFromCacheMap(const QVariantMap& dictionaryItemMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

    int keyId() const;
    void setKeyId(int keyId);
	bool isBreak() const;
	void setIsBreak(bool isBreak);
	bool isLunch() const;
	void setIsLunch(bool isLunch);
	bool isEvent() const;
	void setIsEvent(bool isEvent);
	bool isRegistration() const;
	void setIsRegistration(bool isRegistration);
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
	



	virtual ~DictionaryItem();

	Q_SIGNALS:

    void keyIdChanged(int keyId);
	void isBreakChanged(bool isBreak);
	void isLunchChanged(bool isLunch);
	void isEventChanged(bool isEvent);
	void isRegistrationChanged(bool isRegistration);
    // key lazy pointing to Key* (domainKey: keyId)
    void keyChanged(int key);
    void keyAsDataObjectChanged(Key* key);
	

private:

	int mKeyId;
	bool mIsBreak;
	bool mIsLunch;
	bool mIsEvent;
	bool mIsRegistration;
	int mKey;
	bool mKeyInvalid;
	Key* mKeyAsDataObject;

	Q_DISABLE_COPY (DictionaryItem)
};
Q_DECLARE_METATYPE(DictionaryItem*)

#endif /* DICTIONARYITEM_HPP_ */

