#include "stdafx.h"
#include "GVariant.h"

GVariant::~GVariant()
{
}

bool GVariant::toBool() const
{
	return  toInt32() ? true : false;
}

char GVariant::toChar() const
{
	return char( toInt32() );
}

int16_t GVariant::toInt16() const
{
	return int16_t( toInt32() );
}

uint16_t GVariant::toUInt16() const
{
	return uint16_t( toUInt32() );
}

int32_t GVariant::toInt32() const
{
	switch ( m_type )
	{
	case eInvalid:
		break;
	case eBool:
		return int32_t( m_bool );
	case eChar:
		return int32_t( m_char );
	case eInt16:
		return int32_t( m_int16 );
	case eUInt16:
		return int32_t( m_uint16 );
	case eInt32:
		return int32_t( m_int32 );
	case eUInt32:
		return int32_t( m_uint32 );
	case eInt64:
		return int32_t( m_int64 );
	case eUInt64:
		return int32_t( m_uint64 );
	case eFloat:
		return int32_t( m_float );
	case eDoulbe:
		return int32_t( m_double );
	case eLongDouble:
		return int32_t( m_longdouble );
	case eString:
		if ( strIsFloat( m_str.c_str() ) )
		{
			char*		end		= nullptr;
			return int32_t( strtod( m_str.c_str(), &end ) );
		}
		return int32_t( strToUInt64( m_str.c_str() ) );
	case eWString:
		if ( strIsFloat( m_wstr.c_str() ) )
		{
			wchar_t*	end		= nullptr;
			return int32_t( wcstod( m_wstr.c_str(), &end ) );
		}
		return int32_t( strToUInt64( m_wstr.c_str() ) );
	}
	return int32_t(0);
}

uint32_t GVariant::toUInt32() const
{
	switch ( m_type )
	{
	case eInvalid:
		break;
	case eBool:
		return uint32_t( m_bool );
	case eChar:
		return uint32_t( m_char );
	case eInt16:
		return uint32_t( m_int16 );
	case eUInt16:
		return uint32_t( m_uint16 );
	case eInt32:
		return uint32_t( m_int32 );
	case eUInt32:
		return uint32_t( m_uint32 );
	case eInt64:
		return uint32_t( m_int64 );
	case eUInt64:
		return uint32_t( m_uint64 );
	case eFloat:
		return uint32_t( m_float );
	case eDoulbe:
		return uint32_t( m_double );
	case eLongDouble:
		return uint32_t( m_longdouble );
	case eString:
		if ( strIsFloat( m_str.c_str() ) )
		{
			char*		end		= nullptr;
			return uint32_t( strtod( m_str.c_str(), &end ) );
		}
		return uint32_t( strToUInt64( m_str.c_str() ) );
	case eWString:
		if ( strIsFloat( m_str.c_str() ) )
		{
			wchar_t*	end		= nullptr;
			return uint32_t( wcstod( m_wstr.c_str(), &end ) );
		}
		return uint32_t( strToUInt64( m_str.c_str() ) );
	}
	return uint32_t(0);
}

int64_t GVariant::toInt64() const
{
	switch ( m_type )
	{
	case eInvalid:
		break;
	case eBool:
		return int64_t( m_bool );
	case eChar:
		return int64_t( m_char );
	case eInt16:
		return int64_t( m_int16 );
	case eUInt16:
		return int64_t( m_uint16 );
	case eInt32:
		return int64_t( m_int32 );
	case eUInt32:
		return int64_t( m_uint32 );
	case eInt64:
		return int64_t( m_int64 );
	case eUInt64:
		return int64_t( m_uint64 );
	case eFloat:
		return int64_t( m_float );
	case eDoulbe:
		return int64_t( m_double );
	case eLongDouble:
		return int64_t( m_longdouble );
	case eString:
		if ( strIsFloat( m_str.c_str() ) )
		{
			char*		end		= nullptr;
			return int64_t( strtod( m_str.c_str(), &end ) );
		}
		return strToUInt64( m_str.c_str() );
	case eWString:
		if ( strIsFloat( m_wstr.c_str() ) )
		{
			wchar_t*	end		= nullptr;
			return int64_t( wcstod( m_wstr.c_str(), &end ) );
		}
		return int64_t(strToUInt64( m_wstr.c_str() ));
	}
	return int64_t(0);
}

