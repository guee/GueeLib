#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;
#include "GVariant.h"
#include "GBaseNode.h"
#include "GFile.h"

//读写 XML 文档
class GXmlNode;
class GXmlDocument
{
public:
	GXmlDocument();
	~GXmlDocument();

	//清除对象的所有状态和缓存。
	//如果要重新加载一个 XML 文档，必须调用 clear。
	void clear();

	//可以多次调用 loadFile / loadString / loadBinary，追加 XML 数据，直到 XML 文档完全加载完成。
	//从文件加载 xml 数据。
	bool loadFile( const string& file );
	bool loadFile( const wstring& file );
	//从字符串加载 xml 数据。只支持 UTF-8 和 UTF-16 的编码
	bool loadString( const string& str );
	bool loadString( const wstring& str );
	//从字节数组加载 xml
	//default	: 默认的字符编码格式，如果文件有 BOM 或 xml declare 中指定了编码格式，则忽略这个参数。
	bool loadBinary( const uint8_t* data, int32_t length, GFile::ETextCodec default = GFile::eCodecUtf8 );
	//XML 文档是否已经加载完成。
	bool isDone() { return m_rootClosed; }
	//取得根节点
	GXmlNode* root();
	//写入整个XML文档到 UTF-8 或 UTF-16 编码的字符串
	bool toString( string& str );
	bool toString( wstring& str );

	//保存文件时是否使用 BOM(Byte Order Mark)
	enum EBom
	{
		eUseIfExist,	//如果原本存在BOM，保存文件时就使用
		eUseBom,		//使用
		eNotBom,		//不使用
	};
	//保存 XML 数据到文件
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
		eXmlDeclare,		//XML第一行，声明 XML 基本属性。eg:<?xml version="1.0" encoding="utf-8" standalone="yes" ?>
		eXmlInstruction,	//XML处理指令。eg:<?xml-stylesheet type="text/css" href="1.css"?>
		eXmlHtmlDocType,	//HTTP 专属 <!DOCTYPE html>
		eXmlComment,		//注释。eg:<!--abc-->
		eXmlCharData,		//不由XML解析器解析的字符串。eg:<![CDATA[abc]]>
		eXmlElement,		//元素、节点。eg:<abc>123<abc>
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

