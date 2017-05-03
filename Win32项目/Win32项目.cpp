// Win32项目.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Win32项目.h"
#pragma comment(lib,"gltools.lib")

#include <GL/glew.h>
#include <GL/glut.h>  
#pragma comment (lib, "winmm.lib")       /* link with Windows MultiMedia lib */
#pragma comment (lib, "opengl32.lib")   /* link with Microsoft OpenGL lib */
#pragma comment (lib, "glu32.lib")       /* link with OpenGL Utility lib */
#pragma comment (lib, "glut32.lib")      /* link with Win32 GLUT lib */ 


#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


HGLRC           hRC = NULL;		// 窗口着色描述表句柄   
HDC             hDC = NULL;		// OpenGL渲染描述表句柄   
HWND            hWnd = NULL;	// 保存我们的窗口句柄   
HINSTANCE       hInstance;		// 保存程序的实例 
bool    keys[256];				// 保存键盘按键的数组   
bool    active = TRUE;			// 窗口的活动标志，缺省为TRUE   
bool    fullscreen = TRUE;		// 全屏标志缺省，缺省设定成全屏模式 

GLfloat	rtri;				// Angle For The Triangle ( NEW )
GLfloat	rquad;				// Angle For The Quad ( NEW )

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height == 0)// 防止被零除   
	{
		height = 1;										// Making Height Equal One
	}
	//下面几行为透视图设置屏幕。意味着越远的东西看起来越小。
	//这么做创建了一个现实外观的场景。此处透视按照基于窗口宽度和高度的45度视角来计算。
	//0.1f，100.0f是我们在场景中所能绘制深度的起点和终点。
	glViewport(0, 0, width, height);					// 重置当前的视口 
	//glMatrixMode(GL_PROJECTION)指明接下来的两行代码将影响projection matrix(投影矩阵)。
	//投影矩阵负责为我们的场景增加透视。 
	glMatrixMode(GL_PROJECTION);						// 选择投影矩阵   
	//glLoadIdentity()近似于重置。
	//它将所选的矩阵状态恢复成其原始状态。
	//调用 glLoadIdentity()之后我们为场景设置透视图。
	glLoadIdentity();									// 重置投影矩阵   

	// 设置视口的大小   
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	//glMatrixMode(GL_MODELVIEW)指明任何新的变换将会影响 modelview matrix(模型观察矩阵)。
	glMatrixMode(GL_MODELVIEW);							// 选择模型观察矩阵   
	glLoadIdentity();									// 重置模型观察矩阵   
}

/*
接下的代码段中，我们将对OpenGL进行所有的设置。
我们将设置清除屏幕所用的颜色，打开深度缓存，启用smooth shading(阴影平滑)，等等。
这个例程直到OpenGL窗口创建之后才会被调用。
此过程将有返回值。
但我们此处的初始化没那么复杂，现在还用不着担心这个返回值。
*/
int InitGL(GLvoid)										// 此处开始对OpenGL进行所有设置
{
	glShadeModel(GL_SMOOTH);							// 启用阴影平滑 
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// 黑色背景 
	//接下来的三行必须做的是关于depth buffer(深度缓存)的。
	glClearDepth(1.0f);									// 设置深度缓存
	glEnable(GL_DEPTH_TEST);							// 启用深度测试   
	glDepthFunc(GL_LEQUAL);								// 所作深度测试的类型
	//接着告诉OpenGL我们希望进行最好的透视修正。
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// 告诉系统对透视进行修正
	return TRUE;										// Initialization Went OK
}

