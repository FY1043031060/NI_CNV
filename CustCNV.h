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
    CNVSubscriber m_subscriberRecv = NULL;
    CNVBufferedWriter m_bufferSend = NULL;
    QString m_strValuePath;
    QMutex m_mutexSend;

    void DisposeSubscriber();
    Q_DISABLE_COPY(CustCNV)
};
#include <QHash>
class CustCNVManager : public QObject
{
    Q_OBJECT
public:
    CustCNV* createCustCNV(QString str, QObject* parent);
    void deleteCustCNV(CustCNV* pObj);
    ~CustCNVManager();
    static CustCNVManager& getInstance(){
        static CustCNVManager instance;
        return instance;
    }
private:
    explicit CustCNVManager(QObject* parent = Q_NULLPTR) : QObject(parent) {}
    QHash<CustCNV*, QString> m_hash;
};

#endif // CUSTCNV_H
