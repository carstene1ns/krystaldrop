#include "EditField.h"

#include "../WidgetController.h"

#include "../../Video/Image.h"
#include "../../Video/Font.h"
#include "Mouse.h"
#include "Keyboard.h"

#include <assert.h>

KD_EditField::KD_EditField(int x, int y, int width, int height) : KD_Widget(), KD_KeyboardCursor()
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
	}

	font=0;

	this->text = "";

	actionOnReturn = 0;
	actionOnUnselect = 0;

	editfieldState = KD_EDITFIELD_NORMAL;

	cursorPosition=0;
	cursorPixelPosition=0;

	fontName = "editfield_font";

	maxChars = 100;
}

KD_EditField::~KD_EditField()
{
	deleteEditField();
}

void KD_EditField::deleteEditField()
{
	if (background[KD_EDITFIELD_NORMAL])
		ownerController->ReleaseResource("editfield_background");
	if (top_border[KD_EDITFIELD_NORMAL])
		ownerController->ReleaseResource("editfield_top_border");
	if (bottom_border[KD_EDITFIELD_NORMAL])
		ownerController->ReleaseResource("editfield_bottom_border");
	if (left_border[KD_EDITFIELD_NORMAL])
		ownerController->ReleaseResource("editfield_left_border");
	if (right_border[KD_EDITFIELD_NORMAL])
		ownerController->ReleaseResource("editfield_right_border");
	if (bottomleft_corner[KD_EDITFIELD_NORMAL])
		ownerController->ReleaseResource("editfield_bottomleft_corner");
	if (bottomright_corner[KD_EDITFIELD_NORMAL])
		ownerController->ReleaseResource("editfield_bottomright_corner");
	if (topleft_corner[KD_EDITFIELD_NORMAL])
		ownerController->ReleaseResource("editfield_topleft_corner");
	if (topright_corner[KD_EDITFIELD_NORMAL])
		ownerController->ReleaseResource("editfield_topright_corner");
	
	if (background[KD_EDITFIELD_OVER])
		ownerController->ReleaseResource("editfield_over_background");
	if (top_border[KD_EDITFIELD_OVER])
		ownerController->ReleaseResource("editfield_over_top_border");
	if (bottom_border[KD_EDITFIELD_OVER])
		ownerController->ReleaseResource("editfield_over_bottom_border");
	if (left_border[KD_EDITFIELD_OVER])
		ownerController->ReleaseResource("editfield_over_left_border");
	if (right_border[KD_EDITFIELD_OVER])
		ownerController->ReleaseResource("editfield_over_right_border");
	if (bottomleft_corner[KD_EDITFIELD_OVER])
		ownerController->ReleaseResource("editfield_over_bottomleft_corner");
	if (bottomright_corner[KD_EDITFIELD_OVER])
		ownerController->ReleaseResource("editfield_over_bottomright_corner");
	if (topleft_corner[KD_EDITFIELD_OVER])
		ownerController->ReleaseResource("editfield_over_topleft_corner");
	if (topright_corner[KD_EDITFIELD_OVER])
		ownerController->ReleaseResource("editfield_over_topright_corner");
	
	if (background[KD_EDITFIELD_SELECTED])
		ownerController->ReleaseResource("editfield_selected_background");
	if (top_border[KD_EDITFIELD_SELECTED])
		ownerController->ReleaseResource("editfield_selected_top_border");
	if (bottom_border[KD_EDITFIELD_SELECTED])
		ownerController->ReleaseResource("editfield_selected_bottom_border");
	if (left_border[KD_EDITFIELD_SELECTED])
		ownerController->ReleaseResource("editfield_selected_left_border");
	if (right_border[KD_EDITFIELD_SELECTED])
		ownerController->ReleaseResource("editfield_selected_right_border");
	if (bottomleft_corner[KD_EDITFIELD_SELECTED])
		ownerController->ReleaseResource("editfield_selected_bottomleft_corner");
	if (bottomright_corner[KD_EDITFIELD_SELECTED])
		ownerController->ReleaseResource("editfield_selected_bottomright_corner");
	if (topleft_corner[KD_EDITFIELD_SELECTED])
		ownerController->ReleaseResource("editfield_selected_topleft_corner");
	if (topright_corner[KD_EDITFIELD_SELECTED])
		ownerController->ReleaseResource("editfield_selected_topright_corner");

    if (font)
		ownerController->ReleaseResource(fontName);

	if (cursor)
		ownerController->ReleaseResource("editfield_cursor");
}

void KD_EditField::SetText(const string &text)
{
	this->text = text;
	if (cursorPosition > (signed)text.size())
		cursorPosition = (signed)text.size();
}

