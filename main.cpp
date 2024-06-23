#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <curl/curl.h>
#include <iostream>

// Function to write curl data into a QByteArray
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((QByteArray*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Initialize libcurl
    CURL* curl;
    CURLcode res;
    QByteArray responseData;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        // Replace with a known working URL
        curl_easy_setopt(curl, CURLOPT_URL, "http://api.openweathermap.org/geo/1.0/direct?q=Kathmandu&limit=5&appid=c75997502cfcd1b939260acf6e491ec4");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            qDebug() << "curl_easy_perform() failed:" << curl_easy_strerror(res);
        else
            qDebug() << "Data fetched successfully.";

        curl_easy_cleanup(curl);
    } else {
        qDebug() << "Failed to initialize curl.";
    }
    curl_global_cleanup();

    // Check if data was fetched
    if(responseData.isEmpty()) {
        qDebug() << "No data fetched.";
        return -1;
    }

    qDebug() << "Raw data fetched:" << responseData;
    qDebug() << "\n";

    // Trim any leading/trailing whitespace
    responseData = responseData.trimmed();

    // Parse the JSON data
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Error parsing JSON:" << error.errorString();
        return -1;
    }


    if (jsonDoc.isArray()) {
        QJsonArray jsonArray = jsonDoc.array();
        qDebug() << "JSON Array:" << jsonArray;
        qDebug() << "\n";
        // Iterate through array and print each object
        for (int i = 0; i < jsonArray.size(); ++i) {
            QJsonObject obj = jsonArray[i].toObject();
            qDebug() << "Object at index" << i << ":" << obj;
            qDebug() << "\n";
            // Access specific data
            if (obj.contains("name")) {
                QJsonValue value = obj.value("name");
                qDebug() << "Value for 'name':" << i << " " << value.toString();
            }
            qDebug() << "\n";
            if (obj.contains("lat")) {
                QJsonValue latvalue = obj.value("lat");
                double lat = latvalue.toDouble();
                qDebug() << "Value for 'lat':" << QString::number(lat, 'f', 8);
            }
            qDebug() << "\n";
            if(obj.contains("lon")){
                QJsonValue lonvalue = obj.value("lon");
                qDebug() << "lon:" << lonvalue.toDouble();
            }
        }
    }
    else {
        qDebug() << "Failed to create JSON doc.";
    }

    return a.exec();
}
