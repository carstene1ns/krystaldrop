// AccWinEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AccWinEditor.h"
#include "AccWinEditorDlg.h"
#include "AskForSaveDialog.h"

#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAccWinEditorDlg dialog



CAccWinEditorDlg::CAccWinEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccWinEditorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAccWinEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, fileList);
}

BEGIN_MESSAGE_MAP(CAccWinEditorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
END_MESSAGE_MAP()


// CAccWinEditorDlg message handlers

BOOL CAccWinEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	workingFile = new TACCEditMem();
	filePath = "Default";
	fileName = "Default";
	
	isModified = false;

	SetWindowText("AccWinEditor - an editor for .Acc files - "+fileName);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAccWinEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAccWinEditorDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAccWinEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAccWinEditorDlg::OnLbnSelchangeList1()
{
	// TODO: Add your control notification handler code here
}

void CAccWinEditorDlg::UpdateFileList()
{
	assert(workingFile);

	fileList.ResetContent();

	for (int i=0; i<workingFile->NbEntry; i++)
	{
		fileList.AddString( workingFile->EntryName(i) );
	}
}

// Open Button
void CAccWinEditorDlg::OnBnClickedButton2()
{
	if (isModified)
	{
		CAskForSaveDialog ask;
		INT_PTR nResponse = ask.DoModal();
		if (nResponse == IDOK)
		{
			// Here: save!
			SaveDialog();
		}
	}

	// TODO: Add your control notification handler code here
	char /*CChildFrame::*/szFilters[]= "Acc Files (*.acc)|*.acc|All Files (*.*)|*.*||";
	
	// TODO: Add your control notification handler code here
	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg (TRUE, "acc", "*.acc", OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);
	//CFileDialog fileDialog();
	
	INT_PTR nResponse = fileDlg.DoModal();
	
	if( nResponse==IDOK )
	{
		filePath = fileDlg.GetPathName();
   
		// Implement opening and reading file in here.
	
		//Change the window's title to the opened file's title.
		fileName = fileDlg.GetFileTitle ();
   		SetWindowText("AccWinEditor - an editor for .Acc files - "+fileName);

		signed char res= workingFile->LoadACC((char *)(const char *)filePath );
		
		isModified = false;

		UpdateFileList();
			
	}

}

// New
void CAccWinEditorDlg::OnBnClickedButton1()
{
	if (isModified)
	{
		CAskForSaveDialog ask;
		INT_PTR nResponse = ask.DoModal();
		if (nResponse == IDOK)
		{
			// Here: save!
			SaveDialog();
		}
	}

	// TODO: Add your control notification handler code here
	delete workingFile;
	workingFile = new TACCEditMem();
	filePath = "Default";
	fileName = "Default";

	isModified = false;

	SetWindowText("AccWinEditor - an editor for .Acc files - "+fileName);

	UpdateFileList();
}

void CAccWinEditorDlg::SaveDialog()
{
	char szFilters[]= "Acc Files (*.acc)|*.acc|All Files (*.*)|*.*||";
	
	CFileDialog fileDlg (FALSE, "acc", "*.acc", OFN_HIDEREADONLY, szFilters, this);
		
	INT_PTR nResponse = fileDlg.DoModal();
	
	if( nResponse==IDOK )
	{
		filePath = fileDlg.GetPathName();
		fileName = fileDlg.GetFileTitle();
   		SetWindowText("AccWinEditor - an editor for .Acc files - "+fileName);

		signed char res= workingFile->SaveACC((char *)(const char *)filePath );
		
		isModified = false;
	}
}

// Add Files
void CAccWinEditorDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	char szFilters[]= "All Files (*.*)|*.*||";
	
	CFileDialog fileDlg (TRUE, "", "",OFN_ALLOWMULTISELECT, szFilters, this);
		
	INT_PTR nResponse = fileDlg.DoModal();

	if( nResponse==IDOK )
	{
		POSITION pos = fileDlg.GetStartPosition();
		while (pos)
		{
			CString fileToAdd = fileDlg.GetNextPathName(pos);

			signed char err = workingFile->AddEntry ((char *)(const char *)fileToAdd, workingFile->NbEntry);

			fileList.AddString( workingFile->EntryName(workingFile->NbEntry-1) );
			isModified=true;
		}
	}
}

void CAccWinEditorDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	SaveDialog();
}

void CAccWinEditorDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	// Get the indexes of all the selected items.
	int nCount = fileList.GetSelCount();
	CArray<int,int> aryListBoxSel;

	aryListBoxSel.SetSize(nCount);
	fileList.GetSelItems(nCount, aryListBoxSel.GetData()); 

	for (int i=nCount-1; i>=0; i--)
	{
		workingFile->DelEntry(aryListBoxSel[i]);
	}
	UpdateFileList();
}

void CAccWinEditorDlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	int nCount = fileList.GetSelCount();
	CArray<int,int> aryListBoxSel;

	aryListBoxSel.SetSize(nCount);
	fileList.GetSelItems(nCount, aryListBoxSel.GetData()); 

	for (int i=0; i<nCount; i++)
	{
		//workingFile->DelEntry(aryListBoxSel[i]);
		char szFilters[]= "All Files (*.*)|*.*||";
	
		CFileDialog fileDlg (FALSE, "", workingFile->EntryName(aryListBoxSel[i]), OFN_HIDEREADONLY, szFilters, this);
			
		INT_PTR nResponse = fileDlg.DoModal();
		
		if( nResponse==IDOK )
		{
			filePath = fileDlg.GetPathName();
			fileName = fileDlg.GetFileTitle();
   			
			FILE *fpt = fopen((const char *)filePath,"wb");
			fwrite(workingFile->EntryPtr(aryListBoxSel[i]),workingFile->EntryLength(aryListBoxSel[i]),1,fpt);
			fclose(fpt);
			//signed char res= workingFile->SaveACC((char *)(const char *)filePath );
		}
	}
}
