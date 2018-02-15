#include "stdafx.h"
#include "GBaseNode.h"


GBaseNode::GBaseNode( GBaseNode* parent )
{
	m_nextNode		= nullptr;
	m_prevNode		= nullptr;
	m_parentNode	= nullptr;
	m_firstChild	= nullptr;
	m_lastChild		= nullptr;
	m_childCount	= 0;
	m_objectType	= eBase;
	moveToNode( parent, eMoveToLastChild );
}

GBaseNode::~GBaseNode()
{
	while ( m_firstChild )
	{
		delete m_firstChild;
	}
	removeCorrelation();
}

bool GBaseNode::moveToNode( GBaseNode * node, EMoveTo moveto )
{
	//���Ŀ��ڵ��ǵ�ǰ�ڵ㣬�����ǵ�ǰ�ڵ���ӽڵ㣬�Ͳ������ƶ���
	GBaseNode*	parNode	= node;
	while ( parNode )
	{
		if ( parNode == this )
			return false;
		parNode	= parNode->m_parentNode;
	}
	switch ( moveto )
	{
	case eMoveToFirstChild:
	case eMoveToLastChild:
		//���Ŀ��ڵ��Ѿ��Ǹ��ڵ㣬����ǰ�ڵ��Ѿ�����Ŀ��λ�ã��Ͳ���Ҫ�ƶ���
		if ( node == m_parentNode )
		{
			if ( !node || node->m_childCount == 1 ||
				( node && moveto == eMoveToFirstChild && m_parentNode->m_firstChild == this ) ||
				( node && moveto == eMoveToLastChild && m_parentNode->m_lastChild == this ) )
				return true;
		}
		removeCorrelation();
		m_parentNode	= node;
		if ( m_parentNode )
		{
			if ( m_parentNode->m_childCount )
			{
				if ( moveto == eMoveToFirstChild )
				{
					m_nextNode	= m_parentNode->m_firstChild;
					m_parentNode->m_firstChild->m_prevNode	= this;
					m_parentNode->m_firstChild	= this;
				}
				else
				{
					m_prevNode	= m_parentNode->m_lastChild;
					m_parentNode->m_lastChild->m_nextNode	= this;
					m_parentNode->m_lastChild	= this;
				}
			}
			else
			{
				m_parentNode->m_firstChild	= this;
				m_parentNode->m_lastChild	= this;
			}
			++m_parentNode->m_childCount;
		}
		break;
	case eMoveToBefore:
	case eMoveToAfter:
		if ( nullptr == node ) return false;
		//���Ŀ��ڵ��뵱ǰ�ڵ�ͬ��������ǰ�ڵ��Ѿ�����Ŀ��λ�ã��Ͳ���Ҫ�ƶ���
		if ( node->m_parentNode == m_parentNode )
		{
			if ( ( moveto == eMoveToBefore && node->m_prevNode == this ) ||
				( moveto == eMoveToAfter && node->m_nextNode == this ) )
				return true;
		}
		removeCorrelation();
		m_parentNode	= node->m_parentNode;
		if ( moveto == eMoveToBefore )
		{
			if ( node->m_prevNode )
				node->m_prevNode->m_nextNode	= this;
			m_prevNode	= node->m_prevNode;
			node->m_prevNode	= this;
			m_nextNode	= node;
			if ( m_parentNode && m_parentNode->m_firstChild == node )
				m_parentNode->m_firstChild	= this;
		}
		else
		{
			if ( node->m_nextNode )
				node->m_nextNode->m_prevNode	= this;
			m_nextNode	= node->m_nextNode;
			node->m_nextNode	= this;
			m_prevNode	= node;
			if ( m_parentNode && m_parentNode->m_lastChild == node )
				m_parentNode->m_lastChild	= this;
		}
		if ( m_parentNode ) ++m_parentNode->m_childCount;
		break;
	}
	return true;
}

void GBaseNode::removeCorrelation()
{
	if ( m_prevNode && m_prevNode->m_nextNode == this )
		m_prevNode->m_nextNode	= m_nextNode;
	if ( m_nextNode && m_nextNode->m_prevNode == this )
		m_nextNode->m_prevNode	= m_prevNode;
	if ( m_parentNode )
	{
		if ( m_parentNode->m_firstChild == this )
			m_parentNode->m_firstChild	= m_nextNode;
		if ( m_parentNode->m_lastChild == this )
			m_parentNode->m_lastChild	= m_prevNode;
		--m_parentNode->m_childCount;
		m_parentNode	= nullptr;
	}
	m_nextNode	= nullptr;
	m_prevNode	= nullptr;
}
