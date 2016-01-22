#include <emgl/pcdriver.h>
#include <emgl/debug.h>
#include <windows.h>
#include <stdint.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>

#pragma comment(lib, "opengl32.lib")

const TCHAR g_pcdrv_ClassName [] = TEXT("EMGL class");

struct _pcdrv_status {
	U32 *lcdBuffer;
	HWND hWnd;
	HGLRC hRC;
	HDC hDC;
	GLuint texture;
	U16 texWidth;
	U16 texHeight;
} g_pcdrv_status;

const emgl_driverAPI_t g_pcdrv = {
	&g_pcdrv_status,
	pcdrv_poll,
	pcdrv_getPixel,
	pcdrv_setPixel,
	pcdrv_fillRect,
	pcdrv_drawLineH,
	pcdrv_drawLineV,
	pcdrv_drawBitmap,
	pcdrv_init,
	pcdrv_getSize
};

const emgl_driverAPI_t *pcdrv_CreateDriver()
{
	return &g_pcdrv;
}

void pcdrv_fillRect(void * api, coord_t x1, coord_t y1, coord_t x2, 
	coord_t y2, emgl_color_t lcdColor)
{
	struct _pcdrv_status *sts = api;
	U32 color = emgl_colorConvToRGBA8888(lcdColor);
	if (x1 > x2)
	{
		coord_t x = x1;
		x1 = x2;
		x2 = x;
	}
	if (y1 > y2)
	{
		coord_t y = y1;
		y1 = y2;
		y2 = y;
	}

	if (x1 >= LCD_WIDTH || x2 < 0 || y1 >= LCD_HEIGHT || y2 < 0)
	{
		// Out screen
		EMGL_LOG(EMGL_LOGLVL_WARN|EMGL_LOGMOD_DRIVER, 
			"pcdrv_fillRect: Drawing outside screen\n");
		return;
	}
	if (x1 < 0)
	{
		x1 = 0;
	}
	if (x2 >= LCD_WIDTH)
	{
		x2 = LCD_WIDTH - 1;
	}
	if (y1 < 0)
	{
		y1 = 0;
	}
	if (y2 >= LCD_HEIGHT)
	{
		y2 = LCD_HEIGHT - 1;
	}

	for (coord_t x = x1; x <= x2; x++)
	{
		for (coord_t y = y1; y <= y2; y++)
		{
			sts->lcdBuffer[y*LCD_WIDTH+x] = color;
		}
	}
}

void pcdrv_drawLineH(void *api, coord_t x1, coord_t x2, coord_t y,
	emgl_color_t lcdColor)
{
	struct _pcdrv_status *sts = api;
	U32 color = emgl_colorConvToRGBA8888(lcdColor);
	if (x1 > x2)
	{
		coord_t x = x1;
		x1 = x2;
		x2 = x;
	}

	if (y < 0 || y >= LCD_HEIGHT || x1 >= LCD_WIDTH || x2 < 0)
	{
		// Outside screen
		EMGL_LOG(EMGL_LOGLVL_WARN | EMGL_LOGMOD_DRIVER,
			"pcdrv_drawLineH: Drawing outside screen\n");
		return;
	}
	if (x1 < 0)
	{
		x1 = 0;
	}
	if (x2 >= LCD_WIDTH)
	{
		x2 = LCD_WIDTH - 1;
	}
	for (coord_t x = x1; x <= x2; x++)
	{
		sts->lcdBuffer[y*LCD_WIDTH+x] = color;
	}
}

void pcdrv_drawLineV(void *api, coord_t y1, coord_t y2, coord_t x,
	emgl_color_t lcdColor)
{
	struct _pcdrv_status *sts = api;
	if (y1 > y2)
	{
		coord_t y = y1;
		y1 = y2;
		y2 = y;
	}

	if (x < 0 || x >= LCD_WIDTH || y1 >= LCD_HEIGHT || y2 < 0)
	{
		// Outside screen
		EMGL_LOG(EMGL_LOGLVL_WARN | EMGL_LOGMOD_DRIVER,
			"pcdrv_drawLineV: Drawing outside screen\n");
		return;
	}
	if (y1 < 0)
	{
		y1 = 0;
	}
	if (y2 >= LCD_HEIGHT)
	{
		y2 = LCD_HEIGHT - 1;
	}

	U32 color = emgl_colorConvToRGBA8888(lcdColor);
	for (coord_t y = y1; y <= y2; y++)
	{
		sts->lcdBuffer[y*LCD_WIDTH + x] = color;
	}
}

