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
           1200,                 /* The programs width */
           700,                 /* and height in pixels */
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

int calcRadius(int xc, int yc, int x2, int y2)
{
    double dy = y2 - yc;
    double dx = x2 - xc;
    int R = sqrt(pow(dy,2) + pow(dx,2));
    return R;
}

void drawCircle(HDC hdc, int xc, int yc, int x2, int y2)
{
    int R = calcRadius(xc,yc,x2,y2);

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
}

struct Vector{
	double v[2];
	Vector(double x = 0, double y = 0)
    {
        v[0] = x; v[1] = y;
    }
	double& operator[](int i){
	    return v[i];
	}
};

void DrawHermiteCurve(HDC hdc,Vector& p1, Vector& T1, Vector& p2, Vector& T2, int xc, int yc, int x2, int y2)
{
	double a0 = p1[0], a1 = T1[0],
		a2 = -3 * p1[0] - 2 * T1[0] + 3 * p2[0] - T2[0],
		a3 = 2 * p1[0] + T1[0] - 2 * p2[0] + T2[0];

	double b0 = p1[1], b1 = T1[1],
		b2 = -3 * p1[1] - 2 * T1[1] + 3 * p2[1] - T2[1],
		b3 = 2 * p1[1] + T1[1] - 2 * p2[1] + T2[1];

    int R = calcRadius(xc,yc,x2,y2);

	for (double t = 0; t <= 1; t += 0.001)
	{
		double t2 = t*t, t3 = t2*t;
		double x = a0 + a1*t + a2*t2 + a3*t3;
		double y = b0 + b1*t + b2*t2 + b3*t3;

		if((x-xc)*(x-xc) + (y-yc)*(y-yc) - R*R > 0)     // equation of circle
        {
            SetPixel(hdc,Round(x), Round(y),RGB(0,0,255));
        }else{
            SetPixel(hdc,Round(x), Round(y),RGB(255,0,0));
        }
	}
}

/*  This function is called by the Windows function DispatchMessage()  */
int xc,yc,x2,y2,R;
int counter = 1;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);
    static Vector p[4];
    switch (message)                  /* handle the messages */
    {
        case WM_LBUTTONUP:
            if(counter == 1){
                xc = LOWORD(lParam);
                yc = HIWORD(lParam);
                counter++;
            }
            else if(counter == 2){
                x2 = LOWORD(lParam);
                y2 = HIWORD(lParam);
                R = calcRadius(xc,yc,x2,y2);
                drawCircle(hdc,xc,yc,x2,y2);        //The outer circle
                drawCircle(hdc,xc-(R/3),yc-(R/3),xc-(R/5),yc-(R/5)); // inner circle
                drawCircle(hdc,xc+(R/3),yc-(R/3),xc+(R/5),yc-(R/5)); // inner circle
                counter++;
            }
            else if(counter == 3){
                p[0] = Vector(LOWORD(lParam), HIWORD(lParam));
                counter++;
            }
            else if(counter == 4){
                p[1] = Vector(LOWORD(lParam), HIWORD(lParam));
                counter++;
            }
            else if(counter == 5){
                p[2] = Vector(LOWORD(lParam), HIWORD(lParam));
                counter++;
            }
            else if(counter == 6){
                p[3] = Vector(LOWORD(lParam), HIWORD(lParam));
                Vector T1(3 * (p[1][0] - p[0][0]), 3 * (p[1][1] - p[0][1]));
                Vector T2(3 * (p[3][0] - p[2][0]), 3 * (p[3][1] - p[2][1]));
                DrawHermiteCurve(hdc, p[0], T1, p[3], T2, xc,yc,x2,y2);
                counter = 3;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
