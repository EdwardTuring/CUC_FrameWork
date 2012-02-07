#include "slidewidget.h"
#include "ui_slidewidget.h"

SlideWidget::SlideWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SlideWidget)
{
    ui->setupUi(this);
    setBGColor();
}

void SlideWidget::setBGColor()
{
    QPalette myPalette;
    QColor myColor(29,29,31);
    myColor.setAlphaF(0.7);
    myPalette.setBrush(backgroundRole(),myColor);
    setPalette(myPalette);
    setAutoFillBackground(true);
}

SlideWidget::~SlideWidget()
{
    delete ui;
}
