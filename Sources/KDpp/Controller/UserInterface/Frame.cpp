#include "Frame.h"

#include "../WidgetController.h"

#include "../../Video/Image.h"

KD_Frame::KD_Frame(int x, int y, int width, int height) : KD_WidgetContainer()
{
	xWidget = x;
	yWidget = y;
	widthWidget = width;
	heightWidget = height;

	background = 0;
	top_border = 0;
	bottom_border = 0;
	left_border = 0;
	right_border = 0;
	topleft_corner = 0;
	topright_corner = 0;
	bottomleft_corner = 0;
	bottomright_corner = 0;
}

KD_Frame::~KD_Frame()
{
	deleteFrame();
}

void KD_Frame::deleteFrame()
{
	if (background)
		ownerController->ReleaseResource("window_background");
	if (top_border)
		ownerController->ReleaseResource("window_top_border");
	if (bottom_border)
		ownerController->ReleaseResource("window_bottom_border");
	if (left_border)
		ownerController->ReleaseResource("window_left_border");
	if (right_border)
		ownerController->ReleaseResource("window_right_border");
	if (bottomleft_corner)
		ownerController->ReleaseResource("window_bottomleft_corner");
	if (bottomright_corner)
		ownerController->ReleaseResource("window_bottomright_corner");
	if (topleft_corner)
		ownerController->ReleaseResource("window_topleft_corner");
	if (topright_corner)
		ownerController->ReleaseResource("window_topright_corner");
}

