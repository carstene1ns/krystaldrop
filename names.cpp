#include "names.h"

char current_game= KD_G_SURVIVAL;

short pl_chars[KD_NB_MAX_PLAYERS];
char  pl_names[KD_NB_MAX_PLAYERS][PLAYER_NAME_SIZE+ 1];

char* CHAR_IMG_NAME[KD_NB_CHAR_IMG]= {
   "chaosb.png", "darknessb.png", "fireb.png", "forestb.png", "lightb.png",
   "snowb.png",  "spaceb.png",    "timeb.png", "waterb.png",  "windb.png",
   "chaosc.png", "darknessc.png", "firec.png", "forestc.png", "lightc.png",
   "snowc.png",  "spacec.png",    "timec.png", "waterc.png",  "windc.png" };
   
char* CHAR_ANIM_NAME[KD_NB_CHAR]= {
   "chaos.txt",  "darkness.txt",  "fire.txt",  "forest.txt",  "light.txt",
   "snow.txt",   "space.txt",     "time.txt",  "water.txt",   "wind.txt" };

char* MUSIC_NAME[KD_NB_MUSIC]= {
   "art/puzzle1.ogg", "art/puzzle2.ogg", "art/puzzle3.ogg", "art/puzzle4.ogg" };

char* HST_NAME[KD_NB_HST]= {
   "art/defsco.acc", "survival.sco" };

char* GEM_ANIM_NAME[KD_GEM_NB_KINDS]=
{ "nr.txt", "nb.txt", "ng.txt", "ny.txt", "sr.txt", "sb.txt", "sg.txt", "sy.txt",
  "bg.txt","bcr.txt","bcb.txt","bcg.txt","bcy.txt","b.txt","pa.txt","da.txt",
  "flu.txt","fld.txt","fll.txt","flr.txt","fi.txt",
  "bn1.txt", "bn2.txt", "bn3.txt", "bn4.txt", 
  "bn5.txt", "bn6.txt", "bn7.txt", "bn8.txt", "bn9.txt",
  "tr.txt", "po.txt"
};
