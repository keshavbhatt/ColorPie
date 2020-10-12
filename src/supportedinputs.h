#ifndef SUPPORTEDINPUTS_H
#define SUPPORTEDINPUTS_H

#include <QDialog>

namespace Ui {
class SupportedInputs;
}

class SupportedInputs : public QDialog
{
    Q_OBJECT

public:
    explicit SupportedInputs(QWidget *parent = nullptr);
    ~SupportedInputs();

private:
    Ui::SupportedInputs *ui;
};

#endif // SUPPORTEDINPUTS_H
