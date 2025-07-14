
// chatserverDlg.cpp: 实现文件
//

#include "ListenSocket.h"
#include "pch.h"
#include "framework.h"
#include "chatserver.h"
#include "chatserverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CchatserverDlg 对话框



CchatserverDlg::CchatserverDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CchatserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CchatserverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT2, &CchatserverDlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT1, &CchatserverDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CchatserverDlg 消息处理程序

BOOL CchatserverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_pListenSocket = new CListenSocket(this);
	m_pListenSocket->Create(8080);

	if (!m_pListenSocket->Listen()) {
		MessageBox(_T("监听失败"));
		return FALSE;
	}


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CchatserverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CchatserverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CchatserverDlg::OnEnChangeEdit2() {

}
void CchatserverDlg::OnEnChangeEdit1() {

}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CchatserverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CchatserverDlg::ProcessPendingAccept() {
	CConnectSocket* pConnectSocket = new CConnectSocket(this);
	if (m_pListenSocket->Accept(*pConnectSocket)) {
		m_pConnectsocketlist.AddTail(pConnectSocket);
	}
	else {
		delete pConnectSocket;
	}
}

void CchatserverDlg::ProcessPendingRead(CConnectSocket* connectSocket) {
	TCHAR buff[4096];
	ClientAddr clientaddr;

	//接收数据
	int nread = connectSocket->Receive(buff, 4096);
	if (nread == SOCKET_ERROR) {
		return;
	}
	
	//获取连接的客户端的ip、端口
	connectSocket->GetPeerName(clientaddr.strip, clientaddr.uiport);

	buff[nread] = _T('\0');
	CString strtemp(buff);
	POSITION pos = m_pConnectsocketlist.GetHeadPosition();
	CString temp;

	//解析数据（进入房间、离开房间、消息）
	if (strtemp.CompareNoCase(_T("enter")) == 0) {
		//有人进入房间
		nRoom++;
		SetDlgItemInt(IDC_EDIT1, nRoom);
		m_pClientAddrlist.AddTail(clientaddr);

		temp.Format(L"系统消息：%ls(%u)进入了房间!", clientaddr.strip.GetString(), clientaddr.uiport);
		while(pos!=NULL) {
			CConnectSocket* p = m_pConnectsocketlist.GetNext(pos);
			p->Send(temp, temp.GetLength() + 100);
		}
		

		CString allMsg;
		GetDlgItemText(IDC_EDIT2, allMsg);
		SetDlgItemText(IDC_EDIT2, allMsg + _T("\r\n") + temp);//追加消息
	}
	else if (strtemp.CompareNoCase(_T("leave")) == 0) {
		//有人离开房间
		nRoom--;
		SetDlgItemInt(IDC_EDIT1, nRoom);
		
		temp.Format(L"系统消息：%ls(%u)离开了房间!", clientaddr.strip.GetString(), clientaddr.uiport);
		while (pos != NULL) {
			CConnectSocket* p = m_pConnectsocketlist.GetNext(pos);
			p->Send(temp, temp.GetLength() + 100);
		}

		//删除该套接字和addr
		pos = m_pConnectsocketlist.GetHeadPosition();
		while (pos != NULL){
			CConnectSocket* p = m_pConnectsocketlist.GetAt(pos);
			if (*p == *connectSocket){
				m_pConnectsocketlist.RemoveAt(pos);
				break;
			}
			m_pConnectsocketlist.GetNext(pos);
		}
		pos = m_pClientAddrlist.GetHeadPosition();
		while (pos != NULL) {
			ClientAddr p = m_pClientAddrlist.GetAt(pos);
			if (p == clientaddr) {
				m_pClientAddrlist.RemoveAt(pos);
				break;
			}
			m_pClientAddrlist.GetNext(pos);
		}

		CString allMsg;
		GetDlgItemText(IDC_EDIT2, allMsg);
		SetDlgItemText(IDC_EDIT2, allMsg + _T("\r\n") + temp);//追加消息
	}
	else {
		temp.Format(L"%ls(%u): %ls", clientaddr.strip.GetString(), clientaddr.uiport, strtemp.GetString());
		while (pos != NULL) {
			CConnectSocket* p = m_pConnectsocketlist.GetNext(pos);
			p->Send(temp, temp.GetLength() + 100);
		}

		CString allMsg;
		GetDlgItemText(IDC_EDIT2, allMsg);
		SetDlgItemText(IDC_EDIT2, allMsg + _T("\r\n") + temp);//追加消息
	}

}

