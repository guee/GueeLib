#include "stdafx.h"
#include "GXmlDocument.h"


GXmlDocument::GXmlDocument()
{
	m_texCocec		= GFile::eCodecUnknow;
	m_declareGeted	= false;
	m_rootClosed	= false;
	m_bomSize		= 0;
	m_rootNode		= nullptr;
	m_currNode		= nullptr;
	m_iAnalysis		= 0;
}


GXmlDocument::~GXmlDocument()
{
}

void GXmlDocument::clear()
{
	m_texCocec		= GFile::eCodecUnknow;
	m_declareGeted	= false;
	m_rootClosed	= false;
	m_dataCache.clear();
	m_bomSize		= 0;
	if ( m_rootNode )
	{
		delete m_rootNode;
		m_rootNode		= nullptr;
	}
	m_currNode		= nullptr;
	m_iAnalysis		= 0;
}

bool GXmlDocument::loadFile( const string& file )
{
	GFile	xmlFile( file, ios::in | ios::binary );
	if ( !xmlFile.isOpened() ) return false;
	uint8_t*	buf = nullptr;
	while ( ( buf = xmlFile.read( 65536 ) ) != nullptr )
	{
		int32_t		len	= xmlFile.readed();
		if ( !loadBinary( buf, len ) )
		{
			return false;
		}
	}
	return true;
}

bool GXmlDocument::loadFile( const wstring& file )
{
	GFile	xmlFile( file );
	if ( !xmlFile.isOpened() ) return false;
	uint8_t*	buf = nullptr;
	while ( ( buf = xmlFile.read( 65536 ) ) != nullptr )
	{
		int32_t		len	= xmlFile.readed();
		if ( !loadBinary( buf, len ) )
		{
			return false;
		}
	}
	return true;
}

bool GXmlDocument::loadString( const string & str )
{
	return analysisXmlString( GFile::toUtf16(str) );
}

bool GXmlDocument::loadString( const wstring & str )
{
	return analysisXmlString( str );
}

bool GXmlDocument::loadBinary( const uint8_t * data, int32_t length, GFile::ETextCodec default )
{
	if ( data == nullptr || length <= 0 ) return false;
	int32_t		oldSize	= m_dataCache.size();
	int32_t		bomSize	= 0;
	m_dataCache.resize( m_dataCache.size() + length );
	memcpy( &m_dataCache[oldSize], data, length );
	if ( m_dataCache.size() < 4 ) return true;
	if ( m_texCocec == GFile::eCodecUnknow )
	{
		m_bomSize	= m_dataCache.size();
		m_texCocec	= GFile::checkBom( &m_dataCache.front(), m_bomSize );
	}
	wstring	str;
	oldSize	= m_dataCache.size();
	GFile::toUtf16( str, (const char*)m_dataCache.data(), oldSize, m_texCocec );
	m_dataCache.erase( m_dataCache.begin(), m_dataCache.begin() + oldSize );
	return analysisXmlString( str );
}

bool GXmlDocument::toString( string & str )
{
	wstring	strw;
	if ( toString( strw ) )
	{
		str	= GFile::toUtf8( strw );
		return true;
	}
	return false;
}

bool GXmlDocument::toString( wstring & str )
{
	if ( nullptr == m_rootNode ) return false;
	str.clear();
	GXmlNode*	node	= m_rootNode->firstChild();
	while ( node )
	{
		if ( node->m_type == GXmlNode::eXmlDeclare )
		{
			node->attribute( L"encoding" )	= texCodec( m_texCocec );
			GXmlNode::toString( str, node );
			break;
		}
		node	= node->nextNode();
	}
	if ( nullptr == node )
	{
		node	= new GXmlNode();
		node->moveToNode( m_rootNode, GXmlNode::eMoveToFirstChild );
		node->m_type	= GXmlNode::eXmlDeclare;
		node->attribute( L"version" )	= L"1.0";
		node->attribute( L"encoding" )	= texCodec( m_texCocec );
		GXmlNode::toString( str, node );
	}
	node	= m_rootNode->firstChild();
	while ( node )
	{
		if ( node->m_type != GXmlNode::eXmlDeclare )
		{
			GXmlNode::toString( str, node );
		}
		node	= node->nextNode();
	}
	return !str.empty();
}

