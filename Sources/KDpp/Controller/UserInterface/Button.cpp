#include "Button.h"

#include "../WidgetController.h"

#include "../../Video/Image.h"
#include "../../Video/Font.h"
#include "Mouse.h"

KD_Button::KD_Button(int x, int y, int width, int height, string text, int actionNumber) : KD_Widget()
{
	xWidget = x;
	yWidget = y;
	widthWidget = width;
	heightWidget = height;

	for (int i=0; i<3; i++)
	{
		background[i] = 0;
		top_border[i] = 0;
		bottom_border[i] = 0;
		left_border[i] = 0;
		right_border[i] = 0;
		topleft_corner[i] = 0;
		topright_corner[i] = 0;
		bottomleft_corner[i] = 0;
		bottomright_corner[i] = 0;
		font[i]=0;
	}

	this->text = text;

	this->actionNumber = actionNumber;

	buttonState = KD_BUTTON_NORMAL;

	fontName[KD_BUTTON_NORMAL] = "button_font";
	fontName[KD_BUTTON_MOUSE_OVER] = "button_over_font";
	fontName[KD_BUTTON_PRESSED] = "button_pressed_font";
}

KD_Button::~KD_Button()
{
	deleteButton();
}

void KD_Button::SetText(const string &text)
{
	this->text = text;
}

void KD_Button::setFont(const string &fontNormal, const string &fontOver,const string &fontPressed)
{
	if (font[KD_BUTTON_NORMAL]!=0 && fontNormal!="")
	{
		ownerController->ReleaseResource(fontName[KD_BUTTON_NORMAL]);

		fontName[KD_BUTTON_NORMAL] = fontNormal;

		font[KD_BUTTON_NORMAL] = (KD_Font*) ownerController->GetResource(fontNormal);
	}
	else if (fontNormal!="")
	{
		fontName[KD_BUTTON_NORMAL] = fontNormal;
	}

	if (font[KD_BUTTON_MOUSE_OVER]!=0 && fontNormal!="")
	{
		ownerController->ReleaseResource(fontName[KD_BUTTON_MOUSE_OVER]);

		fontName[KD_BUTTON_MOUSE_OVER] = fontOver;

		font[KD_BUTTON_MOUSE_OVER] = (KD_Font*) ownerController->GetResource(fontOver);
	}
	else if (fontOver!="")
	{
		fontName[KD_BUTTON_MOUSE_OVER] = fontOver;
	}

	if (font[KD_BUTTON_PRESSED]!=0 && fontNormal!="")
	{
		ownerController->ReleaseResource(fontName[KD_BUTTON_PRESSED]);

		fontName[KD_BUTTON_PRESSED] = fontPressed;

		font[KD_BUTTON_PRESSED] = (KD_Font*) ownerController->GetResource(fontPressed);
	}
	else if (fontPressed!="")
	{
		fontName[KD_BUTTON_PRESSED] = fontPressed;
	}
}

