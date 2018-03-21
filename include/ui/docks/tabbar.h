#pragma once

#include <QtWidgets/QTabBar>
#include <QMouseEvent>



namespace veles {
namespace ui {

class TabBar : public QTabBar {

  // customizable TabBar for enabling tab dragging and (un)docking
  Q_OBJECT

  void mousePressEvent(QMouseEvent *event);

  void mouseMoveEvent(QMouseEvent *event);

  void mouseReleaseEvent(QMouseEvent *event);


};

}}