#ifndef ROWMACROS_H
#define ROWMACROS_H
sdflksdlfksldfkslfkslkflsd
/* Everybody know macros are dangerous and tricky... I knew it...
   But I still lost hours debugging,
   to eventually discover that I forgot a pair of bracket. Aaaaargh ! */
#define GEM_PTR_SIZE (sizeof(KD_Gem*)/sizeof(short))
#define GEMBLOCK_HEADER_SIZE 4 // in short's
#define B_GEM_PTR(p_block,n)   ( p_block+ GEMBLOCK_HEADER_SIZE+ (n)*GEM_PTR_SIZE ) 

#define B_READ_NB(p_block)     ( *(p_block+0) )
#define B_READ_SPEED(p_block)  ( *(p_block+1) )
#define B_READ_ACCEL(p_block)  ( *(p_block+2) )
#define B_READ_GEM(p_block,n)  ( *( (KD_Gem**) B_GEM_PTR(p_block,(n)))) 
                                                              // ^- the @&!# bracket
#define B_WRITE_NB(p_block,x)    p_block[0]= x;
#define B_WRITE_SPEED(p_block,y) p_block[1]= y;
#define B_WRITE_ACCEL(p_block,z) p_block[2]= z;
#define B_WRITE_GEM(p_block,n,p) *( (KD_Gem**) (B_GEM_PTR(p_block,(n)))   )= p;

#define B_NEXT_BLOCK(p_block) ( B_GEM_PTR(p_block,B_READ_NB(p_block)) )
#define B_IS_LAST_BLOCK(p_block) ( *(B_NEXT_BLOCK(p_block))== 0 )
/* B_IS_LAST_BLOCK will fail if p_block points on an empty block */

#endif
