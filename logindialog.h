#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QObject>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QAction>

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    LoginDialog(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *evnet);

private slots:
    void loginBtnPressed();
    void loginTimerTimeout();

signals:
    void loginSuccess(const int &userID, const QString &userImgUrl);

private:
    QVBoxLayout *mainLayout;
    QHBoxLayout *hLayout;
    QLabel *iconLabel;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QAction *loginPhoneIcon;
    QAction *loginPasswordIcon;
    QCheckBox *autoLoginCheckBox;
    QLabel *noticeIcon;
    QLabel *noticeLabel;
    QPushButton *closeBtn;
    QPushButton *loginBtn;
    bool mousePressed;
    QPoint mousePressedPos;
    QTimer *loginTimer;
};

#endif // LOGINDIALOG_H
