#include "optionwindow.h"
#include "ui_optionwindow.h"

#include <QRegularExpression>

#include <QFileDialog>

OptionWindow::OptionWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OptionWindow)
{
    ui->setupUi(this);

    /* ----------------------------------------------------------------------------------------------------- */
    /* -------------------------------------- ↓↓↓ Вкладка Answer конструктор ↓↓↓ -------------------------------------- */

    connect(ui->sortFromToButton,&QPushButton::clicked,[this]() {
        if(sortFromToAnswer) {
            ui->sortFromToButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::GoDown));
            sortFromToAnswer = false;
        }
        else {
            ui->sortFromToButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::GoUp));
            sortFromToAnswer = true;
        }
        emit this->stateChangedSignal();
    });

    connect(this,&OptionWindow::stateChangedSignal,this,&OptionWindow::stateChangedSlot);

    connect(ui->formAnswerButton,&QPushButton::clicked,this,&OptionWindow::formAnswerSlot);

    connect(ui->answerSearchLineEdit,&QLineEdit::textChanged,this,&OptionWindow::setStrAnswerSlot);

    connect(ui->sortAnswerComboBox,&QComboBox::currentIndexChanged,[this] () {
        ui->answerSearchLineEdit->clear();

        ui->answerTextBrowser->clear();

        sortAnswer();
    });

    /* -------------------------------------- ↑↑↑ Вкладка Answer конструктор ↑↑↑ --------------------------------------- */
    /* ---------------------------------------------------------------------------------------------------- */
    /* -------------------------------------- ↓↓↓ Вкладка Options конструктор ↓↓↓ ------------------------------------- */

    connect(ui->answPathLineEdit,&QLineEdit::textChanged,[this](){
        ui->answPathLineEdit->setStyleSheet(setBlack());
    });

    connect(ui->rqstPathLineEdit,&QLineEdit::textChanged,[this](){
        ui->rqstPathLineEdit->setStyleSheet(setBlack());
    });

    connect(ui->answPathLineEdit,&QLineEdit::returnPressed,[this](){
        srcEngine.setPathToAnswer(ui->answPathLineEdit->text().toStdString());
        ui->answPathLineEdit->setStyleSheet(setGreen());
    });

    connect(ui->rqstPathLineEdit,&QLineEdit::returnPressed,[this](){
        srcEngine.setPathToToRequests(ui->rqstPathLineEdit->text().toStdString());
        ui->rqstPathLineEdit->setStyleSheet(setGreen());
    });


    /* ----------------------------------------------------- */
    /* ↓↓↓ Файловый диалог для пути сохранений ↓↓↓ */

    connect(ui->specifyPathAnswerButton,&QPushButton::clicked,[this]{
        QString pathToAnswer = QFileDialog::getSaveFileName(nullptr,"Save Answer Path","/opt/", "json file (*.json");
        if(!pathToAnswer.isEmpty()){
            ui->answPathLineEdit->setText(pathToAnswer);
            ui->answPathLineEdit->setStyleSheet(setGreen());
            srcEngine.setPathToAnswer(pathToAnswer.toStdString());
        }
    });

    connect(ui->specifyPathRequestsButton,&QPushButton::clicked,[this]{
        QString pathToRequests = QFileDialog::getSaveFileName(nullptr,"Save Requests Path","/opt/", "json file (*.json");
        if(!pathToRequests.isEmpty()){
            ui->answPathLineEdit->setText(pathToRequests);
            ui->answPathLineEdit->setStyleSheet(setGreen());
            srcEngine.setPathToAnswer(pathToRequests.toStdString());
        }
    });

    /* ↑↑↑ Файловый диалог для пути сохранений ↑↑↑ */
    /* ----------------------------------------------------- */
    /* ↓↓↓ Файловый диалог для пресета ↓↓↓ */

    connect(ui->savePrsButton,&QPushButton::clicked,[this](){
        QString savedPrestPath = QFileDialog::getSaveFileName(nullptr,"Save OptionsPreset","/opt/", "json file (*.json");
        if(!savedPrestPath.isEmpty()){
            ui->currentPresetLabel->setText(savedPrestPath);
            srcEngine.saveOptions(savedPrestPath.toStdString());
        }
    });

    connect(ui->loadPrsButton,&QPushButton::clicked,[this](){
        QString loadedPresetPath = QFileDialog::getOpenFileName(nullptr,"Load OptionsPreset","/opt/", "json file (*.json)");
        if(!loadedPresetPath.isEmpty()) {

            ui->requestsTextBrowser->clear();            
            ui->answerTextBrowser->clear();

            ui->currentPresetLabel->setText(loadedPresetPath);
            srcEngine.loadOptions(loadedPresetPath.toStdString());

            for(auto & currentRequests : srcEngine.getListOfRequests()){
                updateRequestsTextBrowser(currentRequests);
            }

            updateDocsTextBrowser();
        }
    });

    /* ↑↑↑ Файловый диалог для пресета ↑↑↑ */
    /* ----------------------------------------------------- */

    connect(ui->saveAnswCheckBox,&QCheckBox::stateChanged,[this](){
        if(ui->saveAnswCheckBox->checkState() == Qt::CheckState::Checked) {
            ui->answPathLineEdit->setEnabled(true);
        }
        else {
            ui->answPathLineEdit->setEnabled(false);
        }
    });

    connect(ui->saveRqstCheckBox,&QCheckBox::stateChanged,[this](){
        if(ui->saveRqstCheckBox->checkState() == Qt::CheckState::Checked) {
            ui->rqstPathLineEdit->setEnabled(true);
        }
        else {
            ui->rqstPathLineEdit->setEnabled(false);
        }
    });

    connect(ui->enableNumCheckBox,&QCheckBox::stateChanged,[this](){
        if(ui->enableNumCheckBox->checkState() == Qt::CheckState::Checked){
            srcEngine.setEnableNum(true);
        }
        else {
            srcEngine.setEnableNum(false);
        }
    });

    /* -------------------------------------- ↑↑↑ Вкладка Options конструктор ↑↑↑ -------------------------------------- */
    /* ---------------------------------------------------------------------------------------------------------------- */
    /* -------------------------------------- ↓↓↓ Вкладка Requests конструктор ↓↓↓ ----------------------------------- */

    connect(ui->requestsSearchLineEdit,&QLineEdit::textChanged,[this](QString text){
        if(!text.isEmpty() && (ui->searchRequestRadioButton->isChecked() || ui->alterRequestRadioButton->isChecked()) ){

            QString temp = ui->requestsSearchLineEdit->text();

            QRegularExpression exp;

            if(ui->alterRequestRadioButton->isChecked()) {
                exp = QRegularExpression("^\\d{1,3}(\\s\\w*||\\d*){1,10}+$");
            }
            else{
                exp = QRegularExpression("^\\d{1,3}$");
            }

            ui->requestsTextBrowser->clear();

            if(exp.match(temp).hasMatch()){

                QString numStr = "";

                for(auto & currentChar : temp){
                    if(currentChar >= '0' && currentChar <= '9'){
                        numStr += currentChar;
                    }
                    else break;
                }

                auto iter = srcEngine.getOneRequests(numStr.toInt());

                if(iter != srcEngine.getListOfRequests().end()){
                    updateRequestsTextBrowser(*iter);
                }
            }

            for(auto & currentRequests : srcEngine.getListOfRequests()){
                bool found = false;

                for(auto & currentWord : currentRequests.second) {
                    QString tempWord = currentWord.c_str();
                    if(tempWord.contains(text)){
                        found = true;
                        break;
                    }
                }

                if(found) {
                    updateRequestsTextBrowser(currentRequests);
                }
            }
        }
        else {
            ui->requestsTextBrowser->clear();
            for(auto & currentRequests : srcEngine.getListOfRequests()) {
                updateRequestsTextBrowser(currentRequests);
            }
        }

    });

    connect(ui->requestsSearchLineEdit,&QLineEdit::returnPressed,[this]{        
        if(ui->addRequestRadioButton->isChecked()){

            int index = srcEngine.addOneRequests(ui->requestsSearchLineEdit->text().toStdString());

            auto iter = srcEngine.getOneRequests(index);

            updateRequestsTextBrowser(*iter);
        }
        else if(ui->alterRequestRadioButton->isChecked()){
            srcEngine.alterListOfRequests(ui->requestsSearchLineEdit->text().toStdString());
            ui->requestsTextBrowser->clear();
            for(auto & currentRequests : srcEngine.getListOfRequests()){
                updateRequestsTextBrowser(currentRequests);
            }
        }
        else {
            QString temp = ui->requestsSearchLineEdit->text();

            bool itsIndex = true;

            for(auto & current : temp) {
                if(current < '0' || current > '9') {
                    itsIndex = false;
                    break;
                }
            }

            if(itsIndex){
                srcEngine.eraseRequests(temp.toInt());

                ui->requestsTextBrowser->clear();

                for(auto & currentRequests : srcEngine.getListOfRequests()) {
                    updateRequestsTextBrowser(currentRequests);
                }
            }
        }
    });

    connect(ui->loadRequestPushButton,&QPushButton::clicked,[this](){
        QList<QString> loadedRequestsPaths = QFileDialog::getOpenFileNames(nullptr,"add requests","/opt/", "json file (*.json)");

        for(auto & current : loadedRequestsPaths){
            srcEngine.addRequests(current.toStdString());
        }

        ui->requestsTextBrowser->clear();

        for(auto & currentRequests : srcEngine.getListOfRequests()){
            updateRequestsTextBrowser(currentRequests);
        }

    });

    /* -------------------------------------- ↑↑↑ Вкладка Requests конструктор ↑↑↑ --------------------------------------- */
    /* ------------------------------------------------------------------------------------------------------------------ */
    /* -------------------------------------- ↓↓↓ Вкладка Docs Paths конструктор ↓↓↓ ----------------------------------- */

    connect(ui->loadDocsPathPushButton,&QPushButton::clicked,[this](){
        QList<QString> loadedDocsPaths = QFileDialog::getOpenFileNames(nullptr,"add docs","/opt/", "txt file (*.txt)");

        for(auto & current : loadedDocsPaths) {
            srcEngine.addDocs(current.toStdString());
        }

        updateDocsTextBrowser();

    });

    connect(ui->docsPathsSearchLineEdit,&QLineEdit::returnPressed,[this](){
        if(ui->deleteDocsRadioButton->isChecked()){

            srcEngine.eraseDocs(ui->docsPathsSearchLineEdit->text().toInt());

            ui->docsPathsTextBrowser->clear();

            size_t i = 0;

            for(auto & currentDocs : srcEngine.getVectorOfDocsPath()){
                QString temp = "doc_id: " + QString::number(i) + "  " + currentDocs.c_str();
                ui->docsPathsTextBrowser->append(temp);
                ++i;
            }
        }
        else if(ui->addDocsRadioButton->isChecked()){
            QString temp = ui->docsPathsSearchLineEdit->text();
            if(srcEngine.addOneDock(temp.toStdString())){
                QString result = "doc_id: " + QString::number(srcEngine.getVectorOfDocsPath().size() - 1) + "  " + temp;
                ui->docsPathsTextBrowser->append(temp);
            }
        }
        else {

            QString temp = ui->docsPathsSearchLineEdit->text();

            bool itsIndex = true;

            for(auto & current : temp) {
                if(current < '0' || current > '9') {
                    itsIndex = false;
                    break;
                }
            }

            if(itsIndex){
                ui->docsPathsTextBrowser->clear();

                if(srcEngine.getDocsPath(temp.toInt()) != srcEngine.getVectorOfDocsPath().end()){
                    QString dockPath = srcEngine.getDocsPath(temp.toInt())->c_str();
                    QString result = "doc_id: " + temp + "  " + dockPath;

                    ui->docsPathsTextBrowser->append(result);

                }
                else {
                    ui->docsPathsTextBrowser->append(QString ("Nothing on \"" + temp + "\""));
                    itsIndex = false;
                }

            }

            if(!itsIndex) {
                ui->docsPathsTextBrowser->clear();
                int i = 0;

                for(auto & currentDock : srcEngine.getVectorOfDocsPath()) {
                    QString currentStr = currentDock.c_str();

                    if(currentStr.contains(temp)){
                        QString result = "doc_id: " + QString::number(i) + "  " + currentStr;
                        ui->docsPathsTextBrowser->append(result);
                    }
                    ++i;
                }
            }
        }
    });
    /* -------------------------------------- ↑↑↑ Вкладка Docs Paths конструктор ↑↑↑ ----------------------------------- */
    /* ---------------------------------------------------------------------------------------------------------------- */
}
/* ----------------------------------------------------------------------------------------------------------------- */
/* -------------------------------------- ↓↓↓ Вкладка Options определение ↓↓↓ ------------------------------------- */

