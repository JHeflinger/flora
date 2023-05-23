#pragma once

namespace Flora {
	class DeveloperUtils {
	public:
		static void Init();
		static void ClearLogs() { m_CommandLog->clear(); m_QueuedCommands->clear(); m_TimeLog->clear(); }
		static std::vector<std::string> ReadLogs() { return *m_CommandLog; }
		static std::vector<std::string> ReadTimeLogs() { return *m_TimeLog; }
	public:
		static void SendCommand(std::string command);
		static std::string GetQueuedCommand();
		static bool HasQueuedCommand();
		static void ResolveCommand();
		static uint32_t CountQueuedCommands() { return (uint32_t)m_QueuedCommands->size(); }
	private:
		static Ref<std::vector<std::string>> m_CommandLog;
		static Ref<std::vector<std::string>> m_QueuedCommands;
		static Ref<std::vector<std::string>> m_TimeLog;
	};
}