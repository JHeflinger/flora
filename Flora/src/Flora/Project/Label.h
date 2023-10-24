#pragma once

namespace Flora {
	class Label {
	public:
		Label(std::string name) : m_Name(name) {}
		std::string Name() { return m_Name; }
	private:
		std::string m_Name;
	};
}