#include "pch.h"
#include "ConnectSocket.h"
#include "chatclientDlg.h"


CConnectSocket::CConnectSocket(CchatclientDlg* pdlg) {//����
	m_pMainDlg = pdlg;
}
CConnectSocket::~CConnectSocket() {//����

}

void CConnectSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���

	CSocket::OnReceive(nErrorCode);
	m_pMainDlg->ProcessPendingRead();
}
