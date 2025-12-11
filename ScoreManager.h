#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

struct ScoreEntry
{
    std::string name;
    int score;
};

class ScoreManager
{
private:
    const std::string filename = "wyniki.txt";

public:
    void saveScore(const std::string& nick, int points)
    {
        std::ofstream file(filename, std::ios::app);
        if (file.is_open())
        {
            file << nick << " " << points << "\n";
        }
    }

    std::vector<ScoreEntry> loadScores()
    {
        std::vector<ScoreEntry> scores;
        std::ifstream file(filename);

        std::string name;
        int points;

        while (file >> name >> points)
        {
            scores.push_back({ name, points });
        }

        std::sort(scores.begin(), scores.end(),
            [](const ScoreEntry& a, const ScoreEntry& b)
            {
                return a.score > b.score;
            });

        return scores;
    }
};
