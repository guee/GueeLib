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
	//�Ѵ�����Ϊ���棬Guee�����Ĵ��ڶ������Ƶ��˴����У�����������������������ʾ��
	//wndHandle		: ����������Ĵ��ڵľ��������Ϊ nullptr ��ʹ������ģʽ��
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

