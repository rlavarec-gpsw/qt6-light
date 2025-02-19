/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#include "fieldindexrange.qpb.h"
#include <QtProtobuf/qprotobufserializer.h>
#include <cmath>

namespace qtprotobufnamespace::tests {

class FieldIndexTest1Message_QtProtobufData : public QSharedData
{
public:
    FieldIndexTest1Message_QtProtobufData()
        : QSharedData(),
          m_testField(0)
    {
    }

    FieldIndexTest1Message_QtProtobufData(const FieldIndexTest1Message_QtProtobufData &other)
        : QSharedData(other),
          m_testField(other.m_testField)
    {
    }

    QtProtobuf::sint32 m_testField;
};

FieldIndexTest1Message::~FieldIndexTest1Message() = default;

static constexpr struct {
    QtProtobufPrivate::QProtobufPropertyOrdering::Data data;
    const std::array<uint, 5> qt_protobuf_FieldIndexTest1Message_uint_data;
    const char qt_protobuf_FieldIndexTest1Message_char_data[60];
} qt_protobuf_FieldIndexTest1Message_metadata {
    // data
    {
        0, /* = version */
        1, /* = num fields */
        2, /* = field number offset */
        3, /* = property index offset */
        4, /* = field flags offset */
        48, /* = message full name length */
    },
    // uint_data
    {
        // JSON name offsets:
        49, /* = testField */
        59, /* = end-of-string-marker */
        // Field numbers:
        31, /* = testField */
        // Property indices:
        0, /* = testField */
        // Field flags:
        QtProtobufPrivate::NoFlags, /* = testField */
    },
    // char_data
    /* metadata char_data: */
    "qtprotobufnamespace.tests.FieldIndexTest1Message\0" /* = full message name */
    /* field char_data: */
    "testField\0"
};

const QtProtobufPrivate::QProtobufPropertyOrdering FieldIndexTest1Message::propertyOrdering = {
    &qt_protobuf_FieldIndexTest1Message_metadata.data
};

void FieldIndexTest1Message::registerTypes()
{
    qRegisterMetaType<FieldIndexTest1Message>();
    qRegisterMetaType<FieldIndexTest1MessageRepeated>();
}

FieldIndexTest1Message::FieldIndexTest1Message()
    : QProtobufMessage(&FieldIndexTest1Message::staticMetaObject),
      dptr(new FieldIndexTest1Message_QtProtobufData)
{
}

FieldIndexTest1Message::FieldIndexTest1Message(const FieldIndexTest1Message &other)
    : QProtobufMessage(other),
      dptr(other.dptr)
{
}
FieldIndexTest1Message &FieldIndexTest1Message::operator =(const FieldIndexTest1Message &other)
{
    QProtobufMessage::operator=(other);
    dptr = other.dptr;
    return *this;
}
FieldIndexTest1Message::FieldIndexTest1Message(FieldIndexTest1Message &&other) noexcept
    : QProtobufMessage(std::move(other)),
      dptr(std::move(other.dptr))
{
}
FieldIndexTest1Message &FieldIndexTest1Message::operator =(FieldIndexTest1Message &&other) noexcept
{
    QProtobufMessage::operator=(std::move(other));
    dptr.swap(other.dptr);
    return *this;
}
bool FieldIndexTest1Message::operator ==(const FieldIndexTest1Message &other) const
{
    return QProtobufMessage::isEqual(*this, other)
        && dptr->m_testField == other.dptr->m_testField;
}

bool FieldIndexTest1Message::operator !=(const FieldIndexTest1Message &other) const
{
    return !this->operator ==(other);
}

QtProtobuf::sint32 FieldIndexTest1Message::testField() const
{
    return dptr->m_testField;
}

void FieldIndexTest1Message::setTestField(const QtProtobuf::sint32 &testField)
{
    if (dptr->m_testField != testField) {
        dptr.detach();
        dptr->m_testField = testField;
    }
}


class FieldIndexTest2Message_QtProtobufData : public QSharedData
{
public:
    FieldIndexTest2Message_QtProtobufData()
        : QSharedData(),
          m_testField(0)
    {
    }

    FieldIndexTest2Message_QtProtobufData(const FieldIndexTest2Message_QtProtobufData &other)
        : QSharedData(other),
          m_testField(other.m_testField)
    {
    }

