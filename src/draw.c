#include "TUM_Draw.h"
#include "TUM_Event.h"
#include "TUM_Ball.h"
#include "TUM_Sound.h"
#include "TUM_Utils.h"
#include "TUM_Print.h"
#include "TUM_Font.h"

#include "defines.h"
#include "main.h"
#include "demo_tasks.h"
#include "draw.h"
#include "async_message_queues.h"
#include "async_sockets.h"
#include "buttons.h"
#include "sm.h"
#include "bird.h"
#include "pipes.h"

#include <stdlib.h>

#define GET_READY_FILENAME "getReady.png"
#define SINGLE_START_FILENAME "message.png"
#define EXIT_FILENAME "Exit.png"
#define GAME_OVER_FILENAME "gameover.png"
#define SCOREBOARD_FILENAME "scoreboard.png"

#define LOGO_FILENAME "logo.png"
#define BACKGROUND_FILENAME "background-day.png"
#define BASE_FILENAME "base.png"
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
image_handle_t logo_image = NULL;
image_handle_t base_image = NULL;
image_handle_t base_image2 = NULL;
image_handle_t start_single_image = NULL;
image_handle_t get_ready_image = NULL;
image_handle_t quit_image = NULL;
image_handle_t game_over_image = NULL;
image_handle_t scoreboard_image = NULL;

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

image_handle_t small_zero = NULL;
image_handle_t small_one = NULL;
image_handle_t small_two = NULL;
image_handle_t small_three = NULL;
image_handle_t small_four = NULL;
image_handle_t small_five = NULL;
image_handle_t small_six = NULL;
image_handle_t small_seven = NULL;
image_handle_t small_eight = NULL;
image_handle_t small_nine = NULL;

spritesheet_handle_t bird_spritesheet = NULL;
spritesheet_handle_t base_spritesheet = NULL;

sequence_handle_t forward_sequence = NULL;
sequence_handle_t reverse_sequence = NULL;
sequence_handle_t base_forward_sequence = NULL;

int score = 0;
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
	if (status)
	{
		if (msg)
			fprints(stderr, "[ERROR] %s, %s\n", msg,
					tumGetErrorMessage());
		else
		{
			fprints(stderr, "[ERROR] %s\n", tumGetErrorMessage());
		}
	}
}

void vDrawLogo(void)
{

	if (logo_image == NULL)
		logo_image = tumDrawLoadImage(LOGO_FILENAME);

	tumDrawSetLoadedImageScale(logo_image, 0.2);

	checkDraw(tumDrawLoadedImage(logo_image, SCREEN_WIDTH / 2 - tumDrawGetLoadedImageWidth(logo_image) / 2, SCREEN_HEIGHT / 2 - tumDrawGetLoadedImageHeight(logo_image) / 2),
			  __FUNCTION__);
}

