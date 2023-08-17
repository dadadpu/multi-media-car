#include "mainwindow.h"
void MainWindow::slotFinishedWeatherGetReply(void)
{
    qDebug() << "slotFinishedWeatherGetReply---begin";
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    QString temp = reply->readAll();
    //qDebug() << temp;
    reply->deleteLater();

    if (temp.isEmpty() || temp.isNull()) {
        cityPushButton->setText("定位失败");
        return;
    }

    QJsonParseError error;
    QJsonObject jsonObj;
    QJsonDocument jsonDocument;

    jsonDocument = QJsonDocument::fromJson(temp.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError) {
        cityPushButton->setText("定位失败");
        return;
    }

    if (!jsonDocument.isObject()) {
        cityPushButton->setText("定位失败");
        return;
    }

    jsonObj = jsonDocument.object();
    QJsonArray forecasts = jsonObj.value("forecasts").toArray();
    if (forecasts.isEmpty()) {
        cityPushButton->setText("定位失败");
        return;
    }

    QJsonObject forecastObj = forecasts.at(0).toObject();

    QString city = forecastObj.value("city").toString();
    cityPushButton->setText(city);

    QJsonArray casts = forecastObj.value("casts").toArray();
    for (int i = 0; i < casts.size(); ++i) {
        QJsonObject castObj = casts.at(i).toObject();

        QString date = castObj.value("date").toString();
        QString week = castObj.value("week").toString();
        QString dayWeather = castObj.value("dayweather").toString();
        QString nightWeather = castObj.value("nightweather").toString();
        QString dayTemp = castObj.value("daytemp").toString();
        QString nightTemp = castObj.value("nighttemp").toString();
        QString dayWind = castObj.value("daywind").toString();
        QString nightWind = castObj.value("nightwind").toString();


        QString currentDate = currentTime->currentDateTime().toString("yyyy-MM-dd\nhh:mm:ss ddd");

        // 获取当前时间的小时部分
        int currentHour = currentTime->currentDateTime().time().hour();

        // 获取凌晨时刻的最低温度值
        int nightTemperature = nightTemp.toInt();

        // 获取中午时刻的最高温度值
        int dayTemperature = dayTemp.toInt();

        // 计算时间与凌晨之间的差值（小时）
        int delta = currentHour;

        // 计算时间差对应的温度范围的比例
        double ratio = static_cast<double>(delta) / 12.0;

        // 进行线性插值计算
        int interpolatedTemp = nightTemperature + static_cast<int>(ratio * (dayTemperature - nightTemperature));

        // 将插值温度值显示在界面上或进行其他操作
        // 例如：
        QString interpolatedTempStr = QString::number(interpolatedTemp);
        currentTempLabel->setText(interpolatedTempStr);



        // Process date to display in the desired format
        QString processedDate;
        if (date.length() == 10) {
            processedDate = date.mid(5, 5);
        } else if (date.length() == 9) {
            processedDate = date.mid(5, 4);
        } else {
            processedDate = date.mid(5, 3);
        }

        // Set the week label
        if (i == 0) {
            weekLabelArr[i]->setText(processedDate + " (" + week + ")(今天)");
        } else {
            weekLabelArr[i]->setText(processedDate + " (" + week + ")");
        }

        // Set the temperature labels
        QString temperature = dayTemp + "℃~" + nightTemp + "℃";
        if (i == 0) {
            minMaxTempLabelArr[i]->setText(temperature);
        } else {
            minMaxTempLabelArr[i]->setText(temperature);
        }

        // Set the weather type labels
        QString weatherType = dayWeather;
        if (i == 0) {
            weatherTypeLabelArr[i]->setText(weatherType + "(实时)");
            currentWeather->setText(weatherType);
        } else {
            weatherTypeLabelArr[i]->setText(weatherType);
        }

        // Set the weather icon labels
        QString dayWeatherIconPath = "./pic/" + dayWeather + ".png";
        QString nightWeatherIconPath = "./pic/" + nightWeather + ".png";
        QFileInfo dayWeatherIconFile(dayWeatherIconPath);
        QFileInfo nightWeatherIconFile(nightWeatherIconPath);
        if (dayWeatherIconFile.exists()) {
            weatherIconLabelArr[i]->setStyleSheet("QLabel{border-image:url(" + dayWeatherIconPath + ");background-color:transparent}");
            if (i == 0) {
                currentWeatherIcon->setStyleSheet("QLabel{border-image:url(" + dayWeatherIconPath + ");background-color:transparent}");
            }
        } else if (nightWeatherIconFile.exists()) {
            weatherIconLabelArr[i]->setStyleSheet("QLabel{border-image:url(" + nightWeatherIconPath + ");background-color:transparent}");
            if (i == 0) {
                currentWeatherIcon->setStyleSheet("QLabel{border-image:url(" + nightWeatherIconPath + ");background-color:transparent}");
            }
        } else {
            weatherIconLabelArr[i]->setStyleSheet("QLabel{border-image:url(./pic/未定义.png);background-color:transparent}");
        }
    }
}
