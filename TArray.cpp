#include "TArray.h"

void cArrayException::ThrowIndexOutOfBound(int index, int maxSize)
{
    Throw<cArrayException>("Array index %d out of bound (%d)", index, maxSize );
}

void cArrayException::ThrowArrayToSmall(int requestedSize, int maxSize)
{
    Throw<cArrayException>("Array to small to support %d items (max. = %d)", requestedSize, maxSize );
}

void cArrayException::ThrowNotEmpty()
{
    Throw<cArrayException>("Array is not empty" );
}
