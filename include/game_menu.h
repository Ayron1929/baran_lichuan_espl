#ifndef __GAME_MENU_H__
#define __GAME_MENU_H__

void EnterStartMenu(void* enter_start_menu);

void EnterSettingMenu(void* enter_setting_menu);

void EnterSingleStart(void *enter_single_start);

void EnterGameOver(void *enter_game_over);

void ExitStartMenu(void* exit_start_menu);

void ExitSettingMenu(void* exit_setting_menu);

void ExitSingleStart(void *eixt_single_start);
void ExitGameOver(void *exit_game_over);



void GameMenu(void* pvParameters);


#endif