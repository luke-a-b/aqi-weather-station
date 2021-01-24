#ifndef _FETCHER_H
#define _FETCHER_H

#include "Config.h"
#include "Models/DisplayModel.h"
#include "DataSources/LocalTempSensorSource.h"
#include "Models/ModelChangeObserver.h"
#include "DataSources/AqiDataSource.h"

#define TIME_UPDATE_INTERVAL 3600L // 60*60 -> update every hour

class Fetcher {
    public:
        Fetcher(Config *config, DisplayModel* displayModel, ModelChangeObserver* modelChangeObserver);
        ~Fetcher();
        void updateAll();
        void update(uint32_t now);

    private:
        void updateAqiData(uint8_t station);
        void updateOwmData();
        bool updateTimeData();
        void updateSensorsData();
        long lastAqiUpdate = 0;
        uint8_t lastAqiStation = 0;
        long lastWeatherUpdate = 0;
        long lastSensorUpdate = 0;
        long lastTimeUpdate = 0;
        Config* config;
        DisplayModel* displayModel;
        LocalTempSensorSource* localTemSensorSource = nullptr;
        ModelChangeObserver* modelChangeObserver;
        AqiDataSource currentAqiClient;
};

#endif