#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QMouseEvent>
#include <QAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QDialog>
#include <QIntValidator>


class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = nullptr);
    QPushButton *pushButton_search;
    QPushButton *pushButton_lastPage;
    QPushButton *pushButton_nextPage;
    QPushButton *pushButton_minimize;
    QPushButton *pushButton_maximize;
    QPushButton *pushButton_close;
    QLineEdit *lineEdit_search;
    QLineEdit *lineEdit_page;
    QAction *action_set;
    QAction *action_search;
    QPushButton *loginPushBtn;

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    QPoint relativePos;
    bool isMLBD;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void moveMainWindow(QPoint);

public:
    void showUserImg(const QPixmap &pixmap);

private slots:
    void about();
};

#endif // TITLEBAR_H
