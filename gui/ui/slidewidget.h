#ifndef SLIDEWIDGET_H
#define SLIDEWIDGET_H

#include <QWidget>

namespace Ui {
class SlideWidget;
}

class SlideWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SlideWidget(QWidget *parent = 0);
    ~SlideWidget();
protected:
    void setBGColor();
private:
    Ui::SlideWidget *ui;
};

#endif // SLIDEWIDGET_H
