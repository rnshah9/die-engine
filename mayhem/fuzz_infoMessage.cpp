#include <stdint.h>
#include <stdio.h>
#include <climits>

#include <FuzzedDataProvider.h>
#include <QObject>
#include "consoleoutput.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    FuzzedDataProvider provider(data, size);

    std::string str = provider.ConsumeRandomLengthString();
    QString qstr = QString::fromStdString(str);
    ConsoleOutput co(nullptr);
    co.infoMessage(qstr);

    return 0;
}