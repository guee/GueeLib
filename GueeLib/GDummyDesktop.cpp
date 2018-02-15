#include "stdafx.h"
#include "GDummyDesktop.h"
#include "GWidget.h"

GDummyDesktop::GDummyDesktop()
{
	m_objectType		= eDesktop;
}


GDummyDesktop::~GDummyDesktop()
{
}

bool GDummyDesktop::bindWindow( HWND wndHandle )
{
	return false;
}

bool GDummyDesktop::exec()
{
#ifdef WIN32
	MSG msg;

	// 主消息循环: 
	while ( GetMessage( &msg, nullptr, 0, 0 ) )
	{
		//if ( !TranslateAccelerator( msg.hwnd, hAccelTable, &msg ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

	}
#endif
	return true;
}

int32_t GDummyDesktop::screenCount()
{
	if ( 0 == m_screens.size() ) regetScreenInfo();
	return m_screens.size();
}

GDummyDesktop::SScreenInfo GDummyDesktop::screenInfo( int32_t index )
{
	if ( 0 == m_screens.size() ) regetScreenInfo();
	if ( index >= 0 && index < m_screens.size() )
		return m_screens[index];
	return SScreenInfo();
}

GRect GDummyDesktop::desktopRect()
{
	if ( 0 == m_screens.size() ) regetScreenInfo();
	return m_destkopRect;
}

GRect GDummyDesktop::desktopWork()
{
	if ( 0 == m_screens.size() ) regetScreenInfo();
	return m_destkopWork;
}

#ifdef WIN32
bool GDummyDesktop::createNativeWindow( GWidget * widget )
{
	static const wchar_t*	classNames[]	= {
		L"GueeWidget_Base",
		L"GueeWidget_NoClose",
	};
	WNDCLASSEXW	wndClass	= { 0 };
	DWORD		dwError		= 0;
	DWORD		dwStyle		= 0;
	DWORD		dwStyleEx	= 0;

	wndClass.cbSize			= sizeof( WNDCLASSEXW );
	wndClass.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS
		| ( widget->attribute( GWidget::eWA_NoCloseButton ) ? CS_NOCLOSE : 0 );
	wndClass.lpfnWndProc	= WindowProc;
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance		= GetModuleHandle( nullptr );
	wndClass.hIcon			= 0;
	wndClass.hIconSm		= 0;
	wndClass.hCursor		= LoadCursor( nullptr, IDC_ARROW );
	wndClass.hbrBackground	= (HBRUSH)( COLOR_WINDOW + 1 );
	wndClass.lpszMenuName	= 0;
	wndClass.lpszClassName	= widget->attribute( GWidget::eWA_NoCloseButton ) ? classNames[1] : classNames[0];

	if ( 0 == RegisterClassEx( &wndClass ) )
	{
		dwError	= GetLastError();
		if ( ERROR_CLASS_ALREADY_EXISTS != dwError )
			return false;
	}
	GMargins		border	= NativeWindowSytle( widget, dwStyle, dwStyleEx );
	const GRect&	rect	= widget->borderRect();

	HWND	hWnd	= CreateWindowEx( dwStyleEx, wndClass.lpszClassName,
		widget->text().c_str(), dwStyle,
		rect.left(), rect.top(), rect.width(), rect.height(),
		(HWND)widget->rootNativePopup(), nullptr,
		wndClass.hInstance, widget );
	if ( hWnd )
	{
		widget->setBorderSize( border );
		widget->associateNativeWindow( hWnd );
		m_natives[hWnd]	= widget;
	}
	else
	{
		dwError	= GetLastError();
	}
	return hWnd ? true : false;
}

void GDummyDesktop::moveNativeWindow( GWidget * widget )
{
	GRect	dest	= widget->marginRect();
	if ( widget->parentWidget()->objectType() != eDesktop )
	{
		//SBox&	parBox	= widget->parentWidget()->m_box;
		if ( widget->parentWidget()->attribute( GWidget::eWA_SystemWindowFrame ) )
		{
			dest.moveLeft( widget->parentWidget()->contentRect().x() + dest.x() );
			dest.moveTop( widget->parentWidget()->contentRect().y() + dest.y() );
		}
		else
		{
			dest.moveLeft( widget->parentWidget()->contentRect().x() + widget->parentWidget()->borderSize().left()
				+ widget->parentWidget()->marginSize().left() + dest.x() );
			dest.moveTop( widget->parentWidget()->contentRect().y() + widget->parentWidget()->borderSize().top()
				+ widget->parentWidget()->marginSize().top() + dest.y() );
		}
	}
	MoveWindow( HWND( widget->nativeWindow() ), dest.x(), dest.y(), dest.width(), dest.height(), TRUE );
}

GMargins GDummyDesktop::getNativeWindowBorder( GWidget * widget )
{
	DWORD			dwStyle		= 0;
	DWORD			dwStyleEx	= 0;
	return NativeWindowSytle( widget, dwStyle, dwStyleEx );
}

