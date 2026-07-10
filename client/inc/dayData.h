#ifndef DAY_DATA_H
#define DAY_DATA_H

#include <string>
#include <chrono>

#include <map>
// TODO: content will (may) be replaced with smth more complex

                        // peer               // timestamp                         // content
#define REGISTER_STATE std::tuple<u_int16_t, std::chrono::_V2::system_clock::time_point, std::string>

// CRDT LWW //
class Register_DayData{
public:
    Register_DayData();
    Register_DayData(REGISTER_STATE state);

    void         setContent(std::string content);
    std::string  getContent();

          REGISTER_STATE getState();

    void         merge(REGISTER_STATE new_state);
private:
    REGISTER_STATE state;
};

#define DAY_VALUE_MAP    std::map<u_int32_t, std::string>
#define DAY_STATE_MAP std::map<u_int32_t, Register_DayData>


class Map_DayData{
public:
    Map_DayData();
    Map_DayData(DAY_STATE_MAP state);

    DAY_VALUE_MAP  getValue();
    DAY_STATE_MAP  getState();

    bool has(u_int32_t t);
    void set(u_int32_t t, std::string content);

    std::string get(u_int32_t t);

    void merge(DAY_STATE_MAP new_state);

    void deleteDay(u_int32_t day);

private:
    DAY_STATE_MAP data;
};


#endif