    QtProtobuf::sint32 m_testField;
};

FieldIndexTest2Message::~FieldIndexTest2Message() = default;

static constexpr struct {
    QtProtobufPrivate::QProtobufPropertyOrdering::Data data;
    const std::array<uint, 5> qt_protobuf_FieldIndexTest2Message_uint_data;
    const char qt_protobuf_FieldIndexTest2Message_char_data[60];
} qt_protobuf_FieldIndexTest2Message_metadata {
    // data
    {
        0, /* = version */
        1, /* = num fields */
        2, /* = field number offset */
        3, /* = property index offset */
        4, /* = field flags offset */
        48, /* = message full name length */
    },
    // uint_data
    {
        // JSON name offsets:
        49, /* = testField */
        59, /* = end-of-string-marker */
        // Field numbers:
        8191, /* = testField */
        // Property indices:
        0, /* = testField */
        // Field flags:
        QtProtobufPrivate::NoFlags, /* = testField */
    },
    // char_data
    /* metadata char_data: */
    "qtprotobufnamespace.tests.FieldIndexTest2Message\0" /* = full message name */
    /* field char_data: */
    "testField\0"
};

const QtProtobufPrivate::QProtobufPropertyOrdering FieldIndexTest2Message::propertyOrdering = {
    &qt_protobuf_FieldIndexTest2Message_metadata.data
};

void FieldIndexTest2Message::registerTypes()
{
    qRegisterMetaType<FieldIndexTest2Message>();
    qRegisterMetaType<FieldIndexTest2MessageRepeated>();
}

FieldIndexTest2Message::FieldIndexTest2Message()
    : QProtobufMessage(&FieldIndexTest2Message::staticMetaObject),
      dptr(new FieldIndexTest2Message_QtProtobufData)
{
}

FieldIndexTest2Message::FieldIndexTest2Message(const FieldIndexTest2Message &other)
    : QProtobufMessage(other),
      dptr(other.dptr)
{
}
FieldIndexTest2Message &FieldIndexTest2Message::operator =(const FieldIndexTest2Message &other)
{
    QProtobufMessage::operator=(other);
    dptr = other.dptr;
    return *this;
}
FieldIndexTest2Message::FieldIndexTest2Message(FieldIndexTest2Message &&other) noexcept
    : QProtobufMessage(std::move(other)),
      dptr(std::move(other.dptr))
{
}
FieldIndexTest2Message &FieldIndexTest2Message::operator =(FieldIndexTest2Message &&other) noexcept
{
    QProtobufMessage::operator=(std::move(other));
    dptr.swap(other.dptr);
    return *this;
}
bool FieldIndexTest2Message::operator ==(const FieldIndexTest2Message &other) const
{
    return QProtobufMessage::isEqual(*this, other)
        && dptr->m_testField == other.dptr->m_testField;
}

bool FieldIndexTest2Message::operator !=(const FieldIndexTest2Message &other) const
{
    return !this->operator ==(other);
}

QtProtobuf::sint32 FieldIndexTest2Message::testField() const
{
    return dptr->m_testField;
}

void FieldIndexTest2Message::setTestField(const QtProtobuf::sint32 &testField)
{
    if (dptr->m_testField != testField) {
        dptr.detach();
        dptr->m_testField = testField;
    }
}


class FieldIndexTest3Message_QtProtobufData : public QSharedData
{
public:
    FieldIndexTest3Message_QtProtobufData()
        : QSharedData(),
          m_testField(0)
    {
    }

    FieldIndexTest3Message_QtProtobufData(const FieldIndexTest3Message_QtProtobufData &other)
        : QSharedData(other),
          m_testField(other.m_testField)
    {
    }

    QtProtobuf::sint32 m_testField;
};

FieldIndexTest3Message::~FieldIndexTest3Message() = default;

static constexpr struct {
    QtProtobufPrivate::QProtobufPropertyOrdering::Data data;
    const std::array<uint, 5> qt_protobuf_FieldIndexTest3Message_uint_data;
    const char qt_protobuf_FieldIndexTest3Message_char_data[60];
} qt_protobuf_FieldIndexTest3Message_metadata {
    // data
    {
        0, /* = version */
        1, /* = num fields */
        2, /* = field number offset */
        3, /* = property index offset */
        4, /* = field flags offset */
        48, /* = message full name length */
    },
    // uint_data
    {
        // JSON name offsets:
        49, /* = testField */
        59, /* = end-of-string-marker */
        // Field numbers:
        2097151, /* = testField */
        // Property indices:
        0, /* = testField */
        // Field flags:
        QtProtobufPrivate::NoFlags, /* = testField */
    },
    // char_data
    /* metadata char_data: */
    "qtprotobufnamespace.tests.FieldIndexTest3Message\0" /* = full message name */
    /* field char_data: */
    "testField\0"
};

const QtProtobufPrivate::QProtobufPropertyOrdering FieldIndexTest3Message::propertyOrdering = {
    &qt_protobuf_FieldIndexTest3Message_metadata.data
};

void FieldIndexTest3Message::registerTypes()
{
    qRegisterMetaType<FieldIndexTest3Message>();
    qRegisterMetaType<FieldIndexTest3MessageRepeated>();
}

FieldIndexTest3Message::FieldIndexTest3Message()
    : QProtobufMessage(&FieldIndexTest3Message::staticMetaObject),
      dptr(new FieldIndexTest3Message_QtProtobufData)
{
}

FieldIndexTest3Message::FieldIndexTest3Message(const FieldIndexTest3Message &other)
    : QProtobufMessage(other),
      dptr(other.dptr)
{
}
FieldIndexTest3Message &FieldIndexTest3Message::operator =(const FieldIndexTest3Message &other)
{
    QProtobufMessage::operator=(other);
    dptr = other.dptr;
    return *this;
}
FieldIndexTest3Message::FieldIndexTest3Message(FieldIndexTest3Message &&other) noexcept
    : QProtobufMessage(std::move(other)),
      dptr(std::move(other.dptr))
{
}
FieldIndexTest3Message &FieldIndexTest3Message::operator =(FieldIndexTest3Message &&other) noexcept
{
    QProtobufMessage::operator=(std::move(other));
    dptr.swap(other.dptr);
    return *this;
}
bool FieldIndexTest3Message::operator ==(const FieldIndexTest3Message &other) const
{
    return QProtobufMessage::isEqual(*this, other)
        && dptr->m_testField == other.dptr->m_testField;
}

bool FieldIndexTest3Message::operator !=(const FieldIndexTest3Message &other) const
{
    return !this->operator ==(other);
}

QtProtobuf::sint32 FieldIndexTest3Message::testField() const
{
    return dptr->m_testField;
}

void FieldIndexTest3Message::setTestField(const QtProtobuf::sint32 &testField)
{
    if (dptr->m_testField != testField) {
        dptr.detach();
        dptr->m_testField = testField;
    }
}


class FieldIndexTest4Message_QtProtobufData : public QSharedData
{
public:
    FieldIndexTest4Message_QtProtobufData()
        : QSharedData(),
          m_testField(0)
    {
    }

