#include <string.h>

#include "SDL/SDL_image.h"
#include "Display.h"
//#include "imagemanager.h"
#include "../Resources/ResourceManager.h"
#include "Image.h"
#include "Sprite.h"
#include "../Tools/Textfile.h"
//#include "../Tools/direct.h"
#include "../Tools/Logfile.h"
#include "XMLSpriteParser.h"
#include "SpriteInstance.h"

#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include <string>
using namespace std;

KD_Anim::KD_Anim()
{
	next_anim=-1;
}

KD_Anim::~KD_Anim()
{ 
	KD_ResourceManager *resMgr = KD_ResourceManager::GetResourceManager();

	for (unsigned int i=0; i<images.size(); i++)
		resMgr->ReleaseResource(images[i].image);
}

void KD_Anim::addFileImage(const KD_FilePath &name, int x, int y)
{
	KD_ResourceManager *resMgr = KD_ResourceManager::GetResourceManager();
	addSurface((KD_Image *)(resMgr->GetResource(name)),x,y);
}

void KD_Anim::addSurface(KD_Image *surf, int x, int y)
{
	images.push_back(CenteredImage(surf,x,y));
}

void KD_Anim::SetColorKey(Uint32 key)
{
	for (unsigned int i=0; i<images.size(); i++)
		images[i].image->SetColorKey(key);
}

void KD_Anim::SetColorKey(Uint8 r, Uint8 g, Uint8 b)
{
	for (unsigned int i=0; i<images.size(); i++)
		images[i].image->SetColorKey(r,g,b);
}


/*void KD_Anim::Display(int x, int y, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle, int frame)
{
	images[frame].image->DisplayColorZoomRotate(x-images[frame].x,y-images[frame].y,r,g,b,alpha,resizeX,resizeY,rotX+images[frame].x,rotY+images[frame].y,angle);
}*/

void KD_Anim::Display(int frame, float x, float y)
{
	images[frame].image->Display(x-images[frame].x,y-images[frame].y);
}

/*void KD_Sprite::Display(int x, int y, int anim, int frame, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle)
{
	anims[anim]->Display(x, y, r, g, b, alpha, resizeX, resizeY, rotX, rotY, angle, frame);
}*/

void KD_Anim::DisplayAlpha(int frame, float x, float y, int alpha)
{
	images[frame].image->DisplayAlpha(x-images[frame].x,y-images[frame].y, alpha);
}

void KD_Anim::DisplayColorZoom(int frame, float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY)
{
	images[frame].image->DisplayColorZoom(x-images[frame].x,y-images[frame].y, r, g, b, alpha, resizeX, resizeY);
}

void KD_Anim::DisplayColorZoomRotate(int frame, float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle)
{
	images[frame].image->DisplayColorZoomRotate(x-images[frame].x,y-images[frame].y, r, g, b, alpha, resizeX, resizeY, rotX+x/*+images[frame].x*/, rotY+y/*+images[frame].y*/, angle);
}

void KD_Anim::DisplayAlphaPart(int frame, float x, float y, int alpha, int xPart, int yPart, int widthPart, int heightPart)
{
	images[frame].image->DisplayAlphaPart(x-images[frame].x,y-images[frame].y, alpha, xPart, yPart, widthPart-images[frame].x, heightPart-images[frame].y);
}

void KD_Anim::DisplayFullParameters(int frame, float x1, float y1, int r1, int g1, int b1, int alpha1, float x2, float y2, int r2, int g2, int b2, int alpha2, float x3, float y3, int r3, int g3, int b3, int alpha3, float x4, float y4, int r4, int g4, int b4, int alpha4)
{
	images[frame].image->DisplayFullParameters(x1,y1,r1,g1,b1,alpha1, x2,y2,r2,g2,b2,alpha2, x3,y3,r3,g3,b3,alpha3, x4,y4,r4,g4,b4,alpha4);
}

int KD_Anim::GetHeight(int frame)
{
	return images[frame].image->GetHeight();
}

int KD_Anim::GetWidth(int frame)
{
	return images[frame].image->GetHeight();
}

void KD_Anim::setNextAnim(int next_anim)
{
	this->next_anim = next_anim;
}

KD_Image *KD_Anim::getImage(int i)
{
	return images[i].image;
}

unsigned KD_Anim::getAnimSize()
{
	return (unsigned) images.size();
}

/************************************************************************************/

KD_Sprite::KD_Sprite() : KD_DisplayableResource()
{
	framePerSeconds= 0;
}

KD_Sprite::~KD_Sprite()
{ 
	for (unsigned int i= 0; i< anims.size(); i++)
		delete anims[i];
}

void KD_Sprite::SetColorKey(Uint32 key)
{
	for (unsigned int i= 0; i< anims.size(); i++)
		anims[i]->SetColorKey(key);	
}

void KD_Sprite::SetColorKey(Uint8 r, Uint8 g, Uint8 b)
{
	for (unsigned int i=0; i<anims.size(); i++)
		anims[i]->SetColorKey(r,g,b);	
}

KD_Anim *KD_Sprite::newAnim()
{
	KD_Anim *anim = new KD_Anim();
	anims.push_back(anim);
	return anim;
}


