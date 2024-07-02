#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QString>
#include <mainwindow.h>

namespace Ui {
class home;
}

class home : public QWidget
{
    Q_OBJECT

public:
    explicit home(const QList<Pkm>& pkms, const QString& username, uint port, QWidget *parent = nullptr);
    ~home();

private slots:
    void on_comboBox_activated(int index);

private:
    Ui::home *ui;
    QList<Pkm> pkms;
    QString username;
    uint port;
};

#endif // HOME_H
