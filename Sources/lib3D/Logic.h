#pragma once
#include <string>
#include <map>

namespace libEngine
{
	class LOGICBASE
	{
	public:
		LOGICBASE(std::string sName);
		virtual ~LOGICBASE();

		void Register();
		void Unregister();

		virtual LOGICBASE* CreateInstance() = 0;
		virtual void OnStart() = 0;
		virtual void OnProcess(float TimerDelta) = 0;
		virtual void OnEnd() = 0;

		void SetActionRadius(float fActionRadius);
		float GetActionRadius();
		std::string GetName();
	private:
		static std::string m_sName;
		static float m_fActionRadius;
	};

	class LOGICMANAGER
	{
	public:
		LOGICMANAGER();
		virtual ~LOGICMANAGER();

		bool Register(std::string& sName,LOGICBASE* Logic);
		bool Unregister(std::string& sName);
	private:
		std::map<std::string,LOGICBASE*> LogicMap;
	};
	LOGICMANAGER LogicManager;
}
