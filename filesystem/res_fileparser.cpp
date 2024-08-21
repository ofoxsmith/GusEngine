#include "res_fileparser.h"
#include <regex>
void ResourceFileParser::LoadResource(const string filePath)
{
}

// Probably highly inefficient but it works
ResourceFileParser::ParsedPropertyResourceFile ResourceFileParser::LoadPropResource(const string filePath)
{
	ParsedPropertyResourceFile output;

	string data = file_helpers::read_file_text(filePath);
	
	if (!data.starts_with("[PRES]\n")) throw new runtime_error("Invalid PRES file data");
	// Remove lines with only whitespace
	std::regex wspace("\\n\\s*\\n");
	data = std::regex_replace(data, wspace, "\n");

	std::string metaData;
	std::string mainData;

	std::regex const meta{ R"(^\[PRES\]\n([^\[]+))" };
	std::smatch m_meta;
	std::regex_search(data, m_meta, meta);
	if (m_meta.size() == 0) throw new runtime_error("Invalid PRES file data");
	metaData = m_meta[1].str();
	mainData = m_meta.suffix();
	
	// Read metadata section first

	std::regex const metaProps{ R"---(([a-zA-Z]+) (?:(?:"([^"]+)")|(?:\(([a-zA-Z]+), ?([0-9]+), ? ([0-9]+|EOF)\)))\n)---" };
	std::smatch m_metaProps;

	// Consume all key value pairs until metaData is empty
	while (std::regex_search(metaData, m_metaProps, metaProps)) {
		string propName = m_metaProps[1];
		string value = m_metaProps[2];

		if (propName == "name") {
			output.resourceName = value;

		}
		if (propName == "dataFile") {
			output.dataPath = value;
		}
		if (propName == "loader") {
			PropertyResourceSourceLoader loader;
			loader.type = m_metaProps[3];
			loader.start = std::stoi(m_metaProps[4]);
			loader.end = m_metaProps[5] == "EOF" ? INT32_MAX : std::stoi(m_metaProps[5]);
			output.dataLoader = loader;
		}

		metaData = m_metaProps.suffix();
	}

	std::regex const mainProps{ R"---(([a-zA-Z-]+): (.+))---" };
	std::smatch m_mainProps;

	// Consume all key value pairs
	while (std::regex_search(mainData, m_mainProps, mainProps)) {
		std::cout << "eee" << endl;
		string key = m_mainProps[1];
		string value = m_mainProps[2];
		if (std::regex_match(value, std::regex{ R"("[^"]*")" })) {
			output.mainSectionStringData[key] = value.substr(1, value.size() - 2);
		}
		else if (std::regex_match(value, std::regex{ R"([0-9]+)" })) {
			output.mainSectionIntData[key] = std::stoi(value);
		}
		else {
			output.mainSectionUnprocessedData[key] = value;
		}
		mainData = m_mainProps.suffix();
	}
	return output;

}