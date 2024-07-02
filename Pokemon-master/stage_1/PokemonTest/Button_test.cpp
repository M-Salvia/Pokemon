#include <QCoreApplication>
#include <QtTest>
#include <QPushButton>
#include <QLabel>
#include"../Pokemon/pokemon.h"
#include "../Pokemon/mainwindow.h"
#include "../Pokemon/Enity.h"
#include "../Pokemon/mainwindow.cpp"
#include "../Pokemon/Enity.cpp"
#include "../Pokemon/ui_mainwindow.h"
//测试stage1的三个基本按钮，attack,levelup,change

class TESTS : public QObject
{
    Q_OBJECT

public:
    TESTS();
    ~TESTS();

private:
    MainWindow *mainWindow;
    QPushButton *attack;
    QPushButton *levelup;
    QPushButton *change;
    QLabel *AInfoLabel;
    QLabel *InfoLabel;

private slots:
    void initTestCase();
    void cleanupTestCase();
    //这就是两个QTest提供的钩子函数
    void test_AttackBtn();
    void test_Attack();
    void test_LevelUp();
    void test_Change();
};

TESTS::TESTS() {
    this->initTestCase();
}

TESTS::~TESTS() {}

void TESTS::initTestCase() {
    // 在这里编写初始化代码
    // 这个函数在测试对象创建后、第一个测试用例执行前运行一次

    // 例如，为 MainWindow 实例化并设置 UI
    this->mainWindow = new MainWindow;
    mainWindow->InitPkm();
    mainWindow->CreatePkm();
    mainWindow->show();
    QCoreApplication::processEvents(); // 确保 UI 已更新

    // 您可以将 mainWindow 设置为类成员，以便在其他测试用例中使用

}
void TESTS::cleanupTestCase() {
    // 在这里编写清理代码
    // 这个函数在所有测试用例执行完毕后运行一次

    // 例如，删除 MainWindow 对象
    delete this->mainWindow;
    this->mainWindow = nullptr;

    // 执行其他必要的清理工作...
}
void TESTS::test_Attack() {

    Pokemon*  pikachu = new class Pikachu(1);

    QList<QString> expectedAttacks = {"Normal Attack"};
    QList<QString> pikachuSkills = AttackSkills.value("Pikachu");
    expectedAttacks << pikachuSkills;

    QString attackName = pikachu->Attack();
    QVERIFY(!attackName.isEmpty());

    QVERIFY(expectedAttacks.contains(attackName));

    delete pikachu;
}
void TESTS::test_AttackBtn() {
    this->attack = mainWindow->findChild<QPushButton*>("AttackBtn");
    QVERIFY(attack != nullptr);
    QTest::mouseClick(attack,Qt::LeftButton);
    //验证是否能点击按钮

    this->AInfoLabel = mainWindow->findChild<QLabel*>("AInfoLabel");
    QVERIFY(AInfoLabel != nullptr);
    QCOMPARE(AInfoLabel->text(),QString("马甲帅"));

}
void TESTS::test_LevelUp() {}
void TESTS::test_Change() {}

QTEST_MAIN(TESTS)

#include "Button_test.moc"
