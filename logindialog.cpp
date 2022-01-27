#include "logindialog.h"
#include "httprequest.h"

#include <QMouseEvent>
#include <QDebug>
#include <QCryptographicHash>
#include <QIntValidator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

static const QString mainUrl = "http://10.42.0.254:3000/";

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    this->setStyleSheet("QDialog {background-color: #fafafa;}"
                          "QPushButton {background-color: #fafafa;}"
                          "QLineEdit {"
                          "font-size: 14px;  "
                          "color:#000000; "
                          "background-color:#ffffff; "
                          "border:1px solid #d9d9d9; "
                          "}"
                          "QLabel {background-color: #fafafa;}"
                          "QCheckBox {"
                          "background-color: #fafafa;"
                          "color: #848484;"
                          "font-size: 12px;"
                          "}");

    this->setFixedWidth(350);
    mainLayout = new QVBoxLayout;
    hLayout = new QHBoxLayout;

    closeBtn = new QPushButton;
    closeBtn->setFixedSize(24, 24);
    closeBtn->setStyleSheet("QPushButton { border-image: url(:/icon/close.svg); }"
                            "QPushButton:hover { border-image: url(:/icon/close_hover.svg); }"
                            "QPushButton:pressed { border-image: url(:/icon/close_pressed.svg); }");
    closeBtn->setIconSize(QSize(24, 24));
    closeBtn->setFlat(true);
    closeBtn->setFocusPolicy(Qt::NoFocus);
    mainLayout->addWidget(closeBtn, 0, Qt::AlignRight);

    mainLayout->addSpacing(50);

    iconLabel = new QLabel;
    iconLabel->setFixedSize(80, 80);
    iconLabel->setPixmap(QPixmap(":/icon/phone.svg").
                         scaled(iconLabel->size(),
                                Qt::KeepAspectRatioByExpanding,
                                Qt::SmoothTransformation));
    mainLayout->addWidget(iconLabel, 0, Qt::AlignCenter);

    mainLayout->addSpacing(50);
    mainLayout->setSpacing(0);
    usernameEdit = new QLineEdit;
    usernameEdit->setPlaceholderText(tr("请输入手机号"));
    usernameEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));
    usernameEdit->setFixedWidth(230);
    usernameEdit->setFixedHeight(40);
    usernameEdit->setStyleSheet("QLineEdit {border-bottom: 1px solid #d9d9d9;"
                                "border-top-left-radius: 5px;"
                                "border-top-right-radius: 5px;}");
    loginPhoneIcon = new QAction;
    loginPhoneIcon->setIcon(QIcon(":/icon/login_phone.svg"));
    usernameEdit->addAction(loginPhoneIcon, QLineEdit::LeadingPosition);
    mainLayout->addWidget(usernameEdit, 0, Qt::AlignCenter);

    passwordEdit = new QLineEdit;
    passwordEdit->setPlaceholderText(tr("请输入密码"));
    passwordEdit->setFixedWidth(230);
    passwordEdit->setFixedHeight(40);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setStyleSheet("QLineEdit {border-top: 0px;"
                                "border-bottom-left-radius: 5px;"
                                "border-bottom-right-radius: 5px;}");
    loginPasswordIcon = new QAction;
    loginPasswordIcon->setIcon(QIcon(":/icon/password.svg"));
    passwordEdit->addAction(loginPasswordIcon, QLineEdit::LeadingPosition);
    mainLayout->addWidget(passwordEdit, 0, Qt::AlignCenter);

    autoLoginCheckBox = new QCheckBox;
    autoLoginCheckBox->setText(tr("自动登录"));
    autoLoginCheckBox->setStyleSheet("QCheckBox:checked {color: #bd3434;}");
    hLayout->addSpacing(50);
    hLayout->addWidget(autoLoginCheckBox);

    noticeIcon = new QLabel;
    noticeIcon->setFixedSize(16, 16);
    noticeIcon->setPixmap(QPixmap(":/icon/warning.svg").
                          scaled(noticeIcon->size(),
                                 Qt::KeepAspectRatioByExpanding,
                                 Qt::SmoothTransformation));
    noticeIcon->setHidden(true);


    noticeLabel = new QLabel;
    noticeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    noticeLabel->setStyleSheet("QLabel {color: #d87171;"
                               "font-size: 12px;}");
    noticeLabel->setHidden(true);
    hLayout->addSpacing(60);
    hLayout->addWidget(noticeIcon);
    hLayout->addWidget(noticeLabel);
    hLayout->addStretch();
    mainLayout->addSpacing(5);
    mainLayout->addLayout(hLayout);

    mainLayout->addSpacing(10);
    loginBtn = new QPushButton;
    loginBtn->setText(tr("登 录"));
    loginBtn->setFixedWidth(230);
    loginBtn->setFixedHeight(40);
    loginBtn->setStyleSheet("QPushButton {background-color: #d33030; "
                            "color: #ffffff;"
                            "border: none;"
                            "border-radius: 5px;}"
                            "QPushButton:hover {background-color: #c72e2e;}"
                            "QPushButton:disabled {background-color: #dedede;"
                            "color: #bfbfbf;}");
    mainLayout->addWidget(loginBtn, 0, Qt::AlignCenter);
    mainLayout->addSpacing(100);

    this->setLayout(mainLayout);

    mousePressed = false;
    loginTimer = new QTimer;
    loginTimer->setSingleShot(true);
    loginTimer->setInterval(3000);
    loginTimer->stop();

    connect(loginTimer, &QTimer::timeout,
            this, &LoginDialog::loginTimerTimeout);
    connect(closeBtn, &QPushButton::pressed,
            this, &LoginDialog::close);

    connect(loginBtn, &QPushButton::pressed,
            this, &LoginDialog::loginBtnPressed);
}

void LoginDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton )
    {
        mousePressed = true;
        mousePressedPos = event->pos();
    }
}

void LoginDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (mousePressed)
    {
        setCursor(Qt::ClosedHandCursor);
        move(event->globalPos() - mousePressedPos);
    }
}

void LoginDialog::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    setCursor(Qt::ArrowCursor);
    mousePressed = false;
}

void LoginDialog::loginBtnPressed()
{
    loginBtn->setDisabled(true);
    if (usernameEdit->text().isEmpty())
    {
        noticeLabel->setText(tr("请输入手机号"));
        noticeIcon->setHidden(false);
        noticeLabel->setHidden(false);
        loginBtn->setDisabled(false);
        return;
    }
    else if (passwordEdit->text().isEmpty())
    {
        noticeLabel->setText(tr("请输入登录密码"));
        noticeIcon->setHidden(false);
        noticeLabel->setHidden(false);
        loginBtn->setDisabled(false);
        return;
    }
    else
    {
        noticeIcon->setHidden(true);
        noticeLabel->setHidden(true);
    }

    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    QString passwordMD5;
    QByteArray data = QCryptographicHash::hash(password.toLatin1(),
                                               QCryptographicHash::Md5);
    passwordMD5.append(data.toHex());

    QString url = mainUrl + QString("/login/cellphone?phone=%1&md5_password=%2")
                                    .arg(username).arg(passwordMD5);
    loginTimer->start();
    QJsonDocument JD = QJsonDocument::fromJson(HttpRequest::getRequest(url));

    int code = JD.object().value("code").toInt();
    if (code != 200)
    {
        noticeLabel->setText(tr("用户名或密码错误"));
        noticeIcon->setHidden(false);
        noticeLabel->setHidden(false);
        loginBtn->setEnabled(true);
        return;
    }
    if (code == 200)
    {
        QString userImgUrl = JD.object().value("profile").
                             toObject().value("avatarUrl").toString();
        int userID = JD.object().value("account").
                            toObject().value("id").toInt();
        qDebug() << userID;
        qDebug() << userImgUrl;
        emit loginSuccess(userID, userImgUrl);
        this->close();
    }
}

void LoginDialog::loginTimerTimeout()
{
    qDebug() << "timeout";
    noticeLabel->setText(tr("请检查网络"));
    noticeIcon->setHidden(false);
    noticeLabel->setHidden(false);
    loginBtn->setEnabled(true);
}
