
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
#include "HT3X00AsManager.h"
#include "HT3X00AS/XHt3000AsInform.h"
#include "HT3X00AS/XHt3000AsConfig.h"
#include "HT3X00AS/XHt3000AsState.h"


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
EmulatorDlg::EmulatorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHROZENEMULATOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_server = std::make_unique<CPOCOReactorServer>();
	m_HT3X00A = std::make_unique<HT3X00AsManager>();

	//Test_XHt3000AsInformExample1();
	//Test_XHt3000AsInformExample2();
	//Test_XHt3000AsInformExample3();
	//Test_XHt3000AsInformExample4();
	//Test_XHt3000AsInformExample5();
	//Test_XHt3000AsState1();

	//PacketExtention_RequestPacket_Test1();
	//HT3X00AsManager_Test1();	
}

EmulatorDlg::~EmulatorDlg()
{

}

void EmulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDX_Text(pDX, IDC_STATIC_STATUS, m_status);
	DDX_Text(pDX, IDC_EDIT_CONNECTION, m_connection);
	DDX_Text(pDX, IDC_EDIT_FW, m_fw);
	DDX_Text(pDX, IDC_EDIT_SN, m_sn);
	DDX_Text(pDX, IDC_EDIT_INJECTORS, m_injectors);
	DDX_Text(pDX, IDC_EDIT_TRAY, m_tray);
	DDX_Text(pDX, IDC_EDIT_VIAL, m_vial);
	DDX_Text(pDX, IDC_EDIT_BARCODE, m_barcode_installed);
	DDX_Text(pDX, IDC_EDIT_SYRINGE_ID, m_syringe_id_installed);
}

