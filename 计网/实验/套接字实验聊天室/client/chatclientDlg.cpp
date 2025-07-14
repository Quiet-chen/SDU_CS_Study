
// chatclientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "chatclient.h"
#include "chatclientDlg.h"
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


// CchatclientDlg 对话框



CchatclientDlg::CchatclientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CchatclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CchatclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ENTER, &CchatclientDlg::OnBnClickedButtonEnter)
	ON_BN_CLICKED(IDC_BUTTON_LEAVE, &CchatclientDlg::OnBnClickedButtonLeave)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CchatclientDlg::OnBnClickedButtonSend)
	ON_EN_CHANGE(IDC_EDIT_MESSAGE, &CchatclientDlg::OnEnChangeEditMessage)
END_MESSAGE_MAP()

void CchatclientDlg::OnEnChangeEditMessage() {

}

// CchatclientDlg 消息处理程序

BOOL CchatclientDlg::OnInitDialog()
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
	SetDlgItemText(IDC_EDIT_SERVERIP, _T("127.0.0.1"));
	SetDlgItemInt(IDC_EDIT_SERVERPORT, 8080);
	GetDlgItem(IDC_BUTTON_ENTER)->EnableWindow(TRUE);//初始化只有加入房间可用
	GetDlgItem(IDC_BUTTON_LEAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_SENDMESSAGE))->SetReadOnly(TRUE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CchatclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CchatclientDlg::OnPaint()
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CchatclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CchatclientDlg::ProcessPendingRead() {
	TCHAR buff[4096];
	int nread = m_pConnectSocket->Receive(buff, 4096);
	if (nread == SOCKET_ERROR) {
		return;
	}

	buff[nread] = _T('\0');
	CString chatMsg(buff);

	CString allMsg;
	GetDlgItemText(IDC_EDIT_MESSAGE, allMsg);
	SetDlgItemText(IDC_EDIT_MESSAGE, allMsg + _T("\r\n") + chatMsg);//追加消息
}



void CchatclientDlg::OnBnClickedButtonEnter()
{
	// TODO: 在此添加控件通知处理程序代码

	//创建TCP套接字并绑定一个系统分配的端口号
	m_pConnectSocket = new CConnectSocket(this);
	m_pConnectSocket->Create();

	//获取本地ip、端口号
	CString strlocalip;
	UINT uilocalport;
	m_pConnectSocket->GetSockName(strlocalip, uilocalport);
	SetDlgItemText(IDC_EDIT_LOCALIP, strlocalip);//显示到窗口
	SetDlgItemInt(IDC_EDIT_LOCALPORT, uilocalport);

	//调用connect函数，连接到服务器
	CString strserverip;
	UINT uiserverport;
	GetDlgItemText(IDC_EDIT_SERVERIP, strserverip);
	uiserverport = GetDlgItemInt(IDC_EDIT_SERVERPORT);

	if (!m_pConnectSocket->Connect(strserverip, uiserverport)) {
		MessageBox(_T("无法连接服务器"));
		return ;
	}
	
	CString strEnterMsg("enter");
	m_pConnectSocket->Send(strEnterMsg, strEnterMsg.GetLength() + 100);

	GetDlgItem(IDC_BUTTON_ENTER)->EnableWindow(FALSE);//进入房间后
	GetDlgItem(IDC_BUTTON_LEAVE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_SERVERIP))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_SERVERPORT))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_SENDMESSAGE))->SetReadOnly(FALSE);

	m_bEnterRoom = TRUE;
}


void CchatclientDlg::OnBnClickedButtonLeave()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLeaveMsg("leave");
	m_pConnectSocket->Send(strLeaveMsg, strLeaveMsg.GetLength() + 100);

	GetDlgItem(IDC_BUTTON_ENTER)->EnableWindow(TRUE);//离开房间后重回初始化状态
	GetDlgItem(IDC_BUTTON_LEAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_SERVERIP))->SetReadOnly(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_SERVERPORT))->SetReadOnly(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_SENDMESSAGE))->SetReadOnly(TRUE);

	m_bEnterRoom = FALSE;
}


void CchatclientDlg::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strMsg;
	GetDlgItemText(IDC_EDIT_SENDMESSAGE, strMsg);
	m_pConnectSocket->Send(strMsg, strMsg.GetLength() + 100);
	SetDlgItemText(IDC_EDIT_SENDMESSAGE, _T(""));
}


BOOL CchatclientDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_bEnterRoom) {
		OnBnClickedButtonLeave();
	}
	return CDialogEx::DestroyWindow();
}
