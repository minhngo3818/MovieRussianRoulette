# Movie Russian Roulette
A cli to create thrill before watching your favorite movies using Supabase as a backend.  
A fun way to decide what movie to watch next by adding an element of surprise and excitement.  

## Features
- Randomly selects an unwatched movie from your collection 5 safe movie, 1 unsafe.
- Uses Supabase as a backend to store and manage your movie collection.
- Command line interface for easy interaction.
- Lightweight and easy to set up.

## Prerequisites
- gcc
- make
- cpr
- nlohmann
- vcpkg
- Supabase

## Tables
```
movie
    id:             int8
    title:          varchar (255)
    is_watched:     bool
    is_safe:        bool
    created_at:     timestampz
    updated_at:     timestampz
    last_rolled_at: timestampz
stats
    id:             int8
    total:          int8
```

## Setup Instructions
1. Clone the package repository:
   ```bash
   git clone
2. Navigate to the project directory:
   ```bash
   cd movie-russian-roulette
   ```
3. Install dependencies using vcpkg:
   ```bash
   vcpkg install
   ```
4. Build the project using make:
   ```bash
   make
   ```
5. Set up your Supabase project and obtain your API URL and Key.
6. Add your movie collection to the Supabase database
7. Add view to supabase:
   ```SQL
       CREATE OR REPLACE VIEW movie_roulette_pool_view AS
    (
      SELECT *
      FROM movie
      WHERE 
        is_safe = true
        AND is_watched = false
        AND (last_rolled_at IS NULL OR last_rolled_at < current_timestamp - interval '1 day')
      ORDER BY random()
      LIMIT 5
    )
    UNION ALL
    (
      SELECT *
      FROM movie
      WHERE 
        is_safe = false
        AND is_watched = false
        AND (last_rolled_at IS NULL OR last_rolled_at < current_timestamp - interval '1 day')
      ORDER BY random()
      LIMIT 1
    );
   ```
8. Set the environment variables:
   ```bash
   export SUPABASE_URL_=your_supabase_url
   export SUPABASE_KEY=your_supabase_key
   ```

   or add them to System Environment Variables on Windows.
1. 
9. Run the application

## How to use
1. Set the environment variables:
   ```bash
   export SUPABASE_URL_=your_supabase_url
   export SUPABASE_KEY=your_supabase_key
   ```

   or add them to System Environment Variables on Windows.

2. Run the application

3. Follow the prompts to roll the dice and select a movie.

[53616c7465645f5f4ad3fc5e8fcde55c182286f0a530a2e3d25585c79a9d2887d159d8fff2b6a43ac9bab9a76c5729e0]