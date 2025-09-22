// 53616c7465645f5f4ad3fc5e8fcde55c182286f0a530a2e3d25585c79a9d2887d159d8fff2b6a43ac9bab9a76c5729e0

#include "DbClient.hpp"
#include <iostream>
#include "Utils.h"

DbClient::DbClient() {
	const char* supabaseUrl = std::getenv("SUPABASE_URL");
	const char* supabaseKey = std::getenv("SUPABASE_KEY");

	if (supabaseUrl == nullptr) {
		throw std::runtime_error("Missing SUPABASE_URL environment variable");
	}
	if (supabaseKey == nullptr) {
		throw std::runtime_error("Missing SUPABASE_KEY environment variable");
	}

	apiUrl = std::string(supabaseUrl) + "/rest/v1";
	apiKey = std::string(supabaseKey);
}

// ----PRIVATE HELPERS----
cpr::Parameters DbClient::buildQueryParameters(const std::map<std::string, std::string>& query) {
	cpr::Parameters params;
	for (const auto& [key, value] : query) {
		params.Add({ key, value });
	}
	return params;
}

cpr::AsyncResponse DbClient::get(const std::string& endpoint, const std::map<std::string , std::string>& query) {
	const std::string& url = apiUrl + "/" + endpoint;
	return cpr::GetAsync(
		cpr::Url{ url },
		cpr::Parameters{ buildQueryParameters(query) },
		cpr::Header{
			{ "apikey", apiKey},
			{ "Authorization", "Bearer " + apiKey}
		}
	);
}

cpr::AsyncResponse DbClient::getWithCount(const std::string& endpoint, const std::map<std::string, std::string>& query) {
	const std::string& url = apiUrl + "/" + endpoint;
	return cpr::GetAsync(
		cpr::Url{ url },
		cpr::Parameters{ buildQueryParameters(query) },
		cpr::Header{
			{ "apikey", apiKey},
			{ "Authorization", "Bearer " + apiKey},
			{ "Prefer", "count=exact" }
		}
	);
}

cpr::AsyncResponse DbClient::post(const std::string& endpoint, const nlohmann::json& body) {
	const std::string& url = apiUrl + "/" + endpoint;
	return cpr::PostAsync(
		cpr::Url{ url },
		cpr::Body{ body.dump()},
		cpr::Header{
			{ "apikey", apiKey},
			{ "Authorization", "Bearer " + apiKey},
			{ "Content-Type", "application/json" }
		}
	);
}

cpr::AsyncResponse DbClient::patch(const std::string& endpoint, const nlohmann::json& body) {
	const std::string url = apiUrl + "/" + endpoint;
	return cpr::PatchAsync(
		cpr::Url{ url },
		cpr::Body{ body.dump() },         
		cpr::Header{
			{ "apikey", apiKey },
			{ "Authorization", "Bearer " + apiKey },
			{ "Content-Type", "application/json" }
		}
	);
}

std::vector<Movie> parseMovieListFromJson(const std::string& jsonString) {
	std::vector<Movie> movies;
	try {
		nlohmann::json j = nlohmann::json::parse(jsonString);
		for (const auto& item : j) {
			movies.push_back(item.get<Movie>());
		}
	}
	catch (const std::exception& e) {
		std::cerr << "JSON parsing error: " << e.what() << std::endl;
	}
	return movies;
}

int DbClient::resolveCount(cpr::Response* res) {

	if (res->error) {
		std::cerr << "Error: " << res->error.message << std::endl;
		return -1;
	}

	std::string contentRange = res->header["Content-Range"];
	if (!contentRange.empty()) {
		auto delimiter = contentRange.find('/');
		if (delimiter != std::string::npos) {
			std::string totalStr = contentRange.substr(delimiter + 1);
			try {
				return std::stoi(totalStr);
			}
			catch (const std::exception& e) {
				std::cerr << "Error parsing total count: " << e.what() << std::endl;
				return -1;
			}
		}
	}

	return -1;
}

// ----PUBLIC FUNCTIONS----
int DbClient::countAllMovies() {
	const std::map<std::string, std::string> params = {
		{"select", "*"}
	};
	auto future = getWithCount("movie", params);
	cpr::Response res = future.get();
	return resolveCount(&res);
}

