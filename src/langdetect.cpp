#include <string>
#include <map>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <cstring>

#include "./langdetect.h"
#include "./ngram_storage.h"
#include "./code_sequence.h"

#define MATH_PI 3.1415926535897932384626433832795

using std::string;
using std::map;
using std::vector;

namespace langdetect {
    unsigned int const Detector::MAX_READ_SIZE = 12288;
    int const Detector::DEFAULT_TRIAL = 40;
    double const Detector::DEFAULT_ALPHA = 0.5;
    double const Detector::ALPHA_WIDTH = 0.05;
    int const Detector::ITERATION_LIMIT = 1000;
    double const Detector::PROB_THRESHOLD = 0.1;
    double const Detector::CONV_THRESHOLD = 0.99999;
    int const Detector::BASE_FREQ = 10000;
    int const Detector::WINDOW_SIZE = 100;
    std::string const Detector::UNKNOWN_LANG = "unknown";

    Detector::Detector() : trial_(DEFAULT_TRIAL), alpha_(DEFAULT_ALPHA) {}

    Detected Detector::detect(char const *data, unsigned int const &length) {
        if(length == 0) {
            return Detected("empty", 0.0);
        }
        size_t rlen = length;
        if(rlen > MAX_READ_SIZE) rlen = MAX_READ_SIZE;
        CodeSequence codesequence(data, rlen);
        vector<string> grams = codesequence.tongram();
        vector<Detected> detecteds = detect_(grams);
        std::sort(detecteds.begin(), detecteds.end());
        return detecteds.back();
    }

    std::vector<Detected> Detector::detect_multiple(char const *data, unsigned int const &length) {
        vector<Detected> all_windows;
        if(length == 0) {
            all_windows.push_back(Detected("empty", 0.0));
            return all_windows;
        }
        CodeSequence codesequence(data, length);
        unsigned int start = 0, end = 0, num_windows = 0;
        std::vector<std::vector<Detected>> windows;
        while ( start < codesequence.size() ) {
            ++num_windows;
            end = start + WINDOW_SIZE;
            if (end > codesequence.size() ) end = codesequence.size();
            vector<string> ngrams = codesequence.range_to_ngram(start, end);
            windows.push_back(detect_(ngrams));
            start = end;
        }

        for (auto window : windows) {
            if (all_windows.size() == 0) {
                all_windows = window;
            } else {
                for (unsigned i = 0; i < window.size(); ++i) {
                    all_windows[i].score( window[i].score() + all_windows[i].score() );
                }
            }
        }

        for (auto &lang : all_windows ) {
            lang.score(lang.score() / (double) num_windows);
        }

        std::sort(all_windows.begin(), all_windows.end());
        return all_windows;
    }

    std::vector<Detected> Detector::detect_(vector<string> &grams) {
        vector<Detected> detecteds;
        if(grams.empty()) {
            detecteds.push_back(Detected(UNKNOWN_LANG, 0.0));
            return detecteds;
        }
        NgramStorage& storage = NgramStorage::instance();
        vector<double> finalscores(storage.langsize(), 0);
        for(int i = 0; i < trial_; ++i) {
            vector<double> scores(storage.langsize(), 1.0 / storage.langsize());  // priorはuniformに固定する
            double alpha = alpha_ + random_gaussian_() * ALPHA_WIDTH;
            for(int j = 0;;) {
                // 乱数からどのn-gramを使うかを決定する
                size_t tgt_idx = random_range_(0, grams.size() - 1);
                string ngram = grams[tgt_idx];
                if(ngram.empty()) continue;
                update_scores_(ngram, scores, alpha);
                if( j % 5 == 0) {
                    if(normalize_(scores) > CONV_THRESHOLD || j >= ITERATION_LIMIT) break;
                }
                j += 1; // count up
            }
            for(size_t j = 0; j < finalscores.size(); ++j) {
                finalscores[j] += scores[j] / trial_;
            }
        }

        detecteds.resize(storage.langsize());
        for(size_t i = 0; i < finalscores.size(); ++i) {
            detecteds[i].name(storage.lang_fromindex(i));
            detecteds[i].score(finalscores[i]);
        }
        return detecteds;
    }

    void Detector::update_scores_(string const &ngram, vector<double> &scores, double const &alpha) {
        NgramStorage& storage = NgramStorage::instance();
        if(storage.has(ngram)) {
            double weight = alpha / BASE_FREQ;
            auto plist = storage.get(ngram);
            
            for(size_t i = 0; i < plist.size(); ++i) {
                scores[i] *= weight + plist[i];
            }
        }
    }

    double Detector::normalize_(vector<double> &scores) {
        double total = 0.0;
        double maxp = 0.0;
        for(size_t i = 0; i < scores.size(); ++i) total += scores[i];
        for(size_t i = 0; i < scores.size(); ++i) {
            double norm = scores[i] / total;
            if(maxp < norm) maxp = norm;
            scores[i] = norm;
        }
        return maxp;
    }

    double Detector::random_uniform_() {
        return (double)(std::rand()) / RAND_MAX;
    }

    double Detector::random_gaussian_() {
       double alpha = random_uniform_();
       while(alpha == 0) alpha = random_uniform_();
       double beta = random_uniform_();
       return std::sqrt(- 2 * std::log(alpha)) * std::sin(2 * MATH_PI * beta);
    }

    int Detector::random_range_(int const &minnum, int const &maxnum) {
        if(minnum > maxnum) throw std::runtime_error("min exceeds maximum");
        return static_cast<int>(std::floor(minnum + random_uniform_() * (maxnum - minnum)));
    }

    Detected::Detected(string const &name, double const &score)
    : name_(name), score_(score) {}

    Detected::Detected() {};

    bool Detected::operator==(Detected const &other) const {
        return score_ == other.score_;
    }
    bool Detected::operator!=(Detected const &other) const {
        return score_ != other.score_;
    }
    bool Detected::operator<(Detected const &other) const {
        return score_ < other.score_;
    }
    bool Detected::operator>(Detected const &other) const {
        return score_ > other.score_;
    }
    bool Detected::operator<=(Detected const &other) const {
        return score_ <= other.score_;
    }
    bool Detected::operator>=(Detected const &other) const {
        return score_ >= other.score_;
    }

    void Detected::name(std::string const &name) {name_ = name;}
    std::string const & Detected::name() const {return name_;}
    void Detected::score(double const &score) {score_ = score;}
    double const & Detected::score() const {return score_;}
}

#include "./langdetect_c.h"

void langdetect_detect(char const *data, unsigned int const length, char lang[6]) {
    double score = 0.0;
    langdetect_detect_with_score(data, length, lang, &score);
}

void langdetect_detect_with_score(char const *data, unsigned int const length, char lang[6], double *score) {
    langdetect::Detector detector;
    try {
        langdetect::Detected result = detector.detect(data, length);
        *score = result.score();
        memcpy(lang, result.name().c_str(), 
               result.name().length() + 1);
    } catch(std::runtime_error const &e) {
        memcpy(lang, "null", 4);
        lang[5] = '\0';
    }
}
