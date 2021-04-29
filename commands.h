

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual ~DefaultIO() {}

    // you may add additional methods here
};

// you may add here helper classes


// you may edit this class
class Data {
public:
    ofstream train;
    ofstream test;
    int size = 0;
    float threshold = 0.9;
    vector<AnomalyReport> reports;

    Data() {
        train.open("anomalyTrain.csv");
        test.open("anomalyTest.csv");
    }
};

class Command {
protected:
    DefaultIO *dio;
    Data *data;
    string description;
public:
    explicit Command(DefaultIO *dio, Data *data) : dio(dio), data(data) {
    }

    virtual void execute() = 0;

    virtual ~Command() {}

    virtual void display() {
        dio->write(description);
    }
};

class UploadCommand : public Command {
public:

    ~UploadCommand() override {
    }

    explicit UploadCommand(DefaultIO *dio, Data *data) : Command(dio, data) {
        description = "1.upload a time series csv file\n";
    }

    void execute() override {
        dio->write("Please upload your local train CSV file.\n");
        while (true) {
            string row = dio->read();
            if (row == "done") {
                break;
            }
            data->train << row << endl;
        }
        data->train.close();
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        while (true) {
            string row = dio->read();
            if (row == "done") {
                break;
            }
            data->test << row << endl;
            data->size++;
        }
        data->size--;
        dio->write("Upload complete.\n");
    }

    void display() override {
        dio->write(description);
    }

};

class SettingCommand : public Command {
public:
    virtual ~SettingCommand() {

    }

    explicit SettingCommand(DefaultIO *dio, Data *data) : Command(dio, data) {
        description = "2.algorithm settings\n";
    }

    void execute() override {
        data->threshold = floor(1000* data->threshold) / 1000;
        dio->write("The current correlation threshold is ");
        dio->write(data->threshold);
        dio->write("\nType a new threshold\n");
        float new_threshold;
        while (true) {
            dio->read(&new_threshold);
            if ((new_threshold <= 1) && (new_threshold >= 0)) {
                data->threshold = new_threshold;
                return;
            }
            dio->write("please choose a value between 0 and 1.\n");
        }
    }
};

class AnomaliesCommand : public Command {
public:
    virtual ~AnomaliesCommand() {

    }

    explicit AnomaliesCommand(DefaultIO *dio, Data *data) : Command(dio, data) {
        description = "3.detect anomalies\n";
    }

    void execute() override {
        TimeSeries ts("anomalyTrain.csv");
        HybridAnomalyDetector ad;
        ad.setCorrelationThreshold(data->threshold);
        ad.learnNormal(ts);
        TimeSeries ts2("anomalyTest.csv");
        data->reports = ad.detect(ts2);
        dio->write("anomaly detection complete.\n");
    }
};

class ResultsCommand : public Command {
public:
    virtual ~ResultsCommand() {

    }

    explicit ResultsCommand(DefaultIO *dio, Data *data) : Command(dio, data) {
        description = "4.display results\n";
    }

    void execute() override {
        for (int i = 0; i < data->reports.size(); i++) {
            dio->write(data->reports.at(i).timeStep);
            dio->write("\t " + data->reports.at(i).description + "\n");
        }
        dio->write("Done.\n");
    }
};

struct range {
    long begin;
    long end;
    bool flag;
};
class AnalyzeCommand : public Command {
public:
    virtual ~AnalyzeCommand() {

    }

    explicit AnalyzeCommand(DefaultIO *dio, Data *data) : Command(dio, data) {
        description = "5.upload anomalies and analyze results\n";
    }

    void execute() override {
        dio->write("Please upload your local anomalies file.\n");
        vector<range> continReports;
        int size = data->reports.size();
        for (int i = 0; i < size; i++) {
            string des = data->reports.at(i).description;
            long begin = data->reports.at(i).timeStep;
            long end = begin;
            for (i = i + 1; i < size; i++) {
                if ((data->reports.at(i).description != des) || (data->reports.at(i).timeStep != end + 1)) {
                    continReports.emplace_back(range{begin, end, false});
                    i--;
                    break;
                }
                end = data->reports.at(i).timeStep;
            }
            if (i == size) {
                continReports.emplace_back(range{begin, end,false});
            }
        }
        vector<range> continAnom;
        long k=0;
        while (true) {
            string row = dio->read();
            if (row == "done") {
                break;
            }
            int x1 = row.find(',', 0);
            long begin = stoi(row.substr(0, x1));
            long end = stoi(row.substr(x1 + 1, row.size()));
            k += end-begin +1;
            continAnom.emplace_back(range{begin, end,false});

        }
        float tp = 0;
        float fp = 0;
        for (auto & continReport : continReports) {
            for (auto & j : continAnom) {
                long b_1 = continReport.begin;
                long e_1 = continReport.end;
                long b_2 = j.begin;
                long e_2 = j.end;
                if ((b_1 >= b_2) && (b_1 <= e_2)) {
                    tp++;
                    continReport.flag = true;
                    break;
                }
                if ((b_1 < b_2) && (e_1 >= b_2)) {
                    tp++;
                    continReport.flag = true;
                    break;
                }
            }
            if(!continReport.flag) {
                fp++;
            }
        }
        int N = data->size - k;
        float tp_rate = floor(1000 * (tp / continAnom.size())) / 1000;
        float false_alarm = floor(1000 * (fp / N)) / 1000;
        dio->write("Upload complete.\n");
        dio->write("True Positive Rate: ");
        dio->write(tp_rate);
        dio->write("\nFalse Positive Rate: ");
        dio->write(false_alarm);
        dio->write("\n");
    }

};

class MenuCommand : public Command {
    Command *menuCommand[6];
    int size = 6;
public:
    explicit MenuCommand(DefaultIO *dio, Data *data) : Command(dio, data) {
        description = "Welcome to the Anomaly Detection Server.\nPlease choose an option:\n";
        menuCommand[1] = new UploadCommand(dio, data);
        menuCommand[2] = new SettingCommand(dio, data);
        menuCommand[3] = new AnomaliesCommand(dio, data);
        menuCommand[4] = new ResultsCommand(dio, data);
        menuCommand[5] = new AnalyzeCommand(dio, data);
    }

    void execute() override {
        float choice;
        while (true) {
            this->display();
            for (int i = 1; i < 6; ++i) {
                menuCommand[i]->display();
            }
            dio->write("6.exit\n");
            dio->read(&choice);
            if (int(choice) == 6) {
                break;
            }
            menuCommand[int(choice)]->execute();
        }
    }

    ~MenuCommand() {
    }
};




// implement here your command classes


#endif /* COMMANDS_H_ */
