#pragma once
#include "GXmlDocument.h"
#include "GPoint.h"
#include "GSize.h"
#include "GRect.h"
#include "GMargins.h"
#include "GImage.h"
#include "GFont.h"
#include "GColor.h"

class GWidget : protected GBaseNode
{
public:
	GWidget( GWidget* parent = nullptr );
	~GWidget();

	enum EWidgetAttribute
	{
		eWA_NativeWindow,		//�������ڡ��˱�Ǳ����ú󣬻ᴴ�� Native Window������ͽ������ƣ���û�д��ھ����
								//���û�и����ڣ��� GDummyDesktop ����û�а󶨵����ڣ���˱����Ч���������Ƿ�����������
		eWA_PopupDialog,		//�������ڡ��˱�Ǳ����ú󣬻ᴴ���������ڣ�������ǿؼ���
								//���û�и����ڣ�����Դ˱�ǣ���Ȼ�ǵ������ڡ�
		eWA_AlphaDialog,		//������ Alpha ͨ�������԰�͸����
								//�����ڵ�������ָ���˱�ǣ��ؼ�ʼ���ǿ��԰�͸���ġ�
								//�˱���� eWA_SystemWindowFrame �����ݣ���ָ���� eWA_SystemWindowFrame ʱ�˱����Ч��
		eWA_InputTransparent,	//����/�ؼ������������̵����룬�������뽫��͸���²�Ĵ��ڡ�
		eWA_Hide,				//����/�ؼ����ء�
		eWA_OnOtherDesktop,		//���ڱ����أ���Ϊ�����������������д򿪡�
		eWA_Disable,			//����/�ؼ���ֹ���������̵����롣
		eWA_ChildDoModal,		//���� Disable����Ϊ�����Ӵ��ڱ���ʾΪģ̬�Ի���
		//�����봰�ڱ߿���صı��ֻ�� GDialog ������������Ч���� GWidget ��Ч��ֱ�Ӽ̳��� GWidget �Ķ���û�б߿�
		eWA_WindowFrame,		//ʹ�ô��ڿ�ܡ�����һ�����أ����������봰�ڱ߿���صı���Ƿ���Ч��
								//�����˴˱�ǣ�����ʹ����������Ĵ��ڵĲ�Ρ�
		eWA_SystemWindowFrame,	//ʹ��ϵͳ�Ĵ��ڿ�ܡ�
								//�����˴˱�ǣ�eWA_NativeWindow ���Ҳ����Ч���������Ƿ�����������
		eWA_ResizeBorder,		//�����п���ק�ĵ�����С�ı߿�
		eWA_TitleBar,			//�����п���ק�ĵ���λ�õı�������
								//�����б�����������ı�������ť��ǲ���Ч��
		eWA_MinimizeButton,		//��������С����ť��
		eWA_MaximizeButton,		//���������/��ԭ��ť��
		eWA_QuestionButton,		//�������ʺŰ�ť��
		eWA_NoCloseButton,		//����û�йرհ�ť��
		//���ڱ�ǵ���Ϊֹ��
		eWA_MaxCount,
	};

	using	GBaseNode::objectType;

	void setParent( GWidget* parent );
	bool attribute( EWidgetAttribute attrib );
	bool setAttribure( EWidgetAttribute attrib, bool on = true );
	bool isNativeWindow();
	bool isPopupDialog() { return ( m_widgetAtts[eWA_PopupDialog] || m_parentNode->objectType() == eDesktop ); }

	//��ʾ���ڡ�
	//�ɹ����� true��ʧ�ܷ��� false��
	bool show();
	void hide();
	//�����Ƿ����ء�
	//����鴰�� eWA_Hide ���ԣ����븸�������Ƿ�ɼ��޹ء�
	bool isHidden() { return m_widgetAtts[eWA_Hide]; }
	//�����Ƿ�ɼ���
	//������ڱ����أ����ߴ��ڵ��κ�һ�������ڱ����أ������� false�����򷵻� true��
	bool isVisible() { return m_isVisible; }

	void setText( const wstring text );
	const wstring& text() const { return m_texts->text; }

	//��ȡ���ڡ��ؼ���߽�ľ�������
	//���ڵ������ڣ���ȡ�����������ϵľ�������
	//���ڿؼ�����ȡ�����ڸ������ϵľ����������Ͻ������Ը����ڵ����������Ͻ�(0,0)Ϊ��㡣
	const GRect& marginRect() const { return m_box.marginRect; }

	bool setMarginSize( GMargins margin );
	const GMargins& marginSize() const { return m_box.margin; }

	//���úͻ�ȡ���ڡ��ؼ��ľ������򣬲�������߽磬�������߿�����
	//���ڵ������ڣ����á���ȡ�����������ϵľ�������
	//���ڿؼ������á���ȡ�����ڸ������ϵľ����������Ͻ������Ը����ڵ����������Ͻ�(0,0)Ϊ��㡣
	bool setBorderRect( const GRect& rect );
	const GRect& borderRect() const { return m_box.borderRect; }

	bool setBorderSize( GMargins margin );
	const GMargins& borderSize() const { return m_box.border; }

	bool setPaddingSize( GMargins margin );
	const GMargins& paddingSize() const { return m_box.padding; }

