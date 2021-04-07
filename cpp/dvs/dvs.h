#ifndef DVS_H_
#define DVS_H_

#include "low_level_com.h"
#include "math/math.h"
#include "function_header.h"

namespace dvs
{

/*
In the header, there will be a series of bytes specifying different attributes
[attribute_type|num_bytes|<data bytes>] [attribute_type|num_bytes|<data bytes>]

*/

/*template <typename Wx, typename Wy, typename... Us>
void plot(const Vector<Wx> x, const Vector<Wy> y, const Us&... settings)
{
    (void)x;
    (void)y;
    

    plot(x, y, Color('r'), LineWidth(1.3f), Attribute::PERSISTENT)
    FunctionHeader hdr;
    hdr.append(HeaderAttributeType::FUNCTION, Function::PLOT2);
    hdr.append(HeaderAttributeType::DATA_STRUCTURE, DataStructure::VECTOR);
    hdr.append(HeaderAttributeType::DATA_TYPE, typeToDataTypeEnum<double>());
    hdr.append(HeaderAttributeType::NUM_BUFFERS_REQUIRED, static_cast<char>(2));
    hdr.append(HeaderAttributeType::ATTRIBUTE, )

    sendData(x, y, hdr);
    sendData(x, y, z, hdr);
    sendData(values, hdr);
    sendData(text_data, hdr);

    use pushToCore instead of sendData?
    

}*/

/*template <typename... Ts>
void sendDataInternal(const Ts&... data_to_be_sent)
{

}

template <typename... Ts>
void sendData(const Ts&... data_to_be_sent)
{

}*/

// template <typename T, typename... Us>
// void rtPlot(const T timestamp, const T value, const uint8_t plot_id, const uint8_t line_id, const Us&... settings)

/*template <typename T, typename... Us>
void rtPlot(const T value, const uint8_t plot_id, const uint8_t line_id, const Us&... settings)
{

    // std::vector<Attribute> attributes;
    // CommandType command_type = CommandType::RT_PLOT;
    (void)value;
    (void)plot_id;
    (void)line_id;

}*/

}


#endif