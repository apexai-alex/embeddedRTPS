/*
 *
 * Author: Andreas Wüstenberg (andreas.wuestenberg@rwth-aachen.de)
 */

#ifndef RTPS_WRITERMOCK_H
#define RTPS_WRITERMOCK_H

#include <gmock/gmock.h>

#include "rtps/entities/Writer.h"
#include "rtps/storages/HistoryCache.h"

class WriterMock : public rtps::Writer{
public:
    MOCK_METHOD1(addNewMatchedReader, bool(rtps::ReaderLocator));
    MOCK_METHOD0(progress, void());
    MOCK_METHOD3(newChange, const rtps::CacheChange*(rtps::ChangeKind_t, const uint8_t*, rtps::DataSize_t));
    MOCK_METHOD0(unsentChangesReset, void());
};
#endif //RTPS_WRITERMOCK_H
