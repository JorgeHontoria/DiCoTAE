#ifndef CREATORIMAGE_HPP_
#define CREATORIMAGE_HPP_

#include <QObject>
#include <qvariant.h>




class CreatorImage: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int creatorId READ creatorId WRITE setCreatorId NOTIFY creatorIdChanged FINAL)
	Q_PROPERTY(QString originImageUrl READ originImageUrl WRITE setOriginImageUrl NOTIFY originImageUrlChanged FINAL)
	Q_PROPERTY(bool downloadSuccess READ downloadSuccess WRITE setDownloadSuccess NOTIFY downloadSuccessChanged FINAL)
	Q_PROPERTY(bool downloadFailed READ downloadFailed WRITE setDownloadFailed NOTIFY downloadFailedChanged FINAL)
	Q_PROPERTY(bool inAssets READ inAssets WRITE setInAssets NOTIFY inAssetsChanged FINAL)
	Q_PROPERTY(bool inData READ inData WRITE setInData NOTIFY inDataChanged FINAL)
	Q_PROPERTY(QString suffix READ suffix WRITE setSuffix NOTIFY suffixChanged FINAL)
	Q_PROPERTY(int maxScaleFactor READ maxScaleFactor WRITE setMaxScaleFactor NOTIFY maxScaleFactorChanged FINAL)


public:
	CreatorImage(QObject *parent = 0);


	void fillFromMap(const QVariantMap& creatorImageMap);
	void fillFromForeignMap(const QVariantMap& creatorImageMap);
	void fillFromCacheMap(const QVariantMap& creatorImageMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int creatorId() const;
	void setCreatorId(int creatorId);
	QString originImageUrl() const;
	void setOriginImageUrl(QString originImageUrl);
	bool downloadSuccess() const;
	void setDownloadSuccess(bool downloadSuccess);
	bool downloadFailed() const;
	void setDownloadFailed(bool downloadFailed);
	bool inAssets() const;
	void setInAssets(bool inAssets);
	bool inData() const;
	void setInData(bool inData);
	QString suffix() const;
	void setSuffix(QString suffix);
	int maxScaleFactor() const;
	void setMaxScaleFactor(int maxScaleFactor);



	virtual ~CreatorImage();

	Q_SIGNALS:

	void creatorIdChanged(int creatorId);
	void originImageUrlChanged(QString originImageUrl);
	void downloadSuccessChanged(bool downloadSuccess);
	void downloadFailedChanged(bool downloadFailed);
	void inAssetsChanged(bool inAssets);
	void inDataChanged(bool inData);
	void suffixChanged(QString suffix);
	void maxScaleFactorChanged(int maxScaleFactor);
	

private:

	int mCreatorId;
	QString mOriginImageUrl;
	bool mDownloadSuccess;
	bool mDownloadFailed;
	bool mInAssets;
	bool mInData;
	QString mSuffix;
	int mMaxScaleFactor;

	Q_DISABLE_COPY (CreatorImage)
};
Q_DECLARE_METATYPE(CreatorImage*)

#endif /* CREATORIMAGE_HPP_ */

