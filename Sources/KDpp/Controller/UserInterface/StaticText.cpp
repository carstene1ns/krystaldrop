#include "StaticText.h"

#include "../../Video/Font.h"
#include "../WidgetController.h"

KD_StaticText::KD_StaticText(int x, int y, string text) : KD_Widget()
{
	xWidget = x;
	yWidget = y;
	widthWidget = 0;
	heightWidget = 0;

	font=0;
	
	this->text = text;

	fontName = "statictext_font";

	align = KD_ALIGN_LEFT;
}

KD_StaticText::~KD_StaticText()
{
	DeleteStaticText();
}

void KD_StaticText::DeleteStaticText()
{
	if (font)
		ownerController->ReleaseResource(fontName);
}

void KD_StaticText::SetText(const string &text)
{
	this->text = text;
}

void KD_StaticText::SetFont(const string &fontName)
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

void KD_StaticText::SetAlignment(char align)
{
	this->align = align;
}

void KD_StaticText::DisplayWidget(int alpha)
{
	alpha = alpha*alphaWidget/255;

	if (font == 0)
	{
		font = (KD_Font*) ownerController->GetResource(fontName);
	}

	switch (align)
	{
	case KD_ALIGN_LEFT:
		font->xyalphaprintf(alpha, xWidget, yWidget, (char *)text.c_str());
		break;
	case KD_ALIGN_CENTER:
		font->xyalphacenteredprintf(alpha, xWidget, yWidget, (char *)text.c_str());
		break;
	case KD_ALIGN_RIGHT:
		font->xyalpharightprintf(alpha, xWidget, yWidget, (char *)text.c_str());
		break;
	}
}
