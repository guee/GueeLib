#include "stdafx.h"
#include "GFile.h"

#include <algorithm> 
#include <iterator>

GFile::GFile( const string& path, ios::openmode mode )
{
	resetMember();
}

GFile::GFile( const wstring& path, ios::openmode mode )
{
	resetMember();
}

GFile::~GFile()
{
	closeFile();
}

bool GFile::openFile( const string& path, ios::openmode mode )
{
	if ( m_isOpened ) return false;
	checkFileSource( path );
	if ( m_fileSource == eFileLocalDisk || m_fileSource == eFileSmbShare )
	{
		m_file.open( path, mode );
		m_isOpened	= m_file.is_open();
		if ( m_isOpened )
		{
			if ( mode & ios::in )
			{
				m_file.seekg( 0, ios::end );
				m_fileSize	= m_file.tellg();
				m_file.seekg( 0, ios::beg );
				m_allowSeek	= true;
				m_allowRead	= true;
			}
			else if ( mode & ios::out )
			{
				m_file.seekp( 0, ios::end );
				m_fileSize	= m_file.tellp();
				if ( ( mode & ( ios::ate | ios::app ) ) == 0 )m_file.seekg( 0, ios::beg );
				m_allowSeek	= ( mode & ios::app ) ? false : true;
				m_allowWrite= true;
			}
		}
	}
	m_openmode	= mode;
	return m_isOpened;
}

bool GFile::openFile( const wstring & path, ios::openmode mode )
{
	if ( m_isOpened ) return false;
	wstring_convert<codecvt_utf8<wchar_t>> converter;
	checkFileSource( converter.to_bytes( path ) );
	if ( m_fileSource == eFileLocalDisk || m_fileSource == eFileSmbShare )
	{
		m_file.open( path, mode );
		m_isOpened	= m_file.is_open();
		if ( m_isOpened )
		{
			if ( mode & ios::in )
			{
				m_file.seekg( 0, ios::end );
				m_fileSize	= m_file.tellg();
				m_file.seekg( 0, ios::beg );
				m_allowSeek	= true;
				m_allowRead	= true;
			}
			else if( mode & ios::out )
			{
				m_file.seekp( 0, ios::end );
				m_fileSize	= m_file.tellp();
				if ( ( mode & ( ios::ate | ios::app ) ) == 0 )m_file.seekg( 0, ios::beg );
				m_allowSeek	= ( mode & ios::app ) ? false : true;
				m_allowWrite= true;
			}
		}
	}
	m_openmode	= mode;
	return m_isOpened;
}

void GFile::closeFile()
{
	if ( !m_isOpened ) return;
	switch ( m_fileSource )
	{
	case eFileUnknow:
		break;
	case eFileLocalDisk:
		m_file.close();
		break;
	case eFileSmbShare:
		break;
	case eFileFtp:
		break;
	case eFileHttp:
		break;
	case eFileRtmp:
		break;
	case eFileRes:
		break;
	}
	if ( m_fileCache ) free( m_fileCache );
	resetMember();
}

uint8_t * GFile::read( int32_t size )
{
	uint8_t*	buf	= peek( size );
	m_bufOffset	+= m_readed;
	return buf;
}

uint8_t * GFile::peek( int32_t size )
{
	if ( !m_isOpened || !m_allowRead ) return nullptr;
	if ( size + m_bufOffset > m_bufSize )	//缓冲区数据不足
	{
		if ( m_fileCache && m_bufOffset )
		{
			m_bufSize		-= m_bufOffset;
			if ( m_bufSize ) memmove( m_fileCache, m_fileCache + m_bufOffset, m_bufSize );
			m_bufOffset	= 0;
		}
		if ( size > m_bufAlloc )	//如果需要的数据长度超出了缓冲区大小，就扩展缓冲区
		{
			m_bufAlloc		= ( size + ( 1024 * 1024 - 1 ) ) / ( 1024 * 1024 ) * ( 1024 * 1024 );
			m_fileCache	= (uint8_t*)realloc( m_fileCache, m_bufAlloc );
			if ( m_fileCache == nullptr )
			{
				return nullptr;
			}
		}
		int32_t		readSize	= onReadFile( m_fileCache + m_bufSize, m_bufAlloc - m_bufSize );
		m_readPos	+= readSize;
		if ( readSize < size - m_bufSize )
		{
			m_readed	= m_bufSize + readSize;
		}
		else
		{
			m_readed	= size;
		}
		m_bufSize	+= readSize;
	}
	else
	{
		m_readed	= size;
	}
	return m_readed ? m_fileCache + m_bufOffset : nullptr;
}

