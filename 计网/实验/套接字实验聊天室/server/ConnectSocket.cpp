#include "pch.h"
#include "ConnectSocket.h"
#include "chatserverDlg.h"


CConnectSocket::CConnectSocket() {//����

}
CConnectSocket::CConnectSocket(CchatserverDlg* pdlg) {//����
	m_pMainDlg = pdlg;
}
CConnectSocket::~CConnectSocket() {//����

}


void CConnectSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���

	CSocket::OnReceive(nErrorCode);
	m_pMainDlg->ProcessPendingRead(this);
	
}
