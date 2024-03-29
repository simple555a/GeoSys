#include <QMessageBox>
#include "dlgobjname.h"

DlgObjName::DlgObjName(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    label = new QLabel(tr("Name:"));
    textName = new QLineEdit;
    textName->setMaxLength(32);
    buttonBox = new QDialogButtonBox;
    QPushButton *buttonOk = buttonBox->addButton(QDialogButtonBox::Ok);
    QPushButton *buttonCancel = buttonBox->addButton(QDialogButtonBox::Cancel);
    layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(textName);
    layout->addWidget(buttonBox);
    setLayout(layout);

    connect(buttonOk, SIGNAL(clicked(bool)), this, SLOT(ok(bool)));
    connect(buttonCancel, SIGNAL(clicked(bool)), this, SLOT(cancel(bool)));
}

DlgObjName::~DlgObjName()
{
}

void DlgObjName::ok(bool check)
{
    QString name = textName->text();
    if(name.indexOf(";") != -1 || name.indexOf(",") != -1){
        QMessageBox::warning(this, "Warining", tr("Program name can't contains ';' or ','"));
        return;
    }

    accept();
}

void DlgObjName::cancel(bool check)
{
    reject();
}

void DlgObjName::setTitle(QString title)
{
    setWindowTitle(title);
}

void DlgObjName::setName(QString name)
{
    textName->setText(name);
    //textName->setS;
}

QString DlgObjName::getName()
{
    return textName->text();
}