bool KD_Sprite::load(const KD_FilePath &fileName)
{
	KD_TextFile file(fileName);

	char buf[1000]; // ## !! 
	KD_Anim *currAnim = 0;
	
	while (!file.IsEOF())
	{
		int res = sscanf(file.GetPosition(), "%s", buf);

		if (res<=0)
		{
			file.JumpLine();
			if (file.IsEOF()) break;
			continue;
		}

		if ( strcmp(buf, "spriterate")==0 )
		{
			res = sscanf(file.GetPosition(), "%s %f", buf, &framePerSeconds);
			if (res != 2)
			{
// ##				printf("File %s: Syntax error: spriterate must be followed by a floating point number.",fileName);
// ##				KD_LogFile::printf("File %s: Syntax error: spriterate must be followed by a floating point number.",fileName);
				return false;
			}
		}
		else if ( strcmp(buf, "newanim")==0 )
		{
			currAnim = newAnim();
		}
		else if ( strcmp(buf, "file")==0 )
		{
			file.pos += 4;
			string str = file.GetString();
			if (str == "")
			{
// ##				printf("File %s: Syntax error: file must be followed by a file name between \"\".",fileName);
// ##				KD_LogFile::printf("File %s: Syntax error: file must be followed by a file name between \"\".",fileName);
				return false;
			}
			if (currAnim == 0)
			{
// ##				printf("File %s: Syntax error: newanim must be specified before the first instance of file.",fileName);
// ##				KD_LogFile::printf("File %s: Syntax error: newanim must be specified before the first instance of file.",fileName);
				return false;
			}
			//currAnim->addFileImageFromACC(accFile, (char *)str.c_str());
			currAnim->addFileImage(fileName.GetFullDirectory()+str,0,0);
		}
		else if ( strcmp(buf, "jumpto")==0 )
		{
			int jumptoValue;
			res = sscanf(file.GetPosition(), "%s %d", buf, &jumptoValue);
			if (res != 2)
			{
// ##				printf("File %s: Syntax error: jumpt must be followed by an integer value.",fileName);
// ##				KD_LogFile::printf("File %s: Syntax error: jumpt must be followed by an integer value.",fileName);
				return false;
			}
			currAnim->setNextAnim(jumptoValue);
		}
		else if ( strcmp(buf, "onlyonce")==0 )
			currAnim->setNextAnim(KD_NONEXTANIM);
		else
		{
// ##			printf("File %s: Syntax error: unexpected token %s.",fileName, buf);
// ##			KD_LogFile::printf("File %s: Syntax error: unexpected token %s.",fileName, buf);
			return false;
		}
		file.JumpLine();
	}

	return true;
}

KD_Anim *KD_Sprite::getAnim(int i)
{
	return anims[i];
}

void KD_Sprite::setFramePerSec(float framePerSeconds)
{
	this->framePerSeconds = framePerSeconds;
}

bool KD_Sprite::resize(float ratio)
{
	// First let's build a deque of all the single KD_Image in that sprite!
	deque<KD_Image *> images;

	unsigned i, j, k;

	for (j=0; j<anims.size(); j++)
		for (i=0; i<anims[j]->getAnimSize(); i++)
		{
			anims[j]->images[i].x = (int)((float)anims[j]->images[i].x * ratio);
			anims[j]->images[i].y = (int)((float)anims[j]->images[i].y * ratio);


			bool found = false;
			// First let's see if we can find this item in the deque.
			for (k=0; k<images.size(); k++)
			{
				if (images[k]==anims[j]->getImage(i))
				{
					found = true;
					break;
				}
			}

			// If we can't find it, let's add it to the deque.
			if (found == false)
			{
				images.push_back(anims[j]->getImage(i));
			}

		}

	// Now that we've got the deque, let's resize it!
	for (i=0; i<images.size(); i++)
	{
		images[i]->resize(ratio);
	}
	return true;
}

#ifndef NO_XML
bool KD_Sprite::loadXML(const KD_FilePath &fileName)
{
	KD_XMLSpriteParser Parser(fileName, this);

	return Parser.Parse();
}
#endif

KD_DisplayableInstance *KD_Sprite::createInstance()
{
	return new KD_SpriteInstance(this);
}

void KD_Sprite::Display(int anim, int frame, float x, float y)
{
	anims[anim]->Display(frame, x, y);
}

/*void KD_Sprite::Display(int x, int y, int anim, int frame, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle)
{
	anims[anim]->Display(x, y, r, g, b, alpha, resizeX, resizeY, rotX, rotY, angle, frame);
}*/

void KD_Sprite::DisplayAlpha(int anim, int frame, float x, float y, int alpha)
{
	anims[anim]->DisplayAlpha(frame, x, y, alpha);
}

void KD_Sprite::DisplayColorZoom(int anim, int frame, float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY)
{
	anims[anim]->DisplayColorZoom(frame, x, y, r, g, b, alpha, resizeX, resizeY);
}

void KD_Sprite::DisplayColorZoomRotate(int anim, int frame, float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle)
{
	anims[anim]->DisplayColorZoomRotate(frame, x, y, r, g, b, alpha, resizeX, resizeY, rotX, rotY, angle);
}

void KD_Sprite::DisplayAlphaPart(int anim, int frame, float x, float y, int alpha, int xPart, int yPart, int widthPart, int heightPart)
{
	anims[anim]->DisplayAlphaPart(frame, x, y, alpha, xPart, yPart, widthPart, heightPart);
}

void KD_Sprite::DisplayFullParameters(int anim, int frame, float x1, float y1, int r1, int g1, int b1, int alpha1, float x2, float y2, int r2, int g2, int b2, int alpha2, float x3, float y3, int r3, int g3, int b3, int alpha3, float x4, float y4, int r4, int g4, int b4, int alpha4)
{
	anims[anim]->DisplayFullParameters(frame, x1,y1,r1,g1,b1,alpha1, x2,y2,r2,g2,b2,alpha2, x3,y3,r3,g3,b3,alpha3, x4,y4,r4,g4,b4,alpha4);
}

int KD_Sprite::GetHeight(int anim, int frame)
{
	return anims[anim]->GetHeight(frame);
}

int KD_Sprite::GetWidth(int anim, int frame)
{
	return anims[anim]->GetWidth(frame);
}
