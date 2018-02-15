#include "stdafx.h"
#include "GWidget.h"
#include "GDummyDesktop.h"

GWidget::GWidget( GWidget* parent )
	: GBaseNode( parent ? parent : (GBaseNode*)&GDummyDesktop::destkop() )
{
	m_objectType		= eWidget;
	memset( m_widgetAtts, 0, sizeof( m_widgetAtts ) );
	m_box.defaultSize	= GSize( 400, 300 );
	m_box.maximizeSize	= GSize( INT_MAX, INT_MAX );
	m_box.padding		= GMargins( 8 );
	setBorderRect( GRect( GPoint(), m_box.defaultSize ) );

	m_status	= eStaBase;
	m_wndHandle	= nullptr;
	m_isVisible	= false;
	m_isCreated	= false;

}


GWidget::~GWidget()
{
}

void GWidget::setParent( GWidget * parent )
{
	GBaseNode::moveToNode( parent ? parent : ( GBaseNode* )&GDummyDesktop::destkop(), eMoveToLastChild );
}

bool GWidget::attribute( EWidgetAttribute attrib )
{
	return ( attrib >= eWA_NativeWindow && attrib < eWA_MaxCount ) ? m_widgetAtts[attrib] : false;
}

bool GWidget::setAttribure( EWidgetAttribute attrib, bool on )
{
	return false;
}

bool GWidget::isNativeWindow()
{
	return ( m_widgetAtts[eWA_NativeWindow] || m_widgetAtts[eWA_SystemWindowFrame]
		|| ( m_parentNode->objectType() == eDesktop
			&& !GDummyDesktop::destkop().isWindowDummyDesktop() ) );
}

bool GWidget::show()
{
	if ( m_isVisible ) return true;
	if ( isAllowVisible() )	//必须父窗口没有隐藏才可以显示本窗口。
	{
		if ( !m_isCreated )
		{
			if ( isNativeWindow() )	//如果 eWA_NativeWindow 属性有效，就要检查并创建 Native Window。
			{
				if ( m_wndHandle == nullptr && !GDummyDesktop::destkop().createNativeWindow( this ) )
				{
					return false;
				}
				if ( ShowWindow( HWND( m_wndHandle ), SW_SHOW ) )
				{
					m_isVisible	= true;
				}
			}
			m_isCreated	= true;
		}

		//把子窗口设置为可见的。
		if ( firstChild() ) firstChild()->setChildVisible( m_isVisible );
	}
	m_widgetAtts[eWA_Hide]	= false;
	return false;
}

void GWidget::hide()
{
	if ( !isHidden() )	//如果窗口没有隐藏属性，就会设置隐藏属性。
	{
		m_widgetAtts[eWA_Hide]	= true;
		if ( isVisible() )	//如果窗口之前是可见的，那么它和它的子窗口都要设置为不可见。
		{
			m_isVisible	= false;
			if ( isNativeWindow() )
			{
				ShowWindow( HWND( m_wndHandle ), SW_HIDE );
			}
			if ( firstChild() ) firstChild()->setChildVisible( m_isVisible );
		}
	}
}

void GWidget::setText( const wstring text )
{
	for ( int32_t st = eStaBase; st < eStaCount; ++st )
	{
		if ( st == eStaBase || m_texts[st].stand )
		{
			m_texts[st].text	= text;
		}
	}
}

bool GWidget::setMarginSize( GMargins margin )
{
	if ( m_widgetAtts[eWA_SystemWindowFrame] ) return false;
	m_box.margin		= margin;
	m_box.marginRect	= m_box.borderRect + m_box.margin;
	if ( isNativeWindow() && m_wndHandle )
	{
		GDummyDesktop::destkop().moveNativeWindow( this );
	}
	return true;
}

bool GWidget::setBorderRect( const GRect & rect )
{
	m_box.borderRect	= rect;
	m_box.marginRect	= m_box.borderRect + m_box.margin;
	m_box.clientRect	= m_box.borderRect - m_box.border;
	m_box.contentRect.setRect( 0, 0, m_box.clientRect.width(), m_box.clientRect.height() );
	m_box.contentRect	= m_box.contentRect - m_box.otherMargin - m_box.padding;

	if ( isNativeWindow() && m_wndHandle )
	{
		GDummyDesktop::destkop().moveNativeWindow( this );
	}
	return true;
}

bool GWidget::setBorderSize( GMargins margin )
{
	if ( m_widgetAtts[eWA_SystemWindowFrame] ) return false;
	m_box.border	= margin;
	m_box.clientRect	= m_box.borderRect - m_box.border;
	m_box.contentRect.setRect( 0, 0, m_box.clientRect.width(), m_box.clientRect.height() );
	m_box.contentRect	= m_box.contentRect - m_box.otherMargin - m_box.padding;
	return false;
}

