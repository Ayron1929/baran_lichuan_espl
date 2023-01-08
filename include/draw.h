#ifndef __DRAW_H__
#define __DRAW_H__

extern image_handle_t background_image;
extern image_handle_t base_image;
extern image_handle_t base_image2;
extern image_handle_t pipe_1;
extern image_handle_t pipe_2;
extern image_handle_t pipe_3;

void vDrawBackground(void);
void vDrawBase(void);
void vDrawBird(void);
void vDrawSpriteAnimations(TickType_t xLastFrameTime);
void vDrawPipes(void);
void vDrawScore(void);
void vDrawmenu(void);
void vDrawSubmenu(void);
void vDrawStartSingle(void);



#endif //__DRAW_H__