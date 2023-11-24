#include <windows.h>
#include <stdint.h>
#include <stdio.h>



#define false 0
#define true 1


typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;

typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;


static bool running = true;



struct
{
    uint32* pixels;
    HDC DC;
    uint32 width;
    uint32 height;
    BITMAPINFO bitmap;

} typedef WindowBuffer;



static WindowBuffer wBuffer = {0};


void
ClearScreen(uint32 color)
{
    uint32* ptr = wBuffer.pixels;

    for(int j = 0; j < wBuffer.height; ++j)
    {
	for(int i = 0; i < wBuffer.width; ++i)
	{
	    *ptr++ = color;
	}
    }
}


void
RainbowScreen()
{
    uint32* ptr = wBuffer.pixels;
    uint8 r = 0;
    uint8 g = 0;
    uint8 b = 0;

    for(int j = 0; j < wBuffer.height; ++j)
    {
	for(int i = 0; i < wBuffer.width; ++i)
	{
	    
	}
    }
}


void
DrawRect(uint32 x0, uint32 y0, uint32 width, uint32 height, uint32 color)
{

    for(int j = y0; j < y0 + height; ++j)
    {
	uint32* ptr = wBuffer.pixels + x0 + j*wBuffer.width;
	for(int i = x0; i < x0 + width; ++i)
	{
	    *ptr++ = color;
	}
    }
   
}


LRESULT CALLBACK
wndCallback(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    
    switch( msg )
    {
	case WM_QUIT:
	case WM_CLOSE:
	{
	    running = false;
	    break;
	}


	case WM_SIZE:
	{

	    RECT wRect;
	    GetWindowRect(hWindow,&wRect);
	    wBuffer.DC = GetDC(hWindow);
	    wBuffer.width = wRect.right - wRect.left;
	    wBuffer.height = wRect.bottom - wRect.top;
	    
	    
	    wBuffer.bitmap.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);	    
	    wBuffer.bitmap.bmiHeader.biWidth = wBuffer.width;
	    wBuffer.bitmap.bmiHeader.biHeight = wBuffer.height;
	    wBuffer.bitmap.bmiHeader.biPlanes = 1;
	    wBuffer.bitmap.bmiHeader.biBitCount = 32;
	    wBuffer.bitmap.bmiHeader.biCompression = BI_RGB;
	    

	    printf("wBuffer.width = %d\n", wBuffer.width);
	    printf("wBuffer.height = %d\n", wBuffer.height);


	    if(wBuffer.pixels)
	    {
		VirtualFree(wBuffer.pixels,0,MEM_RELEASE);
	    }

	    
	    wBuffer.pixels = (uint32*) VirtualAlloc(NULL,
						    sizeof(uint32)*wBuffer.width*wBuffer.height,
						    MEM_COMMIT | MEM_RESERVE,
						    PAGE_READWRITE);

	    
	    break;

	}
	    
	default:
	{
	    result = DefWindowProcA(hWindow, msg, wParam, lParam);
	}
	    
    }
    
    return result;
}




int WINAPI
WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    WNDCLASSA wndClass = {0};
    wndClass.style = CS_HREDRAW|CS_VREDRAW;
    wndClass.lpfnWndProc = wndCallback;
    wndClass.hInstance = hInst;
    wndClass.lpszClassName = "wndClass";

    RegisterClass(&wndClass);
    
    
    CreateWindow("wndClass",
		 "Brickbreak",
		 WS_OVERLAPPEDWINDOW|WS_VISIBLE,
		 CW_USEDEFAULT,CW_USEDEFAULT,
		 1280, 720,
		 NULL, NULL,
		 hInst,
		 NULL);

    
    int vx = 0;
    int vy = 1;
    while(running)
    {

	MSG msg;
	while( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
	{
	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
	}
	
	
	ClearScreen( 0xFFFFFF );
	DrawRect( 50,50, 100, 100, 0xFF0000);

	int n = StretchDIBits(wBuffer.DC,
		      0, 0, wBuffer.width, wBuffer.height,
		      0, 0, wBuffer.width, wBuffer.height,
		      wBuffer.pixels, &wBuffer.bitmap,
		      DIB_RGB_COLORS, SRCCOPY);
	 
	
		

    }
    


    return 0;
}


/* some comment */
