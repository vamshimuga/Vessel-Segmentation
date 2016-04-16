#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
    class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();
    void showAgreeButtons();
    void setLienceString(QString lic);
    QString getLicenseString();

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
