#pragma once

#include <memory>

class CPOCOReactorServer;
class CPOCOReactorClient;

class EmulatorDlg : public CDialogEx
{
	DECLARE_MESSAGE_MAP()

private:
	HICON m_hIcon;	
	int   m_port = 1470;

	CString m_ip2;
	int   m_port2 = 4242;

	std::unique_ptr<CPOCOReactorServer>    m_server;
	std::unique_ptr<CPOCOReactorClient>    m_client;

public:
	EmulatorDlg(CWnd* pParent = nullptr);

	// std::unique_ptr  사용시에 Destroy function 을 정의 및 구현부를 작성해야 한다.
	virtual ~EmulatorDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHROZENEMULATOR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();	

	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();	
	
	afx_msg void OnBnClickedActivate();
	afx_msg void OnBnClickedActivate2();
};
