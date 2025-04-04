#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../stb-master/stb-master/stb_easy_font.h"
#include "menu.h"
#define nameLen 20


typedef struct
{
 char name[nameLen]; // длина имени кнопки
 float vert[8]; // 4 вершины по 2 координаты
 char isHover, isDown;
 //text
 float buffer[50*nameLen]; // для координат примитивов
 int num_quads; //для количества
 float textPosX,textPosY,textS; // координаты расположения текста и
} TBtn;
TBtn *btn = 0;
int btnCnt = 0;

float mouseX, mouseY;

int Menu_AddButton(char *name, float x, float y, float width, float height,  float textS)
{

 btnCnt++; //заводим счетчик кнопок
 btn = realloc(btn, sizeof(btn[0]) * btnCnt); //выделяем память под нужное
//количество
 snprintf(btn[btnCnt-1].name, nameLen, "%s", name); //выделение памяти и запись
//имени
 float *vert = btn[btnCnt-1].vert; //передача координат кнопки
 vert[0]=vert[6]=x;
 vert[2]=vert[4]=x+width;
 vert[1]=vert[3]=y;
 vert[5]=vert[7]=y+height;
 btn[btnCnt-1].isHover = 0;
 btn[btnCnt-1].isDown = 0;

 TBtn *b= btn + btnCnt - 1; //записываем в буфер данные кнопки
 b->num_quads = stb_easy_font_print(0, 0, name, 0, b->buffer, sizeof(b->buffer)); // запись
//координат вершин элементов имени
 b->textPosX = x +(width-stb_easy_font_width(name)*textS)/2.0;
 b->textPosY = y +(height-stb_easy_font_height(name)*textS)/2.0;
 b->textPosY+= textS*9;
 b->textS = textS;

 return btnCnt-1;
}

void ShowButton(int buttonId)
{
    TBtn btn1 = btn[buttonId]; //пересылка всех данных в буфер
    glVertexPointer(2, GL_FLOAT, 0, btn1.vert); //запись координат в VBO

    glEnableClientState(GL_VERTEX_ARRAY); //разрешение
        if (btn1.isDown) glColor3f(0.2, 1, 0.2);
            else if (btn1.isHover) glColor3f(0.8, 0.8, 1);
                else glColor3f(0.6,0.6,0.8); //цвет кнопки
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); //отрисовка кнопки
        glColor3f(1,1,1); //цвет обводки
        glLineWidth(1); // толщина обводки кнопки
        glDrawArrays(GL_LINE_LOOP,0,4); //отрисовка обводки
    glDisableClientState(GL_VERTEX_ARRAY);

        glPushMatrix(); //матрицу в стек
        glColor3f(0,0,0); //цвет текста
        glTranslatef(btn1.textPosX,btn1.textPosY, 0); //перенос матрицы для отрисовки текста
        glScalef(btn1.textS, -btn1.textS, 1); //масштабирование текста
        glEnableClientState(GL_VERTEX_ARRAY); // разрешение
        glVertexPointer(2, GL_FLOAT, 16, btn1.buffer); //вектор для отрисовки
        glDrawArrays(GL_QUADS, 0, btn1.num_quads*4); //отрисовка текста
        glDisableClientState(GL_VERTEX_ARRAY);
        glPopMatrix();
}

void Menu_ShowMenu()
{
    for (int i = 0; i < btnCnt; i++)
        ShowButton(i);

}

char isCoordInButton(int buttonId, float x, float y)
{
    float *vert = btn[buttonId].vert;
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

int Menu_MouseDown()
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


