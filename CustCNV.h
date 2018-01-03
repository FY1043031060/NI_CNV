#ifndef CUSTCNV_H
#define CUSTCNV_H

#include <QObject>
#include <cvinetv.h>
#include <QString>
#include <QVariant>
#include <QMutex>

class CustCNV : public QObject
{
    Q_OBJECT
public:
    explicit CustCNV(QString strValuePath, QObject *parent = nullptr) ;
    ~CustCNV();

Q_SIGNALS:
    void recvData(QVariant varData);

public Q_SLOTS:
    void sendData(QVariant varData);

private:
    static void CVICALLBACK DataCallback (void * handle, CNVData data,
                                          void * callbackData);
    static void CVICALLBACK StatusCallback (void * handle,
                                            CNVConnectionStatus status,
                                            int error, void * callbackData);
    CNVSubscriber m_subscriberRecv;
    CNVBufferedWriter m_bufferSend;
    QString m_strValuePath;
    QMutex m_mutexSend;

    void DisposeSubscriber();
};

#endif // CUSTCNV_H
