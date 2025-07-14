#include "ConnectSocket.h"
#include "ListenSocket.h"
// chatserverDlg.h: 头文件
//

#pragma once

struct ClientAddr {
	CString strip;
	UINT uiport;
	bool operator==(ClientAddr b) {
		if (strip == b.strip && uiport == b.uiport) return true;
		else return false;
	}
};


// CchatserverDlg 对话框
class CchatserverDlg : public CDialogEx
{
// 构造
public:
	CchatserverDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void ProcessPendingRead(CConnectSocket* connectSocket);
	void ProcessPendingAccept();
	CConnectSocket* m_pConnectSocket;
	CListenSocket* m_pListenSocket;
	int nRoom;
	CList<CConnectSocket*>m_pConnectsocketlist;
	CList<ClientAddr>m_pClientAddrlist;

	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit1();

};
