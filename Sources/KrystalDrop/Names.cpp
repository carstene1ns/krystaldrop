#include "Names.h"

char current_game= KD_G_SURVIVAL;

short pl_chars[KD_NB_MAX_PLAYERS];
char  pl_names[KD_NB_MAX_PLAYERS][PLAYER_NAME_SIZE+ 1];

char* CHAR_IMG_NAME[KD_NB_CHAR_IMG]= {
   "chaosb", "darknessb", "fireb", "forestb", "lightb",
   "snowb",  "spaceb",    "timeb", "waterb",  "windb",
   "chaosc", "darknessc", "firec", "forestc", "lightc",
   "snowc",  "spacec",    "timec", "waterc",  "windc" };
   
char* CHAR_ANIM_NAME[KD_NB_CHAR]= {
   "chaos",  "darkness",  "fire",  "forest",  "light",
   "snow",   "space",     "time",  "water",   "wind" };
   
char* CHAR_NAME[KD_NB_CHAR]= {
   "chaos", "darkness", "fire", "forest", "light",
   "snow",  "space",    "time", "water",  "wind" };

char* MUSIC_NAME[KD_NB_MUSIC]= {
   "musics/puzzle1.ogg", "musics/puzzle2.ogg", "musics/puzzle3.ogg", "musics/puzzle4.ogg" ,
   "musics/survival.ogg", "musics/puzzlelose.ogg" };

/*char* HST_NAME[KD_NB_HST]= {
   "art/defsco.acc", "survival.sco" };*/

/*char* GEM_ANIM_NAME[KD_GEM_NB_KINDS]=
{ "nr.txt", "nb.txt", "ng.txt", "ny.txt", "sr.txt", "sb.txt", "sg.txt", "sy.txt",
  "bg.txt","bcr.txt","bcb.txt","bcg.txt","bcy.txt","b.txt","pa.txt","da.txt",
  "flu.txt","fld.txt","fll.txt","flr.txt","fi.txt",
  "bn1.txt", "bn2.txt", "bn3.txt", "bn4.txt", 
  "bn5.txt", "bn6.txt", "bn7.txt", "bn8.txt", "bn9.txt",
  "tr.txt", "po.txt"
};*/

   /* supported gems: */
char* GEM_ANIM_NAME[KD_GEM_NB_KINDS]=
{ "nr", "nb", "ng", "ny" };

char* CHAR_CLASHSOUND_NAME[KD_SND_NBCLASHSOUND]= {
	"clash1", "clash2", "clash3",
	"clash4", "clash5", "clash6",
	"clash7", "clash8", "clash9",
	"clash10","clash11","clash12",
	"clash13" };
