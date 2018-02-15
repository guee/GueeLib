#pragma once
#include "GGlobal.h"
#include "GRect.h"
//设置与上下左右距离，用于计算 margin、border、padding 和 9 slice 图片的各个区域的 Rect
class GMargins
{
public:
	GMargins();
	GMargins( int32_t margins );
	GMargins( int32_t left, int32_t top, int32_t right, int32_t bottom );

	bool isNull() const;

	int32_t left() const;
	int32_t top() const;
	int32_t right() const;
	int32_t bottom() const;

	void setLeft( int32_t left );
	void setTop( int32_t top );
	void setRight( int32_t right );
	void setBottom( int32_t bottom );

	GMargins &operator+=( const GMargins &margins );
	GMargins &operator-=( const GMargins &margins );
	GMargins &operator+=( int );
	GMargins &operator-=( int );
	GMargins &operator*=( int );
	GMargins &operator/=( int );
	GMargins &operator*=( real_t );
	GMargins &operator/=( real_t );

	//计算 9 slice 图片的各个部分
	GRect middle( const GRect& r ) const;

	GRect leftTop( const GRect& r ) const;
	GRect leftMiddle( const GRect& r ) const;
	GRect leftBottom( const GRect& r ) const;

	GRect rightTop( const GRect& r ) const;
	GRect rightMiddle( const GRect& r ) const;
	GRect rightBottom( const GRect& r ) const;

	GRect topLeft( const GRect& r ) const;
	GRect topMiddle( const GRect& r ) const;
	GRect topRight( const GRect& r ) const;

	GRect bottomLeft( const GRect& r ) const;
	GRect bottomMiddle( const GRect& r ) const;
	GRect bottomRight( const GRect& r ) const;

private:
	int32_t m_left;
	int32_t m_top;
	int32_t m_right;
	int32_t m_bottom;

	friend inline bool operator==( const GMargins &, const GMargins & );
	friend inline bool operator!=( const GMargins &, const GMargins & );
};


inline GMargins::GMargins()
{
	m_top = m_bottom = m_left = m_right = 0;
}

inline GMargins::GMargins( int32_t margins )
	: m_left( margins ), m_top( margins ), m_right( margins ), m_bottom( margins ) {}


inline GMargins::GMargins( int32_t aleft, int32_t atop, int32_t aright, int32_t abottom )
	: m_left( aleft ), m_top( atop ), m_right( aright ), m_bottom( abottom ) {}

inline bool GMargins::isNull() const
{
	return m_left == 0 && m_top == 0 && m_right == 0 && m_bottom == 0;
}

inline int32_t GMargins::left() const
{
	return m_left;
}

inline int32_t GMargins::top() const
{
	return m_top;
}

inline int32_t GMargins::right() const
{
	return m_right;
}

inline int32_t GMargins::bottom() const
{
	return m_bottom;
}


inline void GMargins::setLeft( int32_t aleft )
{
	m_left = aleft;
}

inline void GMargins::setTop( int32_t atop )
{
	m_top = atop;
}

inline void GMargins::setRight( int32_t aright )
{
	m_right = aright;
}

inline void GMargins::setBottom( int32_t abottom )
{
	m_bottom = abottom;
}

inline GRect GMargins::leftTop( const GRect& r ) const
{
	return GRect( r.left(), r.top(), m_left, m_top );
}
inline GRect GMargins::leftMiddle( const GRect& r ) const
{
	return GRect( r.left(), r.top() + m_top, m_left, r.height() - m_top - m_bottom );
}
inline GRect GMargins::leftBottom( const GRect& r ) const
{
	return GRect( r.left(), r.bottom() - m_bottom, m_left, m_bottom );
}

inline GRect GMargins::rightTop( const GRect& r ) const
{
	return GRect( r.right() - m_right, r.top(), m_right, m_top );

}
inline GRect GMargins::rightMiddle( const GRect& r ) const
{
	return GRect( r.right() - m_right, r.top() + m_top, m_right, r.height() - m_top - m_bottom );
}
inline GRect GMargins::rightBottom( const GRect& r ) const
{
	return GRect( r.right() - m_right, r.bottom() - m_bottom, m_right, m_bottom );
}

inline GRect GMargins::topLeft( const GRect& r ) const
{
	return leftTop( r );
}
inline GRect GMargins::topMiddle( const GRect& r ) const
{
	return GRect( r.left() + m_left, r.top(), r.width() - m_left - m_right, m_top );

}
inline GRect GMargins::topRight( const GRect& r ) const
{
	return rightTop( r );
}

inline GRect GMargins::bottomLeft( const GRect& r ) const
{
	return leftBottom( r );
}

inline GRect GMargins::bottomMiddle( const GRect& r ) const
{
	return GRect( r.left() + m_left, r.bottom() - m_bottom, r.width() - m_left - m_right, m_bottom );
}
inline GRect GMargins::bottomRight( const GRect& r ) const
{
	return rightBottom( r );
}

inline GRect GMargins::middle( const GRect& r ) const
{
	return GRect( r.left() + m_left, r.top() + m_top, r.width() - m_left - m_right, r.height() - m_top - m_bottom );
}

