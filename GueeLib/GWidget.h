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
		eWA_NativeWindow,		//本机窗口。此标记被设置后，会创建 Native Window，否则就仅仅绘制，但没有窗口句柄。
								//如果没有父窗口，且 GDummyDesktop 对象没有绑定到窗口，则此标记生效，而无论是否设置了它。
		eWA_PopupDialog,		//弹出窗口。此标记被设置后，会创建弹出窗口，否则就是控件。
								//如果没有父窗口，则忽略此标记，必然是弹出窗口。
		eWA_AlphaDialog,		//窗口有 Alpha 通道，可以半透明。
								//仅对于弹出窗口指定此标记，控件始终是可以半透明的。
								//此标记与 eWA_SystemWindowFrame 不兼容，当指定了 eWA_SystemWindowFrame 时此标记无效。
		eWA_InputTransparent,	//窗口/控件不处理鼠标键盘的输入，所有输入将穿透到下层的窗口。
		eWA_Hide,				//窗口/控件隐藏。
		eWA_OnOtherDesktop,		//窗口被隐藏，因为它在其它虚拟桌面中打开。
		eWA_Disable,			//窗口/控件禁止处理鼠标键盘的输入。
		eWA_ChildDoModal,		//窗口 Disable，因为它有子窗口被显示为模态对话框。
		//以下与窗口边框相关的标记只对 GDialog 及其派生类有效，对 GWidget 无效。直接继承自 GWidget 的对象都没有边框。
		eWA_WindowFrame,		//使用窗口框架。这是一个开关，设置以下与窗口边框相关的标记是否有效。
								//设置了此标记，可以使用鼠标点击更改窗口的层次。
		eWA_SystemWindowFrame,	//使用系统的窗口框架。
								//设置了此标记，eWA_NativeWindow 标记也会生效，而无论是否设置了它。
		eWA_ResizeBorder,		//窗口有可拖拽的调整大小的边框。
		eWA_TitleBar,			//窗口有可拖拽的调整位置的标题栏。
								//必须有标题栏，下面的标题栏按钮标记才有效。
		eWA_MinimizeButton,		//窗口有最小化按钮。
		eWA_MaximizeButton,		//窗口有最大化/还原按钮。
		eWA_QuestionButton,		//窗口有问号按钮。
		eWA_NoCloseButton,		//窗口没有关闭按钮。
		//窗口标记到此为止。
		eWA_MaxCount,
	};

	using	GBaseNode::objectType;

	void setParent( GWidget* parent );
	bool attribute( EWidgetAttribute attrib );
	bool setAttribure( EWidgetAttribute attrib, bool on = true );
	bool isNativeWindow();
	bool isPopupDialog() { return ( m_widgetAtts[eWA_PopupDialog] || m_parentNode->objectType() == eDesktop ); }

	//显示窗口。
	//成功返回 true，失败返回 false。
	bool show();
	void hide();
	//窗口是否隐藏。
	//仅检查窗口 eWA_Hide 属性，而与父级窗口是否可见无关。
	bool isHidden() { return m_widgetAtts[eWA_Hide]; }
	//窗口是否可见。
	//如果窗口被隐藏，或者窗口的任何一级父窗口被隐藏，都返回 false。否则返回 true。
	bool isVisible() { return m_isVisible; }

	void setText( const wstring text );
	const wstring& text() const { return m_texts->text; }

	//获取窗口、控件外边界的矩形区域。
	//对于弹出窗口，获取的是在桌面上的矩形区域。
	//对于控件，获取的是在父窗口上的矩形区域，左上角坐标以父窗口的内容区左上角(0,0)为起点。
	const GRect& marginRect() const { return m_box.marginRect; }

	bool setMarginSize( GMargins margin );
	const GMargins& marginSize() const { return m_box.margin; }

	//设置和获取窗口、控件的矩形区域，不包含外边界，但包含边框自身。
	//对于弹出窗口，设置、获取的是在桌面上的矩形区域。
	//对于控件，设置、获取的是在父窗口上的矩形区域，左上角坐标以父窗口的内容区左上角(0,0)为起点。
	bool setBorderRect( const GRect& rect );
	const GRect& borderRect() const { return m_box.borderRect; }

	bool setBorderSize( GMargins margin );
	const GMargins& borderSize() const { return m_box.border; }

	bool setPaddingSize( GMargins margin );
	const GMargins& paddingSize() const { return m_box.padding; }

	const GRect& clientRect() const { return m_box.clientRect; }
	GRect calcBorderRect( const GRect& contentOnParent );

	//获取窗口、控件的内容区矩形区域。
	//内容区的左上角坐标以窗口、控件的边框（不含外边界）左上角为(0,0)为起点。
	const GRect& contentRect() const { return m_box.contentRect; }

	GRect	mapToScreen( const GRect& rect );
	GRect	mapToParent( const GRect& rect );

	void	moveToCenter();
	void	moveTo( int32_t x, int32_t y );
	//取得 widget 在哪个屏幕上。
	//返回从 0 开始的屏幕索引。
	//如果 widget 跨越多个屏幕，那么它在哪个屏幕上占的面积最大，就返回哪个屏幕的索引。
	int32_t	onScreen();

	//当前 Widget 的 Native Window 句柄。
	//如果当前 Widget 不是 Native Window，就返回 nullptr。
	void* nativeWindow() { return m_wndHandle; }

	//取得顶层的弹出窗口
	GWidget* parentPopupDialog();

	//取得顶层的 Native Popup Window。
	void* rootNativePopup();

	//把一个 Native Window 关联到 Widget。
	//如果 Widget 已经拥有了 Native Window 句柄，则关联失败，返回 false。
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
		GSize		defaultSize;	//默认的大小（borderRect）
		GSize		minimizeSize;	//允许的最大尺寸（borderRect）
		GSize		maximizeSize;	//允许的最小尺寸（borderRect）

		GRect		marginRect;		//Widget 的实际 rect，包含 margin(外边距)
		GMargins	margin;			//外边距，与 marginRect 的距离。外边距的外侧不可点击（可能会绘制阴影），外边距的内侧是可响应消息的区域。
		GRect		borderRect;		//marginRect - margin 之后的部分，从这里开始可点击。默认从这里开始绘制边框。
		GMargins	border;			//边框，与 marginRect 的距离。对于窗口就是可以拖拽改变大小的边线。
		GRect		clientRect;		//borderRect - border 之后的部分。borderRect 与 clientRect 之间的区域是边框。
		//int32_t			titleBar;		//标题栏的大小。
		//int32_t			titleDir;		//标题栏的方向，标题栏可以位于 上(0)、右(1)、下(2)、左(3) 四个方向。
		//GRect			titleRect;		//标题栏的区域。
		GMargins	otherMargin;	//其它的，比如菜单栏、工具栏、状态栏等占用的距离，只由程序计算，不由用户设置。
		GMargins	padding;		//内边距。
		GRect		contentRect;	//内容区域。左上角是相对于 clientRect 左上角的偏移量。
	};

	enum EStatus
	{
		eStaBase,		//基本状态
		eStaActive,		//活动的，有焦点的
		eStaEnter,		//鼠标移入
		eStaDown,		//鼠标按下
		eStaDisable,	//禁用
		eStaCount		//状态的数量
	};

	struct SSliceImage
	{
		GImage*		image;			//源图像
		GRect		source;			//在源图像上的区域
		GMargins	slice;			//切片参数，分成 9 份。
	};

	struct STextDraw
	{
		bool		stand;	//状态下的文本、字体、颜色是否单独设置。默认使用
		GFont		font;
		wstring		text;
		GColor		color;
	};

	struct SHsvColor
	{
		float			hsvHue;			//HSV颜色空间色度(色调)调整; -1 到 1，表示 -180度到+180度
		float			hsvSaturation;	//HSV颜色空间饱合度调整; -1 到 1
		float			hsvValue;		//HSV颜色空间纯度(亮度)调整: -1 到 1
		float			hsvContrast;	//在HSV颜色空间调整对比度: -1 到 1
	};

	SBox			m_box;
	EStatus			m_status;
	SSliceImage		m_borderImages[eStaCount];
	GColor			m_backgroundColors[eStaCount];	//背景颜色。
	STextDraw		m_texts[eStaCount];				//控件文本、窗口标题
	wstring			m_tooltip;						//提示信息

	void*			m_wndHandle;
	bool			m_isVisible;			//窗口是否可见。初始化时不可见，直到调用了 show()。如果窗口或父窗口被隐藏，则不可见。
	bool			m_isCreated;
	inline bool isAllowVisible();
	void setChildVisible( bool isVisible );
protected:
	bool onCreate();

};

