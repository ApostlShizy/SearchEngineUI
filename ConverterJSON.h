#pragma once

#include<fstream>
#include<exception>
#include<vector>
#include<map>
#include<string>
#include<iostream>

#include <iostream>

#include "SearchServer.h"
#include "nlohmann/json.hpp"

/**
*  Класс для работы с JSON-файлами
*/
class ConverterJSON {
private:
    size_t maxResponse = 1000;
public:

    ConverterJSON() = default;

    /* получить максимальное колиество ответов на запрос из файла config.json */
    size_t getMaxResponse() {
        return maxResponse;
    }

    /* Метод принимает вектор строк в который вписывает пути к документам 
    * вписывает в поле maxRespone максимальное количество ответов на один запрос
    * Возврщает int с кодом выполнения
    * configPath путь к файлу конфигурации
    */

    void setConfigPath(std::vector<std::string> & docsPath,
                    std::string configPath = "config.json",
                    std::string requestsPath = "requests.json",
                    std::string answerPath = "answer.json",
                    size_t maxResponses = 5) {

        std::cerr << configPath;

        nlohmann::json optionsJson;
        std::ofstream outConfigFile(configPath);

        optionsJson["config"]["name"] = "SearchEngineUi";
        optionsJson["config"]["version"] = "0.2";
        optionsJson["config"]["max_responses"] = maxResponses;
        optionsJson["requestsFile"] = requestsPath;
        optionsJson["answerFile"] = answerPath;

        for(auto & current : docsPath) {
            optionsJson["files"].push_back(current);
        }

        outConfigFile << optionsJson.dump(4);
    }


    int getConfig(std::vector<std::string>& filePath,
                  std::string & requestsPath,
                  std::string & answerPath,
                  const std::string configPath = "config.json") {

        std::ifstream configFile(configPath);
        nlohmann::json config_dir;

        /* Исключения */

        try {

            if (!configFile.is_open()) {
                throw std::exception();
            }

            configFile >> config_dir;

            if (config_dir.count("config") == 0) {
                throw std::invalid_argument("Config file is empty.");
            }

        }
        catch (const std::invalid_argument& x) {
            std::cerr << x.what() << std::endl;
            configFile.close();
            return 10;
        }
        catch (const std::exception& x) {
            std::cerr << "Config file is missing.\n" << x.what() << std::endl;
            return 11;
        }

        if(config_dir.count("requestsFile") == 0) {
            std::cerr << "not found requests.json";
            return 13;
        }

        configFile.close();

        /* Cоставление списка файлов*/

        std::vector<std::string> listOfFiles;

        {
            auto iter = config_dir.find("files");
            for (auto currentFile = iter->begin(); currentFile != iter->end(); ++currentFile) {
                listOfFiles.push_back(currentFile.value());
            }
        }

        {
            auto iter = config_dir.find("requestsFile");
            requestsPath = iter.value();
        }

        if(config_dir.count("answerFile") == 0) {
            std::cerr << "not found answer.json";
            answerPath = "answer.json";
        }
        else{
            auto iter = config_dir.find("answerFile");
            answerPath = iter.value();
        }

        maxResponse = config_dir.find("config").value().find("max_responses").value();

        if (maxResponse <= 0) {
            std::cerr << "\nNumber of responce <= 0" << std::endl;
            maxResponse = 5;
        }

        filePath = listOfFiles;

        return 0;
    }

    /* Чтение и составление списков запросов
     * requestsPath путь по которому лежит файл с запросами
    */
    std::map<size_t, std::vector<std::string>> getRequests(std::string requestsPath = "requests.json") {

        std::ifstream requestsFile(requestsPath);
        std::map<size_t, std::vector<std::string>> listOfRequests;

        if (!requestsFile.is_open()) {
            std::cerr << " \"requests.json\" not found." << std::endl;
            return listOfRequests;
        }

        nlohmann::json requestsDict;
        requestsFile >> requestsDict;

        if (requestsDict.find("requests").value().size() > 1000) {
            std::cerr << "\nNumbers of requests higther that 1000" << std::endl;
            requestsFile.close();
            return listOfRequests;
        }

        requestsFile.close();

        {
            auto requestsIter = requestsDict.find("requests");

            int idOfRequest = 0;

            for (auto current = requestsIter->begin(); current != requestsIter->end(); ++current) {
                /* принимает запрос целиком, разбивает на отдельные слова, помещает в вектор по указателю
                * Возвразает булевое значение false если количество слов в запросе более 10
                * Тогда такой запрос считается не валидным и не будет записан
                */
                auto vecFunct = [](std::string str, std::vector<std::string>* tempVec) {
                    std::string tempStr = "";
                    for (auto& currentChar : str) {
                        if (currentChar == ' ') {
                            tempVec->push_back(tempStr);
                            tempStr = "";
                        }
                        else {
                            tempStr += currentChar;
                        }
                    }

                    tempVec->push_back(tempStr);

                    if (tempVec->size() > 10) {
                        return false;
                    }

                    return true;
                    };

                std::vector<std::string> tempVector;

                if (vecFunct(current.value(), &tempVector)) {
                    listOfRequests[idOfRequest] = tempVector;
                }
                else {
                    std::cerr << "\nThe query contains more than 10 words id: " << idOfRequest << std::endl;
                }
                ++idOfRequest;
            }
        }

        return listOfRequests;
    }

    /* Положить в файл answers.json результаты поисковых запросов
     * answerPath - путь по которому сформируется ответ
     */
    void putAnswers(std::vector<std::vector<RelativeIndex>>& answerValue,std::string answerPath = "answers.json") {

        int i = 0;
        nlohmann::ordered_json validityJson;

        for (auto& currentAnswer : answerValue) {            
            int num = (i / 10) >= 1 ? num = (i / 100 >= 1 ? 0 : 1) : 2;
            std::string requestNumber = "request" + std::string(num, '0') + std::to_string(i);

            if (currentAnswer.empty()) {
                validityJson["answers"][requestNumber]["result"] = false;
            }
            else {
                int j = 0;

                validityJson["answers"][requestNumber]["result"] = true;

                for (const auto &[doc_id,rank] : currentAnswer) {

                    validityJson["answers"][requestNumber]["relevance"][j]["doc_id"] = doc_id;
                    validityJson["answers"][requestNumber]["relevance"][j]["rank"] = rank;
                    ++j;
                }
            }
            ++i;
        }

        std::ofstream outFileAnswers(answerPath);

        outFileAnswers << validityJson.dump(4);

        outFileAnswers.close();
    }
};
