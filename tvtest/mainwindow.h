#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QTimer>
#include <QDateTime>
#include <QMainWindow>
//#include <QtCore>
//#include <QtGui>
class QStandardItemModel ;
class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private slots:
    void onTimerTimeout(); // 1秒ごとに呼ばれるスロット
private:
    QStandardItemModel *m_model;
    QTimer *m_timer;
    int m_eventCount = 0; // イベントコード生成用のカウンター

    void setupTreeView();
    void addEventRow(const QString &status, const QDateTime &time, 
                     const QString &code, const QString &message);
};
#endif // MAINWINDOW_H