emgl_color_t pcdrv_getPixel(void * api, coord_t x, coord_t y)
{
	struct _pcdrv_status *sts = api;
	if (x >= LCD_WIDTH || y >= LCD_HEIGHT || x < 0 || y < 0)
	{
		EMGL_LOG(EMGL_LOGLVL_WARN | EMGL_LOGMOD_DRIVER,
			"pcdrv_getPixel: Location outside screen\n");
		return emgl_colorConvFromRGBA8888(0x00000000);
	}
	
	return emgl_colorConvFromRGBA8888(sts->lcdBuffer[y*LCD_WIDTH + x]);
}

void pcdrv_setPixel(void * api, coord_t x, coord_t y,
	emgl_color_t lcdColor)
{
	struct _pcdrv_status *sts = api;
	if (x >= LCD_WIDTH || y >= LCD_HEIGHT || x < 0 || y < 0)
	{
		EMGL_LOG(EMGL_LOGLVL_WARN | EMGL_LOGMOD_DRIVER,
			"pcdrv_setPixel: Location outside screen\n");
		return;
	}

	U32 color = emgl_colorConvToRGBA8888(lcdColor);
	
	sts->lcdBuffer[y*LCD_WIDTH + x] = color;
}

void pcdrv_drawBitmap(void * api, coord_t x, coord_t y, coord_t width,
	coord_t height, const emgl_color_t *data)
{
	EMGL_ASSERT("pcdrv_drawBitmap: width >= 0 && height >= 0",
		width >= 0 && height >= 0);
	struct _pcdrv_status *sts = api;
	coord_t startx = 0;
	coord_t starty = 0;
	coord_t endx = width;
	coord_t endy = height;

	if (x < 0)
	{
		startx = -x;
	}
	if (y < 0)
	{
		starty = -y;
	}
	if (x + width > LCD_WIDTH)
	{
		endx = LCD_WIDTH - x;
	}
	if (y + height > LCD_HEIGHT)
	{
		endy = LCD_HEIGHT - y;
	}
	if (startx >= width || starty >= width || endx <= 0 || endy <= 0)
	{
		// Completely outside screen
		EMGL_LOG(EMGL_LOGLVL_WARN | EMGL_LOGMOD_DRIVER,
			"pcdrv_drawBitmap: Drawing outside screen\n");
		return;
	}

	for (coord_t iy = starty; iy < endy; iy++)
	{
		for (coord_t ix = startx; ix < endx; ix++)
		{
			U32 color = emgl_colorConvToRGBA8888(emgl_colorModeGetPixel(data, iy*width+ix));

			sts->lcdBuffer[(y + iy)*LCD_WIDTH + (x + ix)] = color;
		}
	}
}

void pcdrv_onRender(struct _pcdrv_status *sts)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, sts->texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, LCD_WIDTH, LCD_HEIGHT, GL_BGRA_EXT,
		GL_UNSIGNED_BYTE, sts->lcdBuffer);

	glBegin(GL_QUADS);
	glNormal3f(0.0f,0.0f,1.0f);
	glTexCoord2f(LCD_WIDTH / (float) sts->texWidth, 
		LCD_HEIGHT / (float) sts->texHeight);
	glVertex2f(1.0f, 1.0f);
	glTexCoord2f(LCD_WIDTH / (float) sts->texWidth, 0.0f);
	glVertex2f(1.0f, -1.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(0.0f, LCD_HEIGHT / (float) sts->texHeight);
	glVertex2f(-1.0f, 1.0f);
	glEnd();
	SwapBuffers(sts->hDC);
}

