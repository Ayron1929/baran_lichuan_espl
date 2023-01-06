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
#define BIRD_MIDFLAP_FILENAME "yellowbird-midflap.png"

#define ZERO_FILENAME "0.png"
#define ONE_FILENAME "1.png"
#define TWO_FILENAME "2.png"
#define THREE_FILENAME "3.png"
#define FOUR_FILENAME "4.png"
#define FIVE_FILENAME "5.png"
#define SIX_FILENAME "6.png"
#define SEVEN_FILENAME "7.png"
#define EIGHT_FILENAME "8.png"
#define NINE_FILENAME "9.png"

image_handle_t background_image = NULL;
image_handle_t base_image = NULL;
image_handle_t base_image2 = NULL;

image_handle_t pipe_1 = NULL;
image_handle_t pipe_2 = NULL;
image_handle_t pipe_3 = NULL;
image_handle_t bird_midflap = NULL;

image_handle_t zero = NULL;
image_handle_t one = NULL;
image_handle_t two = NULL;
image_handle_t three = NULL;
image_handle_t four = NULL;
image_handle_t five = NULL;
image_handle_t six = NULL;
image_handle_t seven = NULL;
image_handle_t eight = NULL;
image_handle_t nine = NULL;

spritesheet_handle_t bird_spritesheet = NULL;
spritesheet_handle_t base_spritesheet = NULL;

sequence_handle_t forward_sequence = NULL;
sequence_handle_t reverse_sequence = NULL;

sequence_handle_t base_forward_sequence = NULL;

int score = 0;

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
	if (bird_midflap == NULL) {
		bird_midflap = tumDrawLoadImage(BIRD_MIDFLAP_FILENAME);
	}
	tumDrawSetLoadedImageScale(bird_midflap, 1.49);
	char *bird_spritesheet_path =
		tumUtilFindResourcePath("bird_spritesheet.png");

	image_handle_t bird_spritesheet_image =
		tumDrawLoadImage(bird_spritesheet_path);
	tumDrawSetLoadedImageScale(bird_spritesheet_image, 1.5);
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
	if (bBirdAlive == true) {
		tumDrawAnimationDrawFrame(forward_sequence,
					  xTaskGetTickCount() - xLastFrameTime,
					  player1.birdX, player1.birdY);
	} else {
		tumDrawLoadedImage(bird_midflap, player1.birdX, player1.birdY);
	}

	if (bBirdAlive == true) {
		tumDrawAnimationDrawFrame(base_forward_sequence,
					  xTaskGetTickCount() - xLastFrameTime,
					  0, SCREEN_HEIGHT - 140);
	} else { //stops at last drawn frame
		tumDrawAnimationDrawFrame(base_forward_sequence, 0, 0,
					  SCREEN_HEIGHT - 140);
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
		checkDraw(tumDrawLoadedImage(pipe_1, pipe1.x, pipe1.y),
			  __FUNCTION__);
		pipe1.x -= 2;

		if (pipe1.x <= -52) {
			pipe1.x = SCREEN_WIDTH + 116;
			pipe1.y = -350 + rand() % 310;
		}
	} else { //stops moving when bird is dead
		checkDraw(tumDrawLoadedImage(pipe_1, pipe1.x, pipe1.y),
			  __FUNCTION__);
	}
	//pipe 2
	if (bBirdAlive == true) {
		checkDraw(tumDrawLoadedImage(pipe_2, pipe2.x, pipe2.y),
			  __FUNCTION__);
		pipe2.x -= 2;

		if (pipe2.x <= -52) {
			pipe2.x = SCREEN_WIDTH + 116;
			pipe2.y = -350 + rand() % 310;
		}
	} else {
		checkDraw(tumDrawLoadedImage(pipe_2, pipe2.x, pipe2.y),
			  __FUNCTION__);
	}

	//pipe3
	if (bBirdAlive == true) {
		checkDraw(tumDrawLoadedImage(pipe_3, pipe3.x, pipe3.y),
			  __FUNCTION__);
		pipe3.x -= 2;

		if (pipe3.x <= -52) {
			pipe3.x = SCREEN_WIDTH + 116;
			pipe3.y = -350 + rand() % 310;
		}
	} else {
		checkDraw(tumDrawLoadedImage(pipe_3, pipe3.x, pipe3.y),
			  __FUNCTION__);
	}
}

