#include "pch.h"
#include "ConnectSocket.h"
#include "chatclientDlg.h"


CConnectSocket::CConnectSocket(CchatclientDlg* pdlg) {//构造
	m_pMainDlg = pdlg;
}
CConnectSocket::~CConnectSocket() {//析构

}

void CConnectSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CSocket::OnReceive(nErrorCode);
	m_pMainDlg->ProcessPendingRead();
}
