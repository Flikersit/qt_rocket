#include "CustomView.h"

CustomView::CustomView(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent)
{
    setRenderHint(QPainter::Antialiasing);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setAlignment(Qt::AlignCenter);
}

void CustomView::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);

    if (scene()) {
        // 
        QRectF sceneRect = scene()->sceneRect();
        qreal aspectRatio = sceneRect.width() / sceneRect.height();

        int newWidth = event->size().width();
        int newHeight = event->size().height();

        // 
        if (qreal(newWidth) / newHeight > aspectRatio) {
            newWidth = int(newHeight * aspectRatio);
        } else {
            newHeight = int(newWidth / aspectRatio);
        }

        // 
        QRectF newSceneRect(-newWidth / 2, newHeight, newWidth, newHeight);
        scene()->setSceneRect(newSceneRect);

        // 
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
        //this->setFixedSize(newWidth, newHeight);
    }
}