int DbClient::countUnwatchedMovies() {
	const std::map<std::string, std::string> params = {
		{ "select", "*" },
		{ "is_watched", "eq.false" }
	};	
	auto future = getWithCount("movie", params);
	cpr::Response res = future.get();
	return resolveCount(&res); 
}

int DbClient::countRolls() {
	// Compute time threshold (24 hours ago base on UTC)
	std::string timeThreshold = getCurrentTime1DayThreshold();
	const std::map<std::string, std::string> params = {
		{ "select", "*" },
		{ "or", "(last_rolled_at.is.not_null, last_rolled_at.lte." + timeThreshold + ")" }
	};
	auto future = getWithCount("movie", params);
	cpr::Response res = future.get();
	return resolveCount(&res); 
}

std::vector<Movie> DbClient::fetchUnwatchedMovies() {
	const std::map<std::string, std::string> params = {
		{ "select", "*" },
		{ "is_watched", "eq.false" }
	};
	auto future = get("movie", params);
	cpr::Response res = future.get();

	if (res.status_code >= 200 && res.status_code < 300) {
		return parseMovieListFromJson(res.text);
	}

	return {}; 
}

std::vector<Movie> DbClient::fetchUnwatchedSafeMovies() {
	const std::map<std::string, std::string> params = {
		{ "select", "*" },
		{ "is_watched", "eq.false" },
		{ "is_safe", "eq.true" }
	};
	auto future = get("movie", params);
	cpr::Response res = future.get();

	if (res.status_code >= 200 && res.status_code < 300) {
		return parseMovieListFromJson(res.text);
	}

	return {}; 
}

std::vector<Movie> DbClient::fetchUnwatchedUnsafeMovies() {
	const std::map<std::string, std::string> params = {
		{ "select", "*" },
		{ "is_watched", "eq.false" },
		{ "is_safe", "eq.false" }
	};
	auto future = get("movie", params);
	cpr::Response res = future.get();

	if (res.status_code >= 200 && res.status_code < 300) {
		return parseMovieListFromJson(res.text);
	}

	return {}; 
}

std::vector<Movie> DbClient::fetchWatchedMovies() {
	const std::map<std::string, std::string> params = {
		{ "select", "*" },
		{ "is_watched", "eq.true" },
	};
	auto future = get("movie", params);
	cpr::Response res = future.get();

	if (res.status_code >= 200 && res.status_code < 300) {
		return parseMovieListFromJson(res.text);
	}

	return {}; 
}

std::vector<Movie> DbClient::fetchRoulettePoolMovies() {
	// Compute time threshold (24 hours ago base on UTC)
	std::string timeThreshold = getCurrentTime1DayThreshold();
	const std::map<std::string, std::string> params = {
		{ "select", "*" },
	};
	auto future = get("movie_roulette_pool_view", params);
	cpr::Response res = future.get();
	if (res.status_code >= 200 && res.status_code < 300) {
		return parseMovieListFromJson(res.text);
	}
	return {}; 
}

bool DbClient::insertMovie(const std::string& title, bool is_safe) {
	const nlohmann::json body = {
		{ "title", title },
		{ "is_safe", is_safe }
	};
	auto future = post("movie", body);
	cpr::Response res = future.get();

	if (res.status_code >= 200 && res.status_code < 300)
		return true;
	
	return false;
}

bool DbClient::markMovieAsWatched(long long movie_id) {
	nlohmann::json body = {
	   { "is_watched", true }
	};

	std::string endpoint = "movie?id=eq." + std::to_string(movie_id);
	auto future = patch(endpoint, body);
	cpr::Response res = future.get();

	if (res.status_code >= 200 && res.status_code < 300) 
		return true;

	return false;
}

bool DbClient::markMoviesArRolled(const std::vector<long long>& movie_ids) {
	if (movie_ids.empty()) return false;

	// Build comma-separated ID list
	std::ostringstream oss;
	for (size_t i = 0; i < movie_ids.size(); ++i) {
		oss << movie_ids[i];
		if (i < movie_ids.size() - 1) oss << ",";
	}

	std::string idList = oss.str();
	std::string endpoint = "movie?id=in.(" + idList + ")";

	nlohmann::json body = {
		{ "last_rolled_at", getCurrentTimestamp() }
	};
	auto future = patch(endpoint, body);
	cpr::Response res = future.get();

	if (res.status_code >= 200 && res.status_code < 300) 
		return true;
	
	return false;
}