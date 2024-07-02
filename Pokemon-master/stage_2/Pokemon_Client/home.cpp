#include "home.h"
#include "ui_home.h"
#include "mainwindow.h"
home::home(const QList<Pkm>& pkms, const QString& username, uint port, QWidget *parent)
    : QWidget(parent),
    pkms(pkms),
    username(username),
    port(port),
    ui(new Ui::home)
{
    ui->setupUi(this);

    ui->lineEditUsername->setText(username);
    ui->lineEditPort->setText(QString::number(port));

    foreach(Pkm pkm, pkms){
        ui->comboBox->addItem(pkm.name);
    }
}

home::~home()
{
    delete ui;
}

void home::on_comboBox_activated(int index)
{
    QString selectedPkmName = ui->comboBox->itemText(index);

    foreach (const Pkm &pkm, pkms) {
        if (pkm.name == selectedPkmName) {
            // 找到匹配的精灵，创建详细信息字符串
            QString details = QString("ID: %1\n"
                                      "名称: %2\n"
                                      "种类: %3\n"
                                      "稀有度: %4\n"
                                      "等级: %5\n"
                                      "经验: %6\n"
                                      "当前血量: %7\n"
                                      "攻击力: %8\n"
                                      "血量: %9\n"
                                      "防御力: %10\n"
                                      "速度: %11\n"
                                      "技能列表: %12")
                                  .arg(pkm.id)
                                  .arg(pkm.name)
                                  .arg(pkm.kind)
                                  .arg(pkm.raity)
                                  .arg(pkm.level)
                                  .arg(pkm.experience)
                                  .arg(pkm.CurrentBlood)
                                  .arg(pkm.attack)
                                  .arg(pkm.blood)
                                  .arg(pkm.defense)
                                  .arg(pkm.speed);
            ui->label->setText(details);
            break;
        }
    }
}

