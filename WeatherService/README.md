# WeatherService
天气服务程序

包括天气数据采集程序和天气服务的API程序。


sudo apt-get install libmysqlclient-dev 

sudo apt-get install libcurl4-openssl-dev


CREATE USER 'weather_view'@'%' IDENTIFIED BY 'weather_view';

grant select ON DB_WEATHER_MASTER.* to 'weather_view'@'%';