#pragma once
#include <stdint.h>

#define		DISABLE_CLASS_COPY(Class)	private:\
##Class( const Class& other ){};\
Class& operator=( const Class& other ){};

class GBaseNode
{
	DISABLE_CLASS_COPY( GBaseNode );
public:
	GBaseNode( GBaseNode* parent = nullptr );
	virtual ~GBaseNode();

	enum EObjectType
	{
		eBase,
		eWidget,
		eDesktop,
		eXmlNode,
	};

	enum EMoveTo
	{
		eMoveToFirstChild,
		eMoveToLastChild,
		eMoveToBefore,
		eMoveToAfter,
	};

	//�ƶ���ǰ�ڵ�
	//node		: Ŀ��ڵ�
	//moveto	: ���Ŀ��ڵ��λ�á�
	bool moveToNode( GBaseNode* node, EMoveTo moveto = eMoveToFirstChild );

	EObjectType objectType() const { return m_objectType; }
protected:
	GBaseNode*	m_nextNode;			//��һ��
	GBaseNode*	m_prevNode;			//��һ��
	GBaseNode*	m_parentNode;		//��
	GBaseNode*	m_firstChild;		//��һ���ӽڵ�
	GBaseNode*	m_lastChild;		//���һ���ӽڵ�
	int32_t		m_childCount;		//�ӽڵ�����
	EObjectType	m_objectType;		//�ڵ������
private:
	//������ڵ��븸���ֵܽڵ�Ĺ������Ա���뵽����λ��
	void removeCorrelation();
};

