#ifndef __DRAW_H__
#define __DRAW_H__

extern image_handle_t background_image;
extern image_handle_t base_image;
extern image_handle_t pipe_1;
extern image_handle_t pipe_2;
extern image_handle_t pipe_3;
extern image_handle_t flappy_bird_image;

void vDrawBackground(void);
void vDrawBase(void);
void vDrawBird(void);
void vDrawSpriteAnimations(TickType_t xLastFrameTime);
void vDrawPipes(void);
void vDrawScore(void);
void vDrawmenu(void);
void vDrawSubmenu(void);
void vDrawStartSingle(void);
void vDrawFlappyBird(void);
void vDrawScoreboard(void);
void vDrawStop(void);
void vDrawQuit(void);
void vDrawHighScores(void);
void vDrawGameOver(void);
void vDrawCheatMode(void);
void vDrawHighScores(void);
void vDrawMedal(void);
void vDrawNewHigh(void);

void GetMouse(void);
void GetSize(void);

int vCheckViewScores(void);
int vCheckSinglePlay(void);
int vCheckGameOverBack(void);
int vCheckCheatModeBack(void);
int vCheckReplay(void);
int vCheckMenuMouse(void);
int vCheckSingle(void);
int vCheckCheatMode(void);

#endif //__DRAW_H__