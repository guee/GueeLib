#pragma once
#include <string>
#include <codecvt>
using namespace std;

class GVariant
{
public:
	GVariant() { m_type = eInvalid; m_longdouble = 0; };
	~GVariant();

	enum EType
	{
		eInvalid,
		eBool,
		eChar,
		eInt16,
		eUInt16,
		eInt32,
		eUInt32,
		eInt64,
		eUInt64,
		eFloat,
		eDoulbe,
		eLongDouble,
		eString,
		eWString,
	};

	GVariant( bool val ) { m_type = eBool; m_bool = val; };
	GVariant( char val ) { m_type = eChar; m_char = val; };
	GVariant( int16_t val ) { m_type = eInt16; m_int16 = val; };
	GVariant( uint16_t val ) { m_type = eUInt16; m_uint16 = val; };
	GVariant( int32_t val ) { m_type = eInt32; m_int32 = val; };
	GVariant( uint32_t val ) { m_type = eUInt32; m_uint32 = val; };
	GVariant( int64_t val ) { m_type = eInt64; m_int64 = val; };
	GVariant( uint64_t val ) { m_type = eUInt64; m_uint64 = val; };
	GVariant( float val ) { m_type = eFloat; m_float = val; };
	GVariant( double val ) { m_type = eDoulbe; m_double = val; };
	GVariant( long double val ) { m_type = eLongDouble; m_longdouble = val; };
	GVariant( const string& val ) { m_type = eString; m_str = val; };
	GVariant( const wstring& val ) { m_type = eWString; m_wstr = val; };

	EType	type() { return m_type; }
	void	clear() {
		m_type = eInvalid; m_longdouble = 0;
		m_str.clear(); m_wstr.clear();
	}
	bool		toBool() const;
	char		toChar() const;
	int16_t		toInt16() const;
	uint16_t	toUInt16() const;
	int32_t		toInt32() const;
	uint32_t	toUInt32() const;
	int64_t		toInt64() const;
	uint64_t	toUInt64() const;
	float		toFloat() const;
	double		toDouble() const;
	long double	toLongDouble() const;
	const string&	toString();
	const wstring&	toWString();

	GVariant& operator= ( bool val ) { 
		m_type = eBool; m_bool = val; m_str.clear(); m_wstr.clear(); return *this;
	};
	GVariant& operator= ( char val ) { 
		m_type = eChar; m_char = val; m_str.clear(); m_wstr.clear(); return *this;
	};
	GVariant& operator= ( int16_t val ) { 
		m_type = eInt16; m_int16 = val; m_str.clear(); m_wstr.clear(); return *this;
	};
	GVariant& operator= ( uint16_t val ) { 
		m_type = eUInt16; m_uint16 = val; m_str.clear(); m_wstr.clear(); return *this;
	};
	GVariant& operator= ( int32_t val ) { 
		m_type = eInt32; m_int32 = val; m_str.clear(); m_wstr.clear(); return *this;
	};
	GVariant& operator= ( uint32_t val ) { 
		m_type = eUInt32; m_uint32 = val; m_str.clear(); m_wstr.clear(); return *this;
	};
	GVariant& operator= ( int64_t val ) { 
		m_type = eInt64; m_int64 = val; m_str.clear(); m_wstr.clear(); return *this;
	};
	GVariant& operator= ( uint64_t val ) { 
		m_type = eUInt64; m_uint64 = val; m_str.clear(); m_wstr.clear(); return *this;
	};
	GVariant& operator= ( float val ) { 
		m_type = eFloat; m_float = val; m_str.clear(); m_wstr.clear(); return *this;
	};
	GVariant& operator= ( double val ) { 
		m_type = eDoulbe; m_double = val; m_str.clear(); m_wstr.clear(); return *this;
	};
	GVariant& operator= ( long double val ) { 
		m_type = eLongDouble; m_longdouble = val; m_str.clear(); m_wstr.clear(); return *this;
	};
	GVariant& operator= ( const string& val ) { 
		m_type = eString; m_str = val; m_wstr.clear(); return *this;
	};
	GVariant& operator= ( const wstring& val ) { 
		m_type = eWString; m_wstr = val; m_str.clear(); return *this;
	};

	operator bool() const { return toBool(); }
	operator char() const { return toChar(); }
	operator int16_t() const { return toInt16(); }
	operator uint16_t() const { return toUInt16(); }
	operator int32_t() const { return toInt32(); }
	operator uint32_t() const { return toUInt32(); }
	operator int64_t() const { return toInt64(); }
	operator uint64_t() const { return toUInt64(); }
	operator float() const { return toFloat(); }
	operator double() const { return toDouble(); }
	operator long double() const { return toLongDouble(); }
	operator const string&() { return toString(); }
	operator const wstring&() { return toWString(); }
private:
	EType	m_type;
	union
	{
		bool		m_bool;
		char		m_char;
		int16_t		m_int16;
		uint16_t	m_uint16;
		int32_t		m_int32;
		uint32_t	m_uint32;
		int64_t		m_int64;
		uint64_t	m_uint64;
		float		m_float;
		double		m_double;
		long double	m_longdouble;
	};
	string	m_str;
	wstring	m_wstr;
	template<class T>
	uint64_t strToUInt64( T str ) const;
	template<class T>
	bool	strIsFloat( T str ) const;
};