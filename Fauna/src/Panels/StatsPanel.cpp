#include "flpch.h"
#include "StatsPanel.h"
#include "Flora/Renderer/Renderer2D.h"
#include <imgui/imgui.h>
#include <imgui/implot.h>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace Flora {
	void StatsPanel::Initialize() {
		// initialize datamap
		m_DataMap[Stats::DRAWCALLS] = {};
		m_DataMap[Stats::QUADS] = {};
		m_DataMap[Stats::VERTICES] = {};
		m_DataMap[Stats::INDICES] = {};
		m_DataMap[Stats::FRAMETIME] = {};
		m_DataMap[Stats::FPS] = {};
		m_DataMap[Stats::LOWEST_FPS] = {};
		m_DataMap[Stats::HIGHEST_FPS] = {};
		m_DataMap[Stats::CPU_USAGE] = {};
		m_DataMap[Stats::GPU_USAGE] = {};
		m_DataMap[Stats::MEM_USAGE] = {};
		m_DataMap[Stats::DISK_USAGE] = {};

		//get ram size
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(memInfo);
		GlobalMemoryStatusEx(&memInfo);
		DWORDLONG ramSize = memInfo.ullTotalPhys;
		m_Ramsize = ramSize / (1024.0 * 1024.0 * 1024.0);
		FL_CORE_INFO(m_Ramsize);

		//initialize pdh queries
		PdhOpenQuery(NULL, NULL, &cpuQuery);
		PdhAddCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
		PdhCollectQueryData(cpuQuery);
		PdhOpenQuery(NULL, NULL, &gpuQuery);
		PdhAddCounter(gpuQuery, L"\\GPU Engine(*)\\Utilization Percentage", NULL, &gpuTotal);
		PdhCollectQueryData(gpuQuery);
		PdhOpenQuery(NULL, NULL, &memQuery);
		PdhAddCounter(memQuery, L"\\Memory\\Available Bytes", NULL, &memTotal);
		PdhCollectQueryData(memQuery);
		PdhOpenQuery(NULL, NULL, &diskQuery);
		PdhAddCounter(diskQuery, L"\\PhysicalDisk(_Total)\\% Disk Time", NULL, &diskTotal);
		PdhCollectQueryData(diskQuery);
	}

	void StatsPanel::OnImGuiRender() {
		if (m_EditorContext->ShowStatMap.size() <= 0) {
			m_EditorContext->ShowStatMap[Stats::DRAWCALLS] = true;
			m_EditorContext->ShowStatMap[Stats::QUADS] = true;
			m_EditorContext->ShowStatMap[Stats::VERTICES] = true;
			m_EditorContext->ShowStatMap[Stats::INDICES] = true;
			m_EditorContext->ShowStatMap[Stats::FRAMETIME] = true;
			m_EditorContext->ShowStatMap[Stats::FPS] = true;
			m_EditorContext->ShowStatMap[Stats::LOWEST_FPS] = true;
			m_EditorContext->ShowStatMap[Stats::HIGHEST_FPS] = true;
			m_EditorContext->ShowStatMap[Stats::CPU_USAGE] = true;
			m_EditorContext->ShowStatMap[Stats::GPU_USAGE] = true;
			m_EditorContext->ShowStatMap[Stats::MEM_USAGE] = true;
			m_EditorContext->ShowStatMap[Stats::DISK_USAGE] = true;
		}

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen;
		ImVec2 buttonSize = { 180, 0 };

		ImGui::Begin("Stats", &m_Enabled);
		if (ImGui::IsWindowHovered()) m_EditorContext->HoveredPanel = Panels::STATS;
		if (ImGui::IsWindowFocused()) m_EditorContext->FocusedPanel = Panels::STATS;

		if (ImGui::TreeNodeEx("Renderer Volume", treeNodeFlags)) {
			m_RendererStats = Renderer2D::GetStats();
			ImGui::Columns(3);
			ImGui::SetColumnWidth(0, 100);
			ImGui::SetColumnWidth(1, 200);
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("Draw Calls");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("Quads");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("Vertices");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("Indices");
			ImGui::Dummy({ 0, 2 });
			ImGui::NextColumn();
			ImGui::Button(std::to_string(m_RendererStats.DrawCalls).c_str(), buttonSize);
			ImGui::Button(std::to_string(m_RendererStats.QuadCount).c_str(), buttonSize);
			ImGui::Button(std::to_string(m_RendererStats.GetTotalVertexCount()).c_str(), buttonSize);
			ImGui::Button(std::to_string(m_RendererStats.GetTotalIndexCount()).c_str(), buttonSize);
			ImGui::NextColumn();
			ImGui::Checkbox("##drawcalls", &m_EditorContext->ShowStatMap[Stats::DRAWCALLS]);
			ImGui::Checkbox("##quads", &m_EditorContext->ShowStatMap[Stats::QUADS]);
			ImGui::Checkbox("##vertices", &m_EditorContext->ShowStatMap[Stats::VERTICES]);
			ImGui::Checkbox("##indices", &m_EditorContext->ShowStatMap[Stats::INDICES]);
			ImGui::Columns(1);
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNodeEx("Video Performance", treeNodeFlags)) {
			int fps = (int)(1.0f / m_Frametime);
			if (fps < m_LowestFPS) m_LowestFPS = fps;
			if (fps > m_HighestFPS) m_HighestFPS = fps;
			float timestep = std::round(m_Frametime * 100000) / 100;
			std::ostringstream out;
			out << std::fixed << std::setprecision(2) << timestep;
			ImGui::Columns(3);
			ImGui::SetColumnWidth(0, 100);
			ImGui::SetColumnWidth(1, 200);
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("Timestep");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("FPS");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("FPS Floor");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("FPS Ceiling");
			ImGui::Dummy({ 0, 2 });
			ImGui::NextColumn();
			ImGui::Button((out.str() + " us").c_str(), buttonSize);
			ImGui::Button(std::to_string(fps).c_str(), buttonSize);
			ImGui::Button(std::to_string(m_LowestFPS).c_str(), buttonSize);
			ImGui::Button(std::to_string(m_HighestFPS).c_str(), buttonSize);
			ImGui::NextColumn();
			ImGui::Checkbox("##frametime", &m_EditorContext->ShowStatMap[Stats::FRAMETIME]);
			ImGui::Checkbox("##fps", &m_EditorContext->ShowStatMap[Stats::FPS]);
			ImGui::Checkbox("##fpsfloor", &m_EditorContext->ShowStatMap[Stats::LOWEST_FPS]);
			ImGui::Checkbox("##fpsceiling", &m_EditorContext->ShowStatMap[Stats::HIGHEST_FPS]);
			ImGui::Columns(1);
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNodeEx("Hardware Performance", treeNodeFlags)) {
			float latestUsage = 0.0f;
			std::ostringstream out;

			if (m_DataMap[Stats::CPU_USAGE].size() > 0) latestUsage = m_DataMap[Stats::CPU_USAGE].back();
			latestUsage = std::round(latestUsage * 100) / 100;
			out << std::fixed << std::setprecision(2) << latestUsage;
			std::string cpuUsage = out.str() + "%";
			out.str("");

			if (m_DataMap[Stats::GPU_USAGE].size() > 0) latestUsage = m_DataMap[Stats::GPU_USAGE].back();
			latestUsage = std::round(latestUsage * 100) / 100;
			out << std::fixed << std::setprecision(2) << latestUsage;
			std::string gpuUsage = out.str() + "%";
			out.str("");

			if (m_DataMap[Stats::MEM_USAGE].size() > 0) latestUsage = m_DataMap[Stats::MEM_USAGE].back();
			latestUsage = std::round(latestUsage * 100) / 100;
			out << std::fixed << std::setprecision(2) << latestUsage;
			std::string memUsage = out.str() + "%";
			out.str("");

			if (m_DataMap[Stats::DISK_USAGE].size() > 0) latestUsage = m_DataMap[Stats::DISK_USAGE].back();
			latestUsage = std::round(latestUsage * 100) / 100;
			out << std::fixed << std::setprecision(2) << latestUsage;
			std::string diskUsage = out.str() + "%";
			out.str("");

			ImGui::Columns(3);
			ImGui::SetColumnWidth(0, 100);
			ImGui::SetColumnWidth(1, 200);
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("CPU");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("GPU");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("Memory");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("Disk");
			ImGui::Dummy({ 0, 2 });
			ImGui::NextColumn();
			ImGui::Button(cpuUsage.c_str(), buttonSize);
			ImGui::Button(gpuUsage.c_str(), buttonSize);
			ImGui::Button(memUsage.c_str(), buttonSize);
			ImGui::Button(diskUsage.c_str(), buttonSize);
			ImGui::NextColumn();
			ImGui::Checkbox("##cpuusage", &m_EditorContext->ShowStatMap[Stats::CPU_USAGE]);
			ImGui::Checkbox("##gpuusage", &m_EditorContext->ShowStatMap[Stats::GPU_USAGE]);
			ImGui::Checkbox("##memusage", &m_EditorContext->ShowStatMap[Stats::MEM_USAGE]);
			ImGui::Checkbox("##diskusage", &m_EditorContext->ShowStatMap[Stats::DISK_USAGE]);
			ImGui::Columns(1);
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNodeEx("Graph Settings", treeNodeFlags)) {
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 100);
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("Graph History");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("Axis Scalar");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("Axis Minimum");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("Reset Stats");
			ImGui::Dummy({ 0, 2 });
			ImGui::Text("Show Key");
			ImGui::Dummy({ 0, 2 });
			ImGui::NextColumn();
			ImGui::SetNextItemWidth(180);
			ImGui::DragFloat("##history", &(m_EditorContext->Timeframe), 0.01f, 0.0f, 10000.0f, "%.2f");
			ImGui::SetNextItemWidth(180);
			ImGui::DragFloat("##axisscale", &(m_EditorContext->AxisScale), 0.01f, 0.0f, 10000.0f, "%.2f");
			ImGui::SetNextItemWidth(180);
			ImGui::DragFloat("##axismin", &(m_EditorContext->AxisMinimum), 0.01f, 0.0f, 1000000.0f, "%.2f");
			if (ImGui::Button("RESET", buttonSize)) ResetStats();
			ImGui::Checkbox("##showkey", &(m_EditorContext->ShowGraphKey));
			ImGui::Columns(1);
			ImGui::TreePop();
		}

		if (m_ShowGraph && m_FrameCountData.size() != 0) {
			ImGui::Begin("Graphs", NULL, ImGuiWindowFlags_NoScrollbar);

			ImPlot::PushStyleColor(ImPlotCol_FrameBg, { 0,0,0,0 });
			ImPlot::PushStyleColor(ImPlotCol_AxisBgHovered, { 0,0,0,0 });
			ImPlot::PushStyleColor(ImPlotCol_AxisBgActive, { 0,0,0,0 });

			ImPlot::SetNextAxesLimits(m_FrameCountData.front(), m_FrameCountData.back(), m_EditorContext->AxisMinimum, GetYMax(), ImPlotCond_Always);

			ImPlotFlags flags = ImPlotFlags_NoTitle;
			if (!(m_EditorContext->ShowGraphKey)) flags |= ImPlotFlags_NoLegend;
			ImVec2 plotsize = ImGui::GetWindowContentRegionMax();
			plotsize.y -= 20.0f;
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 10.0f);
			if (ImPlot::BeginPlot("Graph data", plotsize, flags)) {
				float* frameCountData = &m_FrameCountData[0];

				if (m_EditorContext->ShowStatMap[Stats::DRAWCALLS]) {
					float* drawCallData = &m_DataMap[Stats::DRAWCALLS][0];
					ImPlot::PlotLine("Draw Calls", frameCountData, drawCallData, m_FrameCountData.size());
				}

				if (m_EditorContext->ShowStatMap[Stats::QUADS]) {
					float* drawQuadData = &m_DataMap[Stats::QUADS][0];
					ImPlot::PlotLine("Quads", frameCountData, drawQuadData, m_FrameCountData.size());
				}

				if (m_EditorContext->ShowStatMap[Stats::VERTICES]) {
					float* vertexData = &m_DataMap[Stats::VERTICES][0];
					ImPlot::PlotLine("Vertices", frameCountData, vertexData, m_FrameCountData.size());
				}

				if (m_EditorContext->ShowStatMap[Stats::INDICES]) {
					float* indexData = &m_DataMap[Stats::INDICES][0];
					ImPlot::PlotLine("Indices", frameCountData, indexData, m_FrameCountData.size());
				}

				if (m_EditorContext->ShowStatMap[Stats::FRAMETIME]) {
					float* frametimeData = &m_DataMap[Stats::FRAMETIME][0];
					ImPlot::PlotLine("Frame time (us)", frameCountData, frametimeData, m_FrameCountData.size());
				}

				if (m_EditorContext->ShowStatMap[Stats::FPS]) {
					float* fpsData = &m_DataMap[Stats::FPS][0];
					ImPlot::PlotLine("FPS", frameCountData, fpsData, m_FrameCountData.size());
				}

				if (m_EditorContext->ShowStatMap[Stats::LOWEST_FPS]) {
					float* lowestFPSData = &m_DataMap[Stats::LOWEST_FPS][0];
					ImPlot::PlotLine("FPS Floor", frameCountData, lowestFPSData, m_FrameCountData.size());
				}

				if (m_EditorContext->ShowStatMap[Stats::HIGHEST_FPS]) {
					float* highestFPSData = &m_DataMap[Stats::HIGHEST_FPS][0];
					ImPlot::PlotLine("FPS Ceiling", frameCountData, highestFPSData, m_FrameCountData.size());
				}

				if (m_EditorContext->ShowStatMap[Stats::CPU_USAGE]) {
					float* cpuUsageData = &m_DataMap[Stats::CPU_USAGE][0];
					ImPlot::PlotLine("CPU", frameCountData, cpuUsageData, m_FrameCountData.size());
				}

				if (m_EditorContext->ShowStatMap[Stats::GPU_USAGE]) {
					float* gpuUsageData = &m_DataMap[Stats::GPU_USAGE][0];
					ImPlot::PlotLine("GPU", frameCountData, gpuUsageData, m_FrameCountData.size());
				}

				if (m_EditorContext->ShowStatMap[Stats::MEM_USAGE]) {
					float* memUsageData = &m_DataMap[Stats::MEM_USAGE][0];
					ImPlot::PlotLine("Memory", frameCountData, memUsageData, m_FrameCountData.size());
				}

				if (m_EditorContext->ShowStatMap[Stats::DISK_USAGE]) {
					float* diskUsageData = &m_DataMap[Stats::DISK_USAGE][0];
					ImPlot::PlotLine("Disk", frameCountData, diskUsageData, m_FrameCountData.size());
				}

				ImPlot::EndPlot();
			}

			ImPlot::PopStyleColor(3);
			ImGui::End();
		}

		ImGui::End();
	}

	static void EmplaceAndPop(std::vector<float>& data, float newpoint, bool pop) {
		data.emplace_back(newpoint);
		if (pop) data.erase(data.begin());
	}

	void StatsPanel::OnUpdate() {
		// set up hardware delay
		m_HardwareCollectionDelay += m_Frametime;
		if (m_HardwareCollectionDelay > 0.25f) m_HardwareCollectionDelay = 0.0f;
		bool readyToCollect = m_HardwareCollectionDelay == 0.0f;

		// get pdh query values
		PdhCollectQueryData(cpuQuery);
		PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &cpuCounterVal);
		double cpuUsage = cpuCounterVal.doubleValue;
		PdhCollectQueryData(gpuQuery);
		PdhGetFormattedCounterValue(gpuTotal, PDH_FMT_DOUBLE, NULL, &gpuCounterVal);
		double gpuUsage = gpuCounterVal.doubleValue;
		PdhCollectQueryData(diskQuery);
		PdhGetFormattedCounterValue(diskTotal, PDH_FMT_DOUBLE, NULL, &diskCounterVal);
		double diskUsage = diskCounterVal.doubleValue;
		PdhCollectQueryData(memQuery);
		PdhGetFormattedCounterValue(memTotal, PDH_FMT_DOUBLE, NULL, &memCounterVal);
		double memUsage = (memCounterVal.doubleValue / (1024.0 * 1024.0 * 1024.0)) / m_Ramsize;

		// compensate for delay
		if (!readyToCollect) {
			cpuUsage = m_DataMap[Stats::CPU_USAGE].back();
			gpuUsage = m_DataMap[Stats::GPU_USAGE].back();
			memUsage = m_DataMap[Stats::GPU_USAGE].back();
			diskUsage = m_DataMap[Stats::GPU_USAGE].back();
		}

		// update frame count
		m_FrameCount++;
		int fps = (1.0f / m_Frametime);

		// limit to time frame
		float period = 0.0f;
		for (int i = 0; i < m_DataMap[Stats::FRAMETIME].size(); i++)
			period += m_DataMap[Stats::FRAMETIME][i];
		bool pop = period > (m_EditorContext->Timeframe * 1000.0f);

		// update graph values
		EmplaceAndPop(m_FrameCountData, (float)m_FrameCount, pop);
		EmplaceAndPop(m_DataMap[Stats::FRAMETIME], 1000 * m_Frametime, pop);
		EmplaceAndPop(m_DataMap[Stats::DRAWCALLS], (float)(m_RendererStats.DrawCalls), pop);
		EmplaceAndPop(m_DataMap[Stats::QUADS], (float)(m_RendererStats.QuadCount), pop);
		EmplaceAndPop(m_DataMap[Stats::VERTICES], (float)(m_RendererStats.GetTotalVertexCount()), pop);
		EmplaceAndPop(m_DataMap[Stats::INDICES], (float)(m_RendererStats.GetTotalIndexCount()), pop);
		EmplaceAndPop(m_DataMap[Stats::FPS], fps, pop);
		EmplaceAndPop(m_DataMap[Stats::LOWEST_FPS], m_LowestFPS, pop);
		EmplaceAndPop(m_DataMap[Stats::HIGHEST_FPS], m_HighestFPS, pop);
		EmplaceAndPop(m_DataMap[Stats::CPU_USAGE], cpuUsage, pop);
		EmplaceAndPop(m_DataMap[Stats::GPU_USAGE], gpuUsage, pop);
		EmplaceAndPop(m_DataMap[Stats::MEM_USAGE], memUsage, pop);
		EmplaceAndPop(m_DataMap[Stats::DISK_USAGE], diskUsage, pop);
	}

	static float GetMaxFromVector(std::vector<float> vec) {
		return (float)(*std::max_element(vec.begin(), vec.end()));
	}

	static float CascadeMax(float oldMax, float newMax) {
		return newMax > oldMax ? newMax : oldMax;
	}

	float StatsPanel::GetYMax() {
		float max = 0.0f;
		if (m_EditorContext->ShowStatMap[Stats::FRAMETIME]) max = CascadeMax(max, 60.0f);
		if (m_EditorContext->ShowStatMap[Stats::DRAWCALLS]) max = CascadeMax(max, 10.0f);
		if (m_EditorContext->ShowStatMap[Stats::QUADS]) max = CascadeMax(max, GetMaxFromVector(m_DataMap[Stats::QUADS]) * 1.1f);
		if (m_EditorContext->ShowStatMap[Stats::VERTICES]) max = CascadeMax(max, GetMaxFromVector(m_DataMap[Stats::VERTICES]) * 1.1f);
		if (m_EditorContext->ShowStatMap[Stats::INDICES]) max = CascadeMax(max, GetMaxFromVector(m_DataMap[Stats::INDICES]) * 1.1f);
		if (m_EditorContext->ShowStatMap[Stats::FPS]) max = CascadeMax(max, 144);
		if (m_EditorContext->ShowStatMap[Stats::LOWEST_FPS]) max = CascadeMax(max, GetMaxFromVector(m_DataMap[Stats::FPS]) * 1.1f);
		if (m_EditorContext->ShowStatMap[Stats::HIGHEST_FPS]) max = CascadeMax(max, GetMaxFromVector(m_DataMap[Stats::HIGHEST_FPS]) * 1.1f);
		if (m_EditorContext->ShowStatMap[Stats::CPU_USAGE]) max = CascadeMax(max, 100.0f);
		if (m_EditorContext->ShowStatMap[Stats::GPU_USAGE]) max = CascadeMax(max, 100.0f);
		if (m_EditorContext->ShowStatMap[Stats::MEM_USAGE]) max = CascadeMax(max, 100.0f);
		if (m_EditorContext->ShowStatMap[Stats::DISK_USAGE]) max = CascadeMax(max, 100.0f);
		return max * m_EditorContext->AxisScale;
	}
}