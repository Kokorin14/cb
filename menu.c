#include <gl/gl.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../stb-master/stb-master/stb_easy_font.h"
#include "menu.h"
#define nameLen 20


typedef struct
{
    char name[nameLen];
    float vert[8];
    char isHover, isDown;
    //text
    float buffer[50*nameLen];
     int num_quads;
     float textPosX,textPosY,textS;
} TBtn;
TBtn *btn = 0;
int btnCnt = 0;

float mouseX,mouseY;

int Menu_AddButton(char *name, float x, float y, float width, float height, float textS)
{
    btnCnt++;
    btn = realloc(btn, sizeof(btn[0]) * btnCnt); //âûäåëÿåì ïàìÿòü ïîä íóæíîå
    snprintf(btn[btnCnt-1].name, nameLen, "%s", name); //âûäåëåíèå ïàìÿòè è çàïèñü
    float *vert = btn[btnCnt-1].vert; //ïåðåäà÷à êîîðäèíàò êíîïêè
    vert[0]=vert[6]=x;
    vert[2]=vert[4]=x+width;
    vert[1]=vert[3]=y;
    vert[5]=vert[7]=y+height;
    btn[btnCnt-1].isHover = 0;
    btn[btnCnt-1].isDown = 0;

    TBtn *b= btn + btnCnt - 1; //указывает на текущую кнопку
     b->num_quads = stb_easy_font_print(0, 0, name, 0, b->buffer, sizeof(b->buffer)); // çàïèñü
    //êîîðäèíàò âåðøèí ýëåìåíòîâ èìåíè
     b->textPosX = x +(width-stb_easy_font_width(name)*textS)/2.0;
     b->textPosY = y +(height-stb_easy_font_height(name)*textS)/2.0;
     b->textS = textS;
     b->textPosY+= textS*2;
     return btnCnt-1;
}

void ShowButton(int buttonId)
{
     TBtn btn1 = btn[buttonId]; //пересылка всех данных в буфер
     glVertexPointer(2, GL_FLOAT, 0, btn1.vert); //запись координат в VBO
     glEnableClientState(GL_VERTEX_ARRAY); //разрешение
     if (btn1.isDown) glColor3f(0.2, 1, 0.2);
            else if (btn1.isHover) glColor3f(0.8, 0.8, 1);
                else glColor3f(0.6,0.6,0.8);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); //отрисовка кнопки
        glColor3f(0.5,0.5,0.5); //цвет обводки
        glLineWidth(1); // толщина обводки кнопки
        glDrawArrays(GL_LINE_LOOP,0,4); //отрисовка обводки
        glDisableClientState(GL_VERTEX_ARRAY);
    glPushMatrix();
        glColor3f(0,0,0);
        glTranslatef(btn1.textPosX,btn1.textPosY, 0);
        glScalef(btn1.textS, btn1.textS, 1);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 16, btn1.buffer);
        glDrawArrays(GL_QUADS, 0, btn1.num_quads*4);
        glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

void Menu_ShowMenu()
{
    for (int i = 0; i < btnCnt; i++)
        ShowButton(i);
}

char isCoordInButton(int buttonId, float x, float y) //проверка попадание по кнопки
{
    float *vert = btn[buttonId].vert; //адрес первого элемента масива
    return (x > vert[0]) && (y > vert[1]) && (x < vert[4]) && (y < vert[5]);
}

int Menu_MouseMove(float x, float y)
{
    mouseX = x;
    mouseY = y;
    int moveBtn = -1;
    for(int i = 0; i < btnCnt; i++)
        if (isCoordInButton(i, mouseX, mouseY))
    {
        btn[i].isHover = 1;
        moveBtn = i;
    }
    else
    {
        btn[i].isHover = 0;
        btn[i].isDown = 0;
    }
    return moveBtn;
}

int Menu_MouseDown()//функция нажатия
{
    int downBtn = -1;
    for (int i=0; i<btnCnt; i++)
        if (isCoordInButton(i, mouseX, mouseY))
    {
        btn[i].isDown = 1;
        downBtn = i;
    }
    return downBtn;
}

void Menu_MouseUp()
{
    for (int i=0; i<btnCnt; i++)
        btn[i].isDown = 0;
}

char *Menu_GetButtonName(int buttonId)
{
    return btn[buttonId].name;
}
