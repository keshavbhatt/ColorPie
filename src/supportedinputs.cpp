#include "supportedinputs.h"
#include "ui_supportedinputs.h"

SupportedInputs::SupportedInputs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SupportedInputs)
{
    ui->setupUi(this);
}

SupportedInputs::~SupportedInputs()
{
    delete ui;
}