bool GFile::seekRead( int64_t pos, ios::seek_dir dir )
{
	if ( !m_isOpened || !m_allowSeek ) return false;
	int64_t		newPos		= 0;
	switch ( dir )
	{
	case ios::beg:
		newPos	= pos;
		break;
	case ios::cur:
		newPos	= m_readPos + pos;
		break;
	case ios::end:
		newPos	= m_fileSize + pos;
		break;
	}
	if ( newPos < 0 || newPos > m_fileSize ) return false;
	if ( newPos <= m_readPos && newPos >= m_readPos - m_bufSize )
	{
		m_bufOffset	= m_bufSize + int32_t( newPos - m_readPos );
	}
	else
	{
		m_bufOffset	= m_bufSize = 0;
		m_file.seekg( newPos );
		m_readPos	= newPos;
	}
	return false;
}

int32_t GFile::write( const void * data, int32_t size )
{
	if ( !m_isOpened ) return 0;
	m_file.write( (const char*)data, size );
	if ( m_file.bad() ) return 0;
	return size;
}

bool GFile::writeBom( ETextCodec codec )
{
	switch ( codec )
	{
	case eCodecUnknow:
		break;
	case eCodecAnsi:
		break;
	case eCodecUtf8:
		if ( write( "\xEF\xBB\xBF", 3 ) ) return true;
		break;
	case eCodecUtf16:
		if ( write( "\xFF\xFE", 2 ) ) return true;
		break;
	case eCodecUtf32:
		if ( write( "\xFF\xFE\x00\x00", 4 ) ) return true;
		break;
	case eCodecUtf16BE:
		if ( write( "\xFE\xFF", 2 ) ) return true;
		break;
	case eCodecUtf32BE:
		if ( write( "\x00\x00\xFE\xFF", 4 ) ) return true;
		break;
	case eCodecGBK:
		break;
	}
	return false;
}

int32_t GFile::write( const string & str, ETextCodec codec )
{
	switch ( codec )
	{
	case eCodecUnknow:
		return write( str.c_str(), str.length() );
	case eCodecAnsi:
		return write( str.c_str(), str.length() );
	case eCodecUtf8:
		return write( str.c_str(), str.length() );
	case eCodecUtf16:
	{
		wstring	utf16;
		toUtf16( utf16, str.c_str(), str.length() , eCodecUtf16 );
		return write( utf16.c_str(), utf16.length() * sizeof( wchar_t ) );
	}
	case eCodecUtf32:
		break;
	case eCodecUtf16BE:
		break;
	case eCodecUtf32BE:
		break;
	case eCodecGBK:
		break;
	}
	return int32_t();
}

int32_t GFile::write( const wstring& str, ETextCodec codec )
{
	switch ( codec )
	{
	case eCodecUnknow:
		return write( str.c_str(), str.length() * sizeof( wchar_t ) );
	case eCodecAnsi:
		break;
	case eCodecUtf8:
	{
		string	utf8;
		toUtf8( utf8, (const char*)str.c_str(), str.length() * sizeof( wchar_t ), eCodecUtf16 );
		return write( utf8.c_str(), utf8.length() * sizeof( char ) );
	}
		break;
	case eCodecUtf16:
		return write( str.c_str(), str.length() * sizeof( wchar_t ) );
	case eCodecUtf32:
		break;
	case eCodecUtf16BE:
		break;
	case eCodecUtf32BE:
		break;
	case eCodecGBK:
		break;
	}
	return int32_t();
}

bool GFile::seekWrite( int64_t pos, ios::seek_dir dir )
{
	if ( !m_isOpened || !m_allowSeek ) return false;
	m_file.seekp( pos, dir );
	return m_file.good();
}