string KD_EditField::GetText()
{
	return text;
}

void KD_EditField::SetFont(const string &fontName)
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

void KD_EditField::DisplayWidget(int alpha)
{
	alpha = alpha*alphaWidget/255;

	// if the images are not loaded, load them now!
	if (background[KD_EDITFIELD_NORMAL] == 0)
	{
		background[KD_EDITFIELD_NORMAL] = (KD_Image*) ownerController->GetResource("editfield_background");
		top_border[KD_EDITFIELD_NORMAL] = (KD_Image*) ownerController->GetResource("editfield_top_border");
		bottom_border[KD_EDITFIELD_NORMAL] = (KD_Image*) ownerController->GetResource("editfield_bottom_border");
		left_border[KD_EDITFIELD_NORMAL] = (KD_Image*) ownerController->GetResource("editfield_left_border");
		right_border[KD_EDITFIELD_NORMAL] = (KD_Image*) ownerController->GetResource("editfield_right_border");
		bottomleft_corner[KD_EDITFIELD_NORMAL] = (KD_Image*) ownerController->GetResource("editfield_bottomleft_corner");
		bottomright_corner[KD_EDITFIELD_NORMAL] = (KD_Image*) ownerController->GetResource("editfield_bottomright_corner");
		topleft_corner[KD_EDITFIELD_NORMAL] = (KD_Image*) ownerController->GetResource("editfield_topleft_corner");
		topright_corner[KD_EDITFIELD_NORMAL] = (KD_Image*) ownerController->GetResource("editfield_topright_corner");
	
		background[KD_EDITFIELD_OVER] = (KD_Image*) ownerController->GetResource("editfield_over_background");
		top_border[KD_EDITFIELD_OVER] = (KD_Image*) ownerController->GetResource("editfield_over_top_border");
		bottom_border[KD_EDITFIELD_OVER] = (KD_Image*) ownerController->GetResource("editfield_over_bottom_border");
		left_border[KD_EDITFIELD_OVER] = (KD_Image*) ownerController->GetResource("editfield_over_left_border");
		right_border[KD_EDITFIELD_OVER] = (KD_Image*) ownerController->GetResource("editfield_over_right_border");
		bottomleft_corner[KD_EDITFIELD_OVER] = (KD_Image*) ownerController->GetResource("editfield_over_bottomleft_corner");
		bottomright_corner[KD_EDITFIELD_OVER] = (KD_Image*) ownerController->GetResource("editfield_over_bottomright_corner");
		topleft_corner[KD_EDITFIELD_OVER] = (KD_Image*) ownerController->GetResource("editfield_over_topleft_corner");
		topright_corner[KD_EDITFIELD_OVER] = (KD_Image*) ownerController->GetResource("editfield_over_topright_corner");

		background[KD_EDITFIELD_SELECTED] = (KD_Image*) ownerController->GetResource("editfield_selected_background");
		top_border[KD_EDITFIELD_SELECTED] = (KD_Image*) ownerController->GetResource("editfield_selected_top_border");
		bottom_border[KD_EDITFIELD_SELECTED] = (KD_Image*) ownerController->GetResource("editfield_selected_bottom_border");
		left_border[KD_EDITFIELD_SELECTED] = (KD_Image*) ownerController->GetResource("editfield_selected_left_border");
		right_border[KD_EDITFIELD_SELECTED] = (KD_Image*) ownerController->GetResource("editfield_selected_right_border");
		bottomleft_corner[KD_EDITFIELD_SELECTED] = (KD_Image*) ownerController->GetResource("editfield_selected_bottomleft_corner");
		bottomright_corner[KD_EDITFIELD_SELECTED] = (KD_Image*) ownerController->GetResource("editfield_selected_bottomright_corner");
		topleft_corner[KD_EDITFIELD_SELECTED] = (KD_Image*) ownerController->GetResource("editfield_selected_topleft_corner");
		topright_corner[KD_EDITFIELD_SELECTED] = (KD_Image*) ownerController->GetResource("editfield_selected_topright_corner");

		font = (KD_Font*) ownerController->GetResource(fontName);

		cursor = (KD_Image*) ownerController->GetResource("editfield_cursor");

		cursorPixelPosition = topleft_corner[KD_EDITFIELD_SELECTED]->GetWidth();
	}

	// top left corner
	topleft_corner[editfieldState]->DisplayAlpha(xWidget,yWidget, alpha);

	// top right corner
	topright_corner[editfieldState]->DisplayAlpha(xWidget+widthWidget-topright_corner[editfieldState]->GetWidth(),yWidget, alpha);

	// bottom left corner
	bottomleft_corner[editfieldState]->DisplayAlpha(xWidget,yWidget+heightWidget-bottomleft_corner[editfieldState]->GetHeight(), alpha);

	// bottom right corner
	bottomright_corner[editfieldState]->DisplayAlpha(xWidget+widthWidget-bottomright_corner[editfieldState]->GetWidth(),yWidget+heightWidget-bottomright_corner[editfieldState]->GetHeight(), alpha);


	int nbRep, xBase,yBase;

	// top border
	nbRep = (widthWidget - topleft_corner[editfieldState]->GetWidth() - topright_corner[editfieldState]->GetWidth()) / top_border[editfieldState]->GetWidth();
	xBase = xWidget+topleft_corner[editfieldState]->GetWidth();
	yBase = yWidget;

	for (int i=0; i<nbRep; i++)
	{
		top_border[editfieldState]->DisplayAlpha(xBase + top_border[editfieldState]->GetWidth() * i,yBase, alpha);
	}
	if ((widthWidget - topleft_corner[editfieldState]->GetWidth() - topright_corner[editfieldState]->GetWidth()) % top_border[editfieldState]->GetWidth() != 0)
	{
		top_border[editfieldState]->DisplayAlphaPart(xBase + top_border[editfieldState]->GetWidth()*nbRep,yBase,alpha,0,0,(widthWidget - topleft_corner[editfieldState]->GetWidth() - topright_corner[editfieldState]->GetWidth())%top_border[editfieldState]->GetWidth(),top_border[editfieldState]->GetHeight());
	}



	// top border
	nbRep = (widthWidget - topleft_corner[editfieldState]->GetWidth() - topright_corner[editfieldState]->GetWidth()) / top_border[editfieldState]->GetWidth();
	xBase = xWidget+topleft_corner[editfieldState]->GetWidth();
	yBase = yWidget;

	for (int i=0; i<nbRep; i++)
	{
		top_border[editfieldState]->DisplayAlpha(xBase + top_border[editfieldState]->GetWidth() * i,yBase,alpha);
	}
	if ((widthWidget - topleft_corner[editfieldState]->GetWidth() - topright_corner[editfieldState]->GetWidth()) % top_border[editfieldState]->GetWidth() != 0)
	{
		top_border[editfieldState]->DisplayAlphaPart(xBase + top_border[editfieldState]->GetWidth()*nbRep,yBase,alpha,0,0,(widthWidget - topleft_corner[editfieldState]->GetWidth() - topright_corner[editfieldState]->GetWidth())%top_border[editfieldState]->GetWidth(),top_border[editfieldState]->GetHeight());
	}

	// bottom border
	nbRep = (widthWidget - bottomleft_corner[editfieldState]->GetWidth() - bottomright_corner[editfieldState]->GetWidth()) / bottom_border[editfieldState]->GetWidth();
	xBase = xWidget+bottomleft_corner[editfieldState]->GetWidth();
	yBase = yWidget + heightWidget - bottom_border[editfieldState]->GetHeight();

	for (int i=0; i<nbRep; i++)
	{
		bottom_border[editfieldState]->DisplayAlpha(xBase + bottom_border[editfieldState]->GetWidth() * i,yBase,alpha);
	}
	if ((widthWidget - bottomleft_corner[editfieldState]->GetWidth() - bottomright_corner[editfieldState]->GetWidth()) % bottom_border[editfieldState]->GetWidth() != 0)
	{
		bottom_border[editfieldState]->DisplayAlphaPart(xBase + bottom_border[editfieldState]->GetWidth()*nbRep,yBase,alpha,0,0,(widthWidget - bottomleft_corner[editfieldState]->GetWidth() - bottomright_corner[editfieldState]->GetWidth())%bottom_border[editfieldState]->GetWidth(),bottom_border[editfieldState]->GetHeight());
	}

	// left border
	nbRep = (heightWidget - topleft_corner[editfieldState]->GetHeight() - bottomleft_corner[editfieldState]->GetHeight()) / left_border[editfieldState]->GetHeight();
	xBase = xWidget;
	yBase = yWidget+topleft_corner[editfieldState]->GetHeight();

	for (int i=0; i<nbRep; i++)
	{
		left_border[editfieldState]->DisplayAlpha(xBase ,yBase + left_border[editfieldState]->GetHeight() * i, alpha);
	}
	if ((heightWidget - topleft_corner[editfieldState]->GetHeight() - bottomleft_corner[editfieldState]->GetHeight()) % left_border[editfieldState]->GetHeight() != 0)
	{
		left_border[editfieldState]->DisplayAlphaPart(xBase,yBase + left_border[editfieldState]->GetHeight()*nbRep,alpha,0,0,left_border[editfieldState]->GetWidth(),(heightWidget - topleft_corner[editfieldState]->GetHeight() - bottomleft_corner[editfieldState]->GetHeight())%left_border[editfieldState]->GetHeight());
	}

	// right border
	nbRep = (heightWidget - topright_corner[editfieldState]->GetHeight() - bottomright_corner[editfieldState]->GetHeight()) / right_border[editfieldState]->GetHeight();
	xBase = xWidget+widthWidget - left_border[editfieldState]->GetWidth();
	yBase = yWidget+topleft_corner[editfieldState]->GetHeight();

	for (int i=0; i<nbRep; i++)
	{
		right_border[editfieldState]->DisplayAlpha(xBase ,yBase + right_border[editfieldState]->GetHeight() * i, alpha);
	}
	if ((heightWidget - topright_corner[editfieldState]->GetHeight() - bottomright_corner[editfieldState]->GetHeight()) % right_border[editfieldState]->GetHeight() != 0)
	{
		right_border[editfieldState]->DisplayAlphaPart(xBase,yBase + right_border[editfieldState]->GetHeight()*nbRep,alpha,0,0,right_border[editfieldState]->GetWidth(),(heightWidget - topright_corner[editfieldState]->GetHeight() - bottomright_corner[editfieldState]->GetHeight())%right_border[editfieldState]->GetHeight());
	}

	// Background
	int xRep,yRep;

	xBase = xWidget+topleft_corner[editfieldState]->GetWidth();
	yBase = yWidget+topleft_corner[editfieldState]->GetHeight();

	xRep = (widthWidget - topleft_corner[editfieldState]->GetWidth() - topright_corner[editfieldState]->GetWidth()) / background[editfieldState]->GetWidth();
	yRep = (heightWidget - topleft_corner[editfieldState]->GetHeight() - bottomleft_corner[editfieldState]->GetHeight()) / background[editfieldState]->GetHeight();

	for (int j=0; j<yRep; j++)
		for (int i=0; i<xRep; i++)
			background[editfieldState]->DisplayAlpha(xBase + background[editfieldState]->GetWidth()*i,yBase + background[editfieldState]->GetHeight()*j,alpha);

	if ((widthWidget - topleft_corner[editfieldState]->GetWidth() - topright_corner[editfieldState]->GetWidth()) % background[editfieldState]->GetWidth() != 0)
	{
		for (int j=0; j<yRep; j++)
			background[editfieldState]->DisplayAlphaPart(xBase + background[editfieldState]->GetWidth()*xRep,yBase + background[editfieldState]->GetHeight()*j,alpha,0,0,(widthWidget - topleft_corner[editfieldState]->GetWidth() - topright_corner[editfieldState]->GetWidth())%background[editfieldState]->GetWidth(),background[editfieldState]->GetHeight());
	}

	if ((heightWidget - topleft_corner[editfieldState]->GetHeight() - bottomleft_corner[editfieldState]->GetHeight()) % background[editfieldState]->GetHeight() != 0)
	{
		for (int i=0; i<xRep; i++)
			background[editfieldState]->DisplayAlphaPart(xBase + background[editfieldState]->GetWidth()*i,yBase + background[editfieldState]->GetHeight()*yRep,alpha,0,0,background[editfieldState]->GetWidth(),(heightWidget - topleft_corner[editfieldState]->GetHeight() - bottomleft_corner[editfieldState]->GetHeight())%background[editfieldState]->GetHeight());

		if ((widthWidget - topleft_corner[editfieldState]->GetWidth() - topright_corner[editfieldState]->GetWidth()) % background[editfieldState]->GetWidth() != 0)
			background[editfieldState]->DisplayAlphaPart(xBase + background[editfieldState]->GetWidth()*xRep,yBase + background[editfieldState]->GetHeight()*yRep,255,0,0,(widthWidget - topleft_corner[editfieldState]->GetWidth() - topright_corner[editfieldState]->GetWidth())%background[editfieldState]->GetWidth(),(heightWidget - topleft_corner[editfieldState]->GetHeight() - bottomleft_corner[editfieldState]->GetHeight())%background[editfieldState]->GetHeight());		
	}

	font->xyalphaprintf(alpha, xWidget + topleft_corner[editfieldState]->GetWidth(), yWidget + heightWidget/2 + font->GetFontHeight()/2, (char *)text.c_str());

	if (editfieldState == KD_EDITFIELD_SELECTED)
		cursor->DisplayAlpha(xBase + cursorPixelPosition - cursor->GetWidth()/2, yWidget + heightWidget/2 - cursor->GetHeight()/2, alpha);
}

