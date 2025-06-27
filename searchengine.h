#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <vector>
#include <string>

#include "SearchServer.h"
#include "ConverterJSON.h"
#include "InvertedIndex.h"

class SearchEngine {

private:

    bool saveAnswer = true;
    bool saveRequests = true;

    bool enableNum = false;

    size_t maxResponse = 5;

    std::string pathToRequests;
    std::string pathToOptions;
    std::string pathToAnswer;

    std::vector<std::string> vectorOfDocsPath;

    std::map<size_t,std::vector<std::string>> listOfRequests;

    std::vector<std::vector<RelativeIndex>> answerValue;

public:

    SearchEngine();
    ~SearchEngine();

    SearchServer * searchServerObj = nullptr;
    InvertedIndex invertedIndexObj;
    ConverterJSON converterJsonObj;

    std::vector<std::string> breakRequestsStr(std::string requests);

    void setEnableNum(bool state);
    void setSaveAnswer(bool state);
    void setSaveRequests(bool sate);
    void setPathToAnswer(std::string path);
    void setPathToToRequests(std::string path);
    void setMaxResponse(int inMaxResponse);

    void loadOptions(std::string inPathToOptions);
    void saveOptions(std::string inPathToOptions);

    void addDocs(std::string path);
    bool addOneDock(std::string path);
    void addRequests(std::string requests);
    int addOneRequests(std::string requests);

    void eraseDocs(int index);
    void eraseRequests(int index);

    void alterListOfRequests(std::string newRequestsStr);

    void engineFormAnswer();

    const std::vector<std::vector<RelativeIndex>> & getAnswerValue();
    const std::map<size_t,std::vector<std::string>> & getListOfRequests();
    const std::vector<std::string> & getVectorOfDocsPath();

    std::map<size_t,std::vector<std::string>>::const_iterator getOneRequests(size_t index);

    std::vector<std::string>::const_iterator getDocsPath(int index);
};

#endif // SEARCHENGINE_H
