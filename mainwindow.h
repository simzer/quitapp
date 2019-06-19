#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QDate>
#include <QTime>
#include <QDateTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	QDateTime lastSmoking;
	double smokePerDay;
	QString moneyUnit;
	double cigarettePrice;
	Ui::MainWindow *ui;
	QTimer timer;
	QString setSize(QString, int size);

public slots:
	void refreshCounters();
	void setFontSize(QLabel *label, double factor);
private slots:
	void on_pushButton_clicked();
	void on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime);
	void on_pushButton_2_clicked();
	void on_spinBox_valueChanged(int arg1);
	void on_unitLineEdit_textChanged(const QString &arg1);
	void on_priceDoubleSpinBox_valueChanged(double arg1);
};

#endif // MAINWINDOW_H
