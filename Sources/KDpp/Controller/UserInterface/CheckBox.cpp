#include "CheckBox.h"

#include "../WidgetController.h"

#include "../../Video/Image.h"
#include "../../Video/Font.h"
#include "Mouse.h"

KD_CheckBox::KD_CheckBox(int x, int y, string text) : KD_Widget()
{
	xWidget = x;
	yWidget = y;
	widthWidget = 0;
	heightWidget = 0;

	for (int i=0; i<4; i++)
	{
		checkbox[i] = 0;
	}

	font=0;

	this->text = text;

	actionOnCheck = 0;
	actionOnUncheck = 0;

	checkboxState = KD_CHECKBOX_NORMAL;

	fontName = "checkbox_font";
}

KD_CheckBox::~KD_CheckBox()
{
	deleteCheckBox();
}

void KD_CheckBox::deleteCheckBox()
{
	if (checkbox[KD_CHECKBOX_NORMAL])
	{
		ownerController->ReleaseResource("checkbox_normal");
		checkbox[KD_CHECKBOX_NORMAL]=0;
	}
	if (checkbox[KD_CHECKBOX_CHECKED])
	{
		ownerController->ReleaseResource("checkbox_checked_normal");
		checkbox[KD_CHECKBOX_CHECKED]=0;
	}
	if (checkbox[KD_CHECKBOX_OVER])
	{
		ownerController->ReleaseResource("checkbox_over");
		checkbox[KD_CHECKBOX_OVER]=0;
	}
	if (checkbox[KD_CHECKBOX_OVER_CHECKED])
	{
		ownerController->ReleaseResource("checkbox_checked_over");
		checkbox[KD_CHECKBOX_OVER_CHECKED]=0;
	}

	if (font)
	{
		ownerController->ReleaseResource(fontName);
		font=0;
	}
}

void KD_CheckBox::SetText(const string &text)
{
	this->text = text;
}

void KD_CheckBox::SetFont(const string &fontName)
{
	if (font!=0 && fontName!="")
	{
		ownerController->ReleaseResource(this->fontName);

		this->fontName = fontName;

		font = (KD_Font*) ownerController->GetResource(fontName);
	}
	else if (fontName!="")
	{
		this->fontName = fontName;
	}
}

void KD_CheckBox::DisplayWidget(int alpha)
{
	alpha = alpha*alphaWidget/255;

	if (checkbox[KD_CHECKBOX_NORMAL] == 0)
	{
		// Warning these resources assume that all the 4 images have the same size!
		checkbox[KD_CHECKBOX_NORMAL] = (KD_Image*) ownerController->GetResource("checkbox_normal");
		checkbox[KD_CHECKBOX_CHECKED] = (KD_Image*) ownerController->GetResource("checkbox_checked_normal");
		checkbox[KD_CHECKBOX_OVER] = (KD_Image*) ownerController->GetResource("checkbox_over");
		checkbox[KD_CHECKBOX_OVER_CHECKED] = (KD_Image*) ownerController->GetResource("checkbox_checked_over");
		widthWidget = checkbox[KD_CHECKBOX_NORMAL]->GetWidth();
		heightWidget = checkbox[KD_CHECKBOX_NORMAL]->GetHeight();

		font = (KD_Font*) ownerController->GetResource(fontName);
	}

	checkbox[checkboxState]->DisplayAlpha(xWidget, yWidget, alpha);

	font->xyalphaprintf(alpha, xWidget+checkbox[checkboxState]->GetWidth(), yWidget+checkbox[checkboxState]->GetHeight(), (char *)text.c_str());	
}

bool KD_CheckBox::onMouseEnter()
{
	if (checkboxState == KD_CHECKBOX_NORMAL)
		checkboxState = KD_CHECKBOX_OVER;
	else if (checkboxState == KD_CHECKBOX_CHECKED)
		checkboxState = KD_CHECKBOX_OVER_CHECKED;
	
	return true;
}

bool KD_CheckBox::onMouseLeave()
{
	if (checkboxState == KD_CHECKBOX_OVER)
		checkboxState = KD_CHECKBOX_NORMAL;
	else if (checkboxState == KD_CHECKBOX_OVER_CHECKED)
		checkboxState = KD_CHECKBOX_CHECKED;

	return true;
}

bool KD_CheckBox::onWidGetMouseButtonDown(int button, int x, int y)
{
	return true;
}

bool KD_CheckBox::onWidGetMouseButtonUp(int button, int x, int y, int xLastClick, int yLastClick)
{
	return true;
}

bool KD_CheckBox::onWidgetClick(int button)
{
	if (button == 1 || button == KD_DOUBLECLICK)
	{
		if (checkboxState == KD_CHECKBOX_OVER)
		{
			checkboxState = KD_CHECKBOX_OVER_CHECKED;
			ownerController->ProcessEvent(actionOnCheck);
		}
		else if (checkboxState == KD_CHECKBOX_OVER_CHECKED)
		{
			checkboxState = KD_CHECKBOX_OVER;
			ownerController->ProcessEvent(actionOnUncheck);
		}
		return true;
	}
	return false;
}

bool KD_CheckBox::isChecked() const
{
	if (checkboxState == KD_CHECKBOX_OVER || checkboxState == KD_CHECKBOX_NORMAL)
		return false;
	else
		return true;
}

void KD_CheckBox::check()
{
	if (checkboxState == KD_CHECKBOX_OVER)
	{
		checkboxState = KD_CHECKBOX_OVER_CHECKED;
	}
	else if (checkboxState == KD_CHECKBOX_NORMAL)
	{
		checkboxState = KD_CHECKBOX_CHECKED;
	}
}
void KD_CheckBox::uncheck()
{
	if (checkboxState == KD_CHECKBOX_OVER_CHECKED)
	{
		checkboxState = KD_CHECKBOX_OVER;
	}
	else if (checkboxState == KD_CHECKBOX_CHECKED)
	{
		checkboxState = KD_CHECKBOX_NORMAL;
	}
}

void KD_CheckBox::setOnCheck(int action)
{
	actionOnCheck = action;
}

void KD_CheckBox::setOnUncheck(int action)
{
	actionOnUncheck = action;
}
