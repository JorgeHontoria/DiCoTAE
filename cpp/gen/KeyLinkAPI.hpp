#ifndef KEYLINKAPI_HPP_
#define KEYLINKAPI_HPP_

#include <QObject>
#include <qvariant.h>




class KeyLinkAPI: public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged FINAL)
	Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged FINAL)
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)


public:
	KeyLinkAPI(QObject *parent = 0);


	void fillFromMap(const QVariantMap& keyLinkAPIMap);
	void fillFromForeignMap(const QVariantMap& keyLinkAPIMap);
	void fillFromCacheMap(const QVariantMap& keyLinkAPIMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	QString uuid() const;
	void setUuid(QString uuid);
	QString url() const;
	void setUrl(QString url);
	QString title() const;
	void setTitle(QString title);



	virtual ~KeyLinkAPI();

	Q_SIGNALS:

	void uuidChanged(QString uuid);
	void urlChanged(QString url);
	void titleChanged(QString title);
	

private:

	QString mUuid;
	QString mUrl;
	QString mTitle;

	Q_DISABLE_COPY (KeyLinkAPI)
};
Q_DECLARE_METATYPE(KeyLinkAPI*)

#endif /* KEYLINKAPI_HPP_ */