void KD_Frame::DisplayWidget(int alpha)
{
	alpha = alpha*alphaWidget/255;

	// if the images are not loaded, load them now!
	if (background == 0)
	{
		background = (KD_Image*) ownerController->GetResource("window_background");
		top_border = (KD_Image*) ownerController->GetResource("window_top_border");
		bottom_border = (KD_Image*) ownerController->GetResource("window_bottom_border");
		left_border = (KD_Image*) ownerController->GetResource("window_left_border");
		right_border = (KD_Image*) ownerController->GetResource("window_right_border");
		bottomleft_corner = (KD_Image*) ownerController->GetResource("window_bottomleft_corner");
		bottomright_corner = (KD_Image*) ownerController->GetResource("window_bottomright_corner");
		topleft_corner = (KD_Image*) ownerController->GetResource("window_topleft_corner");
		topright_corner = (KD_Image*) ownerController->GetResource("window_topright_corner");
	}

	// top left corner
	topleft_corner->DisplayAlpha(xWidget,yWidget, alpha);

	// top right corner
	topright_corner->DisplayAlpha(xWidget+widthWidget-topright_corner->GetWidth(),yWidget, alpha);

	// bottom left corner
	bottomleft_corner->DisplayAlpha(xWidget,yWidget+heightWidget-bottomleft_corner->GetHeight(), alpha);

	// bottom right corner
	bottomright_corner->DisplayAlpha(xWidget+widthWidget-bottomright_corner->GetWidth(),yWidget+heightWidget-bottomright_corner->GetHeight(), alpha);


	int nbRep, xBase,yBase;

	// top border
	nbRep = (widthWidget - topleft_corner->GetWidth() - topright_corner->GetWidth()) / top_border->GetWidth();
	xBase = xWidget+topleft_corner->GetWidth();
	yBase = yWidget;

	for (int i=0; i<nbRep; i++)
	{
		top_border->DisplayAlpha(xBase + top_border->GetWidth() * i,yBase, alpha);
	}
	if ((widthWidget - topleft_corner->GetWidth() - topright_corner->GetWidth()) % top_border->GetWidth() != 0)
	{
		top_border->DisplayAlphaPart(xBase + top_border->GetWidth()*nbRep,yBase,alpha,0,0,(widthWidget - topleft_corner->GetWidth() - topright_corner->GetWidth())%top_border->GetWidth(),top_border->GetHeight());
	}



	// top border
	nbRep = (widthWidget - topleft_corner->GetWidth() - topright_corner->GetWidth()) / top_border->GetWidth();
	xBase = xWidget+topleft_corner->GetWidth();
	yBase = yWidget;

	for (int i=0; i<nbRep; i++)
	{
		top_border->DisplayAlpha(xBase + top_border->GetWidth() * i,yBase,alpha);
	}
	if ((widthWidget - topleft_corner->GetWidth() - topright_corner->GetWidth()) % top_border->GetWidth() != 0)
	{
		top_border->DisplayAlphaPart(xBase + top_border->GetWidth()*nbRep,yBase,alpha,0,0,(widthWidget - topleft_corner->GetWidth() - topright_corner->GetWidth())%top_border->GetWidth(),top_border->GetHeight());
	}

	// bottom border
	nbRep = (widthWidget - bottomleft_corner->GetWidth() - bottomright_corner->GetWidth()) / bottom_border->GetWidth();
	xBase = xWidget+bottomleft_corner->GetWidth();
	yBase = yWidget + heightWidget - bottom_border->GetHeight();

	for (int i=0; i<nbRep; i++)
	{
		bottom_border->DisplayAlpha(xBase + bottom_border->GetWidth() * i,yBase,alpha);
	}
	if ((widthWidget - bottomleft_corner->GetWidth() - bottomright_corner->GetWidth()) % bottom_border->GetWidth() != 0)
	{
		bottom_border->DisplayAlphaPart(xBase + bottom_border->GetWidth()*nbRep,yBase,alpha,0,0,(widthWidget - bottomleft_corner->GetWidth() - bottomright_corner->GetWidth())%bottom_border->GetWidth(),bottom_border->GetHeight());
	}

	// left border
	nbRep = (heightWidget - topleft_corner->GetHeight() - bottomleft_corner->GetHeight()) / left_border->GetHeight();
	xBase = xWidget;
	yBase = yWidget+topleft_corner->GetHeight();

	for (int i=0; i<nbRep; i++)
	{
		left_border->DisplayAlpha(xBase ,yBase + left_border->GetHeight() * i, alpha);
	}
	if ((heightWidget - topleft_corner->GetHeight() - bottomleft_corner->GetHeight()) % left_border->GetHeight() != 0)
	{
		left_border->DisplayAlphaPart(xBase,yBase + left_border->GetHeight()*nbRep,alpha,0,0,left_border->GetWidth(),(heightWidget - topleft_corner->GetHeight() - bottomleft_corner->GetHeight())%left_border->GetHeight());
	}

	// right border
	nbRep = (heightWidget - topright_corner->GetHeight() - bottomright_corner->GetHeight()) / right_border->GetHeight();
	xBase = xWidget+widthWidget - left_border->GetWidth();
	yBase = yWidget+topleft_corner->GetHeight();

	for (int i=0; i<nbRep; i++)
	{
		right_border->DisplayAlpha(xBase ,yBase + right_border->GetHeight() * i, alpha);
	}
	if ((heightWidget - topright_corner->GetHeight() - bottomright_corner->GetHeight()) % right_border->GetHeight() != 0)
	{
		right_border->DisplayAlphaPart(xBase,yBase + right_border->GetHeight()*nbRep,alpha,0,0,right_border->GetWidth(),(heightWidget - topright_corner->GetHeight() - bottomright_corner->GetHeight())%right_border->GetHeight());
	}

	// Background
	int xRep,yRep;

	xBase = xWidget+topleft_corner->GetWidth();
	yBase = yWidget+topleft_corner->GetHeight();

	xRep = (widthWidget - topleft_corner->GetWidth() - topright_corner->GetWidth()) / background->GetWidth();
	yRep = (heightWidget - topleft_corner->GetHeight() - bottomleft_corner->GetHeight()) / background->GetHeight();

	for (int j=0; j<yRep; j++)
		for (int i=0; i<xRep; i++)
			background->DisplayAlpha(xBase + background->GetWidth()*i,yBase + background->GetHeight()*j,alpha);

	if ((widthWidget - topleft_corner->GetWidth() - topright_corner->GetWidth()) % background->GetWidth() != 0)
	{
		for (int j=0; j<yRep; j++)
			background->DisplayAlphaPart(xBase + background->GetWidth()*xRep,yBase + background->GetHeight()*j,alpha,0,0,(widthWidget - topleft_corner->GetWidth() - topright_corner->GetWidth())%background->GetWidth(),background->GetHeight());
	}

	if ((heightWidget - topleft_corner->GetHeight() - bottomleft_corner->GetHeight()) % background->GetHeight() != 0)
	{
		for (int i=0; i<xRep; i++)
			background->DisplayAlphaPart(xBase + background->GetWidth()*i,yBase + background->GetHeight()*yRep,alpha,0,0,background->GetWidth(),(heightWidget - topleft_corner->GetHeight() - bottomleft_corner->GetHeight())%background->GetHeight());

		if ((widthWidget - topleft_corner->GetWidth() - topright_corner->GetWidth()) % background->GetWidth() != 0)
			background->DisplayAlphaPart(xBase + background->GetWidth()*xRep,yBase + background->GetHeight()*yRep,255,0,0,(widthWidget - topleft_corner->GetWidth() - topright_corner->GetWidth())%background->GetWidth(),(heightWidget - topleft_corner->GetHeight() - bottomleft_corner->GetHeight())%background->GetHeight());		
	}

	KD_WidgetContainer::DisplayWidget(alpha);
}