void KD_Button::deleteButton()
{
	if (background[KD_BUTTON_NORMAL])
		ownerController->ReleaseResource("button_background");
	if (top_border[KD_BUTTON_NORMAL])
		ownerController->ReleaseResource("button_top_border");
	if (bottom_border[KD_BUTTON_NORMAL])
		ownerController->ReleaseResource("button_bottom_border");
	if (left_border[KD_BUTTON_NORMAL])
		ownerController->ReleaseResource("button_left_border");
	if (right_border[KD_BUTTON_NORMAL])
		ownerController->ReleaseResource("button_right_border");
	if (bottomleft_corner[KD_BUTTON_NORMAL])
		ownerController->ReleaseResource("button_bottomleft_corner");
	if (bottomright_corner[KD_BUTTON_NORMAL])
		ownerController->ReleaseResource("button_bottomright_corner");
	if (topleft_corner[KD_BUTTON_NORMAL])
		ownerController->ReleaseResource("button_topleft_corner");
	if (topright_corner[KD_BUTTON_NORMAL])
		ownerController->ReleaseResource("button_topright_corner");
	if (font[KD_BUTTON_NORMAL])
		ownerController->ReleaseResource(fontName[KD_BUTTON_NORMAL]);

	if (background[KD_BUTTON_MOUSE_OVER])
		ownerController->ReleaseResource("button_over_background");
	if (top_border[KD_BUTTON_MOUSE_OVER])
		ownerController->ReleaseResource("button_over_top_border");
	if (bottom_border[KD_BUTTON_MOUSE_OVER])
		ownerController->ReleaseResource("button_over_bottom_border");
	if (left_border[KD_BUTTON_MOUSE_OVER])
		ownerController->ReleaseResource("button_over_left_border");
	if (right_border[KD_BUTTON_MOUSE_OVER])
		ownerController->ReleaseResource("button_over_right_border");
	if (bottomleft_corner[KD_BUTTON_MOUSE_OVER])
		ownerController->ReleaseResource("button_over_bottomleft_corner");
	if (bottomright_corner[KD_BUTTON_MOUSE_OVER])
		ownerController->ReleaseResource("button_over_bottomright_corner");
	if (topleft_corner[KD_BUTTON_MOUSE_OVER])
		ownerController->ReleaseResource("button_over_topleft_corner");
	if (topright_corner[KD_BUTTON_MOUSE_OVER])
		ownerController->ReleaseResource("button_over_topright_corner");
	if (font[KD_BUTTON_MOUSE_OVER])
		ownerController->ReleaseResource(fontName[KD_BUTTON_MOUSE_OVER]);

	if (background[KD_BUTTON_PRESSED])
		ownerController->ReleaseResource("button_pressed_background");
	if (top_border[KD_BUTTON_PRESSED])
		ownerController->ReleaseResource("button_pressed_top_border");
	if (bottom_border[KD_BUTTON_PRESSED])
		ownerController->ReleaseResource("button_pressed_bottom_border");
	if (left_border[KD_BUTTON_PRESSED])
		ownerController->ReleaseResource("button_pressed_left_border");
	if (right_border[KD_BUTTON_PRESSED])
		ownerController->ReleaseResource("button_pressed_right_border");
	if (bottomleft_corner[KD_BUTTON_PRESSED])
		ownerController->ReleaseResource("button_pressed_bottomleft_corner");
	if (bottomright_corner[KD_BUTTON_PRESSED])
		ownerController->ReleaseResource("button_pressed_bottomright_corner");
	if (topleft_corner[KD_BUTTON_PRESSED])
		ownerController->ReleaseResource("button_pressed_topleft_corner");
	if (topright_corner[KD_BUTTON_PRESSED])
		ownerController->ReleaseResource("button_pressed_topright_corner");
	if (font[KD_BUTTON_PRESSED])
		ownerController->ReleaseResource(fontName[KD_BUTTON_PRESSED]);
}

