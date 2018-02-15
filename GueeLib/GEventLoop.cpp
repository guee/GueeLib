#include "stdafx.h"
#include "GEventLoop.h"

map<thread::id, GEventLoop>	GEventLoop::m_threadLoops;
GEventLoop::GEventLoop()
{
	m_tid	= this_thread::get_id();
	m_reference	= 0;
}


GEventLoop::~GEventLoop()
{
}

bool GEventLoop::enableLoop()
{
	thread::id	tid	= this_thread::get_id();
	GEventLoop&	evt	= m_threadLoops[tid];
	if ( evt.m_reference == 0 )
	{

	}
	return true;
}

bool GEventLoop::disableLoop()
{
	return false;
}
