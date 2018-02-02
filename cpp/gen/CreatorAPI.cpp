#include "CreatorAPI.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString idKey = "id";
static const QString firstNameKey = "firstName";
static const QString lastNameKey = "lastName";
static const QString publicNameKey = "publicName";
static const QString bioKey = "abstract";
static const QString avatarKey = "avatar";

// keys used from Server API etc
static const QString idForeignKey = "id";
static const QString firstNameForeignKey = "first_name";
static const QString lastNameForeignKey = "last_name";
static const QString publicNameForeignKey = "full_public_name";
static const QString bioForeignKey = "abstract";
static const QString avatarForeignKey = "avatar";

/*
 * Default Constructor if CreatorAPI not initialized from QVariantMap
 */
CreatorAPI::CreatorAPI(QObject *parent) :
        QObject(parent), mId(-1), mFirstName(""), mLastName(""), mPublicName(""), mBio(""), mAvatar("")
{
}

/*
 * initialize CreatorAPI from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void CreatorAPI::fillFromMap(const QVariantMap& creatorAPIMap)
{
	mId = creatorAPIMap.value(idKey).toInt();
	mFirstName = creatorAPIMap.value(firstNameKey).toString();
	mLastName = creatorAPIMap.value(lastNameKey).toString();
	mPublicName = creatorAPIMap.value(publicNameKey).toString();
	mBio = creatorAPIMap.value(bioKey).toString();
	mAvatar = creatorAPIMap.value(avatarKey).toString();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void CreatorAPI::fillFromForeignMap(const QVariantMap& creatorAPIMap)
{
	mId = creatorAPIMap.value(idForeignKey).toInt();
	mFirstName = creatorAPIMap.value(firstNameForeignKey).toString();
	mLastName = creatorAPIMap.value(lastNameForeignKey).toString();
	mPublicName = creatorAPIMap.value(publicNameForeignKey).toString();
	mBio = creatorAPIMap.value(bioForeignKey).toString();
	mAvatar = creatorAPIMap.value(avatarForeignKey).toString();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void CreatorAPI::fillFromCacheMap(const QVariantMap& creatorAPIMap)
{
	mId = creatorAPIMap.value(idKey).toInt();
	mFirstName = creatorAPIMap.value(firstNameKey).toString();
	mLastName = creatorAPIMap.value(lastNameKey).toString();
	mPublicName = creatorAPIMap.value(publicNameKey).toString();
	mBio = creatorAPIMap.value(bioKey).toString();
	mAvatar = creatorAPIMap.value(avatarKey).toString();
}

void CreatorAPI::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool CreatorAPI::isValid()
{
	if (mId == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from CreatorAPI as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap CreatorAPI::toMap()
{
	QVariantMap creatorAPIMap;
	creatorAPIMap.insert(idKey, mId);
	creatorAPIMap.insert(firstNameKey, mFirstName);
	creatorAPIMap.insert(lastNameKey, mLastName);
	creatorAPIMap.insert(publicNameKey, mPublicName);
	creatorAPIMap.insert(bioKey, mBio);
	creatorAPIMap.insert(avatarKey, mAvatar);
	return creatorAPIMap;
}

/*
 * Exports Properties from CreatorAPI as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap CreatorAPI::toForeignMap()
{
	QVariantMap creatorAPIMap;
	creatorAPIMap.insert(idForeignKey, mId);
	creatorAPIMap.insert(firstNameForeignKey, mFirstName);
	creatorAPIMap.insert(lastNameForeignKey, mLastName);
	creatorAPIMap.insert(publicNameForeignKey, mPublicName);
	creatorAPIMap.insert(bioForeignKey, mBio);
	creatorAPIMap.insert(avatarForeignKey, mAvatar);
	return creatorAPIMap;
}


/*
 * Exports Properties from CreatorAPI as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap CreatorAPI::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: id
// Domain KEY: id
int CreatorAPI::id() const
{
	return mId;
}

void CreatorAPI::setId(int id)
{
	if (id != mId) {
		mId = id;
		emit idChanged(id);
	}
}
// ATT 
// Optional: firstName
QString CreatorAPI::firstName() const
{
	return mFirstName;
}

void CreatorAPI::setFirstName(QString firstName)
{
	if (firstName != mFirstName) {
		mFirstName = firstName;
		emit firstNameChanged(firstName);
	}
}
// ATT 
// Optional: lastName
QString CreatorAPI::lastName() const
{
	return mLastName;
}

void CreatorAPI::setLastName(QString lastName)
{
	if (lastName != mLastName) {
		mLastName = lastName;
		emit lastNameChanged(lastName);
	}
}
// ATT 
// Optional: publicName
QString CreatorAPI::publicName() const
{
	return mPublicName;
}

void CreatorAPI::setPublicName(QString publicName)
{
	if (publicName != mPublicName) {
		mPublicName = publicName;
		emit publicNameChanged(publicName);
	}
}
// ATT 
// Optional: bio
QString CreatorAPI::bio() const
{
	return mBio;
}

void CreatorAPI::setBio(QString bio)
{
	if (bio != mBio) {
		mBio = bio;
		emit bioChanged(bio);
	}
}
// ATT 
// Optional: avatar
QString CreatorAPI::avatar() const
{
	return mAvatar;
}

void CreatorAPI::setAvatar(QString avatar)
{
	if (avatar != mAvatar) {
		mAvatar = avatar;
		emit avatarChanged(avatar);
	}
}


CreatorAPI::~CreatorAPI()
{
	// place cleanUp code here
}
	
