#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

int main()
{
    std::ifstream input_file("IMDB-Movie-Data.csv");
    auto line_count = std::count(std::istreambuf_iterator<char>(input_file),
                                 std::istreambuf_iterator<char>(), '\n');
    input_file.seekg(0, std::ios::beg); // Seek begining of file

    std::vector<std::string> movies;
    movies.reserve(line_count);

    std::string line;
    std::getline(input_file, line);     // Ignore first line

    while (std::getline(input_file, line))
    {
        std::stringstream line_stream(line);
        std::string title;
        std::getline(line_stream, title, ',');  // Ignore index

        std::getline(line_stream, title, ',');  // Movie Title
        movies.push_back(title);
    }

    input_file.close();
    int holl_idx = 1;
    std::string movie, hollywood;
    std::vector<char> good_guesses, bad_guesses, allowed_chars;

    auto reset_game = [&holl_idx, &movie, &hollywood, &good_guesses, &bad_guesses, &allowed_chars, &line_count, &movies]()
    {
        holl_idx = 1;
        hollywood = "HOLLYWOOD";
        good_guesses.clear();
        bad_guesses.clear();
        allowed_chars = { 'A', 'E', 'I', 'O', 'U', ' ', '-', '?', ':', '\'', '.', ',', '!',
                          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '(', ')', '/' };

        std::random_device rand_device;                                             // Obtain a random number from hardware
        std::mt19937 generator(rand_device());                                      // seed the generator
        std::uniform_int_distribution<> dist(0, static_cast<int>(line_count) - 1);  // define the range

        movie = movies[dist(generator)];
        std::transform(movie.begin(), movie.end(), movie.begin(), std::toupper);
    };

    reset_game();   // Initialize
    int wins = 0, losses = 0;

    do
    {
        // 1-Blue 2-Green 3-Aqua 4-Red 5-Purple 6-Yellow 7-White 8-Gray 9-Light Blue 0-Black
        // A-Light Green B-Light Aqua C-Light Red D-Light Purple E-Light Yellow F-Bright White
        system("Color 75"); // Background + text color
        system("cls");      // Clear screen

        std::cout << "\t\t\t" << hollywood << "\t\tW: " << wins << " L: " << losses << "\n\n\t\t";

        if (!good_guesses.empty())
        {
            std::cout << "Good Guesses: ";
            for (auto ch : good_guesses)
                std::cout << ch << " ";
            std::cout << "\n\n\t\t";
        }

        if (!bad_guesses.empty())
        {
            std::cout << "Bad Guesses:  ";
            for (auto ch : bad_guesses)
                std::cout << ch << " ";
            std::cout << "\n\n\t\t";
        }

        //std::cout << movie << "\n\t\t";     // For debugging purpose
        int dash_count = 0;
        for (auto ch : movie)
        {
            if (std::find(allowed_chars.begin(), allowed_chars.end(), ch) != allowed_chars.end())
                std::cout << ch;
            else
            {
                dash_count++;
                std::cout << "_";
            }
        }

        if (dash_count == 0)
        {
            wins++;
            std::cout << "\n\n\t\tSUCCESS!!!\n\nDo you wish to play again? (Y/N) ";
            char input_char;
            std::cin >> input_char;
            if (input_char == 'y' || input_char == 'Y')
            {
                reset_game();
                continue;
            }
            else
                break;
        }

        std::cout << "\n\nEnter guess: ";
        char input_char;
        std::cin >> input_char;
        input_char = std::toupper(input_char);

        if (movie.find(input_char) != std::string::npos)
        {
            if (std::find(good_guesses.begin(), good_guesses.end(), input_char) != good_guesses.end())
                continue;

            allowed_chars.push_back(input_char);
            good_guesses.push_back(input_char);
        }
        else
        {
            if (std::find(bad_guesses.begin(), bad_guesses.end(), input_char) != bad_guesses.end())
                continue;

            if (holl_idx == 9)
            {
                losses++;
                std::cout << "\n\nMovie was: " << movie << "\n\n\t\tGAME OVER!!\n\nDo you wish to play again? (Y/N) ";
                char input_char;
                std::cin >> input_char;
                if (input_char == 'y' || input_char == 'Y')
                {
                    reset_game();
                    continue;
                }
                else
                    break;
            }

            std::string holl_str;
            for (auto i = 0; i < holl_idx; i++)
                holl_str += "-";

            hollywood = holl_str + hollywood.substr(holl_idx++);
            bad_guesses.push_back(input_char);
        }
    } while (true);

    system("pause");
    return EXIT_SUCCESS;
}
