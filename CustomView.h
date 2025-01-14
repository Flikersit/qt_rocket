#ifndef CUSTOMVIEW_H
#define CUSTOMVIEW_H

#include <QGraphicsView>
#include <QResizeEvent>
#include <QGraphicsScene>

class CustomView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit CustomView(QGraphicsScene* scene, QWidget* parent = nullptr);

protected:
    void resizeEvent(QResizeEvent* event) override;
};

#endif // CUSTOMVIEW_H
