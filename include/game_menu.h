#ifndef __GAME_MENU_H__
#define __GAME_MENU_H__

void EnterStartMenu(void* enter_start_menu);
void EnterSettingMenu(void* enter_setting_menu);
void EnterSingleStart(void *enter_single_start);
void EnterGameOver(void *enter_game_over);
void EnterCheatMode(void *enter_cheat_mode);
void EnterViewScores(void * enter_view_scores);

void ExitStartMenu(void* exit_start_menu);
void ExitSettingMenu(void* exit_setting_menu);
void ExitSingleStart(void *eixt_single_start);
void ExitGameOver(void *exit_game_over);
void ExitCheatMode(void *exit_cheat_mode);
void ExitViewScores(void *exit_view_scores);

void RunSingleStart(void *run_single_start);




void GameMenu(void* pvParameters);


#endif