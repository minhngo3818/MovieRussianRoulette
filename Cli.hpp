// 53616c7465645f5f4ad3fc5e8fcde55c182286f0a530a2e3d25585c79a9d2887d159d8fff2b6a43ac9bab9a76c5729e0

#pragma once

#ifndef CLI_HPP
#define CLI_HPP

#include "DbClient.hpp"
#include <cstdlib>

class Cli {
private:
	DbClient dbClient;
	void printMenu();
	void printStats();
	void printUnwatchedMovies();
	void printUnwatchedSafeMovies();
	void printUnwatchedUnsafeMovies();
	void printWatchedMovies();
	void addMovie();
	void startRoulette();
	void printMovies(const std::vector<Movie>& movies);

public:
	Cli();

	void run();
};

#endif // CLI_HPP