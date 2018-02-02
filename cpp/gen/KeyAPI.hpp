#ifndef KEYAPI_HPP_
#define KEYAPI_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>
#include <QTime>


#include "PersonsAPI.hpp"
#include "KeyLinkAPI.hpp"


class KeyAPI: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int keyId READ keyId WRITE setKeyId NOTIFY keyIdChanged FINAL)
    Q_PROPERTY(int keyIdRefView READ keyIdRefView WRITE setKeyIdRefView NOTIFY keyIdRefViewChanged FINAL)
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
	Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY subtitleChanged FINAL)
	Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged FINAL)
	Q_PROPERTY(QString keyType READ keyType WRITE setKeyType NOTIFY keyTypeChanged FINAL)
    Q_PROPERTY(QString contentStandardName READ contentStandardName WRITE setContentStandardName NOTIFY contentStandardNameChanged FINAL)
    Q_PROPERTY(QString contentArticulo1 READ contentArticulo1 WRITE setContentArticulo1 NOTIFY contentArticulo1Changed FINAL)
    Q_PROPERTY(QString contentSituacion1 READ contentSituacion1 WRITE setContentSituacion1 NOTIFY contentSituacion1Changed FINAL)
    Q_PROPERTY(QTime startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged FINAL)
	Q_PROPERTY(QString start_time READ start_time WRITE setDuration NOTIFY start_timeChanged FINAL)
    Q_PROPERTY(QString contentLink READ contentLink WRITE setContentLink NOTIFY contentLinkChanged FINAL)
    Q_PROPERTY(QString section READ section WRITE setSection NOTIFY sectionChanged FINAL)

	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<PersonsAPI> creatorPropertyList READ creatorPropertyList NOTIFY creatorPropertyListChanged)
	Q_PROPERTY(QQmlListProperty<KeyLinkAPI> keyLinksPropertyList READ keyLinksPropertyList NOTIFY keyLinksPropertyListChanged)

	Q_PROPERTY(QStringList keyTagsStringList READ keyTagsStringList  WRITE setKeyTagsStringList NOTIFY keyTagsStringListChanged FINAL)
    Q_PROPERTY(QStringList keyContentBlocksStringList READ keyContentBlocksStringList  WRITE setKeyContentBlocksStringList NOTIFY keyContentBlocksStringListChanged FINAL)
    Q_PROPERTY(QStringList keyContentStandardTypesStringList READ keyContentStandardTypesStringList  WRITE setKeyContentStandardTypesStringList NOTIFY keyContentStandardTypesStringListChanged FINAL)

