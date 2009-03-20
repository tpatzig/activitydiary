#include "wizarddialog.h"
#include <QDebug>


ADWizard::ADWizard(QObject* parent) : QWizard(), m_parent(parent)
{


        introPage = 0;
        namePage = 0;
        trackSourcePage = 0;
        physicalPage = 0;


        setAttribute(Qt::WA_DeleteOnClose);
        QSettings settings;
        iconDir = settings.value("ActivityIconDir").toString();

        addPage(createIntroPage());
        addPage(createDefineNamePage());
        addPage(createSelectTrackSourcePage());
        addPage(createPhysicalSourcePage());

        setWindowIcon(QIcon(iconDir + "logo_64.png"));
        setWindowTitle("ActivityDiary - New Activity Wizard");


        connect(button(QWizard::CancelButton),SIGNAL(clicked()),this,SLOT(slotCancel()));
        connect(button(QWizard::NextButton),SIGNAL(clicked()),this,SLOT(slotNext()));
        connect(button(QWizard::FinishButton),SIGNAL(clicked()),this,SLOT(slotFinish()));

        QPixmap pixmap(iconDir + "kompassberg_medium.png");

        setPixmap(QWizard::WatermarkPixmap,pixmap);

        show();
}

ADWizard::~ADWizard()
{
        qDebug() << "Deleting Wizard";

        if (introPage)
                delete introPage;
        if (namePage)
                delete namePage;
        if (trackSourcePage)
                delete trackSourcePage;
        if (physicalPage)
                delete physicalPage;

}

WizardPage* ADWizard::createIntroPage()
{
     introPage = new WizardPage;

     introPage->setTitle("Add New Activity");

     introLabel = new QLabel("This wizard will help to add new Activities to ActivityDiary.",introPage);
     introLabel->setWordWrap(true);

     introLayout = new QVBoxLayout(introPage);
     introLayout->addWidget(introLabel);
     introPage->setLayout(introLayout);

     introPage->setComplete(true);
     return introPage;

}

WizardPage* ADWizard::createDefineNamePage()
{
        namePage = new WizardPage;

        namePage->setTitle("Set Activity Name");

        nameLabel = new QLabel("Please define a name for your Activity.",namePage);
        nameLabel->setWordWrap(true);

        activityName = new QLineEdit(namePage);
        connect(activityName,SIGNAL(textChanged(const QString &)), this, SLOT(slotNameEditChanged(const QString &)));

        nameLayout = new QVBoxLayout(namePage);
        nameLayout->addWidget(nameLabel);
        nameLayout->addWidget(activityName);

        namePage->setLayout(nameLayout);
        namePage->setComplete(false);

        return namePage;
}

WizardPage* ADWizard::createSelectTrackSourcePage()
{
        trackSourcePage = new WizardPage;
        trackSourcePage->setComplete(false);

        trackSourcePage->setTitle("Select Track Source");

        trackLabel = new QLabel("Please select your Track Source.",trackSourcePage);
        trackLabel->setWordWrap(true);

        gpsRadioButton = new QRadioButton("Use a GPS recorded Track",trackSourcePage);
        gpsRadioButton->setChecked(true);
        connect(gpsRadioButton,SIGNAL(toggled(bool)),this,SLOT(slotGpsTrackSourceToggled(bool)));
        gpsLineEdit = new QLineEdit(trackSourcePage);
        connect(gpsLineEdit,SIGNAL(textChanged(const QString &)), this, SLOT(slotGPSLineEditChanged(const QString &)));

        gpsToolButton = new QPushButton("Choose...", trackSourcePage);
        connect(gpsToolButton,SIGNAL(clicked()),this,SLOT(slotGpsToolButtonClicked()));

        manualRadioButton = new QRadioButton("Create Track manually in the map",trackSourcePage);
        connect(manualRadioButton,SIGNAL(toggled(bool)),this,SLOT(slotManualTrackSourceToggled(bool)));

        startDateLabel = new QLabel("Activity Start");

        endDateLabel = new QLabel("Activity End");

        startDateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime(),trackSourcePage);
        endDateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime(),trackSourcePage);

        startDateLabel->setEnabled(false);
        endDateLabel->setEnabled(false);
        startDateTimeEdit->setEnabled(false);
        endDateTimeEdit->setEnabled(false);

        trackLayout1 = new QHBoxLayout;
        trackLayout2 = new QHBoxLayout;
        trackLayout3 = new QHBoxLayout;

        trackLayout1->addWidget(startDateLabel);
        trackLayout1->addWidget(startDateTimeEdit);

        trackLayout2->addWidget(endDateLabel);
        trackLayout2->addWidget(endDateTimeEdit);

        trackLayout3->addWidget(gpsLineEdit);
        trackLayout3->addWidget(gpsToolButton);

        trackSourceLayout = new QVBoxLayout;
        trackSourceLayout->addWidget(trackLabel);
        trackSourceLayout->addWidget(gpsRadioButton);
        trackSourceLayout->addLayout(trackLayout3);
        trackSourceLayout->addWidget(manualRadioButton);
        trackSourceLayout->addLayout(trackLayout1);
        trackSourceLayout->addLayout(trackLayout2);

        trackSourcePage->setLayout(trackSourceLayout);

        return trackSourcePage;

}