bool GXmlDocument::toFile( const string & file, GFile::ETextCodec codec, EBom bom )
{
	GFile	xmlFile( file, ios::out | ios::binary | ios::trunc );
	if ( !xmlFile.isOpened() ) return false;
	wstring	xmlStr;
	if ( toString( xmlStr ) )
	{
		if ( bom == eUseBom || ( bom == eUseIfExist && m_bomSize ) )
		{
			xmlFile.writeBom( codec );
		}
		if ( xmlFile.write( xmlStr, codec ) )
			return true;
	}
	return false;
}

bool GXmlDocument::toFile( const wstring & file, GFile::ETextCodec codec, EBom bom )
{
	GFile	xmlFile( file, ios::out | ios::binary | ios::trunc );
	if ( !xmlFile.isOpened() ) return false;
	wstring	xmlStr;
	if ( toString( xmlStr ) )
	{
		if ( bom == eUseBom || ( bom == eUseIfExist && m_bomSize ) )
		{
			xmlFile.writeBom( codec );
		}
		if ( xmlFile.write( xmlStr, codec ) )
			return true;
	}
	return false;
}

bool GXmlDocument::analysisXmlString( const wstring & xmlString )
{
#define	CUR_TAG_VAL	((wstring&)(m_currNode->m_tagValue))
#define	CUR_TAG_NAM	((wstring&)(m_currNode->m_tagName))
	size_t	pos		= 0;
	wchar_t	ch		= 0;
	wstring	attribName, attribValue;
	enum	EAnalysisStep
	{
		eBracketLeft,		//正在寻找左尖括号
		eTagNameStart,		//正在寻找标签名
		eTagNameEnd,		//标签名第一个字符已经找到，正在寻找完整的标签名
		eAttribNameStart,	//正在寻找属性名
		eAttribNameEnd,		//属性名第一个字符已经找到，正在寻找完整的属性名
		eAttribEqualSign,	//正在寻找属性中的等号
		eAttribQuotesLeft,	//正在寻找属性值中的左引号
		eAttribQuotesRight,	//正在寻找属性值中的右引号
		eAttribSingleQuotes,//正在寻找属性值中的右引号(单引号)
		eCommentEnd,		//正在寻找注释的结束符号
		eCharDataEnd,		//正在寻找 CDATA 的结束符号
		eBracketRight,		//正在寻找右尖括号
	};
	if ( nullptr == m_rootNode )
	{
		m_rootNode	= new GXmlNode();
		m_currNode	= m_rootNode;
	}

	while ( pos < xmlString.length() )
	{
		ch	= xmlString[pos++];
		switch ( ch )
		{
		case '<':
			if ( m_iAnalysis == eBracketLeft || m_iAnalysis == eBracketRight || ( m_iAnalysis >= eTagNameEnd && m_iAnalysis < eCommentEnd ) )
			{
				m_iAnalysis = eTagNameStart;
				const	wchar_t*	pStrTmp	= xmlString.c_str() + pos;
				while ( *pStrTmp )
				{
					if ( pStrTmp[0] == ' ' || pStrTmp[0] == '\t' || pStrTmp[0] == '\r' || pStrTmp[0] == '\n' )
					{
						++pStrTmp;
						continue;
					}
					else if ( pStrTmp[0] == '/' )
					{
						m_iAnalysis	= eBracketRight;
						break;
					}
					else
					{
						break;
					}
					++pStrTmp;
				}
				if ( m_currNode->m_type == GXmlNode::eXmlElement )
				{
					m_currNode->trim( CUR_TAG_VAL );
					GXmlNode::xmlCharDecode( CUR_TAG_VAL );
				}
				if ( !attribName.empty() )
				{
					m_currNode->trimRight( attribValue );
					GXmlNode::xmlCharDecode( attribValue );
					m_currNode->m_attributes[attribName]	= attribValue;
					attribName.clear();
					attribValue.clear();
				}
				if ( m_iAnalysis == eBracketRight ) break;
				if ( CUR_TAG_NAM[0] == '!' || CUR_TAG_NAM[0] == '?' )
				{
					GXmlNode*	pCurNode	= new GXmlNode( m_currNode->parentNode() );
					if ( m_currNode->m_type == GXmlNode::eXmlDeclare && !analysisXmlDeclare() )
					{
						delete m_currNode;
					}
					m_currNode = pCurNode;
				}
				else
				{
					m_currNode = new GXmlNode( m_currNode );
				}
			}
			else if ( m_iAnalysis >= eCommentEnd && m_iAnalysis != eBracketRight )
				CUR_TAG_VAL += ch;
			break;
		case '>':
			if ( m_iAnalysis == eBracketRight )
			{
				m_iAnalysis = eBracketLeft;
				if ( m_currNode->m_type == GXmlNode::eXmlElement && m_currNode->parentNode() == m_rootNode )
				{
					m_rootClosed	= true;
				}
				GXmlNode*	pCurNode	= m_currNode->parentNode();
				if ( m_currNode->m_type == GXmlNode::eXmlDeclare && !analysisXmlDeclare() )
				{
					delete m_currNode;
				}
				m_currNode	= pCurNode;
			}
			else if ( ( m_iAnalysis == eCommentEnd && CUR_TAG_VAL.find( L"--", CUR_TAG_VAL.length() - 2 ) >= 0 ) ||
				( m_iAnalysis == eCharDataEnd && CUR_TAG_VAL.find( L"]]", CUR_TAG_VAL.length() - 2 ) >= 0 ) )
			{
				CUR_TAG_VAL.resize( CUR_TAG_VAL.length() - 2 );
				m_iAnalysis = eBracketLeft;
				m_currNode	= m_currNode->parentNode();
			}
			else if ( m_iAnalysis >= eTagNameEnd && m_iAnalysis <= eAttribEqualSign )
			{
				if ( CUR_TAG_NAM[0] == '!' || CUR_TAG_NAM[0] == '?' )
				{
					m_iAnalysis = eBracketLeft;
					GXmlNode*	pCurNode	= m_currNode->parentNode();
					if ( m_currNode->m_type == GXmlNode::eXmlDeclare && !analysisXmlDeclare() )
					{
						delete m_currNode;
					}
					m_currNode	= pCurNode;
				}
				else
				{
					m_iAnalysis = eBracketLeft;
				}
			}
			else if ( m_iAnalysis == eAttribQuotesRight || m_iAnalysis == eAttribSingleQuotes )
				attribValue += ch;
			else if ( m_iAnalysis == eBracketLeft || ( m_iAnalysis >= eCommentEnd && m_iAnalysis != eBracketRight ) )
				CUR_TAG_VAL += ch;
			break;
		case '/':
			if ( m_iAnalysis == eAttribQuotesRight || m_iAnalysis == eAttribSingleQuotes )
				attribValue += ch;
			else if ( m_iAnalysis == eBracketLeft || ( m_iAnalysis >= eCommentEnd && m_iAnalysis != eBracketRight ) )
				CUR_TAG_VAL += ch;
			else if ( m_iAnalysis >= eTagNameEnd && m_iAnalysis <= eAttribEqualSign )
				m_iAnalysis = eBracketRight;
			break;
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			if ( m_iAnalysis == eTagNameEnd ) {
				m_iAnalysis	= eAttribNameStart;
				if ( CUR_TAG_NAM.length() == 4 &&
					GXmlNode::compareNoCase( CUR_TAG_NAM, L"?xml" ) == 0 )
				{
					m_currNode->m_type	= GXmlNode::eXmlDeclare;
				}
				else if ( CUR_TAG_NAM.length() == 8 &&
					GXmlNode::compareNoCase( CUR_TAG_NAM, L"!DOCTYPE" ) == 0 )
				{
					m_currNode->m_type	= GXmlNode::eXmlHtmlDocType;
				}
			}
			else if ( m_iAnalysis == eAttribNameEnd )
				m_iAnalysis	= eAttribEqualSign;
			else if ( m_iAnalysis == eAttribQuotesRight || m_iAnalysis == eAttribSingleQuotes )
				attribValue += ch;
			else if ( m_iAnalysis == eAttribQuotesLeft ) {
				m_iAnalysis	= eAttribNameStart;
				if ( !attribName.empty() )
				{
					m_currNode->trimRight( attribValue );
					GXmlNode::xmlCharDecode( attribValue );
					m_currNode->m_attributes[attribName]	= attribValue;
					attribName.clear();
					attribValue.clear();
				}
			}
			else if ( m_iAnalysis == eBracketLeft || ( m_iAnalysis >= eCommentEnd && m_iAnalysis != eBracketRight ) )
				if ( m_currNode->m_type == GXmlNode::eXmlElement )
				{
					if ( !CUR_TAG_VAL.empty() ) CUR_TAG_VAL += ch;
				}
				else
				{
					CUR_TAG_VAL += ch;
				}
			break;
		case '\'':
			if ( m_iAnalysis == eAttribSingleQuotes || !attribValue.empty() )
			{
				GXmlNode::xmlCharDecode( attribValue );
				m_currNode->m_attributes[attribName]	= attribValue;
				attribName.clear();
				attribValue.clear();
				m_iAnalysis	= eAttribNameStart;
			}
			else if ( m_iAnalysis == eAttribQuotesLeft || m_iAnalysis == eAttribEqualSign )
				m_iAnalysis	= eAttribSingleQuotes;
			else if ( m_iAnalysis == eAttribQuotesRight )
				attribValue += ch;
			else if ( m_iAnalysis >= eAttribNameEnd && m_iAnalysis < eCommentEnd )
				m_iAnalysis	= eAttribSingleQuotes;
			else if ( m_iAnalysis == eBracketLeft || ( m_iAnalysis >= eCommentEnd && m_iAnalysis != eBracketRight ) )
				CUR_TAG_VAL += ch;
			break;
		case '\"':
			if ( m_iAnalysis == eAttribQuotesRight || !attribValue.empty() )
			{
				GXmlNode::xmlCharDecode( attribValue );
				m_currNode->m_attributes[attribName]	= attribValue;
				attribName.clear();
				attribValue.clear();
				m_iAnalysis	= eAttribNameStart;
			}
			else if ( m_iAnalysis == eAttribQuotesLeft || m_iAnalysis == eAttribEqualSign )
				m_iAnalysis	= eAttribQuotesRight;
			else if ( m_iAnalysis == eAttribSingleQuotes )
				attribValue += ch;
			else if ( m_iAnalysis >= eAttribNameEnd && m_iAnalysis < eCommentEnd )
				m_iAnalysis	= eAttribQuotesRight;
			else if ( m_iAnalysis == eBracketLeft || ( m_iAnalysis >= eCommentEnd && m_iAnalysis != eBracketRight ) )
				CUR_TAG_VAL += ch;
			break;
		case '=':
			if ( m_iAnalysis == eAttribNameEnd || m_iAnalysis == eAttribEqualSign )
				m_iAnalysis	= eAttribQuotesLeft;
			else if ( m_iAnalysis == eAttribQuotesRight || m_iAnalysis == eAttribSingleQuotes )
				attribValue += ch;
			else if ( m_iAnalysis == eBracketLeft || ( m_iAnalysis >= eCommentEnd && m_iAnalysis != eBracketRight ) )
				CUR_TAG_VAL += ch;
			break;
		case 0:
			break;
		default:
			if ( m_iAnalysis == eTagNameStart || m_iAnalysis == eTagNameEnd ) {
				CUR_TAG_NAM += ch;
				if ( m_iAnalysis == eTagNameStart && ( ch == '!' || ch == '?' ) )
					m_currNode->m_type	= GXmlNode::eXmlUnknow;
				else
					m_currNode->m_type	= GXmlNode::eXmlElement;

				m_iAnalysis	= eTagNameEnd;
				if ( CUR_TAG_NAM.length() == 3 &&
					CUR_TAG_NAM == L"!--" )
				{
					m_currNode->m_type	= GXmlNode::eXmlComment;
					m_iAnalysis	= eCommentEnd;
				}
				else if ( CUR_TAG_NAM.length() == 8 &&
					GXmlNode::compareNoCase( CUR_TAG_NAM, L"![CDATA[" ) == 0 )
				{
					m_currNode->m_type	= GXmlNode::eXmlCharData;
					m_iAnalysis	= eCharDataEnd;
				}
			}
			else if ( m_iAnalysis == eAttribEqualSign )
			{
				attribValue.clear();
				m_currNode->m_attributes[attribName]	= attribValue;
				attribName	= ch;
				m_iAnalysis	= eAttribNameStart;
			}
			else if ( m_iAnalysis == eAttribNameStart || m_iAnalysis == eAttribNameEnd ) {
				if ( ch == '?' && CUR_TAG_NAM[0] == '?' )
				{
					m_iAnalysis = eBracketRight;
				}
				else
				{
					attribName += ch;
					m_iAnalysis	= eAttribNameEnd;
				}
			}
			else if ( m_iAnalysis == eAttribQuotesRight || m_iAnalysis == eAttribSingleQuotes )
				attribValue += ch;
			else if ( m_iAnalysis == eAttribQuotesLeft ) {
				attribValue += ch;
			}
			else if ( m_iAnalysis == eBracketLeft || ( m_iAnalysis >= eCommentEnd && m_iAnalysis != eBracketRight ) )
				CUR_TAG_VAL += ch;
			break;
		}
	}
	return true;
}

