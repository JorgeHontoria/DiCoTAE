#ifndef CONFERENCE_HPP_
#define CONFERENCE_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>
#include <QDate>


#include "Day.hpp"


class Repository: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged FINAL)
	Q_PROPERTY(QString repositoryName READ repositoryName WRITE setRepositoryName NOTIFY repositoryNameChanged FINAL)
	Q_PROPERTY(QString repositoryCity READ repositoryCity WRITE setRepositoryCity NOTIFY repositoryCityChanged FINAL)
	Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged FINAL)
	Q_PROPERTY(QString timeZoneName READ timeZoneName WRITE setTimeZoneName NOTIFY timeZoneNameChanged FINAL)
	Q_PROPERTY(int timeZoneOffsetSeconds READ timeZoneOffsetSeconds WRITE setTimeZoneOffsetSeconds NOTIFY timeZoneOffsetSecondsChanged FINAL)
	Q_PROPERTY(QDate repositoryFrom READ repositoryFrom WRITE setRepositoryFrom NOTIFY repositoryFromChanged FINAL)
	Q_PROPERTY(QDate repositoryTo READ repositoryTo WRITE setRepositoryTo NOTIFY repositoryToChanged FINAL)
	Q_PROPERTY(QString hashTag READ hashTag WRITE setHashTag NOTIFY hashTagChanged FINAL)
    Q_PROPERTY(QString hashContentBlock READ hashContentBlock WRITE setHashContentBlock NOTIFY hashContentBlockChanged FINAL)
    Q_PROPERTY(QString hashContentStandardType READ hashContentStandardType WRITE setHashContentStandardType NOTIFY hashContentStandardTypeChanged FINAL)
	Q_PROPERTY(QString homePage READ homePage WRITE setHomePage NOTIFY homePageChanged FINAL)
	Q_PROPERTY(QString coordinate READ coordinate WRITE setCoordinate NOTIFY coordinateChanged FINAL)
	Q_PROPERTY(int lastDictionaryId READ lastDictionaryId WRITE setLastDictionaryId NOTIFY lastDictionaryIdChanged FINAL)
	Q_PROPERTY(int lastCategoryId READ lastCategoryId WRITE setLastCategoryId NOTIFY lastCategoryIdChanged FINAL)
	Q_PROPERTY(int lastSectionId READ lastSectionId WRITE setLastSectionId NOTIFY lastSectionIdChanged FINAL)
	Q_PROPERTY(int lastKeyTagId READ lastKeyTagId WRITE setLastKeyTagId NOTIFY lastKeyTagIdChanged FINAL)
    Q_PROPERTY(int lastKeyContentBlockId READ lastKeyContentBlockId WRITE setLastKeyContentBlockId NOTIFY lastKeyContentBlockIdChanged FINAL)
    Q_PROPERTY(int lastKeyContentStandardTypeId READ lastKeyContentStandardTypeId WRITE setLastKeyContentStandardTypeId NOTIFY lastKeyContentStandardTypeIdChanged FINAL)

	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Day> daysPropertyList READ daysPropertyList NOTIFY daysPropertyListChanged)

