#include "searchengine.h"

SearchEngine::SearchEngine() {
    searchServerObj = new SearchServer(invertedIndexObj);
}

void SearchEngine::engineFormAnswer() {

    /* Чтение из документов */

    std::vector<std::string> docs;

    for (auto& currentFilePath : vectorOfDocsPath) {

        std::ifstream inFileStream(currentFilePath);

        if (!inFileStream.is_open()) {
            std::cerr << "File not found on path: \"" << currentFilePath << "\"" << std::endl;
            docs.push_back("");
            continue;
        }

        std::string strFromFile = "";

        /* Чтение из документа всех слов */

        while (!inFileStream.eof()) {
            std::string tempFromFile = "";
            inFileStream >> tempFromFile;
            strFromFile += tempFromFile + " ";
        }

        /* Поиск слов и перевод в нижний регистр для корректного поиска */

        std::string temp = "";

        for (auto& currentChar : strFromFile) {
            if ((currentChar >= 'a' && currentChar <= 'z') || currentChar == ' ') {
                temp += currentChar;
            }
            else if (currentChar >= 'A' && currentChar <= 'Z') {
                temp += 'a' + (currentChar - 'A');
            }
            else if(enableNum && (currentChar >= '0' && currentChar <= '9')) {
                temp += currentChar;
            }
        }
        if (temp.length() > 0) {

            docs.push_back(temp);
        }
    }

    invertedIndexObj.updateDocumentBase(docs); /* Передать содержимое документов */

    {
        /* составить список уникальных слов из запросов */
        std::map<std::string, bool> uniqueWords;
        for (auto& currentRequest : listOfRequests) {
            for (auto& currentWord : currentRequest.second) {
                uniqueWords[currentWord] = true;
            }
        }

        for (auto& currentWord : uniqueWords) {
            /*  Обновить или создать индексированную библиотеку */
            std::vector<Entry> tempEntry = invertedIndexObj.getWordCount(currentWord.first);
            invertedIndexObj.setfreqDictionary(currentWord.first, tempEntry);
        }
    }

    answerValue = searchServerObj->search(listOfRequests,maxResponse);


    if(saveAnswer) {
        if(pathToAnswer.empty()){
            pathToAnswer = "answer.json";
        }
        converterJsonObj.putAnswers(answerValue, pathToAnswer);
    }
}

/* -------------------------------------- ↓↓↓ Set values ↓↓↓ -------------------------------------- */

void SearchEngine::setEnableNum(bool state) {
    enableNum = state;
}

void SearchEngine::setSaveAnswer(bool state) {
    saveAnswer = state;
}

void SearchEngine::setSaveRequests(bool state) {
    saveRequests = state;
}

void SearchEngine::setPathToToRequests(std::string path) {
    pathToRequests = path;
}

void SearchEngine::setMaxResponse(int inMaxResponse) {
    maxResponse = inMaxResponse;
}

void SearchEngine::setPathToAnswer(std::string path) {
    pathToAnswer = path;
}

/* -------------------------------------- ↑↑↑ Set values ↑↑↑ ---------------------------------------- */
/* ------------------------------------------------------------------------------------------------- */
/* -------------------------------------- ↓↓↓ Get values ↓↓↓ -------------------------------------- */

const std::vector<std::vector<RelativeIndex>> & SearchEngine::getAnswerValue() {
    return answerValue;
}
const std::map<size_t,std::vector<std::string>> & SearchEngine::getListOfRequests() {
    return listOfRequests;
}
const std::vector<std::string> & SearchEngine::getVectorOfDocsPath() {
    return vectorOfDocsPath;
}

std::map<size_t,std::vector<std::string>>::const_iterator SearchEngine::getOneRequests(size_t index) {
    if(listOfRequests.count(index) == 0){
        return listOfRequests.end();
    }
    else{
        return listOfRequests.find(index);
    }
}



std::vector<std::string>::const_iterator SearchEngine::getDocsPath(int index) {
    if(index >= 0 && index < vectorOfDocsPath.size()) {
        return vectorOfDocsPath.begin() + index;
    }
    return vectorOfDocsPath.end();
}

/* -------------------------------------- ↑↑↑ Get values ↑↑↑ ---------------------------------------- */
/* ------------------------------------------------------------------------------------------------- */
/* -------------------------------------- ↓↓↓ Config ↓↓↓ ------------------------------------------ */