bool GXmlDocument::analysisXmlDeclare()
{
	if ( !m_declareGeted )
	{
		wstring	codec	= m_currNode->attribute( "encoding" ).toWString();
		if ( codec.empty() )
		{
			return false;
		}
		if ( GXmlNode::compareNoCase( codec, L"utf-8" ) == 0 ||
			GXmlNode::compareNoCase( codec, L"utf8" ) == 0 )
		{
			m_texCocec	= GFile::eCodecUtf8;
		}
		else if ( GXmlNode::compareNoCase( codec, L"utf-16" ) == 0 ||
			GXmlNode::compareNoCase( codec, L"utf16" ) == 0 ||
			GXmlNode::compareNoCase( codec, L"unicode" ) == 0 )
		{
			m_texCocec	= GFile::eCodecUtf16;
		}
		else if ( GXmlNode::compareNoCase( codec, L"gb-2312" ) == 0 ||
			GXmlNode::compareNoCase( codec, L"gb2312" ) == 0 ||
			GXmlNode::compareNoCase( codec, L"GBK" ) == 0 ||
			GXmlNode::compareNoCase( codec, L"GB18030" ) == 0 ||
			GXmlNode::compareNoCase( codec, L"GB-18030" ) == 0 )
		{
			m_texCocec	= GFile::eCodecGBK;
		}
		else if ( GXmlNode::compareNoCase( codec, L"utf-16be" ) == 0 ||
			GXmlNode::compareNoCase( codec, L"utf16be" ) == 0 )
		{
			m_texCocec	= GFile::eCodecUtf16BE;
		}
		m_declareGeted	= true;
		return true;
	}
	return false;
}

