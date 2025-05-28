#pragma once

#include <sstream>
#include<string>
#include<vector>
#include<map>
#include<mutex>
#include<thread>
#include<iostream>

struct Entry {
    size_t doc_id, count;

    // Данный оператор необходим для проведения тестовых сценариев
    bool operator == (const Entry& other) const {
    return (doc_id == other.doc_id && count == other.count);
    }
};

/* Класс InvertedIndex будет принимать текстовые блоки и формировать из них
инвертированный индекс */
    class InvertedIndex {
    private:

        std::mutex freqDictionaryMutex;

        std::mutex updateDocsMutex;

        std::vector<std::string> docs; /* список содержимого документов */

        std::map<std::string, std::vector<Entry>> freqDictionary; /* частотный словарь */

        /* Метод передаётся в поток для обновления списка содержимого документов*/
        void updateDocumentBaseThread(std::string filePath, size_t index) {
            {
                std::lock_guard<std::mutex> guard(updateDocsMutex);
                docs[index] = filePath;
            }
        }

        /* Метод передаётся в поток для формирования частотного словаря */
        void getWordCountThread(Entry * entryPtr,const std::string word,size_t index) { 

            size_t count = 0;

            std::stringstream strStream;
            std::string tempStr;

            strStream << docs[index];

            while (strStream >> tempStr) {
                if (tempStr == word) {
                    count += 1;
                }
            }

            
            std::lock_guard<std::mutex> entryGuard(freqDictionaryMutex);
            entryPtr->doc_id = index;
            entryPtr->count = count;
            
        }


    public:

        /* Обновить частотный словарь */
        void setfreqDictionary(const std::string & word, std::vector<Entry> & wordCount) {
            if (freqDictionary.count(word) != 0) {
                freqDictionary.erase(word);
            }
            freqDictionary.insert(std::pair<std::string, std::vector<Entry>>(word, wordCount));
        }

        const std::vector<std::string>& getDoc() {
            return docs;
        }

    InvertedIndex() = default;

    /* Обновить или заполнить базу документов, по которой будем совершать поиск
    * inputDocs список путей документов  */        
    void updateDocumentBase(const std::vector<std::string>& inputDocs) {

        std::vector<std::thread> threadDocs;

        for (int i = 0; i < inputDocs.size(); ++i) {
            docs.push_back(std::string{});
        }

        {
            size_t index = 0;
            for (auto& currentDoc : inputDocs) {
                threadDocs.push_back(std::thread(&InvertedIndex::updateDocumentBaseThread, this, currentDoc, index));
                ++index;
            }
        }

        for (int i = 0; i < inputDocs.size();) {
            if (threadDocs[i].joinable()) {
                threadDocs[i].join();
                ++i;
            }
        }
    }
    
    /* Возвращает ссылку на частотный словарь */
    const std::map<std::string, std::vector<Entry>> & getDictionary() {
        return freqDictionary;
    }

    /* Метод определяет количество вхождений слова в загруженной базе документов */
    std::vector<Entry> getWordCount(const std::string& word) {

        size_t const size = docs.size();

        if (size == 0) {
            throw std::ios_base::failure("data base empty please update calling \"updateDocumentBase\".");
        }

        std::vector<Entry> tempEntry;

        std::vector<Entry> entryForThread;

        for (size_t i = 0; i < size; ++i) {
            entryForThread.push_back({});
        }

        std::vector<std::thread> threadIndex;

        for (size_t index = 0; index < size; ++index) {
            threadIndex.push_back(std::thread(&InvertedIndex::getWordCountThread, this, &entryForThread[index], word, index));
        }

        for (size_t i = 0; i < size;) {
            if (threadIndex[i].joinable()) {
                threadIndex[i].join();
                ++i;
            }
        }

        for (size_t i = 0; i < size; ++i) {
            if (entryForThread[i].count != 0) {
                tempEntry.push_back(entryForThread[i]);
            }
        }

        return tempEntry;
    }
 };