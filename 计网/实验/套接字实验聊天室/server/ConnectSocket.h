#pragma once
#include <afxsock.h>

class CchatserverDlg;

class CConnectSocket :
    public CSocket
{
public:
    CConnectSocket();
    CConnectSocket(CchatserverDlg* pdlg);
    virtual ~CConnectSocket();

private:
    CchatserverDlg* m_pMainDlg;

public:
    virtual void OnReceive(int nErrorCode);
};