	const GRect& clientRect() const { return m_box.clientRect; }
	GRect calcBorderRect( const GRect& contentOnParent );

	//��ȡ���ڡ��ؼ�����������������
	//�����������Ͻ������Դ��ڡ��ؼ��ı߿򣨲�����߽磩���Ͻ�Ϊ(0,0)Ϊ��㡣
	const GRect& contentRect() const { return m_box.contentRect; }

	GRect	mapToScreen( const GRect& rect );
	GRect	mapToParent( const GRect& rect );

	void	moveToCenter();
	void	moveTo( int32_t x, int32_t y );
	//ȡ�� widget ���ĸ���Ļ�ϡ�
	//���ش� 0 ��ʼ����Ļ������
	//��� widget ��Խ�����Ļ����ô�����ĸ���Ļ��ռ�������󣬾ͷ����ĸ���Ļ��������
	int32_t	onScreen();

	//��ǰ Widget �� Native Window �����
	//�����ǰ Widget ���� Native Window���ͷ��� nullptr��
	void* nativeWindow() { return m_wndHandle; }

	//ȡ�ö���ĵ�������
	GWidget* parentPopupDialog();

	//ȡ�ö���� Native Popup Window��
	void* rootNativePopup();

	//��һ�� Native Window ������ Widget��
	//��� Widget �Ѿ�ӵ���� Native Window ����������ʧ�ܣ����� false��
	bool associateNativeWindow( void* wndHandle );
	//bool exec();

	GWidget* parentWidget();
	GWidget* firstChild() { return (GWidget*)m_firstChild; }
	GWidget* nextWidget() { return (GWidget*)m_nextNode; }
	GWidget* prevWidget() { return (GWidget*)m_prevNode; }
protected:
	bool	m_widgetAtts[eWA_MaxCount];

	struct SBox
	{
		GSize		defaultSize;	//Ĭ�ϵĴ�С��borderRect��
		GSize		minimizeSize;	//��������ߴ磨borderRect��
		GSize		maximizeSize;	//�������С�ߴ磨borderRect��

		GRect		marginRect;		//Widget ��ʵ�� rect������ margin(��߾�)
		GMargins	margin;			//��߾࣬�� marginRect �ľ��롣��߾����಻�ɵ�������ܻ������Ӱ������߾���ڲ��ǿ���Ӧ��Ϣ������
		GRect		borderRect;		//marginRect - margin ֮��Ĳ��֣������￪ʼ�ɵ����Ĭ�ϴ����￪ʼ���Ʊ߿�
		GMargins	border;			//�߿��� marginRect �ľ��롣���ڴ��ھ��ǿ�����ק�ı��С�ı��ߡ�
		GRect		clientRect;		//borderRect - border ֮��Ĳ��֡�borderRect �� clientRect ֮��������Ǳ߿�
		//int32_t			titleBar;		//�������Ĵ�С��
		//int32_t			titleDir;		//�������ķ��򣬱���������λ�� ��(0)����(1)����(2)����(3) �ĸ�����
		//GRect			titleRect;		//������������
		GMargins	otherMargin;	//�����ģ�����˵�������������״̬����ռ�õľ��룬ֻ�ɳ�����㣬�����û����á�
		GMargins	padding;		//�ڱ߾ࡣ
		GRect		contentRect;	//�����������Ͻ�������� clientRect ���Ͻǵ�ƫ������
	};

	enum EStatus
	{
		eStaBase,		//����״̬
		eStaActive,		//��ģ��н����
		eStaEnter,		//�������
		eStaDown,		//��갴��
		eStaDisable,	//����
		eStaCount		//״̬������
	};

	struct SSliceImage
	{
		GImage*		image;			//Դͼ��
		GRect		source;			//��Դͼ���ϵ�����
		GMargins	slice;			//��Ƭ�������ֳ� 9 �ݡ�
	};

	struct STextDraw
	{
		bool		stand;	//״̬�µ��ı������塢��ɫ�Ƿ񵥶����á�Ĭ��ʹ��
		GFont		font;
		wstring		text;
		GColor		color;
	};

	struct SHsvColor
	{
		float			hsvHue;			//HSV��ɫ�ռ�ɫ��(ɫ��)����; -1 �� 1����ʾ -180�ȵ�+180��
		float			hsvSaturation;	//HSV��ɫ�ռ䱥�϶ȵ���; -1 �� 1
		float			hsvValue;		//HSV��ɫ�ռ䴿��(����)����: -1 �� 1
		float			hsvContrast;	//��HSV��ɫ�ռ�����Աȶ�: -1 �� 1
	};

	SBox			m_box;
	EStatus			m_status;
	SSliceImage		m_borderImages[eStaCount];
	GColor			m_backgroundColors[eStaCount];	//������ɫ��
	STextDraw		m_texts[eStaCount];				//�ؼ��ı������ڱ���
	wstring			m_tooltip;						//��ʾ��Ϣ

	void*			m_wndHandle;
	bool			m_isVisible;			//�����Ƿ�ɼ�����ʼ��ʱ���ɼ���ֱ�������� show()��������ڻ򸸴��ڱ����أ��򲻿ɼ���
	bool			m_isCreated;
	inline bool isAllowVisible();
	void setChildVisible( bool isVisible );
protected:
	bool onCreate();

};

