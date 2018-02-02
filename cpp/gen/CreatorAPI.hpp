#ifndef CREATORAPI_HPP_
#define CREATORAPI_HPP_

#include <QObject>
#include <qvariant.h>




class CreatorAPI: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged FINAL)
	Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged FINAL)
	Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged FINAL)
	Q_PROPERTY(QString publicName READ publicName WRITE setPublicName NOTIFY publicNameChanged FINAL)
	Q_PROPERTY(QString bio READ bio WRITE setBio NOTIFY bioChanged FINAL)
	Q_PROPERTY(QString avatar READ avatar WRITE setAvatar NOTIFY avatarChanged FINAL)


public:
	CreatorAPI(QObject *parent = 0);


	void fillFromMap(const QVariantMap& creatorAPIMap);
	void fillFromForeignMap(const QVariantMap& creatorAPIMap);
	void fillFromCacheMap(const QVariantMap& creatorAPIMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int id() const;
	void setId(int id);
	QString firstName() const;
	void setFirstName(QString firstName);
	QString lastName() const;
	void setLastName(QString lastName);
	QString publicName() const;
	void setPublicName(QString publicName);
	QString bio() const;
	void setBio(QString bio);
	QString avatar() const;
	void setAvatar(QString avatar);



	virtual ~CreatorAPI();

	Q_SIGNALS:

	void idChanged(int id);
	void firstNameChanged(QString firstName);
	void lastNameChanged(QString lastName);
	void publicNameChanged(QString publicName);
	void bioChanged(QString bio);
	void avatarChanged(QString avatar);
	

private:

	int mId;
	QString mFirstName;
	QString mLastName;
	QString mPublicName;
	QString mBio;
	QString mAvatar;

	Q_DISABLE_COPY (CreatorAPI)
};
Q_DECLARE_METATYPE(CreatorAPI*)

#endif /* CREATORAPI_HPP_ */