void KD_Button::DisplayWidget(int alpha)
{
	alpha = alpha*alphaWidget/255;

	// if the images are not loaded, load them now!
	if (background[KD_BUTTON_NORMAL] == 0)
	{
		background[KD_BUTTON_NORMAL] = (KD_Image*) ownerController->GetResource("button_background");
		top_border[KD_BUTTON_NORMAL] = (KD_Image*) ownerController->GetResource("button_top_border");
		bottom_border[KD_BUTTON_NORMAL] = (KD_Image*) ownerController->GetResource("button_bottom_border");
		left_border[KD_BUTTON_NORMAL] = (KD_Image*) ownerController->GetResource("button_left_border");
		right_border[KD_BUTTON_NORMAL] = (KD_Image*) ownerController->GetResource("button_right_border");
		bottomleft_corner[KD_BUTTON_NORMAL] = (KD_Image*) ownerController->GetResource("button_bottomleft_corner");
		bottomright_corner[KD_BUTTON_NORMAL] = (KD_Image*) ownerController->GetResource("button_bottomright_corner");
		topleft_corner[KD_BUTTON_NORMAL] = (KD_Image*) ownerController->GetResource("button_topleft_corner");
		topright_corner[KD_BUTTON_NORMAL] = (KD_Image*) ownerController->GetResource("button_topright_corner");
		font[KD_BUTTON_NORMAL] = (KD_Font*) ownerController->GetResource(fontName[KD_BUTTON_NORMAL]);

		background[KD_BUTTON_MOUSE_OVER] = (KD_Image*) ownerController->GetResource("button_over_background");
		top_border[KD_BUTTON_MOUSE_OVER] = (KD_Image*) ownerController->GetResource("button_over_top_border");
		bottom_border[KD_BUTTON_MOUSE_OVER] = (KD_Image*) ownerController->GetResource("button_over_bottom_border");
		left_border[KD_BUTTON_MOUSE_OVER] = (KD_Image*) ownerController->GetResource("button_over_left_border");
		right_border[KD_BUTTON_MOUSE_OVER] = (KD_Image*) ownerController->GetResource("button_over_right_border");
		bottomleft_corner[KD_BUTTON_MOUSE_OVER] = (KD_Image*) ownerController->GetResource("button_over_bottomleft_corner");
		bottomright_corner[KD_BUTTON_MOUSE_OVER] = (KD_Image*) ownerController->GetResource("button_over_bottomright_corner");
		topleft_corner[KD_BUTTON_MOUSE_OVER] = (KD_Image*) ownerController->GetResource("button_over_topleft_corner");
		topright_corner[KD_BUTTON_MOUSE_OVER] = (KD_Image*) ownerController->GetResource("button_over_topright_corner");
		font[KD_BUTTON_MOUSE_OVER] = (KD_Font*) ownerController->GetResource(fontName[KD_BUTTON_MOUSE_OVER]);

		background[KD_BUTTON_PRESSED] = (KD_Image*) ownerController->GetResource("button_pressed_background");
		top_border[KD_BUTTON_PRESSED] = (KD_Image*) ownerController->GetResource("button_pressed_top_border");
		bottom_border[KD_BUTTON_PRESSED] = (KD_Image*) ownerController->GetResource("button_pressed_bottom_border");
		left_border[KD_BUTTON_PRESSED] = (KD_Image*) ownerController->GetResource("button_pressed_left_border");
		right_border[KD_BUTTON_PRESSED] = (KD_Image*) ownerController->GetResource("button_pressed_right_border");
		bottomleft_corner[KD_BUTTON_PRESSED] = (KD_Image*) ownerController->GetResource("button_pressed_bottomleft_corner");
		bottomright_corner[KD_BUTTON_PRESSED] = (KD_Image*) ownerController->GetResource("button_pressed_bottomright_corner");
		topleft_corner[KD_BUTTON_PRESSED] = (KD_Image*) ownerController->GetResource("button_pressed_topleft_corner");
		topright_corner[KD_BUTTON_PRESSED] = (KD_Image*) ownerController->GetResource("button_pressed_topright_corner");
		font[KD_BUTTON_PRESSED] = (KD_Font*) ownerController->GetResource(fontName[KD_BUTTON_PRESSED]);

	}

	// top left corner
	topleft_corner[buttonState]->DisplayAlpha(xWidget,yWidget, alpha);

	// top right corner
	topright_corner[buttonState]->DisplayAlpha(xWidget+widthWidget-topright_corner[buttonState]->getWidth(),yWidget, alpha);

	// bottom left corner
	bottomleft_corner[buttonState]->DisplayAlpha(xWidget,yWidget+heightWidget-bottomleft_corner[buttonState]->getHeight(), alpha);

	// bottom right corner
	bottomright_corner[buttonState]->DisplayAlpha(xWidget+widthWidget-bottomright_corner[buttonState]->getWidth(),yWidget+heightWidget-bottomright_corner[buttonState]->getHeight(), alpha);


	int nbRep, xBase,yBase;

	// top border
	nbRep = (widthWidget - topleft_corner[buttonState]->getWidth() - topright_corner[buttonState]->getWidth()) / top_border[buttonState]->getWidth();
	xBase = xWidget+topleft_corner[buttonState]->getWidth();
	yBase = yWidget;

	for (int i=0; i<nbRep; i++)
	{
		top_border[buttonState]->DisplayAlpha(xBase + top_border[buttonState]->getWidth() * i,yBase, alpha);
	}
	if ((widthWidget - topleft_corner[buttonState]->getWidth() - topright_corner[buttonState]->getWidth()) % top_border[buttonState]->getWidth() != 0)
	{
		top_border[buttonState]->DisplayAlphaPart(xBase + top_border[buttonState]->getWidth()*nbRep,yBase,alpha,0,0,(widthWidget - topleft_corner[buttonState]->getWidth() - topright_corner[buttonState]->getWidth())%top_border[buttonState]->getWidth(),top_border[buttonState]->getHeight());
	}



	// top border
	nbRep = (widthWidget - topleft_corner[buttonState]->getWidth() - topright_corner[buttonState]->getWidth()) / top_border[buttonState]->getWidth();
	xBase = xWidget+topleft_corner[buttonState]->getWidth();
	yBase = yWidget;

	for (int i=0; i<nbRep; i++)
	{
		top_border[buttonState]->DisplayAlpha(xBase + top_border[buttonState]->getWidth() * i,yBase,alpha);
	}
	if ((widthWidget - topleft_corner[buttonState]->getWidth() - topright_corner[buttonState]->getWidth()) % top_border[buttonState]->getWidth() != 0)
	{
		top_border[buttonState]->DisplayAlphaPart(xBase + top_border[buttonState]->getWidth()*nbRep,yBase,alpha,0,0,(widthWidget - topleft_corner[buttonState]->getWidth() - topright_corner[buttonState]->getWidth())%top_border[buttonState]->getWidth(),top_border[buttonState]->getHeight());
	}

	// bottom border
	nbRep = (widthWidget - bottomleft_corner[buttonState]->getWidth() - bottomright_corner[buttonState]->getWidth()) / bottom_border[buttonState]->getWidth();
	xBase = xWidget+bottomleft_corner[buttonState]->getWidth();
	yBase = yWidget + heightWidget - bottom_border[buttonState]->getHeight();

	for (int i=0; i<nbRep; i++)
	{
		bottom_border[buttonState]->DisplayAlpha(xBase + bottom_border[buttonState]->getWidth() * i,yBase,alpha);
	}
	if ((widthWidget - bottomleft_corner[buttonState]->getWidth() - bottomright_corner[buttonState]->getWidth()) % bottom_border[buttonState]->getWidth() != 0)
	{
		bottom_border[buttonState]->DisplayAlphaPart(xBase + bottom_border[buttonState]->getWidth()*nbRep,yBase,alpha,0,0,(widthWidget - bottomleft_corner[buttonState]->getWidth() - bottomright_corner[buttonState]->getWidth())%bottom_border[buttonState]->getWidth(),bottom_border[buttonState]->getHeight());
	}

	// left border
	nbRep = (heightWidget - topleft_corner[buttonState]->getHeight() - bottomleft_corner[buttonState]->getHeight()) / left_border[buttonState]->getHeight();
	xBase = xWidget;
	yBase = yWidget+topleft_corner[buttonState]->getHeight();

	for (int i=0; i<nbRep; i++)
	{
		left_border[buttonState]->DisplayAlpha(xBase ,yBase + left_border[buttonState]->getHeight() * i, alpha);
	}
	if ((heightWidget - topleft_corner[buttonState]->getHeight() - bottomleft_corner[buttonState]->getHeight()) % left_border[buttonState]->getHeight() != 0)
	{
		left_border[buttonState]->DisplayAlphaPart(xBase,yBase + left_border[buttonState]->getHeight()*nbRep,alpha,0,0,left_border[buttonState]->getWidth(),(heightWidget - topleft_corner[buttonState]->getHeight() - bottomleft_corner[buttonState]->getHeight())%left_border[buttonState]->getHeight());
	}

	// right border
	nbRep = (heightWidget - topright_corner[buttonState]->getHeight() - bottomright_corner[buttonState]->getHeight()) / right_border[buttonState]->getHeight();
	xBase = xWidget+widthWidget - left_border[buttonState]->getWidth();
	yBase = yWidget+topleft_corner[buttonState]->getHeight();

	for (int i=0; i<nbRep; i++)
	{
		right_border[buttonState]->DisplayAlpha(xBase ,yBase + right_border[buttonState]->getHeight() * i, alpha);
	}
	if ((heightWidget - topright_corner[buttonState]->getHeight() - bottomright_corner[buttonState]->getHeight()) % right_border[buttonState]->getHeight() != 0)
	{
		right_border[buttonState]->DisplayAlphaPart(xBase,yBase + right_border[buttonState]->getHeight()*nbRep,alpha,0,0,right_border[buttonState]->getWidth(),(heightWidget - topright_corner[buttonState]->getHeight() - bottomright_corner[buttonState]->getHeight())%right_border[buttonState]->getHeight());
	}

	// Background
	int xRep,yRep;

	xBase = xWidget+topleft_corner[buttonState]->getWidth();
	yBase = yWidget+topleft_corner[buttonState]->getHeight();

	xRep = (widthWidget - topleft_corner[buttonState]->getWidth() - topright_corner[buttonState]->getWidth()) / background[buttonState]->getWidth();
	yRep = (heightWidget - topleft_corner[buttonState]->getHeight() - bottomleft_corner[buttonState]->getHeight()) / background[buttonState]->getHeight();

	for (int j=0; j<yRep; j++)
		for (int i=0; i<xRep; i++)
			background[buttonState]->DisplayAlpha(xBase + background[buttonState]->getWidth()*i,yBase + background[buttonState]->getHeight()*j,alpha);

	if ((widthWidget - topleft_corner[buttonState]->getWidth() - topright_corner[buttonState]->getWidth()) % background[buttonState]->getWidth() != 0)
	{
		for (int j=0; j<yRep; j++)
			background[buttonState]->DisplayAlphaPart(xBase + background[buttonState]->getWidth()*xRep,yBase + background[buttonState]->getHeight()*j,alpha,0,0,(widthWidget - topleft_corner[buttonState]->getWidth() - topright_corner[buttonState]->getWidth())%background[buttonState]->getWidth(),background[buttonState]->getHeight());
	}

	if ((heightWidget - topleft_corner[buttonState]->getHeight() - bottomleft_corner[buttonState]->getHeight()) % background[buttonState]->getHeight() != 0)
	{
		for (int i=0; i<xRep; i++)
			background[buttonState]->DisplayAlphaPart(xBase + background[buttonState]->getWidth()*i,yBase + background[buttonState]->getHeight()*yRep,alpha,0,0,background[buttonState]->getWidth(),(heightWidget - topleft_corner[buttonState]->getHeight() - bottomleft_corner[buttonState]->getHeight())%background[buttonState]->getHeight());

		if ((widthWidget - topleft_corner[buttonState]->getWidth() - topright_corner[buttonState]->getWidth()) % background[buttonState]->getWidth() != 0)
			background[buttonState]->DisplayAlphaPart(xBase + background[buttonState]->getWidth()*xRep,yBase + background[buttonState]->getHeight()*yRep,255,0,0,(widthWidget - topleft_corner[buttonState]->getWidth() - topright_corner[buttonState]->getWidth())%background[buttonState]->getWidth(),(heightWidget - topleft_corner[buttonState]->getHeight() - bottomleft_corner[buttonState]->getHeight())%background[buttonState]->getHeight());		
	}

	font[buttonState]->xyalphacenteredprintf(alpha, xWidget + widthWidget/2, yWidget + heightWidget/2 + font[buttonState]->getFontHeight()/2, (char *)text.c_str());

}