/*
下一段包括了所有的绘图代码。
任何您所想在屏幕上显示的东东都将在此段代码中出现。
*/
int DrawGLScene(GLvoid)									// 从这里开始进行所有的绘制
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 清除屏幕和深度缓存
	glLoadIdentity();									// 重置当前的模型观察矩阵
	//当您调用glLoadIdentity()之后，您实际上将当前点移到了屏幕中心，X坐标轴从左至右，Y坐标轴从下至上，Z坐标轴从里至外。
	//中心左面的坐标值是负值，右面是正值。移向屏幕顶端是正值，移向屏幕底端是负值。移入屏幕深处是负值，移出屏幕则是正值。 

	//glTranslatef(x, y, z)沿着 X, Y 和 Z 轴移动。
	//根据前面的次序，下面的代码沿着X轴左移1.5个单位，Y轴不动(0.0f)，最后移入屏幕6.0f个单位。
	//注意在glTranslatef(x, y, z)中当您移动的时候，您并不是相对屏幕中心移动，而是相对与当前所在的屏幕位置。
	glTranslatef(-1.5f, 0.0f, -6.0f);					// 左移 1.5 单位，并移入屏幕 6.0
	glRotatef(rtri, 0.0f, 0.0f, 1.0f);					// Rotate The Triangle On The Y axis ( NEW )
	//glBegin(GL_TRIANGLES)的意思是开始绘制三角形，glEnd() 告诉OpenGL三角形已经创建好了。
	//本节的简单示例中，我们只画一个三角形。如果要画第二个三角形的话，可以在这三点之后，再加三行代码(3点)。
	glBegin(GL_TRIANGLES);								// Drawing Using Triangles
	//接下来的一行代码设置三角形的第一个顶点(三角形的上顶点)，并使用当前颜色(红色)来绘制。
	glColor3f(1.0f, 0.0f, 0.0f);						// 设置当前色为红色
	glVertex3f(0.0f, 1.0f, 0.0f);						// Top
	glColor3f(0.0f, 1.0f, 0.0f);						// 设置当前色为绿色
	glVertex3f(-1.0f, -1.0f, 0.0f);						// Bottom Left
	glColor3f(0.0f, 0.0f, 1.0f);						// 设置当前色为蓝色
	glVertex3f(1.0f, -1.0f, 0.0f);						// Bottom Right
	glEnd();											// Finished Drawing The Triangle
	//在屏幕的左半部分画完三角形后，我们要移到右半部分来画正方形。为此要再次使用glTranslate。
	glLoadIdentity();									// 重置模型观察矩阵
	glTranslatef(1.5f, 0.0f, -6.0f);						// Move Right 3 Units
	glRotatef(rquad, 1.0f, 0.0f, 0.0f);					//  绕X轴旋转四边形
	//现在使用GL_QUADS绘制正方形。
	glBegin(GL_QUADS);									// Draw A Quad
	//glColor3f(1.0f, 0.0f, 0.0f);						// 将当前色设置为蓝色
	glVertex3f(-1.0f, 1.0f, 0.0f);						// Top Left
	glVertex3f(1.0f, 1.0f, 0.0f);						// Top Right
	//glColor3f(0.0f, 0.1f, 0.0f);						// 将当前色设置为蓝色
	glVertex3f(1.0f, -1.0f, 0.0f);						// Bottom Right
	glVertex3f(-1.0f, -1.0f, 0.0f);						// Bottom Left
	glEnd();											// Done Drawing The Quad
	if (rtri >= 360.0f) rtri -= 360.0;					// control the range of the rtri
	if (rtri <= 0.0f) rtri += 360.0;
	rtri += 0.2f;										// Increase The Rotation Variable For The Triangle ( NEW )
	rquad -= 0.15f;      // 减少四边形的旋转变量
	return TRUE;										// Everything Went OK
}

