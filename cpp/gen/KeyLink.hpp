#ifndef KEYLINK_HPP_
#define KEYLINK_HPP_

#include <QObject>
#include <qvariant.h>




class KeyLink: public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged FINAL)
	Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged FINAL)
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)


public:
	KeyLink(QObject *parent = 0);


	void fillFromMap(const QVariantMap& keyLinkMap);
	void fillFromForeignMap(const QVariantMap& keyLinkMap);
	void fillFromCacheMap(const QVariantMap& keyLinkMap);
	
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



	virtual ~KeyLink();

	Q_SIGNALS:

	void uuidChanged(QString uuid);
	void urlChanged(QString url);
	void titleChanged(QString title);
	

private:

	QString mUuid;
	QString mUrl;
	QString mTitle;

	Q_DISABLE_COPY (KeyLink)
};
Q_DECLARE_METATYPE(KeyLink*)

#endif /* KEYLINK_HPP_ */

