/*
   modified: maximilian@glumb.de
   original:

   Arduino Buffered Serial
   A library that helps establish buffered serial communication with a
   host application.
   Copyright (C) 2010 Sigurður Örn Aðalgeirsson (siggi@media.mit.edu)
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


 #include "RingBuffer.h"

const unsigned char RingBuffer::END         = '!';
const unsigned char RingBuffer::EMPTY       = '$';
const unsigned int  RingBuffer::STATUS_OK   = 1;
const unsigned int  RingBuffer::STATUS_FULL = 0;


RingBuffer::RingBuffer(unsigned int buf_size) {
    data     = (char *)malloc(sizeof(char) * buf_size);
    capacity = buf_size;
    position = 0;
    length   = 0;
}

void RingBuffer::deAllocate() {
    free(data);
}

void RingBuffer::clear() {
    position = 0;
    length   = 0;
}

int RingBuffer::getSize() {
    return length;
}

int RingBuffer::getCapacity() {
    return capacity;
}

char RingBuffer::peek(unsigned int index) {
    char b = data[(position + index) % capacity];

    return b;
}

void RingBuffer::put(char in) {
    // if (length < capacity) {
    // save data char at end of buffer
    data[(position + length) % capacity] = in;

    // increment the length
    length++;

    // return 1;
    // }
    // Serial.print("put char ");
    // Serial.print(in);
    // return failure
    // return 0;
}

int RingBuffer::putBytes(char *chars, unsigned int charsLength) {
    if (length + charsLength < capacity) {
        for (size_t i = 0; i < charsLength; i++) {
            this->put(chars[i]);
        }
        this->put(RingBuffer::END);
        return RingBuffer::STATUS_OK;
    }

    // return failure
    return RingBuffer::STATUS_FULL;
}

void RingBuffer::putInFront(char in) {
    // if (length < capacity) {
    // save data char at end of buffer
    if (position == 0) position = capacity - 1;
    else position = (position - 1) % capacity;
    data[position] = in;

    // increment the length
    length++;

    // return 1;
    // }

    // return failure
    // return 0;
}

int RingBuffer::putBytesInFront(char *chars, unsigned int charsLength) {
    if (length + charsLength < capacity) {
        this->putInFront(RingBuffer::END);

        unsigned int i = charsLength - 1;

        while (i--) // reverse iterate
        {
            this->putInFront(chars[i]);
        }

        return RingBuffer::STATUS_OK;
    }

    // return failure
    return RingBuffer::STATUS_FULL;
}

char RingBuffer::get() {
    char b = RingBuffer::END;


    if (length > 0) {
        b = data[position];

        // move index down and decrement length
        position = (position + 1) % capacity;
        length--;
    }

    return b;
}

unsigned int RingBuffer::getMessage(char message[]) {
    char b;

    unsigned int length = 0;

    do {
        b               = this->getByte();
        message[length] = b;
        length++;
    } while (b != RingBuffer::END);

    return --length;
}

char RingBuffer::getByte() {
    char b = RingBuffer::END;


    if (length > 0) {
        b = data[position];

        // move index down and decrement length
        position = (position + 1) % capacity;
        length--;
    }

    return b;
}

char RingBuffer::getFromBack() {
    char b = 0;

    if (length > 0) {
        b = data[(position + length - 1) % capacity];
        length--;
    }

    return b;
}
