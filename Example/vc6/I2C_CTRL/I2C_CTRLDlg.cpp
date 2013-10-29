// I2C_CTRLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "I2C_CTRL.h"
#include "I2C_CTRLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CI2C_CTRLDlg dialog

CI2C_CTRLDlg::CI2C_CTRLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CI2C_CTRLDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CI2C_CTRLDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CI2C_CTRLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CI2C_CTRLDlg)
	DDX_Control(pDX, IDC_EDIT1, m_Print);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CI2C_CTRLDlg, CDialog)
	//{{AFX_MSG_MAP(CI2C_CTRLDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_GO, OnButtonGo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CI2C_CTRLDlg message handlers

BOOL CI2C_CTRLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	OnButtonGo();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CI2C_CTRLDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CI2C_CTRLDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

CString CI2C_CTRLDlg::XferData(BYTE *pData, BYTE cOperation,BYTE cI2C_Addr, BYTE cData_Addr_High, BYTE cData_Addr_Low, BYTE cLength)
{
	CString str="";
	CString tmp="";
	if ((cOperation & 0x01) == 1)
		str = "RD"; // Read
	else
		str = "WR"; // Write

	tmp.Format(" 0x%02X 0x%02X%02X %d Byte ", cI2C_Addr, cData_Addr_High, cData_Addr_Low, cLength);
	str += tmp;

	if (MyIIC.XferData(MyIIC.pDataOut, cOperation, cI2C_Addr, cData_Addr_High, cData_Addr_Low, cLength))
		str += " OK "; // Success
	else
		str += " FD "; // Failed

	if ((cOperation & 0x01) == 1) // Show Read Data
	{
		str += " <--";
		for(BYTE i=0; i<cLength; i++)
		{
			tmp.Format(" %02X", MyIIC.pDataIn[i]);
			str+=tmp;
		}
	}
	else // Show Write Data
	{
		str += " -->";
		for(BYTE i=0; i<cLength; i++)
		{
			tmp.Format(" %02X", MyIIC.pDataOut[i]);
			str+=tmp;
		}
	}
	
	str+="\r\n";
	return str;
}

void CI2C_CTRLDlg::OnButtonGo() 
{
	CString tmp="";

	// Copy the Codes Below

	// Read
	tmp+=XferData(MyIIC.pDataOut, OP_R2N, 0x51, 0x00, 0x00, 8);


	// Write
	
	MyIIC.pDataOut[0] = 0xC2; /* Load data into write buffer*/
	MyIIC.pDataOut[1] = 0x47; 
	tmp+=XferData(MyIIC.pDataOut, OP_W2N, 0x51, 0x00, 0x00, 2);

	m_Print.SetWindowText(tmp);
}
