// AccWinEditorDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "../../util/direct.h"

// CAccWinEditorDlg dialog
class CAccWinEditorDlg : public CDialog
{
// Construction
public:
	CAccWinEditorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ACCWINEDITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeList1();
	// List of files used contained in the Acc File
	CListBox fileList;

	TACCEditMem *workingFile;

	// has the working file been modified?
	bool isModified;

	CString filePath;
	CString fileName;

	void UpdateFileList();
	void SaveDialog();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