QString OptionWindow::setGreen() {
    return QString("QLineEdit{color: green;}"
                   "QLineEdit::!focus{border-bottom-right-radius: 5px;border-top-right-radius: 5px;border: 1px solid #C2C7CB;}"
                   "QLineEdit::focus{border-bottom-right-radius: 5px;border-top-right-radius: 5px;border: 1px solid rgb(0, 170, 255);}");
}

QString OptionWindow::setBlack() {
    return QString("QLineEdit{color: black;}"
                   "QLineEdit::!focus{border-bottom-right-radius: 5px;border-top-right-radius: 5px;border: 1px solid #C2C7CB;}"
                   "QLineEdit::focus{border-bottom-right-radius: 5px;border-top-right-radius: 5px;border: 1px solid rgb(0, 170, 255);}");
}

/* -------------------------------------- ↑↑↑ Вкладка Options определение ↑↑↑ ----------------------------------------- */
/* ------------------------------------------------------------------------------------------------------------------- */
/* -------------------------------------- ↓↓↓ Вкладка Answer определение ↓↓↓ ---------------------------------------- */

void OptionWindow::stateChangedSlot() {
    sortAnswer();
}

void OptionWindow::setStrAnswerSlot(QString text){
    this->strAnswerSearch = text;

    emit this->stateChangedSignal();
}