wstring GXmlDocument::texCodec( GFile::ETextCodec codec )
{
	switch ( codec )
	{
	case  GFile::eCodecUnknow:
		break;
	case  GFile::eCodecAnsi:
		break;
	case  GFile::eCodecUtf8:
		return L"utf-8";
	case  GFile::eCodecUtf16:
		return L"utf-16";
	case  GFile::eCodecUtf32:
		return L"utf-32";
	case  GFile::eCodecUtf16BE:
		return L"utf-16be";
	case  GFile::eCodecUtf32BE:
		return L"utf-32be";
	case  GFile::eCodecGBK:
		return L"gbk";
	}
	return wstring();
}

int32_t GXmlNode::compareNoCase( const wstring & str1, const wstring & str2 )
{
	const wchar_t*	p1	= str1.c_str();
	const wchar_t*	p2	= str2.c_str();
	while ( *p1 && *p2 )
	{
		wchar_t	c1	= ( *p1 >= 'A' && *p1 <= 'Z' ) ? *p1 - 'A' + 'a' : *p1;
		wchar_t	c2	= ( *p2 >= 'Z' && *p2 <= 'Z' ) ? *p2 - 'A' + 'a' : *p1;
		if ( c1 < c2 )
			return -1;
		else if ( c1 > c2 )
			return 1;
		++p1;
		++p2;
	}
	if ( *p1 == 0 && *p2 == 0 )
		return 0;
	else if ( *p1 == 0 )
		return -1;
	return 1;
}

