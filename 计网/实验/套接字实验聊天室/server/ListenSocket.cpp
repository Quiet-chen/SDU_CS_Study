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
	// TODO: �ڴ����ר�ô����/����û���

	CSocket::OnAccept(nErrorCode);
	m_pMainDlg->ProcessPendingAccept();
	
}
