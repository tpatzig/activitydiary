/*
 *    ActivityDiary - The smart way to manage your GPS tracks
 *
 *    Copyright (C) 2009
 *
 *                - Tom Patzig <tom@activitydiary.org>
 *                - Thomas Goettlicher <thomas@activitydiary.org>
 *
 *    This file is part of ActivityDiary.
 *
 *    ActivityDiary is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    ActivityDiary is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with ActivityDiary.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef WIZARDDIALOG_H
#define WIZARDDIALOG_H


#include <QtGui>
#include <QWizard>


#include "wizardpage.h"


class ADWizard :   public QWizard
{
Q_OBJECT

public:
        ADWizard(QObject* parent = 0);
        ~ADWizard();

        QDateTime getManualStartDateTime();
        QDateTime getManualEndDateTime();
        QString getGPXFileName();

private:

        QObject* m_parent;
        WizardPage* introPage;
        QLabel* introLabel;
        QVBoxLayout* introLayout;

        WizardPage* namePage;
        QLabel* nameLabel;
        QVBoxLayout* nameLayout;
        QLineEdit* activityName;

        WizardPage* trackSourcePage;
        QLabel* trackLabel;
        QLabel* startDateLabel;
        QLabel* endDateLabel;
        QDateTimeEdit* startDateTimeEdit;
        QDateTimeEdit* endDateTimeEdit;

        QLineEdit* gpsLineEdit;
        QPushButton* gpsToolButton;

        QRadioButton* gpsRadioButton;
        QRadioButton* manualRadioButton;

        QVBoxLayout* trackSourceLayout;
        QHBoxLayout* trackLayout1;
        QHBoxLayout* trackLayout2;
        QHBoxLayout* trackLayout3;

        WizardPage* physicalPage;
        QLabel* physLabel;
        QLineEdit* physLineEdit;
        QPushButton* physToolButton;

        QVBoxLayout* physLayout;
        QHBoxLayout* physLayout2;


        WizardPage* createIntroPage();
        WizardPage* createDefineNamePage();
        WizardPage* createSelectTrackSourcePage();
        WizardPage* createPhysicalSourcePage();

        QString trackSource;

        QString iconDir;


private slots:
        void slotCancel();
        void slotNext();
        void slotFinish();

        void slotManualTrackSourceToggled(bool);
        void slotGpsTrackSourceToggled(bool);
        void slotNameEditChanged(const QString &);
        void slotGPSLineEditChanged(const QString &);
        void slotGpsToolButtonClicked();
        void slotPhysToolButtonClicked();

signals:
        void wizardFinished(QString,QString,QString);
        void wizardCancelled();

};


#endif // WIZARDDIALOG_H