void GXmlNode::xmlCharDecode( wstring& str )
{
	wstring			input	= str;
	wstring&		output	= str;
	const wchar_t*	pSta	= input.c_str();
	const wchar_t*	pEnd	= pSta;
	output.clear();
	while ( *pEnd )
	{
		if ( *pEnd == '&' )
		{
			if ( wcsncmp( pEnd, L"&amp;", 5 ) == 0 )
			{
				output.append( pSta, pEnd - pSta );
				output.append( L"&" );
				pEnd	+= 5;
				pSta	= pEnd;
				continue;
			}
			else if ( wcsncmp( pEnd, L"&lt;", 4 ) == 0 )
			{
				output.append( pSta, pEnd - pSta );
				output.append( L"<" );
				pEnd	+= 4;
				pSta	= pEnd;
				continue;
			}
			else if ( wcsncmp( pEnd, L"&gt;", 4 ) == 0 )
			{
				output.append( pSta, pEnd - pSta );
				output.append( L">" );
				pEnd	+= 4;
				pSta	= pEnd;
				continue;
			}
			else if ( wcsncmp( pEnd, L"&quot;", 6 ) == 0 )
			{
				output.append( pSta, pEnd - pSta );
				output.append( L"\"" );
				pEnd	+= 6;
				pSta	= pEnd;
				continue;
			}
			else if ( wcsncmp( pEnd, L"&apos;", 6 ) == 0 )
			{
				output.append( pSta, pEnd - pSta );
				output.append( L"\'" );
				pEnd	+= 6;
				pSta	= pEnd;
				continue;
			}
		}
		++pEnd;
	}
	if ( pEnd != pSta )
	{
		output.append( pSta, pEnd - pSta );
	}
}

