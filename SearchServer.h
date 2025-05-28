#pragma once

#include<map>
#include<vector>
#include<string>

#include "InvertedIndex.h"

struct RelativeIndex{
    size_t doc_id;
    float rank;

    bool operator ==(const RelativeIndex& other) const {
    return (doc_id == other.doc_id && rank == other.rank);
    }
};


/* Класс SearchServer составляет релеватные списки */
    class SearchServer {

    private:

        InvertedIndex & invIndex;

    public:

    /* в конструктор класса передаётся ссылка на класс InvertedIndex,
    *  чтобы SearchServer мог узнать частоту слов встречаемых в запросе */
    SearchServer(InvertedIndex& idx) : invIndex(idx){ };

    /* Метод обработки поисковых запросов
    * currentRequest поисковые запросы взятые из файла requests.json
    * maxResponses количество ответов на запрос взятое из config.json
    */
    /* Метод обработки поисковых запросов
    * request поисковые запросы взятые из файла requests.json
    * maxResponses количество ответов на запрос взятое из config.json
    */
    std::vector<std::vector<RelativeIndex>> search(const std::map<size_t, std::vector<std::string>>& request,size_t maxResponse = 1000) {

        std::vector<std::vector<RelativeIndex>> result;

        for (auto& currentRequst : request) {

            size_t abs = 0;
            std::map<size_t, float> rankOfValidity;
            std::map<size_t, size_t> relative;

            for (auto& currentWordRequest : currentRequst.second) {

                if (invIndex.getDictionary().count(currentWordRequest) != 0) {
                   
                    auto iter = invIndex.getDictionary().find(currentWordRequest);

                    for (auto iterVectorEnrty = iter->second.cbegin();
                        iterVectorEnrty != iter->second.cend();
                        ++iterVectorEnrty) {

                        relative[iterVectorEnrty->doc_id] += iterVectorEnrty->count;
                    } 
                }
            }

            for (auto& currentRelative : relative) {
                if (currentRelative.second > abs) {
                    abs = currentRelative.second;
                }
            }

            for (auto& currentRelative : relative) {                
                rankOfValidity[currentRelative.first] = (float)currentRelative.second / (float)abs;
            }

            std::vector<RelativeIndex> interResult;

            std::multimap<float, size_t> sortedValidity;

            for (auto& currentRank : rankOfValidity) {
                sortedValidity.insert(std::pair<float, size_t>(currentRank.second, currentRank.first));
            }    

            size_t copyMaxResponse = maxResponse;

            for (auto iterRank = sortedValidity.rbegin();
                copyMaxResponse > 0 && iterRank != sortedValidity.rend();
                ++iterRank, --copyMaxResponse) {
                interResult.push_back(RelativeIndex{ iterRank->second,iterRank->first });
            }

            result.push_back(interResult);

         }
        return result;
    }
};
   