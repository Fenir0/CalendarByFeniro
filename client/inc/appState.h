#ifndef APP_STATE_H
#define APP_STATE_H

#include <QObject>
#include <QtQml/QQmlEngine> 
#include <qqml.h>

#include "date.h"
using date::YEAR_MONTH_DAY;

// =================================
// CURRENT APP STATE (+ USER AND FILE)
// =================================

enum AccessLevel {
    READ,
    EDIT
};

class AppState: public QObject{
    Q_OBJECT
    QML_ELEMENT
    
    Q_PROPERTY(bool    loggedIn     READ isLoggetIn WRITE setLoggedIn   NOTIFY userChanged)
    Q_PROPERTY(bool   documentSaved READ isSaved   WRITE setSaved       NOTIFY userChanged)

    Q_PROPERTY(QString username     READ getUsername WRITE setUsername  NOTIFY userChanged)
    Q_PROPERTY(uint32_t userId       READ getUserId   WRITE setUserId    NOTIFY userChanged)
    Q_PROPERTY(uint32_t documentId   READ getDocumentId WRITE setDocumentId  NOTIFY userChanged)
    Q_PROPERTY(QString documentName READ getDocumentName WRITE setDocumentName  NOTIFY userChanged)
    Q_PROPERTY(AccessLevel         accessLevel  READ getAccessLevel WRITE setAccessLevel  NOTIFY parameterChanged)

    Q_PROPERTY(int dayAmountPrevious READ getDayAmountPrevious   NOTIFY parameterChanged)
    Q_PROPERTY(int dayAmountCurrent READ getDayAmountCurrent   NOTIFY parameterChanged)
    Q_PROPERTY(YEAR_MONTH_DAY currentDate NOTIFY parameterChanged)    

    Q_PROPERTY(QString visibleMonthString READ getVisibleMonthString NOTIFY parameterChanged)

    Q_PROPERTY(int visibleMonth READ getVisibleMonth WRITE setVisibleMonth NOTIFY parameterChanged)
    Q_PROPERTY(int visibleYear READ getVisibleYear    NOTIFY parameterChanged)

        
    Q_PROPERTY(int previousMonth READ getPreviousMonth    NOTIFY parameterChanged)
    Q_PROPERTY(int previousYear READ getPreviousYear    NOTIFY parameterChanged)

    Q_PROPERTY(int nextMonth READ getNextMonth    NOTIFY parameterChanged)
    Q_PROPERTY(int nextYear  READ getNextYear    NOTIFY parameterChanged)

    Q_PROPERTY(int highlightedDay READ getHighlightedDay WRITE setHighlightedDay NOTIFY parameterChanged)
    public:
    static AppState& instance();

    void setDate(YEAR_MONTH_DAY new_date);
    int getDateAsYM() const;

    int  getVisibleYear()const;
    void setVisibleYear(int year);

    int  getVisibleMonth()const;
    void setVisibleMonth(int month);
    
    int  getHighlightedDay()  const;
    void setHighlightedDay(int day);

    int  getPreviousMonth()  const;
    int  getPreviousYear()  const;

    int  getNextMonth()  const;
    int  getNextYear()  const;

    int  getDayAmountCurrent()  const;
    int  getDayAmountPrevious() const;

    bool isSaved() const;
    void setSaved(bool newSaved);

    bool isLoggetIn() const;
    void setLoggedIn(bool newloggedIn);

    QString getUsername() const;
    void setUsername(QString newUserName);

    uint32_t getUserId() const;
    void setUserId(uint32_t id);

    QString getDocumentName() const;
    void setDocumentName(QString newDocumentName);

    uint32_t getDocumentId() const;
    void setDocumentId(uint32_t id);

    AccessLevel getAccessLevel() const;
    void setAccessLevel(AccessLevel newAccessLevel);

    QString getVisibleMonthString() const;

    Q_INVOKABLE uint32_t getWeekDay(quint32 day) const;

signals:
    void parameterChanged();
    void userChanged();

private:
    int dayAmountPrevious;

    YEAR_MONTH_DAY visibleDate;
    int dayAmountCurrent;

    int highlightedDay = -1;


    bool saved = false;
    bool loggedIn = false;
    QString username = "";
    uint32_t user_id = 0;

    QString documentName = "";
    uint32_t document_id = 0;

    AccessLevel accessLevel = EDIT;
};

#endif