void GXmlNode::xmlCharEncode( wstring& str )
{
	wstring			input	= str;
	wstring&		output	= str;
	output.clear();
	xmlCharEncode( input, output );
}

void GXmlNode::xmlCharEncode( const wstring & input, wstring & output )
{
	const wchar_t*	pSta	= input.c_str();
	const wchar_t*	pEnd	= pSta;
	while ( *pEnd )
	{
		switch ( *pEnd )
		{
		case '&':
			output.append( pSta, pEnd - pSta );
			output.append( L"&amp;", 5 );
			pSta	= ++pEnd;
			continue;
		case '<':
			output.append( pSta, pEnd - pSta );
			output.append( L"&lt;", 4 );
			pSta	= ++pEnd;
			continue;
		case '>':
			output.append( pSta, pEnd - pSta );
			output.append( L"&gt;", 4 );
			pSta	= ++pEnd;
			continue;
		case '\"':
			output.append( pSta, pEnd - pSta );
			output.append( L"&quot;", 6 );
			pSta	= ++pEnd;
			continue;
		case '\'':
			output.append( pSta, pEnd - pSta );
			output.append( L"&apos;", 6 );
			pSta	= ++pEnd;
			continue;
		}
		++pEnd;
	}
	if ( pEnd != pSta )
	{
		output.append( pSta, pEnd - pSta );
	}
}

