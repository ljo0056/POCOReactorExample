#pragma once

#include <memory>

class CPOCOReactorServer;
class HT3X00AsManager;

class EmulatorDlg : public CDialogEx
{
	DECLARE_MESSAGE_MAP()

private:
	HICON		m_hIcon;	
	int			m_port = 20101;
	CString		m_status;
	CString     m_connection;
	CString     m_fw;
	CString     m_sn;
	CString     m_barcode_installed;
	CString     m_syringe_id_installed;
	int			m_injectors = 1;
	int			m_tray = 121;
	int         m_vial = 2;

    std::unique_ptr<CPOCOReactorServer>    m_server;
	std::unique_ptr<HT3X00AsManager>       m_HT3X00A;	

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

	void CopyStrToArray(const CString& str, char* arr, int size);

	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();	

	afx_msg LRESULT OnHT3x00AUpdate(WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnBnClickedActivate();	
	afx_msg void OnCbnSelchangeComboTypeOfSampler();
	afx_msg void OnCbnSelchangeComboSyringeType();
	afx_msg void OnBnClickedAlarm();
	afx_msg void OnBnClickedReset();
	afx_msg void OnCbnSelchangeComboError();
};