    FieldIndexTest4Message_QtProtobufData(const FieldIndexTest4Message_QtProtobufData &other)
        : QSharedData(other),
          m_testField(other.m_testField)
    {
    }

    QtProtobuf::sint32 m_testField;
};

FieldIndexTest4Message::~FieldIndexTest4Message() = default;

static constexpr struct {
    QtProtobufPrivate::QProtobufPropertyOrdering::Data data;
    const std::array<uint, 5> qt_protobuf_FieldIndexTest4Message_uint_data;
    const char qt_protobuf_FieldIndexTest4Message_char_data[60];
} qt_protobuf_FieldIndexTest4Message_metadata {
    // data
    {
        0, /* = version */
        1, /* = num fields */
        2, /* = field number offset */
        3, /* = property index offset */
        4, /* = field flags offset */
        48, /* = message full name length */
    },
    // uint_data
    {
        // JSON name offsets:
        49, /* = testField */
        59, /* = end-of-string-marker */
        // Field numbers:
        536870911, /* = testField */
        // Property indices:
        0, /* = testField */
        // Field flags:
        QtProtobufPrivate::NoFlags, /* = testField */
    },
    // char_data
    /* metadata char_data: */
    "qtprotobufnamespace.tests.FieldIndexTest4Message\0" /* = full message name */
    /* field char_data: */
    "testField\0"
};

const QtProtobufPrivate::QProtobufPropertyOrdering FieldIndexTest4Message::propertyOrdering = {
    &qt_protobuf_FieldIndexTest4Message_metadata.data
};

void FieldIndexTest4Message::registerTypes()
{
    qRegisterMetaType<FieldIndexTest4Message>();
    qRegisterMetaType<FieldIndexTest4MessageRepeated>();
}

FieldIndexTest4Message::FieldIndexTest4Message()
    : QProtobufMessage(&FieldIndexTest4Message::staticMetaObject),
      dptr(new FieldIndexTest4Message_QtProtobufData)
{
}

FieldIndexTest4Message::FieldIndexTest4Message(const FieldIndexTest4Message &other)
    : QProtobufMessage(other),
      dptr(other.dptr)
{
}
FieldIndexTest4Message &FieldIndexTest4Message::operator =(const FieldIndexTest4Message &other)
{
    QProtobufMessage::operator=(other);
    dptr = other.dptr;
    return *this;
}
FieldIndexTest4Message::FieldIndexTest4Message(FieldIndexTest4Message &&other) noexcept
    : QProtobufMessage(std::move(other)),
      dptr(std::move(other.dptr))
{
}
FieldIndexTest4Message &FieldIndexTest4Message::operator =(FieldIndexTest4Message &&other) noexcept
{
    QProtobufMessage::operator=(std::move(other));
    dptr.swap(other.dptr);
    return *this;
}
bool FieldIndexTest4Message::operator ==(const FieldIndexTest4Message &other) const
{
    return QProtobufMessage::isEqual(*this, other)
        && dptr->m_testField == other.dptr->m_testField;
}

bool FieldIndexTest4Message::operator !=(const FieldIndexTest4Message &other) const
{
    return !this->operator ==(other);
}

QtProtobuf::sint32 FieldIndexTest4Message::testField() const
{
    return dptr->m_testField;
}

void FieldIndexTest4Message::setTestField(const QtProtobuf::sint32 &testField)
{
    if (dptr->m_testField != testField) {
        dptr.detach();
        dptr->m_testField = testField;
    }
}

} // namespace qtprotobufnamespace::tests

#include "moc_fieldindexrange.qpb.cpp"
