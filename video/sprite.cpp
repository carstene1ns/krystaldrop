#include "sprite.h"

#include "SDL/SDL_image.h"

#include "Display.h"
#include "imagemanager.h"
#include "image.h"
#include "../util/textfile.h"
#include "../util/direct.h"
#include "../util/logfile.h"

#include <string.h>

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
	KD_ImageManager *imgMgr = KD_ImageManager::getImageManager();


	for (unsigned int i=0; i<images.size(); i++)
		imgMgr->releaseImage(images[i]);
}

void KD_Anim::addFileImage(char *name)
{
	KD_ImageManager *imgMgr = KD_ImageManager::getImageManager();

	imgMgr->Load(name);
	addSurface(imgMgr->getImage(name));
}

void KD_Anim::addFileImageFromACC(TACCRes *accFile, char *name)
{
	KD_ImageManager *imgMgr = KD_ImageManager::getImageManager();

	imgMgr->Load(accFile, name);
	addSurface(imgMgr->getImage(name));
}

void KD_Anim::addSurface(KD_Image *surf)
{
	images.push_back(surf);
}

void KD_Anim::setColorKey(Uint32 key)
{
	for (unsigned int i=0; i<images.size(); i++)
		images[i]->setColorKey(key);
}

void KD_Anim::setColorKey(Uint8 r, Uint8 g, Uint8 b)
{
	for (unsigned int i=0; i<images.size(); i++)
		images[i]->setColorKey(r,g,b);
}

void KD_Anim::Display(int x, int y, int frame)
{
	images[frame]->Display(x,y);
}

void KD_Anim::setNextAnim(int next_anim)
{
	this->next_anim = next_anim;
}

KD_Image *KD_Anim::getImage(int i)
{
	return images[i];
}


/************************************************************************************/

KD_Sprite::KD_Sprite()
{
	framePerSeconds=0;
}

KD_Sprite::~KD_Sprite()
{
	for (unsigned int i=0; i<anims.size(); i++)
		delete anims[i];
}

void KD_Sprite::Display(int x, int y, int anim, int frame)
{
	anims[anim]->Display(x, y, frame);
}

void KD_Sprite::setColorKey(Uint32 key)
{
	for (unsigned int i=0; i<anims.size(); i++)
		anims[i]->setColorKey(key);	
}

void KD_Sprite::setColorKey(Uint8 r, Uint8 g, Uint8 b)
{
	for (unsigned int i=0; i<anims.size(); i++)
		anims[i]->setColorKey(r,g,b);	
}

KD_Anim *KD_Sprite::newAnim()
{
	KD_Anim *anim = new KD_Anim();
	anims.push_back(anim);
	return anim;
}

bool KD_Sprite::Load(char *fileName)
{
	return Load(0,fileName);
}

bool KD_Sprite::Load(TACCRes *accFile, char *fileName)
{
	KD_TextFile file(accFile, fileName);

	char buf[1000];
	KD_Anim *currAnim = 0;
	
	while (!file.isEOF())
	{
		int res = sscanf(file.getPosition(), "%s", buf);

		if (res<=0)
		{
			file.jumpLine();
			if (file.isEOF()) break;
			continue;
		}

		if ( strcmp(buf, "spriterate")==0 )
		{
			res = sscanf(file.getPosition(), "%s %f", buf, &framePerSeconds);
			if (res != 2)
			{
				printf("File %s: Syntax error: spriterate must be followed by a floating point number.",fileName);
				KD_LogFile::printf("File %s: Syntax error: spriterate must be followed by a floating point number.",fileName);
				return false;
			}
		}
		else if ( strcmp(buf, "newanim")==0 )
		{
			currAnim = newAnim();
		}
		else if ( strcmp(buf, "file")==0 )
		{
			string str = file.getString();
			if (str == "")
			{
				printf("File %s: Syntax error: file must be followed by a file name between \"\".",fileName);
				KD_LogFile::printf("File %s: Syntax error: file must be followed by a file name between \"\".",fileName);
				return false;
			}
			if (currAnim == 0)
			{
				printf("File %s: Syntax error: newanim must be specified before the first instance of file.",fileName);
				KD_LogFile::printf("File %s: Syntax error: newanim must be specified before the first instance of file.",fileName);
				return false;
			}
			currAnim->addFileImageFromACC(accFile, (char *)str.c_str());
		}
		else if ( strcmp(buf, "jumpto")==0 )
		{
			int jumptoValue;
			res = sscanf(file.getPosition(), "%s %d", buf, &jumptoValue);
			if (res != 2)
			{
				printf("File %s: Syntax error: jumpt must be followed by an integer value.",fileName);
				KD_LogFile::printf("File %s: Syntax error: jumpt must be followed by an integer value.",fileName);
				return false;
			}
			currAnim->setNextAnim(jumptoValue);
		}
		else if ( strcmp(buf, "onlyonce")==0 )
			currAnim->setNextAnim(KD_NONEXTANIM);
		else
		{
			printf("File %s: Syntax error: unexpected token %s.",fileName, buf);
			KD_LogFile::printf("File %s: Syntax error: unexpected token %s.",fileName, buf);
			return false;
		}
		file.jumpLine();
	}

	return true;
}

KD_Anim *KD_Sprite::getAnim(int i)
{
	return anims[i];
}
