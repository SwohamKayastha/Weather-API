#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


/*#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <curl/curl.h>

#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fetchContent();
}

MainWindow::~MainWindow()
{
    delete ui;
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void MainWindow::fetchContent()
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    std::string api_key;
    std::string city_name;
    api_key = "c75997502cfcd1b939260acf6e491ec4";
    QString jsonString;
    curl = curl_easy_init();
    if(curl) {
        // Set URL for GET request
        curl_easy_setopt(curl, CURLOPT_URL, "http://api.openweathermap.org/geo/1.0/direct?q=Kathmandu&limit=5&appid=c75997502cfcd1b939260acf6e491ec4");

        // Set callback function to handle response data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, jsonString );

        // Perform the request
        res = curl_easy_perform(curl);
        cout<<"\n"<<readBuffer;
        cout<<"\nLoops:"<<endl;

        for(int i=0;i<10;i++)
        {
            cout<<readBuffer<<endl;
        }
        // Check for errors
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // Update the QTextEdit with the fetched content
            ui->textEdit->setText(QString::fromStdString(readBuffer));
            ui->Lat->setText(QString::fromStdString(readBuffer));
        }

        // Cleanup
        curl_easy_cleanup(curl);
    }
}
*/
