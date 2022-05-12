#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <cmath>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           1250,                 /* The programs width */
           720,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

void Draw8Points(HDC hdc,int xc, int yc, int a, int b)
{
    SetPixel(hdc, xc+a, yc+b, RGB(255,0,0));
    SetPixel(hdc, xc-a, yc+b, RGB(255,0,0));
    SetPixel(hdc, xc-a, yc-b, RGB(255,0,0));
    SetPixel(hdc, xc+a, yc-b, RGB(255,0,0));
    SetPixel(hdc, xc+b, yc+a, RGB(255,0,0));
    SetPixel(hdc, xc-b, yc+a, RGB(255,0,0));
    SetPixel(hdc, xc-b, yc-a, RGB(255,0,0));
    SetPixel(hdc, xc+b, yc-a, RGB(255,0,0));
}

int Round(double x)
{
    return (int)(x+0.5);
}

void parammLine(HDC hdc, int x1, int y1, int x2, int y2)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    for(double i = 0; i < 1; i += 0.001)
    {
        int x = x1 + (i * dx);
        int y = y1 + (i * dy);
        SetPixel(hdc,x,y,RGB(255,0,0));
    }
}

void drawLines(HDC hdc, int xc, int yc, int R)
{
    parammLine(hdc,xc,yc,xc,yc+R);
    parammLine(hdc,xc,yc,xc+R,yc);
    parammLine(hdc,xc,yc,xc,yc-R);
    parammLine(hdc,xc,yc,xc-R,yc);

    parammLine(hdc,xc,yc,xc+R*cos(0.785398),yc+R*sin(0.785398));
    parammLine(hdc,xc,yc,xc+R*cos(0.785398),yc-R*sin(0.785398));
    parammLine(hdc,xc,yc,xc-R*cos(0.785398),yc+R*sin(0.785398));
    parammLine(hdc,xc,yc,xc-R*cos(0.785398),yc-R*sin(0.785398));

}

void drawCircle(HDC hdc, int xc, int yc, int x2, int y2)
{
    double dy = y2 - yc;
    double dx = x2 - xc;
    int R = sqrt(pow(dy,2) + pow(dx,2));

    int x=R,y=0;
    double theta=0,dtheta=1.0/R;
    Draw8Points(hdc,xc,yc,x,y);
    while(x > y)
    {
        theta += dtheta;
        x = Round(R*cos(theta));
        y = Round(R*sin(theta));
        Draw8Points(hdc,xc,yc,x,y);
    }
    drawLines(hdc,xc,yc,R);
}

int xc,yc,x2,y2;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
        case WM_LBUTTONUP:
            xc = LOWORD(lParam);
            yc = HIWORD(lParam);
            break;

        case WM_RBUTTONUP:
            x2 = LOWORD(lParam);
            y2 = HIWORD(lParam);
            drawCircle(hdc,xc,yc,x2,y2);
            break;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
