#pragma once
#include <vector>
#include "Blok.h"

using namespace std;

class Level {
private:
    vector<Blok> bloki;

public:
    Level() {}

    // Tworzymy siatkê 7 kolumn × 5 wierszy
    void generate(float screenW, float screenH)
    {
        bloki.clear();

        const int COLS = 3;
        const int ROWS = 3;
        const float BLOCK_W = 72;
        const float BLOCK_H = 30;
        const float MARGIN = 2.5;

        float totalWidth = COLS * BLOCK_W + (COLS - 1) * MARGIN;
        float startX = (screenW - totalWidth) / 2.f;
        float startY = 100;  // odsuniêcie od góry

        for (int r = 0; r < ROWS; r++)
        {
            for (int c = 0; c < COLS; c++)
            {
                float x = startX + c * (BLOCK_W + MARGIN);
                float y = startY + r * (BLOCK_H + MARGIN);

                bloki.emplace_back(x, y);
            }
        }
    }

    std::vector<Blok>& getBloki() { return bloki; }

    void draw(RenderTarget& target)
    {
        for (auto& b : bloki)
            b.draw(target);
    }

    const vector<Blok>& getBloki() const {
        return bloki;
    }
};
