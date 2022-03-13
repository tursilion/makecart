// MakeCartDlg.h : header file
//

#if !defined(AFX_MAKECARTDLG_H__1BEC6301_CC2A_4194_AAB6_0DE4130983ED__INCLUDED_)
#define AFX_MAKECARTDLG_H__1BEC6301_CC2A_4194_AAB6_0DE4130983ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMakeCartDlg dialog

class CMakeCartDlg : public CDialog
{
// Construction
public:
	CMakeCartDlg(CWnd* pParent = NULL);	// standard constructor

	void DumpAsGROM();

// Dialog Data
	//{{AFX_DATA(CMakeCartDlg)
	enum { IDD = IDD_MAKECART_DIALOG };
	CComboBox	m_ctrlGROMIdx;
	CButton	m_ctrlConsole;
	CButton	m_ctrlChars;
	CString	m_Filename;
	CString	m_ProgName;
	BOOL	m_Use2k;
	BOOL	m_UseEA;
	BOOL	m_LoadChars;
	int		m_GROMIdx;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMakeCartDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMakeCartDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBClose();
	afx_msg void OnBrowse();
	afx_msg void OnClose();
	afx_msg void OnBegin();
	afx_msg void OnUseea();
	afx_msg void OnConsole();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAKECARTDLG_H__1BEC6301_CC2A_4194_AAB6_0DE4130983ED__INCLUDED_)
