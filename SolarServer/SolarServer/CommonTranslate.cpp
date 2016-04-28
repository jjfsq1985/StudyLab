#include "stdafx.h"
#include "CommonTranslate.h"

#include <string>
using namespace std;


CommonTranslate::CommonTranslate()
{
}


CommonTranslate::~CommonTranslate()
{
}

QString CommonTranslate::GetDataType(LONG dataType)
{
    //等价于VARENUM
    switch ((VARENUM)dataType)
    {
    case VARENUM::VT_I1:                         return "char";
    case VARENUM::VT_UI1:                        return  "unsigned char";
    case VARENUM::VT_I2:                         return "short";
    case VARENUM::VT_UI2:                        return "unsigned short";
    case VARENUM::VT_I4:                         return "int32";
    case VARENUM::VT_UI4:                        return "unsigned int32";
    case VARENUM::VT_R4:                         return "float";
    case VARENUM::VT_R8:                         return "double";
    case VARENUM::VT_CY:                         return  "decimal";
    case VARENUM::VT_BOOL:                       return "bool";
    case VARENUM::VT_DATE:                       return "DateTime";
    case VARENUM::VT_BSTR:                       return "string";
    case VARENUM::VT_ARRAY | VARENUM::VT_I1:      return "sbyte[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_UI1:     return "byte[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_I2:      return "short[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_UI2:     return "ushort[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_I4:      return "int[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_UI4:     return "uint[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_R4:      return "float[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_R8:      return "double[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_CY:      return "decimal[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_BOOL:    return "bool[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_DATE:    return "DateTime[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_BSTR:    return "string[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_VARIANT: return "object[]";
    default:   return "object";
    }
}

QString CommonTranslate::GetDataValue(VARIANT varValue)
{
    switch (varValue.vt)
    {
    case VARENUM::VT_I1:                         return QString("%1").arg(varValue.cVal);
    case VARENUM::VT_UI1:                        return   QString("%1").arg(varValue.bVal);
    case VARENUM::VT_I2:                         return QString("%1").arg(varValue.iVal);
    case VARENUM::VT_UI2:                        return QString("%1").arg(varValue.uiVal);
    case VARENUM::VT_I4:                         return QString("%1").arg(varValue.intVal);
    case VARENUM::VT_UI4:                        return QString("%1").arg(varValue.uintVal);
    case VARENUM::VT_R4:                         return QString("%1").arg(varValue.fltVal);
    case VARENUM::VT_R8:                         return QString("%1").arg(varValue.dblVal);
    case VARENUM::VT_CY:                         return  QString("%1").arg((qlonglong)varValue.cyVal.int64);
    case VARENUM::VT_BOOL:                       return QString("%1").arg(varValue.boolVal);
    case VARENUM::VT_DATE:
    {
        SYSTEMTIME systime;
        VariantTimeToSystemTime(varValue.date, &systime);
        return GetTimeStamp(systime);
        QDate dateval(systime.wYear, systime.wMonth, systime.wDay);
        QTime timeval(systime.wHour, systime.wMinute, systime.wSecond);
        QDateTime timeVal(dateval, timeval);
        return timeVal.toString("yyyy-MM-dd HH:mm:ss");
    }
    case VARENUM::VT_BSTR:
    {
        wstring strVal = varValue.bstrVal;
        return QString::fromStdWString(strVal);
    }
    case VARENUM::VT_ARRAY | VARENUM::VT_I1:
    {
        LONG uBound, lBound;
        ::SafeArrayGetUBound(varValue.parray, 1, &uBound);
        ::SafeArrayGetLBound(varValue.parray, 1, &lBound);//OPC lBound从1开始
        CHAR *cVal = NULL;
        QString strAllVal = "";
        for (LONG i = lBound; i < uBound; i++)
        {
            SafeArrayGetElement(varValue.parray, &i, (void*)&cVal);
            strAllVal += QString("%1;").arg(*cVal);
        }
        SafeArrayGetElement(varValue.parray, &uBound, (void*)&cVal);
        strAllVal += QString("%1").arg(*cVal);
        return strAllVal;
    }
    case VARENUM::VT_ARRAY | VARENUM::VT_UI1:
    {
        LONG uBound, lBound;
        ::SafeArrayGetUBound(varValue.parray, 1, &uBound);
        ::SafeArrayGetLBound(varValue.parray, 1, &lBound);//OPC lBound从1开始
        BYTE *bVal = NULL;
        QString strAllVal = "";
        for (LONG i = lBound; i < uBound; i++)
        {
            SafeArrayGetElement(varValue.parray, &i, (void*)&bVal);
            strAllVal += QString("%1;").arg(*bVal);
        }
        SafeArrayGetElement(varValue.parray, &uBound, (void*)&bVal);
        strAllVal += QString("%1").arg(*bVal);
        return strAllVal;
    }
    case VARENUM::VT_ARRAY | VARENUM::VT_I2:
    {
        LONG uBound, lBound;
        ::SafeArrayGetUBound(varValue.parray, 1, &uBound);
        ::SafeArrayGetLBound(varValue.parray, 1, &lBound);//OPC lBound从1开始
        short *shortVal = NULL;
        QString strAllVal = "";
        for (LONG i = lBound; i < uBound; i++)
        {
            SafeArrayGetElement(varValue.parray, &i, (void*)&shortVal);
            strAllVal += QString("%1;").arg(*shortVal);
        }
        SafeArrayGetElement(varValue.parray, &uBound, (void*)&shortVal);
        strAllVal += QString("%1").arg(*shortVal);
        return strAllVal;
    }
    case VARENUM::VT_ARRAY | VARENUM::VT_UI2:
    {
        LONG uBound, lBound;
        ::SafeArrayGetUBound(varValue.parray, 1, &uBound);
        ::SafeArrayGetLBound(varValue.parray, 1, &lBound);//OPC lBound从1开始
        ushort *ushortVal = NULL;
        QString strAllVal = "";
        for (LONG i = lBound; i < uBound; i++)
        {
            SafeArrayGetElement(varValue.parray, &i, (void*)&ushortVal);
            strAllVal += QString("%1;").arg(*ushortVal);
        }
        SafeArrayGetElement(varValue.parray, &uBound, (void*)&ushortVal);
        strAllVal += QString("%1").arg(*ushortVal);
        return strAllVal;
    }
    case VARENUM::VT_ARRAY | VARENUM::VT_I4:
    {
        LONG uBound, lBound;
        ::SafeArrayGetUBound(varValue.parray, 1, &uBound);
        ::SafeArrayGetLBound(varValue.parray, 1, &lBound);//OPC lBound从1开始
        int *intVal = NULL;
        QString strAllVal = "";
        for (LONG i = lBound; i < uBound; i++)
        {
            SafeArrayGetElement(varValue.parray, &i, (void*)&intVal);
            strAllVal += QString("%1;").arg(*intVal);
        }
        SafeArrayGetElement(varValue.parray, &uBound, (void*)&intVal);
        strAllVal += QString("%1").arg(*intVal);
        return strAllVal;
    }
    case VARENUM::VT_ARRAY | VARENUM::VT_UI4:
    {
        LONG uBound, lBound;
        ::SafeArrayGetUBound(varValue.parray, 1, &uBound);
        ::SafeArrayGetLBound(varValue.parray, 1, &lBound);//OPC lBound从1开始
        unsigned int *uintVal = NULL;
        QString strAllVal = "";
        for (LONG i = lBound; i < uBound; i++)
        {
            SafeArrayGetElement(varValue.parray, &i, (void*)&uintVal);
            strAllVal += QString("%1;").arg(*uintVal);
        }
        SafeArrayGetElement(varValue.parray, &uBound, (void*)&uintVal);
        strAllVal += QString("%1").arg(*uintVal);
        return strAllVal;
    }
    case VARENUM::VT_ARRAY | VARENUM::VT_R4:
    {
        LONG uBound, lBound;
        ::SafeArrayGetUBound(varValue.parray, 1, &uBound);
        ::SafeArrayGetLBound(varValue.parray, 1, &lBound);//OPC lBound从1开始
        float *fltVal = NULL;
        QString strAllVal = "";
        for (LONG i = lBound; i < uBound; i++)
        {
            SafeArrayGetElement(varValue.parray, &i, (void*)&fltVal);
            strAllVal += QString("%1;").arg(*fltVal);
        }
        SafeArrayGetElement(varValue.parray, &uBound, (void*)&fltVal);
        strAllVal += QString("%1").arg(*fltVal);
        return strAllVal;
    }
    case VARENUM::VT_ARRAY | VARENUM::VT_R8:
    {
        LONG uBound, lBound;
        ::SafeArrayGetUBound(varValue.parray, 1, &uBound);
        ::SafeArrayGetLBound(varValue.parray, 1, &lBound);//OPC lBound从1开始
        double *dblVal = NULL;
        QString strAllVal = "";
        for (LONG i = lBound; i < uBound; i++)
        {
            SafeArrayGetElement(varValue.parray, &i, (void*)&dblVal);
            strAllVal += QString("%1;").arg(*dblVal);
        }
        SafeArrayGetElement(varValue.parray, &uBound, (void*)&dblVal);
        strAllVal += QString("%1").arg(*dblVal);
        return strAllVal;
    }
    case VARENUM::VT_ARRAY | VARENUM::VT_CY: return "decimal[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_BOOL: return "VARIANT_BOOL[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_DATE: return "DATE[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_BSTR: return "BSTR[]";
    case VARENUM::VT_ARRAY | VARENUM::VT_VARIANT: return "Variant[]";
    default:   return "Empty";
    }
}

QString CommonTranslate::GetTimeStamp(SYSTEMTIME sysTime)
{
    QDate dateval(sysTime.wYear, sysTime.wMonth, sysTime.wDay);
    QTime timeval(sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
    QDateTime timeVal(dateval, timeval);
    timeVal.setTimeSpec(Qt::OffsetFromUTC);
    return timeVal.toLocalTime().toString("yyyy-MM-dd HH:mm:ss");
}