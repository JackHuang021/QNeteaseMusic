#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>

class HttpRequest : public QObject
{
    Q_OBJECT
public:
    explicit HttpRequest(QObject *parent = nullptr);
    static QByteArray getRequest(const QString url);

signals:

};

#endif // HTTPREQUEST_H
