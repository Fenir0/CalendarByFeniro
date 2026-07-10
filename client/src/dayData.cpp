#include "../inc/dayData.h"

Register_DayData::Register_DayData(){}


Register_DayData::Register_DayData(REGISTER_STATE state)
:state(state){}

void Register_DayData::setContent(std::string content)
{std::get<2>(this->state) = content;}

std::string Register_DayData::getContent()
{
    return std::get<2>(this->state);
}

REGISTER_STATE Register_DayData::getState()
{
    return state;
}

void Register_DayData::merge(REGISTER_STATE new_state)
{
    if(std::get<1>(this->state) > std::get<1>(new_state)){
        return;
    }
    if((std::get<1>(this->state) == std::get<1>(new_state)) && std::get<0>(this->state) > std::get<0>(new_state)){
        return;
    }
}


//              -----

Map_DayData::Map_DayData()
{
}

Map_DayData::Map_DayData(DAY_STATE_MAP state)
{
    for(auto i: state){
        this->data[i.first] = Register_DayData(i.second.getState());
    }
}

DAY_VALUE_MAP Map_DayData::getValue()
{
    DAY_VALUE_MAP value;
    for(auto i: data){
        if(!i.second.getContent().empty()){
            value[i.first] = i.second.getContent();
        }
    }
    return value;
}

DAY_STATE_MAP Map_DayData::getState()
{
    DAY_STATE_MAP state;
    for(auto i: data){
        state[i.first] = i.second.getState(); 
    }
    return state;
}

bool Map_DayData::has(u_int32_t t){
    return this->data.find(t) != this->data.end();
}

void Map_DayData::set(u_int32_t t, std::string content){
    this->data[t].setContent(content);
}

std::string Map_DayData::get(u_int32_t t)
{
    return this->data[t].getContent();
}

void Map_DayData::merge(DAY_STATE_MAP new_state){
    for(auto i: new_state){
        if(this->data.find(i.first) != this->data.end())
             this->data[i.first].merge(i.second.getState());
        else this->data[i.first] = Register_DayData(i.second.getState());
    }
}

void Map_DayData::deleteDay(u_int32_t day){
    this->data.erase(day);
}
