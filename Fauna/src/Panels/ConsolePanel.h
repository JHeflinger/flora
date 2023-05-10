#pragma once
#include "EditorPanel.h"

namespace Flora {
	enum class LogType { TRACE = 0, INFO = 1, WARNING = 2, FATAL = 3 };

	struct ConsoleLog {
		std::string Log = "";
		LogType Type = LogType::TRACE;
	};

	class ConsolePanel : public EditorPanel {
	public:
		ConsolePanel() = default;
		virtual void Initialize() override {};
		virtual void OnUpdate() override;
		virtual void OnImGuiRender() override;
		void AddLog(LogType type, std::string log);
		void AddLog(ConsoleLog log);
		void Trace(std::string log);
		void Info(std::string log);
		void Warning(std::string log);
		void Fatal(std::string log);
		void Clear() { m_Logs.clear(); }
	private:
		std::string SnipLog(std::string log, int snips = 1);
	private:
		std::vector<ConsoleLog> m_Logs;
		std::vector<std::string> m_CoreLogs;
		std::vector<std::string> m_ClientLogs;
		uint32_t m_MaxLogs = 1000;
	};
}