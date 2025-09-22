// 53616c7465645f5f4ad3fc5e8fcde55c182286f0a530a2e3d25585c79a9d2887d159d8fff2b6a43ac9bab9a76c5729e0

#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <string>
#include <nlohmann/json.hpp>

struct Movie {
	long long id;
	std::string title;
	bool is_watched;
	bool is_safe;
	std::string created_at;
	std::string updated_at;
	std::string last_rolled_at;
};

void from_json(const nlohmann::json& j, Movie& m);

#endif // MOVIE_HPP