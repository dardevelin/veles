#include <QtWidgets/QWidget>
namespace veles {
namespace ui {


class TabButton : public QWidget {

 public:
  explicit TabButton(QWidget * parent = nullptr);

  signals:
  void tabCloseRequested(int index);

 private:



};
}
}