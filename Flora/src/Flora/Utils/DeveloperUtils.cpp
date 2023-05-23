#include "flpch.h"
#include "Flora/Utils/DeveloperUtils.h"
#include <ctime>

namespace Flora {
	Ref<std::vector<std::string>> DeveloperUtils::m_CommandLog = CreateRef<std::vector<std::string>>();
	Ref<std::vector<std::string>> DeveloperUtils::m_QueuedCommands = CreateRef<std::vector<std::string>>();
	Ref<std::vector<std::string>> DeveloperUtils::m_TimeLog = CreateRef<std::vector<std::string>>();

	static std::string ToTimeString(int time) {
		if (time < 10)
			return "0" + std::to_string(time);
		return std::to_string(time);
	}

	void DeveloperUtils::Init() {
		
	}

	void DeveloperUtils::SendCommand(std::string command) {
		m_QueuedCommands->emplace_back(command);
		m_CommandLog->emplace_back(command);

		std::time_t currTime = std::time(nullptr);
		std::tm* localTime = std::localtime(&currTime);
		int hour = localTime->tm_hour;
		int minute = localTime->tm_min;
		int second = localTime->tm_sec;
		m_TimeLog->emplace_back("[" + ToTimeString(hour) + ":" + ToTimeString(minute) + ":" + ToTimeString(second) + "]");
	}

	std::string DeveloperUtils::GetQueuedCommand() {
		if (m_QueuedCommands->size() > 0) {
			return m_QueuedCommands->back();
		}
		return "";
	}

	bool DeveloperUtils::HasQueuedCommand() {
		return m_QueuedCommands->size() > 0;
	}

	void DeveloperUtils::ResolveCommand() {
		if (HasQueuedCommand())
			m_QueuedCommands->pop_back();
	}
}