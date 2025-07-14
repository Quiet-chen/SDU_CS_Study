#pragma once
#include <afxsock.h>

class  CchatclientDlg;

class CConnectSocket :
    public CSocket
{
public:
    CConnectSocket(CchatclientDlg* pdlg);
    virtual ~CConnectSocket();

private:
    CchatclientDlg* m_pMainDlg;

public:
    virtual void OnReceive(int nErrorCode);
};

