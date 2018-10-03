//
// Created by wj_de on 18-9-28.
//

#ifndef NET_BUFFER_H
#define NET_BUFFER_H

#include <vector>
#include "StringPiece.h"
namespace ZL{
namespace Net{
class Buffer {

    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;
public:
    explicit Buffer(size_t initialSize = kInitialSize);
    void swap(Buffer& rhs);

    size_t readableBytes() const;

    size_t writableBytes() const;

    size_t prependableBytes() const;

    const char* peek() const;

    const char* findCRLF() const;

    const char* findCRLF(const char* start) const;

    const char* findEOL() const;

    const char* findEOL(const char* start) const;

    void retrieve(size_t len);

    void retrieveUntil(const char* end);

    void retrieveInt64();

    void retrieveInt32();

    void retrieveInt16();

    void retrieveInt8();

    void retrieveAll();

    string retrieveAllAsString();

    string retrieveAsString(size_t len);

    StringPiece toStringPiece() const;

    void append(const StringPiece& str);

    void append(const char* /*restrict*/ data, size_t len);

    void append(const void* /*restrict*/ data, size_t len);

    void ensureWritableBytes(size_t len);

    char* beginWrite();

    const char* beginWrite() const;

    void hasWritten(size_t len);

    void unwrite(size_t len);

    void appendInt64(int64_t x);


    void appendInt32(int32_t x);

    void appendInt16(int16_t x);

    void appendInt8(int8_t x);

    int64_t readInt64();

    int32_t readInt32();

    int16_t readInt16();

    int8_t readInt8();

    int64_t peekInt64() const;

    int32_t peekInt32() const;

    int16_t peekInt16() const;

    int8_t peekInt8() const;

    void prependInt64(int64_t x);

    void prependInt32(int32_t x);

    void prependInt16(int16_t x);

    void prependInt8(int8_t x);

    void prepend(const void* /*restrict*/ data, size_t len);

    void shrink(size_t reserve);


    ssize_t readFd(int fd, int* savedErrno);





private:
    char* begin();

    const char* begin() const;

    void makeSpace(size_t len);

    std::vector<char> bufff_;
    int readindex_;
    int writeindex_;
};
}
}


#endif //NET_BUFFER_H