void SearchEngine::loadOptions(std::string inPathToOptions) {

    vectorOfDocsPath.clear();
    pathToRequests.clear();
    pathToAnswer.clear();
    pathToOptions.clear();
    answerValue.clear();

    pathToOptions = inPathToOptions;

    converterJsonObj.getConfig(vectorOfDocsPath,pathToRequests,pathToAnswer,inPathToOptions);

    listOfRequests = converterJsonObj.getRequests(pathToRequests);
}

void SearchEngine::saveOptions(std::string inPathToOptions) {
    pathToOptions.clear();
    pathToOptions = inPathToOptions;
    converterJsonObj.setConfigPath(vectorOfDocsPath,inPathToOptions,pathToRequests,pathToAnswer,maxResponse);
}

/* -------------------------------------- ↑↑↑ Config ↑↑↑ -------------------------------------------- */
/* ------------------------------------------------------------------------------------------------- */
/* ------------------------------ ↓↓↓ Requests & Docs ↓↓↓ ----------------------------------------- */

void SearchEngine::addRequests(std::string requests){

    auto tempRequests = converterJsonObj.getRequests(requests);

    int idOfRequest = 0;

    for(auto & currentRequests : tempRequests) {

        for(int i = 0 + idOfRequest; i < 1000;++i) {
            if(listOfRequests.count(i) == 0) {
                idOfRequest = i;
                break;
            }
        }

        listOfRequests.insert(std::pair<size_t,std::vector<std::string>>(idOfRequest,currentRequests.second));
    }
}

int SearchEngine::addOneRequests(std::string requests) {

    int idOfRequest = 0;

    for(int i = 0; i < 1000;++i) {
        if(listOfRequests.count(i) == 0) {
            idOfRequest = i;
            break;
        }
    }

    std::vector<std::string> tempVec = breakRequestsStr(requests);

    listOfRequests.insert(std::pair<size_t,std::vector<std::string>>(idOfRequest,tempVec));

    return idOfRequest;
}

void SearchEngine::eraseRequests(int index) {
    listOfRequests.erase(index);
}

void SearchEngine::alterListOfRequests(std::string newRequestsStr) {

    int toPos = 0;
    std::string numStr = "";

    for(auto & currentChar : newRequestsStr){
        if(currentChar >= '0' && currentChar <= '9'){
            numStr += currentChar;
            ++toPos;
        }
        else break;
    }

    std::string copy = newRequestsStr;

    for(int i = 0; i < toPos; ++i) {
        copy[i] = ' ';
    }

    int index = std::stoi(numStr);

    if(listOfRequests.count(index) != 0){
        std::vector<std::string> tempVec = breakRequestsStr(copy);
        listOfRequests[index] = tempVec;
    }
}


void SearchEngine::addDocs(std::string path){
    vectorOfDocsPath.push_back(path);
}

bool SearchEngine::addOneDock(std::string path) {
    std::ifstream inFile(path);
    if(inFile.is_open()){
        vectorOfDocsPath.push_back(path);
        return true;
    }
    return false;
}

void SearchEngine::eraseDocs(int index) {
    if(index >= 0 && index < vectorOfDocsPath.size()) {
        vectorOfDocsPath.erase(vectorOfDocsPath.begin() + index);
    }
}

/* ------------------------------ ↑↑↑ Requests & Docs ↑↑↑ ----------------------------------------- */
/* ----------------------------------------------------------------------------------------------- */

SearchEngine::~SearchEngine() {
    if(searchServerObj != nullptr) {
        delete searchServerObj;
    }
}

std::vector<std::string> SearchEngine::breakRequestsStr(std::string requests) {

    std::vector<std::string> tempVec;
    std::string tempStr = "";

    for(auto & currentChar: requests ){
        if(currentChar == ' ' && !tempStr.empty()){
            tempVec.push_back(tempStr);
            tempStr.clear();
        }
        else if(currentChar != ' ') {
            tempStr += currentChar;
        }
    }

    tempVec.push_back(tempStr);

    for(auto & current : tempVec) {
        std::cerr << "Word is : " << current << std::endl;
    }

    return tempVec;
}
