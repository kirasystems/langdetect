#pragma once
#include <string>
#include <vector>

namespace langdetect {

/// 言語判定に使う為の文字列配列を表現するコンテナクラス
/// 内部で正規化され、Unicodeコードポイントの配列になる
/// UTF-8のみに対応
class CodeSequence {
    public:
        CodeSequence(char const *data, size_t const &length);

        // The length of the sequence.
        size_t size();

        /// ngramバイト列の配列を返す。
        /// バイト列は4byteコードポイント（リトルエンディアン）の列になっている
        std::vector<std::string> tongram();

        // Convert only a range to ngrams.
        std::vector<std::string> range_to_ngram(size_t start, size_t end);



    private:
        std::vector<uint32_t> codes_;

        /// 余分な文字列を除去する。その上で、latin, non_latinの判断をしてどちらかに振る
        /// 正規表現を使わない程度にURL, MAILの判断をする
        void cleanchar_(std::string &data);

        /// bufferの位置cursorから一文字読み込み、読み込んだコードポイントを返す
        /// cursorは一文字分進められる
        uint32_t readchar_(std::string const &buffer, size_t &cursor);

        /// ASCII系か非ASCII系かをざっくり分ける
        void rough_judge_();
};

}