GXmlNode::GXmlNode( GXmlNode * parent )
	:GBaseNode(parent)
{
	m_objectType	= eWidget;
	m_type	= eXmlUnknow;
}

GXmlNode::~GXmlNode()
{
}

GXmlNode * GXmlNode::appendChildElement( const string & tagName )
{
	if ( tagName.empty() ) return nullptr;
	GXmlNode*	child	= new GXmlNode( this );
	child->m_tagName	= tagName;
	return child;
}

GXmlNode * GXmlNode::appendChildElement( const wstring & tagName )
{
	if ( tagName.empty() ) return nullptr;
	GXmlNode*	child	= new GXmlNode( this );
	child->m_tagName	= tagName;
	return child;
}

GXmlNode * GXmlNode::childElement( const string & tagName )
{
	GXmlNode*	node	= firstChild();
	while ( node )
	{
		if ( node->m_type == eXmlElement )
		{
			if ( tagName.empty() || tagName == node->m_tagName.toString() )
			{
				break;
			}
		}
		node	= node->nextNode();
	}
	return node;
}

GXmlNode * GXmlNode::childElement( const wstring & tagName )
{
	GXmlNode*	node	= firstChild();
	while ( node )
	{
		if ( node->m_type == eXmlElement )
		{
			if ( tagName.empty() || tagName == node->m_tagName.toWString() )
			{
				break;
			}
		}
		node	= node->nextNode();
	}
	return node;
}

GXmlNode * GXmlNode::nextElement( const string & tagName )
{
	GXmlNode*	node	= this->nextNode();
	while ( node )
	{
		if ( node->m_type == eXmlElement )
		{
			if ( tagName.empty() || tagName == node->m_tagName.toString() )
			{
				break;
			}
		}
		node	= node->nextNode();
	}
	return node;
}

GXmlNode * GXmlNode::nextElement( const wstring & tagName )
{
	GXmlNode*	node	= this->nextNode();
	while ( node )
	{
		if ( node->m_type == eXmlElement )
		{
			if ( tagName.empty() || tagName == node->m_tagName.toWString() )
			{
				break;
			}
		}
		node	= node->nextNode();
	}
	return node;
}

bool GXmlNode::toString( wstring& str )
{
	str.clear();
	return toString( str, this );
}

bool GXmlNode::toString( string & str )
{
	wstring	strw;
	if ( toString( strw, this ) )
	{
		str	= GFile::toUtf8( strw );
		return true;
	}
	return false;
}