GFile::ETextCodec GFile::checkBom( const uint8_t * data, int32_t& size )
{
	int32_t		bomSize	= 0;
	ETextCodec	codec	= eCodecUnknow;
	if ( size < 4 ) return codec;
	if ( data[0] == 0xFF && data[1] == 0xFE )	//utf-16
	{
		if ( data[2] == 0 && data[3] == 0 )
		{
			codec	= eCodecUtf32;
			bomSize	= 4;
		}
		else
		{
			codec	= eCodecUtf16;
			bomSize	= 2;
		}
	}
	else if ( data[0] == 0xFE && data[1] == 0xFF )	//utf-16  big endian byte order
	{
		codec	= eCodecUtf16BE;
		bomSize	= 2;
	}
	else if ( data[0] == 0 && data[1] == 0 && data[2] == 0xFE && data[3] == 0xFF )
	{
		codec	= eCodecUtf32BE;
		bomSize	= 4;
	}
	else if ( data[0] == 0xEF && data[1] == 0xBB && data[2] == 0xBF )	//Utf-8
	{
		codec	= eCodecUtf8;
		bomSize	= 3;
	}
	else if ( data[1] == 0 || data[3] == 0 )	//Utf-16
	{
		codec	= eCodecUtf16;
	}
	else
	{
		codec	= eCodecAnsi;
	}
	size	= bomSize;
	return codec;
}

int32_t GFile::toUtf16( wstring& output, const char* input, int32_t size, ETextCodec codec )
{
	int32_t		ret	= 0;
	switch ( codec )
	{
	case eCodecUnknow:
		return 0;
	case eCodecAnsi:
		break;
	case eCodecUtf8:
		if ( input && size > 0 )
		{
			wstring_convert<codecvt_utf8<wchar_t>> converter;
			output	= converter.from_bytes( input, input + size );
			ret		= converter.converted();
		}
		break;
	case eCodecUtf16:
		if ( input && size > 1 )
		{
			output.append( (const wchar_t*)input, size / 2 );
			ret	= size / 2 * 2;
		}
		break;
	case eCodecUtf32:
		break;
	case eCodecUtf16BE:
		break;
	case eCodecUtf32BE:
		break;
	case eCodecGBK:
		break;
	}
	return ret;
}

int32_t GFile::toUtf8( string & output, const char * input, int32_t size, ETextCodec codec )
{
	int32_t		ret	= 0;
	switch ( codec )
	{
	case eCodecUnknow:
		return 0;
	case eCodecAnsi:
		break;
	case eCodecUtf8:
		if ( input && size > 1 )
		{
			output.append( input, size );
			ret	= size;
		}
		break;
	case eCodecUtf16:
		if ( input && size > 0 )
		{
			wstring_convert<codecvt_utf8<wchar_t>> converter;
			output	= converter.to_bytes( (wchar_t*)input, ((wchar_t*)input) + size / 2 );
			ret		= converter.converted();
		}
		break;
	case eCodecUtf32:
		break;
	case eCodecUtf16BE:
		break;
	case eCodecUtf32BE:
		break;
	case eCodecGBK:
		break;
	}
	return ret;
}

void GFile::checkFileSource( const string & path )
{
	m_fileSource	= eFileUnknow;

	string		filePath;
	transform( path.begin(), path.end(), back_inserter( filePath ), ::tolower );
	if ( filePath.size() > 2 && filePath[0] == '\\' && filePath[1] == '\\' )
		m_fileSource	= eFileSmbShare;
	else if ( filePath[0] >='a' && filePath[0] <= 'z' && filePath[1] == ':' )
		m_fileSource	= eFileLocalDisk;
	else if ( filePath.find( "ftp://" ) == 0 )
		m_fileSource	= eFileFtp;
	else if ( filePath.find( "http://" ) == 0 )
		m_fileSource	= eFileHttp;
	else if ( filePath.find( "rtmp://" ) == 0 )
		m_fileSource	= eFileRtmp;
	else if ( filePath.find( "res://" ) == 0 )
		m_fileSource	= eFileRes;

}

inline int32_t GFile::onReadFile( uint8_t * buffer, int32_t size )
{
	streamsize	readSize	= 0;
	do
	{
		m_file.read( (char*)buffer + readSize, size - readSize );
		if ( m_file.gcount() == 0 )
		{
			break;
		}
		readSize	+= m_file.gcount();
	} while ( readSize < size );
	return (int32_t)readSize;
}

void GFile::resetMember()
{
	m_openmode		= 0;
	m_isOpened		= false;
	m_allowRead		= false;
	m_allowWrite	= false;
	m_allowSeek		= false;
	m_fileSource	= eFileUnknow;

	m_fileSize		= 0;
	m_readPos		= 0;
	m_writePos		= 0;
	m_fileCache		= nullptr;
	m_bufSize		= 0;
	m_bufAlloc		= 0;
	m_bufOffset		= 0;
	m_readed		= 0;
}
