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
 * type
 */
template <typename OUT_TYPE, typename PORT_TYPE, typename IN_TYPE>
void FEI_FileReader_i::convertAndCopyPacket(PORT_TYPE *port,
        std::vector<IN_TYPE> &inData, std::vector<OUT_TYPE> &outData)
{
    if (port->isActive()) {
        if (typeid(OUT_TYPE) != typeid(IN_TYPE)) {
            outData.assign(inData.begin(), inData.end());
        }
    }
}

/*
 * Check if a port is active and if it is push the packet to the port
 */
template <typename PORT_TYPE, typename IN_TYPE>
void FEI_FileReader_i::pushPacketIfActive(PORT_TYPE *port,
        const IN_TYPE *data, size_t dataSize, BULKIO::PrecisionUTCTime &T,
        bool EOS, const std::string &streamID)
{
    if (port->isActive()) {
        port->pushPacket(data, dataSize, T, EOS, streamID);
    }
}

#endif /* FEI_FILEREADER_TEMPLATE_H_ */
