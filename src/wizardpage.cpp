#include "wizardpage.h"


WizardPage::WizardPage(QWidget* parent) : QWizardPage(parent)
{


}

WizardPage::~WizardPage()
{
}

bool WizardPage::isComplete() const
{
        return complete;
}

void WizardPage::setComplete(bool val)
{
        complete = val;
        emit completeChanged();
}


