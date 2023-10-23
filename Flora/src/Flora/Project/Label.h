#pragma once

namespace Flora {
	class Label {
	public:
		Label(std::string name) : m_Name(name) {}
		int Weight();
		std::string m_Name;
	};
}