void vDrawScore(void)
{
	countScore();

	int digit1 = score % 10;
	int digit10 = ((score - digit1) / 10) % 10;
	int digit100 = ((score - 10 * digit10 - digit1) / 100)  % 10;

	if (zero == NULL) {
		zero = tumDrawLoadImage(ZERO_FILENAME);
	}
	if (one == NULL) {
		one = tumDrawLoadImage(ONE_FILENAME);
	}
	if (two == NULL) {
		two = tumDrawLoadImage(TWO_FILENAME);
	}
	if (three == NULL) {
		three = tumDrawLoadImage(THREE_FILENAME);
	}
	if (four == NULL) {
		four = tumDrawLoadImage(FOUR_FILENAME);
	}
	if (five == NULL) {
		five = tumDrawLoadImage(FIVE_FILENAME);
	}
	if (six == NULL) {
		six = tumDrawLoadImage(SIX_FILENAME);
	}
	if (seven == NULL) {
		seven = tumDrawLoadImage(SEVEN_FILENAME);
	}
	if (eight == NULL) {
		eight = tumDrawLoadImage(EIGHT_FILENAME);
	}
	if (nine == NULL) {
		nine = tumDrawLoadImage(NINE_FILENAME);
	}

	switch (digit1) {
	case 0:
		tumDrawLoadedImage(zero, 200, 72);
		break;
	case 1:
		tumDrawLoadedImage(one, 200, 72);
		break;
	case 2:
		tumDrawLoadedImage(two, 200, 72);
		break;
	case 3:
		tumDrawLoadedImage(three, 200, 72);
		break;
	case 4:
		tumDrawLoadedImage(four, 200, 72);
		break;
	case 5:
		tumDrawLoadedImage(five, 200, 72);
		break;
	case 6:
		tumDrawLoadedImage(six, 200, 72);
		break;
	case 7:
		tumDrawLoadedImage(seven, 200, 72);
		break;
	case 8:
		tumDrawLoadedImage(eight,200, 72);
		break;
	case 9:
		tumDrawLoadedImage(nine, 200, 72);
		break;
	default:
		break;
	}

	if (score >= 10) {
		switch (digit10) {
		case 0:
			tumDrawLoadedImage(zero, 180, 72);
			break;
		case 1:
			tumDrawLoadedImage(one, 180, 72);
			break;
		case 2:
			tumDrawLoadedImage(two, 180, 72);
			break;
		case 3:
			tumDrawLoadedImage(three, 180, 72);
			break;
		case 4:
			tumDrawLoadedImage(four, 180, 72);
			break;
		case 5:
			tumDrawLoadedImage(five, 180, 72);
			break;
		case 6:
			tumDrawLoadedImage(six, 180, 72);
			break;
		case 7:
			tumDrawLoadedImage(seven, 180, 72);
			break;
		case 8:
			tumDrawLoadedImage(eight, 180, 72);
			break;
		case 9:
			tumDrawLoadedImage(nine, 180, 72);
			break;
		default:
			break;
		}
	}

	if (score >= 100) {
		switch (digit100) {
		case 0:
			tumDrawLoadedImage(zero, 160, 72);
			break;
		case 1:
			tumDrawLoadedImage(one, 160, 72);
			break;
		case 2:
			tumDrawLoadedImage(two, 160, 72);
			break;
		case 3:
			tumDrawLoadedImage(three, 160, 72);
			break;
		case 4:
			tumDrawLoadedImage(four, 160, 72);
			break;
		case 5:
			tumDrawLoadedImage(five, 160, 72);
			break;
		case 6:
			tumDrawLoadedImage(six, 160, 72);
			break;
		case 7:
			tumDrawLoadedImage(seven, 160, 72);
			break;
		case 8:
			tumDrawLoadedImage(eight, 160, 72);
			break;
		case 9:
			tumDrawLoadedImage(nine, 160, 72);
			break;
		default:
			break;
		}
	}
}