#include "pch.h"
#include "ListenSocket.h"
#include "chatserverDlg.h"

class CchatserverDlg;


CListenSocket::CListenSocket(CchatserverDlg* pdlg) {
	m_pMainDlg = pdlg;
}
CListenSocket::~CListenSocket() {

}

void CListenSocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CSocket::OnAccept(nErrorCode);
	m_pMainDlg->ProcessPendingAccept();
	
}
