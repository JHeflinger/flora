#include "flpch.h"
#include "ConsolePanel.h"
#include "Flora/Renderer/Renderer2D.h"
#include <imgui/imgui.h>
#include "Flora/Utils/DeveloperUtils.h"

namespace Flora {
	void ConsolePanel::OnImGuiRender() {
		auto rendererStats = Renderer2D::GetStats();
		ImGui::Begin("Console", &m_Enabled);
		if (ImGui::IsWindowHovered()) m_EditorContext->HoveredPanel = Panels::CONSOLE;
		if (ImGui::IsWindowFocused()) m_EditorContext->FocusedPanel = Panels::CONSOLE;
		for (int i = 0; i < m_Logs.size(); i++) {
			ImVec4 color;
			switch (m_Logs[i].Type) {
			case LogType::TRACE:
				color = { 1, 1, 1, 1 };
				break;
			case LogType::INFO:
				color = { 0.1, 0.9, 0.3, 1 };
				break;
			case LogType::WARNING:
				color = { 0.95, 0.95, 0.25, 1 };
				break;
			case LogType::FATAL:
				color = { 0.9, 0.25, 0.1, 1 };
				break;
			default:
				FL_CORE_ASSERT(false, "invalid log type!");
			}
			ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::Text(m_Logs[i].Log.c_str());
			ImGui::PopStyleColor();
		}
		ImGui::End();

		RenderDevInput();
	}

	void ConsolePanel::RenderDevInput() {
		ImGui::Begin("Developer Input", &m_Enabled);
		ImVec2 contentRegionMax = ImGui::GetWindowContentRegionMax();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
		float remainingHeight = contentRegionAvailable.y;
		float inputHeight = ImGui::GetFrameHeight();
		remainingHeight -= inputHeight + ImGui::GetStyle().ItemSpacing.y;

		ImGui::BeginChild("MainContent", ImVec2(contentRegionAvailable.x, remainingHeight), false);
		std::vector<std::string> commandLogs = DeveloperUtils::ReadLogs();
		std::vector<std::string> timeLogs = DeveloperUtils::ReadTimeLogs();
		uint32_t unresolvedCommands = DeveloperUtils::CountQueuedCommands();
		for (int i = 0; i < commandLogs.size(); i++) {
			bool isUnresolved = false;
			if (i >= commandLogs.size() - unresolvedCommands) {
				ImGui::PushStyleColor(ImGuiCol_Text, { 0.95, 0.95, 0.25, 1 });
				isUnresolved = true;
			}
			ImGui::Text((timeLogs[i] + " " + commandLogs[i]).c_str());
			if (isUnresolved) ImGui::PopStyleColor();
		}
		ImGui::EndChild();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().ItemSpacing.y);

		static char inputCommand[512] = "";
		ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
		static bool enterPressed = false;
		if (enterPressed) {
			ImGui::SetKeyboardFocusHere();
			enterPressed = false;
		}
		enterPressed = ImGui::InputText("##Input", inputCommand, 512, ImGuiInputTextFlags_EnterReturnsTrue);
		if (enterPressed) {
			DeveloperUtils::SendCommand(std::string(inputCommand));
			inputCommand[0] = '\0';
		}

		ImGui::End();
	}

	void ConsolePanel::OnUpdate() {
		// clear logs if there are too many
		if (m_Logs.size() > m_MaxLogs) Clear();

		std::vector<std::string> newCoreLogs = ::Flora::Log::GetCoreRingBuffer()->last_formatted();
		if (newCoreLogs.size() < m_CoreLogs.size()) m_CoreLogs.clear();
		if (newCoreLogs.size() > m_CoreLogs.size())
			for (int i = m_CoreLogs.size(); i < newCoreLogs.size(); i++) {
				std::string input_string = newCoreLogs[i];
				m_CoreLogs.push_back(input_string);
				size_t start_pos = input_string.find("[", input_string.find("[", input_string.find("[") + 1) + 1) + 1;
				size_t end_pos = input_string.find("]", start_pos);
				std::string type_string = input_string.substr(start_pos, end_pos - start_pos);
				if (type_string == "trace")
					Trace(input_string);
				else if (type_string == "warning")
					Warning(input_string);
				else if (type_string == "info")
					Info(input_string);
				else if (type_string == "error")
					Fatal(input_string);
				else
					FL_CORE_ASSERT(false, "Log does not fit correct format");
			}

		std::vector<std::string> newClientLogs = ::Flora::Log::GetClientRingBuffer()->last_formatted();
		if (newClientLogs.size() < m_ClientLogs.size()) m_ClientLogs.clear();
		if (newClientLogs.size() > m_ClientLogs.size())
			for (int i = m_ClientLogs.size(); i < newClientLogs.size(); i++) {
				std::string input_string = newClientLogs[i];
				m_ClientLogs.push_back(input_string);
				size_t start_pos = input_string.find("[", input_string.find("[", input_string.find("[") + 1) + 1) + 1;
				size_t end_pos = input_string.find("]", start_pos);
				std::string type_string = input_string.substr(start_pos, end_pos - start_pos);
				if (type_string == "trace")
					Trace(input_string);
				else if (type_string == "warning")
					Warning(input_string);
				else if (type_string == "info")
					Info(input_string);
				else if (type_string == "error")
					Fatal(input_string);
				else
					FL_CORE_ASSERT(false, "Log does not fit correct format");
			}
	}

	std::string ConsolePanel::SnipLog(std::string log, int snips) {
		std::string result = log;
		for (int i = 0; i < snips; i++) {
			bool inBracket = false;
			int startIndex = 0;
			for (int i = 0; i < result.size(); i++) {
				if (result[i] == '[') {
					inBracket = true;
					startIndex = i;
				}
				else if (result[i] == ']' && inBracket) {
					inBracket = false;
					result.erase(startIndex, i - startIndex + 1);
					break;
				}
			}
		}
		return result;
	}

	void ConsolePanel::AddLog(LogType type, std::string log) {
		ConsoleLog clg;
		clg.Type = type;
		clg.Log = SnipLog(log);
		AddLog(clg);
		if (clg.Type == LogType::FATAL) m_EditorContext->Error = SnipLog(log, 3);
	}

	void ConsolePanel::AddLog(ConsoleLog log) {
		m_Logs.push_back(log);
	}

	void ConsolePanel::Trace(std::string log) {
		AddLog(LogType::TRACE, log);
	}

	void ConsolePanel::Info(std::string log) {
		AddLog(LogType::INFO, log);
	}

	void ConsolePanel::Warning(std::string log) {
		AddLog(LogType::WARNING, log);
	}

	void ConsolePanel::Fatal(std::string log) {
		AddLog(LogType::FATAL, log);
	}
}