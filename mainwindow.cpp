#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	QCoreApplication::setOrganizationName("Laci");
	QCoreApplication::setOrganizationDomain("laci.laci");
	QCoreApplication::setApplicationName("Smoke Quitter");

	ui->setupUi(this);

	ui->settingsWidget->hide();
/*
	setFontSize(ui->label, 2);
	setFontSize(ui->label_3, 2);
	setFontSize(ui->label_5, 2);
	setFontSize(ui->label_2, 2);
	setFontSize(ui->label_4, 2);

	setFontSize(ui->timeLabel, 5);
	setFontSize(ui->countLabel, 5);
	setFontSize(ui->nicLevelLabel, 5);
	setFontSize(ui->progressLabel, 5);
	setFontSize(ui->difficultyLabel, 5);
	setFontSize(ui->relapseLabel, 5);
*/
	timer.setInterval(50);
	connect(&timer, SIGNAL(timeout()), this, SLOT(refreshCounters()));
	timer.start();

	QSettings settings;

	lastSmoking = QDateTime::currentDateTime();
	if (settings.contains("lastSmoke"))
	{
		lastSmoking = settings.value("lastSmoke").toDateTime();
	}
	ui->dateTimeEdit->setDateTime(lastSmoking);

	smokePerDay = 10;
	if (settings.contains("smokePerDay"))
	{
		smokePerDay = settings.value("smokePerDay").toInt();
	}
	ui->spinBox->setValue(smokePerDay);

	moneyUnit = "USD";
	if (settings.contains("moneyUnit"))
	{
		moneyUnit = settings.value("moneyUnit").toString();
	}
	ui->unitLineEdit->setText(moneyUnit);

	cigarettePrice = 0.5;
	if (settings.contains("cigarettePrice"))
	{
		cigarettePrice = settings.value("cigarettePrice").toDouble();
	}
	ui->priceDoubleSpinBox->setValue(cigarettePrice);

	refreshCounters();
}

MainWindow::~MainWindow()
{
	delete ui;
}

QString MainWindow::setSize(QString str, int size)
{
	return "<font size=" + QString::number(size) + ">" + str + "</font>";
}

void MainWindow::refreshCounters()
{
	const int fontSize = 72;

	auto now = QDateTime::currentDateTime();

	auto nonSmokingSecs = lastSmoking.secsTo(now);
	auto nonSmokingMSecs = lastSmoking.msecsTo(now);
	auto cnt = nonSmokingSecs;
	QStringList timeStr;
	if (cnt > 0) timeStr.push_back(QString::number(cnt % 60) + " secs ");
	cnt /= 60;
	if (cnt > 0) timeStr.push_back(QString::number(cnt % 60) + " mins ");
	cnt /= 60;
	if (cnt > 0) timeStr.push_back(QString::number(cnt % 24) + " hours ");
	cnt /= 24;
	if (cnt > 0) timeStr.push_back(QString::number(cnt) + " days ");

	QString timeS;
	if (timeStr.size() > 0) {
		timeS += timeStr.back();
		timeStr.pop_back();
	}
	if (timeStr.size() > 0) {
		timeS += timeStr.back();
		timeStr.pop_back();
	}
	ui->timeLabel->setText(setSize(timeS, fontSize));

	auto smokesPerSecond = smokePerDay / 24 / 60 / 60;
	int smokeNotSmoked = int(nonSmokingSecs * smokesPerSecond);
	auto countStr = QString::number(smokeNotSmoked);
	auto worth = smokeNotSmoked * cigarettePrice;
	countStr += " (" + QString::number(worth) + " " + moneyUnit + ")";
	ui->countLabel->setText(setSize(countStr, fontSize));

	auto hours = (double)nonSmokingMSecs / 60 / 60 / 1000;
	auto nicotineLevel = 100 * exp(-0.17*hours);
	auto nociLevelStr = QString::number(nicotineLevel, 'f', 2) + " %";
	ui->nicLevelLabel->setText(setSize(nociLevelStr, fontSize));

	auto days = (double)nonSmokingMSecs / 1000 / 60 / 60 / 24;
	auto progress = 100*(1-exp(-days/5.5));
	if (days > 21) progress = 100;
	auto progressStr = QString::number(progress, 'f', 2) + " %";
	ui->progressLabel->setText(setSize(progressStr, fontSize));

	auto difficulty = 100*(exp(-days/5.5));
	auto difficultyStr = QString::number(difficulty, 'f', 2) + " %";
	ui->difficultyLabel->setText(setSize(difficultyStr, fontSize));

	auto factor = 0.79;
	auto fact1 = 0.3;
	auto fact2 = 35;
	auto year = days/365.0;
	auto months = 12*year;
	auto abstinent = 100*(factor*exp(-months/fact1)+(1-factor)*exp(-months/fact2));
	auto abstinent1Year = 100*(factor*exp(-12/fact1)+(1-factor)*exp(-12/fact2));
	auto relapse = 100*(abstinent-abstinent1Year)/(100-abstinent1Year);
	auto relapseStr = QString::number(relapse, 'f', 2) + " %";
	ui->relapseLabel->setText(setSize(relapseStr, fontSize));
}

void MainWindow::setFontSize(QLabel *label, double factor)
{
	auto font = label->font();
	font.setPointSizeF(font.pointSizeF() * factor);
	label->setFont(font);
}

void MainWindow::on_pushButton_clicked()
{
	ui->settingsWidget->setVisible(!ui->settingsWidget->isVisible());
}

void MainWindow::on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
	QSettings settings;
	lastSmoking = dateTime;
	settings.setValue("lastSmoke", lastSmoking);
}

void MainWindow::on_pushButton_2_clicked()
{
	on_dateTimeEdit_dateTimeChanged(QDateTime::currentDateTime());
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
	QSettings settings;
	smokePerDay = arg1;
	settings.setValue("smokePerDay", smokePerDay);
}

void MainWindow::on_unitLineEdit_textChanged(const QString &arg1)
{
	QSettings settings;
	moneyUnit = arg1;
	settings.setValue("moneyUnit", moneyUnit);
}

void MainWindow::on_priceDoubleSpinBox_valueChanged(double arg1)
{
	QSettings settings;
	cigarettePrice = arg1;
	settings.setValue("cigarettePrice", cigarettePrice);
}
