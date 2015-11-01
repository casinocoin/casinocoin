#include "casinocoinwebapiparser.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <QDebug>
#include <QDir>
#include <QFile>

CasinoCoinWebAPIParser::CasinoCoinWebAPIParser( QObject* a_pParent )
	: QObject(a_pParent)
{
}

void CasinoCoinWebAPIParser::slotParseAnswer( const QByteArray& a_rJsonFile )
{
	QJsonParseError oError;
	QJsonDocument jsonDoc = QJsonDocument::fromJson( a_rJsonFile, &oError );
	if ( oError.error == QJsonParseError::NoError )
	{
		QJsonObject docAsObject = jsonDoc.object();
		if ( docAsObject.find( "Result" ).value().isObject() )
		{
			QJsonObject jsonObjectResult = docAsObject.find( "Result" ).value().toObject();
			if ( jsonObjectResult.find( "ActivePromotions" ).value().isArray() )
			{
				StoreFile( "ActivePromotions", a_rJsonFile );
				ParsePromotions( docAsObject );
			}
			else if ( jsonObjectResult.find( "ActiveNewsItems" ).value().isArray() )
			{
				StoreFile( "ActiveNewsItems", a_rJsonFile );
				ParseNewsItems( docAsObject );
			}
			else if ( jsonObjectResult.find( "ActiveCasinos" ).value().isArray() )
			{
				StoreFile( "ActiveCasinos", a_rJsonFile );
				ParseCasinos( docAsObject );
			}
			else if ( jsonObjectResult.find( "ActiveExchanges" ).value().isArray() )
			{
				StoreFile( "ActiveExchanges", a_rJsonFile );
				ParseExchanges( docAsObject );
			}
		}
	}
}

void CasinoCoinWebAPIParser::slotNetworkError( QNetworkReply::NetworkError a_eError
											 , const QUrl a_rFailedUrl
											 )
{
	qDebug() << "network error: " << a_eError;
	QString strAccessedUrl = a_rFailedUrl.toString().split("/").last();
	slotParseAnswer( ReadFile( strAccessedUrl ) );
}

void CasinoCoinWebAPIParser::ParsePromotions( const QJsonObject& a_rJsonPromotions )
{
	qDebug() << "ParsePromotions";
	emit signalActivePromotionsParsed( new JsonActivePromotionsParser( a_rJsonPromotions ) );
}

void CasinoCoinWebAPIParser::ParseCasinos( const QJsonObject& a_rJsonCasinos )
{
	qDebug() << "Coming soon - ParseCasinos";
	qDebug() << a_rJsonCasinos;
}

void CasinoCoinWebAPIParser::ParseExchanges( const QJsonObject& a_rJsonExchanges )
{
	qDebug() << "Coming soon - ParseExchanges";
	qDebug() << a_rJsonExchanges;
}

void CasinoCoinWebAPIParser::ParseNewsItems( const QJsonObject& a_rJsonNewsItems )
{
	qDebug() << "Coming soon - ParseNewsItems";
	qDebug() << a_rJsonNewsItems;
}

QByteArray CasinoCoinWebAPIParser::ReadFile( QString a_strSourcePath )
{
	QByteArray strAnswer;
	if ( !QDir( "offlineData" ).exists() )
	{
		QDir().mkdir( "offlineData" );
	}
	QFile fileOutput( QDir( "offlineData" ).absoluteFilePath( a_strSourcePath ) );
	if ( !fileOutput.open( QIODevice::ReadOnly ) )
	{
		qWarning() << "cannot open file to read: " << QDir::current().relativeFilePath( a_strSourcePath );
	}
	else
	{
		strAnswer = fileOutput.readAll();
		fileOutput.close();
	}
	return strAnswer;
}

void CasinoCoinWebAPIParser::StoreFile( QString a_strDestinationPath, const QByteArray& a_rJsonFile )
{
	if ( !QDir( "offlineData" ).exists() )
	{
		QDir().mkdir( "offlineData" );
	}
	QFile fileOutput( QDir( "offlineData" ).absoluteFilePath( a_strDestinationPath ) );
	if ( !fileOutput.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
	{
		qWarning() << "cannot open file to write: " << QDir::current().relativeFilePath( a_strDestinationPath );
	}
	else
	{
		fileOutput.write( a_rJsonFile );
		fileOutput.close();
	}
}
