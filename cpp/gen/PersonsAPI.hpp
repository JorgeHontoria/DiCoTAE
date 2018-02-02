#ifndef PERSONSAPI_HPP_
#define PERSONSAPI_HPP_

#include <QObject>
#include <qvariant.h>




class PersonsAPI: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int creatorId READ creatorId WRITE setCreatorId NOTIFY creatorIdChanged FINAL)


public:
	PersonsAPI(QObject *parent = 0);


	void fillFromMap(const QVariantMap& creatorsAPIMap);
	void fillFromForeignMap(const QVariantMap& creatorsAPIMap);
	void fillFromCacheMap(const QVariantMap& creatorsAPIMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int creatorId() const;
	void setCreatorId(int creatorId);



	virtual ~PersonsAPI();

	Q_SIGNALS:

	void creatorIdChanged(int creatorId);
	

private:

	int mCreatorId;

	Q_DISABLE_COPY (PersonsAPI)
};
Q_DECLARE_METATYPE(PersonsAPI*)

#endif /* PERSONSAPI_HPP_ */

