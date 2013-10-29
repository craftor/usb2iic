// I2C_CTRLDlg.h : header file
//

#if !defined(AFX_I2C_CTRLDLG_H__5A7D0679_A51F_4197_9EFF_F68694AF40B1__INCLUDED_)
#define AFX_I2C_CTRLDLG_H__5A7D0679_A51F_4197_9EFF_F68694AF40B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "usb2iic.h"

/////////////////////////////////////////////////////////////////////////////
// CI2C_CTRLDlg dialog

class CI2C_CTRLDlg : public CDialog
{
// Construction
public:
	CI2C_CTRLDlg(CWnd* pParent = NULL);	// standard constructor

	CUSB2IIC MyIIC;

	CString XferData(BYTE *pData, BYTE cOperation,BYTE cI2C_Addr, BYTE cData_Addr_High,	BYTE cData_Addr_Low, BYTE cLength);

// Dialog Data
	//{{AFX_DATA(CI2C_CTRLDlg)
	enum { IDD = IDD_I2C_CTRL_DIALOG };
	CEdit	m_Print;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CI2C_CTRLDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CI2C_CTRLDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonGo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_I2C_CTRLDLG_H__5A7D0679_A51F_4197_9EFF_F68694AF40B1__INCLUDED_)
