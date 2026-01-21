
// ChroZenEmulatorDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Emulator.h"
#include "EmulatorDlg.h"
#include "afxdialogex.h"

#include "common/Util.h"
#include "common/UtilWnd.h"
#include "CPOCOReactorServer.h"
#include "CPOCOReactorClient.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// EmulatorDlg dialog
#define TIMER_ID_AS_UPDATE	(100)


EmulatorDlg::EmulatorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHROZENEMULATOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_ip2 = "127.0.0.1";
	m_server = std::make_unique<CPOCOReactorServer>();
	m_client = std::make_unique<CPOCOReactorClient>();
}

EmulatorDlg::~EmulatorDlg()
{

}

void EmulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDX_Text(pDX, IDC_EDIT_IP2, m_ip2);
	DDX_Text(pDX, IDC_EDIT_PORT2, m_port2);
}

BEGIN_MESSAGE_MAP(EmulatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()	
	
	ON_BN_CLICKED(IDC_ACTIVATE, &EmulatorDlg::OnBnClickedActivate)
	ON_BN_CLICKED(IDC_ACTIVATE2, &EmulatorDlg::OnBnClickedActivate2)
END_MESSAGE_MAP()

BOOL EmulatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here		

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void EmulatorDlg::OnDestroy()
{
	CDialogEx::OnDestroy();	

	m_server->Deactivate();
	m_client->Deactivate();
}

void EmulatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void EmulatorDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		case TIMER_ID_AS_UPDATE:
			UpdateData(FALSE);
			break;		
		default:
			break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void EmulatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR EmulatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void EmulatorDlg::OnBnClickedActivate()
{
    bool check = GetButtonCheck(this, IDC_ACTIVATE);
    if (check)
    {
        UpdateData(TRUE);
        m_server->Activate(m_port);        
    }
    else
    {
        m_server->Deactivate();        
    }
}

void EmulatorDlg::OnBnClickedActivate2()
{
	bool check = GetButtonCheck(this, IDC_ACTIVATE2);
	if (check)
	{
        UpdateData(TRUE);				
		m_client->Activate(std::string(CT2CA(m_ip2)), m_port2);
	}
	else
	{		
		m_client->Deactivate();
	}
}
