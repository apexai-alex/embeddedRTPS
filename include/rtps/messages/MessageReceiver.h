/*
 *
 * Author: Andreas Wüstenberg (andreas.wuestenberg@rwth-aachen.de)
 */

#ifndef RTPS_MESSAGERECEIVER_H
#define RTPS_MESSAGERECEIVER_H

#include <cstdint>
#include "rtps/common/types.h"
#include "rtps/config.h"
#include "rtps/discovery/BuiltInEndpoints.h"

namespace rtps {
    class Reader;
    class Writer;

    class MessageReceiver {
    public:
        GuidPrefix_t sourceGuidPrefix = GUIDPREFIX_UNKNOWN;
        ProtocolVersion_t sourceVersion = PROTOCOLVERSION;
        VendorId_t sourceVendor = VENDOR_UNKNOWN;
        bool haveTimeStamp = false;

        explicit MessageReceiver(GuidPrefix_t partGuid);

        void reset();

        bool addReader(Reader* reader); // for new CacheChanges
        bool addWriter(Writer* writer); // for acks etc.
        void addBuiltInEndpoints(BuiltInEndpoints& endpoints);

        bool processMessage(const uint8_t* data, DataSize_t size);

    private:
        struct MessageProcessingInfo{
            MessageProcessingInfo(const uint8_t* data, DataSize_t size)
                : data(data), size(size){}
            const uint8_t* data;
            const DataSize_t size;

            //! Offset to the next unprocessed byte
            DataSize_t nextPos = 0;

            inline const uint8_t* getPointerToPos(){
                return &data[nextPos];
            }

            //! Returns the size of data which isn't processed yet
            inline DataSize_t getRemainingSize(){
                return size - nextPos;
            }
        };

        std::array<Writer*, Config::NUM_WRITERS_PER_PARTICIPANT> m_writers{nullptr};
        uint8_t m_numWriters = 0;
        std::array<Reader*, Config::NUM_READERS_PER_PARTICIPANT> m_readers{nullptr};
        uint8_t m_numReaders = 0;

        GuidPrefix_t ourGuid;

        // TODO make msgInfo a member
        // This probably make processing faster, as no parameter needs to be passed around
        // However, we need to make sure data is set to nullptr after processMsg to make sure
        // we don't access it again afterwards.
        /**
         * Check header for validity, modifys the state of the receiver and
         * adjusts the position of msgInfo accordingly
         */
        bool processHeader(MessageProcessingInfo& msgInfo);
        bool processSubMessage(MessageProcessingInfo& msgInfo);
        bool processDataSubmessage(MessageProcessingInfo &msgInfo);
        bool processHeartbeatSubmessage(MessageProcessingInfo &msgInfo);
    };
}

#endif //RTPS_MESSAGERECEIVER_H
