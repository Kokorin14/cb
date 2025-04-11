#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

int Menu_AddButton(char *name, float x, float y, float width, float height, float textS);
void Menu_ShowMenu();
char *Menu_GetButtonName(int buttonId);
int Menu_MouseMove(float x, float y);
int Menu_MouseDown();
void Menu_MouseUp();
#endif // MENU_H_INCLUDED
