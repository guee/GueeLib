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

	//移动当前节点
	//node		: 目标节点
	//moveto	: 相对目标节点的位置。
	bool moveToNode( GBaseNode* node, EMoveTo moveto = eMoveToFirstChild );

	EObjectType objectType() const { return m_objectType; }
protected:
	GBaseNode*	m_nextNode;			//下一个
	GBaseNode*	m_prevNode;			//上一个
	GBaseNode*	m_parentNode;		//父
	GBaseNode*	m_firstChild;		//第一个子节点
	GBaseNode*	m_lastChild;		//最后一个子节点
	int32_t		m_childCount;		//子节点数量
	EObjectType	m_objectType;		//节点的类型
private:
	//解除本节点与父及兄弟节点的关联，以便插入到其它位置
	void removeCorrelation();
};

