#pragma once
class CommonTranslate
{
public:
    CommonTranslate();
    ~CommonTranslate();

public:
    static QString GetDataType(LONG dataType);
    static QString GetDataValue(VARIANT varValue);
    static QString GetTimeStamp(SYSTEMTIME sysTime);
};

