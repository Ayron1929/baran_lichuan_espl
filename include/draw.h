#ifndef __DRAW_H__
#define __DRAW_H__

extern image_handle_t background_image;
extern image_handle_t base_image;
extern image_handle_t base_image2;


void vDrawBackground(void);
void vDrawBase(void);
void vDrawBird(void);
void vDrawSpriteAnimations(TickType_t xLastFrameTime);
void vDrawPipes(void);
void vDrawmenu(void);
void vDrawQuit(void);
//void vDrawStop(void);
void vDrawSubmenu(void);
void vDrawGameOver(void);
void vDrawCheatMode(void);
void vDrawStartSingle(void);
int vCheckViewScores(void);
int vCheckSingleTest(void);
int vCheckGameOverBack(void);
int vCheckCheatModeBack(void);
int vCheckReplay(void);
int vCheckMenuMouse(void);
int vCheckSingle(void);
int vCheckCheatMode(void);
void vDrawBackground(void);


#endif //__DRAW_H__