LRESULT CALLBACK pcdrv_wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Handle all the messages
	switch (msg)
	{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

HWND pcdrv_createWindow()
{
	WNDCLASSEX wc;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HWND hwnd;
	RECT size = {0,0,LCD_WIDTH*PCDRV_ZOOM, LCD_HEIGHT*PCDRV_ZOOM};
	UINT style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	UINT exStyle = WS_EX_CLIENTEDGE;

	// Register the window class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = pcdrv_wndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;//(HBRUSH) COLOR_WINDOW+1;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_pcdrv_ClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("Window registration failed!"),
			TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
		abort();
	}

	// Adjust the size of the window
	AdjustWindowRectEx(&size, style, FALSE, exStyle);

	// Create the window
	hwnd = CreateWindowEx(exStyle, g_pcdrv_ClassName,
		TEXT(PCDRV_WINDOW_TITLE), style, CW_USEDEFAULT, CW_USEDEFAULT,
		size.right-size.left, size.bottom-size.top, NULL, NULL,
		hInstance, NULL);
	if (hwnd == NULL)
	{
		MessageBox(NULL, TEXT("Window creation failed!"),
			TEXT("Error!"), MB_ICONEXCLAMATION | MB_OK);
		abort();
	}
	return hwnd;
}

U16 fitPower2(U16 x)
{
	for (int i = 14; i >= 0; i--)
	{
		if ((x & (1 << i)) != 0)
		{
			return 1 << (i+1);
		}
	}
	return 0;
}

void pcdrv_initGL(struct _pcdrv_status *sts)
{
	// Initialize GL
	sts->hDC = GetDC(sts->hWnd);
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int iFormat = ChoosePixelFormat(sts->hDC, &pfd);
	SetPixelFormat(sts->hDC, iFormat, &pfd);
	// Activate GL
	sts->hRC = wglCreateContext(sts->hDC);
	wglMakeCurrent(sts->hDC, sts->hRC);

	// Configure GL
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.0f,0.0f,0.0f,0.0f);

	// Create objects
	glGenTextures(1, &sts->texture);
	glBindTexture(GL_TEXTURE_2D, sts->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	sts->texWidth = fitPower2(LCD_WIDTH);
	sts->texHeight = fitPower2(LCD_HEIGHT);
	
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, sts->texWidth, sts->texHeight, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);


}

void pcdrv_destroy(struct _pcdrv_status *sts)
{
	// Destroy LCD buffer
	if (sts->lcdBuffer != NULL)
	{
		free(sts->lcdBuffer);
		sts->lcdBuffer = NULL;
	}
	// Destroy openGL
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(sts->hRC);
	ReleaseDC(sts->hWnd, sts->hDC);
}

void pcdrv_init(void *api)
{
	// Create buffer for drawing
	struct _pcdrv_status *sts = api;
	ZeroMemory(sts, sizeof(struct _pcdrv_status));
	sts->lcdBuffer = (U32 *) emgl_malloc(LCD_WIDTH*LCD_HEIGHT*sizeof(U32));
	for (int x = 0; x < LCD_WIDTH; x++)
	{
		for (int y = 0; y < LCD_HEIGHT; y++)
		{
			sts->lcdBuffer[y*LCD_WIDTH + x] = 0xFF000000;
		}
	}
	sts->hWnd = pcdrv_createWindow();
	pcdrv_initGL(sts);
	ShowWindow(sts->hWnd, SW_SHOWNORMAL);
	UpdateWindow(sts->hWnd);
}

void pcdrv_poll(void * api)
{
	struct _pcdrv_status *sts = api;
	MSG Msg;
	if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
	{
		if (Msg.message == WM_QUIT)
		{
			// WM_QUIT message
			pcdrv_destroy(sts);
			exit(Msg.wParam);
		}
		else {
			// Handle the message
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}
	else {
		pcdrv_onRender(sts);
	}
}

void pcdrv_getSize(void *api, coord_t *width, coord_t *height)
{
	(void)api;
	if (width != NULL)
	{
		*width = LCD_WIDTH;
	}
	if (height != NULL)
	{
		*height = LCD_HEIGHT;
	}
}
