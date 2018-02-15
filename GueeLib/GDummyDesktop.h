#pragma once
#include <vector>
#include <string>
#include <map>
using namespace std;
#include "GWidget.h"
#include "GRect.h"
#include "GMargins.h"

#ifdef WIN32
#include <Windows.h>
#endif

class GDummyDesktop : protected GBaseNode
{
public:
	~GDummyDesktop();

	static GDummyDesktop& destkop() {
		static GDummyDesktop desk;
		return desk;
	}
	//把窗口做为桌面，Guee创建的窗口都将绘制到此窗口中，而不是在真正的桌面上显示。
	//wndHandle		: 被当做桌面的窗口的句柄。设置为 nullptr 则不使用这种模式。
#ifdef WIN32
	bool bindWindow( HWND wndHandle );

#endif
	bool isWindowDummyDesktop() { return m_wndHandle ? true : false; }

	bool exec();

	struct SScreenInfo
	{
		GRect	rect;
		GRect	work;
		wstring	name;
	};

	int32_t	screenCount();
	SScreenInfo	screenInfo( int32_t index = 0 );
	GRect	desktopRect();
	GRect	desktopWork();
private:
	GDummyDesktop();
	friend		GWidget;
	GRect		m_destkopRect;
	GRect		m_destkopWork;
	vector<SScreenInfo>	m_screens;
	GMargins getNativeWindowBorder( GWidget* widget );
#ifdef WIN32
	HWND		m_wndHandle;
	map<HWND, GWidget*>	m_natives;
	bool createNativeWindow( GWidget* widget );
	void moveNativeWindow( GWidget* widget );
	GMargins NativeWindowSytle( GWidget* widget, DWORD& dwStyle, DWORD& dwStyleEx );
	static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam	);
	void regetScreenInfo();
	static BOOL CALLBACK monitorEnumProc( HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData );
#endif
};