bool GXmlNode::toString( wstring& str, GXmlNode* node )
{
	GXmlNode*	topNode	= node;
	int	iLtrim		= 0;
	do
	{
		str.append( ' ', iLtrim * 4 );

		switch ( node->m_type )
		{
		case eXmlElement:
			str		+= L"<";
			str		+= node->m_tagName;
			if ( node->m_attributes.size() )
			{
				str		+= L' ';
				for ( map<wstring, GVariant>::iterator i = node->m_attributes.begin(); i != node->m_attributes.end(); ++i )
				{
					str	+= i->first;
					str	+= L"=\"";
					xmlCharEncode( i->second, str );
					str	+= L"\" ";
				}
			}
			if ( node->m_tagValue.toWString().empty() )
			{
				if ( node->firstChild() )
				{
					str	+= L">\r\n";
					node	= node->firstChild();
					++iLtrim;
					continue;
				}
				else
				{
					if ( node->m_attributes.size() )
						str	+= L"/>\r\n";
					else
						str	+= L" />\r\n";
				}
			}
			else
			{
				if ( node->firstChild() )
				{
					str	+= L">\r\n";
					str.append( ' ', ( iLtrim + 1 ) * 4 );
					xmlCharEncode( node->m_tagValue, str );
					str	+= L"\r\n";
					node	= node->firstChild();
					++iLtrim;
					continue;
				}
				else if ( node->m_tagValue.toWString().length() <= 80 )
				{
					str	+= L">";
					xmlCharEncode( node->m_tagValue, str );
					str	+= L"</";
					str	+= node->m_tagName;
					str	+= L">\r\n";
				}
				else
				{
					str	+= L">\r\n";
					str.append( ' ', ( iLtrim + 1 ) * 4 );
					xmlCharEncode( node->m_tagValue, str );
					str.append( ' ', iLtrim * 4 );
					str	+= L"</";
					str	+= node->m_tagName;
					str	+= L">\r\n";
				}
			}
			if ( node == topNode ) break;
			if ( node->nextNode() )
			{
				node	= node->nextNode();
			}
			else
			{
				while ( node )
				{
					--iLtrim;
					node	= node->parentNode();
					if ( nullptr == node ) break;
					str.append( ' ', iLtrim * 4 );
					str	+= L"</";
					str	+= node->m_tagName;
					str	+= L">\r\n";
					if ( node == topNode ) break;
					if ( node->nextNode() )
					{
						node	= node->nextNode();
						break;
					}
				}
			}
			break;
		case eXmlDeclare:
			str		+= L"<?xml";
			if ( node->m_attributes.size() )
			{
				str		+= L' ';
				for ( map<wstring, GVariant>::iterator i = node->m_attributes.begin(); i != node->m_attributes.end(); ++i )
				{
					str	+= i->first;
					str	+= L"=\"";
					xmlCharEncode( i->second, str );
					str	+= L"\" ";
				}
			}
			str		+= L"?>\r\n";
			if ( node == topNode ) break;
			node	= node->nextNode();
			break;
		case eXmlHtmlDocType:
			str		+= L"<!DOCTYPE";
			if ( node->m_attributes.size() )
			{
				str		+= L' ';
				for ( map<wstring, GVariant>::iterator i = node->m_attributes.begin(); i != node->m_attributes.end(); ++i )
				{
					str	+= i->first;
					str	+= L" ";
				}
			}
			str		+= L">\r\n";
			if ( node == topNode ) break;
			node	= node->nextNode();
			break;
		case eXmlComment:
			str		+= L"<!--";
			str.append( node->m_tagValue );
			str		+= L"-->\r\n";
			if ( node == topNode ) break;
			node	= node->nextNode();
			break;
		case eXmlCharData:
			str		+= L"<![CDATA[";
			str.append( node->m_tagValue );
			str		+= L"]]>\r\n";
			if ( node == topNode ) break;
			node	= node->nextNode();
			break;
		default:
			str		+= L"<";
			str		+= node->m_tagName;
			if ( node->m_attributes.size() )
			{
				str		+= L' ';
				for ( map<wstring, GVariant>::iterator i = node->m_attributes.begin(); i != node->m_attributes.end(); ++i )
				{
					str	+= i->first;
					str	+= L"=\"";
					xmlCharEncode( i->second, str );
					str	+= L"\" ";
				}
			}
			if ( node->m_tagName.toWString()[0] == '?' )
				str		+= L"?>\r\n";
			else
				str		+= L">\r\n";
			if ( node == topNode ) break;
			node	= node->nextNode();
			break;
		}
	} while ( node && node != topNode );

	return true;
}