uint64_t GVariant::toUInt64() const
{
	switch ( m_type )
	{
	case eInvalid:
		break;
	case eBool:
		return uint64_t( m_bool );
	case eChar:
		return uint64_t( m_char );
	case eInt16:
		return uint64_t( m_int16 );
	case eUInt16:
		return uint64_t( m_uint16 );
	case eInt32:
		return uint64_t( m_int32 );
	case eUInt32:
		return uint64_t( m_uint32 );
	case eInt64:
		return uint64_t( m_int64 );
	case eUInt64:
		return uint64_t( m_uint64 );
	case eFloat:
		return uint64_t( m_float );
	case eDoulbe:
		return uint64_t( m_double );
	case eLongDouble:
		return uint64_t( m_longdouble );
	case eString:
		if ( strIsFloat( m_str.c_str() ) )
		{
			char*		end		= nullptr;
			return uint64_t( strtod( m_str.c_str(), &end ) );
		}
		return strToUInt64( m_str.c_str() );
	case eWString:
		if ( strIsFloat( m_wstr.c_str() ) )
		{
			wchar_t*	end		= nullptr;
			return uint64_t( wcstod( m_wstr.c_str(), &end ) );
		}
		return strToUInt64( m_wstr.c_str() );
	}
	return uint64_t(0);
}

float GVariant::toFloat() const
{
	return float( toDouble() );
}

double GVariant::toDouble() const
{
	switch ( m_type )
	{
	case eInvalid:
		break;
	case eBool:
		return double( m_bool );
	case eChar:
		return double( m_char );
	case eInt16:
		return double( m_int16 );
	case eUInt16:
		return double( m_uint16 );
	case eInt32:
		return double( m_int32 );
	case eUInt32:
		return double( m_uint32 );
	case eInt64:
		return double( m_int64 );
	case eUInt64:
		return double( m_uint64 );
	case eFloat:
		return double( m_float );
	case eDoulbe:
		return double( m_double );
	case eLongDouble:
		return double( m_longdouble );
	case eString:
		if ( !m_str.empty() )
		{
			char*		end		= nullptr;
			return strtod( m_str.c_str(), &end );
		}
		break;
	case eWString:
		if ( !m_wstr.empty() )
		{
			wchar_t*		end		= nullptr;
			return wcstod( m_wstr.c_str(), &end );
		}
		break;
	}
	return 0.0;
}

long double GVariant::toLongDouble() const
{
	switch ( m_type )
	{
	case eInvalid:
		break;
	case eBool:
		return long double( m_bool );
	case eChar:
		return long double( m_char );
	case eInt16:
		return long double( m_int16 );
	case eUInt16:
		return long double( m_uint16 );
	case eInt32:
		return long double( m_int32 );
	case eUInt32:
		return long double( m_uint32 );
	case eInt64:
		return long double( m_int64 );
	case eUInt64:
		return long double( m_uint64 );
	case eFloat:
		return long double( m_float );
	case eDoulbe:
		return long double( m_double );
	case eLongDouble:
		return long double( m_longdouble );
	case eString:
		if ( !m_str.empty() )
		{
			char*		end		= nullptr;
			return strtold( m_str.c_str(), &end );
		}
		break;
	case eWString:
		if ( !m_wstr.empty() )
		{
			wchar_t*		end		= nullptr;
			return wcstold( m_wstr.c_str(), &end );
		}
		break;
	}
	return 0;
}

const string& GVariant::toString()
{
	if ( m_str.empty() )
	{
		switch ( m_type )
		{
		case eInvalid:
			break;
		case eBool:
			m_str	= to_string( m_bool );
			break;
		case eChar:
			m_str	= to_string( m_char );
			break;
		case eInt16:
			m_str	= to_string( m_int16 );
			break;
		case eUInt16:
			m_str	= to_string( m_uint16 );
			break;
		case eInt32:
			m_str	= to_string( m_int32 );
			break;
		case eUInt32:
			m_str	= to_string( m_uint32 );
			break;
		case eInt64:
			m_str	= to_string( m_int64 );
			break;
		case eUInt64:
			m_str	= to_string( m_uint64 );
			break;
		case eFloat:
			m_str	= to_string( m_float );
			break;
		case eDoulbe:
			m_str	= to_string( m_double );
			break;
		case eLongDouble:
			m_str	= to_string( m_longdouble );
			break;
		case eString:
			m_str	= m_str;
			break;
		case eWString:
			if ( !m_wstr.empty() )
			{
				wstring_convert<codecvt_utf8<wchar_t>> converter;
				m_str	= converter.to_bytes( m_wstr );
			}
			break;
		}
	}
	return m_str;
}

const wstring& GVariant::toWString()
{
	if ( m_wstr.empty() )
	{
		switch ( m_type )
		{
		case eInvalid:
			break;
		case eBool:
			m_wstr	= to_wstring( m_bool );
			break;
		case eChar:
			m_wstr	= to_wstring( m_char );
			break;
		case eInt16:
			m_wstr	= to_wstring( m_int16 );
			break;
		case eUInt16:
			m_wstr	= to_wstring( m_uint16 );
			break;
		case eInt32:
			m_wstr	= to_wstring( m_int32 );
			break;
		case eUInt32:
			m_wstr	= to_wstring( m_uint32 );
			break;
		case eInt64:
			m_wstr	= to_wstring( m_int64 );
			break;
		case eUInt64:
			m_wstr	= to_wstring( m_uint64 );
			break;
		case eFloat:
			m_wstr	= to_wstring( m_float );
			break;
		case eDoulbe:
			m_wstr	= to_wstring( m_double );
			break;
		case eLongDouble:
			m_wstr	= to_wstring( m_longdouble );
			break;
		case eString:
			if ( !m_str.empty() )
			{
				wstring_convert<codecvt_utf8<wchar_t>> converter;
				m_wstr	= converter.from_bytes( m_str );
			}
			break;
		}
	}
	return m_wstr;
}

