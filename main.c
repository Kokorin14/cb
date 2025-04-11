#include <windows.h>
#include <gl/gl.h>
#include <stdbool.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../../stb-master/stb-master/stb_image.h"
#include "menu.h"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

void Init() {
  Menu_AddButton("Start", 10, 10, 100, 30, 2);
  Menu_AddButton("Stop", 10, 50, 100, 30, 2);
  Menu_AddButton("Quit", 10, 90, 100, 30, 2);
}

bool showPlayer = false;

void MouseDown() // обработ нажатия кн
{
  int buttonId = Menu_MouseDown();
  if (buttonId < 0)
    return;
  char *name = Menu_GetButtonName(buttonId);
  printf("%s\n", name);
  if (strcmp(name, "Quit") == 0) {
    // Логика выхода из программы
    printf("QUIT PROGRAMM\n");
    exit(0); // Завершение программы
  }

  if (strcmp(name, "Start") == 0) {
    showPlayer = true;
    printf("Start button clicked.\n");
  }
}

unsigned int texture;

void Game_fon() {
  int width, height, cnt;
  unsigned char *data = stbi_load("fon.png", &width, &height, &cnt, 0);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
               cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(data);
}

unsigned int playerTexture;

void Game_list() // Changed to reflect purpose: loading the player texture
{
  int width, height, cnt;
  unsigned char *data =
      stbi_load("sprayt-list.png", &width, &height, &cnt, 0);  // Load player sprite
  glGenTextures(1, &playerTexture); // Different texture ID for the player
  glBindTexture(GL_TEXTURE_2D, playerTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
               cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(data);
}

float vertex[] = {-1, -1, 0, 1, -1, 0, 1, 1, 0, -1, 1, 0};
float texCoord[] = {0, 1, 1, 1, 1, 0, 0, 0};

void Game_Show() {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);

  glColor3f(1, 1, 1);
  glPushMatrix();
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertex);
  glTexCoordPointer(2, GL_FLOAT, 0, texCoord);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glPopMatrix();
}

static float svertix[] = {-0.2f, -0.1f,
                          0.2f,  -0.1f,
                          0.2f,  0.1f,
                          -0.2f, 0.1f
};

static float TexCord[] = {0.0f, 1.0f,
                         1.0f, 1.0f,
                         1.0f, 0.0f,
                         0.0f, 0.0f
};

void Game_player() {
  if (showPlayer) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, playerTexture);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.99);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, svertix);
    glTexCoordPointer(2, GL_FLOAT, 0, TexCord);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_TEXTURE_2D);  // Cleanup!
  }
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{

    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1000,
                          700,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);


    Init();

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            Game_fon();
            Game_Show();
            Game_list();
            Game_player();


            glPushMatrix();
            RECT rct;
            GetClientRect(hwnd,&rct);
            glOrtho(0, rct.right, rct.bottom, 0,1,-1);
            Menu_ShowMenu();
            glPopMatrix();

            SwapBuffers(hDC);

            theta += 1.0f;
            Sleep (1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_MOUSEMOVE:
            Menu_MouseMove(LOWORD(lParam), HIWORD(lParam));//пересылка координат мыши
        break;

        case WM_LBUTTONDOWN:
            MouseDown();
        break;

        case WM_LBUTTONUP:
            Menu_MouseUp();
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{

    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

