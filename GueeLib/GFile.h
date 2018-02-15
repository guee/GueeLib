#pragma once
#include <fstream>
#include <string>
#include <codecvt>
using namespace std;

class GFile
{
public:
	GFile( const string& path = string(), ios::openmode mode = ios::in );
	GFile( const wstring& path = wstring(), ios::openmode mode = ios::in );
	~GFile();
	enum	EFileSource
	{
		eFileUnknow,
		eFileLocalDisk,
		eFileSmbShare,
		eFileFtp,
		eFileHttp,
		eFileRtmp,
		eFileRes,
	};
	enum	ETextCodec
	{
		eCodecUnknow,
		eCodecAnsi,		//暂不支持字符串转码和以字符串方式写文件。
		eCodecUtf8,
		eCodecUtf16,
		eCodecUtf32,	//暂不支持字符串转码和以字符串方式写文件。
		eCodecUtf16BE,	//暂不支持字符串转码和以字符串方式写文件。
		eCodecUtf32BE,	//暂不支持字符串转码和以字符串方式写文件。
		eCodecGBK,		//暂不支持字符串转码和以字符串方式写文件。
	};

	//打开文件
	bool openFile( const string& path, ios::openmode mode );
	bool openFile( const wstring& path, ios::openmode mode );
	//关闭文件
	void closeFile();
	//是否成功打开文件
	bool isOpened() const { return m_isOpened; }
	//文件是否可读、可写
	bool isAllowRead() const { return m_allowRead; }
	bool isAllowWrite() const { return m_allowWrite; }
	//文件是否允许 seek 读写位置
	bool isAllowSeek() const { return m_allowSeek; }
	//取得文件的字节数
	int64_t	fileSize()  { return m_fileSize; }
	//取得输入(写)数据的当前位置
	int64_t	writePos() { return m_writePos; }
	//取得输出(读)数据的当前位置
	int64_t readPos() { return m_readPos; }
	//读取数据，并递增读文件的位置
	//如果读取数失败，则返回 nullptr
	uint8_t* read( int32_t size );
	//读取数据，但不改变读文件的位置
	//如果读取数失败，则返回 nullptr
	uint8_t* peek( int32_t size );
	//读取数据后，取得本次读取数据的长度。
	int32_t	readed() { return m_readed; }
	//重置读文件的位置
	bool seekRead( int64_t pos, ios::seek_dir dir );
	//写文件，返回成功写入的字节数
	int32_t write( const void* data, int32_t size );
	bool	writeBom( ETextCodec codec );
	int32_t write( const string& str, ETextCodec codec = eCodecUnknow );
	int32_t write( const wstring& str, ETextCodec codec = eCodecUnknow );
	//重置写文件的位置
	bool seekWrite( int64_t pos, ios::seek_dir dir );
	//
	//取得已经缓冲的字节数
	int32_t cachedSize() { return m_bufSize - m_bufOffset; }
	//检查传入的数据是否是 bom，返回bom表示的编码。
	//size	: 传入时是 data 的字节数。
	//			返回时更改为 bom 字节数。如果不是 bom，则为 0。
	static ETextCodec checkBom( const uint8_t* data, int32_t& size );
	//把字节数组转换为 utf-16 字符串。
	//返回值是字节数组中转换成功的字节数。
	static int32_t toUtf16( wstring& output, const char* input, int32_t size, ETextCodec codec );
	static wstring toUtf16( const string &str ) {
		wstring_convert<codecvt_utf8<wchar_t>> converter;
		return converter.from_bytes( str );
	}
	static int32_t toUtf8( string& output, const char* input, int32_t size, ETextCodec codec );
	static string toUtf8( const wstring &str ) {
		wstring_convert<codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes( str );
	}
private:
	fstream		m_file;
	ios_base::openmode	m_openmode;
	bool		m_isOpened;
	bool		m_allowRead;
	bool		m_allowWrite;
	bool		m_allowSeek;
	EFileSource	m_fileSource;
	int64_t		m_fileSize;
	int64_t		m_readPos;
	int64_t		m_writePos;
	uint8_t*	m_fileCache;
	int32_t		m_bufSize;
	int32_t		m_bufAlloc;
	int32_t		m_bufOffset;
	int32_t		m_readed;

	void checkFileSource( const string& path );
	int32_t	onReadFile( uint8_t* buffer, int32_t size );
	void resetMember();
};

