#ifndef NAMES_H
#define NAMES_H

#define KD_NB_MAX_PLAYERS 1
#define PLAYER_NAME_SIZE  3
#define MAX_PLAYERS_IN_HIGH_SCORE 9

/* characters ID */
#define KD_NB_CHAR 10
#define KD_CHAR_CHAOS    0
#define KD_CHAR_DARKNESS 1  
#define KD_CHAR_FIRE     2
#define KD_CHAR_FOREST   3
#define KD_CHAR_LIGHT    4
#define KD_CHAR_SNOW     5
#define KD_CHAR_SPACE    6 
#define KD_CHAR_TIME     7
#define KD_CHAR_WATER    8  
#define KD_CHAR_WIND     9 

/* musics ID */
#define KD_NB_MUSIC  6
#define KD_MUS_INTRO      1
#define KD_MUS_CHARSELECT 3
#define KD_MUS_SURVIVAL   4
#define KD_MUS_CONTINUE   5
#define KD_MUS_HIGHSCORES 0

#define KD_NB_CHAR_IMG (KD_NB_CHAR* 2)

/* game being played: survival/challenge */
#define KD_G_SURVIVAL 0
#define KD_G_CHALLENGE 1
extern char current_game;

/* players's characters and names */
extern short pl_chars[KD_NB_MAX_PLAYERS];
extern char  pl_names[KD_NB_MAX_PLAYERS][PLAYER_NAME_SIZE+ 1];

/* characters images */
extern char* CHAR_IMG_NAME[KD_NB_CHAR_IMG];

/* animation filenames for the characters */
extern char* CHAR_ANIM_NAME[KD_NB_CHAR];
extern char* CHAR_NAME[KD_NB_CHAR];

/* musics */
extern char* MUSIC_NAME[KD_NB_MUSIC];

/* high scores files */
//#define KD_NB_HST 2 /* high scores tables */
//extern char* HST_NAME[KD_NB_HST];

/* gems resources names */
/*#define KD_GEM_NB_KINDS 32*/
#define KD_GEM_NB_KINDS 4
extern char* GEM_ANIM_NAME[KD_GEM_NB_KINDS];

/* clash sounds for the gems */
/* number of clash sounds recorded */
#define KD_SND_NBCLASHSOUND 13
extern char* CHAR_CLASHSOUND_NAME[KD_SND_NBCLASHSOUND];

#endif