BEGIN_MESSAGE_MAP(EmulatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()	
	
	ON_MESSAGE(WM_HT3X00A_UPDATE, &EmulatorDlg::OnHT3x00AUpdate)
	ON_BN_CLICKED(IDC_ACTIVATE, &EmulatorDlg::OnBnClickedActivate)	
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_OF_SAMPLER, &EmulatorDlg::OnCbnSelchangeComboTypeOfSampler)
	ON_CBN_SELCHANGE(IDC_COMBO_SYRINGE_TYPE, &EmulatorDlg::OnCbnSelchangeComboSyringeType)
	ON_BN_CLICKED(IDC_ALARM, &EmulatorDlg::OnBnClickedAlarm)
	ON_BN_CLICKED(IDC_RESET, &EmulatorDlg::OnBnClickedReset)
	ON_CBN_SELCHANGE(IDC_COMBO_ERROR, &EmulatorDlg::OnCbnSelchangeComboError)
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
	CComboBox& combo_model = *GetComboBoxPtr(this, IDC_COMBO_TYPE_OF_SAMPLER);
	XHt3000AsInform* inform = m_HT3X00A->GetInform();
	AddComboBoxItem(combo_model, _T("HT3000A"), XHtHT3000A);
	AddComboBoxItem(combo_model, _T("HT3100A"), XHtHT3100A);
	AddComboBoxItem(combo_model, _T("HT3200A"), XHtHT3200A);
	SetCurSel_ByData(combo_model, inform->model);
	
	CComboBox& combo_syringe = *GetComboBoxPtr(this, IDC_COMBO_SYRINGE_TYPE);
	XHt3000AsConfig* config = m_HT3X00A->GetConfig();
	AddComboBoxItem(combo_syringe, _T("1")  , 0);
	AddComboBoxItem(combo_syringe, _T("5")  , 1);
	AddComboBoxItem(combo_syringe, _T("10") , 2);
	AddComboBoxItem(combo_syringe, _T("25") , 3);
	AddComboBoxItem(combo_syringe, _T("50") , 4);
	AddComboBoxItem(combo_syringe, _T("100"), 5);
	AddComboBoxItem(combo_syringe, _T("250"), 6);
	AddComboBoxItem(combo_syringe, _T("500"), 7);
	AddComboBoxItem(combo_syringe, _T("0.5"), 8);
	SetCurSel_ByData(combo_syringe, config->syringe);
	
	CComboBox& combo_error = *GetComboBoxPtr(this, IDC_COMBO_ERROR);
	AddComboBoxItem(combo_error, _T("Missing vial"), SingleMissingVial);
	AddComboBoxItem(combo_error, _T("Single injection - Error found during execution"), SingleErrorFound);
	AddComboBoxItem(combo_error, _T("Automatic run - Error found during execution"), AutoErrorFound);
	AddComboBoxItem(combo_error, _T("Single injection - Over Hardware Error"), SingleOvenHardwareError);
	AddComboBoxItem(combo_error, _T("Automatic run - Over Hardware Error"), AutoOvenHardwareError);
	AddComboBoxItem(combo_error, _T("Single Preparation - Error found during execution"), SingleErrorFoundDuringExecution);
	AddComboBoxItem(combo_error, _T("Single Preparation - Checking parameter Error"), SingleCheckingParameterError1);
	AddComboBoxItem(combo_error, _T("Single injection - Checking paramether Error"), SingleCheckingParameterError2);
	AddComboBoxItem(combo_error, _T("Automatic run - Checking paramether Error"), AutoCheckingParameterError);
	AddComboBoxItem(combo_error, _T("Single injection - Error on Conditioning Temperature"), SingleErrorOnConditioningTemper);
	AddComboBoxItem(combo_error, _T("Automatic run - Error on Conditioning Temperature"), AutoErrorOnConditioningTemper);
	SetCurSel_ByData(combo_error, SingleMissingVial);

	m_connection = CFormat(_T("TCP_IP %d.%d.%d.%d:%d"), inform->ip_address[0], inform->ip_address[1], inform->ip_address[2], inform->ip_address[3], m_port);
	m_fw = CString(inform->version);
	m_sn = CString(inform->serial);
	m_injectors = config->injectors;
	m_tray = config->tray;
	m_vial = config->vial / 10;
	m_barcode_installed = _T("Not installed");
	m_syringe_id_installed = _T("Not installed");

	UpdateData(FALSE);
	OnHT3x00AUpdate(0, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void EmulatorDlg::OnDestroy()
{
	CDialogEx::OnDestroy();	

	m_HT3X00A->Deactivate();
	m_server->Deactivate();		
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

LRESULT EmulatorDlg::OnHT3x00AUpdate(WPARAM wParam, LPARAM lParam)
{
	m_status = m_HT3X00A->GetStatusStr();
	UpdateData(FALSE);
	return 0;
}

void EmulatorDlg::CopyStrToArray(const CString& str, char* arr, int size)
{
	memset(arr, 0, size);

	std::string s;
	convert_string(str.GetString(), s);	

	memcpy(arr, s.c_str(), size);
}

void EmulatorDlg::OnBnClickedActivate()
{
	int ctrl_id[] = {
			IDC_COMBO_TYPE_OF_SAMPLER,
			IDC_EDIT_CONNECTION,
			IDC_EDIT_FW,
			IDC_EDIT_SN,
			IDC_EDIT_VIAL,
			IDC_COMBO_SYRINGE_TYPE,
			IDC_EDIT_INJECTORS,
			IDC_EDIT_TRAY,
			IDC_EDIT_BARCODE,
			IDC_EDIT_SYRINGE_ID
	};
	int count = sizeof(ctrl_id) / sizeof(int);

    bool check = GetButtonCheck(this, IDC_ACTIVATE);
    if (check)
    {		
		for (int i = 0; i < count; i++)
			GetDlgItem(ctrl_id[i])->EnableWindow(false);

        UpdateData(TRUE);

		XHt3000AsInform* inform = m_HT3X00A->GetInform();
		XHt3000AsConfig* config = m_HT3X00A->GetConfig();

		CopyStrToArray(m_fw, inform->version, sizeof(inform->version) - 1);
		CopyStrToArray(m_sn, inform->serial, sizeof(inform->serial) - 1);

		config->tray = m_tray;
		config->vial = m_vial * 10;

		m_HT3X00A->Activate(GetSafeHwnd());
        m_server->Activate(m_port, m_HT3X00A.get());
    }
    else
    {		
		for (int i = 0; i < count; i++)
			GetDlgItem(ctrl_id[i])->EnableWindow(true);

		m_HT3X00A->Deactivate();
        m_server->Deactivate();        
    }
}

void EmulatorDlg::OnCbnSelchangeComboTypeOfSampler()
{
	CComboBox& combo = *GetComboBoxPtr(this, IDC_COMBO_TYPE_OF_SAMPLER);
	XHt3000AsInform* inform = m_HT3X00A->GetInform();

	int data = 0;
	GetCurData(combo, data);

	if (data != 0)
	{
		inform->model = data;
		XHt3000AsConfig* config = m_HT3X00A->GetConfig();
		
		switch (inform->model)
		{
		case XHtHT3000A:
			config->tray = 121;
			config->vial = 20;
			break;
		case XHtHT3100A:
			config->tray = 20;
			config->vial = 20;
			break;
		case XHtHT3200A:
			config->tray = 209;
			config->vial = 20;
			break;
		default:
			break;
		}
		
		m_tray = config->tray;
		m_vial = config->vial / 10;

		UpdateData(FALSE);
	}
}

void EmulatorDlg::OnCbnSelchangeComboSyringeType()
{
	CComboBox& combo = *GetComboBoxPtr(this, IDC_COMBO_SYRINGE_TYPE);
	XHt3000AsConfig* config = m_HT3X00A->GetConfig();

	int data = 0;
	GetCurData(combo, data);

	if (data != 0)
	{
		config->syringe = data;
	}
}

void EmulatorDlg::OnBnClickedAlarm()
{
	CComboBox& combo = *GetComboBoxPtr(this, IDC_COMBO_ERROR);

	int data = 0;
	GetCurData(combo, data);

	m_HT3X00A->SetError((XHt3000Status1NewCode)data);
}

void EmulatorDlg::OnBnClickedReset()
{
	m_HT3X00A->SetError(StandBy);
}

void EmulatorDlg::OnCbnSelchangeComboError()
{
	// TODO: Add your control notification handler code here
}
