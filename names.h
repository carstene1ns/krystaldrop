#ifndef NAMES_H
#define NAMES_H

#define KD_NB_MAX_PLAYERS 1

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
#define KD_NB_MUSIC   4
#define KD_MUS_INTRO      2
#define KD_MUS_CHARSELECT 3

#define KD_NB_CHAR_IMG (KD_NB_CHAR* 2)
/* players's characters */
extern short pl_chars[KD_NB_MAX_PLAYERS];

/* characters images */
extern char* CHAR_IMG_NAME[KD_NB_CHAR_IMG];

/* animation filenames for the characters */
extern char* CHAR_ANIM_NAME[KD_NB_CHAR];

/* musics */
extern char* MUSIC_NAME[KD_NB_MUSIC];

#endif