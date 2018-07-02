TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    appimp.cpp \
    cJSON.cpp \
    ckafkaproducer.cpp \
    recssl.cpp \
    topkg.cpp \
    tinyxml2.cpp \
    configpara.cpp \

INCLUDEPATH +=/usr/local/include/
INCLUDEPATH +=/opt/instantclient_11_2/sdk/include

LIBS +=-L../lib
LIBS += -D_REENTRANT  -fpermissive -lstdc++ -lzlog -lcrypto -lrdkafka -lzmq -lczmq
LIBS +=-L/usr/local/lib/ -levpp -levpp_boost

HEADERS += \
    winmain-inl.h \
    appimp.h \
    ckafkaproducer.h \
    recssl.h \
    topkg.h \
    tinyxml2.h \
    configpara.h \

DISTFILES += \
    rec.json \
    RecData_log.conf \
    RecData.log \
    Makefile \
    ../conf/aoptions.xml
