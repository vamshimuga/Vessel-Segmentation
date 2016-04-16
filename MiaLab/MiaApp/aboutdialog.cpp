#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->agreeButton->setVisible(false);
    ui->disagreeButton->setVisible(false);
    ui->licenseMessage->setVisible(false);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
void AboutDialog::showAgreeButtons()
{
    ui->agreeButton->setVisible(true);
    ui->disagreeButton->setVisible(true);
    ui->licenseMessage->setVisible(true);
    ui->okButton->setVisible(false);
}

void AboutDialog::setLienceString(QString lic)
{
    ui->licenseString->setText(lic);
}

QString AboutDialog::getLicenseString()
{
    return ui->licenseString->text();
}
