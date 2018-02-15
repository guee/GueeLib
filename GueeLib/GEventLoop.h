#pragma once
#include <map>
#include <thread>
using namespace std;
class GEventLoop
{
public:
	GEventLoop();
	~GEventLoop();

	
	static bool enableLoop();
	static bool disableLoop();
	static map<thread::id, GEventLoop>	m_threadLoops;
private:
	thread::id	m_tid;
	int32_t		m_reference;
};

