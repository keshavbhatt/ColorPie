#include "manager.h"
#include "ui_manager.h"

Manager::Manager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Manager)
{
    ui->setupUi(this);

    clipboard = QApplication::clipboard();

    colorWheel    = new color_widgets::ColorWheel(this);
    colorLineEdit = new color_widgets::ColorLineEdit(this);
    colorPreview  = new color_widgets::ColorPreview(this);

    colorLineEdit->setReadOnly(true);
    colorPreview->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    colorPreview->setFixedSize(50,colorLineEdit->sizeHint().height());

    connect(colorWheel,&color_widgets::ColorWheel::colorChanged,[=](const QColor color){
        colorPreview->setColor(color);
        colorLineEdit->blockSignals(true);
        colorLineEdit->setColor(color);
        colorLineEdit->blockSignals(false);

        Q_EMIT colorChanged(color.name());
    });
    ui->colorBoxLayout->addWidget(colorWheel);
    ui->previewLayout->addWidget(colorLineEdit);
    ui->previewLayout->addWidget(colorPreview);
}

void Manager::setFromHex6(QString hex6)
{
    colorLineEdit->setText(hex6);

    colorWheel->blockSignals(true);
    colorWheel->setColor(QColor(hex6));
    colorWheel->blockSignals(false);

    colorPreview->setColor(QColor(hex6));
}

void Manager::initialize(QColor color)
{
    ui->colorControl->setText(color.name());
}

Manager::~Manager()
{
    delete ui;
}

void Manager::on_colorControl_textChanged(const QString &arg1)
{
    Q_EMIT colorChanged(arg1);
    if(arg1.length()==7){
        QColor color(arg1);
        if(color.isValid()){
                colorWheel->setColor(color);
                colorPreview->setColor(color);
                colorLineEdit->setText(color.name());
        }
    }
}

void Manager::on_supportedInputs_clicked()
{
    SupportedInputs *sup = new SupportedInputs(this);
    sup->setWindowTitle(QApplication::applicationName()+" | "+"Supported Input");
    sup->setAttribute(Qt::WA_DeleteOnClose);
    sup->show();
}