bool GWidget::setPaddingSize( GMargins margin )
{
	m_box.padding	= margin;
	m_box.contentRect.setRect( 0, 0, m_box.clientRect.width(), m_box.clientRect.height() );
	m_box.contentRect	= m_box.contentRect - m_box.otherMargin - m_box.padding;
	return true;
}

GRect GWidget::calcBorderRect( const GRect& contentOnParent )
{
	if ( isNativeWindow() )
	{
		GMargins	border	= GDummyDesktop::destkop().getNativeWindowBorder( this );
		return contentOnParent + m_box.padding + m_box.otherMargin + border;
	}
	return contentOnParent + m_box.padding + m_box.otherMargin + m_box.border;
}

GRect GWidget::mapToScreen( const GRect& rect )
{
	GWidget*	widget	= this;
	GRect		mapTo	= rect;
	while ( widget )
	{
		mapTo	= widget->mapToParent( mapTo );
		if ( widget->isPopupDialog() )
			return mapTo;
		widget	= widget->parentWidget();
	}
	return GRect();
}

GRect GWidget::mapToParent( const GRect& rect )
{
	return GRect( m_box.clientRect.left() + m_box.otherMargin.left() + m_box.padding.left() + rect.left() ,
		m_box.clientRect.top() + m_box.otherMargin.top() + m_box.padding.top() + rect.top(),
		rect.width(), rect.height() );
}

void GWidget::moveToCenter()
{
	int32_t		x = 0, y = 0;
	GRect		rect;
	if ( m_parentNode->objectType() == eDesktop )
	{
		GDummyDesktop::SScreenInfo	info	= GDummyDesktop::destkop().screenInfo( 0 );
		rect	= info.work;
	}
	else if ( isPopupDialog() )
	{
		rect	= parentWidget()->borderRect();
	}
	else
	{
		rect.setRect( 0, 0, parentWidget()->contentRect().width(), parentWidget()->contentRect().height() );
	}

	x	= ( rect.width() - m_box.borderRect.width() ) / 2 + rect.left();
	y	= ( rect.height() - m_box.borderRect.height() ) / 2 + rect.top();

	moveTo( x, y );
}

void GWidget::moveTo( int32_t x, int32_t y )
{
	GRect	to( x, y, m_box.borderRect.width(), m_box.borderRect.height() );
	setBorderRect( to );
}


int32_t GWidget::onScreen()
{
	GRect	scr;
	if ( isPopupDialog() )
	{
		scr	= m_box.borderRect;
	}
	else
	{
		scr	= parentWidget()->mapToScreen( m_box.borderRect );

	}

	int32_t		count	= GDummyDesktop::destkop().screenCount();
	int32_t		screen	= 0;
	GRect		maxIn;
	GDummyDesktop::SScreenInfo	info;
	for ( int32_t i = 0; i < count; ++i )
	{
		info	= GDummyDesktop::destkop().screenInfo(i);
		GRect	insert	= info.rect.intersected( scr );
		if ( insert.width() * insert.height() > maxIn.width() * maxIn.height() )
		{
			maxIn	= insert;
			screen	= i;
		}
	}
	return screen;
}

GWidget * GWidget::parentPopupDialog()
{
	GWidget*	widget	= this;
	while ( widget )
	{
		if ( widget->isPopupDialog() )
			return widget;
		widget	= widget->parentWidget();
	}
	return nullptr;
}

void * GWidget::rootNativePopup()
{
	GWidget*	widget	= this;
	while ( widget )
	{
		if ( widget->isPopupDialog() && widget->isNativeWindow() )
			return widget->nativeWindow();
		widget	= widget->parentWidget();
	}
	return nullptr;
}

bool GWidget::associateNativeWindow( void * wndHandle )
{
	if ( wndHandle == nullptr ) return false;
	if ( m_wndHandle )
	{
		return false;
	}
	m_wndHandle	= wndHandle;
	return true;
}

GWidget * GWidget::parentWidget()
{
	return m_parentNode->objectType() == eWidget ? nullptr : (GWidget*)m_parentNode;
}

inline bool GWidget::isAllowVisible()
{
	GWidget*	parent	= parentWidget();
	while ( parent && parent->objectType() != eDesktop )
	{
		if ( parent->isHidden() ) return false;
		parent	= parent->parentWidget();
	}
	return true;
}

void GWidget::setChildVisible( bool isVisible )
{
	GWidget*	node	= this;
	while ( node )
	{
		if ( !node->isHidden() )	//只有当窗口没有被设置隐藏属性时，它和它的子窗口才可以改变可见状态。否则它和它的子窗口都是不可见的。
		{
			node->m_isVisible	= isVisible;
			if ( node->firstChild() )
			{
				node->firstChild()->setChildVisible( isVisible );
			}
		}
		node	= node->nextWidget();
	}
}
