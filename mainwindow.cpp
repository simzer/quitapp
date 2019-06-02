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

	timer.setInterval(1000);
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
	auto cnt = nonSmokingSecs;
	QString timeStr;
	if (cnt > 0) timeStr = QString::number(cnt % 60) + " secs " + timeStr;
	cnt /= 60;
	if (cnt > 0) timeStr = QString::number(cnt % 60) + " mins " + timeStr;
	cnt /= 60;
	if (cnt > 0) timeStr = QString::number(cnt % 24) + " hours " + timeStr;
	cnt /= 24;
	if (cnt > 0) timeStr = QString::number(cnt) + " days " + timeStr;

	ui->timeLabel->setText(setSize(timeStr, fontSize));

	auto smokesPerSecond = smokePerDay / 24 / 60 / 60;
	int smokeNotSmoked = int(nonSmokingSecs * smokesPerSecond);
	auto countStr = QString::number(smokeNotSmoked);
	ui->countLabel->setText(setSize(countStr, fontSize));

	auto hours = (double)nonSmokingSecs / 60 / 60;
	auto nicotineLevel = 100 * exp(-0.17*hours);
	auto nociLevelStr = QString::number(nicotineLevel, 'f', 2) + " %";
	ui->nicLevelLabel->setText(setSize(nociLevelStr, fontSize));

	auto days = (double)nonSmokingSecs / 60 / 60 / 24;
	auto progress = 100*(1-exp(-days/5.5));
	if (days > 21) progress = 100;
	auto progressStr = QString::number(progress, 'f', 2) + " %";
	ui->progressLabel->setText(setSize(progressStr, fontSize));

	auto difficulty = 100*(exp(-days/5.5));
	auto difficultyStr = QString::number(difficulty, 'f', 2) + " %";
	ui->difficultyLabel->setText(setSize(difficultyStr, fontSize));
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