inline bool operator==( const GMargins &m1, const GMargins &m2 )
{
	return
		m1.m_left == m2.m_left &&
		m1.m_top == m2.m_top &&
		m1.m_right == m2.m_right &&
		m1.m_bottom == m2.m_bottom;
}

inline bool operator!=( const GMargins &m1, const GMargins &m2 )
{
	return
		m1.m_left != m2.m_left ||
		m1.m_top != m2.m_top ||
		m1.m_right != m2.m_right ||
		m1.m_bottom != m2.m_bottom;
}


inline GMargins operator+( const GMargins &m1, const GMargins &m2 )
{
	return GMargins( m1.left() + m2.left(), m1.top() + m2.top(),
		m1.right() + m2.right(), m1.bottom() + m2.bottom() );
}

inline GMargins operator-( const GMargins &m1, const GMargins &m2 )
{
	return GMargins( m1.left() - m2.left(), m1.top() - m2.top(),
		m1.right() - m2.right(), m1.bottom() - m2.bottom() );
}

inline GMargins operator+( const GMargins &lhs, int rhs )
{
	return GMargins( lhs.left() + rhs, lhs.top() + rhs,
		lhs.right() + rhs, lhs.bottom() + rhs );
}

inline GMargins operator+( int lhs, const GMargins &rhs )
{
	return GMargins( rhs.left() + lhs, rhs.top() + lhs,
		rhs.right() + lhs, rhs.bottom() + lhs );
}

inline GMargins operator-( const GMargins &lhs, int rhs )
{
	return GMargins( lhs.left() - rhs, lhs.top() - rhs,
		lhs.right() - rhs, lhs.bottom() - rhs );
}

inline GRect operator+( const GMargins &lhs, const GRect& rhs )
{
	return GRect( rhs.top() - lhs.top(), rhs.left() - lhs.left(),
		rhs.width() + lhs.left() + lhs.right(), rhs.height() + lhs.top() + lhs.bottom() );
}

inline GRect operator+( const GRect& rhs, const GMargins &lhs )
{
	return GRect( rhs.top() - lhs.top(), rhs.left() - lhs.left(),
		rhs.width() + lhs.left() + lhs.right(), rhs.height() + lhs.top() + lhs.bottom() );
}


inline GRect operator-( const GMargins &lhs, const GRect& rhs )
{
	return GRect( rhs.top() + lhs.top(), rhs.left() + lhs.left(),
		rhs.width() - lhs.left() - lhs.right(), rhs.height() - lhs.top() - lhs.bottom() );
}

inline GRect operator-( const GRect& rhs, const GMargins &lhs )
{
	return GRect( rhs.top() + lhs.top(), rhs.left() + lhs.left(),
		rhs.width() - lhs.left() - lhs.right(), rhs.height() - lhs.top() - lhs.bottom() );
}

inline GMargins operator*( const GMargins &margins, int factor )
{
	return GMargins( margins.left() * factor, margins.top() * factor,
		margins.right() * factor, margins.bottom() * factor );
}

inline GMargins operator*( int factor, const GMargins &margins )
{
	return GMargins( margins.left() * factor, margins.top() * factor,
		margins.right() * factor, margins.bottom() * factor );
}

inline GMargins operator*( const GMargins &margins, real_t factor )
{
	return GMargins( gRound( margins.left() * factor ), gRound( margins.top() * factor ),
		gRound( margins.right() * factor ), gRound( margins.bottom() * factor ) );
}

inline GMargins operator*( real_t factor, const GMargins &margins )
{
	return GMargins( gRound( margins.left() * factor ), gRound( margins.top() * factor ),
		gRound( margins.right() * factor ), gRound( margins.bottom() * factor ) );
}

inline GMargins operator/( const GMargins &margins, int divisor )
{
	return GMargins( margins.left() / divisor, margins.top() / divisor,
		margins.right() / divisor, margins.bottom() / divisor );
}

inline GMargins operator/( const GMargins &margins, real_t divisor )
{
	return GMargins( gRound( margins.left() / divisor ), gRound( margins.top() / divisor ),
		gRound( margins.right() / divisor ), gRound( margins.bottom() / divisor ) );
}

inline GMargins &GMargins::operator+=( const GMargins &margins )
{
	return *this = *this + margins;
}

inline GMargins &GMargins::operator-=( const GMargins &margins )
{
	return *this = *this - margins;
}

inline GMargins &GMargins::operator+=( int margin )
{
	m_left += margin;
	m_top += margin;
	m_right += margin;
	m_bottom += margin;
	return *this;
}

inline GMargins &GMargins::operator-=( int margin )
{
	m_left -= margin;
	m_top -= margin;
	m_right -= margin;
	m_bottom -= margin;
	return *this;
}

inline GMargins &GMargins::operator*=( int factor )
{
	return *this = *this * factor;
}

inline GMargins &GMargins::operator/=( int divisor )
{
	return *this = *this / divisor;
}

inline GMargins &GMargins::operator*=( real_t factor )
{
	return *this = *this * factor;
}

inline GMargins &GMargins::operator/=( real_t divisor ){
	return *this = *this / divisor;
}

inline GMargins operator+( const GMargins &margins ){
	return margins;
}

inline GMargins operator-( const GMargins &margins ){
	return GMargins( -margins.left(), -margins.top(), -margins.right(), -margins.bottom() );
}