public:
	Repository(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& repositoryMap);
	void fillFromForeignMap(const QVariantMap& repositoryMap);
	void fillFromCacheMap(const QVariantMap& repositoryMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int id() const;
	void setId(int id);
	QString repositoryName() const;
	void setRepositoryName(QString repositoryName);
	QString repositoryCity() const;
	void setRepositoryCity(QString repositoryCity);
	QString address() const;
	void setAddress(QString address);
	QString timeZoneName() const;
	void setTimeZoneName(QString timeZoneName);
	int timeZoneOffsetSeconds() const;
	void setTimeZoneOffsetSeconds(int timeZoneOffsetSeconds);
	QDate repositoryFrom() const;

	Q_INVOKABLE
	bool hasRepositoryFrom();
	void setRepositoryFrom(QDate repositoryFrom);
	QDate repositoryTo() const;

	Q_INVOKABLE
	bool hasRepositoryTo();
	void setRepositoryTo(QDate repositoryTo);
	QString hashTag() const;
	void setHashTag(QString hashTag);
    QString hashContentBlock() const;
    void setHashContentBlock(QString hashContentBlock);
    QString hashContentStandardType() const;
    void setHashContentStandardType(QString hashContentStandardType);
	QString homePage() const;
	void setHomePage(QString homePage);
	QString coordinate() const;
	void setCoordinate(QString coordinate);
	int lastDictionaryId() const;
	void setLastDictionaryId(int lastDictionaryId);
	int lastCategoryId() const;
	void setLastCategoryId(int lastCategoryId);
	int lastSectionId() const;
	void setLastSectionId(int lastSectionId);
	int lastKeyTagId() const;
	void setLastKeyTagId(int lastKeyTagId);
    int lastKeyContentBlockId() const;
    void setLastKeyContentBlockId(int lastKeyContentBlockId);
    int lastKeyContentStandardTypeId() const;
    void setLastKeyContentStandardTypeId(int lastKeyContentStandardTypeId);
	
	Q_INVOKABLE
	QVariantList daysAsQVariantList();
	
	Q_INVOKABLE
	QVariantList daysAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToDays(Day* day);
	
	Q_INVOKABLE
	bool removeFromDays(Day* day);

	Q_INVOKABLE
	void clearDays();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areDaysKeysResolved();

	Q_INVOKABLE
	QStringList daysKeys();

	Q_INVOKABLE
	void resolveDaysKeys(QList<Day*> days);
	
	Q_INVOKABLE
	int daysCount();
	
	 // access from C++ to days
	QList<Day*> days();
	void setDays(QList<Day*> days);
	// access from QML to days
	QQmlListProperty<Day> daysPropertyList();


	virtual ~Repository();

	Q_SIGNALS:

	void idChanged(int id);
	void repositoryNameChanged(QString repositoryName);
	void repositoryCityChanged(QString repositoryCity);
	void addressChanged(QString address);
	void timeZoneNameChanged(QString timeZoneName);
	void timeZoneOffsetSecondsChanged(int timeZoneOffsetSeconds);
	void repositoryFromChanged(QDate repositoryFrom);
	void repositoryToChanged(QDate repositoryTo);
	void hashTagChanged(QString hashTag);
    void hashContentBlockChanged(QString hashContentBlock);
    void hashContentStandardTypeChanged(QString hashContentStandardType);
	void homePageChanged(QString homePage);
	void coordinateChanged(QString coordinate);
	void lastDictionaryIdChanged(int lastDictionaryId);
	void lastCategoryIdChanged(int lastCategoryId);
	void lastSectionIdChanged(int lastSectionId);
	void lastKeyTagIdChanged(int lastKeyTagId);
    void lastKeyContentBlockIdChanged(int lastKeyContentBlockId);
    void lastKeyContentStandardTypeIdChanged(int lastKeyContentStandardTypeId);
	void daysChanged(QList<Day*> days);
	void addedToDays(Day* day);
	void daysPropertyListChanged();
	
	

private:

	int mId;
	QString mRepositoryName;
	QString mRepositoryCity;
	QString mAddress;
	QString mTimeZoneName;
	int mTimeZoneOffsetSeconds;
	QDate mRepositoryFrom;
	QDate mRepositoryTo;
	QString mHashTag;
    QString mHashContentBlock;
    QString mHashContentStandardType;
    QString mHomePage;
	QString mCoordinate;
	int mLastDictionaryId;
	int mLastCategoryId;
	int mLastSectionId;
	int mLastKeyTagId;
    int mLastKeyContentBlockId;
    int mLastKeyContentStandardTypeId;
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mDaysKeys;
	bool mDaysKeysResolved;
	QList<Day*> mDays;
	// implementation for QQmlListProperty to use
	// QML functions for List of Day*
	static void appendToDaysProperty(QQmlListProperty<Day> *daysList,
		Day* day);
	static int daysPropertyCount(QQmlListProperty<Day> *daysList);
	static Day* atDaysProperty(QQmlListProperty<Day> *daysList, int pos);
	static void clearDaysProperty(QQmlListProperty<Day> *daysList);
	
	Q_DISABLE_COPY (Repository)
};
Q_DECLARE_METATYPE(Repository*)

#endif /* CONFERENCE_HPP_ */

