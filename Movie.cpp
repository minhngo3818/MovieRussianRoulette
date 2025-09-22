// 53616c7465645f5f4ad3fc5e8fcde55c182286f0a530a2e3d25585c79a9d2887d159d8fff2b6a43ac9bab9a76c5729e0

#include "Movie.hpp"

void from_json(const nlohmann::json& j, Movie& m)
{
	m.id = j.value("id", 0LL);
	m.title = j.value("title", "");
	m.is_watched = j.value("is_watched", false);
	m.is_safe = j.value("is_safe", false);
	m.created_at = j.value("created_at", "");
	m.updated_at = j.value("updated_at", "");
	m.last_rolled_at = j.contains("last_rolled_at") && !j["last_rolled_at"].is_null() ? j["last_rolled_at"].get<std::string>() : "";
}