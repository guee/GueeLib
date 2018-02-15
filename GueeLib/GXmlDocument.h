#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;
#include "GVariant.h"
#include "GBaseNode.h"
#include "GFile.h"

//��д XML �ĵ�
class GXmlNode;
class GXmlDocument
{
public:
	GXmlDocument();
	~GXmlDocument();

	//������������״̬�ͻ��档
	//���Ҫ���¼���һ�� XML �ĵ���������� clear��
	void clear();

	//���Զ�ε��� loadFile / loadString / loadBinary��׷�� XML ���ݣ�ֱ�� XML �ĵ���ȫ������ɡ�
	//���ļ����� xml ���ݡ�
	bool loadFile( const string& file );
	bool loadFile( const wstring& file );
	//���ַ������� xml ���ݡ�ֻ֧�� UTF-8 �� UTF-16 �ı���
	bool loadString( const string& str );
	bool loadString( const wstring& str );
	//���ֽ�������� xml
	//default	: Ĭ�ϵ��ַ������ʽ������ļ��� BOM �� xml declare ��ָ���˱����ʽ����������������
	bool loadBinary( const uint8_t* data, int32_t length, GFile::ETextCodec default = GFile::eCodecUtf8 );
	//XML �ĵ��Ƿ��Ѿ�������ɡ�
	bool isDone() { return m_rootClosed; }
	//ȡ�ø��ڵ�
	GXmlNode* root();
	//д������XML�ĵ��� UTF-8 �� UTF-16 ������ַ���
	bool toString( string& str );
	bool toString( wstring& str );

	//�����ļ�ʱ�Ƿ�ʹ�� BOM(Byte Order Mark)
	enum EBom
	{
		eUseIfExist,	//���ԭ������BOM�������ļ�ʱ��ʹ��
		eUseBom,		//ʹ��
		eNotBom,		//��ʹ��
	};
	//���� XML ���ݵ��ļ�
	bool toFile( const string& file, GFile::ETextCodec codec = GFile::eCodecUnknow, EBom bom = eUseIfExist );
	bool toFile( const wstring& file, GFile::ETextCodec codec = GFile::eCodecUnknow, EBom bom = eUseIfExist );
private:
	GFile::ETextCodec	m_texCocec;
	bool				m_declareGeted;
	bool				m_rootClosed;
	int32_t				m_bomSize;
	vector<uint8_t>		m_dataCache;
	GXmlNode*			m_rootNode;
	GXmlNode*			m_currNode;
	int					m_iAnalysis;

	bool analysisXmlString( const wstring & xmlString );
	bool analysisXmlDeclare();
	wstring texCodec( GFile::ETextCodec codec );
};

class GXmlNode : public GBaseNode
{
public:
	GXmlNode( GXmlNode* parent = nullptr );
	~GXmlNode();

	enum EXmlNodeType
	{
		eXmlUnknow,
		eXmlDeclare,		//XML��һ�У����� XML �������ԡ�eg:<?xml version="1.0" encoding="utf-8" standalone="yes" ?>
		eXmlInstruction,	//XML����ָ�eg:<?xml-stylesheet type="text/css" href="1.css"?>
		eXmlHtmlDocType,	//HTTP ר�� <!DOCTYPE html>
		eXmlComment,		//ע�͡�eg:<!--abc-->
		eXmlCharData,		//����XML�������������ַ�����eg:<![CDATA[abc]]>
		eXmlElement,		//Ԫ�ء��ڵ㡣eg:<abc>123<abc>
	};

	GXmlNode* appendChildElement( const string& tagName );
	GXmlNode* appendChildElement( const wstring& tagName );
	GXmlNode* childElement( const string& tagName = string() );
	GXmlNode* childElement( const wstring& tagName = wstring() );
	GXmlNode* nextElement( const string& tagName = string() );
	GXmlNode* nextElement( const wstring& tagName = wstring() );
	void remove() { delete this; }
	GVariant& tagName() { return m_tagName; }
	GVariant& attribute( const string& attName ) { return m_attributes[GFile::toUtf16( attName )]; }
	GVariant& attribute( const wstring& attName ) { return m_attributes[attName]; }
	GVariant& tagValue() { return m_tagValue; }

	bool toString( wstring& str );
	bool toString( string& str );

	GXmlNode* parentNode() { return (GXmlNode*)m_parentNode; }
	GXmlNode* firstChild() { return (GXmlNode*)m_firstChild; }
	GXmlNode* nextNode() { return (GXmlNode*)m_nextNode; }
	GXmlNode* prevNode() { return (GXmlNode*)m_prevNode; }
private:
	friend			GXmlDocument;
	GVariant		m_tagValue;
	GVariant		m_tagName;
	EXmlNodeType	m_type;
	map<wstring, GVariant>	m_attributes;

	inline void trimRight( wstring& str ){
		while ( !str.empty() &&
			( str.back() == ' ' || str.back() == '\t' || str.back() == '\r' || str.back() == '\n' ) ) str.resize( str.size() - 1 );
	}
	inline void trimLeft( wstring& str ) {
		const wchar_t*	sta = str.c_str();
		while ( *sta &&
			( *sta == ' ' || *sta == '\t' || *sta == '\r' || *sta == '\n' ) ) ++sta;
		str.erase( 0, sta - str.c_str() );
	}
	inline void trim( wstring& str ) { trimLeft( str ); trimRight( str ); }
	static int32_t compareNoCase( const wstring& str1, const wstring& str2 );
	static void xmlCharDecode( wstring& str );
	static void xmlCharEncode( wstring& str );
	static void xmlCharEncode( const wstring& input, wstring& output );
	static bool toString( wstring& str, GXmlNode* node );
};

