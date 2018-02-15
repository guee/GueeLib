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
		eCodecAnsi,		//�ݲ�֧���ַ���ת������ַ�����ʽд�ļ���
		eCodecUtf8,
		eCodecUtf16,
		eCodecUtf32,	//�ݲ�֧���ַ���ת������ַ�����ʽд�ļ���
		eCodecUtf16BE,	//�ݲ�֧���ַ���ת������ַ�����ʽд�ļ���
		eCodecUtf32BE,	//�ݲ�֧���ַ���ת������ַ�����ʽд�ļ���
		eCodecGBK,		//�ݲ�֧���ַ���ת������ַ�����ʽд�ļ���
	};

	//���ļ�
	bool openFile( const string& path, ios::openmode mode );
	bool openFile( const wstring& path, ios::openmode mode );
	//�ر��ļ�
	void closeFile();
	//�Ƿ�ɹ����ļ�
	bool isOpened() const { return m_isOpened; }
	//�ļ��Ƿ�ɶ�����д
	bool isAllowRead() const { return m_allowRead; }
	bool isAllowWrite() const { return m_allowWrite; }
	//�ļ��Ƿ����� seek ��дλ��
	bool isAllowSeek() const { return m_allowSeek; }
	//ȡ���ļ����ֽ���
	int64_t	fileSize()  { return m_fileSize; }
	//ȡ������(д)���ݵĵ�ǰλ��
	int64_t	writePos() { return m_writePos; }
	//ȡ�����(��)���ݵĵ�ǰλ��
	int64_t readPos() { return m_readPos; }
	//��ȡ���ݣ����������ļ���λ��
	//�����ȡ��ʧ�ܣ��򷵻� nullptr
	uint8_t* read( int32_t size );
	//��ȡ���ݣ������ı���ļ���λ��
	//�����ȡ��ʧ�ܣ��򷵻� nullptr
	uint8_t* peek( int32_t size );
	//��ȡ���ݺ�ȡ�ñ��ζ�ȡ���ݵĳ��ȡ�
	int32_t	readed() { return m_readed; }
	//���ö��ļ���λ��
	bool seekRead( int64_t pos, ios::seek_dir dir );
	//д�ļ������سɹ�д����ֽ���
	int32_t write( const void* data, int32_t size );
	bool	writeBom( ETextCodec codec );
	int32_t write( const string& str, ETextCodec codec = eCodecUnknow );
	int32_t write( const wstring& str, ETextCodec codec = eCodecUnknow );
	//����д�ļ���λ��
	bool seekWrite( int64_t pos, ios::seek_dir dir );
	//
	//ȡ���Ѿ�������ֽ���
	int32_t cachedSize() { return m_bufSize - m_bufOffset; }
	//��鴫��������Ƿ��� bom������bom��ʾ�ı��롣
	//size	: ����ʱ�� data ���ֽ�����
	//			����ʱ����Ϊ bom �ֽ������������ bom����Ϊ 0��
	static ETextCodec checkBom( const uint8_t* data, int32_t& size );
	//���ֽ�����ת��Ϊ utf-16 �ַ�����
	//����ֵ���ֽ�������ת���ɹ����ֽ�����
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

