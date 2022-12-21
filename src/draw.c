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
#include "sm.h"

#define BACKGROUND_FILENAME "background-day.png"
#define BASE_FILENAME "base.png"
#define SINGLE_START_FILENAME "message.png"
#define EXIT_FILENAME "Exit.png"
#define GAME_OVER_FILENAME "gameover.png"

image_handle_t background_image = NULL;
image_handle_t base_image = NULL;
image_handle_t base_image2 = NULL;
image_handle_t start_single_image = NULL;
image_handle_t quit_image = NULL;
image_handle_t game_over_image = NULL;

spritesheet_handle_t bird_spritesheet = NULL;
sequence_handle_t forward_sequence = NULL;
sequence_handle_t reverse_sequence = NULL;

int menu_width;
int menu_height;
int mouse_x, mouse_y;
int single_width, single_height, multi_width, cheats_width, cheats_height, high_score_width, high_score_height, back_width, back_height;
int replay_width, replay_height;
int screen_mid = SCREEN_WIDTH / 2;
int screen_height_mid = SCREEN_HEIGHT / 2;
char *back = "Back";
char *menu = "Menu";
char *single = "Single Player";
char *multi = "Two Players";
char *cheats = "Cheat Mode";
char *high_score = "View Scores";
char *replay = "Replay";

char *test = "Game Over";
int test_width, test_height;






void GetMouse(void)
{
	mouse_x = tumEventGetMouseX();
	mouse_y = tumEventGetMouseY();
}

void GetSize(void)
{
    tumFontSetSize((ssize_t)30);

	tumGetTextSize(menu, &menu_width, &menu_height);
	tumGetTextSize(single, &single_width, &single_height);
	tumGetTextSize(multi, &multi_width, NULL);	
	tumGetTextSize(cheats, &cheats_width, &cheats_height);
	tumGetTextSize(high_score, &high_score_width, &high_score_height);
	tumGetTextSize(replay, &replay_width, &replay_height);
	tumGetTextSize(back, &back_width, &back_height);
	
}


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
	GetSize();

    if (!tumGetTextSize(menu, &menu_width, NULL))
        checkDraw(tumDrawText(menu, SCREEN_WIDTH * 0.5- menu_width * 0.5,
                              SCREEN_HEIGHT * 0.77, Maroon),
                  __FUNCTION__);

}

void vDrawQuit(void)
{
	int quit_width, quit_height;
	quit_image = tumDrawLoadImage(EXIT_FILENAME);
	tumDrawSetLoadedImageScale(quit_image, 0.2);
	tumDrawGetLoadedImageSize(quit_image, &quit_width, &quit_height);
	checkDraw(tumDrawLoadedImage(quit_image, SCREEN_WIDTH - quit_width - 10, 10 + quit_height), __FUNCTION__);

}

void vDrawStop(void)
{
	// static char stop[100] = { 0 };
    // static int stop_width;

	// ssize_t prev_font_size = tumFontGetCurFontSize();


    // tumFontSetSize((ssize_t)30);
	// sprintf(stop, "[P]ause");

    // if (!tumGetTextSize((char *)stop, &stop_width, NULL))
    //     checkDraw(tumDrawText(stop, SCREEN_WIDTH - stop_width - 100,
    //                           DEFAULT_FONT_SIZE * 0.5, Black),
    //               __FUNCTION__);

	// tumFontSetSize(prev_font_size);

}

void vDrawSubmenu(void)
{
	GetSize();

	checkDraw(tumDrawText(single, screen_mid - single_width / 2, DEFAULT_FONT_SIZE * 5 + 150, Maroon), __FUNCTION__);
	checkDraw(tumDrawText(multi, screen_mid - multi_width / 2, DEFAULT_FONT_SIZE * 10 + 150, Maroon), __FUNCTION__);
	checkDraw(tumDrawText(cheats, screen_mid - cheats_width / 2, DEFAULT_FONT_SIZE * 15 + 150, Maroon), __FUNCTION__);
	checkDraw(tumDrawText(high_score, screen_mid - high_score_width / 2, DEFAULT_FONT_SIZE * 20 + 150, Maroon), __FUNCTION__);
	// checkDraw(tumDrawText(back, screen_mid - back_width / 2, DEFAULT_FONT_SIZE * 25 + 150, Maroon), __FUNCTION__);

}

void vDrawGameOver(void)
{
	game_over_image = tumDrawLoadImage(GAME_OVER_FILENAME);
	tumDrawSetLoadedImageScale(game_over_image, 1.5);
	checkDraw(tumDrawLoadedImage(game_over_image, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4), __FUNCTION__);

	GetSize();

	checkDraw(tumDrawText(replay, screen_mid - replay_width / 2, screen_height_mid - replay_height / 2, Maroon), __FUNCTION__);
	checkDraw(tumDrawText(back, screen_mid - back_width / 2, screen_height_mid - back_height / 2 + 50, Maroon), __FUNCTION__);
}

void vDrawCheatMode(void)
{
	GetSize();

	checkDraw(tumDrawText(back, screen_mid - back_width / 2, SCREEN_HEIGHT * 0.77, Maroon), __FUNCTION__);
}