public:
	KeyAPI(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& keyAPIMap);
	void fillFromForeignMap(const QVariantMap& keyAPIMap);
	void fillFromCacheMap(const QVariantMap& keyAPIMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int keyId() const;
	void setKeyId(int keyId);
    int keyIdRefView() const;
    void setKeyIdRefView(int keyIdRefView);
	QString title() const;
	void setTitle(QString title);
	QString subtitle() const;
	void setSubtitle(QString subtitle);
	QString description() const;
	void setDescription(QString description);
	QString keyType() const;
	void setKeyType(QString keyType);
    QString contentStandardName() const;
    void setContentStandardName(QString contentStandardName);
    QString contentArticulo1() const;
    void setContentArticulo1(QString contentArticulo1);
    QString contentSituacion1() const;
    void setContentSituacion1(QString contentSituacion1);

    QTime startTime() const;

	Q_INVOKABLE
	bool hasStartTime();

	Q_INVOKABLE
	void setStartTimeFromPickerValue(QString startTimeValue);
	void setStartTime(QTime startTime);
	QString start_time() const;
	void setDuration(QString start_time);
    QString contentLink() const;
    void setContentLink(QString contentLink);
	QString section() const;
    void setSection(QString section);

	
	Q_INVOKABLE
	void addToKeyTagsStringList(const QString& stringValue);
	
	Q_INVOKABLE
	bool removeFromKeyTagsStringList(const QString& stringValue);
	
	Q_INVOKABLE
	int keyTagsCount();
	
	QStringList keyTagsStringList();
	void setKeyTagsStringList(const QStringList& keyTags);
	


    Q_INVOKABLE
    void addToKeyContentBlocksStringList(const QString& stringValue);

    Q_INVOKABLE
    bool removeFromKeyContentBlocksStringList(const QString& stringValue);

    Q_INVOKABLE
    int keyContentBlocksCount();

    QStringList keyContentBlocksStringList();
    void setKeyContentBlocksStringList(const QStringList& keyContentBlocks);


    Q_INVOKABLE
    void addToKeyContentStandardTypesStringList(const QString& stringValue);

    Q_INVOKABLE
    bool removeFromKeyContentStandardTypesStringList(const QString& stringValue);

    Q_INVOKABLE
    int keyContentStandardTypesCount();

    QStringList keyContentStandardTypesStringList();
    void setKeyContentStandardTypesStringList(const QStringList& keyContentStandardTypes);


	Q_INVOKABLE
	QVariantList creatorAsQVariantList();
	
	Q_INVOKABLE
	QVariantList creatorAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToCreator(PersonsAPI* creatorsAPI);
	
	Q_INVOKABLE
	bool removeFromCreator(PersonsAPI* creatorsAPI);

	Q_INVOKABLE
	void clearCreator();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areCreatorKeysResolved();

	Q_INVOKABLE
	QStringList creatorKeys();

	Q_INVOKABLE
	void resolveCreatorKeys(QList<PersonsAPI*> creator);
	
	Q_INVOKABLE
	int creatorCount();
	
	 // access from C++ to creator
	QList<PersonsAPI*> creator();
	void setCreator(QList<PersonsAPI*> creator);
	// access from QML to creator
	QQmlListProperty<PersonsAPI> creatorPropertyList();
	
	Q_INVOKABLE
	QVariantList keyLinksAsQVariantList();
	
	Q_INVOKABLE
	QVariantList keyLinksAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToKeyLinks(KeyLinkAPI* keyLinkAPI);
	
	Q_INVOKABLE
	bool removeFromKeyLinks(KeyLinkAPI* keyLinkAPI);

	Q_INVOKABLE
	void clearKeyLinks();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areKeyLinksKeysResolved();

	Q_INVOKABLE
	QStringList keyLinksKeys();

	Q_INVOKABLE
	void resolveKeyLinksKeys(QList<KeyLinkAPI*> keyLinks);
	
	Q_INVOKABLE
	int keyLinksCount();
	
	 // access from C++ to keyLinks
	QList<KeyLinkAPI*> keyLinks();
	void setKeyLinks(QList<KeyLinkAPI*> keyLinks);
	// access from QML to keyLinks
	QQmlListProperty<KeyLinkAPI> keyLinksPropertyList();


	virtual ~KeyAPI();

	Q_SIGNALS:

	void keyIdChanged(int keyId);
    void keyIdRefViewChanged(int keyIdRefView);

	void titleChanged(QString title);
	void subtitleChanged(QString subtitle);
	void descriptionChanged(QString description);
	void keyTypeChanged(QString keyType);   
    void contentStandardNameChanged(QString contentStandardName);
    void contentArticulo1Changed(QString contentArticulo1);
    void contentSituacion1Changed(QString contentSituacion1);
	void startTimeChanged(QTime startTime);
	void start_timeChanged(QString start_time);

    void contentLinkChanged(QString contentLink);

	void sectionChanged(QString section);

	void keyTagsStringListChanged(QStringList keyTags);
	void addedToKeyTagsStringList(QString stringValue);
	void removedFromKeyTagsStringList(QString stringValue);

    void keyContentBlocksStringListChanged(QStringList keyContentBlocks);
    void addedToKeyContentBlocksStringList(QString stringValue);
    void removedFromKeyContentBlocksStringList(QString stringValue);

    void keyContentStandardTypesStringListChanged(QStringList keyContentStandardTypes);
    void addedToKeyContentStandardTypesStringList(QString stringValue);
    void removedFromKeyContentStandardTypesStringList(QString stringValue);

	void creatorChanged(QList<PersonsAPI*> creator);
	void addedToCreator(PersonsAPI* creatorsAPI);
	void creatorPropertyListChanged();

	void keyLinksChanged(QList<KeyLinkAPI*> keyLinks);
	void addedToKeyLinks(KeyLinkAPI* keyLinkAPI);
	void keyLinksPropertyListChanged();

private:

	int mKeyId;
    int mKeyIdRefView;

	QString mTitle;
	QString mSubtitle;
	QString mDescription;
	QString mKeyType;
    QString mContentStandardName;
    QString mContentArticulo1;
    QString mContentSituacion1;
    QTime mStartTime;
	QString mDuration;
	QString mContentLink;
    QString mContentSection;
	QStringList mKeyTagsStringList;
    QStringList mKeyContentBlocksStringList;
    QStringList mKeyContentStandardTypesStringList;

	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mCreatorKeys;
	bool mCreatorKeysResolved;
	QList<PersonsAPI*> mCreator;
	// implementation for QQmlListProperty to use
	// QML functions for List of PersonsAPI*
	static void appendToCreatorProperty(QQmlListProperty<PersonsAPI> *creatorList,
		PersonsAPI* creatorsAPI);
	static int creatorPropertyCount(QQmlListProperty<PersonsAPI> *creatorList);
	static PersonsAPI* atCreatorProperty(QQmlListProperty<PersonsAPI> *creatorList, int pos);
	static void clearCreatorProperty(QQmlListProperty<PersonsAPI> *creatorList);
	
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mKeyLinksKeys;
	bool mKeyLinksKeysResolved;
	QList<KeyLinkAPI*> mKeyLinks;
	// implementation for QQmlListProperty to use
	// QML functions for List of KeyLinkAPI*
	static void appendToKeyLinksProperty(QQmlListProperty<KeyLinkAPI> *keyLinksList,
		KeyLinkAPI* keyLinkAPI);
	static int keyLinksPropertyCount(QQmlListProperty<KeyLinkAPI> *keyLinksList);
	static KeyLinkAPI* atKeyLinksProperty(QQmlListProperty<KeyLinkAPI> *keyLinksList, int pos);
	static void clearKeyLinksProperty(QQmlListProperty<KeyLinkAPI> *keyLinksList);
	

	Q_DISABLE_COPY (KeyAPI)
};
Q_DECLARE_METATYPE(KeyAPI*)

#endif /* KEYAPI_HPP_ */

