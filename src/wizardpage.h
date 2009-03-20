#ifndef WIZARDPAGE_H
#define WIZARDPAGE_H
#include <QtGui>

class WizardPage : public QWizardPage
{
Q_OBJECT

public:
        WizardPage(QWidget* parent = 0);
        ~WizardPage();

        bool isComplete() const;
        void setComplete(bool);

private:

        bool complete;

signals:
        void completeChanged();

};

#endif // WIZARDPAGE_H
