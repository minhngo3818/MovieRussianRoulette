// 53616c7465645f5f4ad3fc5e8fcde55c182286f0a530a2e3d25585c79a9d2887d159d8fff2b6a43ac9bab9a76c5729e0

#pragma once

#ifndef DBCLIENT_HPP
#define DBCLIENT_HPP

#include <string>
#include <vector>
#include <map>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "Movie.hpp"

// API Client
class DbClient {
private:
	std::string apiUrl;
	std::string apiKey;

	// private helpers
	cpr::AsyncResponse get(const std::string& endpoint, const std::map<std::string, std::string>& query);
	cpr::AsyncResponse getWithCount(const std::string& endpoint, const std::map<std::string, std::string>& query);
	cpr::AsyncResponse post(const std::string& endpoint, const nlohmann::json& body);
	cpr::AsyncResponse patch(const std::string& endpoint, const nlohmann::json& body);
	cpr::Parameters buildQueryParameters(const std::map<std::string, std::string>& query);
	int resolveCount(cpr::Response* res);

public:

	DbClient();

	int countAllMovies();
	int countUnwatchedMovies();
	int countRolls();
	std::vector<Movie> fetchUnwatchedMovies();
	std::vector<Movie> fetchUnwatchedSafeMovies();
	std::vector<Movie> fetchUnwatchedUnsafeMovies();
	std::vector<Movie> fetchWatchedMovies();
	std::vector<Movie> fetchRoulettePoolMovies();
	bool insertMovie(const std::string& title, bool is_safe);
	bool markMovieAsWatched(long long movie_id);
	bool markMoviesArRolled(const std::vector<long long>& movie_ids);
};

#endif