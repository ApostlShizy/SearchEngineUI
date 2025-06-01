#ifndef OPTIONWINDOW_H
#define OPTIONWINDOW_H

#include <QMainWindow>
#include "searchengine.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class OptionWindow;
}
QT_END_NAMESPACE

class OptionWindow : public QMainWindow {
    Q_OBJECT

public:

    OptionWindow(QWidget *parent = nullptr);
    ~OptionWindow();

private:

    Ui::OptionWindow *ui;
    SearchEngine srcEngine;
    QString strAnswerSearch;

    bool sortFromToAnswer = true;

    void sortAnswer();
    void updateRequestsTextBrowser(const std::pair<size_t,std::vector<std::string>> & currentRequests);
    void updateDocsTextBrowser();

signals :

    void stateChangedSignal();

private slots:

    QString setGreen();
    QString setBlack();
    void stateChangedSlot();
    void formAnswerSlot();
    void setStrAnswerSlot(QString text);

};
#endif // OPTIONWINDOW_H
