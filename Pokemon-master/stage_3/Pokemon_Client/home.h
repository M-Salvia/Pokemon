#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QString>
#include <mainwindow.h>
#include <QPixmap>

namespace Ui {
class home;
}

class home : public QWidget
{
    Q_OBJECT

public:
    explicit home(const QString& username, QWidget *parent = nullptr);
    ~home();

    void sendRequestToServer(uint requestType, const QString &username);
    void readPendingDatagrams();
private slots:
    void on_comboBox_activated(int index);

private:
    Ui::home *ui;

    QUdpSocket *server;
    QUdpSocket *client;

    QList<Pkm> pkms;
    QString username;
    uint port;

};

#endif // HOME_H