void vDrawMenu(void)
{
	GetSize();

	if (!tumGetTextSize(menu, &menu_width, NULL))
		checkDraw(tumDrawText(menu, SCREEN_WIDTH * 0.5 - menu_width * 0.5,
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
	scoreboard_image = tumDrawLoadImage(SCOREBOARD_FILENAME);
	tumDrawSetLoadedImageScale(scoreboard_image, 1.3);
	checkDraw(tumDrawLoadedImage(scoreboard_image, SCREEN_WIDTH / 2 - tumDrawGetLoadedImageWidth(scoreboard_image) / 2, SCREEN_HEIGHT / 2.5 - tumDrawGetLoadedImageHeight(scoreboard_image) / 2), __FUNCTION__);

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
	get_ready_image = tumDrawLoadImage(GET_READY_FILENAME);
	tumDrawSetLoadedImageScale(get_ready_image, 1.2);
	checkDraw(tumDrawLoadedImage(get_ready_image, SCREEN_WIDTH / 2 - tumDrawGetLoadedImageWidth(get_ready_image) / 2,
								 SCREEN_HEIGHT / 2 - tumDrawGetLoadedImageHeight(get_ready_image) / 2),
			  __FUNCTION__);

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

	if ((mouse_x >= score_left) && (mouse_x <= score_right) && (mouse_y >= score_up) && (mouse_y <= score_down))
		return 1;

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

	if ((mouse_x >= test_left) && (mouse_x <= test_right) && (mouse_y >= test_up) && (mouse_y <= test_down))
		return 1;

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

	if ((mouse_x >= back_left) && (mouse_x <= back_right) && (mouse_y >= back_up) && (mouse_y <= back_down))
		return 1;

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

	if ((mouse_x >= back_left) && (mouse_x <= back_right) && (mouse_y >= back_up) && (mouse_y <= back_down))
		return 1;

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

	if ((mouse_x >= replay_left) && (mouse_x <= replay_right) && (mouse_y >= replay_up) && (mouse_y <= replay_down))
		return 1;

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

	if ((mouse_x >= menu_left) && (mouse_x <= menu_right) && (mouse_y >= menu_up) && (mouse_y <= menu_down))
		return 1;

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

	if ((mouse_x >= single_left) && (mouse_x <= single_right) && (mouse_y >= single_up) && (mouse_y <= single_down))
		return 1;

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

	if ((mouse_x >= cheat_left) && (mouse_x <= cheat_right) && (mouse_y >= cheat_up) && (mouse_y <= cheat_down))
		return 1;

	return 0;
}

void vDrawBackground(void)
{
	static int image_height;

	if (background_image == NULL)
	{
		background_image = tumDrawLoadImage(BACKGROUND_FILENAME);
	}
	tumDrawSetLoadedImageScale(background_image, 1.5);

	if ((image_height = tumDrawGetLoadedImageHeight(background_image)) !=
		-1)
		checkDraw(tumDrawLoadedImage(background_image, 0, 0),
				  __FUNCTION__);
	else
	{
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
	if (bird_midflap == NULL)
	{
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
	if (bBirdAlive == true)
	{
		tumDrawAnimationDrawFrame(forward_sequence,
								  xTaskGetTickCount() - xLastFrameTime,
								  player1.birdX, player1.birdY);
	}
	else
	{
		tumDrawLoadedImage(bird_midflap, player1.birdX, player1.birdY);
	}

	if (bBirdAlive == true)
	{
		tumDrawAnimationDrawFrame(base_forward_sequence,
								  xTaskGetTickCount() - xLastFrameTime,
								  0, SCREEN_HEIGHT - 140);
	}
	else
	{ // stops at last drawn frame
		tumDrawAnimationDrawFrame(base_forward_sequence, 0, 0,
								  SCREEN_HEIGHT - 140);
	}
}

void vDrawPipes(void)
{
	if (pipe_1 == NULL)
	{
		pipe_1 = tumDrawLoadImage(PIPES_FILENAME);
	}
	if (pipe_2 == NULL)
	{
		pipe_2 = tumDrawLoadImage(PIPES_FILENAME);
	}
	if (pipe_3 == NULL)
	{
		pipe_3 = tumDrawLoadImage(PIPES_FILENAME);
	}
	tumDrawSetLoadedImageScale(pipe_1, 0.5);
	tumDrawSetLoadedImageScale(pipe_2, 0.5);
	tumDrawSetLoadedImageScale(pipe_3, 0.5);

	// pipe 1
	if (bBirdAlive == true)
	{
		checkDraw(tumDrawLoadedImage(pipe_1, pipe1.x, pipe1.y),
				  __FUNCTION__);
		pipe1.x -= 2;

		if (pipe1.x <= -52)
		{
			pipe1.x = SCREEN_WIDTH + 116;
			pipe1.y = -350 + rand() % 310;
		}
	}
	else
	{ // stops moving when bird is dead
		checkDraw(tumDrawLoadedImage(pipe_1, pipe1.x, pipe1.y),
				  __FUNCTION__);
	}
	// pipe 2
	if (bBirdAlive == true)
	{
		checkDraw(tumDrawLoadedImage(pipe_2, pipe2.x, pipe2.y),
				  __FUNCTION__);
		pipe2.x -= 2;

		if (pipe2.x <= -52)
		{
			pipe2.x = SCREEN_WIDTH + 116;
			pipe2.y = -350 + rand() % 310;
		}
	}
	else
	{
		checkDraw(tumDrawLoadedImage(pipe_2, pipe2.x, pipe2.y),
				  __FUNCTION__);
	}

	// pipe3
	if (bBirdAlive == true)
	{
		checkDraw(tumDrawLoadedImage(pipe_3, pipe3.x, pipe3.y),
				  __FUNCTION__);
		pipe3.x -= 2;

		if (pipe3.x <= -52)
		{
			pipe3.x = SCREEN_WIDTH + 116;
			pipe3.y = -350 + rand() % 310;
		}
	}
	else
	{
		checkDraw(tumDrawLoadedImage(pipe_3, pipe3.x, pipe3.y),
				  __FUNCTION__);
	}
}
void vDrawScore(void)
{
	countScore();

	int digit1 = score % 10;
	int digit10 = ((score - digit1) / 10) % 10;
	int digit100 = ((score - 10 * digit10 - digit1) / 100) % 10;

	if (zero == NULL)
	{
		zero = tumDrawLoadImage(ZERO_FILENAME);
	}
	if (one == NULL)
	{
		one = tumDrawLoadImage(ONE_FILENAME);
	}
	if (two == NULL)
	{
		two = tumDrawLoadImage(TWO_FILENAME);
	}
	if (three == NULL)
	{
		three = tumDrawLoadImage(THREE_FILENAME);
	}
	if (four == NULL)
	{
		four = tumDrawLoadImage(FOUR_FILENAME);
	}
	if (five == NULL)
	{
		five = tumDrawLoadImage(FIVE_FILENAME);
	}
	if (six == NULL)
	{
		six = tumDrawLoadImage(SIX_FILENAME);
	}
	if (seven == NULL)
	{
		seven = tumDrawLoadImage(SEVEN_FILENAME);
	}
	if (eight == NULL)
	{
		eight = tumDrawLoadImage(EIGHT_FILENAME);
	}
	if (nine == NULL)
	{
		nine = tumDrawLoadImage(NINE_FILENAME);
	}

	switch (digit1)
	{
	case 0:
		tumDrawLoadedImage(zero, 205, 72);
		break;
	case 1:
		tumDrawLoadedImage(one, 205, 72);
		break;
	case 2:
		tumDrawLoadedImage(two, 205, 72);
		break;
	case 3:
		tumDrawLoadedImage(three, 205, 72);
		break;
	case 4:
		tumDrawLoadedImage(four, 205, 72);
		break;
	case 5:
		tumDrawLoadedImage(five, 205, 72);
		break;
	case 6:
		tumDrawLoadedImage(six, 205, 72);
		break;
	case 7:
		tumDrawLoadedImage(seven, 205, 72);
		break;
	case 8:
		tumDrawLoadedImage(eight, 205, 72);
		break;
	case 9:
		tumDrawLoadedImage(nine, 205, 72);
		break;
	default:
		break;
	}

	if (score >= 10)
	{
		switch (digit10)
		{
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

	if (score >= 100)
	{
		switch (digit100)
		{
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

void vDrawScoreboard(void) {

	//Add new sign if neew highscore (bool newHigh)
	vSetHighscore();

	if (small_zero == NULL)
	{
		small_zero = tumDrawLoadImage(ZERO_FILENAME);
		tumDrawSetLoadedImageScale(small_zero,0.8);
	}
	if (small_one == NULL)
	{
		small_one = tumDrawLoadImage(ONE_FILENAME);
		tumDrawSetLoadedImageScale(small_one,0.8);
	}
	if (small_two == NULL)
	{
		small_two = tumDrawLoadImage(TWO_FILENAME);
		tumDrawSetLoadedImageScale(small_two,0.8);
	}
	if (small_three == NULL)
	{
		small_three = tumDrawLoadImage(THREE_FILENAME);
		tumDrawSetLoadedImageScale(small_three,0.8);
	}
	if (small_four == NULL)
	{
		small_four = tumDrawLoadImage(FOUR_FILENAME);
		tumDrawSetLoadedImageScale(small_four,0.8);
	}
	if (small_five == NULL)
	{
		small_five = tumDrawLoadImage(FIVE_FILENAME);
		tumDrawSetLoadedImageScale(small_five,0.8);
	}
	if (small_six == NULL)
	{
		small_six = tumDrawLoadImage(SIX_FILENAME);
		tumDrawSetLoadedImageScale(small_six,0.8);
	}
	if (small_seven == NULL)
	{
		small_seven = tumDrawLoadImage(SEVEN_FILENAME);
		tumDrawSetLoadedImageScale(small_seven,0.8);
	}
	if (small_eight == NULL)
	{
		small_eight = tumDrawLoadImage(EIGHT_FILENAME);
		tumDrawSetLoadedImageScale(small_eight,0.8);
	}
	if (small_nine == NULL)
	{
		small_nine = tumDrawLoadImage(NINE_FILENAME);
		tumDrawSetLoadedImageScale(small_nine,0.8);
	}


	int digit1 = highscore % 10;
	int digit10 = ((highscore - digit1) / 10) % 10;
	int digit100 = ((highscore - 10 * digit10 - digit1) / 100) % 10;

	

	switch (digit1)
	{
		case 0:
			tumDrawLoadedImage(small_zero, 325, SCREEN_HEIGHT / 2 - 70);
			break;
		case 1:
			tumDrawLoadedImage(small_one, 325, SCREEN_HEIGHT / 2 - 70);
			break;
		case 2:
			tumDrawLoadedImage(small_two, 325, SCREEN_HEIGHT / 2 - 70);
			break;
		case 3:
			tumDrawLoadedImage(small_three, 325, SCREEN_HEIGHT / 2 - 70);
			break;
		case 4:
			tumDrawLoadedImage(small_four, 325, SCREEN_HEIGHT / 2 - 70);
			break;
		case 5:
			tumDrawLoadedImage(small_five, 325, SCREEN_HEIGHT / 2 - 70);
			break;
		case 6:
			tumDrawLoadedImage(small_six, 325, SCREEN_HEIGHT / 2 - 70);
			break;
		case 7:
			tumDrawLoadedImage(small_seven, 325, SCREEN_HEIGHT / 2 - 70);
			break;
		case 8:
			tumDrawLoadedImage(small_eight, 325, SCREEN_HEIGHT / 2 - 70);
			break;
		case 9:
			tumDrawLoadedImage(small_nine, 325, SCREEN_HEIGHT / 2 - 70);
			break;
		default:
			break;

	}

	if (highscore >= 10)
	{
		switch (digit10)
		{
		case 0:
			tumDrawLoadedImage(small_zero, 305, SCREEN_HEIGHT / 2 - 70);
			break;
		case 1:
			tumDrawLoadedImage(small_one, 305, SCREEN_HEIGHT / 2 - 70);
			break;
		case 2:
			tumDrawLoadedImage(small_two, 305, SCREEN_HEIGHT / 2 - 70);
			break;
		case 3:
			tumDrawLoadedImage(small_three, 305, SCREEN_HEIGHT / 2 - 70);
			break;
		case 4:
			tumDrawLoadedImage(small_four, 305, SCREEN_HEIGHT / 2 - 70);
			break;
		case 5:
			tumDrawLoadedImage(small_five, 305, SCREEN_HEIGHT / 2 - 70);
			break;
		case 6:
			tumDrawLoadedImage(small_six, 305, SCREEN_HEIGHT / 2 - 70);
			break;
		case 7:
			tumDrawLoadedImage(small_seven, 305, SCREEN_HEIGHT / 2 - 70);
			break;
		case 8:
			tumDrawLoadedImage(small_eight, 305, SCREEN_HEIGHT / 2 - 70);
			break;
		case 9:
			tumDrawLoadedImage(small_nine, 305, SCREEN_HEIGHT / 2 - 70);
			break;
		default:
			break;
		}
	}

	if (highscore >= 100)
	{
		switch (digit100)
		{
		case 0:
			tumDrawLoadedImage(small_zero, 285, SCREEN_HEIGHT / 2 - 70);
			break;
		case 1:
			tumDrawLoadedImage(small_one, 285, SCREEN_HEIGHT / 2 - 70);
			break;
		case 2:
			tumDrawLoadedImage(small_two, 285, SCREEN_HEIGHT / 2 - 70);
			break;
		case 3:
			tumDrawLoadedImage(small_three, 285, SCREEN_HEIGHT / 2 - 70);
			break;
		case 4:
			tumDrawLoadedImage(small_four, 285, SCREEN_HEIGHT / 2 - 70);
			break;
		case 5:
			tumDrawLoadedImage(small_five, 285, SCREEN_HEIGHT / 2 - 70);
			break;
		case 6:
			tumDrawLoadedImage(small_six, 285, SCREEN_HEIGHT / 2 - 70);
			break;
		case 7:
			tumDrawLoadedImage(small_seven, 285, SCREEN_HEIGHT / 2 - 70);
			break;
		case 8:
			tumDrawLoadedImage(small_eight, 285, SCREEN_HEIGHT / 2 - 70);
			break;
		case 9:
			tumDrawLoadedImage(small_nine, 285, SCREEN_HEIGHT / 2 - 70);
			break;
		default:
			break;
		}

	}
}