GMargins GDummyDesktop::NativeWindowSytle( GWidget * widget, DWORD & dwStyle, DWORD & dwStyleEx )
{
	dwStyle		= 0;
	dwStyleEx	= 0;
	dwStyle	|= widget->isPopupDialog() ? WS_POPUP : WS_CHILD;
	dwStyle	|= widget->attribute( GWidget::eWA_Disable ) ? WS_DISABLED : 0;
	dwStyleEx	|= widget->attribute( GWidget::eWA_InputTransparent ) ? WS_EX_TRANSPARENT : 0;
	//如果使用系统的窗口框架，就创建有边框的窗口。
	if ( widget->attribute( GWidget::eWA_WindowFrame ) && widget->attribute( GWidget::eWA_SystemWindowFrame ) )
	{
		dwStyle	|= widget->attribute( GWidget::eWA_ResizeBorder ) ? WS_THICKFRAME : WS_BORDER;
		if ( widget->attribute( GWidget::eWA_TitleBar ) )
		{
			dwStyle	|= ( WS_CAPTION | WS_SYSMENU );
			dwStyle	|= widget->attribute( GWidget::eWA_MinimizeButton ) ? WS_MINIMIZEBOX : 0;
			dwStyle	|= widget->attribute( GWidget::eWA_MaximizeButton ) ? WS_MAXIMIZEBOX : 0;
			dwStyleEx	|= widget->attribute( GWidget::eWA_QuestionButton ) ? WS_EX_CONTEXTHELP : 0;
		}
	}
	else
	{
		dwStyleEx	|= widget->attribute( GWidget::eWA_AlphaDialog ) ? WS_EX_LAYERED : 0;
	}

	GMargins		marRet;
	const GRect&	client		= widget->clientRect();
	RECT			rtWindow	= { client.left(), client.top(), client.right() + 1, client.bottom() + 1 };
	AdjustWindowRect( &rtWindow, dwStyle, false );
	marRet.setLeft( client.left() - rtWindow.left );
	marRet.setTop( client.top() - rtWindow.top );
	marRet.setRight( rtWindow.right - client.right() - 1 );
	marRet.setBottom( rtWindow.bottom - client.bottom() - 1 );
	return marRet;
}

LRESULT GDummyDesktop::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	GDummyDesktop&			desk	= GDummyDesktop::destkop();
	map<HWND, GWidget*>&	wmds	= desk.m_natives;
	if ( WM_MOUSEFIRST <= uMsg && uMsg <= WM_MOUSELAST )
	{

	}
	switch ( uMsg )
	{
	case WM_CREATE:
		if ( lParam )
		{
			CREATESTRUCT*	createStruct	= (CREATESTRUCT*)lParam;
			wmds[hwnd]	= (GWidget*)createStruct->lpCreateParams;
		}
		break;
	case WM_DESTROY:
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		return DefWindowProc( hwnd, uMsg, wParam, lParam );
	}
	return LRESULT();
}

void GDummyDesktop::regetScreenInfo()
{
	m_screens.clear();
	m_destkopRect	= GRect();
	m_destkopWork	= GRect();
	if ( !EnumDisplayMonitors( NULL, NULL, monitorEnumProc, LPARAM( this ) ) )
	{
		return;
	}
}

BOOL GDummyDesktop::monitorEnumProc( HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData )
{
	GDummyDesktop*	thisPtr	= (GDummyDesktop*)dwData;
	MONITORINFOEX	monDesc;
	memset( &monDesc, 0, sizeof( MONITORINFOEX ) );
	monDesc.cbSize	= sizeof( MONITORINFOEX );
	if ( GetMonitorInfo( hMonitor, &monDesc ) )
	{
		SScreenInfo	info;
		info.name	= monDesc.szDevice;
		info.rect.setRect( monDesc.rcMonitor.left, monDesc.rcMonitor.top,
			monDesc.rcMonitor.right - monDesc.rcMonitor.left,
			monDesc.rcMonitor.bottom - monDesc.rcMonitor.top );
		info.work.setRect( monDesc.rcWork.left, monDesc.rcWork.top,
			monDesc.rcWork.right - monDesc.rcWork.left,
			monDesc.rcWork.bottom - monDesc.rcWork.top );

		thisPtr->m_screens.push_back( info );

		if ( thisPtr->m_destkopRect.isEmpty() )
			thisPtr->m_destkopRect	= info.rect;
		else
			thisPtr->m_destkopRect	= thisPtr->m_destkopRect.intersected( info.rect );

		if ( thisPtr->m_destkopWork.isEmpty() )
			thisPtr->m_destkopWork	= info.work;
		else
			thisPtr->m_destkopWork	= thisPtr->m_destkopWork.intersected( info.work );
	}
	return TRUE;
}

#endif
