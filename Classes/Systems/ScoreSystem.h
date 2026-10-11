#pragma once

namespace aw {

class ScoreSystem
{
public:
    ScoreSystem();

    int score() const { return _score; }
    int best() const { return _best; }

    void load();
    void save();
    void add(int value);
    void reset();
    bool isNewBest() const { return _newBest; }

private:
    static const char* kBestKey;
    int _score = 0;
    int _best = 0;
    bool _newBest = false;
};

} // namespace aw
