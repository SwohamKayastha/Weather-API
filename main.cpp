#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <curl/curl.h>
#include <iostream>

QString lat(QJsonDocument);
int parse_json(QByteArray);

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
    char url[150] = "http://api.openweathermap.org/geo/1.0/direct?q=Kathmandu&limit=5&appid=";
    CURL* curl;
    CURLcode res;
    QByteArray responseData;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        // Replace  with a known working URL
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) // error checking
            qDebug() << "curl_easy_perform() failed:" << curl_easy_strerror(res);
        else
            qDebug() << "Data fetched successfully.";

        curl_easy_cleanup(curl);
    } else {
        qDebug() << "Failed to initialize curl."; //error checking
    }
    curl_global_cleanup();

    // Check if data was fetched
    if(responseData.isEmpty()) {
        qDebug() << "No data fetched.";
        return -1;
    }

    // qDebug() << "Raw data fetched:" << responseData;
    // qDebug() << "\n";

    // Trim any leading/trailing whitespace
    responseData = responseData.trimmed();

    //Function Call
    int error = parse_json(responseData);

    if(error == -1)
    {
        return 0;
    }

    return a.exec();
}



QString lat(QJsonDocument jsonDoc) // get's latitude
{
    if (jsonDoc.isArray()) {
        QJsonArray jsonArray = jsonDoc.array();

        //Parsing the JSON to Object
        QJsonObject obj = jsonArray[0].toObject();
        if(obj.contains("lat")) //finding the latitude of the given city name
        {
            QJsonValue latvalue = obj.value("lat");
            double lat = latvalue.toDouble();
            qDebug() << "lat:" << QString::number(lat, 'f', 8);
            QString lats =  QString::number(lat,'f',8);
            qDebug() << lats;
        }
        qDebug() << "\n";
    }
    else {
        qDebug() << "Failed to create JSON doc.";
    }
    return "1";
}

int parse_json(QByteArray responseData)
{

    // Parse the JSON data
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Error parsing JSON:" << error.errorString();
        return -1;
    }

    lat(jsonDoc);


    return 1;
}