bool KD_EditField::onMouseEnter()
{
	if (editfieldState == KD_EDITFIELD_NORMAL)
		editfieldState = KD_EDITFIELD_OVER;

	return true;
}

bool KD_EditField::onMouseLeave()
{
	if (editfieldState == KD_EDITFIELD_OVER)
		editfieldState = KD_EDITFIELD_NORMAL;
	
	return true;
}

bool KD_EditField::onWidGetMouseButtonDown(int button, int x, int y)
{
	GrabKeyboardCursor();

	int xPos = xWidget + topleft_corner[editfieldState]->GetWidth();

	if (x<xPos)
	{
		cursorPosition = 0;
	}
	else
	{
		char ch[2];
		ch[1]=0;
		cursorPosition=0;
		for (unsigned int i=0; i<text.size(); i++)
		{
			cursorPosition++;
			ch[0] = text[i];	
			xPos += font->ComputeLength(ch);
			if (xPos>=x)
				break;
		}
	}

	string substr = text.substr(0,cursorPosition);
	cursorPixelPosition = topleft_corner[KD_EDITFIELD_SELECTED]->GetWidth()+font->ComputeLength((char *)substr.c_str());

	editfieldState = KD_EDITFIELD_SELECTED;

	return true;
}

bool KD_EditField::onWidGetMouseButtonUp(int button, int x, int y, int xLastClick, int yLastClick)
{
	return true;
}

