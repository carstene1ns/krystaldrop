#pragma once


// CAskForSaveDialog dialog

class CAskForSaveDialog : public CDialog
{
	DECLARE_DYNAMIC(CAskForSaveDialog)

public:
	CAskForSaveDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAskForSaveDialog();

// Dialog Data
	enum { IDD = IDD_ASKFORSAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
