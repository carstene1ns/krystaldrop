// AskForSaveDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AccWinEditor.h"
#include "AskForSaveDialog.h"


// CAskForSaveDialog dialog

IMPLEMENT_DYNAMIC(CAskForSaveDialog, CDialog)
CAskForSaveDialog::CAskForSaveDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAskForSaveDialog::IDD, pParent)
{
}

CAskForSaveDialog::~CAskForSaveDialog()
{
}

void CAskForSaveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAskForSaveDialog, CDialog)
END_MESSAGE_MAP()


// CAskForSaveDialog message handlers