WizardPage* ADWizard::createPhysicalSourcePage()
{
        physicalPage = new WizardPage;
        physicalPage->setComplete(false);
        physicalPage->setTitle("Select Physical Data Source");
        physLabel = new QLabel("Define your Heartrate Data source file",physicalPage);
        physLabel->setWordWrap(true);

        physLineEdit = new QLineEdit(physicalPage);
        physToolButton = new QPushButton("Choose...", physicalPage);
        connect(physToolButton,SIGNAL(clicked()),this,SLOT(slotPhysToolButtonClicked()));


        physLayout = new QVBoxLayout(physicalPage);
        physLayout2 = new QHBoxLayout(physicalPage);
        physLayout2->addWidget(physLineEdit);
        physLayout2->addWidget(physToolButton);

        physLayout->addWidget(physLabel);
        physLayout->addLayout(physLayout2);

        physicalPage->setLayout(physLayout);

        return physicalPage;
}


void ADWizard::slotNext()
{
        qDebug() << "Page ID: " << currentId();
        switch (currentId()) {
                case 1 : {
                        qDebug() << "page 1 reached!";
                        break;

                         }
                case 2 : {

                        if (manualRadioButton->isChecked()) {
                            trackSourcePage->setComplete(true);
                        }
                        qDebug() << "page 2 reached!";

                        break;
                }
                case 3 : {
                        qDebug() << "page 3 reached!";
                        physicalPage->setComplete(true);
                        break;
                }
                case 4: {
                        qDebug() << "page 4 reached!";
                    }
        }
}


void ADWizard::slotFinish()
{
    qDebug() << "Track Name: " << activityName->text();

    if (gpsRadioButton->isChecked())
        trackSource = "gps";
    else if (manualRadioButton->isChecked()) {
        trackSource = "manual";
    }

    emit wizardFinished(activityName->text(),trackSource,physLineEdit->text() );
    close();
}



void ADWizard::slotCancel()
{
    emit wizardCancelled();
    close();
}



void ADWizard::slotManualTrackSourceToggled(bool checked)
{
    if (checked) {
        startDateLabel->setEnabled(true);
        endDateLabel->setEnabled(true);
        startDateTimeEdit->setEnabled(true);
        endDateTimeEdit->setEnabled(true);
        trackSourcePage->setComplete(true);

    } else {
        startDateLabel->setEnabled(false);
        endDateLabel->setEnabled(false);
        startDateTimeEdit->setEnabled(false);
        endDateTimeEdit->setEnabled(false);
        if (gpsLineEdit->text().size() > 0)
            trackSourcePage->setComplete(true);
        else
            trackSourcePage->setComplete(false);

    }
}

void ADWizard::slotGpsTrackSourceToggled(bool checked)
{
    if (checked) {
        gpsLineEdit->setEnabled(true);
        gpsToolButton->setEnabled(true);
    } else {
        gpsLineEdit->setEnabled(false);
        gpsToolButton->setEnabled(false);
    }
}

void ADWizard::slotNameEditChanged(const QString & text)
{
    if (text.size() > 0)
        namePage->setComplete(true);
    else
        namePage->setComplete(false);
}



void ADWizard::slotGPSLineEditChanged(const QString & text)
{
    if (text.size() > 0)
        trackSourcePage->setComplete(true);
    else
        trackSourcePage->setComplete(false);
}

void ADWizard::slotGpsToolButtonClicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select GPS File"), QDir::homePath(), tr("GPX Files (*.gpx)"));
    gpsLineEdit->setText(filename);
}


void ADWizard::slotPhysToolButtonClicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select HRM File"), QDir::homePath(), tr("HRM Files (*.hrm)"));
    physLineEdit->setText(filename);
}

QDateTime ADWizard::getManualStartDateTime()
{
    return startDateTimeEdit->dateTime();
}

QDateTime ADWizard::getManualEndDateTime()
{
    return endDateTimeEdit->dateTime();
}

QString ADWizard::getGPXFileName()
{
    return gpsLineEdit->text();
}