bool KD_Button::onMouseEnter()
{
	int xLastClick, yLastClick;
	KD_Mouse::getMouse()->getLastClickCoordinates(xLastClick, yLastClick);
	bool leftButton = KD_Mouse::getMouse()->getButtonState(1);
	if (isInside(xLastClick, yLastClick) && leftButton==true)
		buttonState = KD_BUTTON_PRESSED;
	else
		buttonState = KD_BUTTON_MOUSE_OVER;
	return true;
}

bool KD_Button::onMouseLeave()
{
	buttonState = KD_BUTTON_NORMAL;
	return true;
}

bool KD_Button::onWidgetMouseButtonDown(int button, int x, int y)
{
	if (button==1 || button == KD_DOUBLECLICK)
	{
		buttonState = KD_BUTTON_PRESSED;
	}
	return true;
}

bool KD_Button::onWidgetMouseButtonUp(int button, int x, int y, int xLastClick, int yLastClick)
{
	if (button == 1 || button == KD_DOUBLECLICK)
	{
		buttonState = KD_BUTTON_MOUSE_OVER;
	}
	return true;
}

bool KD_Button::onWidgetClick(int button)
{
	if (button == 1 || button == KD_DOUBLECLICK)
	{
		buttonState = KD_BUTTON_MOUSE_OVER;
		ownerController->ProcessEvent(actionNumber);
		return true;
	}
	return false;
}