/*
下一段代码只在程序退出之前调用。
KillGLWindow() 的作用是依次释放着色描述表，设备描述表和窗口句柄。
我已经加入了许多错误检查。
如果程序无法销毁窗口的任意部分，都会弹出带相应错误消息的讯息窗口，告诉您什么出错了。
使您在您的代码中查错变得更容易些。
*/
GLvoid KillGLWindow(GLvoid)								// 正常销毁窗口
{
	if (fullscreen)										// 我们处于全屏模式吗?
	{
		//将NULL作为第一个参数，0作为第二个参数传递强制Windows使用当前存放在注册表中的值(缺省的分辨率、色彩深度、刷新频率，等等)来有效的恢复我们的原始桌面。
		ChangeDisplaySettings(NULL, 0);					// 是的话，切换回桌面
		ShowCursor(TRUE);								// 显示鼠标指针
	}
	//接下来的代码查看我们是否拥有着色描述表(hRC)
	if (hRC)											// 我们拥有OpenGL渲染描述表吗? 
	{
		//如果存在着色描述表的话，下面的代码将查看我们能否释放它(将 hRC从hDC分开)。
		//这里请注意我使用的的查错方法。
		//基本上我只是让程序尝试释放着色描述表(通过调用wglMakeCurrent(NULL,NULL)，然后我再查看释放是否成功。
		//巧妙的将数行代码结合到了一行。
		if (!wglMakeCurrent(NULL, NULL))				// 我们能否释放DC和RC描述表? 
		{
			MessageBox(NULL, "释放DC或RC失败。", "关闭错误", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// 我们能否删除RC?
		{
			MessageBox(NULL, "释放RC失败。", "关闭错误", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// 将RC设为 NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// 我们能否释放 DC? 
	{
		MessageBox(NULL, "释放DC失败。", "关闭错误", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// 能否销毁窗口?
	{
		MessageBox(NULL, "释放窗口句柄失败。", "关闭失败", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;									// 将 hWnd 设为 NULL
	}
	//最后要做的事是注销我们的窗口类。
	//这允许我们正常销毁窗口，接着在打开其他窗口时，不会收到诸如"Windows Class already registered"(窗口类已注册)的错误消息。
	if (!UnregisterClass("OpenGL", hInstance))			// 能否注销类?
	{
		MessageBox(NULL, "不能注销窗口类。", "关闭错误", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;								// 将 hInstance 设为 NULL
	}
}

/*
此过程返回布尔变量(TRUE 或 FALSE)。
他还带有5个参数：窗口的标题栏，窗口的宽度，窗口的高度，色彩位数(16/24/32)，和全屏标志(TRUE --全屏模式， FALSE--窗口模式 )。
返回的布尔值告诉我们窗口是否成功创建。

	This Code Creates Our OpenGL Window.  Parameters Are:					*
*	title			- Title To Appear At The Top Of The Window				*
*	width			- Width Of The GL Window Or Fullscreen Mode				*
*	height			- Height Of The GL Window Or Fullscreen Mode			*
*	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
*	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	//当我们要求Windows为我们寻找相匹配的象素格式时，Windows寻找结束后将模式值保存在变量PixelFormat中。
	GLuint		PixelFormat;			// 保存查找匹配的结果
	//wc用来保存我们的窗口类的结构。窗口类结构中保存着我们的窗口信息。通过改变类的不同字段我们可以改变窗口的外观和行为。
	//每个窗口都属于一个窗口类。当您创建窗口时，您必须为窗口注册类。
	WNDCLASS	wc;						// 窗口类结构
	//dwExStyle和dwStyle存放扩展和通常的窗口风格信息。
	//我使用变量来存放风格的目的是为了能够根据我需要创建的窗口类型(是全屏幕下的弹出窗口还是窗口模式下的带边框的普通窗口)；
	//来改变窗口的风格。
	DWORD		dwExStyle;				// 扩展窗口风格 
	DWORD		dwStyle;				// 窗口风格
	//下面的5行代码取得矩形的左上角和右下角的坐标值。
	//我们将使用这些值来调整我们的窗口使得其上的绘图区的大小恰好是我们所需的分辨率的值。
	//通常如果我们创建一个640x480的窗口，窗口的边框会占掉一些分辨率的值。
	RECT		WindowRect;				// 取得矩形的左上角和右下角的坐标值
	WindowRect.left = (long)0;			// 将Left设为 0
	WindowRect.right = (long)width;		// 将Right  设为要求的宽度
	WindowRect.top = (long)0;			// 将Top设为 0
	WindowRect.bottom = (long)height;	// 将Bottom 设为要求的高度

	fullscreen = fullscreenflag;		// 设置全局全屏标志

	//下一部分的代码中，我们取得窗口的实例，然后定义窗口类。 
	//CS_HREDRAW 和 CS_VREDRAW 的意思是无论何时，只要窗口发生变化时就强制重画。
	hInstance = GetModuleHandle(NULL);				// 取得我们窗口的实例
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// 移动时重画，并为窗口取得DC
	wc.lpfnWndProc = (WNDPROC)WndProc;				// WndProc处理消息
	wc.cbClsExtra = 0;								// 无额外窗口数据
	wc.cbWndExtra = 0;								// 无额外窗口数据
	wc.hInstance = hInstance;						// 设置实例
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// 装入缺省图标
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);		// 装入鼠标指针
	wc.hbrBackground = NULL;						// GL不需要背景
	wc.lpszMenuName = NULL;							// 不需要菜单
	wc.lpszClassName = "OpenGL";					// 设定类名字

	//现在注册类名字。如果有错误发生，弹出错误消息窗口。
	//按下上面的OK按钮后，程序退出
	if (!RegisterClass(&wc))									// 尝试注册窗口类
	{
		MessageBox(NULL, "注册窗口失败", "错误", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;											// 退出并返回FALSE 
	}
	/*
	在切换到全屏模式时，有几件十分重要的事您必须牢记。
	必须确保您在全屏模式下所用的宽度和高度等同于窗口模式下的宽度和高度。
	最最重要的是要在创建窗口之前设置全屏模式。
	
	这里的代码中，您无需再担心宽度和高度，它们已被设置成与显示模式所对应的大小。
	*/
	if (fullscreen)												// 要尝试全屏模式吗?
	{
		DEVMODE dmScreenSettings;								// 设备模式
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// 确保内存清空为零
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Devmode 结构的大小
		dmScreenSettings.dmPelsWidth = width;					// 所选屏幕宽度
		dmScreenSettings.dmPelsHeight = height;					// 所选屏幕高度
		dmScreenSettings.dmBitsPerPel = bits;					// 每象素所选的色彩深度
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		
		//上面的代码中，我们分配了用于存储视频设置的空间。设定了屏幕的宽，高，色彩深度。
		//下面的代码我们尝试设置全屏模式。
		//我们在dmScreenSettings中保存了所有的宽，高，色彩深度讯息。
		//下一行使用ChangeDisplaySettings来尝试切换成与dmScreenSettings所匹配模式。
		// 尝试设置显示模式并返回结果。注: CDS_FULLSCREEN 移去了状态条。   
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// 如果模式未能设置成功，我们将进入以下的代码。
			//如果不能匹配全屏模式，弹出消息窗口，提供两个选项：在窗口模式下运行或退出。
			if (MessageBox(NULL, "全屏模式在当前显卡上设置失败！\n使用窗口模式？", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;		// 选择窗口模式(Fullscreen=FALSE) 
			}
			else
			{
				// 弹出一个对话框，告诉用户程序结束
				MessageBox(NULL, "程序将被关闭", "错误", MB_OK | MB_ICONSTOP);
				return FALSE;									//  退出并返回 FALSE
			}
		}
	}
	//由于全屏模式可能失败，用户可能决定在窗口下运行，我们需要在设置屏幕/窗口之前，再次检查fullscreen的值是TRUE或FALSE。
	//如果我们使用窗口而不是全屏模式，我们在扩展窗体风格中增加了 WS_EX_WINDOWEDGE，增强窗体的3D感观
	if (fullscreen)												// 仍处于全屏模式吗?
	{
		dwExStyle = WS_EX_APPWINDOW;							// 扩展窗体风格
		dwStyle = WS_POPUP;										// 窗体风格
		ShowCursor(FALSE);										// 隐藏鼠标指针
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// 扩展窗体风格
		dwStyle = WS_OVERLAPPEDWINDOW;							//  窗体风格
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	// 调整窗口达到真正要求的大小

	//窗口并检查窗口是否成功创建。								// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,						// 扩展窗体风格
		("OpenGL"),							// 类名字
		(title),							// 窗口标题
		dwStyle |							// 选择的窗体属性
		WS_CLIPSIBLINGS |					// 必须的窗体风格属性
		WS_CLIPCHILDREN,					// 必须的窗体风格属性
		0, 0,								// 窗口位置(左上角)
		WindowRect.right - WindowRect.left,	// 计算调整好的窗口宽度
		WindowRect.bottom - WindowRect.top,	// 计算调整好的窗口高度
		NULL,								// 无父窗口
		NULL,								// 无菜单
		hInstance,							// 实例
		NULL)))								//不向WM_CREATE传递任何东西
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL, ("不能创建一个窗口设备描述表"), ("错误"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd 告诉窗口我们所希望的东东，即窗口使用的像素格式
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// 上述格式描述符的大小
		1,											// 版本号
		PFD_DRAW_TO_WINDOW |						// 格式支持窗口
		PFD_SUPPORT_OPENGL |						// 格式必须支持OpenGL
		PFD_DOUBLEBUFFER,							// 必须支持双缓冲
		PFD_TYPE_RGBA,								// 申请 RGBA 格式
		bits,										// 选定色彩深度
		0, 0, 0, 0, 0, 0,							// 忽略的色彩位
		0,											// 无Alpha缓存
		0,											// 忽略Shift Bit
		0,											// 无累加缓存
		0, 0, 0, 0,									// 忽略聚集位
		16,											// 16位 Z-缓存 (深度缓存)
		0,											// 无蒙板缓存
		0,											// 无辅助缓存
		PFD_MAIN_PLANE,								// 主绘图层
		0,											// Reserved
		0, 0, 0										// 忽略层遮罩
	};
	//如果前面创建窗口时没有错误发生，我们接着尝试取得OpenGL设备描述表。若无法取得DC，弹出错误消息程序退出(返回FALSE)。
	if (!(hDC = GetDC(hWnd)))						// 取得设备描述表了么?
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL, ("不能创建一种相匹配的像素格式"), "错误", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}
	//设法为OpenGL窗口取得设备描述表后，我们尝试找到对应与此前我们选定的象素格式的象素格式。如果Windows不能找到的话，弹出错误消息，并退出程序(返回FALSE)。
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Windows 找到相应的象素格式了吗?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "不能设置像素格式", "错误", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	//Windows 找到相应的象素格式后，尝试设置象素格式。如果无法设置，弹出错误消息，并退出程序(返回FALSE)。
	if (!SetPixelFormat(hDC, PixelFormat, &pfd))	// 能够设置象素格式么?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "不能设置像素格式", "错误", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	//正常设置象素格式后，尝试取得着色描述表。如果不能取得着色描述表的话，弹出错误消息，并退出程序(返回FALSE)。
	if (!(hRC = wglCreateContext(hDC)))				// 能否取得着色描述表?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "不能创建OpenGL渲染描述表", "错误", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	//如果到现在仍未出现错误的话，我们已经设法取得了设备描述表和着色描述表。接着要做的是激活着色描述表。
	if (!wglMakeCurrent(hDC, hRC))					// 尝试激活着色描述表
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "不能激活当前的OpenGL渲然描述表", "错误", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	// 一切顺利的话，OpenGL窗口已经创建完成，接着可以显示它啦。将它设为前端窗口(给它更高的优先级)，并将焦点移至此窗口。
	// 然后调用ReSizeGLScene 将屏幕的宽度和高度设置给透视OpenGL屏幕。
	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority 略略提高优先级
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window 设置键盘的焦点至此窗口 
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen 设置透视 GL 屏幕
	//跳转至 InitGL()，这里可以设置光照、纹理、等等任何需要设置的东东。
	//您可以在 InitGL()内部自行定义错误检查，并返回 TRUE (一切正常)或FALSE (有什么不对)。
	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	//到这里可以安全的推定创建窗口已经成功了。我们向WinMain()返回TRUE，告知WinMain()没有错误，以防止程序退出。

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(HWND	hWnd,				// Handle For This Window 窗口的句柄
	UINT	uMsg,			// Message For This Window 窗口的消息
	WPARAM	wParam,			// Additional Message Information 附加的消息内容
	LPARAM	lParam)			// Additional Message Information 附加的消息内容
{
	switch (uMsg)									// Check For Windows Messages 检查Windows消息
	{//如果uMsg等于WM_ACTIVE，查看窗口是否仍然处于激活状态。如果窗口已被最小化，将变量active设为FALSE。如果窗口已被激活，变量active的值为TRUE。 
		case WM_ACTIVATE:							// Watch For Window Activate Message 监视窗口激活消息
		{
			if (!HIWORD(wParam))					// Check Minimization State 检查最小化状态
			{
				active = TRUE;						// Program Is Active 程序处于激活状态
			}
			else
			{
				active = FALSE;						// Program Is No Longer Active 程序不再激活
			}

			return 0;								// Return To The Message Loop 返回消息循环
		}
		//如果消息是WM_SYSCOMMAND(系统命令)，再次比对wParam。
		//如果wParam 是 SC_SCREENSAVE 或 SC_MONITORPOWER的话，不是有屏幕保护要运行，就是显示器想进入节电模式。返回0可以阻止这两件事发生。 
		case WM_SYSCOMMAND:							// Intercept System Commands 系统中断命令
		{
			switch (wParam)							// Check System Calls 检查系统调用
			{
			case SC_SCREENSAVE:						// Screensaver Trying To Start? 屏保要运行?
			case SC_MONITORPOWER:					// Monitor Trying To Enter Powersave? 显示器要进入节电模式?
				return 0;							// Prevent From Happening 阻止发生
			}
			break;									// Exit
		}
		//如果 uMsg是WM_CLOSE，窗口将被关闭。
		//我们发出退出消息，主循环将被中断。变量done被设为TRUE，WinMain()的主循环中止，程序关闭。
		case WM_CLOSE:								// Did We Receive A Close Message? 收到Close消息?
		{
			PostQuitMessage(0);						// Send A Quit Message 发出退出消息
			return 0;								// Jump Back
		}
		//如果键盘有键按下，通过读取wParam的信息可以找出键值。
		//我将键盘数组keys[ ]相应的数组组成员的值设为TRUE。
		//这样以后就可以查找key[ ]来得知什么键被按下。允许同时按下多个键。 
		case WM_KEYDOWN:							// Is A Key Being Held Down? 有键按下么?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}
		//同样，如果键盘有键释放，通过读取wParam的信息可以找出键值。
		//然后将键盘数组keys[ ]相应的数组组成员的值设为FALSE。
		case WM_KEYUP:								// Has A Key Been Released? 有键放开么?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}
		//当调整窗口时，uMsg 最后等于消息WM_SIZE。
		//读取lParam的LOWORD 和HIWORD可以得到窗口新的宽度和高度。
		//将他们传递给ReSizeGLScene()，OpenGL场景将调整为新的宽度和高度。 
		case WM_SIZE:								// Resize The OpenGL Window 调整OpenGL窗口大小
		{
			ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}
	//其余无关的消息被传递给DefWindowProc，让Windows自行处理。
	// Pass All Unhandled Messages To DefWindowProc 向 DefWindowProc传递所有未处理的消息。
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE	hInstance,			// 当前窗口实例
	HINSTANCE	hPrevInstance,		// 前一个窗口实例 
	LPSTR		lpCmdLine,			// 命令行参数
	int			nCmdShow)			// 窗口显示状态 
{
	//我们设置两个变量。
	//msg 用来检查是否有消息等待处理。
	//done的初始值设为FALSE。这意味着我们的程序仍未完成运行。只要程序done保持FALSE，程序继续运行。一旦done的值改变为TRUE，程序退出。 
	MSG		msg;									// Windows Message Structure Windowsx消息结构
	BOOL	done = FALSE;							// Bool Variable To Exit Loop 用来退出循环的Bool 变量

													// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL, "你想在全屏模式下运行么？", "设置全屏模式", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		fullscreen = FALSE;							// Windowed Mode
	}
	//接着创建OpenGL窗口。CreateGLWindow函数的参数依次为标题、宽度、高度、色彩深度，以及全屏标志。
	//就这么简单！我很欣赏这段代码的简洁。
	//如果未能创建成功，函数返回FALSE。程序立即退出。 
	// Create Our OpenGL Window
	if (!CreateGLWindow("NeHe's颜色实例", 640, 480, 16, fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while (!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting? 有消息在等待吗?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message? 收到退出消息?
			{
				done = TRUE;						// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages 
			{										//如果不是退出消息，我们翻译消息，然后发送消息，使得WndProc() 或 Windows能够处理他们。 
				TranslateMessage(&msg);				// Translate The Message 翻译消息
				DispatchMessage(&msg);				// Dispatch The Message 发送消息
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			//如果没有消息，绘制我们的OpenGL场景。代码的第一行查看窗口是否激活。如果按下ESC键，done变量被设为TRUE，程序将会退出。 
			if (active)								// Program Active? 程序激活的么?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done = TRUE;					// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen 不是退出的时候，刷新屏幕
				{
					DrawGLScene();					// Draw The Scene 绘制场景 
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering) 交换缓存 (双缓存)
				}
			}
			//下面的一点代码是最近新加的(05-01-00)。允许用户按下F1键在全屏模式和窗口模式间切换。
			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1] = FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen = !fullscreen;				// Toggle Fullscreen / Windowed Mode
														// Recreate Our OpenGL Window
				if (!CreateGLWindow("NeHe's 第一个多边形程序", 640, 480, 16, fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}
	//如果done变量不再是FALSE，程序退出。正常销毁OpenGL窗口，将所有的内存释放，退出程序。
	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}


/*
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}
*/


//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
/*
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}
*/
//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
/*
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
*/
/*
这部分函数实现了窗口的绘图部分代码
传入窗口的句柄，岁该句柄指向的窗口进行操作
*/
/*
void Event(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: 在此处添加使用 hdc 的任何绘图代码...
	//hdc = BeginPaint(hWnd, &ps);
	// TODO: 在此添加任意绘图代码...  
	SetTextColor(hdc, RGB(0, 0, 0));//黑色的画笔  
	TextOut(hdc, 100, 100, "黑色的画笔", strlen("黑色的画笔"));//在(100,100)这个文件，输出Helloworld。  
	EndPaint(hWnd, &ps);
}
*/
//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
/*
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
			Event(hWnd);
            
			//PAINTSTRUCT ps;
            //HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
			//hdc = BeginPaint(hWnd, &ps);
			// TODO: 在此添加任意绘图代码...  
			//SetTextColor(hdc, RGB(0, 0, 0));//黑色的画笔  
			//TextOut(hdc, 100, 100, L"Helloworld", strlen("Helloworld"));//在(100,100)这个文件，输出Helloworld。  
            //EndPaint(hWnd, &ps);
			
			
			//HGLRC           hRC = NULL;   // 窗口着色描述表句柄   
			//HDC             hDC = NULL;   // OpenGL渲染描述表句柄   
			//HWND            hWnd = NULL;  // 保存我们的窗口句柄   
			//HINSTANCE       hInstance;  // 保存程序的实例 
			//bool    keys[256];  // 保存键盘按键的数组   
			//bool    active = TRUE;    // 窗口的活动标志，缺省为TRUE   
			//bool    fullscreen = TRUE;    // 全屏标志缺省，缺省设定成全屏模式 
			
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
*/

// “关于”框的消息处理程序。
/*
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
*/