bool KD_EditField::onWidgetClick(int button)
{
	return false;
}

void KD_EditField::ReleaseCursor()
{
	int x,y;
	KD_Mouse::GetMouse()->GetCoordinates(x,y);
	if (isInside(x,y))
		editfieldState = KD_EDITFIELD_OVER;
	else
		editfieldState = KD_EDITFIELD_NORMAL;
}

/**
	Method called when a key is pressed
*/
bool KD_EditField::onWidgetKeyDown(SDLKey key)
{
	if (editfieldState != KD_EDITFIELD_SELECTED)
		return false;

	switch (key)
	{
		case SDLK_LEFT:
		{
			if (cursorPosition>0)
				cursorPosition--;
			string substr = text.substr(0,cursorPosition);
			cursorPixelPosition = font->ComputeLength((char *)substr.c_str());
			return true;
		}
		case SDLK_RIGHT:
		{
			if (cursorPosition<(signed)text.size())
				cursorPosition++;
			string substr = text.substr(0,cursorPosition);
			cursorPixelPosition = font->ComputeLength((char *)substr.c_str());
			return true;
		}
		case SDLK_BACKSPACE:
		{
			if (cursorPosition==0)
				return false;
            
			cursorPosition--;
			text.erase(cursorPosition,1);
			string substr = text.substr(0,cursorPosition);
			cursorPixelPosition = font->ComputeLength((char *)substr.c_str());
			return true;
		}
		case SDLK_DELETE:
		{
			if (cursorPosition==(signed)text.size())
				return false;
            
			text.erase(cursorPosition,1);
			string substr = text.substr(0,cursorPosition);
			cursorPixelPosition = font->ComputeLength((char *)substr.c_str());
			return true;
		}
		case SDLK_RETURN:
		{
			return ownerController->ProcessEvent(actionOnReturn);
		}
		default:
		{
			if (text.size() == maxChars)
				return true;
			
			unsigned short chsh = KD_Keyboard::GetKeyboard()->GetAsciiCharacter();

			// If the character is out of ASCII range.
			if (chsh == 0 || chsh >= 0x80) return false;

			char ch[2];
			ch[0] = (char)chsh;
			ch[1] = 0;

			if (ch[0]!=0)
			{
				text.insert(cursorPosition, ch);

				// If the text is coming out of the box... cancel
				int textLength = font->ComputeLength((char *)text.c_str());
				if (textLength > widthWidget-topleft_corner[KD_EDITFIELD_SELECTED]->GetWidth()-topright_corner[KD_EDITFIELD_SELECTED]->GetWidth() )
				{
					text.erase(text.size()-1);
					return true;
				}


				cursorPosition++;
				string substr = text.substr(0,cursorPosition);
				cursorPixelPosition = font->ComputeLength((char *)substr.c_str());
				return true;
			}
			else
				return false;
		}
	}

	return false;
}

void KD_EditField::setMaxChars(int maxChars)
{
	assert(maxChars<0);
	this->maxChars = maxChars;
	if ((int)text.size()>maxChars)
		text = text.substr(0, maxChars);
}

int KD_EditField::getMaxChars() const
{
	return maxChars;
}
