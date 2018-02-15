#pragma once
#include "GGlobal.h"
class GEvent
{

public:
	enum EType
	{
		eNone,					//无效的 event
		eTimer,					//定时器触发
		eMouseButtonPress,		//鼠标键按下
		eMouseButtonRelease,	//鼠标键释放
		eMouseButtonDblClick,	//鼠标键双击
		eMouseWheel,			//鼠标滚轮
		eMouseMove,				//鼠标移动
		eKeyPress,				//键盘键按下
		eKeyRelease,			//键盘键释放
		eFocusIn,				//控件获得焦点
		eFocusOut,				//控件失去焦点
		eMouseEnter,			//鼠标进入
		eMouseLeave,			//鼠标移出
		eMove,					//窗口、控件移动（之后）
		eResize,				//窗口、控件重设大小（之后）
		eCreate,				//窗口、控件创建（之前）
		eDestroy,				//窗口、控件释放（之前）
		eShow,					//窗口、控件显示（之前）
		eHide,					// widget is hidden
		eClose,					// request to close widget
		eQuit,					// request to quit application
		eWindowActivate,		// window was activated
		eWindowDeactivate,		// window was deactivated


		OkRequest,                         // CE (Ok) button pressed
		HelpRequest,                       // CE (?)  button pressed



		ToolTip,
		WhatsThis,
		StatusTip,

		NonClientAreaMouseMove,
		
		NonClientAreaMouseButtonPress,
		NonClientAreaMouseButtonRelease,
		NonClientAreaMouseButtonDblClick,
	};
	GEvent( EType type ) : m_type( type ) {}
	~GEvent() {}

	EType type() const { return m_type; }
private:
	EType	m_type;
};

class GMouseEvent : public GEvent
{
public:
	GMouseEvent( EType type, uint32_t wParam, uint32_t lParam ) : GEvent( type )
	{

	}
private:

};