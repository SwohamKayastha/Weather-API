#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <curl/curl.h>
#include <string>
#include <cstring>


QByteArray response(char []);
void temperature(QJsonDocument);
void weather(QJsonDocument);
QString lat(QJsonDocument);
QString lon(QJsonDocument);


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

    QByteArray responseData = response(url);

    // Check if data was fetched
    if(responseData.isEmpty()) {
        qDebug() << "No data fetched.";
        return -1;
    }

    // Trim any leading/trailing whitespace
    responseData = responseData.trimmed();

    // Parse the JSON data
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Error parsing JSON:" << error.errorString();
        return -1;
    }

    //Getting Latitude and Longitude
    QString latitude = lat(jsonDoc);
    QString longitude = lon(jsonDoc);

    std::string string_weather_url = "https://api.openweathermap.org/data/2.5/weather?lat="+latitude.toStdString()+"&lon="+longitude.toStdString()+"&appid=c75997502cfcd1b939260acf6e491ec4";
    char char_weather_url[150];
    //Converting std::string to char type
    strcpy(char_weather_url, string_weather_url.c_str());

    //Function Call
    QByteArray responsedata = response(char_weather_url);

    //Trim any leading/trailing whitespace
    responsedata = responsedata.trimmed();

    //Parsing the JSON Data
    QJsonDocument jsonDoc1 = QJsonDocument::fromJson(responsedata, &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Error parsing JSON:" << error.errorString();
        return -1;
    }

    //Function Call
    weather(jsonDoc1);
    temperature(jsonDoc1);

    return a.exec();
}


void temperature(QJsonDocument jsonDoc) //get's temperature info
{
    //Checking for Error
    QJsonParseError jsonError;
    if (jsonError.error != QJsonParseError::NoError) {
        qDebug() << "Error parsing JSON:" << jsonError.errorString();
        return;
    }

    if (!jsonDoc.isObject()) {
        qDebug() << "JSON is not an object.";
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();
    QJsonObject temp = jsonObj["main"].toObject();
    qDebug() << temp;

    double temperature = temp["temp"].toDouble();
    qDebug() << "Temperature: " << temperature;
}


void weather(QJsonDocument jsonDoc) // get's weather info
{
    QJsonParseError jsonError;
    if (jsonError.error != QJsonParseError::NoError) {
        qDebug() << "Error parsing JSON:" << jsonError.errorString();
        return;
    }

    if (!jsonDoc.isObject()) {
        qDebug() << "JSON is not an object.";
        return;
    }

    qDebug()<<jsonDoc<<"\n";
    QJsonObject jsonObj = jsonDoc.object();

    //Exctracting the Weather:
    QJsonArray Weather = jsonObj["weather"].toArray();

    qDebug()<<Weather;

    QJsonObject weather_data = Weather[0].toObject();
    QString description = weather_data["description"].toString();
    qDebug() <<"Description: "<<description << "\n";
    return;
}


QString lat(QJsonDocument jsonDoc) // get's latitude
{
    QString result;
    if (jsonDoc.isArray()) {
        QJsonArray jsonArray = jsonDoc.array();

        //Parsing the JSON to Object
        QJsonObject obj = jsonArray[0].toObject();
        if(obj.contains("lat")) //finding the latitude of the given city name
        {
            QJsonValue latvalue = obj.value("lat");
            double lat = latvalue.toDouble();
            qDebug() << "lat:" << QString::number(lat, 'f', 8);
            result =  QString::number(lat,'f',8);
            qDebug() << result;
        }
        qDebug() << "\n";
    }
    else {
        qDebug() << "Failed to create JSON doc.";
    }
    return result;
}


QString lon(QJsonDocument jsonDoc) // get's longitude
{
    QString result;
    if (jsonDoc.isArray())
    {
        QJsonArray jsonArray = jsonDoc.array();

        //Parsing the JSON to Object
        QJsonObject obj = jsonArray[0].toObject();
        if(obj.contains("lon")) //finding the latitude of the given city name
        {
            QJsonValue lonvalue = obj.value("lon");
            double lon = lonvalue.toDouble();
            qDebug() << "lon:" << QString::number(lon, 'f', 8); // Taking the precision
           result = QString::number(lon, 'f', 8);
            qDebug() << result;
        }
        qDebug()<<"\n";
    }
    else
    {
        qDebug() << "Failed to create JSON doc.";
    }
    return result;
}


QByteArray response(char url[]) //Fetching the API
{
    CURL* curl;
    CURLcode res;
    QByteArray responseData;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        // Replace  with a known working URL
        curl_easy_setopt(curl, CURLOPT_URL, url);                       //sets the URL to fetch
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);   //handle the data received from the request
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        // Provide a CA certificate bundle
        curl_easy_setopt(curl, CURLOPT_CAINFO, "C:/Coding/c++/cacert.pem");

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
    return responseData;
}
