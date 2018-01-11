#include "CustCNV.h"
#include <QMutexLocker>
////
/// \brief CustCNV::CustCNV
/// \param strValuePath
/// \param parent
///
CustCNV::CustCNV(QString strValuePath, QObject *parent)
    : QObject(parent), m_strValuePath(strValuePath)
{
    ////subscriber for recv data
    CNVCreateSubscriber(m_strValuePath.toLocal8Bit().data(),
                        DataCallback,
                        StatusCallback,
                        this,
                        10000,
                        0,
                        &m_subscriberRecv);
    ////bufferWriter for send data
    CNVCreateBufferedWriter(m_strValuePath.toLocal8Bit().data(),
                            0,
                            0,
                            0,
                            64,
                            5000,
                            0,
                            &m_bufferSend);
}

CustCNV::~CustCNV()
{
    DisposeSubscriber();
//    CNVFinish();
}
////
/// \brief CustCNV::sendData
/// \param varData	data to send
///
void CustCNV::sendData(QVariant varData)
{
    QMutexLocker locker (&m_mutexSend);
    CNVData	data = 0;
    if(varData.type() == QVariant::Double)
    {
        CNVCreateScalarDataValue(&data, CNVDouble, 0);
        CNVSetScalarDataValue(data, CNVDouble, varData.toDouble());
        CNVPutDataInBuffer(m_bufferSend, data, 5000);
    }
    CNVDisposeData(data);
}
////
/// \brief CustCNV::DataCallback
/// 		Network Variable Data Callback - called whenever the data is updated
/// 		NOTE: This callback is called on a worker thread and not the main thread.
/// \param handle/Handle of the Network Variable connection
/// \param data/Handle of the new Network Variable data
/// \param callbackData/User specified callback data
///
void CustCNV::DataCallback(void *handle, CNVData data, void *callbackData)
{
    CustCNV* pCnv = static_cast<CustCNV*>(callbackData);
    unsigned long	nDims;
    CNVDataType		type;

    if (data == 0)
        return;

    CNVGetDataType (data, &type, &nDims);
    if (type == CNVDouble && nDims == 0)
    {
        double y;

        CNVGetScalarDataValue (data, type, &y);
        ////TODO::
        if(pCnv)
            emit pCnv->recvData(QVariant::fromValue(y));
    }

    CNVDisposeData (data);
}
////
/// \brief CustCNV::StatusCallback
///			Network Variable Status Callback called whenever connection status changes.
///			NOTE: This callback is called on a worker thread and not the main thread.
/// \param handle/Handle of the Network Variable connection
/// \param status/The new status of the Network Variable connection
/// \param error/The error, if any, in the Network Variable connection
/// \param callbackData/User specified callback data
///
void CustCNV::StatusCallback(void *handle, CNVConnectionStatus status, int error, void *callbackData)
{
    if (error < 0)
    {
            CNVGetErrorDescription(error);
    }
    else
    {
        switch (status)
        {
        case CNVConnecting:
            ////TODO::
            break;
        case CNVConnected:
            ////TODO::
            break;
        case CNVDisconnected:
            ////TODO::
            break;
        }
    }
}
////
/// \brief CustCNV::DisposeSubscriber

void CustCNV::DisposeSubscriber ()
{
    if (m_subscriberRecv)
    {
        CNVDispose (m_subscriberRecv);
        m_subscriberRecv = 0;
    }
    if(m_bufferSend)
    {
        CNVDispose(m_bufferSend);
        m_bufferSend = 0;
    }
}

CustCNV *CustCNVManager::createCustCNV(QString str, QObject *parent)
{
    auto pObj = new CustCNV(str, parent);
    m_hash.insert(pObj, str);
    return pObj;
}

void CustCNVManager::deleteCustCNV(CustCNV *pObj)
{
    if(m_hash.contains(pObj))
        pObj->deleteLater();
}
