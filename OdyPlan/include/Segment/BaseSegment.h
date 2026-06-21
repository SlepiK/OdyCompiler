#ifndef SIERRA_ODYPLAN_V1_SEGMENT_BASESEGMENT_H
#define SIERRA_ODYPLAN_V1_SEGMENT_BASESEGMENT_H

#include "ISegment.h"

namespace Sierra::OdyPlan::v1::Segment {
    class BaseSegment : public ISegment {
    public:
        BaseSegment();
        ~BaseSegment() override = default;
    };
} // Sierra::OdyPlan::v1::Segment

#endif //SIERRA_ODYPLAN_V1_SEGMENT_BASESEGMENT_H