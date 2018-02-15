#pragma once
#include "GGlobal.h"
class GEvent
{

public:
	enum EType
	{
		eNone,					//��Ч�� event
		eTimer,					//��ʱ������
		eMouseButtonPress,		//��������
		eMouseButtonRelease,	//�����ͷ�
		eMouseButtonDblClick,	//����˫��
		eMouseWheel,			//������
		eMouseMove,				//����ƶ�
		eKeyPress,				//���̼�����
		eKeyRelease,			//���̼��ͷ�
		eFocusIn,				//�ؼ���ý���
		eFocusOut,				//�ؼ�ʧȥ����
		eMouseEnter,			//������
		eMouseLeave,			//����Ƴ�
		eMove,					//���ڡ��ؼ��ƶ���֮��
		eResize,				//���ڡ��ؼ������С��֮��
		eCreate,				//���ڡ��ؼ�������֮ǰ��
		eDestroy,				//���ڡ��ؼ��ͷţ�֮ǰ��
		eShow,					//���ڡ��ؼ���ʾ��֮ǰ��
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