template<class T>
uint64_t GVariant::strToUInt64( T str ) const
{
	if ( nullptr == str || *str == 0 ) return 0;
	int32_t		base	= 10;
	bool		falg	= false;
	uint64_t	ret		= 0;
	while ( *str == ' ' || *str == '\t' || *str == '\r' || *str == '\n' ) ++str;
	if ( str[0] == 'f' || str[0] == 'F' )	// false or F
	{
		if ( str[1] == 0 || str[1] == ' ' || str[1] == '\t' || str[1] == '\r' || str[1] == '\n' )
			return 0;
		else if ( m_wstr.size() >= 5 && ( str[5] == 0 || str[5] == ' ' || str[5] == '\t' || str[5] == '\r' || str[5] == '\n' ) )
			if ( ( str[1] == 'a' || str[1] == 'A' ) && ( str[2] == 'l' || str[2] == 'L' ) && ( str[3] == 's' || str[3] == 'S' ) && ( str[4] == 'e' || str[4] == 'E' ) )
				return 0;
	}
	else if ( str[0] == 't' || str[0] == 'T' )	// true or T
	{
		if ( str[1] == 0 || str[1] == ' ' || str[1] == '\t' || str[1] == '\r' || str[1] == '\n' )
			return 1;
		else if ( m_wstr.size() >= 4 && ( str[5] == 0 || str[5] == ' ' || str[5] == '\t' || str[5] == '\r' || str[5] == '\n' ) )
			if ( ( str[1] == 'r' || str[1] == 'R' ) && ( str[2] == 'u' || str[2] == 'U' ) && ( str[3] == 'e' || str[3] == 'E' ) )
				return 1;
	}
	else if ( str[0] == 'n' || str[0] == 'N' )	// no or N
	{
		if ( str[1] == 0 || str[1] == ' ' || str[1] == '\t' || str[1] == '\r' || str[1] == '\n' )
			return 0;
		else if ( m_wstr.size() >= 2 && ( str[2] == 0 || str[2] == ' ' || str[2] == '\t' || str[2] == '\r' || str[2] == '\n' ) )
			if ( ( str[1] == 'o' || str[1] == 'O' ) )
				return 0;
	}
	else if ( str[0] == 'y' || str[0] == 'Y' )	// yes or Y
	{
		if ( str[1] == 0 || str[1] == ' ' || str[1] == '\t' || str[1] == '\r' || str[1] == '\n' )
			return 1;
		else if ( m_wstr.size() >= 3 && ( str[3] == 0 || str[3] == ' ' || str[3] == '\t' || str[3] == '\r' || str[3] == '\n' ) )
			if ( ( str[1] == 'e' || str[1] == 'E' ) && ( str[2] == 's' || str[2] == 'S' ) )
				return 1;
	}
	else if ( str[0] == '&' || str[0] == '#' )
	{
		++str;
		base	= 16;
		if ( str[1] == 'h' || str[1] == 'H' )
		{
			++str;
		}
	}
	else if ( str[0] == '0' && ( str[1] == 'x' || str[1] == 'X' ) )
	{
		str	+= 2;
		base	= 16;
	}
	else if ( str[0] == '-' )
	{
		falg	= true;
		++str;
	}

	if ( base == 10 )
	{
		while ( *str )
		{
			if ( *str < '0' || *str > '9' ) break;
			ret	= ret * 10 + ( *str - '0' );
			++str;
		}
	}
	else if ( base == 16 )
	{
		while ( *str )
		{
			if ( *str >= '0' && *str <= '9' )
			{
				ret	= ret * 16 + ( *str - '0' );
			}
			else if ( *str >= 'a' && *str <= 'f' )
			{
				ret	= ret * 16 + ( *str - 'a' + 10 );
			}
			else if ( *str >= 'A' && *str <= 'F' )
			{
				ret	= ret * 16 + ( *str - 'A' + 10 );
			}
			else
			{
				break;
			}
			++str;
		}
	}
	if ( falg )
	{
		*( (int64_t*)&ret )	= int64_t( ret ) * -1;
	}
	return ret;
}

template<class T>
bool GVariant::strIsFloat( T str ) const
{
	if ( nullptr == str || *str == 0 ) return false;
	while ( *str == ' ' || *str == '\t' || *str == '\r' || *str == '\n' ) ++str;
	if ( ( *str < '0' || *str > '9' ) && *str != '.' && *str != '-' && *str != '+' ) return false;
	while ( *str )
	{
		if ( *str == '.' )
			return true;
		else if ( *str < '0' || *str > '9' )
			return false;
		++str;
	}
	return false;
}