void OptionWindow::formAnswerSlot() {
    srcEngine.engineFormAnswer();
    sortAnswer();
}

void OptionWindow::sortAnswer() {

    ui->answerTextBrowser->clear();

    if(sortFromToAnswer){

        int i = 0;

        auto currentAnswer = srcEngine.getAnswerValue();

        for(auto & currentAnswer : srcEngine.getAnswerValue()){

            int num = (i / 10) >= 1 ? num = (i / 100 >= 1 ? 0 : 1) : 2;

            QString requestNumber = "request" + QString(num,'0') + QString::number(i) + ' ';

            if(ui->sortAnswerComboBox->currentIndex() == 3 && !strAnswerSearch.isEmpty() && strAnswerSearch.toInt() != i) {
                ++i;
                continue;
            }

            if(ui->sortAnswerComboBox->currentIndex() == 2 && !strAnswerSearch.isEmpty() && (strAnswerSearch.toInt() !=  currentAnswer.size() || currentAnswer.size() == 0)) {
                continue;
            }

            if(currentAnswer.empty()) {
                ui->answerTextBrowser->append(requestNumber + QString("- result is \"false\" \n"));
                ui->answerTextBrowser->append(QString(100,'-'));
            }

            else {

                ui->answerTextBrowser->append(requestNumber + QString("- result is \"true\" \n"));
                for(auto & currentRelative : currentAnswer) {

                    QString doc_id = "doc_id: " + QString::number(currentRelative.doc_id);

                    if(ui->sortAnswerComboBox->currentIndex() == 1 && !strAnswerSearch.isEmpty() && strAnswerSearch.toInt() != currentRelative.doc_id) {
                        continue;
                    }

                    QString rank = "rank: " + QString::number(currentRelative.rank);

                    if(ui->sortAnswerComboBox->currentIndex() == 0 && !strAnswerSearch.isEmpty()) {

                        bool isRange = strAnswerSearch.contains('-');

                        if(isRange) {
                            float lowNum, hightNum;

                            QString temp;

                            bool found = false;

                            for(auto & currentChar : strAnswerSearch) {
                                if(currentChar == '-'){
                                    found = true;
                                    lowNum = temp.toFloat();
                                    temp.clear();
                                    continue;
                                }
                                else if(!found) {
                                    temp += currentChar;
                                }
                                else {
                                    temp += currentChar;
                                }
                            }

                            hightNum = temp.toFloat();

                            if(currentRelative.rank < lowNum || currentRelative.rank > hightNum) {
                                continue;
                            }
                        }
                        else if (strAnswerSearch.toFloat() != currentRelative.rank) {
                            continue;
                        }
                    }

                    ui->answerTextBrowser->append(doc_id + "     " + rank);
                }
                ui->answerTextBrowser->append(QString(100,'-'));
            }
            ++i;
        }
    }
    else {

        int i = srcEngine.getAnswerValue().size()-1;

        for(auto iter = srcEngine.getAnswerValue().rbegin(); iter != srcEngine.getAnswerValue().rend();++iter){

            int num = (i / 10) >= 1 ? num = (i / 100 >= 1 ? 0 : 1) : 2;

            QString requestNumber = "request" + QString(num,'0') + QString::number(i) + ' ';

            if(ui->sortAnswerComboBox->currentIndex() == 3 && !strAnswerSearch.isEmpty() && strAnswerSearch.toInt() != i) {
                ++i;
                continue;
            }

            if(ui->sortAnswerComboBox->currentIndex() == 2 && !strAnswerSearch.isEmpty() && (strAnswerSearch.toInt() !=  iter->size() || iter->size() == 0)) {
                continue;
            }

            if(iter->empty()) {
                ui->answerTextBrowser->append(requestNumber + QString("- result is \"false\" \n"));
                ui->answerTextBrowser->append(QString(100,'-'));
            }

            else {

                ui->answerTextBrowser->append(requestNumber + QString("- result is \"true\" \n"));
                for(auto & currentRelative : *iter) {

                    QString doc_id = "doc_id: " + QString::number(currentRelative.doc_id);

                    if(ui->sortAnswerComboBox->currentIndex() == 1 && !strAnswerSearch.isEmpty() && strAnswerSearch.toInt() != currentRelative.doc_id) {
                        continue;
                    }

                    QString rank = "rank: " + QString::number(currentRelative.rank);

                    if(ui->sortAnswerComboBox->currentIndex() == 0 && !strAnswerSearch.isEmpty()) {

                        bool isRange = strAnswerSearch.contains('-');

                        if(isRange) {
                            float lowNum, hightNum;

                            QString temp;

                            bool found = false;

                            for(auto & currentChar : strAnswerSearch) {
                                if(currentChar == '-'){
                                    found = true;
                                    lowNum = temp.toFloat();
                                    temp.clear();
                                    continue;
                                }
                                else if(!found) {
                                    temp += currentChar;
                                }
                                else {
                                    temp += currentChar;
                                }
                            }

                            hightNum = temp.toFloat();

                            if(currentRelative.rank < lowNum || currentRelative.rank > hightNum) {
                                continue;
                            }
                        }
                        else if (strAnswerSearch.toFloat() != currentRelative.rank) {
                            continue;
                        }
                    }

                    ui->answerTextBrowser->append(doc_id + "     " + rank);
                }
                ui->answerTextBrowser->append(QString(100,'-'));
            }
            --i;
        }
    }
}

/* -------------------------------------- ↑↑↑ Вкладка Answer определение ↑↑↑ ---------------------------------------- */
/* ----------------------------------------------------------------------------------------------------------------- */

void OptionWindow::updateRequestsTextBrowser(const std::pair<size_t,std::vector<std::string>> & currentRequests) {

    QString temp;

    ui->requestsTextBrowser->append(QString("Request id: " + QString::number(currentRequests.first)));

    for(auto & currentWord : currentRequests.second){
        temp += currentWord + " ";
    }
    ui->requestsTextBrowser->append(temp);
    ui->requestsTextBrowser->append(QString(100,'-'));
}

void OptionWindow::updateDocsTextBrowser() {

    ui->docsPathsTextBrowser->clear();

    int i = 0 ;

    for(auto & current : srcEngine.getVectorOfDocsPath()) {
        QString temp = "doc_id: " + QString::number(i) + "  " + current.c_str();
        ui->docsPathsTextBrowser->append(temp);
        ui->docsPathsTextBrowser->append(QString(100,'-'));
        ++i;
    }

}

OptionWindow::~OptionWindow() {
    delete ui;
}
