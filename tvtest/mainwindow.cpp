#include "mainwindow.h"
#include <QHeaderView>
#include <QBrush>
#include <QColor>
#include <QRandomGenerator>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    // 1. QTreeViewとモデルの初期化
    setupTreeView();

    // 2. QTimerの設定 (1000ms = 1秒ごとに発火)
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
    m_timer->start(1000);
}
void MainWindow::setupTreeView()
{
    // 4カラムのモデルを作成
    m_model = new QStandardItemModel(0, 4, this);
    m_model->setHeaderData(0, Qt::Horizontal, "発生状態");
    m_model->setHeaderData(1, Qt::Horizontal, "発生時刻");
    m_model->setHeaderData(2, Qt::Horizontal, "イベントコード");
    m_model->setHeaderData(3, Qt::Horizontal, "イベントメッセージ");

    // デザイナーで配置した ui->treeView にモデルをセット
    treeView->setModel(m_model);

    // 見た目・挙動の調整
    treeView->setRootIsDecorated(false);               // 展開矢印を非表示（テーブル化）
    treeView->setAlternatingRowColors(true);          // 行背景を交互に変える
    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 編集不可
    treeView->setSelectionBehavior(QAbstractItemView::SelectRows); // 行選択

    // カラム幅調整
    treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    treeView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    treeView->header()->setSectionResizeMode(3, QHeaderView::Stretch);
}

void MainWindow::addEventRow(const QString &status, const QDateTime &time, 
                            const QString &code, const QString &message)
{
    auto *statusItem  = new QStandardItem(status);
    auto *timeItem    = new QStandardItem(time.toString("yy/MM/dd hh:mm:ss"));
    auto *codeItem    = new QStandardItem(code);
    auto *messageItem = new QStandardItem(message);

    // 「発生したばかり」は背景色を薄い赤色にする
    if (status == "発生したばかり") {
        QBrush alertBrush(QColor(255, 200, 200));
        statusItem->setBackground(alertBrush);
        timeItem->setBackground(alertBrush);
        codeItem->setBackground(alertBrush);
        messageItem->setBackground(alertBrush);
    } else if (status == "発生して確認している") {
        QBrush ackBrush(QColor(230, 240, 255));
        statusItem->setBackground(ackBrush);
    }

    // 最新のイベントを一番上（0行目）に挿入する場合
    m_model->insertRow(0, { statusItem, timeItem, codeItem, messageItem });
    qDebug() << m_model->rowCount();

    // ※ 最下部に追加したい場合は insertRow ではなく appendRow を使用します:
    // m_model->appendRow({ statusItem, timeItem, codeItem, messageItem });
}

// 1秒ごとに呼び出される処理
void MainWindow::onTimerTimeout()
{
    m_eventCount++;

    // 1. 古い「発生したばかり」のステータスを「発生して確認している」に更新する例
    // （過去のイベントの状態変化をシミュレート）
    for (int row = 0; row < m_model->rowCount(); ++row) {
        QStandardItem *statusItem = m_model->item(row, 0);
        if (statusItem && statusItem->text() == "発生したばかり") {
            // 確率で状態を更新
            if (QRandomGenerator::global()->bounded(100) < 40) {
                statusItem->setText("発生して確認している");
                
                // 行全体の背景色をクリア・変更
                for (int col = 0; col < 4; ++col) {
                    m_model->item(row, col)->setBackground(QBrush()); // デフォルトに戻す
                }
                statusItem->setBackground(QBrush(QColor(230, 240, 255))); // 薄い青
            }
        }
    }

    // 2. 新しいイベントを追加（ランダムなメッセージを生成）
    QString code = QString("EV-%1").arg(m_eventCount, 4, 10, QChar('0'));
    
    static const QStringList sampleMessages = {
        "センサー通信エラー検知",
        "メモリー使用量が閾値を超過",
        "電源電圧の変動を検知",
        "タスク処理タイムアウト"
    };
    
    int randomIndex = QRandomGenerator::global()->bounded(sampleMessages.size());
    QString msg = sampleMessages.at(randomIndex);

    addEventRow("発生したばかり", QDateTime::currentDateTime(), code, msg);

    // 行数が多くなりすぎないように制限（例: 最大100件）
    if (m_model->rowCount() > 100) {
        m_model->removeRow(m_model->rowCount() - 1);
    }
}

MainWindow::~MainWindow()
{
    
    
}

