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
	topright_corner->DisplayAlpha(xWidget+widthWidget-topright_corner->getWidth(),yWidget, alpha);

	// bottom left corner
	bottomleft_corner->DisplayAlpha(xWidget,yWidget+heightWidget-bottomleft_corner->getHeight(), alpha);

	// bottom right corner
	bottomright_corner->DisplayAlpha(xWidget+widthWidget-bottomright_corner->getWidth(),yWidget+heightWidget-bottomright_corner->getHeight(), alpha);


	int nbRep, xBase,yBase;

	// top border
	nbRep = (widthWidget - topleft_corner->getWidth() - topright_corner->getWidth()) / top_border->getWidth();
	xBase = xWidget+topleft_corner->getWidth();
	yBase = yWidget;

	for (int i=0; i<nbRep; i++)
	{
		top_border->DisplayAlpha(xBase + top_border->getWidth() * i,yBase, alpha);
	}
	if ((widthWidget - topleft_corner->getWidth() - topright_corner->getWidth()) % top_border->getWidth() != 0)
	{
		top_border->DisplayAlphaPart(xBase + top_border->getWidth()*nbRep,yBase,alpha,0,0,(widthWidget - topleft_corner->getWidth() - topright_corner->getWidth())%top_border->getWidth(),top_border->getHeight());
	}



	// top border
	nbRep = (widthWidget - topleft_corner->getWidth() - topright_corner->getWidth()) / top_border->getWidth();
	xBase = xWidget+topleft_corner->getWidth();
	yBase = yWidget;

	for (int i=0; i<nbRep; i++)
	{
		top_border->DisplayAlpha(xBase + top_border->getWidth() * i,yBase,alpha);
	}
	if ((widthWidget - topleft_corner->getWidth() - topright_corner->getWidth()) % top_border->getWidth() != 0)
	{
		top_border->DisplayAlphaPart(xBase + top_border->getWidth()*nbRep,yBase,alpha,0,0,(widthWidget - topleft_corner->getWidth() - topright_corner->getWidth())%top_border->getWidth(),top_border->getHeight());
	}

	// bottom border
	nbRep = (widthWidget - bottomleft_corner->getWidth() - bottomright_corner->getWidth()) / bottom_border->getWidth();
	xBase = xWidget+bottomleft_corner->getWidth();
	yBase = yWidget + heightWidget - bottom_border->getHeight();

	for (int i=0; i<nbRep; i++)
	{
		bottom_border->DisplayAlpha(xBase + bottom_border->getWidth() * i,yBase,alpha);
	}
	if ((widthWidget - bottomleft_corner->getWidth() - bottomright_corner->getWidth()) % bottom_border->getWidth() != 0)
	{
		bottom_border->DisplayAlphaPart(xBase + bottom_border->getWidth()*nbRep,yBase,alpha,0,0,(widthWidget - bottomleft_corner->getWidth() - bottomright_corner->getWidth())%bottom_border->getWidth(),bottom_border->getHeight());
	}

	// left border
	nbRep = (heightWidget - topleft_corner->getHeight() - bottomleft_corner->getHeight()) / left_border->getHeight();
	xBase = xWidget;
	yBase = yWidget+topleft_corner->getHeight();

	for (int i=0; i<nbRep; i++)
	{
		left_border->DisplayAlpha(xBase ,yBase + left_border->getHeight() * i, alpha);
	}
	if ((heightWidget - topleft_corner->getHeight() - bottomleft_corner->getHeight()) % left_border->getHeight() != 0)
	{
		left_border->DisplayAlphaPart(xBase,yBase + left_border->getHeight()*nbRep,alpha,0,0,left_border->getWidth(),(heightWidget - topleft_corner->getHeight() - bottomleft_corner->getHeight())%left_border->getHeight());
	}

	// right border
	nbRep = (heightWidget - topright_corner->getHeight() - bottomright_corner->getHeight()) / right_border->getHeight();
	xBase = xWidget+widthWidget - left_border->getWidth();
	yBase = yWidget+topleft_corner->getHeight();

	for (int i=0; i<nbRep; i++)
	{
		right_border->DisplayAlpha(xBase ,yBase + right_border->getHeight() * i, alpha);
	}
	if ((heightWidget - topright_corner->getHeight() - bottomright_corner->getHeight()) % right_border->getHeight() != 0)
	{
		right_border->DisplayAlphaPart(xBase,yBase + right_border->getHeight()*nbRep,alpha,0,0,right_border->getWidth(),(heightWidget - topright_corner->getHeight() - bottomright_corner->getHeight())%right_border->getHeight());
	}

	// Background
	int xRep,yRep;

	xBase = xWidget+topleft_corner->getWidth();
	yBase = yWidget+topleft_corner->getHeight();

	xRep = (widthWidget - topleft_corner->getWidth() - topright_corner->getWidth()) / background->getWidth();
	yRep = (heightWidget - topleft_corner->getHeight() - bottomleft_corner->getHeight()) / background->getHeight();

	for (int j=0; j<yRep; j++)
		for (int i=0; i<xRep; i++)
			background->DisplayAlpha(xBase + background->getWidth()*i,yBase + background->getHeight()*j,alpha);

	if ((widthWidget - topleft_corner->getWidth() - topright_corner->getWidth()) % background->getWidth() != 0)
	{
		for (int j=0; j<yRep; j++)
			background->DisplayAlphaPart(xBase + background->getWidth()*xRep,yBase + background->getHeight()*j,alpha,0,0,(widthWidget - topleft_corner->getWidth() - topright_corner->getWidth())%background->getWidth(),background->getHeight());
	}

	if ((heightWidget - topleft_corner->getHeight() - bottomleft_corner->getHeight()) % background->getHeight() != 0)
	{
		for (int i=0; i<xRep; i++)
			background->DisplayAlphaPart(xBase + background->getWidth()*i,yBase + background->getHeight()*yRep,alpha,0,0,background->getWidth(),(heightWidget - topleft_corner->getHeight() - bottomleft_corner->getHeight())%background->getHeight());

		if ((widthWidget - topleft_corner->getWidth() - topright_corner->getWidth()) % background->getWidth() != 0)
			background->DisplayAlphaPart(xBase + background->getWidth()*xRep,yBase + background->getHeight()*yRep,255,0,0,(widthWidget - topleft_corner->getWidth() - topright_corner->getWidth())%background->getWidth(),(heightWidget - topleft_corner->getHeight() - bottomleft_corner->getHeight())%background->getHeight());		
	}

	KD_WidgetContainer::DisplayWidget(alpha);
}
