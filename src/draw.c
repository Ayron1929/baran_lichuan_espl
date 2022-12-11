#include "TUM_Draw.h"
#include "TUM_Event.h"
#include "TUM_Ball.h"
#include "TUM_Sound.h"
#include "TUM_Utils.h"
#include "TUM_Print.h"

#include "defines.h"
#include "main.h"
#include "demo_tasks.h"
#include "draw.h"
#include "async_message_queues.h"
#include "async_sockets.h"
#include "buttons.h"
#include "state_machine.h"
// #include "menu.h"

#define BACKGROUND_FILENAME "background-day.png"
#define BASE_FILENAME "base.png"

image_handle_t background_image = NULL;
image_handle_t base_image = NULL;
image_handle_t base_image2 = NULL;

spritesheet_handle_t bird_spritesheet = NULL;
sequence_handle_t forward_sequence = NULL;
sequence_handle_t reverse_sequence = NULL;

void checkDraw(unsigned char status, const char *msg)
{
	if (status) {
		if (msg)
			fprints(stderr, "[ERROR] %s, %s\n", msg,
				tumGetErrorMessage());
		else {
			fprints(stderr, "[ERROR] %s\n", tumGetErrorMessage());
		}
	}
}

void vDrawmenu(void)
{

	static char str[100] = { 0 };
    static int text_width;
	char *menu = "Menu";


    ssize_t prev_font_size = tumFontGetCurFontSize();


    tumFontSetSize((ssize_t)30);

    sprintf(str, "[Q]uit");

    if (!tumGetTextSize((char *)str, &text_width, NULL))
        checkDraw(tumDrawText(str, SCREEN_WIDTH - text_width - 10,
                              DEFAULT_FONT_SIZE * 0.5, Black),
                  __FUNCTION__);

	checkDraw(tumDrawText(menu, 260, 925, Maroon), __FUNCTION__);
    tumFontSetSize(prev_font_size);


}

void vDrawSubmenu(void)
{
	char *single = "Single Player";
	char *multi = "Two Player";
	char *cheats = "Cheat Mode";
	char *high_score = "View Scores";
	char *back = "Back";

	tumFontSetSize((ssize_t)30);
	checkDraw(tumDrawText(single, 200, DEFAULT_FONT_SIZE * 5, Maroon), __FUNCTION__);

}

void vDrawBackground(void)
{
	static int image_height;

	if (background_image == NULL) {
		background_image = tumDrawLoadImage(BACKGROUND_FILENAME);
	}
	tumDrawSetLoadedImageScale(background_image, 2.0);

	if ((image_height = tumDrawGetLoadedImageHeight(background_image)) !=
	    -1)
		checkDraw(tumDrawLoadedImage(background_image, 0, 0),
			  __FUNCTION__);
	else {
		fprints(stderr,
			"Failed to get size of image '%s', does it exist?\n",
			BACKGROUND_FILENAME);
	}
}

void vDrawBase(void)
{
	static int image_height;
	static int imgOutOfScreen = 0; //Checks if image is out of the screen and moves accordingly

	if (base_image == NULL) {
		base_image = tumDrawLoadImage(BASE_FILENAME);
	}
	if (base_image2 == NULL) {
		base_image2 = tumDrawLoadImage(BASE_FILENAME);
	}

	tumDrawSetLoadedImageScale(base_image, 1.7142);
	tumDrawSetLoadedImageScale(base_image2, 1.7142);

	if ((image_height = tumDrawGetLoadedImageHeight(base_image)) != -1) {
		checkDraw(tumDrawLoadedImage(base_image, 0 - imgOutOfScreen - 1,
					     SCREEN_HEIGHT - 190),
			  __FUNCTION__);
		imgOutOfScreen += 5;
		if (imgOutOfScreen >= tumDrawGetLoadedImageWidth(base_image)) {
			imgOutOfScreen = 0;
			checkDraw(tumDrawLoadedImage(base_image, SCREEN_WIDTH,
						     SCREEN_HEIGHT - imgOutOfScreen),
				  __FUNCTION__);
		}
	} else {
		fprints(stderr,
			"Failed to get size of image '%s', does it exist?\n",
			BASE_FILENAME);
	}
	if ((image_height = tumDrawGetLoadedImageHeight(base_image2)) != -1)
		checkDraw(tumDrawLoadedImage(base_image2,
					     SCREEN_WIDTH - imgOutOfScreen,
					     SCREEN_HEIGHT - 190),
			  __FUNCTION__);

	else {
		fprints(stderr,
			"Failed to get size of image '%s', does it exist?\n",
			BASE_FILENAME);
	}
}

void vDrawBird(void)
{
	char *bird_spritesheet_path =
		tumUtilFindResourcePath("bird_spritesheet.png");

	image_handle_t bird_spritesheet_image =
		tumDrawLoadImage(bird_spritesheet_path);
	bird_spritesheet = tumDrawLoadSpritesheet(bird_spritesheet_image, 3, 1);

	animation_handle_t bird_animation =
		tumDrawAnimationCreate(bird_spritesheet);

	tumDrawAnimationAddSequence(bird_animation, "FORWARDS", 0, 0,
				    SPRITE_SEQUENCE_HORIZONTAL_POS, 3);
	tumDrawAnimationAddSequence(bird_animation, "REVERSE", 0, 3,
				    SPRITE_SEQUENCE_HORIZONTAL_NEG, 3);

	forward_sequence = tumDrawAnimationSequenceInstantiate(bird_animation,
							       "FORWARDS", 120);
	reverse_sequence = tumDrawAnimationSequenceInstantiate(bird_animation,
							       "REVERSE", 120);
}

void vDrawSpriteAnimations(TickType_t xLastFrameTime)
{
	tumDrawAnimationDrawFrame(forward_sequence,
				  xTaskGetTickCount() - xLastFrameTime,
				  SCREEN_WIDTH / 3 - 50, SCREEN_HEIGHT / 2);
	tumDrawAnimationDrawFrame(reverse_sequence,
				  xTaskGetTickCount() - xLastFrameTime,
				  SCREEN_WIDTH / 3 - 50, SCREEN_HEIGHT / 2);

	//checkDraw(tumDrawSprite(bird_spritesheet, 0, 0, SCREEN_WIDTH / 3,
	//	SCREEN_HEIGHT / 2),
	//__FUNCTION__);
}