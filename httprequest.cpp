#include "httprequest.h"

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

HttpRequest::HttpRequest(QObject *parent) : QObject(parent)
{

}

QByteArray HttpRequest::getRequest(const QString url)
{
    QNetworkRequest request;
    QNetworkAccessManager *naManager=new QNetworkAccessManager();
    QUrl strUrl = url;
    request.setUrl(strUrl);
    QNetworkReply* reply = naManager->get(request);

    QEventLoop eventLoop;
    connect(naManager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    reply->deleteLater();
    return reply->readAll();
}
