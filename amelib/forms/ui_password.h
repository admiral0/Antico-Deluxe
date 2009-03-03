/********************************************************************************
** Form generated from reading ui file 'password.ui'
**
** Created: Tue Mar 3 23:48:22 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PASSWORD_H
#define UI_PASSWORD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_passwordFrm
{
public:
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_3;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QLineEdit *usernameEdit;
    QLabel *label_4;
    QLineEdit *passwordEdit;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QToolButton *helpBtn;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelBtn;
    QPushButton *okBtn;

    void setupUi(QDialog *passwordFrm)
    {
    if (passwordFrm->objectName().isEmpty())
        passwordFrm->setObjectName(QString::fromUtf8("passwordFrm"));
    passwordFrm->resize(472, 207);
    gridLayout_2 = new QGridLayout(passwordFrm);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    horizontalSpacer_2 = new QSpacerItem(15, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    gridLayout_2->addItem(horizontalSpacer_2, 0, 0, 1, 1);

    label = new QLabel(passwordFrm);
    label->setObjectName(QString::fromUtf8("label"));
    label->setMaximumSize(QSize(64, 64));
    label->setPixmap(QPixmap(QString::fromUtf8(":/icons/dialog-password.png")));
    label->setScaledContents(true);

    gridLayout_2->addWidget(label, 0, 1, 1, 1);

    horizontalSpacer_6 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    gridLayout_2->addItem(horizontalSpacer_6, 0, 2, 1, 1);

    label_2 = new QLabel(passwordFrm);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    QFont font;
    font.setPointSize(10);
    font.setBold(true);
    font.setWeight(75);
    label_2->setFont(font);
    label_2->setWordWrap(true);

    gridLayout_2->addWidget(label_2, 0, 3, 1, 1);

    horizontalSpacer_3 = new QSpacerItem(60, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    gridLayout_2->addItem(horizontalSpacer_3, 0, 4, 1, 1);

    gridLayout = new QGridLayout();
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label_3 = new QLabel(passwordFrm);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    QFont font1;
    font1.setBold(false);
    font1.setWeight(50);
    label_3->setFont(font1);
    label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    gridLayout->addWidget(label_3, 0, 0, 1, 1);

    usernameEdit = new QLineEdit(passwordFrm);
    usernameEdit->setObjectName(QString::fromUtf8("usernameEdit"));

    gridLayout->addWidget(usernameEdit, 0, 1, 1, 1);

    label_4 = new QLabel(passwordFrm);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setFont(font1);
    label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    gridLayout->addWidget(label_4, 1, 0, 1, 1);

    passwordEdit = new QLineEdit(passwordFrm);
    passwordEdit->setObjectName(QString::fromUtf8("passwordEdit"));
    passwordEdit->setEchoMode(QLineEdit::Password);

    gridLayout->addWidget(passwordEdit, 1, 1, 1, 1);


    gridLayout_2->addLayout(gridLayout, 1, 3, 1, 2);

    verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

    gridLayout_2->addItem(verticalSpacer, 2, 4, 1, 1);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    helpBtn = new QToolButton(passwordFrm);
    helpBtn->setObjectName(QString::fromUtf8("helpBtn"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(helpBtn->sizePolicy().hasHeightForWidth());
    helpBtn->setSizePolicy(sizePolicy);
    helpBtn->setMinimumSize(QSize(27, 27));
    helpBtn->setMaximumSize(QSize(27, 27));
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/icons/question.png")), QIcon::Normal, QIcon::Off);
    helpBtn->setIcon(icon);
    helpBtn->setIconSize(QSize(12, 12));

    horizontalLayout->addWidget(helpBtn);

    horizontalSpacer = new QSpacerItem(208, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);

    cancelBtn = new QPushButton(passwordFrm);
    cancelBtn->setObjectName(QString::fromUtf8("cancelBtn"));
    QFont font2;
    font2.setBold(true);
    font2.setWeight(75);
    cancelBtn->setFont(font2);
    cancelBtn->setAutoDefault(false);

    horizontalLayout->addWidget(cancelBtn);

    okBtn = new QPushButton(passwordFrm);
    okBtn->setObjectName(QString::fromUtf8("okBtn"));
    okBtn->setFont(font2);
    okBtn->setDefault(true);

    horizontalLayout->addWidget(okBtn);


    gridLayout_2->addLayout(horizontalLayout, 3, 0, 1, 5);


    retranslateUi(passwordFrm);
    QObject::connect(cancelBtn, SIGNAL(clicked()), passwordFrm, SLOT(reject()));
    QObject::connect(okBtn, SIGNAL(clicked()), passwordFrm, SLOT(accept()));

    QMetaObject::connectSlotsByName(passwordFrm);
    } // setupUi

    void retranslateUi(QDialog *passwordFrm)
    {
    passwordFrm->setWindowTitle(QApplication::translate("passwordFrm", "Authentication?", 0, QApplication::UnicodeUTF8));
    label->setText(QString());
    label_2->setText(QApplication::translate("passwordFrm", "An application requires that you type your administrative password.", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("passwordFrm", "Name:", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("passwordFrm", "Password:", 0, QApplication::UnicodeUTF8));
    helpBtn->setText(QApplication::translate("passwordFrm", "...", 0, QApplication::UnicodeUTF8));
    cancelBtn->setText(QApplication::translate("passwordFrm", "Cancel", 0, QApplication::UnicodeUTF8));
    okBtn->setText(QApplication::translate("passwordFrm", "OK", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(passwordFrm);
    } // retranslateUi

};

namespace Ui {
    class passwordFrm: public Ui_passwordFrm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PASSWORD_H
