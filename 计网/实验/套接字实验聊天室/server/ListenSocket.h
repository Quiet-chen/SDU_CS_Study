#pragma once
#include <afxsock.h>

class CchatserverDlg;

class CListenSocket :
    public CSocket
{
public:
    CListenSocket(CchatserverDlg* pdlg);
    virtual ~CListenSocket();
private:
    CchatserverDlg* m_pMainDlg;

public:
    virtual void OnAccept(int nErrorCode);
};