void vDrawStartSingle(void)
{
	start_single_image = tumDrawLoadImage(SINGLE_START_FILENAME);
	tumDrawSetLoadedImageScale(start_single_image, 1.5);
	checkDraw(tumDrawLoadedImage(start_single_image, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4), __FUNCTION__);

	GetSize();

	tumGetTextSize(test, &test_width, &test_height);
	checkDraw(tumDrawText(test, screen_mid - test_width / 2, SCREEN_HEIGHT * 0.2, Maroon), __FUNCTION__);

}

int vCheckViewScores(void)
{
	GetSize();

	int score_left = screen_mid - high_score_width / 2;
	int score_right = screen_mid + high_score_width / 2;
	int score_up = DEFAULT_FONT_SIZE * 20 + 150;
	int score_down = DEFAULT_FONT_SIZE * 20 + 150 + high_score_height;

	GetMouse();

	if((mouse_x >= score_left) && (mouse_x <= score_right) && (mouse_y >= score_up) && (mouse_y <= score_down)) return 1;

	return 0;
}

int vCheckSingleTest(void)
{
	GetSize();

	int test_left = screen_mid - test_width / 2;
	int test_right = screen_mid + test_width / 2;
	int test_up = SCREEN_HEIGHT * 0.2;
	int test_down = SCREEN_HEIGHT * 0.2 + test_height;

	GetMouse();
	
	if((mouse_x >= test_left) && (mouse_x <= test_right) && (mouse_y >= test_up) && (mouse_y <= test_down)) return 1;

	return 0;
}

int vCheckGameOverBack(void)
{
	GetSize();

	int back_left = screen_mid - back_width / 2;
	int back_right = screen_mid + back_width / 2;
	int back_up = screen_height_mid - back_height / 2 + 50;
	int back_down = screen_height_mid + back_height / 2 + 50;

	GetMouse();
	
	if((mouse_x >= back_left) && (mouse_x <= back_right) && (mouse_y >= back_up) && (mouse_y <= back_down)) return 1;

	return 0;
}

int vCheckCheatModeBack(void)
{
	GetSize();

	int back_left = screen_mid - back_width / 2;
	int back_right = screen_mid + back_width / 2;
	int back_up = SCREEN_HEIGHT * 0.77;
	int back_down = SCREEN_HEIGHT * 0.77 + back_height;

	GetMouse();

	if((mouse_x >= back_left) && (mouse_x <= back_right) && (mouse_y >= back_up) && (mouse_y <= back_down)) return 1;

	return 0;
}

int vCheckReplay(void)
{
	GetSize();

	int replay_left = screen_mid - replay_width / 2;
	int replay_right = screen_mid + replay_width / 2;
	int replay_up = screen_height_mid - replay_height / 2;
	int replay_down = screen_height_mid + replay_height / 2;

	GetMouse();

	if((mouse_x >= replay_left) && (mouse_x <= replay_right) && (mouse_y >= replay_up) && (mouse_y <= replay_down)) return 1;

	return 0;
}

int vCheckMenuMouse(void)
{
	GetSize();

	int menu_left = SCREEN_WIDTH / 2 - menu_width / 2;
	int menu_right = SCREEN_WIDTH / 2 + menu_width / 2;
	int menu_up = SCREEN_HEIGHT * 0.77;
	int menu_down = SCREEN_HEIGHT * 0.77 + menu_height;

	GetMouse();

	if((mouse_x >= menu_left) && (mouse_x <= menu_right) && (mouse_y >= menu_up) && (mouse_y <= menu_down)) return 1;

	return 0;
}

int vCheckSingle(void)
{
	GetSize();

	int single_left = screen_mid - single_width / 2;
	int single_right = screen_mid + single_width / 2;
	int single_up = DEFAULT_FONT_SIZE * 5 + 150;
	int single_down = DEFAULT_FONT_SIZE * 5 + 150 + single_height;

	GetMouse();

	if((mouse_x >= single_left) && (mouse_x <= single_right) && (mouse_y >= single_up) && (mouse_y <= single_down)) return 1;

	return 0;
}

int vCheckCheatMode(void)
{
	GetSize();

	int cheat_left = screen_mid - cheats_width / 2;
	int cheat_right = screen_mid + cheats_width / 2;
	int cheat_up = DEFAULT_FONT_SIZE * 15 + 150;
	int cheat_down = DEFAULT_FONT_SIZE * 15 + 150 + cheats_height;

	GetMouse();

	if((mouse_x >= cheat_left) && (mouse_x <= cheat_right) && (mouse_y >= cheat_up) && (mouse_y <= cheat_down)) return 1;

	return 0;
}

void vDrawBackground(void)
{
	static int image_height;

	if (background_image == NULL) {
		background_image = tumDrawLoadImage(BACKGROUND_FILENAME);
	}
	tumDrawSetLoadedImageScale(background_image, 2);

	if ((image_height = tumDrawGetLoadedImageHeight(background_image)) !=
	    -1)
		checkDraw(tumDrawLoadedImage(background_image, 0, -0.25 * SCREEN_WIDTH),
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
					     SCREEN_HEIGHT * 0.7),
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
					     SCREEN_HEIGHT * 0.7),
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