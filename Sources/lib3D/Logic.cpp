#include "Logic.h"

using namespace libEngine;

LOGICMANAGER LogicManager;


LOGICBASE::LOGICBASE(std::string sName) 
{
	m_sName = sName;
	m_fActionRadius = 0;
	Register();
}
LOGICBASE::~LOGICBASE() 
{
	Unregister();
}

void LOGICBASE::Register()
{
}
void LOGICBASE::Unregister()
{
}

/*LOGICBASE* LOGICBASE::CreateInstance()
{
	return 0;
}
void LOGICBASE::Start() 
{
}
void LOGICBASE::Process(float TimerDelta) 
{
}
void LOGICBASE::End() 
{
}
*/
void LOGICBASE::SetActionRadius(float fActionRadius) 
{
	m_fActionRadius = fActionRadius;
}
float LOGICBASE::GetActionRadius() 
{
	return m_fActionRadius;
}
std::string LOGICBASE::GetName()
{
	return m_sName;
}

LOGICMANAGER::LOGICMANAGER()
{
}
LOGICMANAGER::~LOGICMANAGER()
{
}
bool LOGICMANAGER::Register(std::string& sName,LOGICBASE* Logic)
{
	std::map<std::string,LOGICBASE*>::iterator it = LogicMap.find(sName);
	if(it == LogicMap.end())
	{
		LogicMap[sName] = Logic;
		return true;
	}
	return false;
}
bool LOGICMANAGER::Unregister(std::string& sName)
{
	std::map<std::string,LOGICBASE*>::iterator it = LogicMap.find(sName);
	if(it != LogicMap.end())
	{
		delete it->second;
		LogicMap.erase(it);
		return true;
	}
	return false;
}
