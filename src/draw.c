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
#include "bird.h"
#include "pipes.h"
#include <stdlib.h>

#define BACKGROUND_FILENAME "background-day.png"
#define PIPES_FILENAME "double_pipe.png"

image_handle_t background_image = NULL;
image_handle_t base_image = NULL;
image_handle_t base_image2 = NULL;

image_handle_t pipe_1 = NULL;
image_handle_t pipe_2 = NULL;
image_handle_t pipe_3 = NULL;

spritesheet_handle_t bird_spritesheet = NULL;
spritesheet_handle_t base_spritesheet = NULL;

sequence_handle_t forward_sequence = NULL;
sequence_handle_t reverse_sequence = NULL;

sequence_handle_t base_forward_sequence = NULL;

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

void vDrawBackground(void)
{
	static int image_height;

	if (background_image == NULL) {
		background_image = tumDrawLoadImage(BACKGROUND_FILENAME);
	}
	tumDrawSetLoadedImageScale(background_image, 1.5);

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
	char *base_spritesheet_path =
		tumUtilFindResourcePath("base_spritesheet_optimized.png");

	image_handle_t base_spritesheet_image =
		tumDrawLoadImage(base_spritesheet_path);

	base_spritesheet = tumDrawLoadSpritesheet(base_spritesheet_image, 4, 1);

	animation_handle_t base_animation =
		tumDrawAnimationCreate(base_spritesheet);

	tumDrawAnimationAddSequence(base_animation, "FORWARDS", 0, 0,
				    SPRITE_SEQUENCE_HORIZONTAL_POS, 4);

	base_forward_sequence = tumDrawAnimationSequenceInstantiate(
		base_animation, "FORWARDS", 300);
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
				  SCREEN_WIDTH / 3 - 50, player1.birdPosition);
	printf("bird y: %d\n", player1.birdPosition);

	if (bBirdAlive == true) {
		tumDrawAnimationDrawFrame(base_forward_sequence,
					  xTaskGetTickCount() - xLastFrameTime,
					  0, SCREEN_HEIGHT - 168);
	} else { //stops at last drawn frame
		tumDrawAnimationDrawFrame(base_forward_sequence, 0, 0,
					  SCREEN_HEIGHT - 150);
	}
}

//tumDrawAnimationDrawFrame(reverse_sequence,
//			  xTaskGetTickCount() - xLastFrameTime,
//			  SCREEN_WIDTH / 3 - 50, SCREEN_HEIGHT / 2);

//checkDraw(tumDrawSprite(bird_spritesheet, 0, 0, SCREEN_WIDTH / 3,
//	SCREEN_HEIGHT / 2),
//__FUNCTION__);

void vDrawPipes(void)
{
	int gap = 100;
	static int shiftPipe1 = 0;
	static int shiftPipe2 = -162;
	static int shiftPipe3 = -324;

	if (pipe_1 == NULL) {
		pipe_1 = tumDrawLoadImage(PIPES_FILENAME);
	}
	if (pipe_2 == NULL) {
		pipe_2 = tumDrawLoadImage(PIPES_FILENAME);
	}
	if (pipe_3 == NULL) {
		pipe_3 = tumDrawLoadImage(PIPES_FILENAME);
	}
	tumDrawSetLoadedImageScale(pipe_1, 0.5);
	tumDrawSetLoadedImageScale(pipe_2, 0.5);
	tumDrawSetLoadedImageScale(pipe_3, 0.5);

	//pipe 1
	if (bBirdAlive == true) {
		checkDraw(tumDrawLoadedImage(pipe_1, pipe1.x - shiftPipe1,
					     pipe1.y),
			  __FUNCTION__);
		shiftPipe1 += 2;

		if (shiftPipe1 > SCREEN_WIDTH + 52) {
			shiftPipe1 = -162;
		}
	}

	//pipe 2
	if (bBirdAlive == true) {
		checkDraw(tumDrawLoadedImage(pipe_2, pipe2.x - shiftPipe2,
					     pipe2.y),
			  __FUNCTION__);
		shiftPipe2 += 2;

		if (shiftPipe2 > SCREEN_WIDTH + 52 + 162) {
			shiftPipe2 = -162;
		}
	}
}

/*
	static int image_height;
	static int shift = 0;
	static int imgOutOfScreen =
		0; //Checks if image is out of the screen and moves accordingly

	if (base_image == NULL) {
		base_image = tumDrawLoadImage(BASE_FILENAME);
	}
	if (base_image2 == NULL) {
		base_image2 = tumDrawLoadImage(BASE_FILENAME);
	}

	tumDrawSetLoadedImageScale(base_image, 1.2857);
	tumDrawSetLoadedImageScale(base_image2, 1.2857);
	printf("width: %d", tumDrawGetLoadedImageWidth(base_image));

	if ((image_height = tumDrawGetLoadedImageHeight(base_image)) != -1) {
		if (bBirdAlive == true) {
			checkDraw(tumDrawLoadedImage(base_image,
						     0 - imgOutOfScreen - 1,
						     SCREEN_HEIGHT - 143),
				  __FUNCTION__);
			imgOutOfScreen += 3;
			if (imgOutOfScreen >= 432) {
				imgOutOfScreen = 0;
				checkDraw(
					tumDrawLoadedImage(
						base_image, SCREEN_WIDTH,
						SCREEN_HEIGHT - imgOutOfScreen),
					__FUNCTION__);
			}
		} else {
			checkDraw(tumDrawLoadedImage(base_image,
						     0 - imgOutOfScreen - 1,
						     SCREEN_HEIGHT - 143),
				  __FUNCTION__);
		}
	} else {
		fprints(stderr,
			"Failed to get size of image '%s', does it exist?\n",
			BASE_FILENAME);
	}

	if ((image_height = tumDrawGetLoadedImageHeight(base_image2)) != -1) {
		checkDraw(tumDrawLoadedImage(base_image2,
					     SCREEN_WIDTH - shift - 2,
					     SCREEN_HEIGHT - 143),
			  __FUNCTION__);
		shift += 3;

		if (shift >= 432) {
			shift = 0;
			checkDraw(tumDrawLoadedImage(base_image, SCREEN_WIDTH,
						     SCREEN_HEIGHT - shift),
				  __FUNCTION__);
		}

		else {
			fprints(stderr,
				"Failed to get size of image '%s', does it exist?\n",
				BASE_FILENAME);
		}
	}
*/