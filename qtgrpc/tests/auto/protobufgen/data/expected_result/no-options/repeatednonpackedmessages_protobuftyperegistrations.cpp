#include <QtProtobuf/qprotobufserializer.h>
#include "repeatednonpackedmessages.qpb.h"

namespace qtprotobufnamespace::tests {
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedNonPackedIntMessage(qRegisterProtobufType<RepeatedNonPackedIntMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedNonPackedSIntMessage(qRegisterProtobufType<RepeatedNonPackedSIntMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedNonPackedUIntMessage(qRegisterProtobufType<RepeatedNonPackedUIntMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedNonPackedInt64Message(qRegisterProtobufType<RepeatedNonPackedInt64Message>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedNonPackedSInt64Message(qRegisterProtobufType<RepeatedNonPackedSInt64Message>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedNonPackedUInt64Message(qRegisterProtobufType<RepeatedNonPackedUInt64Message>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedNonPackedFixedIntMessage(qRegisterProtobufType<RepeatedNonPackedFixedIntMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedNonPackedSFixedIntMessage(qRegisterProtobufType<RepeatedNonPackedSFixedIntMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedNonPackedFixedInt64Message(qRegisterProtobufType<RepeatedNonPackedFixedInt64Message>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedNonPackedSFixedInt64Message(qRegisterProtobufType<RepeatedNonPackedSFixedInt64Message>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedNonPackedBoolMessage(qRegisterProtobufType<RepeatedNonPackedBoolMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedNonPackedDoubleMessage(qRegisterProtobufType<RepeatedNonPackedDoubleMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarRepeatedNonPackedFloatMessage(qRegisterProtobufType<RepeatedNonPackedFloatMessage>);
static QtProtobuf::ProtoTypeRegistrar ProtoTypeRegistrarNonPackedIntMessageWithExtraMember(qRegisterProtobufType<NonPackedIntMessageWithExtraMember>);
static bool RegisterRepeatednonpackedmessagesProtobufTypes = [](){ qRegisterProtobufTypes(); return true; }();
} // namespace qtprotobufnamespace::tests

