// 53616c7465645f5f4ad3fc5e8fcde55c182286f0a530a2e3d25585c79a9d2887d159d8fff2b6a43ac9bab9a76c5729e0

#include "Cli.hpp"
#include "Utils.h"
#include <iostream>

Cli::Cli() {
	DbClient dbClient = DbClient();
	srand(static_cast<unsigned int>(time(0))); // Seed for randomness
}

void Cli::printMenu() {
	std::cout << "Select an option from the menu below:" << std::endl;
	std::cout << "1. Start Roulette" << std::endl;
	std::cout << "2. Add a movie" << std::endl;
	std::cout << "3. Get unwatched movies" << std::endl;
	std::cout << "4. Get unwatched safe movies" << std::endl;
	std::cout << "5. Get unwatched unsafe movies" << std::endl;
	std::cout << "6. Get watched movies" << std::endl;
	std::cout << "7. Exit" << std::endl;
	std::cout << "Enter your choice: ";
}

void Cli::printStats() {
	std::cout << "Displaying stats..." << std::endl;
	int total = dbClient.countAllMovies();
	int unwatchedCount = dbClient.countUnwatchedMovies();
	int rolledCount = dbClient.countRolls();
	std::cout << "Total: " << total << ", Unwatched: " << unwatchedCount << ", Rolled: " << rolledCount << std::endl;
}

void Cli::startRoulette() {
	std::cout << "Starting the movie roulette..." << std::endl;
	std::vector<Movie> pool = dbClient.fetchRoulettePoolMovies();
	if (pool.size() < 6) {
		std::cout << "Not enough movies to start the roulette. Please add more movies." << std::endl;
		return;
	}
	std::cout << "You have 6 movies to choose from." << std::endl;
	printMovies(pool);

	int random_index = rand() % pool.size();
	std::cout << "You rolled: " << pool[random_index].title << " (id=" << pool[random_index].id << ")" << std::endl;

	std::vector<long long> movieIds;	
	for (const auto& movie : pool) {
		movieIds.push_back(movie.id);
	}

	dbClient.markMoviesArRolled(movieIds);

	std::cout << "Did you watch the movie? (Y/n): ";
	char watchedInput;
	std::cin >> watchedInput;
	if (watchedInput == 'Y' || watchedInput == 'y') {
		dbClient.markMovieAsWatched(pool[random_index].id);
		std::cout << "Marked as watched." << std::endl;
	} else {
		std::cout << "Marked as unwatched." << std::endl;
	}
	
	std::cout << "\n";
}

void Cli::addMovie() {
	std::string title;
	bool isSafe = false;
	char safeInput;

	std::cout << "Adding a new movie..." << std::endl;
	std::cout << "-> Enter movie title: ";
	std::cin.ignore(); // Clear newline character from input buffer
	std::getline(std::cin, title);
	std::cout << "-> Is the movie safe? (Y/n): ";
	std::cin >> safeInput;
	isSafe = (safeInput == 'Y' || safeInput == 'y');

	bool success = dbClient.insertMovie(title, isSafe);
	std::cout << (success ? title + " was added successfully." : "Failed to add movie.") << "\n\n";
}

void Cli::printUnwatchedMovies() {
	std::cout << "Fetching unwatched movies..." << std::endl;
	std::vector<Movie> movies = dbClient.fetchUnwatchedMovies();

	if (movies.empty()) {
		std::cout << "No unwatched movies available." << std::endl;
		return;
	}

	printMovies(movies);
}

void Cli::printUnwatchedSafeMovies() {
	std::cout << "Fetching unwatched safe movies..." << std::endl;
	std::vector<Movie> movies = dbClient.fetchUnwatchedSafeMovies();

	if (movies.empty()) {
		std::cout << "No unwatched safe movies available." << std::endl;
		return;
	}

	printMovies(movies);
}

void Cli::printUnwatchedUnsafeMovies() {
	std::cout << "Fetching unwatched unsafe movies..." << std::endl;
	std::vector<Movie> movies = dbClient.fetchUnwatchedUnsafeMovies();

	if (movies.empty()) {
		std::cout << "No unwatched unsafe movies available." << std::endl;
		return;
	}

	printMovies(movies);
}

void Cli::printWatchedMovies() {
	std::cout << "Fetching watched movies..." << std::endl;
	std::vector<Movie> movies = dbClient.fetchWatchedMovies();

	if (movies.empty()) {
		std::cout << "No watched movies available." << std::endl;
		return;
	}

	printMovies(movies);
}

void Cli::printMovies(const std::vector<Movie>& movies) {
	for (const auto& movie : movies) {
		std::cout << "ID: " << movie.id << ", Title: " << movie.title
			<< ", Watched: " << (movie.is_watched ? "Yes" : "No")
			<< ", Safe: " << (movie.is_safe ? "Yes" : "No")
			<< ", Last Rolled At: " << movie.last_rolled_at
			<< std::endl;
	}

	std::cout << "\n";
}

void Cli::run() {
	std::cout << "Welcome to Movie Russian Roulette!" << std::endl;
	std::cout << "         *Laugh vicously*" << std::endl;
	std::cout << "====================================" << std::endl;
	std::cout << "You have 6 movies to choose from." << std::endl;
	std::cout << "One of them is a terrible movie." << std::endl;
	std::cout << "If you pick the terrible movie, you lose!" << std::endl;
	std::cout << "If you pick a good movie, you win!" << std::endl;
	std::cout << "All 6 rolled movies won't be available until the next day" << std::endl;
	std::cout << "====================================" << std::endl;

	printStats();

	bool run = true;
	while (run) {
		printMenu();
		int choice;
		std::cin >> choice;
		switch (choice) {
		case 1:
			startRoulette();
			break;
		case 2:
			addMovie();
			break;
		case 3:
			printUnwatchedMovies();
			break;
		case 4:
			printUnwatchedSafeMovies();
			break;
		case 5:
			printUnwatchedUnsafeMovies();
			break;
		case 6:
			printWatchedMovies();
			break;
		case 7:
			std::cout << "Exiting the program. Goodbye!" << std::endl;
			return;
		default:
			std::cout << "Invalid choice. Please try again." << std::endl;
			break;
		}
	}
}