/*
 * FEI_FileReader_template.h
 *
 *  Created on: Nov 13, 2015
 *      Author: Patrick
 */

#ifndef FEI_FILEREADER_TEMPLATE_H_
#define FEI_FILEREADER_TEMPLATE_H_

/*
 * Check if a port is active and if it is, perform a conversion to the correct
 * type and push it to the port
 */
template <typename OUT_TYPE, typename PORT_TYPE, typename IN_TYPE>
void FEI_FileReader_i::convertAndPushPacket(PORT_TYPE *port,
        std::vector<IN_TYPE> &data, BULKIO::PrecisionUTCTime &T,
        bool EOS, const std::string &streamID)
{
    if (port->isActive()) {
        if (typeid(OUT_TYPE) != typeid(IN_TYPE)) {
            std::vector<OUT_TYPE> output(data.begin(), data.end());

            port->pushPacket(output, T, EOS, streamID);
        }
    }
}

#endif /* FEI_FILEREADER_TEMPLATE_H_ */
